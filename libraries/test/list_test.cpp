#include "gtest/gtest.h"
#include "list.h"
#include <iostream>

TEST(ListTest, push) {
		auto list = new List<int>();
		list->push(1);
		auto i = list->begin();
		ASSERT_EQ(i.value(), 1);
		EXPECT_FALSE(i.end());
		++i;
		EXPECT_TRUE(i.end());
}

TEST(ListTest, iteratorIncrementOnEmptyList) {
		auto list = new List<int>();
		auto i = list->begin();
		EXPECT_TRUE(i.end());
		++i;
		EXPECT_TRUE(i.end());
}

TEST(ListTest, iteratorIncrement) {
		auto list = new List<int>();
		list->push(1);
		list->push(2);
		auto i = list->begin();
		ASSERT_EQ(i.value(), 2);
		EXPECT_FALSE(i.end());
		++i;
		ASSERT_EQ(i.value(), 1);
		EXPECT_FALSE(i.end());
		++i;
		EXPECT_TRUE(i.end());
}

TEST(ListTest, iteration) {
		auto list = new List<int>();
		list->push(1);
		list->push(2);
		list->push(3);
		int a[] = {0,0,0};
		int j = 0;
		for(auto i = list->begin(); i.more(); ++i)
			a[j++] = i.value();
		ASSERT_EQ(a[0], 3);
		ASSERT_EQ(a[1], 2);
		ASSERT_EQ(a[2], 1);
}

TEST(ListTest, removeFromTheMiddle) {
		auto list = new List<int>();
		list->push(3);
		list->push(2);
		list->push(1);
		auto i = list->begin();
		++i;
		i.remove(); // remove the 2;
		ASSERT_EQ(i.value(), 3);
		++i;
		EXPECT_TRUE(i.end());
		i = list->begin();
		ASSERT_EQ(i.value(), 1);
		++i;
		ASSERT_EQ(i.value(), 3);
}
