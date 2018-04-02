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

#include "history.h"
#include <fstream>
#include <iostream>
#include "file_operation_exception.h"

using namespace std;

namespace lagom {

History::History(std::string _filePath)
    : filePath{_filePath} {
  pos = storage.end();
  readHistoryFile();
}

History::~History() { writeHistoryToFile(); }

void History::readHistoryFile() {
  ifstream inFile{filePath, ios::in};
  string inLine;

  if (inFile.is_open()) {
    while (getline(inFile, inLine)) {
      addEntry(inLine);
    }

    inFile.close();
  }
  else {
    cerr << "Couldn't open history file " << filePath << endl;
  }
}

void History::writeHistoryToFile() {
  ofstream outFile{filePath};

  if (outFile.is_open()) {
    for (auto entry : storage) {
      outFile << entry << endl;
    }

    outFile.close();
  }
}

std::string History::getPrevEntry() {
  if (storage.empty()) {
    return {};
  }

  std::list<std::string>::iterator prev = pos;
  if (prev != storage.begin()) {
    --prev;
  }
  pos = prev;
  return *pos;
}

std::string History::getNextEntry() {
  if (storage.empty()) {
    return "";
  }
  if (pos == storage.end()) return "";
  if (++pos == storage.end()) return "";
  return *pos;
}

void History::addEntry(std::string entry) {
  if (pos != storage.end()) {
    if (*pos == entry) {
      moveCurrentToEnd();
    }
  }
  else {
    storage.push_back(entry);
  }
  pos = storage.end();
}

void History::moveCurrentToEnd() {
  storage.push_back(*pos);
  storage.erase(pos);
  pos = storage.end();
}

void History::resetPosition() { pos = storage.end(); }

}  // namespace lagom
