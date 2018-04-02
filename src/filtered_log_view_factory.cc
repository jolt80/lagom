/*
 * Copyright (c) 2018 Tomas Szabo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "filtered_log_view_factory.h"

#include <re2/re2.h>
#include <iostream>
#include <memory>
#include <set>

#include "auto_measure_duration.h"

using namespace std;

namespace lagom {

std::string FilteredLogViewFactory::getLastErrorMessage() {
  std::string ret = errorMessage;
  errorMessage.clear();
  return ret;
}

std::shared_ptr<LogView> FilteredLogViewFactory::getFilteredLogView(std::string pattern) {
  std::shared_ptr<LogView> filteredView = filteredViews[pattern];
  if (!filteredView) {
    AutoMeasureDuration meas(cout, "parsing " + pattern);
    std::vector<int>* vectorOfMatchingLines = buildVectorOfMatchingLines(pattern);
    if (nullptr == vectorOfMatchingLines) return nullptr;
    filteredView.reset(new FilteredLogView(log, shared_ptr<vector<int>>(vectorOfMatchingLines)));
  }
  return filteredView;
}

std::vector<int>* FilteredLogViewFactory::buildVectorOfMatchingLines(std::string pattern) {
  std::vector<int>* matchingLines = new std::vector<int>();
  std::list<std::string> parts = splitMultiplePattern(pattern);

  if (parts.size() > 1) {
    std::set<int> totalSetOfMatchingLines;

    for (auto part : parts) {
      std::vector<int>* partMatchingLines = buildVectorOfMatchingLines(part);
      if (nullptr == partMatchingLines) {
        delete matchingLines;
        return nullptr;
      }
      for (auto lineNr : *partMatchingLines) {
        totalSetOfMatchingLines.insert(lineNr);
      }
      delete partMatchingLines;
    }
    for (auto lineNr : totalSetOfMatchingLines) {
      matchingLines->push_back(lineNr);
    }
  }
  // simple pattern
  else {
    RE2::Options options;
    options.set_log_errors(false);
    RE2 matcher{pattern, options};
    if (!matcher.ok()) {
      errorMessage += matcher.error();
      delete matchingLines;
      return nullptr;
    }
    if (matcher.NumberOfCapturingGroups() != 0) {
      errorMessage += "expressions shouldn't contain capturing groups: ";
      errorMessage += to_string(matcher.NumberOfCapturingGroups());
      errorMessage += " groups in ";
      errorMessage += matcher.pattern();
      delete matchingLines;
      return nullptr;
    }
    for (int i{0}; i < log.getNumLines(); ++i) {
      if (RE2::PartialMatch(log.getLine(i).toStringPiece(), matcher)) {
        matchingLines->push_back(i);
      }
    }
  }
  return matchingLines;
}

std::list<std::string> FilteredLogViewFactory::splitMultiplePattern(std::string& pattern) {
  std::list<std::string> ret;
  StringLiteral search{pattern};

  do {
    while (!search.empty() && search[0] == ' ') search.trimFromStart(1);
    int firstSeparator = search.findFirstOf(' ');
    ret.push_front(search.subString(0, firstSeparator).toString());
    search.trimFromStart(firstSeparator + 1);
  } while (!search.empty());

  return ret;
}

}  // namespace lagom
