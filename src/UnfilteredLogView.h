/*
 * UnfilteredLogView.h
 *
 *  Created on: Nov 30, 2015
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

#ifndef UNFILTEREDLOGVIEW_H_
#define UNFILTEREDLOGVIEW_H_

#include <LogView.h>
#include <Log.h>

class UnfilteredLogView : public LogView {
public:
	UnfilteredLogView() : log{nullptr} {}
	UnfilteredLogView(Log* _log) : log{_log} {}
	virtual ~UnfilteredLogView() {}

	int getNumLines() const {
		return log->getNumLines();
	}

	int searchForLineContaining(int startLine, std::string search) {
		int lineIndex = startLine;

		while(!(log->getLine(lineIndex).contains(search))) {
			lineIndex++;
			if(lineIndex >= log->getNumLines()) return log->getNumLines();
		}
		return lineIndex;
	}

	StringLiteral getLine(int index) {
		return log->getLine(index);
	}

	int getLineNumber(int index) {
		return index;
	}

	std::string** getLogTokens(int index) {
		return log->getLogTokens(index);
	}

	int findCurrentLine(int lineNumber) {
		return lineNumber;
	}

private:
	Log* log;
};

#endif /* UNFILTEREDLOGVIEW_H_ */
