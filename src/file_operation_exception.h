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

#ifndef LAGOM_FILEOPERATIONEXCEPTION_H_
#define LAGOM_FILEOPERATIONEXCEPTION_H_

#include <exception>
#include <string>

namespace lagom {


enum class FileOperationExceptionFailureCode { OPEN, CLOSE };

class FileOperationException : public std::exception {
 public:
  FileOperationException() = default;
  FileOperationException(FileOperationExceptionFailureCode _fault, std::string _message)
      : fault{_fault}
      , message{_message} {};
  virtual ~FileOperationException() {}

  virtual const char* what() const noexcept { return message.c_str(); }

  FileOperationExceptionFailureCode fault{FileOperationExceptionFailureCode::OPEN};
  std::string message;
};

} // namespace lagom

#endif /* LAGOM_FILEOPERATIONEXCEPTION_H_ */
