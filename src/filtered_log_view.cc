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

#include "filtered_log_view.h"
#include <cassert>
#include <iostream>
#include "auto_measure_duration.h"

using namespace std;

namespace lagom {

FilteredLogView::~FilteredLogView() { delete matchingLines; }

int FilteredLogView::getNumLines() const { return matchingLines->size(); }

int FilteredLogView::searchForLineContaining(int startLine, std::string search, bool searchBackwards) {
  int lineIndex;
  if (searchBackwards) {
    if (startLine > 0) {
      lineIndex = startLine - 1;
    }
    else {
      return 0;
    }

    while (!(log->getLine(matchingLines->at(lineIndex)).containsCaseInsensitive(search))) {
      if (lineIndex == 0) return startLine;
      lineIndex--;
    }
    return lineIndex;
  }
  else {
    int lastIndex = log->getNumLines() - 1;
    if (startLine < lastIndex) {
      lineIndex = startLine + 1;
    }
    else {
      return lastIndex;
    }

    while (!(log->getLine(matchingLines->at(lineIndex)).containsCaseInsensitive(search))) {
      if (lineIndex >= lastIndex) return startLine;
      lineIndex++;
    }
    return lineIndex;
  }
}

StringLiteral FilteredLogView::getLine(int index) {
  int size = matchingLines->size();
  if (index < size) {
    return log->getLine(matchingLines->at(index));
  }
  else {
    return StringLiteral{};
  }
}

int FilteredLogView::getLineNumber(int index) {
  if (index < (int)matchingLines->size())
    return matchingLines->at(index);
  else
    return 0;
}

std::string** FilteredLogView::getLogTokens(int index) {
  if (index < (int)matchingLines->size()) {
    return log->getLogTokens(matchingLines->at(index));
  }
  else {
    return nullptr;
  }
}

int FilteredLogView::findCurrentLine(int lineNumber) {
  if (matchingLines->empty()) return 0;
  for (unsigned int i{0}; i < matchingLines->size(); ++i) {
    if (matchingLines->at(i) >= lineNumber) return i;
  }
  return matchingLines->size() - 1;
}

}  // namespace lagom
