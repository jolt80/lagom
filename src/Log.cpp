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
	for(auto line : lines) delete line;

}

bool Log::areLineNumbersParsed() const {
	return INT_MAX != numLines;
}

bool Log::areAllLinesTokenized() {
	return lineAt(numLines-1).tokens != nullptr;
}

int Log::getNumLines() const {
	return numLines;
}

bool Log::map(std::string fileName) {
	//cout << __PRETTY_FUNCTION__ << endl;
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
	lines.push_back(new Line{lineStart,lineEnd});

	return true;
}

bool Log::unmap() {
	//cout << __PRETTY_FUNCTION__ << endl;
	AutoLock lock(mutex);

	if (::munmap (fileStart, fileEnd - fileStart) == -1) {
		::perror ("munmap");
		return false;
	}
	return true;
}

re2::StringPiece Log::getLine(int index) {
	//cout<< __PRETTY_FUNCTION__ << endl;
	AutoLock lock(mutex);
	return lineAt(index).contents.toStringPiece();
}


Line& Log::lineAt(int index) {
	if(index < numLines) {
		if(static_cast<int>(lines.size()) <= index) {
			if(index <= (INT_MAX - 100))
				scanForLinesNotLocked(index+100);
			else
				scanForLinesNotLocked(INT_MAX);
		}

		if(index < numLines) {
			return *(lines.at(index));
		}
	}
	return *(lines.at(numLines-1));
}

std::string** Log::getLogTokens(int index) {
	//cout<< __PRETTY_FUNCTION__ << endl;
	AutoLock lock(mutex);
	Line& line = lineAt(index);
	if(!line.tokenized) tokenizeLine(line);
	return line.tokens;
}

void Log::scanForLines(int index, long maxDuration) {
	//cout<< __PRETTY_FUNCTION__ << endl;
	AutoLock lock(mutex);
	scanForLinesNotLocked(index,maxDuration);
}

void Log::scanForLinesNotLocked(int index, long maxDuration) {
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
		lines.push_back(new Line{lineStart,lineEnd});
	}
	else {
		lastScannedLine--;
	}
	int startLine = lastScannedLine;
	auto start = high_resolution_clock::now();

	for(; lastScannedLine <= index && lines.at(lastScannedLine)->contents.getStrEnd() < fileEnd; ++lastScannedLine) {
		// second point to '\n' at the end of the line
		char* search = const_cast<char*>(lines.at(lastScannedLine)->contents.getStrEnd());

		char* lineStart = search + 1;

		int maxLength = fileEnd - lineStart;
		char* lineEnd = (char*)::memchr(lineStart,'\n',maxLength);
		if(NULL == lineEnd) {
			lineEnd = fileEnd;
			// Found end of file
			numLines = lastScannedLine + 1;
		}
		lines.push_back(new Line{lineStart,lineEnd});

		if(lastScannedLine % 100 == 0) {
			auto end = high_resolution_clock::now();
			auto duration = end - start;
			if(duration_cast<microseconds>(duration).count() > maxDuration) break;
		}
	}

	auto end = high_resolution_clock::now();
	auto duration = end - start;

	logger << "scanned " << lastScannedLine - startLine << " in " << duration_cast<microseconds>(duration).count() << " us\n";
}



int Log::tokenizeLines(int index, long maxDuration) {
	auto start = high_resolution_clock::now();
	auto end = start;
	long duration = duration_cast<microseconds>(end - start).count();

	int i{index};
	for(; i < numLines && duration < maxDuration; ++i) {
		AutoLock lock(mutex);
		Line& line = lineAt(i);
		if(line.tokens == nullptr)
			tokenizeLine(line);
		if((i % 10) == 0) {
			end = high_resolution_clock::now();
			duration = duration_cast<microseconds>(end - start).count();
		}
	}
	return i;
}

void Log::tokenizeLine(Line& line) {
	if(line.tokens == nullptr) {
		line.tokens = new std::string*[9];
		for(int i{0}; i < 9; ++i) {
			line.tokens[i] = new std::string();
		}
	}

	bool success = false;
	for(auto tokenizer : settings.getTokenizers()) {
		success = tokenizer->tokenizeLine(line.contents.toStringPiece(),line.tokens);
		if(success) break;
	}

	if(!success) {
		for(int i{0}; i < 9; ++i) {
			delete line.tokens[i];
		}
		delete[] line.tokens;
		line.tokens = nullptr;
	}
	line.tokenized = true;
}

int Log::searchForLineContaining(int startLine, std::string search) {
	//cout<< __PRETTY_FUNCTION__ << endl;
	AutoLock lock(mutex);
	int lineIndex = startLine;

	while(!(lineAt(lineIndex).contents.contains(search))) {
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

