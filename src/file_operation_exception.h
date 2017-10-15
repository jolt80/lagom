/*
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

	FileOperationExceptionFailureCode fault{FileOperationExceptionFailureCode::OPEN};
	std::string message;
};

#endif /* FILEOPERATIONEXCEPTION_H_ */
