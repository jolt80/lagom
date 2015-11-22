/*
 * Log.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#include <AutoLock.h>
#include <Log.h>

#include <sstream>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <Logger.h>
#include <AutoLock.h>

#include <re2/re2.h>
#include <re2/stringpiece.h>

using namespace std;
using namespace std::chrono;


extern Logger logger;

Log::Log(Settings& _settings) :
		numLines{INT_MAX}, settings{_settings}, fileStart{nullptr}, fileEnd{nullptr} {
}

Log::~Log() {
}

bool Log::areLineNumbersParsed() const {
	return INT_MAX != numLines;
}

int Log::getNumLines() const {
	return numLines;
}

bool Log::map(std::string fileName) {
	AutoLock lock(mutex);

	struct stat sb;
	int fd;

	fd = ::open (fileName.c_str(), O_RDONLY);
	if (fd == -1) {
		::perror ("open");
		return false;
	}

	if (::fstat(fd, &sb) == -1) {
		::perror ("fstat");
		return false;
	}

	if (!S_ISREG (sb.st_mode)) {
		::fprintf (stderr, "%s is not a file\n", fileName.c_str());
		return false;
	}

	fileStart = (char*) ::mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	fileEnd = fileStart + sb.st_size;

	if (fileStart == MAP_FAILED) {
		::perror ("mmap");
		return false;
	}

	if (close (fd) == -1) {
		::perror ("close");
		return false;
	}

	// Scan first line to seed the lines vector
	char* lineStart = fileStart;

	char* lineEnd = (char*)::memchr(lineStart,'\n',fileEnd - lineStart);
	lines.push_back(StringLiteral{lineStart,lineEnd});

	return true;
}

bool Log::unmap() {
	AutoLock lock(mutex);

	if (::munmap (fileStart, fileEnd - fileStart) == -1) {
		::perror ("munmap");
		return false;
	}
	return true;
}

std::string Log::getLine(int index) const {
	return lineAt(index).toString();
}

StringLiteral Log::getLine(int index, int maxLen, int lineOffset) const {
	StringLiteral line = lineAt(index);
	if(line.trimFromStart(lineOffset)) {
		return line;
	}
	return StringLiteral{};
}

StringLiteral Log::lineAt(int index) const {
	AutoLock lock(mutex);

	if(index < numLines) {
		if(static_cast<int>(lines.size()) <= index) {
			if(index <= (INT_MAX - 100))
				scanForLines(index+100);
			else
				scanForLines(INT_MAX);
		}

		if(index < numLines) {
			if(index < 0 || index > lines.size()) {
				char* crash = 0;
				cout << crash;
			}

			return lines.at(index);
		}
	}
	return StringLiteral{};
}

bool Log::getTriLogTokens(int index, re2::StringPiece s[]) const {
	// Expect to get 9 matches for a TRI log
	StringLiteral line = lineAt(index);

	for(auto tokenizer : settings.getTokenizers()) {
		if(tokenizer->tokenizeLine(line.toStringPiece(),s)) return true;
	}

	return false;
}

void Log::scanForLines(int index, long maxDuration) const {
	AutoLock lock(mutex);
	int lastScannedLine = lines.size();

	// find first line
	if(lastScannedLine == 0) {
		char* lineStart = fileStart;

		int maxLength = fileEnd - lineStart;
		char* lineEnd = (char*)::memchr(lineStart,'\n',maxLength);
		if(NULL == lineEnd) {
			lineEnd = fileEnd;
			// Found end of file
			numLines = lastScannedLine + 1;
		}
		lines.push_back(StringLiteral{lineStart,lineEnd});
	}
	else {
		lastScannedLine--;
	}
	int startLine = lastScannedLine;
	auto start = high_resolution_clock::now();

	for(; lastScannedLine <= index && lines.at(lastScannedLine).getStrEnd() < fileEnd; ++lastScannedLine) {
		// second point to '\n' at the end of the line
		char* search = const_cast<char*>(lines.at(lastScannedLine).getStrEnd());

		char* lineStart = search + 1;

		int maxLength = fileEnd - lineStart;
		char* lineEnd = (char*)::memchr(lineStart,'\n',maxLength);
		if(NULL == lineEnd) {
			lineEnd = fileEnd;
			// Found end of file
			numLines = lastScannedLine + 1;
		}
		lines.push_back(StringLiteral{lineStart,lineEnd});

		if(lastScannedLine % 1000 == 0) {
			auto end = high_resolution_clock::now();
			auto duration = end - start;
			if(duration_cast<microseconds>(duration).count() > maxDuration) break;
		}
	}

	auto end = high_resolution_clock::now();
	auto duration = end - start;

	logger << "scanned " << lastScannedLine - startLine << " in " << duration_cast<microseconds>(duration).count() << " us\n";
}

int Log::searchForLineContaining(int startLine, std::string search) const {
	int lineIndex = startLine;

	while(!(lineAt(lineIndex).contains(search))) {
		lineIndex++;
		if(lineIndex >= numLines) return numLines;
	}
	return lineIndex;
}

std::string Log::toString() const {
	stringstream ret;

	ret << "fileStart = ";
	ret << hex << "0x" << to_string((intptr_t)fileStart);
	ret << ", fileEnd = ";
	ret << hex << "0x" << to_string((intptr_t)fileEnd);
	ret << "size = " << dec << (intptr_t)((fileEnd - fileStart));

	return ret.str();
}

