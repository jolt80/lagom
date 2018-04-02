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

#ifndef LAGOM_LOGGER_H_
#define LAGOM_LOGGER_H_

#include <fstream>
#include <map>
#include <mutex>
#include <thread>

namespace lagom {


class Logger : public std::ostream {
 public:
  std::mutex mutex;
  std::ofstream of;

  Logger(const char* logFileName);
  virtual ~Logger();

  void log(std::string);

  void registerClient(std::string name);

  template <typename T>
  Logger& operator<<(const T& data) {
    //        of << data;

    return *this;
  }

 private:
  std::map<std::thread::id, std::string> clients;
};

} // namespace lagom

#endif /* LAGOM_LOGGER_H_ */
