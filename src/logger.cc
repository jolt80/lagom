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

#include "logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include "auto_lock.h"

using namespace std;
using namespace std::chrono;

Logger logger(".debug_log");

Logger::Logger(const char* logFileName) {
  //	of.open (logFileName, std::ofstream::out | std::ofstream::app);
}

Logger::~Logger() {
  //	of.flush();
  //	of.close();
}

void Logger::log(std::string msg) {
  AutoLock lock(mutex);
  //	high_resolution_clock::time_point tp = high_resolution_clock::now();
  //	time_t now_c = system_clock::to_time_t(tp);
  //	auto duration = tp.time_since_epoch();
  //	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
  //
  //	string name = clients.at(std::this_thread::get_id());

  //<< std::put_time(std::localtime(&now_c),"%H:%M:%S:")
  //	of << "- " << millis << " - " << name << " - " << msg << endl;
  //	of.flush();
}

void Logger::registerClient(std::string name) {
  // AutoLock lock(mutex);
  // clients.insert(std::pair<std::thread::id,std::string>(std::this_thread::get_id(),name));
}
