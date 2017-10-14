/*
 * AutoLock.h
 *
 *  Created on: Nov 22, 2015
 *      Author: Tomas Szabo
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AUTOLOCK_H_
#define AUTOLOCK_H_

#include <mutex>
#include <iostream>
#include <cstdlib>
extern "C" {
	#include <sys/types.h>
	#include <sys/unistd.h>
	#include <sys/syscall.h>
}

class AutoLock {
public:
	AutoLock(std::mutex& _mut) : mut{_mut} {
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

#endif /* AUTOLOCK_H_ */
