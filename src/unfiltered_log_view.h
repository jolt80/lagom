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

#include "log_view.h"
#include "log.h"

class UnfilteredLogView : public LogView {
public:
	UnfilteredLogView() : log{nullptr} {}
	UnfilteredLogView(Log* _log) : log{_log} {}
	virtual ~UnfilteredLogView() {}

	int getNumLines() const {
		return log->getNumLines();
	}

	int searchForLineContaining(int startLine, std::string search, bool searchBackwards = false) {
		int lineIndex;
		if(searchBackwards) {
			if(startLine > 0) {
				lineIndex = startLine - 1;
			}
			else
			{
				return 0;
			}

			while(!(log->getLine(lineIndex).containsCaseInsensitive(search))) {
				if(lineIndex == 0) return startLine;
				lineIndex--;
			}
			return lineIndex;
		}
		else {
			int lastIndex = log->getNumLines() - 1;
			if(startLine < lastIndex) {
				lineIndex = startLine + 1;
			}
			else
			{
				return lastIndex;
			}

			while(!(log->getLine(lineIndex).containsCaseInsensitive(search))) {
				if(lineIndex >= lastIndex) return startLine;
				lineIndex++;
			}
			return lineIndex;
		}
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
