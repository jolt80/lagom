/*
 * FilteredLogView.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: jolt
 */

#include <FilteredLogView.h>
#include <cassert>
#include <iostream>
#include <AutoMeasureDuration.h>

using namespace std;

int FilteredLogView::getNumLines() const {
	return matchingLines.size();
}

int FilteredLogView::searchForLineContaining(int startLine, std::string search) {
	int lineIndex = startLine;
	int maxLines = matchingLines.size();

	while(!(log->getLine(matchingLines.at(lineIndex)).contains(search))) {
		lineIndex++;
		if(lineIndex >= maxLines) return maxLines;
	}
	return lineIndex;
}

StringLiteral FilteredLogView::getLine(int index) {
	return log->getLine(matchingLines.at(index));
}

int FilteredLogView::getLineNumber(int index) {
	return matchingLines.at(index);
}

std::string** FilteredLogView::getLogTokens(int index) {
	return log->getLogTokens(matchingLines.at(index));
}

void FilteredLogView::populateMatchingLines() {
	AutoMeasureDuration meas(cout,"populateMatchingLines");
	for(int i{0}; i < log->getNumLines(); ++i) {
		if(log->getLine(i).contains(filterExpression)) {
			matchingLines.push_back(i);
		}
	}
}

int FilteredLogView::findCurrentLine(int lineNumber) {
	for(unsigned int i{0}; i < matchingLines.size(); ++i) {
		if(matchingLines.at(i) >= lineNumber) return i;
	}
	return matchingLines.size() - 1;
}

