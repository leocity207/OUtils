#include <gtest/gtest.h>

#include "include/utils/unowned_ptr.h"

TEST(Unowned_Ptr_Test, integral_type)
{
	O::Unowned_Ptr<int> ptr = nullptr;
	int a = 1;
	ptr = &a;
	EXPECT_EQ(*ptr, a);
	EXPECT_EQ(ptr, &a);
}

TEST(Unowned_Ptr_Test, complex_type)
{
	struct Int {
		int value;
	};
	O::Unowned_Ptr<Int> ptr = nullptr;
	Int a{ 1 };
	ptr = &a;
	EXPECT_EQ(ptr->value, a.value);
	EXPECT_EQ(ptr, &a);
}

TEST(Unowned_Ptr_Test, constructor_from_T)
{
	struct Int {
		int value;
	};
	Int a{ 1 };
	O::Unowned_Ptr<Int> ptr = &a;
	EXPECT_EQ(ptr->value, a.value);
	EXPECT_EQ(ptr, &a);
}


TEST(Unowned_Ptr_Test, constructor_in_vector)
{
	struct Int {
		int value;
	};
	Int a{ 1 };
	std::vector<O::Unowned_Ptr<Int>> ptr;
	O::Unowned_Ptr<Int> b = &a;
	ptr.push_back(&a);
	ptr.push_back(b);
}