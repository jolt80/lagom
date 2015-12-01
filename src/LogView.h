/*
 * LogView.h
 *
 *  Created on: Nov 30, 2015
 *      Author: jolt
 */

#ifndef LOGVIEW_H_
#define LOGVIEW_H_

#include <string>
#include <StringLiteral.h>

class LogView {
public:
	LogView() {}
	virtual ~LogView() {}

	virtual int getNumLines() const =0;
	virtual int searchForLineContaining(int startLine, std::string search) =0;
	virtual StringLiteral getLine(int index) =0;
	virtual int getLineNumber(int index) =0;
	virtual std::string** getLogTokens(int index) =0;
	virtual int findCurrentLine(int lineNumber) =0;
};

#endif /* LOGVIEW_H_ */
