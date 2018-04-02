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

#ifndef LAGOM_LOGVIEW_H_
#define LAGOM_LOGVIEW_H_

#include <string>
#include "string_literal.h"

namespace lagom {

/**
 * @brief Represents a 'view' into a Log, ie. a subset of lines.
 *
 */
class LogView {
 public:
  LogView() = default;
  virtual ~LogView() = 0;

  virtual int getNumLines() const = 0;

  virtual int searchForLineContaining(int startLine, std::string search, bool searchBackwards = false) = 0;
  virtual StringLiteral getLine(int index) = 0;
  virtual int getLineNumber(int index) = 0;
  virtual std::string** getLogTokens(int index) = 0;
  virtual int findCurrentLine(int lineNumber) = 0;
};

inline LogView::~LogView() {}

}  // namespace lagom

#endif /* LAGOM_LOGVIEW_H_ */
