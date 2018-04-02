/*
 * Copyright (c) 2018 Tomas Szabo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <assert.h>
#include <iostream>
#include <sstream>

#include "log_line_tokenizer.h"
#include "string_literal.h"

using namespace std;
using namespace re2;

namespace lagom {

LogLineTokenizer::LogLineTokenizer(std::string _name,
                                   TokenMatcherSettings& prefix,
                                   std::vector<TokenMatcherSettings>& tokenPatterns)
    : name{_name}
    , numTokens{0} {
  prefixMatcher = new TokenMatcher{prefix};
  numTokens += prefixMatcher->getNumTokens();

  for (auto pattern : tokenPatterns) {
    TokenMatcher* tokenMatcher = new TokenMatcher{pattern};
    tokenMatchers.push_back(tokenMatcher);
    numTokens += tokenMatcher->getNumTokens();
  }
}

LogLineTokenizer::~LogLineTokenizer() {
  delete prefixMatcher;

  for (auto tokenMatcher : tokenMatchers) {
    delete tokenMatcher;
  }
}

int LogLineTokenizer::getNumTokens() const { return numTokens; }

bool LogLineTokenizer::tokenizeLine(const re2::StringPiece line, std::string** tokens) const {

  StringPiece lineContents{line};
  bool result{true};
  int tokenIndex{0};

  result = prefixMatcher->consume(lineContents, &(tokens[tokenIndex]));
  tokenIndex += prefixMatcher->getNumTokens();
  for (auto tokenMatcher : tokenMatchers) {
    tokenMatcher->match(lineContents, &(tokens[tokenIndex]));
    tokenIndex += tokenMatcher->getNumTokens();
  }
  return result;
}

// bool LogLineTokenizer::tokenizeLine(re2::StringPiece line, re2::StringPiece res[]) const {
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
  for (auto tokenMatcher : tokenMatchers) {
    ss << index << ":" << *tokenMatcher << "\n";
    ++index;
  }
  ss << ")}";
  return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const LogLineTokenizer& tokenizer) {
  return stream << tokenizer.toString();
}

}  // namespace lagom
