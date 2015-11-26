/*
 * Logger.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: jolt
 */

#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include <AutoLock.h>

using namespace std;
using namespace std::chrono;

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
	//AutoLock lock(mutex);
	//clients.insert(std::pair<std::thread::id,std::string>(std::this_thread::get_id(),name));
}
