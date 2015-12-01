/*
 * UnfilteredLogView.h
 *
 *  Created on: Nov 30, 2015
 *      Author: jolt
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
