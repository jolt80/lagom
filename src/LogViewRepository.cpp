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
#include <re2/re2.h>

using namespace std;

LogViewRepository::~LogViewRepository() {
	for(auto elem : filteredViews) {
		delete elem.second;
		elem.second = nullptr;
	}
}

std::string LogViewRepository::getLastErrorMessage() {
	std::string ret = errorMessage;
	errorMessage.clear();
	return ret;
}

LogView* LogViewRepository::getFilteredLogView(std::string pattern) {
	LogView* filteredView = filteredViews[pattern];
	if(nullptr == filteredView) {
		AutoMeasureDuration meas(cout,"parsing " + pattern);
		std::vector<int>* vectorOfMatchingLines = buildVectorOfMatchingLines(pattern);
		if(nullptr == vectorOfMatchingLines) return nullptr;
		filteredView = new FilteredLogView(&log,vectorOfMatchingLines);
	}

	return filteredView;
}


std::vector<int>* LogViewRepository::buildVectorOfMatchingLines(std::string pattern) {
	std::vector<int>* matchingLines = new std::vector<int>();
	std::list<std::string> parts = splitMultiplePattern(pattern);

	if(parts.size() > 1) {
		std::set<int> totalSetOfMatchingLines;

		for(auto part : parts) {
			std::vector<int>* partMatchingLines = buildVectorOfMatchingLines(part);
			if(nullptr == partMatchingLines) {
				delete matchingLines;
				return nullptr;
			}
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
		RE2::Options options;
		options.set_log_errors(false);
		RE2 matcher{pattern,options};
		if(!matcher.ok()) {
			errorMessage += matcher.error();
			delete matchingLines;
			return nullptr;
		}
		if(matcher.NumberOfCapturingGroups() != 0) {
			errorMessage += "expressions shouldn't contain capturing groups: ";
			errorMessage += to_string(matcher.NumberOfCapturingGroups());
			errorMessage += " groups in ";
			errorMessage += matcher.pattern();
			delete matchingLines;
			return nullptr;
		}
		for(int i{0}; i < log.getNumLines(); ++i) {
			if(RE2::PartialMatch(log.getLine(i).toStringPiece(),matcher)) {
				matchingLines->push_back(i);
			}
		}
	}
	return matchingLines;
}

std::list<std::string> LogViewRepository::splitMultiplePattern(std::string& pattern) {
	std::list<std::string> ret;
	StringLiteral search{pattern};

	do {
		while(!search.empty() && search[0] == ' ') search.trimFromStart(1);
		int firstSeparator = search.findFirstOf(' ');
		ret.push_front(search.subString(0,firstSeparator).toString());
		search.trimFromStart(firstSeparator+1);
	} while (!search.empty());

	return ret;
}
