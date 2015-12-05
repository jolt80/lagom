#include <gtest/gtest.h>
#include <History.h>
#include <iostream>

using namespace std;

TEST(HistoryTest, test1) {
	History history;

	EXPECT_EQ("",history.getPrevEntry());
	EXPECT_EQ("",history.getPrevEntry());
	EXPECT_EQ("",history.getNextEntry());
	EXPECT_EQ("",history.getNextEntry());


	// Add one entry and verify behaviour
	std::string entry1{"entry1"};
	history.addEntry(entry1);
	EXPECT_EQ(entry1,history.getPrevEntry());
	EXPECT_EQ(entry1,history.getPrevEntry());
	EXPECT_EQ("",history.getNextEntry());
	EXPECT_EQ("",history.getNextEntry());

	// Add one more entry and verify behaviour
	std::string entry2{"entry2"};
	history.addEntry(entry2);
	EXPECT_EQ(entry2,history.getPrevEntry());
	EXPECT_EQ(entry1,history.getPrevEntry());
	EXPECT_EQ(entry2,history.getNextEntry());
	EXPECT_EQ("",history.getNextEntry());

	// Add one more entry and verify behaviour
	std::string entry3{"entry3"};
	history.addEntry(entry3);
	EXPECT_EQ(entry3,history.getPrevEntry());
	EXPECT_EQ(entry2,history.getPrevEntry());
	EXPECT_EQ(entry1,history.getPrevEntry());
	EXPECT_EQ(entry2,history.getNextEntry());
	EXPECT_EQ(entry3,history.getNextEntry());
	EXPECT_EQ("",history.getNextEntry());

};
