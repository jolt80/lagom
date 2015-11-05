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

#include <re2/re2.h>

class Log {
public:
	Log(std::string triRegex);
	virtual ~Log();

	bool map(std::string fileName);
	bool unmap();

	int getNumLines() const;

	int searchForLineContaining(int startLine, std::string search) const;

	std::string getLine(int index) const;
	std::string getLine(int index, int maxLen, int lineOffset = 0) const;

	bool getTriLogTokens(int index, re2::StringPiece[]) const;

	std::string toString() const;

	StringLiteral lineAt(int index) const;

protected:
	RE2 TriLog;

	void scanForLines(int index) const;

	char *fileStart;
	char* fileEnd;
	mutable int numLines;

	mutable std::vector< StringLiteral > lines;
};

#endif /* LOG_H_ */
