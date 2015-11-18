/*
 * LogLineTokenizer.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: jolt
 */

#include <LogLineTokenizer.h>
#include <assert.h>
#include <sstream>
#include <iostream>

using namespace std;

static const char* separator = ".*?";

LogLineTokenizer::LogLineTokenizer(std::string _name, std::vector<std::pair<int,std::string>> patternInit) : name{_name} {
	std::string fullPattern = "";

	cout << "constructor" << endl;
	int tokenIndex{0};
	int match{0};
	cout << "constructor2" << endl;
	for(auto token : patternInit) {
		if(token.second != "") {
			cout << "tokenIndex = " << tokenIndex << ", match = " << match << endl;
			fullPattern += separator;
			fullPattern += token.second;
			for(int j{0}; j < token.first; ++j) {
				cout << "stuff" << endl;
				matchMapping[tokenIndex] = match;
				match++;
				tokenIndex++;
			}
		}
		else {
			for(int j{0}; j < token.first; ++j) {
				matchMapping[tokenIndex] = -1;
				tokenIndex++;
			}
		}
	}
	fullPattern += ".*?";

	argc = match;

	pattern = new RE2{ fullPattern };

	assert(pattern->ok());

	for(unsigned int i{0}; i < NUM_TOKENS; ++i) {
		argv[i] = new RE2::Arg();
		*argv[i] = &matches[i];
	}
}

LogLineTokenizer::~LogLineTokenizer() {
	delete pattern;
}

bool LogLineTokenizer::tokenizeLine(re2::StringPiece line, re2::StringPiece res[]) const {
	bool result = false;
	re2::StringPiece s;

	result = RE2::FullMatchN(line,*pattern,argv,argc);

	for(unsigned int i{0}; i < NUM_TOKENS; ++i) {
		if(matchMapping[i] != -1) res[i] = matches[i];
		else res[i].clear();
	}

	return result;
}

std::string LogLineTokenizer::toString() const {
	stringstream ss;

	ss << ">>name" << "," << pattern->pattern() << ",";
	for(auto match : matchMapping) {
		ss << match << ",";
	}
	return ss.str();
}



