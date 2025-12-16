// test_bounded_array.cpp
#include <gtest/gtest.h>

#include "utils/bounded_array.h"

#include <numeric>
#include <string>

using namespace std;

// A small helper type to track constructions/destructions and check in-place semantics.
struct Trackable {
    static int constructions;
    static int destructions;

    int value;
    explicit Trackable(int v) : value(v) { ++constructions; }
    Trackable(const Trackable& o) : value(o.value) { ++constructions; }
    Trackable(Trackable&& o) noexcept : value(o.value) { ++constructions; }
    ~Trackable() { ++destructions; }
};

int Trackable::constructions = 0;
int Trackable::destructions  = 0;

// Helper to reset counters
static void ResetTrackableCounters() {
    Trackable::constructions = 0;
    Trackable::destructions  = 0;
}

/* ------------------------
   Basic state / capacity
   ------------------------ */
TEST(Bounded_Vector, Default_Constructed_Empty)
{
    O::Bounded_Vector<int, 8> v;
    EXPECT_EQ(v.Capacity(), 8u);
    EXPECT_EQ(v.Size(), 0u);
    EXPECT_TRUE(v.Empty());
    EXPECT_FALSE(v.Full());
}

// Fill to capacity and check Full/Empty/Size
TEST(Bounded_Vector, Fill_And_Full)
{
    O::Bounded_Vector<int, 3> v;
    v.Push_Back(10);
    v.Push_Back(20);
    v.Push_Back(30);

    EXPECT_EQ(v.Size(), 3u);
    EXPECT_TRUE(v.Full());
    EXPECT_FALSE(v.Empty());

    // iterators and std::accumulate
    int sum = std::accumulate(v.begin(), v.end(), 0);
    EXPECT_EQ(sum, 60);
}

TEST(Bounded_Vector, Initializer_List)
{
    O::Bounded_Vector<size_t, 3> v{ 1,2,3 };
    EXPECT_EQ(v.Size(), 3u);
}

/* ------------------------
   Element access
   ------------------------ */
TEST(Bounded_Vector, Operator_Bracket_And_At)
{
    O::Bounded_Vector<std::string, 4> v;
    v.Push_Back(std::string("a"));
    v.Push_Back(std::string("bc"));

    // operator[] is unchecked (no throw)
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "bc");

    // At() should throw on out-of-range
    EXPECT_THROW(v.At(5), O::Bounded_Vector_Exception);

    // valid At
    EXPECT_NO_THROW({
        auto &s = v.At(1);
        EXPECT_EQ(s, "bc");
    });
}

/* ------------------------
   Modifiers & exceptions
   ------------------------ */
TEST(Bounded_Vector, Push_Emplace_Pop_Clear_And_Exceptions)
{
    O::Bounded_Vector<int, 2> v;
    EXPECT_THROW({
        // popping empty -> exception
        v.Pop_Back();
    }, typename O::Bounded_Vector_Exception);

    v.Push_Back(1);
    v.Push_Back(2);
    EXPECT_TRUE(v.Full());

    // pushing past capacity -> exception
    try {
        v.Push_Back(3);
        FAIL() << "Expected push to throw";
    } catch (const O::Bounded_Vector_Exception& ex) {
        EXPECT_EQ(ex, O::Bounded_Vector_Exception::NO_MORE_PLACE_TO_EMPLACE);
    }

    // Pop then Push works
    v.Pop_Back();
    EXPECT_EQ(v.Size(), 1u);
    v.Push_Back(99);
    EXPECT_EQ(v.Size(), 2u);

    // Clear
    v.Clear();
    EXPECT_EQ(v.Size(), 0u);
    EXPECT_TRUE(v.Empty());
}

/* ------------------------
   Emplace_Back with arguments
   ------------------------ */
TEST(Bounded_Vector, Emplace_Back_Constructs_In_Place)
{
    ResetTrackableCounters();
    {
        O::Bounded_Vector<Trackable, 4> v;
        EXPECT_EQ(Trackable::constructions, 0);
        v.Emplace_Back(5); // construct Trackable(5)
        EXPECT_EQ(Trackable::constructions, 1);
        v.Emplace_Back(6);
        EXPECT_EQ(Trackable::constructions, 2);

        EXPECT_EQ(v.Size(), 2u);
        EXPECT_EQ(v[0].value, 5);
        EXPECT_EQ(v[1].value, 6);
    }
    // destructor called for both elements when v goes out of scope
    EXPECT_EQ(Trackable::destructions, Trackable::constructions);
}

/* ------------------------
   Copy and move semantics
   ------------------------ */
TEST(Bounded_Vector, Copy_Constructor_And_Assignment)
{
    O::Bounded_Vector<std::string, 5> a;
    a.Push_Back("one");
    a.Push_Back("two");

    // copy construct
    O::Bounded_Vector<std::string, 5> b(a);
    EXPECT_EQ(b.Size(), a.Size());
    EXPECT_EQ(b[0], "one");
    EXPECT_EQ(b[1], "two");

    // copy assign
    O::Bounded_Vector<std::string, 5> c;
    c = a;
    EXPECT_EQ(c.Size(), 2u);
    EXPECT_EQ(c[1], "two");
}

TEST(Bounded_Vector, Move_Constructor_Leaves_Source_Empty)
{
    O::Bounded_Vector<int, 5> a;
    a.Push_Back(11);
    a.Push_Back(22);

    O::Bounded_Vector<int, 5> b(std::move(a));
    EXPECT_EQ(b.Size(), 2u);
    EXPECT_EQ(b[0], 11);
    EXPECT_EQ(b[1], 22);

    // moved-from should be empty
    EXPECT_EQ(a.Size(), 0u);

    // move assign
    O::Bounded_Vector<int, 5> c;
    c = std::move(b);
    EXPECT_EQ(c.Size(), 2u);
    EXPECT_EQ(b.Size(), 0u);
}

/* ------------------------
   Data pointer and iterator correctness
   ------------------------ */
TEST(Bounded_Vector, Data_Pointer_And_Range)
{
    O::Bounded_Vector<int, 6> v;
    v.Push_Back(1);
    v.Push_Back(2);
    v.Push_Back(3);

    EXPECT_EQ(&v[0], v.Data());
    EXPECT_EQ(v.begin() + 3, v.end());
    EXPECT_EQ(std::distance(v.begin(), v.end()), static_cast<std::ptrdiff_t>(v.Size()));
}

/* ------------------------
   Over-capacity constructor behavior (variadic)
   ------------------------ */
TEST(Bounded_Vector, Variadic_Constructor_Rejects_Too_Many)
{
    // The constructor is explicit and variadic: Bounded_Vector<int,2>(1,2) ok, (1,2,3) should be a compile-time static_assert
    // but at runtime we test correct construction for a normal case.
    O::Bounded_Vector<int, 4> v(10, 20, 30);
    EXPECT_EQ(v.Size(), 3u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

/* ------------------------
   Exception payload content tests
   ------------------------ */
TEST(Bounded_Vector, Exception_Types_Are_Correct)
{
    O::Bounded_Vector<int, 1> v;
    // Pop on empty
    try {
        v.Pop_Back();
        FAIL() << "Expected pop to throw";
    } catch (const O::Bounded_Vector_Exception& ex) {
        EXPECT_EQ(ex, O::Bounded_Vector_Exception::NO_MORE_ELEMENT_TO_POP);
    }

    // push to fill and then push again
    v.Push_Back(42);
    try {
        v.Push_Back(43);
        FAIL() << "Expected push to throw";
    } catch (const O::Bounded_Vector_Exception& ex) {
        EXPECT_EQ(ex, O::Bounded_Vector_Exception::NO_MORE_PLACE_TO_EMPLACE);
    }

    // At out-of-range
    try {
        v.At(5);
        FAIL() << "Expected At to throw";
    } catch (const O::Bounded_Vector_Exception& ex) {
        EXPECT_EQ(ex, O::Bounded_Vector_Exception::OUT_OF_RANGE);
    }
}