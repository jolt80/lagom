/*
 * TokenDefinition.h
 *
 *  Created on: Nov 29, 2015
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

#ifndef TOKENDEFINITION_H_
#define TOKENDEFINITION_H_

#include <string>

enum class Alignment {
	LEFT,
	RIGHT
};

class TokenDefinition {
public:
	TokenDefinition()  : name{}, width{}, alignment{Alignment::LEFT}, crop{Alignment::LEFT}, initialVisibility{false} { }
	TokenDefinition(std::string _name, int _width, Alignment _alignment, bool _initialVisibility) : name{_name}, width{_width}, alignment{_alignment}, crop{Alignment::LEFT}, initialVisibility{_initialVisibility} { }
	TokenDefinition(std::string _name, int _width, Alignment _alignment, Alignment _crop, bool _initialVisibility) : name{_name}, width{_width}, alignment{_alignment}, crop{_crop}, initialVisibility{_initialVisibility} { }
	virtual ~TokenDefinition() {};

	inline const std::string& getName() const {
		return name;
	}

	inline int getWidth() const {
		return width;
	}

	inline Alignment getAlignment() const {
		return alignment;
	}

	inline Alignment getCrop() const {
		return crop;
	}

	bool getInitialVisibilty() const {
		return initialVisibility;
	}

private:
	std::string name;
	int width;
	Alignment alignment;
	Alignment crop;
	bool initialVisibility;
};

#endif /* TOKENDEFINITION_H_ */
