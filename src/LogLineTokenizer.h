/*
 * LogLineTokenizer.h
 *
 *  Created on: Nov 17, 2015
 *      Author: jolt
 */

#ifndef LOGLINETOKENIZER_H_
#define LOGLINETOKENIZER_H_

#include <re2/re2.h>
#include <vector>
#include <array>
#include <string>

#define NUM_TOKENS 9

/**
 * Wraps an RE2 regex and provides matching for which log token index
 * each regex match represents.
 *
 * This class will expect to be able to perform a FullMatch for a log line
 * or a specific format.
 */
class LogLineTokenizer {
public:
	LogLineTokenizer(std::string _name, std::vector<std::pair<int,std::string>> patternInit);
	virtual ~LogLineTokenizer();

	bool tokenizeLine(re2::StringPiece line, re2::StringPiece res[]) const;

	std::string toString() const;
private:
	std::string name;
	RE2* pattern;

	std::array<re2::StringPiece,NUM_TOKENS> matches;
	RE2::Arg* argv[NUM_TOKENS];
	int argc;

	std::array<int,NUM_TOKENS> matchMapping;
};

#endif /* LOGLINETOKENIZER_H_ */
