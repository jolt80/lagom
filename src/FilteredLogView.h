/*
 * FilteredLogView.h
 *
 *  Created on: Nov 30, 2015
 *      Author: jolt
 */

#ifndef FILTEREDLOGVIEW_H_
#define FILTEREDLOGVIEW_H_

#include <LogView.h>
#include <Log.h>
#include <string>

class FilteredLogView : public LogView {
public:
	FilteredLogView(Log* _log, std::string _filterExpression) : log{_log}, filterExpression{_filterExpression} {
		populateMatchingLines();
	}
	virtual ~FilteredLogView() {}

	int getNumLines() const;
	int searchForLineContaining(int startLine, std::string search);
	StringLiteral getLine(int index);
	int getLineNumber(int index);
	std::string** getLogTokens(int index);
	int findCurrentLine(int lineNumber);


private:
	void populateMatchingLines();

	Log* log;
	std::string filterExpression;

	std::vector<int> matchingLines;
};

#endif /* FILTEREDLOGVIEW_H_ */
