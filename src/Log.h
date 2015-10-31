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

	std::string getLine(size_t index);

protected:
	void scanForLines(size_t index);

	char *fileStart;
	char* fileEnd;

	std::vector< std::pair<char*,char*> > lines;
};

#endif /* LOG_H_ */
