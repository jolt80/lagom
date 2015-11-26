/*
 * Settings.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: jolt
 */

#include <Settings.h>

Settings::Settings() {
	const char* timeStr = "\\[\\d{4}-\\d{2}-\\d{2}\\s*(\\d{1,2}:\\d{1,2}:\\d{1,2}\\.\\d{3}).*?\\]";
	const char* timeDiffStr = "\\((\\+\\d+\\.\\d+)\\)";
	const char* card = "(\\w+)";
	const char* triTraceLevel = "com_ericsson_triobjif:(.*?):";
	const char* lttngObjAndTraceLevel = "\\w+:(\\w+):";
	const char* cpuId = "{ cpu_id = (\\w+) },";
	const char* processAndObjIf = "\\{ processAndObjIf = \"(.*?)\\((.*?)\\)\",";
	const char* fileAndLine = "fileAndLine = \".*?(\\w+\\.\\w+):(\\d+)\",";
	const char* msg = "msg = \"\\s*(.*)\" }";
	const char* lttngMsg = "\\{ (.*) \\}";
	const char* separator = "\\s*";

	std::string lttngPrefixPattern;
	lttngPrefixPattern += timeStr;
	lttngPrefixPattern += separator;
	lttngPrefixPattern += timeDiffStr;
	lttngPrefixPattern += separator;
	lttngPrefixPattern += card;
	lttngPrefixPattern += separator;
	lttngPrefixPattern += lttngObjAndTraceLevel;
	lttngPrefixPattern += separator;
	lttngPrefixPattern += cpuId;

	std::vector<std::string> tokenPatterns;
	tokenPatterns.push_back(fileAndLine);
	tokenPatterns.push_back(msg);

	LogLineTokenizer* lttngBase = new LogLineTokenizer{"LttngBase",lttngPrefixPattern,tokenPatterns};
	tokenizers.push_back(lttngBase);

	tokenWidths.push_back(5);
	tokenWidths.push_back(12);
	tokenWidths.push_back(12);
	tokenWidths.push_back(3);
	tokenWidths.push_back(6);
	tokenWidths.push_back(2);
	tokenWidths.push_back(2);
	tokenWidths.push_back(20);
	tokenWidths.push_back(200);
	tokenWidths.push_back(200);


}

Settings::~Settings() {
	for(auto tokenizer : tokenizers) delete tokenizer;
}

int Settings::getWidth(int tokenIndex) const {
	return tokenWidths.at(tokenIndex);
}
