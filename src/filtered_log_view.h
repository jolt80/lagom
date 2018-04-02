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

#ifndef FILTEREDLOGVIEW_H_
#define FILTEREDLOGVIEW_H_

#include <string>

#include "log.h"
#include "log_view.h"

namespace lagom {

class FilteredLogView : public LogView {
 public:
  friend class LogViewRepository;

  FilteredLogView(Log* _log, std::vector<int>* _matchingLines)
      : log{_log}
      , matchingLines{_matchingLines} {}
  virtual ~FilteredLogView();

  int getNumLines() const;
  int searchForLineContaining(int startLine, std::string search, bool searchBackwards = false);
  StringLiteral getLine(int index);
  int getLineNumber(int index);
  std::string** getLogTokens(int index);
  int findCurrentLine(int lineNumber);

 protected:
  Log* log;
  std::vector<int>* matchingLines;
};

} // namespace lagom

#endif /* FILTEREDLOGVIEW_H_ */
