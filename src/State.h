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

namespace TriFormatMask {
static const uint32_t line                {0b1}; // 1
static const uint32_t time               {0b10}; // 2
static const uint32_t timeDiff          {0b100}; // 3
static const uint32_t card             {0b1000}; // 4
static const uint32_t traceLevel      {0b10000}; // 5
static const uint32_t cpuId          {0b100000}; // 6
static const uint32_t process       {0b1000000}; // 7
static const uint32_t traceObj     {0b10000000}; // 8
static const uint32_t fileAndLine {0b100000000}; // 9
static const uint32_t msg        {0b1000000000}; // 0
}

class State {
public:
	State() {
		// Init format
		format = 0;
		format |= TriFormatMask::line;
		format |= TriFormatMask::time;
		format |= TriFormatMask::fileAndLine;
		format |= TriFormatMask::msg;
	}

	State ( const State & ) = default;
	virtual ~State() {}

	bool operator==( const State& other ) const {
		bool ret = forceUpdate == other.forceUpdate &&
				running == other.running &&
				filtered == other.filtered &&
				format == other.format &&
				currLine == other.currLine &&
				lineOffset == other.lineOffset &&
				width == other.width &&
				search == other.search;

		if(ret) {
			for(unsigned int  i = 0; i < width.size(); ++i)
			{
				if ( width[i] != other.width[i] ) return false;
			}
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

	std::array<int,12> width{ { 3,1,1,1,1,1,1,1,1,1,1,1 } };

	std::string search;

	bool running = true;
	int currLine = 0;
	int lineOffset = 0;

	bool filtered = false;

	bool forceUpdate = false;

	uint32_t format;
};

#endif /* STATE_H_ */
