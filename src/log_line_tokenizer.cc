/*
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

#include <assert.h>
#include <sstream>
#include <iostream>

#include "string_literal.h"
#include "log_line_tokenizer.h"

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

	ss << "LogLineTokenizer{";
	ss << "prefixMatcher=" << *prefixMatcher << "\n";
	ss << "tokenMatchers(\n";
	int index{0};
	for(auto tokenMatcher : tokenMatchers) {
		ss << index << ":" << *tokenMatcher << "\n";
		++index;
	}
	ss << ")}";
	return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const LogLineTokenizer& tokenizer) {
	return stream << tokenizer.toString();
}

