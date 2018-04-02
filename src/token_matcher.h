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

#ifndef TOKENMATCHER_H_
#define TOKENMATCHER_H_

#include <re2/re2.h>
#include <re2/stringpiece.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace lagom {

struct TokenMatcherSettings {
  TokenMatcherSettings() = default;
  TokenMatcherSettings(const std::string& _name,
                       int _numberOfMatches,
                       const std::string& _pattern,
                       bool _combine,
                       const std::string& _separator)
      : name{_name}
      , numberOfTokens(_numberOfMatches)
      , pattern{_pattern}
      , combine{_combine}
      , separator{_separator} {};
  TokenMatcherSettings(const std::string& _name, int _numberOfMatches, const std::string& _pattern)
      : name{_name}
      , numberOfTokens(_numberOfMatches)
      , pattern{_pattern}
      , combine{false}
      , separator{} {};
  TokenMatcherSettings(const std::string& _name, const std::string& _pattern)
      : name{_name}
      , numberOfTokens(1)
      , pattern{_pattern}
      , combine{false}
      , separator{} {};

  TokenMatcherSettings(const TokenMatcherSettings&) = default;

  TokenMatcherSettings& operator=(const TokenMatcherSettings&) = default;

  bool isInitialized() const { return !pattern.empty(); }

  std::string name;
  int numberOfTokens{0};
  std::string pattern;
  bool combine{false};
  std::string separator;
};

/**
 * Will find numMatches regex matches.
 *
 * If numMatches > numTokens and numTokens == 1:
 * Matches will be combined into one resulting token, separated by
 * separator.
 */
class TokenMatcher {
 public:
  TokenMatcher(const TokenMatcherSettings& settings);
  virtual ~TokenMatcher();

  int getNumMatches() const;
  int getNumTokens() const;
  std::string getPattern() const;

  bool match(const re2::StringPiece& input, std::string** result) const;
  bool consume(re2::StringPiece& input, std::string** result) const;
  bool findAndConsume(re2::StringPiece& input, std::string** result) const;

  bool operator!=(const TokenMatcher& other) const { return !(*this == other); }
  bool operator==(const TokenMatcher& other) const {
    if (name != other.name) {
      std::cout << "name" << std::endl;
      return false;
    }

    if (pattern->pattern() != other.pattern->pattern()) {
      std::cout << "pattern not matching" << std::endl;
      std::cout << "1: " << pattern->pattern() << std::endl;
      std::cout << "1: " << other.pattern->pattern() << std::endl;
      return false;
    }
    if (combine != other.combine) {
      std::cout << "combine not matching" << std::endl;
      return false;
    }
    if (separator != other.separator) {
      std::cout << "separator not matching" << std::endl;
      return false;
    }
    return true;
  }
  std::string toString() const;

  // Simple version
  std::string getMatch(re2::StringPiece str) const;

  static int findNumberOfMatches(std::string pattern);

 private:
  std::string name;
  bool combine = false;
  int numMatches = 0;

  std::string separator = ":";

  re2::StringPiece* matches = nullptr;
  RE2::Arg** argv = nullptr;
  RE2* pattern = nullptr;

  inline void finalizeResult(std::string** result, bool ok) const {
    if (ok) {
      if (!combine) {
        for (int i{0}; i < numMatches; ++i) {
          const re2::StringPiece match = matches[i];
          result[i]->assign(match.data(), match.length());
        }
      }
      else {
        for (int i{0}; i < numMatches; ++i) {
          const re2::StringPiece match = matches[i];
          result[0]->append(match.data(), match.length());
          if (i != numMatches - 1) result[0]->append(separator);
        }
      }
    }
    else {
      for (int i{0}; i < numMatches; ++i) {
        result[i]->clear();
      }
    }
  };
};

std::ostream& operator<<(std::ostream& stream, const TokenMatcher& tokenMatcher);

} // namespace lagom

#endif /* TOKENMATCHER_H_ */
