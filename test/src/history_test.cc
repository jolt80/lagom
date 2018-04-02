#include "history.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

namespace lagom {

TEST(HistoryTest, test1) {
  string testHistoryFile{".lagomHistory_Test_File"};
  if (remove(testHistoryFile.c_str()) != 0) perror("Error deleting file");

  std::string entry1{"entry1"};
  std::string entry2{"entry2"};
  std::string entry3{"entry3"};

  {
    History history{testHistoryFile};

    EXPECT_EQ("", history.getPrevEntry());
    EXPECT_EQ("", history.getPrevEntry());
    EXPECT_EQ("", history.getNextEntry());
    EXPECT_EQ("", history.getNextEntry());

    // Add one entry and verify behaviour
    history.addEntry(entry1);
    EXPECT_EQ(entry1, history.getPrevEntry());
    EXPECT_EQ(entry1, history.getPrevEntry());
    EXPECT_EQ("", history.getNextEntry());
    EXPECT_EQ("", history.getNextEntry());

    // Add one more entry and verify behaviour
    history.addEntry(entry2);
    EXPECT_EQ(entry2, history.getPrevEntry());
    EXPECT_EQ(entry1, history.getPrevEntry());
    EXPECT_EQ(entry2, history.getNextEntry());
    EXPECT_EQ("", history.getNextEntry());

    // Add one more entry and verify behaviour
    history.addEntry(entry3);
    EXPECT_EQ(entry3, history.getPrevEntry());
    EXPECT_EQ(entry2, history.getPrevEntry());
    EXPECT_EQ(entry1, history.getPrevEntry());
    EXPECT_EQ(entry2, history.getNextEntry());
    EXPECT_EQ(entry3, history.getNextEntry());
    EXPECT_EQ("", history.getNextEntry());
  }

  // Create new history object from same file
  History history{testHistoryFile};

  // Expect to find saved history
  EXPECT_EQ(entry3, history.getPrevEntry());
  EXPECT_EQ(entry2, history.getPrevEntry());
  EXPECT_EQ(entry1, history.getPrevEntry());
  EXPECT_EQ(entry2, history.getNextEntry());
  EXPECT_EQ(entry3, history.getNextEntry());
  EXPECT_EQ("", history.getNextEntry());

  if (remove(testHistoryFile.c_str()) != 0) perror("Error deleting file");
}

}  // namespace lagom
