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

#ifndef FILTEREDLOGVIEW_H_
#define FILTEREDLOGVIEW_H_

#include <string>

#include "log_view.h"
#include "log.h"

class FilteredLogView : public LogView {
public:
	friend class LogViewRepository;

	FilteredLogView(Log* _log, std::vector<int>* _matchingLines) : log{_log}, matchingLines{_matchingLines} { }
	virtual ~FilteredLogView();

	int getNumLines() const;
	int searchForLineContaining(int startLine, std::string search, bool searchBackwards = false);
	StringLiteral getLine(int index);
	int getLineNumber(int index);
	std::string** getLogTokens(int index);
	int findCurrentLine(int lineNumber);

protected:
	Log* log;
	std::vector<int>* matchingLines;
};

#endif /* FILTEREDLOGVIEW_H_ */
