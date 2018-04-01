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

#ifndef HISTORY_H_
#define HISTORY_H_

#include <list>
#include <string>

class History {
 public:
  History(std::string filePath);
  virtual ~History();

  std::string getPrevEntry();
  std::string getNextEntry();
  void addEntry(std::string entry);
  void moveCurrentToEnd();
  void resetPosition();

 private:
  void readHistoryFile();
  void writeHistoryToFile();

  std::string filePath;

  std::list<std::string> storage;
  std::list<std::string>::iterator pos;
};

#endif /* HISTORY_H_ */
