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

#ifndef STATE_H_
#define STATE_H_

#include <cstdint>
#include <string>
#include <array>
#include "settings.h"

class State {
public:
	State(Settings& settings) {
		for(unsigned int i{0}; i < tokenVisible.size(); ++i) {
			tokenVisible[i] = settings.getTokenDefinition(i).getInitialVisibilty();
		}
	}
	State() { }

	virtual ~State() {}

	bool operator==( const State& other ) const {
		bool ret = forceUpdate == other.forceUpdate &&
				currLine == other.currLine &&
				running == other.running &&
				tokenized == other.tokenized &&
				filtered == other.filtered &&
				searchBackwards == other.searchBackwards &&
				lineOffset == other.lineOffset &&
				tokenVisible == other.tokenVisible &&
				filterExpression == other.filterExpression &&
				search == other.search;

		if(ret) {
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator!=( const State& other ) const {
		return !(*this == other);
	}

	std::string search;
	std::string filterExpression;

	int currLine = 0;
	int lineOffset = 0;

	bool running = true;
	bool filtered = false;
	bool searchBackwards = false;

	bool tokenized = false;
	std::array<bool,10> tokenVisible;

	bool forceUpdate = false;
};

#endif /* STATE_H_ */
