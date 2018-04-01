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

#ifndef UNFILTEREDLOGVIEW_H_
#define UNFILTEREDLOGVIEW_H_

#include "log.h"
#include "log_view.h"

class UnfilteredLogView : public LogView {
 public:
  UnfilteredLogView()
      : log{nullptr} {}
  UnfilteredLogView(Log* _log)
      : log{_log} {}
  virtual ~UnfilteredLogView() {}

  int getNumLines() const { return log->getNumLines(); }

  int searchForLineContaining(int startLine, std::string search, bool searchBackwards = false) {
    int lineIndex;
    if (searchBackwards) {
      if (startLine > 0) {
        lineIndex = startLine - 1;
      }
      else {
        return 0;
      }

      while (!(log->getLine(lineIndex).containsCaseInsensitive(search))) {
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

      while (!(log->getLine(lineIndex).containsCaseInsensitive(search))) {
        if (lineIndex >= lastIndex) return startLine;
        lineIndex++;
      }
      return lineIndex;
    }
  }

  StringLiteral getLine(int index) { return log->getLine(index); }

  int getLineNumber(int index) { return index; }

  std::string** getLogTokens(int index) { return log->getLogTokens(index); }

  int findCurrentLine(int lineNumber) { return lineNumber; }

 private:
  Log* log;
};

#endif /* UNFILTEREDLOGVIEW_H_ */
