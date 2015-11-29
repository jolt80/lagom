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
#include <TokenDefinition.h>

class Settings {
public:
	Settings();
	virtual ~Settings();

	const std::vector<LogLineTokenizer*> getTokenizers() const {
		return tokenizers;
	}

	const TokenDefinition& getTokenDefinition(int tokenIndex) const;
private:
	std::vector<LogLineTokenizer*> tokenizers;
	std::array<TokenDefinition,10> tokens;
};

#endif /* SETTINGS_H_ */
