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

#ifndef LOGLINETOKENIZER_H_
#define LOGLINETOKENIZER_H_

#include <array>
#include <string>
#include <vector>
#include "token_matcher.h"

namespace lagom {

/**
 * Wraps an RE2 regex and provides matching for which log token index
 * each regex match represents.
 *
 * This class will be expected to match a whole line with a FullMatch for a log line
 * of a specific format.
 */
class LogLineTokenizer {
 public:
  LogLineTokenizer(std::string _name, TokenMatcherSettings& prefix, std::vector<TokenMatcherSettings>& tokenPatterns);
  virtual ~LogLineTokenizer();

  bool tokenizeLine(const re2::StringPiece line, std::string** tokens) const;

  int getNumTokens() const;

  bool operator!=(const LogLineTokenizer& other) const { return !(*this == other); }

  bool operator==(const LogLineTokenizer& other) const {
    if (*prefixMatcher != *other.prefixMatcher) return false;
    if (tokenMatchers.size() != other.tokenMatchers.size()) return false;
    for (unsigned int i{0}; i < tokenMatchers.size(); ++i) {
      if (*tokenMatchers[i] != *other.tokenMatchers[i]) return false;
    }
    return true;
  }
  std::string toString() const;

 private:
  std::string name;
  int numTokens;
  TokenMatcher* prefixMatcher;

  std::vector<TokenMatcher*> tokenMatchers;
};

std::ostream& operator<<(std::ostream& stream, const LogLineTokenizer& tokenizer);

} // namespace lagom

#endif /* LOGLINETOKENIZER_H_ */
