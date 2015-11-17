/*
 * LogLineTokenizer.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: jolt
 */

#include <LogLineTokenizer.h>
#include <assert.h>
#include <sstream>

using namespace std;

static const char* separator = ".*?";

LogLineTokenizer::LogLineTokenizer(std::string _name, std::vector<std::pair<int,std::string>> patternInit) : name{_name} {
	std::string fullPattern;

	int match{0};
	for(auto token : patternInit) {
		if(token.second != "") {
			fullPattern += separator;
			fullPattern += token.second;
			for(int j{0}; j < token.first; ++j) {
				matchMapping.push_back(match);
				match++;
			}
		}
		else {
			match += token.first;
		}
	}
	fullPattern += ".*?";

	pattern = new RE2{ fullPattern };

	assert(pattern->ok());
}

LogLineTokenizer::~LogLineTokenizer() {
	delete pattern;
}

bool LogLineTokenizer::tokenizeLine(re2::StringPiece line, re2::StringPiece res[]) const {
	bool result = false;

	result = RE2::FullMatchN(line,*pattern,matches,matchMapping.size());

	return result;
}

std::string LogLineTokenizer::toString() const {
	stringstream ss;

	ss << ">>name" << "," << pattern->pattern() << ",";
	for(auto match : matchMapping) {
		ss << match;
	}
	return ss.str();
}



