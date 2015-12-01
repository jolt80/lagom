/*
 * AutoMeasureDuration.h
 *
 *  Created on: Dec 1, 2015
 *      Author: jolt
 */

#ifndef AUTOMEASUREDURATION_H_
#define AUTOMEASUREDURATION_H_

#include <chrono>
#include <iostream>

class AutoMeasureDuration {
public:
	AutoMeasureDuration(std::ostream& _os, const std::string& _message) : os(_os), message(_message) {
		start = std::chrono::high_resolution_clock::now();
	}
	~AutoMeasureDuration() {
		end = std::chrono::high_resolution_clock::now();
		auto duration = end - start;

		//os << message << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms\n";
	}

private:
	std::ostream &os;
	std::string message;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

#endif /* AUTOMEASUREDURATION_H_ */
