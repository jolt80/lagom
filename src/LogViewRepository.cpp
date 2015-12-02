/*
 * LogViewFactory.cpp
 *
 *  Created on: Dec 2, 2015
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

#include <LogViewRepository.h>
#include <AutoMeasureDuration.h>
#include <iostream>
#include <set>

using namespace std;

LogViewRepository::~LogViewRepository() {
	for(auto elem : filteredViews) {
		delete elem.second;
		elem.second = nullptr;
	}
}

LogView* LogViewRepository::getFilteredLogView(std::string pattern) {
	LogView* filteredView = filteredViews[pattern];
	if(nullptr == filteredView) {
		AutoMeasureDuration meas(cout,"parsing " + pattern);
		filteredView = new FilteredLogView(&log,buildVectorOfMatchingLines(pattern));
	}

	return filteredView;
}


std::vector<int>* LogViewRepository::buildVectorOfMatchingLines(std::string pattern) {
	std::vector<int>* matchingLines = new std::vector<int>();

	if(isMultiplePattern(pattern)) {
		std::forward_list<std::string> parts = splitMultiplePattern(pattern);
		std::set<int> totalSetOfMatchingLines;

		for(auto part : parts) {
			std::vector<int>* partMatchingLines = buildVectorOfMatchingLines(part);
			for(auto lineNr : *partMatchingLines) {
				totalSetOfMatchingLines.insert(lineNr);
			}
			delete partMatchingLines;
		}
		for(auto lineNr : totalSetOfMatchingLines) {
			matchingLines->push_back(lineNr);
		}
	}
	// simple pattern
	else {
		for(int i{0}; i < log.getNumLines(); ++i) {
			if(log.getLine(i).containsCaseInsensitive(pattern)) {
				matchingLines->push_back(i);
			}
		}
	}
	return matchingLines;
}

bool LogViewRepository::isMultiplePattern(std::string& pattern) {

	StringLiteral search{pattern};

	if(search.contains(" ")) return true;

	return false;
}

std::forward_list<std::string> LogViewRepository::splitMultiplePattern(std::string& pattern) {
	std::forward_list<std::string> ret;
	StringLiteral search{pattern};

	do {
		int offset = search.findFirstOf(' ');
		ret.push_front(search.subString(0,offset).toString());
		search.trimFromStart(offset+1);
	} while (!search.empty());

	return ret;
}
