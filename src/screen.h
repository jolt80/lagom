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

#ifndef SCREEN_H_
#define SCREEN_H_

#include <ncurses.h>
#include <re2/stringpiece.h>
#include <chrono>
#include <string>
#include "history.h"
#include "log_view.h"
#include "settings.h"
#include "state.h"

namespace lagom {


class Screen {
 public:
  LogView* logView;
  State& currentState;
  Settings& settings;

  Screen(LogView* _logView, State& _state, Settings& _settings);
  virtual ~Screen();

  int getRows() const;
  int getCols() const;

  void updateSize();

  void print(const std::string str);
  void print(const char* str);

  void println(const std::string str);
  void println(const char* str);

  void printToken(StringLiteral token, int formatIndex, bool printSeparatorPrefix);
  void printToken(StringLiteral token);
  void printLine(int line);

  void fillRestOfLine(int c);

  void drawLog();

  void refresh();

  void setLogView(LogView* newLogView);

  std::string getInputLine(History* history = nullptr);
  int getInputInteger();
  int getInput();

 protected:
  State lastDrawnState;

  bool isNumberOrLetter(int c) const;
  bool updateNeeded() const;

  int rows;
  int cols;
};

} // namespace lagom

#endif /* SCREEN_H_ */
