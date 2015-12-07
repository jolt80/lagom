/*
 * TokenMatcher.h
 *
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

#ifndef TOKENMATCHER_H_
#define TOKENMATCHER_H_

#include <re2/re2.h>
#include <vector>
#include <array>
#include <string>
#include <re2/stringpiece.h>
#include <iostream>

struct TokenMatcherSettings {
	TokenMatcherSettings() =default;
	TokenMatcherSettings(const std::string& _name, const std::string& _pattern, bool _combine, const std::string& _separator) : name{_name}, pattern{_pattern}, combine{_combine}, separator{_separator} {};
	TokenMatcherSettings(const std::string& _name, const std::string& _pattern) : name{_name}, pattern{_pattern}, combine{false}, separator{} {};

	TokenMatcherSettings(const TokenMatcherSettings&) =default;

	TokenMatcherSettings& operator=(const TokenMatcherSettings&) =default;

	bool isInitialized() const {
		return !pattern.empty();
	}

	std::string name;
	std::string pattern;
	bool combine;
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

	bool operator!=(const TokenMatcher& other) const {
		return !(*this == other);
	}
	bool operator==(const TokenMatcher& other) const {
		if(name != other.name) {
			std::cout << "name" << std::endl;
			return false;
		}

		if(pattern->pattern() != other.pattern->pattern()) {
			std::cout << "pattern not matching" << std::endl;
			std::cout << "1: " << pattern->pattern() << std::endl;
			std::cout << "1: " << other.pattern->pattern() << std::endl;
			return false;
		}
		if(combine != other.combine)  {
			std::cout << "combine not matching" << std::endl;
			return false;
		}
		if(separator != other.separator)  {
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
					result[0]->append(match.data(),match.length());
					if(i != numMatches-1) result[0]->append(separator);
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

std::ostream& operator<<(std::ostream& stream, const TokenMatcher& tokenMatcher);

#endif /* TOKENMATCHER_H_ */
