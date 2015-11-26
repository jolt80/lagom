/*
 * TokenMatcher.h
 *
 *  Created on: Nov 27, 2015
 *      Author: jolt
 */

#ifndef TOKENMATCHER_H_
#define TOKENMATCHER_H_

#include <re2/re2.h>
#include <vector>
#include <array>
#include <string>
#include <re2/stringpiece.h>

/**
 * Will find numMatches regex matches.
 *
 * If numMatches > numTokens and numTokens == 1:
 * Matches will be combined into one resulting token, separated by
 * separator.
 */
class TokenMatcher {
public:
	TokenMatcher(std::string _pattern);
	TokenMatcher(std::string _pattern, bool _combine, std::string _separator);
	virtual ~TokenMatcher();

	int getNumMatches() const;
	int getNumTokens() const;
	std::string getPattern() const;

	bool match(const re2::StringPiece& input, std::string** result) const;
	bool consume(re2::StringPiece& input, std::string** result) const;
	bool findAndConsume(re2::StringPiece& input, std::string** result) const;

	// Simple version
	std::string getMatch(re2::StringPiece str) const;

	static int findNumberOfMatches(std::string pattern);

private:
	bool combine = false;
	int numMatches = 0;

	std::string separator = ":";

	re2::StringPiece *matches = nullptr;
	RE2::Arg **argv = nullptr;
	RE2* pattern = nullptr;

	inline void finalizeResult(std::string** result, bool ok) const {
		if(ok) {
			if(!combine)
			{
				for(int i{0}; i < numMatches; ++i) {
					const re2::StringPiece match = matches[i];
					result[i]->assign(match.data(),match.length());
				}
			}
			else {
				for(int i{0}; i < numMatches; ++i) {
					const re2::StringPiece match = matches[i];
					result[i]->append(match.data(),match.length());
					result[i]->append(separator);
				}
			}
		}
		else {
			for(int i{0}; i < numMatches; ++i) {
				result[i]->clear();
			}
		}
	};
};

#endif /* TOKENMATCHER_H_ */
