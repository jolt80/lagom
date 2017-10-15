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

#ifndef LOGVIEW_H_
#define LOGVIEW_H_

#include <string>
#include "string_literal.h"

class LogView {
public:
	LogView() {}
	virtual ~LogView() {}

	virtual int getNumLines() const =0;
	virtual int searchForLineContaining(int startLine, std::string search, bool searchBackwards = false) =0;
	virtual StringLiteral getLine(int index) =0;
	virtual int getLineNumber(int index) =0;
	virtual std::string** getLogTokens(int index) =0;
	virtual int findCurrentLine(int lineNumber) =0;
};

#endif /* LOGVIEW_H_ */
