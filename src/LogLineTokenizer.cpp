/*
 * LogLineTokenizer.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: jolt
 */

#include <LogLineTokenizer.h>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <StringLiteral.h>

using namespace std;
using namespace re2;

LogLineTokenizer::LogLineTokenizer(std::string _name, TokenMatcherSettings& prefix, std::vector<TokenMatcherSettings>& tokenPatterns) : name{_name}, numTokens{0} {
	prefixMatcher = new TokenMatcher{prefix};
	numTokens += prefixMatcher->getNumTokens();

	for(auto pattern : tokenPatterns) {
		TokenMatcher* tokenMatcher = new TokenMatcher{pattern};
		tokenMatchers.push_back(tokenMatcher);
		numTokens += tokenMatcher->getNumTokens();
	}
}

LogLineTokenizer::~LogLineTokenizer() {
	delete prefixMatcher;

	for(auto tokenMatcher : tokenMatchers) {
		delete tokenMatcher;
	}
}

int LogLineTokenizer::getNumTokens() const {
	return numTokens;
}

bool LogLineTokenizer::tokenizeLine(const re2::StringPiece line, std::string** tokens) const {

	StringPiece lineContents{line};
	bool result{true};
	int tokenIndex{0};

	result = prefixMatcher->consume(lineContents,&(tokens[tokenIndex]));
	tokenIndex += prefixMatcher->getNumTokens();
	for(auto tokenMatcher : tokenMatchers) {
		tokenMatcher->match(lineContents,&(tokens[tokenIndex]));
		tokenIndex += tokenMatcher->getNumTokens();
	}
	return result;
}


//bool LogLineTokenizer::tokenizeLine(re2::StringPiece line, re2::StringPiece res[]) const {
//	bool result = false;
//	re2::StringPiece s;
//
//	result = RE2::FullMatchN(line,*pattern,argv,argc);
//
//	unsigned int matchIdx{0};
//	for(unsigned int i{0}; i < NUM_TOKENS; ++i) {
//		if(matchMapping[i] != -1) {
//			res[i] = matches[matchIdx];
//			matchIdx++;
//		}
//		else res[i].clear();
//	}
//
//	return result;
//}

std::string LogLineTokenizer::toString() const {
	stringstream ss;
//
//	ss << ">>name" << "," << pattern->pattern() << ",";
//	for(auto match : matchMapping) {
//		ss << match << ",";
//	}
	return ss.str();
}
