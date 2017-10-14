/*
 *  Created on: Nov 27, 2015
 *      Author: Tomas Szabo
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <sstream>
#include <iostream>

#include "token_matcher.h"
#include "string_literal.h"

using namespace std;

TokenMatcher::TokenMatcher(const TokenMatcherSettings& settings) : name{settings.name}, combine{settings.combine}, separator{settings.separator} {
	numMatches = findNumberOfMatches(settings.pattern);
	assert(numMatches > 0);

	matches = new re2::StringPiece[numMatches];
	argv = new RE2::Arg*[numMatches];
	pattern = new RE2{ settings.pattern };

	assert(pattern->ok());
	assert(numMatches == pattern->NumberOfCapturingGroups());

	for(int i{0}; i < numMatches; ++i) {
		argv[i] = new RE2::Arg();
		*argv[i] = &matches[i];
	}
}

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


std::string TokenMatcher::toString() const {
	std::stringstream ss;
	ss << name;
	ss << "<" << pattern->NumberOfCapturingGroups();
	if(combine) {
		ss << "," << separator;
	}
	ss << ">=" << pattern->pattern();
	return ss.str();


	ss << "TokenMatcher{";
	ss << "pattern=" << pattern->pattern();
	ss << ",combine=" << combine;
	ss << ",separator=" << separator;
	ss << "}";
	return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const TokenMatcher& tokenMatcher) {
	return stream << tokenMatcher.toString();
}
