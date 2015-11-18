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
	const char* lttngObjAndTraceLevel = "\\w+:(\\w+)";
	const char* cpuId = "{ cpu_id = (\\w+) },";
	const char* processAndObjIf = "\\{ processAndObjIf = \"(.*?)\\((.*?)\\)\",";
	const char* fileAndLine = "fileAndLine = \".*?(\\w+\\.\\w+:\\d+)\",";
	const char* msg = "msg = \"\\s*(.*)\" }";
	const char* lttngMsg = "\\{ (.*) \\}";

	std::vector<std::pair<int,std::string>> triRegex;

	triRegex.push_back(std::pair<int,std::string>(1,timeStr));
	triRegex.push_back(std::pair<int,std::string>(1,timeDiffStr));
	triRegex.push_back(std::pair<int,std::string>(1,card));
	triRegex.push_back(std::pair<int,std::string>(1,triTraceLevel));
	triRegex.push_back(std::pair<int,std::string>(1,cpuId));
	triRegex.push_back(std::pair<int,std::string>(2,processAndObjIf));
	triRegex.push_back(std::pair<int,std::string>(1,fileAndLine));
	triRegex.push_back(std::pair<int,std::string>(1,msg));

	std::vector<std::pair<int,std::string>> baseLttngRegex;
	baseLttngRegex.push_back(std::pair<int,std::string>(1,timeStr));
	baseLttngRegex.push_back(std::pair<int,std::string>(1,timeDiffStr));
	baseLttngRegex.push_back(std::pair<int,std::string>(1,card));
	baseLttngRegex.push_back(std::pair<int,std::string>(1,lttngObjAndTraceLevel));
	baseLttngRegex.push_back(std::pair<int,std::string>(1,cpuId));
	baseLttngRegex.push_back(std::pair<int,std::string>(3,std::string{}));
	baseLttngRegex.push_back(std::pair<int,std::string>(1,lttngMsg));

	LogLineTokenizer* tri = new LogLineTokenizer{"TRI", triRegex};
	tokenizers.push_back(tri);

	LogLineTokenizer* baseLttng = new LogLineTokenizer{"BaseLttng", baseLttngRegex};
	tokenizers.push_back(baseLttng);

}

Settings::~Settings() {
	// TODO Auto-generated destructor stub
}
