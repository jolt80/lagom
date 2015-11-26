/*
 * AutoLock.h
 *
 *  Created on: Nov 22, 2015
 *      Author: jolt
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
