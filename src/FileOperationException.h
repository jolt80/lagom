/*
 * FileOperationException.h
 *
 *  Created on: Dec 10, 2015
 *      Author: jolt
 */

#ifndef FILEOPERATIONEXCEPTION_H_
#define FILEOPERATIONEXCEPTION_H_

#include <exception>
#include <string>

enum class FileOperationExceptionFailureCode {
	OPEN,
	CLOSE
};

class FileOperationException : public std::exception {
public:
	FileOperationException() =default;
	FileOperationException(FileOperationExceptionFailureCode _fault, std::string _message) : fault{_fault}, message{_message} {};
	virtual ~FileOperationException() {}

	virtual const char* what() const noexcept {
		return message.c_str();
	}

	FileOperationExceptionFailureCode fault;
	std::string message;
};

#endif /* FILEOPERATIONEXCEPTION_H_ */
