/*
 * Settings.h
 *
 *  Created on: Nov 17, 2015
 *      Author: jolt
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_
#include <re2/re2.h>
#include <vector>
#include <LogLineTokenizer.h>

class Settings {
public:
	Settings();
	virtual ~Settings();

	const std::vector<LogLineTokenizer*> getTokenizers() const {
		return tokenizers;
	}

	int getWidth(int tokenIndex) const;

private:
	std::vector<LogLineTokenizer*> tokenizers;
	std::vector<int> tokenWidths;
};

#endif /* SETTINGS_H_ */
