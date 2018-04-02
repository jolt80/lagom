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

#ifndef STATE_H_
#define STATE_H_

#include <array>
#include <cstdint>
#include <string>
#include "settings.h"

namespace lagom {


class State {
 public:
  State(Settings& settings) {
    for (unsigned int i{0}; i < tokenVisible.size(); ++i) {
      tokenVisible[i] = settings.getTokenDefinition(i).getInitialVisibilty();
    }
  }
  State() {}

  virtual ~State() {}

  bool operator==(const State& other) const {
    bool ret = forceUpdate == other.forceUpdate && currLine == other.currLine && running == other.running &&
               tokenized == other.tokenized && filtered == other.filtered && searchBackwards == other.searchBackwards &&
               lineOffset == other.lineOffset && tokenVisible == other.tokenVisible &&
               filterExpression == other.filterExpression && search == other.search;

    if (ret) {
      return true;
    }
    else {
      return false;
    }
  }
  bool operator!=(const State& other) const { return !(*this == other); }

  std::string search;
  std::string filterExpression;

  int currLine = 0;
  int lineOffset = 0;

  bool running = true;
  bool filtered = false;
  bool searchBackwards = false;

  bool tokenized = false;
  std::array<bool, 10> tokenVisible;

  bool forceUpdate = false;
};

} // namespace lagom

#endif /* STATE_H_ */
