/*
 * FilteredLogView.cpp
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

#include <FilteredLogView.h>
#include <cassert>
#include <iostream>
#include <AutoMeasureDuration.h>

using namespace std;

FilteredLogView::~FilteredLogView() {
	delete matchingLines;
}

int FilteredLogView::getNumLines() const {
	return matchingLines->size();
}

int FilteredLogView::searchForLineContaining(int startLine, std::string search) {
	int lineIndex = startLine;
	int maxLines = matchingLines->size();

	while(!(log->getLine(matchingLines->at(lineIndex)).contains(search))) {
		lineIndex++;
		if(lineIndex >= maxLines) return maxLines;
	}
	return lineIndex;
}

StringLiteral FilteredLogView::getLine(int index) {
	return log->getLine(matchingLines->at(index));
}

int FilteredLogView::getLineNumber(int index) {
	return matchingLines->at(index);
}

std::string** FilteredLogView::getLogTokens(int index) {
	return log->getLogTokens(matchingLines->at(index));
}

int FilteredLogView::findCurrentLine(int lineNumber) {
	for(unsigned int i{0}; i < matchingLines->size(); ++i) {
		if(matchingLines->at(i) >= lineNumber) return i;
	}
	return matchingLines->size() - 1;
}

