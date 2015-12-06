#include <gtest/gtest.h>
#include <Settings.h>
#include <iostream>

using namespace std;

TEST(SettingsTest, test_open_and_read_of_file) {
	std::string settingsTestFile{"settingsTestFile"};

	// Parse settings from file
	Settings parsed{settingsTestFile};

	// Compare with default
//	EXPECT_EQ(def,parsed);
}


TEST(SettingsTest, test_parsing_of_file) {
	std::string settingsTestFile{"settingsTestFile"};
	// Default settings
	Settings def;

	// Parse settings from file
	Settings parsed{settingsTestFile};

	// Compare with default
//	EXPECT_EQ(def,parsed);
}
