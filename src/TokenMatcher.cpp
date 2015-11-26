/*
 * TokenMatcher.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: jolt
 */

#include <TokenMatcher.h>
#include <StringLiteral.h>
#include <cassert>
#include <iostream>

using namespace std;

TokenMatcher::TokenMatcher(std::string _pattern, bool _combine, std::string _separator) : combine{_combine}, separator{_separator} {
	numMatches = findNumberOfMatches(_pattern);
	assert(numMatches > 0);

	matches = new re2::StringPiece[numMatches];
	argv = new RE2::Arg*[numMatches];
	pattern = new RE2{ _pattern };

	assert(pattern->ok());
	assert(numMatches == pattern->NumberOfCapturingGroups());

	for(int i{0}; i < numMatches; ++i) {
		argv[i] = new RE2::Arg();
		*argv[i] = &matches[i];
	}
}

TokenMatcher::TokenMatcher(std::string _pattern) : TokenMatcher{_pattern, false, ":"} {}


TokenMatcher::~TokenMatcher() {
	delete[] matches;
	for(int i{0}; i < numMatches; ++i) {
		delete argv[i];
	}
	delete[] argv;
	delete pattern;
}

int TokenMatcher::getNumMatches() const {
	return numMatches;
}

int TokenMatcher::getNumTokens() const {
	if(combine) return 1;
	else return numMatches;
}

std::string TokenMatcher::getPattern() const {
	return pattern->pattern();
}

bool TokenMatcher::match(const re2::StringPiece& input, std::string** result) const {
	bool ok = RE2::PartialMatchN(input,*pattern,argv,numMatches);

	finalizeResult(result,ok);

	return ok;
}

bool TokenMatcher::consume(re2::StringPiece& input, std::string** result) const {
	bool ok = RE2::ConsumeN(&input,*pattern,argv,numMatches);

	finalizeResult(result,ok);

	return ok;
}

bool TokenMatcher::findAndConsume(re2::StringPiece& input, std::string** result) const {
	bool ok = RE2::FindAndConsumeN(&input,*pattern,argv,numMatches);

	finalizeResult(result,ok);

	return ok;
}

std::string TokenMatcher::getMatch(re2::StringPiece str) const {
	cout << "matching " << pattern->pattern() << endl;
	std::string ret;

	bool result = RE2::PartialMatchN(str,*pattern,argv,numMatches);

	if(result) {
		const re2::StringPiece match = matches[0];
		ret.append(match.data(),match.length());
	}

	return ret;
}

int TokenMatcher::findNumberOfMatches(std::string pattern) {
	StringLiteral search{pattern};

	bool inMatch = false;
	int startParentheses{0};
	int endParentheses{0};
	int pos;

	do {
		if(inMatch) {
			pos = search.findFirstOf(')');
			if((pos == 0) || (pos < search.getLength() && pos > 0 && search[pos-1] != '\\')) ++endParentheses;
		}
		else {
			pos = search.findFirstOf('(');
			if((pos == 0) || (pos < search.getLength() && pos > 0 && search[pos-1] != '\\')) ++startParentheses;
		}

		inMatch = startParentheses > endParentheses;
		search.trimFromStart(pos+1);
		pos = 0;
	} while(pos < search.getLength());

	// Start and end parantheses don't add up
	if(startParentheses != endParentheses) return -1;

	return startParentheses;
}
