// test_expected.cpp
#include <gtest/gtest.h>

#include "include/utils/expected.h"

#include "expected_utils_test.h"


TEST(Expected_Test, Default_Constructed_Is_Empty)
{
	O::Expected<int, std::string> e;
	EXPECT_FALSE(e.Has_Value());
	EXPECT_FALSE(e.Has_Error());
	EXPECT_FALSE(static_cast<bool>(e));
	EXPECT_THROW(e.Value(), O::bad_expected_access);
	EXPECT_THROW(e.Error(), O::bad_expected_access);
}

TEST(Expected_Test, Value_Construction_And_Access)
{
	O::Expected<int, std::string> e(42);
	EXPECT_TRUE(e.Has_Value());
	EXPECT_FALSE(e.Has_Error());
	EXPECT_TRUE(static_cast<bool>(e));
	EXPECT_EQ(e.Value(), 42);

	const O::Expected<int, std::string> ce(100);
	EXPECT_EQ(ce.Value(), 100);
}

TEST(Expected_Test, Error_Construction_And_Access)
{
	O::Expected<int, std::string> e(std::string("err"));
	EXPECT_FALSE(e.Has_Value());
	EXPECT_TRUE(e.Has_Error());
	EXPECT_FALSE(static_cast<bool>(e));
	EXPECT_EQ(e.Error(), "err");

	const O::Expected<int, std::string> ce(std::string("const_err"));
	EXPECT_EQ(ce.Error(), "const_err");
}

TEST(Expected_Test, Make_Value)
{
	auto ev = O::Expected<TrackValue, TrackError>::Make_Value(7);
	EXPECT_TRUE(ev.Has_Value());
	EXPECT_EQ(ev.Value().v, 7);
}

TEST(Expected_Test, Make_Error)
{
	auto ee = O::Expected<TrackValue, TrackError>::Make_Error(std::string("boom"));
	EXPECT_TRUE(ee.Has_Error());
	EXPECT_EQ(ee.Error().msg, "boom");
}

TEST(Expected_Test, Emplace_Value_And_Error)
{
	O::Expected<TrackValue, TrackError> e;
	EXPECT_FALSE(e.Has_Value());
	e.Emplace_Value(55);
	EXPECT_TRUE(e.Has_Value());
	EXPECT_EQ(e.Value().v, 55);

	e.Emplace_Error(std::string("oops"));
	EXPECT_TRUE(e.Has_Error());
	EXPECT_EQ(e.Error().msg, "oops");

	// Re-emplace value on existing error
	e.Emplace_Value(3);
	EXPECT_TRUE(e.Has_Value());
	EXPECT_EQ(e.Value().v, 3);
}

TEST(Expected_Test, Move_Constructor_And_Move_Assignment)
{
	TrackValue::Reset();
	TrackError::Reset();

	auto a = O::Expected<TrackValue, TrackError>::Make_Value(123);
	EXPECT_TRUE(a.Has_Value());
	EXPECT_EQ(TrackValue::constructions, 1); // construction of TrackValue(123)

	// Move-construct b from a
	auto b = std::move(a);
	EXPECT_TRUE(b.Has_Value());
	EXPECT_FALSE(a.Has_Value()); // a destroyed/moved-from -> None
	EXPECT_GE(TrackValue::moves + TrackValue::copies, 1);

	// Move-assign c from b
	O::Expected<TrackValue, TrackError> c;
	c = std::move(b);
	EXPECT_TRUE(c.Has_Value());
	EXPECT_FALSE(b.Has_Value());
}

TEST(Expected_Test, Value_And_Error_RValue_Accessors_Move)
{
	TrackValue::Reset();
	TrackError::Reset();

	// Test Value() && (rvalue) moves-out the contained T into the returned object
	auto ev = O::Expected<TrackValue, TrackError>::Make_Value(999);
	EXPECT_TRUE(ev.Has_Value());
	int moves_before = TrackValue::moves;
	TrackValue v = std::move(ev).Value(); // calls Value() && -> move from internal T
	EXPECT_GE(TrackValue::moves, moves_before + 1);
	EXPECT_EQ(v.v, 999);

	// Test Error() && similarly
	auto ee = O::Expected<TrackValue, TrackError>::Make_Error(std::string("errx"));
	EXPECT_TRUE(ee.Has_Error());
	int err_moves_before = TrackError::moves;
	TrackError err = std::move(ee).Error();
	EXPECT_GE(TrackError::moves, err_moves_before + 1);
	EXPECT_EQ(err.msg, "errx");
}

TEST(Expected_Test, Throws_When_Accessing_Missing_Variant)
{
	O::Expected<int, std::string> ev(10);
	EXPECT_NO_THROW(ev.Value());
	EXPECT_THROW(ev.Error(), O::bad_expected_access);

	O::Expected<int, std::string> ee(std::string("err"));
	EXPECT_NO_THROW(ee.Error());
	EXPECT_THROW(ee.Value(), O::bad_expected_access);
}

TEST(Expected_Test, Swap_Between_Value_And_Error_And_Values)
{
	// swap value <-> error
	O::Expected<int, std::string> a(1);
	O::Expected<int, std::string> b(std::string("err"));
	a.Swap(b);
	EXPECT_TRUE(a.Has_Error());
	EXPECT_TRUE(b.Has_Value());
	EXPECT_EQ(b.Value(), 1);
	EXPECT_EQ(a.Error(), "err");

	// swap two values
	O::Expected<int, std::string> v1(11), v2(22);
	v1.Swap(v2);
	EXPECT_EQ(v1.Value(), 22);
	EXPECT_EQ(v2.Value(), 11);
}

TEST(Expected_Test, Const_Value_Accessor)
{
	const auto ev = O::Expected<int, std::string>(77);
	static_cast<void>(ev);
	EXPECT_EQ(ev.Value(), 77);
}

TEST(Expected_Test, Move_Leaves_Source_None)
{
	auto a = O::Expected<int, std::string>::Make_Value(5);
	auto b = std::move(a);
	// moved-from "a" should be empty (Move_From calls other.Destroy())
	EXPECT_FALSE(a.Has_Value());
	EXPECT_FALSE(a.Has_Error());
}