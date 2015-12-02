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

int FilteredLogView::searchForLineContaining(int startLine, std::string search, bool searchBackwards) {
	int lineIndex;
	if(searchBackwards) {
		if(startLine > 0) {
			lineIndex = startLine - 1;
		}
		else
		{
			return 0;
		}

		while(!(log->getLine(matchingLines->at(lineIndex)).containsCaseInsensitive(search))) {
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

		while(!(log->getLine(matchingLines->at(lineIndex)).containsCaseInsensitive(search))) {
			if(lineIndex >= lastIndex) return startLine;
			lineIndex++;
		}
		return lineIndex;
	}
}

StringLiteral FilteredLogView::getLine(int index) {
	if(index < (int)matchingLines->size()) {
		return log->getLine(matchingLines->at(index));
	}
	else
	{
		return StringLiteral{};
	}
}

int FilteredLogView::getLineNumber(int index) {
	if(index < (int)matchingLines->size())
		return matchingLines->at(index);
	else
		return 0;
}

std::string** FilteredLogView::getLogTokens(int index) {
	return log->getLogTokens(matchingLines->at(index));
}

int FilteredLogView::findCurrentLine(int lineNumber) {
	if(matchingLines->empty()) return 0;
	for(unsigned int i{0}; i < matchingLines->size(); ++i) {
		if(matchingLines->at(i) >= lineNumber) return i;
	}
	return matchingLines->size() - 1;
}

