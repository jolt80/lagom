/*
 * Logger.h
 *
 *  Created on: Nov 8, 2015
 *      Author: jolt
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <map>
#include <mutex>
#include <fstream>
#include <thread>

class Logger : public std::ostream {
public:
	std::mutex mutex;
	std::ofstream of;

	Logger(const char* logFileName);
	virtual ~Logger();

	void log(std::string);

	void registerClient(std::string name);

    template<typename T>
    Logger& operator<< (const T& data)
    {
        of << data;

        return *this;
    }

private:
	std::map<std::thread::id,std::string> clients;
};

#endif /* LOGGER_H_ */
