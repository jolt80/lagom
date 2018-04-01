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

#ifndef LOG_H_
#define LOG_H_

#include <climits>
#include <string>
#include <string>
#include <vector>

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <mutex>

#include <re2/re2.h>

#include "settings.h"
#include "string_literal.h"

struct Line {
  StringLiteral contents;
  bool tokenized = false;
  std::string** tokens = nullptr;

  Line(char* start, char* end)
      : contents{start, end}
      , tokens{nullptr} {}
  Line(const Line&) = delete;
  Line(Line&&) = delete;
  ~Line() {
    if (tokens != nullptr) {
      for (int i{0}; i < 9; ++i) {
        delete tokens[i];
      }
      delete[] tokens;
    }
  }
};

class Log {
 public:
  Log(Settings& _settings);
  virtual ~Log();

  bool map(std::string fileName);
  bool unmap();

  bool areLineNumbersParsed() const;
  bool areAllLinesTokenized();
  int getNumLines() const;

  StringLiteral getLine(int index);
  std::string** getLogTokens(int index);

  std::string toString() const;

  void scanForLines(int index, long maxDuration = 2000000);
  int tokenizeLines(int index, long maxDuration = 2000000);

 protected:
  Line& lineAt(int index);
  void tokenizeLine(Line& line);
  void scanForLinesNotLocked(int index, long maxDuration = 2000000);

  mutable std::mutex mutex;

  int numLines;
  Settings& settings;
  char* fileStart;
  char* fileEnd;

  std::vector<Line*> lines;
};

#endif /* LOG_H_ */
