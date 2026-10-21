#include "pch.h"
#include"Header.h"
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(Group4_Q1, ThreeOverloadsOfRecordReading)
{
    Sensor s(1, 0.5);
    s.recordReading(25.0);
    s.recordReading(26.0, 1000);
    s.recordReading(27.0, 'G');   // good quality
    EXPECT_NEAR(s.getReading(), 27.0, 1e-6);
}

TEST(Group4_Q1, CopyConstructorMakesDeepCopyOfHistory)
{
    Sensor a(1, 0.0);
    a.recordReading(10.0);
    a.recordReading(20.0);
    Sensor b = a;
    b.recordReading(30.0);
    EXPECT_NEAR(a.getReading(), 20.0, 1e-6);
    EXPECT_NEAR(b.getReading(), 30.0, 1e-6);
}

TEST(Group4_Q1, DestructorDoesNotAffectCopy)
{
    Sensor* a = new Sensor(1, 0.0);
    a->recordReading(99.0);
    Sensor b = *a;
    delete a;
    EXPECT_NEAR(b.getReading(), 99.0, 1e-6);
}