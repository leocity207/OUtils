// test_expected.cpp
#include <gtest/gtest.h>

#include "include/utils/zip.h"

#include <vector>
#include <array>
#include <list>
#include <algorithm>
#include <numeric>

// Test Zip_View iteration and modification through proxy (get0/get1)
TEST(Zip_Test, Zip_View_Iterates_And_Allows_Modification)
{
	std::vector<int> a{1,2,3};
	std::vector<int> b{10,20,30};

	// Use the concrete implementation type directly
	O::Zip_View<std::vector<int>, std::vector<int>> zip(a, b);

	EXPECT_EQ(zip.size(), 3u);

	// iterate using iterator and ensure proxy exposes elements
	std::size_t idx = 0;
	for (auto it = zip.begin(); it != zip.end(); ++it) {
		auto proxy = *it; // proxy holds pointers to elements
		// check values
		EXPECT_EQ(&proxy.first, &a[idx]);
		EXPECT_EQ(&proxy.second, &b[idx]);

		// modify first component through proxy
		proxy.first *= 2;
		++idx;
	}

	// a elements have been modified through proxy
	EXPECT_EQ(a[0], 2);
	EXPECT_EQ(a[1], 4);
	EXPECT_EQ(a[2], 6);

	// ensure b unchanged
	EXPECT_EQ(b[0], 10);
	EXPECT_EQ(b[1], 20);
	EXPECT_EQ(b[2], 30);
}

TEST(Zip_Test, Zip_View_Iterates_As_Used)
{
	std::vector<int> a{ 1,2,3 };
	std::vector<int> b{ 10,20,30 };

	// Use the concrete implementation type directly
	O::Zip_View<std::vector<int>, std::vector<int>> zip(a, b);

	EXPECT_EQ(zip.size(), 3u);

	// iterate using iterator and ensure proxy exposes elements
	std::size_t idx = 0;
	for (auto&& [a_elt,b_elt] : O::Zip(a, b)) {
		// check values
		EXPECT_EQ(&a_elt, &a[idx]);
		EXPECT_EQ(&b_elt, &b[idx]);

		// modify first component through proxy
		a_elt *= 2;
		++idx;
	}

	// a elements have been modified through proxy
	EXPECT_EQ(a[0], 2);
	EXPECT_EQ(a[1], 4);
	EXPECT_EQ(a[2], 6);

	// ensure b unchanged
	EXPECT_EQ(b[0], 10);
	EXPECT_EQ(b[1], 20);
	EXPECT_EQ(b[2], 30);
}

// Test ZipIndexView iteration, index correctness and ability to modify underlying container
TEST(Zip_Test, Zip_Index_View_Iterates_And_Reports_Index)
{
	std::vector<char> v{'x','y','z'};
	O::Zip_Index_View<std::vector<char>> zid(v);

	EXPECT_EQ(zid.size(), 3u);

	std::size_t count = 0;
	for (auto it = zid.begin(); it != zid.end(); ++it) {
		auto proxy = *it;
		// get0 returns element reference, get1 returns index reference
		char& elem_ref = proxy.p;
		std::size_t& idx_ref = proxy.idx;

		EXPECT_EQ(idx_ref, count);
		// modify element via proxy
		elem_ref = static_cast<char>('A' + static_cast<int>(idx_ref));
		++count;
	}

	EXPECT_EQ(v[0], 'A');
	EXPECT_EQ(v[1], 'B');
	EXPECT_EQ(v[2], 'C');
}

TEST(Zip_Test, Zip_Adjacent_Circular__As_Used)
{
	std::vector<int> a{ 1,1,1,1,1,1 };

	// iterate using iterator and ensure proxy exposes elements
	int filter = 0;
	for (auto&& [i,i_1] : O::Zip_Adjacent_Circular(a))
	{
		// check values
		EXPECT_EQ(i, i_1);
		filter = i_1 - i;
	}
	EXPECT_EQ(filter,0);
}


TEST(Zip_Test, Zip_Index_View_As_Used)
{
	std::vector<char> v{ 'x','y','z' };
	O::Zip_Index_View<std::vector<char>> zid(v);

	EXPECT_EQ(zid.size(), 3u);

	std::size_t count = 0;
	for (auto&& [elem_ref, idx_ref] : O::Zip_Index(v)) {

		EXPECT_EQ(idx_ref, count);
		// modify element via proxy
		elem_ref = static_cast<char>('A' + static_cast<int>(idx_ref));
		++count;
	}

	EXPECT_EQ(v[0], 'A');
	EXPECT_EQ(v[1], 'B');
	EXPECT_EQ(v[2], 'C');
}


// For_Each_Indexed: contiguous path (vector) modifies underlying elements
TEST(Zip_Test, For_Each_Indexed_Contiguous_Vector)
{
	std::vector<int> v(5, 0); // 5 zeros

	O::For_Each_Indexed(v, [](int& elem, std::size_t i) {
		// set to i+1
		elem = static_cast<int>(i + 1);
	});

	for (std::size_t i = 0; i < v.size(); ++i)
		EXPECT_EQ(v[i], static_cast<int>(i + 1));
}

// For_Each_Indexed: non-contiguous path (list) — verify indices and values observed
TEST(Zip_Test, For_Each_Indexed_Non_Contiguous_List)
{
	std::list<int> l = {10, 20, 30, 40};
	std::vector<std::pair<int, std::size_t>> seen;
	O::For_Each_Indexed(l, [&seen](int& elem, std::size_t i) {
		seen.emplace_back(elem, i);
	});

	ASSERT_EQ(seen.size(), 4u);
	EXPECT_EQ(seen[0], std::make_pair(10, std::size_t{0}));
	EXPECT_EQ(seen[1], std::make_pair(20, std::size_t{1}));
	EXPECT_EQ(seen[2], std::make_pair(30, std::size_t{2}));
	EXPECT_EQ(seen[3], std::make_pair(40, std::size_t{3}));
}

// For_Each_Zip: contiguous-contiguous fast path (vector + array) modifies first container using second
TEST(Zip_Test, For_Each_Zip_Contiguous_Contiguous) 
{
	std::vector<int> a{1,2,3};
	std::array<int,3> b{10,20,30};

	O::For_Each_Zip(a, b, [](int& x, int& y) {
		x += y;
	});

	EXPECT_EQ(a[0], 11);
	EXPECT_EQ(a[1], 22);
	EXPECT_EQ(a[2], 33);
}

// For_Each_Zip: generic iterator path (vector + list) — ensure iteration stops at shortest
TEST(Zip_Test, For_Each_Zip_Generic_Iterator_Path_Stops_At_Shortest)
{
	std::vector<int> a{2,4,6,8};
	std::list<int> b{1,3,5}; // shorter than a

	std::vector<int> results;
	O::For_Each_Zip(a, b, [&results](int& x, int& y){
		results.push_back(x * y);
	});

	// should iterate only 3 times (the shorter of the two)
	ASSERT_EQ(results.size(), 3u);
	EXPECT_EQ(results[0], 2 * 1);
	EXPECT_EQ(results[1], 4 * 3);
	EXPECT_EQ(results[2], 6 * 5);

	// a retains original contents for elements not visited
	EXPECT_EQ(a[3], 8);
}