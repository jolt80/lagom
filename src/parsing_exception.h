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

#ifndef PARSINGEXCEPTION_H_
#define PARSINGEXCEPTION_H_

#include <exception>
#include <string>

class ParsingException : public std::exception {
 public:
  ParsingException(std::string _message)
      : message{_message}
      , pos{nullptr} {}

  ParsingException(std::string fileName, int lineNr, int linePos, std::string _message)
      : pos{nullptr} {
    message += fileName;
    message += ":";
    message += std::to_string(lineNr);
    message += ":";
    message += std::to_string(linePos);
    message += "\tparse error:\t";
    message += _message;
  }

  ParsingException(std::string _message, const char* _pos)
      : message{_message}
      , pos{_pos} {}

  virtual ~ParsingException() {}

  virtual const char* what() const noexcept { return message.c_str(); }

  bool hasStringPos() const { return pos != nullptr; }

  const char* getStringPos() const { return pos; }

 private:
  std::string message;
  const char* pos;
};

#endif /* PARSINGEXCEPTION_H_ */
