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

#include <stdint.h>
#include <cassert>
#include <iostream>
#include <sstream>

#include "string_literal.h"

using namespace std;

const char* StringLiteral::getStr() const { return str; }

const char* StringLiteral::getStrEnd() const { return str + strLen; }

int StringLiteral::getLength() const { return strLen; }

const char StringLiteral::operator[](int i) const {
  const char* pos = str + i;
  return *pos;
}

bool StringLiteral::operator<(const StringLiteral& rhs) const {
  if (this->strLen < rhs.strLen)
    return true;
  else
    return strncmp(str, rhs.str, strLen) < 0;
}

bool StringLiteral::operator==(const char* compareStr) const { return 0 == strncmp(str, compareStr, strLen); }

bool StringLiteral::operator==(const std::string& compareStr) const {
  return 0 == strncmp(str, compareStr.c_str(), strLen);
}

bool StringLiteral::operator==(const StringLiteral& compareStr) const {
  return strLen == compareStr.strLen && 0 == strncmp(str, compareStr.str, strLen);
}

bool StringLiteral::operator!=(const char* compareStr) const { return !(*this == compareStr); }

bool StringLiteral::operator!=(const std::string& compareStr) const { return !(*this == compareStr); }

bool StringLiteral::operator!=(const StringLiteral& compareStr) const { return !(*this == compareStr); }

std::string StringLiteral::toString() const { return std::string(str, strLen); }

StringLiteral& StringLiteral::operator=(const StringLiteral& other) {
  this->str = other.str;
  this->strLen = other.strLen;
  return *this;
}

StringLiteral& StringLiteral::operator=(const char* other) {
  this->str = other;
  this->strLen = strlen(other);
  return *this;
}

StringLiteral StringLiteral::subString(int start, size_t length) const {
  if (start + length <= strLen) {
    return StringLiteral{str + start, length};
  }
  else {
    return StringLiteral{};
  }
}

StringLiteral StringLiteral::subString(std::pair<int, int> startAndEnd) const {
  assert(startAndEnd.second >= startAndEnd.first);
  return subString(startAndEnd.first, startAndEnd.second - startAndEnd.first);
}

int StringLiteral::findFirstOf(char charToFind) const {
  for (size_t i{0}; i < strLen; ++i) {
    if (str[i] == charToFind) return i;
  }
  return strLen;
}

void StringLiteral::trimWhitespaceFromStart() {
  int i{0};
  while (str[i] == ' ' || str[i] == '\t') ++i;
  trimFromStart(i);
}

std::pair<int, int> StringLiteral::findFirstAndSecondOf(char charToFind) const {
  int first = strLen;
  int second = strLen;
  size_t i;
  for (i = 0; i < strLen; ++i) {
    if (str[i] == charToFind) {
      first = i;
      break;
    }
  }
  for (; i < strLen; ++i) {
    if (str[i] == charToFind) {
      second = i;
      break;
    }
  }
  return std::pair<int, int>(first, second);
}

bool StringLiteral::startsWith(const StringLiteral& compareStr) const {
  if (compareStr.strLen > strLen) {
    return false;
  }

  return 0 == strncmp(str, compareStr.str, compareStr.strLen);
}

int StringLiteral::contains(std::string containsStr) const {
  int searchSize = containsStr.size();
  int searchIndex = 0;
  for (size_t i{0}; i < strLen; ++i) {
    if (str[i] == containsStr[searchIndex]) {
      searchIndex++;
      if (searchIndex == searchSize) return true;
    }
    else {
      searchIndex = 0;
    }
  }
  return false;
}

int StringLiteral::containsCaseInsensitive(std::string containsStr) const {
  int searchSize = containsStr.size();
  int searchIndex = 0;
  for (size_t i{0}; i < strLen; ++i) {
    // Sloppy comparison but probably pretty efficient
    if ((str[i] | 32) == (containsStr[searchIndex] | 32)) {
      searchIndex++;
      if (searchIndex == searchSize) return true;
    }
    else {
      searchIndex = 0;
    }
  }
  return false;
}

bool StringLiteral::empty() const { return strLen == 0; }

void StringLiteral::clear() {
  str = nullptr;
  strLen = 0;
}

bool StringLiteral::trimFromStart(size_t offset) {
  if (offset > strLen) {
    str = nullptr;
    strLen = 0;
    return false;
  }
  else {
    str += offset;
    strLen -= offset;
    return true;
  }
}

bool StringLiteral::trimFromEnd(size_t offset) {
  if (offset > strLen) {
    str = nullptr;
    strLen = 0;
    return false;
  }
  else {
    strLen -= offset;
    return true;
  }
}

int32_t StringLiteral::toInt() const { return std::atoi(str); }

/**
 * Not bad performance, but might be good to experiment a bit more with this.
 */
std::size_t StringLiteral::hash() const {
  const char* input = str;
  std::size_t result = 0x55555555;

  for (size_t index = 0; index < strLen; ++index) {
    result ^= *input++;
    result = result << 5;
  }

  return result;
}

std::ostream& operator<<(std::ostream& os, const StringLiteral& stringLiteral) {
  os << stringLiteral.toString();
  return os;
}
