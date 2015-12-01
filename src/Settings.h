/*
 * Settings.h
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

#ifndef SETTINGS_H_
#define SETTINGS_H_
#include <re2/re2.h>
#include <vector>
#include <LogLineTokenizer.h>
#include <TokenDefinition.h>

class Settings {
public:
	Settings();
	virtual ~Settings();

	const std::vector<LogLineTokenizer*> getTokenizers() const {
		return tokenizers;
	}

	const TokenDefinition& getTokenDefinition(int tokenIndex) const;
private:
	std::vector<LogLineTokenizer*> tokenizers;
	std::array<TokenDefinition,10> tokens;
};

#endif /* SETTINGS_H_ */
