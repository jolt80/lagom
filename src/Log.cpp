/*
 * Log.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#include <Log.h>

#include <cstring>

using namespace std;


Log::Log() {
	// TODO Auto-generated constructor stub

}

Log::~Log() {
	// TODO Auto-generated destructor stub
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
	lines.push_back(make_pair(lineStart,lineEnd));

	return true;
}

bool Log::unmap() {
	if (::munmap (fileStart, fileEnd - fileStart) == -1) {
		perror ("munmap");
		return false;
	}
	return true;
}

std::string Log::getLine(size_t index) {
	size_t scannedLines = lines.size();
	if(lines.size() <= index) {
		scanForLines(index);
		scannedLines = lines.size();
	}

	if(index < scannedLines) {
		pair<char*,char*> line = lines.at(index);
		return string(line.first,line.second - line.first + 1);
	}
	return "";
}

void Log::scanForLines(size_t index) {
	size_t lastScannedLine = lines.size() - 1;

	if(lines.at(lastScannedLine).second != fileEnd) {
		for(; lastScannedLine <= index; ++lastScannedLine) {
			// second point to '\n' at the end of the line
			char* search = lines.at(lastScannedLine).second;

			char* lineStart = search + 1;

			size_t maxLength = fileEnd - lineStart;
			char* lineEnd = (char*)::memchr(lineStart,'\n',maxLength);
			if(NULL == lineEnd) {
				lineEnd = fileEnd;
			}
			lines.push_back(make_pair(lineStart,lineEnd));
		}
	}
}

