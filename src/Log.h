/*
 * Log.h
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <vector>
#include <utility>
#include <climits>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

class Log {
public:
	Log();
	virtual ~Log();

	bool map(std::string fileName);
	bool unmap();

	size_t getNumLines() const;

	std::string getLine(size_t index) const;
	std::string getLine(size_t index, size_t maxLen, size_t lineOffset = 0) const;

	std::string toString() const;

protected:
	void scanForLines(size_t index) const;

	char *fileStart;
	char* fileEnd;
	mutable size_t numLines;

	mutable std::vector< std::pair<char*,char*> > lines;
};

#endif /* LOG_H_ */
