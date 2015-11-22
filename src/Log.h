/*
 * Log.h
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#ifndef LOG_H_
#define LOG_H_

#include <Settings.h>
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
#include <chrono>
#include <mutex>

#include <re2/re2.h>

class Log {
public:
	Log(Settings& _settings);
	virtual ~Log();

	bool map(std::string fileName);
	bool unmap();

	bool areLineNumbersParsed() const;
	int getNumLines() const;

	int searchForLineContaining(int startLine, std::string search) const;

	std::string getLine(int index) const;
	StringLiteral getLine(int index, int maxLen, int lineOffset = 0) const;

	bool getTriLogTokens(int index, re2::StringPiece[]) const;

	std::string toString() const;

	StringLiteral lineAt(int index) const;
	void scanForLines(int index, long maxDuration = 2000000) const;

protected:

	mutable std::recursive_mutex mutex;

	mutable int numLines;
	Settings& settings;
	char *fileStart;
	char* fileEnd;

	mutable std::vector< StringLiteral > lines;
};

#endif /* LOG_H_ */
