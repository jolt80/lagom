/*
 * Log.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#include <Log.h>

#include <sstream>
#include <cstring>
#include <cstdint>

#include <regex>

using namespace std;


Log::Log() :  numLines{UINT_MAX} {
}

Log::~Log() {
}

size_t Log::getNumLines() const {
	return numLines;
}

bool Log::map(std::string fileName) {
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

	fileStart = (char*) mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	fileEnd = fileStart + sb.st_size;

	if (fileStart == MAP_FAILED) {
		::perror ("mmap");
		return false;
	}

	if (close (fd) == -1) {
		perror ("close");
		return false;
	}

	// Scan first line to seed the lines vector
	char* lineStart = fileStart;

	char* lineEnd = (char*)::memchr(lineStart,'\n',fileEnd - lineStart);
	lines.push_back(StringLiteral{lineStart,lineEnd});

	return true;
}

bool Log::unmap() {
	if (::munmap (fileStart, fileEnd - fileStart) == -1) {
		perror ("munmap");
		return false;
	}
	return true;
}

std::string Log::getLine(size_t index) const {
	size_t scannedLines = lines.size();
	if(lines.size() <= index) {
		scanForLines(index);
		scannedLines = lines.size();
	}

	if(index < scannedLines) {
		return lines.at(index).toString();
	}
	return string();
}

std::string Log::getLine(size_t index, size_t maxLen, size_t lineOffset) const {
	size_t scannedLines = lines.size();
	if(lines.size() <= index) {
		scanForLines(index);
		scannedLines = lines.size();
	}

	if(index < scannedLines) {
		StringLiteral line = lines.at(index);
		if(line.trimFromStart(lineOffset)) {
			return line.toString();
		}
	}
	return string();
}

StringLiteral Log::lineAt(size_t index) const {
	if(index < numLines) {
		if(lines.size() <= index) {
			scanForLines(index+100);
		}

		if(index < numLines) {
			return lines.at(index);
		}
	}
	return StringLiteral{};
}

std::string Log::getFormattedLine(size_t index, size_t maxLen, size_t lineOffset) const {
	static const char* pad = "\033[1;31m|\033[0m";
	string line = lineAt(index).toString();
	string ret;

	cout << line << endl;

	if( (format | FormatMask::timeAndDate) != 0) {
		try {
		  std::regex re("(\\[\\d{4}-\\d{2}-\\d{2})\\s*(\\d{1,2}:\\d{1,2}:\\d{1,2}\\.\\d{3})");
		  std::smatch match;
		  std::regex_search(line, match, re);
		  if (match.size() > 1) {
		    ret.append(match.str(2));
		  } else {
			  // pad
			  ret.append(' ',12);
		  }
		} catch (std::regex_error& e) {
			return e.what();
		}

		ret.append(pad);
	}

	if( (format | FormatMask::timediff) != 0) {
		try {
		  std::regex re("\\((\\+\\d+\\.\\d+)\\)");
		  std::smatch match;
		  std::regex_search(line, match, re);
		  if (match.size() > 1) {
		    ret.append(match.str(1));
		  }
		} catch (std::regex_error& e) {
			return e.what();
		}

		ret.append(pad);
	}

	if( (format | FormatMask::level) != 0) {
		try {
		  std::regex re("du1 com_ericsson_triobjif:(.*?):");
		  std::smatch match;
		  std::regex_search(line, match, re);
		  if (match.size() > 1) {
		    ret.append(match.str(1));
		  }
		} catch (std::regex_error& e) {
			return e.what();
		}

		ret.append(pad);
	}

	if( (format | (FormatMask::traceobj | FormatMask::thread)) != 0) {
		try {
		  std::regex re("processAndObjIf = \"(.*?)\\((.*?)\\)\"");
		  std::smatch match;
		  std::regex_search(line, match, re);
		  if (match.size() > 1 && (format | (FormatMask::thread))) {
		    ret.append(match.str(1));
			ret.append(pad);
		  }
		  if (match.size() > 2 && (format | (FormatMask::traceobj))) {
		    ret.append(match.str(2));
			ret.append(pad);
		  }
		} catch (std::regex_error& e) {
			return e.what();
		}
	}

	if( (format | (FormatMask::traceobj | FormatMask::thread)) != 0) {
		try {
		  std::regex re("processAndObjIf = \"(.*?)\\((.*?)\\)\"");
		  std::smatch match;
		  std::regex_search(line, match, re);
		  if (match.size() > 1 && (format | (FormatMask::thread))) {
		    ret.append(match.str(1));
			ret.append(pad);
		  }
		  if (match.size() > 2 && (format | (FormatMask::traceobj))) {
		    ret.append(match.str(2));
			ret.append(pad);
		  }
		} catch (std::regex_error& e) {
			return e.what();
		}
	}

	try {
		std::regex re("msg = \"(.*)\"\\s+\\}");
		std::smatch match;
		std::regex_search(line, match, re);
		if (match.size() > 1) {
			ret.append(match.str(1));
		}
	} catch (std::regex_error& e) {
		return e.what();
	}

	return ret;
}

void Log::scanForLines(size_t index) const {
	size_t lastScannedLine = lines.size() - 1;

	for(; lastScannedLine <= index && lines.at(lastScannedLine).getStrEnd() < fileEnd; ++lastScannedLine) {
		// second point to '\n' at the end of the line
		char* search = const_cast<char*>(lines.at(lastScannedLine).getStrEnd());

		char* lineStart = search + 1;

		size_t maxLength = fileEnd - lineStart;
		char* lineEnd = (char*)::memchr(lineStart,'\n',maxLength);
		if(NULL == lineEnd) {
			lineEnd = fileEnd;
			// Found end of file
			numLines = lastScannedLine + 1;
		}
		lines.push_back(StringLiteral{lineStart,lineEnd});
	}
}

size_t Log::searchForLineContaining(size_t startLine, std::string search) const {
	size_t lineIndex = startLine;

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

