/*
 * ParsingException.h
 *
 *  Created on: Dec 7, 2015
 *      Author: jolt
 */

#ifndef PARSINGEXCEPTION_H_
#define PARSINGEXCEPTION_H_

#include <exception>
#include <string>

class ParsingException : public std::exception {
public:
	ParsingException(std::string _message) : message{_message}, pos{nullptr} {
	}

	ParsingException(std::string fileName, int lineNr, int linePos, std::string _message) : pos{nullptr}{
		message += fileName;
		message += ":";
		message += std::to_string(lineNr);
		message += ":";
		message += std::to_string(linePos);
		message += "\tparse error:\t";
		message += _message;
	}

	ParsingException(std::string _message, const char* _pos) : message{_message}, pos{_pos} {
	}

	virtual ~ParsingException() {}

	virtual const char* what() const noexcept {
		return message.c_str();
	}

	bool hasStringPos() const {
		return pos != nullptr;
	}

	const char* getStringPos() const {
		return pos;
	}

private:
	std::string message;
	const char* pos;
};

#endif /* PARSINGEXCEPTION_H_ */
