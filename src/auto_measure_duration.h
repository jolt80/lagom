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

#ifndef LAGOM_AUTOMEASUREDURATION_H_
#define LAGOM_AUTOMEASUREDURATION_H_

#include <chrono>
#include <iostream>

namespace lagom {


class AutoMeasureDuration {
 public:
  AutoMeasureDuration(std::ostream& _os, const std::string& _message)
      : os(_os)
      , message(_message) {
    start = std::chrono::high_resolution_clock::now();
  }
  ~AutoMeasureDuration() {
    end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;

    // os << message << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms\n";
  }

 private:
  std::ostream& os;
  std::string message;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

} // namespace lagom

#endif /* LAGOM_AUTOMEASUREDURATION_H_ */
