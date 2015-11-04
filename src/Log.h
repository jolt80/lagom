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
#include <climits>
#include <StringLiteral.h>
#include <string>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

namespace FormatMask {
	static const uint32_t line{0b1};
	static const uint32_t timeAndDate{0b10};
	static const uint32_t timediff{0b100};
	static const uint32_t level{0b1000};
	static const uint32_t cpu{0b10000};
	static const uint32_t thread{0b100000};
	static const uint32_t traceobj{0b1000000};
	static const uint32_t fileandline{0b10000000};
	static const uint32_t msg{0b100000000};
}

class Log {
public:
	Log();
	virtual ~Log();

	uint32_t format = UINT_MAX;

	bool map(std::string fileName);
	bool unmap();

	size_t getNumLines() const;

	size_t searchForLineContaining(size_t startLine, std::string search) const;

	std::string getLine(size_t index) const;
	std::string getLine(size_t index, size_t maxLen, size_t lineOffset = 0) const;
	std::string getFormattedLine(size_t index, size_t maxLen, size_t lineOffset = 0) const;

	std::string toString() const;

protected:
	void scanForLines(size_t index) const;
	StringLiteral lineAt(size_t index) const;

	char *fileStart;
	char* fileEnd;
	mutable size_t numLines;

	mutable std::vector< StringLiteral > lines;
};

#endif /* LOG_H_ */
