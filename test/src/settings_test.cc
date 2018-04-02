#include "settings.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

namespace lagom {

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

  cout << def << endl;

  // Parse settings from file
  Settings parsed{settingsTestFile};

  cout << parsed << endl;

  // Compare with default
  EXPECT_EQ(def, parsed);
}

TEST(SettingsTest, test_creation_of_default_settings_file) {
  std::string settingsTestFile{"default_settingsTestFile"};
  // Default settings
  Settings def;

  cout << def << endl;

  Settings::writeDefaultSettingsFile(settingsTestFile);

  // Parse settings from file
  Settings parsed{settingsTestFile};

  cout << parsed << endl;

  // Compare with default
  EXPECT_EQ(def, parsed);
}

}  // namespace lagom
