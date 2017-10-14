/*
 * TokenDefinition.cpp
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

#include <iomanip>
#include "token_definition.h"

using namespace std;

std::ostream& operator<<(std::ostream& stream, const TokenDefinition& tokenDef)
{
  return stream << tokenDef.toString();
}

std::string TokenDefinition::toString() const {
	std::stringstream ss;
	string nameStr = name + ",";
	string widthStr = to_string(width) + ",";
	string visibilityStr = to_string(initialVisibility) + ",";
	string alignmentStr = alignmentToString(alignment) + ",";
	ss << setfill(' ') << left << setw(20) << nameStr;
	ss << setfill(' ') << left << setw(8) << widthStr;
	ss << setfill(' ') << left << setw(5) << visibilityStr;
	ss << setfill(' ') << left << setw(8) << alignmentStr;
	ss << setfill(' ') << left << setw(8) << alignmentToString(crop);
	return ss.str();
}
