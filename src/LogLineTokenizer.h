/*
 * LogLineTokenizer.h
 *
 *  Created on: Nov 17, 2015
 *      Author: jolt
 */

#ifndef LOGLINETOKENIZER_H_
#define LOGLINETOKENIZER_H_

#include <TokenMatcher.h>
#include <vector>
#include <array>
#include <string>

/**
 * Wraps an RE2 regex and provides matching for which log token index
 * each regex match represents.
 *
 * This class will be expected to match a whole expect to be able to perform a FullMatch for a log line
 * or a specific format.
 */
class LogLineTokenizer {
public:
	LogLineTokenizer(std::string _name, std::string prefixPattern, std::vector<std::string> tokenPatterns);
	virtual ~LogLineTokenizer();

	bool tokenizeLine(const re2::StringPiece line, std::string** tokens) const;

	int getNumTokens() const;

	std::string toString() const;
private:
	std::string name;
	int numTokens;
	TokenMatcher* prefixMatcher;

	std::vector<TokenMatcher*> tokenMatchers;
};

#endif /* LOGLINETOKENIZER_H_ */
