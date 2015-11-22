/*
 * AutoLock.h
 *
 *  Created on: Nov 22, 2015
 *      Author: jolt
 */

#ifndef AUTOLOCK_H_
#define AUTOLOCK_H_

#include <mutex>

class AutoLock {
public:
	AutoLock(std::recursive_mutex& _mut) : mut{_mut} {
		mut.lock();
	}

	~AutoLock() {
		mut.unlock();
	}

private:
	std::recursive_mutex& mut;
};

#endif /* AUTOLOCK_H_ */
