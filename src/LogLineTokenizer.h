/*
 * LogLineTokenizer.h
 *
 *  Created on: Nov 17, 2015
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
	LogLineTokenizer(std::string _name, TokenMatcherSettings& prefix, std::vector<TokenMatcherSettings>& tokenPatterns);
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
