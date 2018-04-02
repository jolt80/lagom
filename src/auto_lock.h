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

#ifndef AUTOLOCK_H_
#define AUTOLOCK_H_

#include <cstdlib>
#include <iostream>
#include <mutex>
extern "C" {
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/unistd.h>

namespace lagom {

}

class AutoLock {
 public:
  AutoLock(std::mutex& _mut)
      : mut{_mut} {
    //		pid_t x = syscall(__NR_gettid);
    //		std::cout << std::hex << "0x" << x << " grabbing lock" << std::endl;
    mut.lock();
  }

  ~AutoLock() {
    //		pid_t x = syscall(__NR_gettid);
    //		std::cout << std::hex << "0x" << x << " unlocking" << std::endl;
    mut.unlock();
  }

 private:
  std::mutex& mut;
};

} // namespace lagom

#endif /* AUTOLOCK_H_ */
