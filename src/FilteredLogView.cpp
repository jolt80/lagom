/*
 * FilteredLogView.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: jolt
 */

#include <FilteredLogView.h>
#include <cassert>
#include <iostream>

using namespace std;

int FilteredLogView::getNumLines() const {
	return matchingLines.size();
}

int FilteredLogView::searchForLineContaining(int startLine, std::string search) {
	assert(startLine > 0 && startLine < (int)matchingLines.size());
	return startLine;
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
	for(int i{0}; i < log->getNumLines(); ++i) {
//		cout << "line = " << log->getLine(i) << endl;
//		cout << "filterExpression = " << filterExpression << endl;
		if(log->getLine(i).contains(filterExpression)) {
//			cout << "matched line " << i << endl;
			matchingLines.push_back(i);
		}
	}
}
