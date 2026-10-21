#include "pch.h"
#include"Header.h"


TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

// ============================================================
// GROUP 4 - Q1
// Pointer as a Structure Member
// ============================================================

TEST(Group4_Q1, SetWidthStoresPointerIntoStructByReference)
{
    Rectangle r;
    int w = 10;
    setWidth(r, &w);
    // The struct's width member must now point at w.
    EXPECT_EQ(r.width, &w);
}

TEST(Group4_Q1, ComputeAreaDereferencesPointerMember)
{
    Rectangle r;
    int w = 10;
    setWidth(r, &w);
    r.height = 5;
    // area = (*width) * height = 10 * 5 = 50
    EXPECT_EQ(computeArea(r), 50);
}

TEST(Group4_Q1, ComputeAreaReflectsExternalChangeToWidth)
{
    Rectangle r;
    int w = 10;
    setWidth(r, &w);
    r.height = 5;
    EXPECT_EQ(computeArea(r), 50);

    // Mutate the integer through the original variable — the struct
    // must see the new value because it only stores the address.
    w = 25;
    EXPECT_EQ(computeArea(r), 125);   // 25 * 5
}

TEST(Group4_Q1, ComputeAreaReflectsChangeThroughStructPointer)
{
    Rectangle r;
    int w = 4;
    setWidth(r, &w);
    r.height = 6;
    EXPECT_EQ(computeArea(r), 24);   // 4 * 6

    // Modify the integer through the pointer member.
    *(r.width) = 9;
    EXPECT_EQ(computeArea(r), 54);   // 9 * 6
}


// ============================================================
// GROUP 4 - Q2
// Accessing a Structure Through a Pointer — Arrow Operator
// ============================================================
//
//TEST(Group4_Q2, MovePointModifiesCoordinatesViaArrow)
//{
//    Point p{ 3, 4 };
//    movePoint(&p, 5, -2);
//    EXPECT_EQ(p.x, 8);
//    EXPECT_EQ(p.y, 2);
//}
//
//TEST(Group4_Q2, ResetPointZeroesCoordinatesViaArrow)
//{
//    Point p{ 100, 200 };
//    resetPoint(&p);
//    EXPECT_EQ(p.x, 0);
//    EXPECT_EQ(p.y, 0);
//}
//
//TEST(Group4_Q2, DistanceFromOriginUsesArrowOnConstPointer)
//{
//    Point p{ 3, 4 };
//    // 3-4-5 triangle -> distance = 5
//    EXPECT_DOUBLE_EQ(distanceFromOrigin(&p), 5.0);
//}
//
//TEST(Group4_Q2, DistanceFromOriginDoesNotMutatePoint)
//{
//    Point p{ 6, 8 };   // distance = 10
//    EXPECT_DOUBLE_EQ(distanceFromOrigin(&p), 10.0);
//    // const Point* contract: original point unchanged.
//    EXPECT_EQ(p.x, 6);
//    EXPECT_EQ(p.y, 8);
//}
//
//
//// ============================================================
//// GROUP 4 - Q3
//// Rectangle Factory — Returning a Structure
//// ============================================================
//
//TEST(Group4_Q3, MakeRectangleComputesArea)
//{
//    Rectangle r = makeRectangle(4.0, 5.0);
//    EXPECT_DOUBLE_EQ(r.width, 4.0);
//    EXPECT_DOUBLE_EQ(r.height, 5.0);
//    EXPECT_DOUBLE_EQ(r.area, 20.0);
//}
//
//TEST(Group4_Q3, MakeRectangleComputesPerimeter)
//{
//    Rectangle r = makeRectangle(4.0, 5.0);
//    EXPECT_DOUBLE_EQ(r.perimeter, 18.0);
//}
//
//TEST(Group4_Q3, IsLargerReturnsTrueForBiggerArea)
//{
//    Rectangle r1 = makeRectangle(4.0, 5.0);    // area 20
//    Rectangle r2 = makeRectangle(3.0, 3.0);    // area 9
//    EXPECT_TRUE(isLarger(r1, r2));
//}
//
//TEST(Group4_Q3, IsLargerReturnsFalseForSmallerArea)
//{
//    Rectangle r1 = makeRectangle(2.0, 2.0);    // area 4
//    Rectangle r2 = makeRectangle(5.0, 5.0);    // area 25
//    EXPECT_FALSE(isLarger(r1, r2));
//}
//
//
//// ============================================================
//// GROUP 4 - Q4
//// Comparing & Copying Structures
//// ============================================================
//
//TEST(Group4_Q4, IsEqualReturnsTrueWhenAllFieldsMatch)
//{
//    Product a{ "Mouse", 101, 1200.0 };
//    Product b{ "Mouse", 101, 1200.0 };
//    EXPECT_TRUE(isEqual(a, b));
//}
//
//TEST(Group4_Q4, IsEqualReturnsFalseWhenNameDiffers)
//{
//    Product a{ "Mouse",  101, 1200.0 };
//    Product b{ "Keyboard", 101, 1200.0 };
//    EXPECT_FALSE(isEqual(a, b));
//}
//
//TEST(Group4_Q4, IsEqualReturnsFalseWhenIdDiffers)
//{
//    Product a{ "Mouse", 101, 1200.0 };
//    Product b{ "Mouse", 102, 1200.0 };
//    EXPECT_FALSE(isEqual(a, b));
//}
//
//TEST(Group4_Q4, IsEqualReturnsFalseWhenPriceDiffers)
//{
//    Product a{ "Mouse", 101, 1200.0 };
//    Product b{ "Mouse", 101, 1500.0 };
//    EXPECT_FALSE(isEqual(a, b));
//}
//
//TEST(Group4_Q4, IsSameIdReturnsTrueForMatchingIdOnly)
//{
//    Product a{ "Mouse",    101, 1200.0 };
//    Product b{ "Keyboard", 101, 3500.0 };   // different name & price, same id
//    EXPECT_TRUE(isSameId(a, b));
//}
//
//TEST(Group4_Q4, IsSameIdReturnsFalseForDifferentId)
//{
//    Product a{ "Mouse",    101, 1200.0 };
//    Product b{ "Keyboard", 102, 3500.0 };
//    EXPECT_FALSE(isSameId(a, b));
//}
//
//TEST(Group4_Q4, CloneProductReturnsEqualCopy)
//{
//    Product original{ "Monitor", 303, 35000.0 };
//    Product copy = cloneProduct(original);
//    EXPECT_EQ(copy.name, "Monitor");
//    EXPECT_EQ(copy.id, 303);
//    EXPECT_DOUBLE_EQ(copy.price, 35000.0);
//}
//
//TEST(Group4_Q4, CloneCopyIsIndependentOfOriginal)
//{
//    Product original{ "Monitor", 303, 35000.0 };
//    Product copy = cloneProduct(original);
//    // Mutate the copy; the original must be left unchanged.
//    copy.name = "Television";
//    copy.id = 999;
//    copy.price = 10000.0;
//    EXPECT_EQ(original.name, "Monitor");
//    EXPECT_EQ(original.id, 303);
//    EXPECT_DOUBLE_EQ(original.price, 35000.0);
//}
//
//
//// ============================================================
//// GROUP 4 - Q5
//// Structure with a Member Function
//// ============================================================
//
//TEST(Group4_Q5, ComputeAverageReturnsMeanOfFiveMarks)
//{
//    StudentRecord s{ "Ali", { 80, 90, 70, 60, 100 }, 0.0 };
//    // (80 + 90 + 70 + 60 + 100) / 5 = 80
//    EXPECT_DOUBLE_EQ(computeAverage(s), 80.0);
//}
//
//TEST(Group4_Q5, ComputeAverageDoesNotMutateRecord)
//{
//    StudentRecord s{ "Ali", { 80, 90, 70, 60, 100 }, 0.0 };
//    (void)computeAverage(s);
//    // const reference contract: the record's own average field stays at 0
//    // until the caller explicitly stores the result.
//    EXPECT_DOUBLE_EQ(s.average, 0.0);
//}
//
//TEST(Group4_Q5, ComputeAverageHandlesAllZeroMarks)
//{
//    StudentRecord s{ "Zero", { 0, 0, 0, 0, 0 }, 0.0 };
//    EXPECT_DOUBLE_EQ(computeAverage(s), 0.0);
//}
//
//TEST(Group4_Q5, MemberFunctionPrintsNameAndAverage)
//{
//    StudentRecord s{ "Ali", { 80, 90, 70, 60, 100 }, 0.0 };
//    s.average = computeAverage(s);   // 80
//    // Member function is callable on the struct via the dot operator,
//    // takes no arguments, and does not crash. Output goes to stdout.
//    // We cannot easily capture stdout in a portable gtest, so we just
//    // confirm the call compiles and runs.
//    s.printAverage();
//    SUCCEED();
//}
//
//TEST(Group4_Q5, MemberFunctionReflectsUpdatedAverageField)
//{
//    StudentRecord s{ "Ali", { 80, 90, 70, 60, 100 }, 0.0 };
//    // Initially zero; after storing the computed average, the member
//    // function would print the new value.
//    EXPECT_DOUBLE_EQ(s.average, 0.0);
//    s.average = computeAverage(s);
//    EXPECT_DOUBLE_EQ(s.average, 80.0);
//    s.printAverage();
//    SUCCEED();
//}
