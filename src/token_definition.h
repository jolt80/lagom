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

#ifndef TOKENDEFINITION_H_
#define TOKENDEFINITION_H_

#include <sstream>
#include <string>

enum class Alignment {
	left,
	right
};

class TokenDefinition {
public:
	TokenDefinition()  : name{}, width{}, alignment{Alignment::left}, crop{Alignment::left}, initialVisibility{false} { }
	TokenDefinition(std::string _name, int _width, Alignment _alignment, bool _initialVisibility) : name{_name}, width{_width}, alignment{_alignment}, crop{Alignment::left}, initialVisibility{_initialVisibility} { }
	TokenDefinition(std::string _name, int _width, Alignment _alignment, Alignment _crop, bool _initialVisibility) : name{_name}, width{_width}, alignment{_alignment}, crop{_crop}, initialVisibility{_initialVisibility} { }
	virtual ~TokenDefinition() {};

	bool operator!=(const TokenDefinition& other) const {
		return !(*this == other);
	}


	bool operator==(const TokenDefinition& other) const {
		return  name == other.name &&
				width == other.width &&
				alignment == other.alignment &&
				crop == other.crop &&
				initialVisibility == other.initialVisibility;
	}

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

	static std::string alignmentToString(Alignment a) {
		if(a == Alignment::left) return "left";
		else return "right";
	}

	std::string toString() const;

private:
	std::string name;
	int width;
	Alignment alignment;
	Alignment crop;
	bool initialVisibility;
};

std::ostream& operator<<(std::ostream& stream, const TokenDefinition& tokenDef);

#endif /* TOKENDEFINITION_H_ */
