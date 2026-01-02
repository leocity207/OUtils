#include <gtest/gtest.h>

#include "utils/comb.h"

#include <vector>
#include <forward_list>

using namespace O::Comb;

// Helper to collect any range into a vector for easy assertions
template <typename R, typename T = std::ranges::range_value_t<std::remove_reference_t<R>>>
std::vector<T> Collect_To_Vector(R&& range)
{
	std::vector<T> out;
	for (auto&& e : range) out.push_back(e);
	return out;
}

using namespace O::Comb;

TEST(Comb_Test, Even_Vector_Basic)
{
	std::vector<int> v{0,1,2,3,4,5,6,7};

	auto even = Even(v);
	auto even_vector = Collect_To_Vector(even);

	std::vector<int> expected_even{0,2,4,6};

	EXPECT_EQ(even_vector, expected_even);
}

TEST(Comb_Test, Odd_Vector_Basic)
{
	std::vector<int> v{0,1,2,3,4,5,6,7};

	auto odd  = Odd(v);
	auto odd_vector = Collect_To_Vector(odd);


	std::vector<int> expected_odd {1,3,5,7};

	EXPECT_EQ(odd_vector, expected_odd);
}

TEST(Comb_Test, Make_Comb_Width_3_Shift_1)
{
	std::vector<int> v{0,1,2,3,4,5,6,7,8};
	auto comb = Make_Comb<3,1>(v); // indices 1,4,7,...
	auto comb_vector = Collect_To_Vector(comb);

	std::vector<int> expected{1,4,7};
	EXPECT_EQ(comb_vector, expected);
}

TEST(Comb_Test, Empty_Container)
{
	std::vector<int> v;
	auto even = Even(v);
	EXPECT_EQ(even.begin(), even.end());
	if constexpr (std::ranges::sized_range<decltype(v)>) {
		EXPECT_EQ(even.size(), 0u);
	}
}

TEST(Comb_Test, Single_Element_Behavior)
{
	std::vector<int> v{42};

	auto even = Even(v);
	auto odd  = Odd(v);

	// Even: index 0 exists
	ASSERT_NE(even.begin(), even.end());
	EXPECT_EQ(*even.begin(), 42);

	// Odd: index 1 doesn't exist -> empty
	EXPECT_EQ(odd.begin(), odd.end());
}

TEST(Comb_Test, Const_Container_Iteration)
{
	const std::vector<int> cv{10,11,12,13};
	auto even_view = Even(cv);
	auto even_view_vector = Collect_To_Vector(even_view);

	std::vector<int> expected{10,12};
	EXPECT_EQ(even_view_vector, expected);
}

TEST(Comb_Test, Forward_List_Works)
{
	std::forward_list<int> fl{0,1,2,3,4};
	auto ev = Even(fl); // selects 0,2,4
	auto ev_vector = Collect_To_Vector(ev);

	std::vector<int> expected{0,2,4};
	EXPECT_EQ(ev_vector, expected);

	// forward_list isn't sized_range: size() should not participate in overload resolution,
	// so we don't attempt to call size() here.
}

TEST(Comb_Test, Size_Computation_And_Edge_Shift)
{
	std::vector<int> v{0,1}; // n = 2
	// width = 3, shift = 2 -> no selected elements because Shift >= n
	auto c = Make_Comb<3,2>(v);
	// sized_range -> size() must be present and be 0
	static_assert(std::ranges::sized_range<decltype(v)>);
	EXPECT_EQ(c.size(), 0u);
	EXPECT_EQ(c.begin(), c.end());
}

TEST(Comb_Test, Random_Access_Iterator_Enhancements)
{
	std::vector<int> v{0,1,2,3,4,5,6,7};
	auto ev = Even(v); // selects indices 0,2,4,6 -> values 0,2,4,6

	auto it = ev.begin();
	// check operator[] (random-access only)
	// it[0] == 0, it[1] == 2, it[2] == 4, it[3] == 6
	EXPECT_EQ(it[0], 0);
	EXPECT_EQ(it[1], 2);
	EXPECT_EQ(it[2], 4);
	EXPECT_EQ(it[3], 6);

	// operator+= stepping by logical elements (not underlying increments)
	auto it2 = ev.begin();
	it2 += 2; // should point to value 4
	EXPECT_EQ(*it2, 4);

	// difference operator (it2 - it) should be 2
	auto it3 = ev.begin();
	it3 += 3; // logical index 3 -> value 6
	EXPECT_EQ((it3 - it), 3);
}
