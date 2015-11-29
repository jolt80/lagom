/*
 * State.h
 *
 *  Created on: Nov 7, 2015
 *      Author: jolt
 */

#ifndef STATE_H_
#define STATE_H_

#include <cstdint>
#include <string>
#include <array>
#include <Settings.h>

class State {
public:
	State(Settings& settings) {
		for(unsigned int i{0}; i < tokenVisible.size(); ++i) {
			tokenVisible[i] = settings.getTokenDefinition(i).getInitialVisibilty();
		}
	}
	State() { }

	virtual ~State() {}

	bool operator==( const State& other ) const {
		bool ret = forceUpdate == other.forceUpdate &&
				running == other.running &&
				filtered == other.filtered &&
				tokenVisible == other.tokenVisible &&
				currLine == other.currLine &&
				lineOffset == other.lineOffset &&
				search == other.search;

		if(ret) {
			return true;
		}
		else
		{
			return false;
		}
	}
	bool operator!=( const State& other ) const {
		return !(*this == other);
	}

	std::string search;

	bool running = true;
	int currLine = 0;
	int lineOffset = 0;

	bool filtered = false;

	bool forceUpdate = false;

	std::array<bool,10> tokenVisible;
};

#endif /* STATE_H_ */
