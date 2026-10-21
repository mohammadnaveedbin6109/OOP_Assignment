#include "pch.h"
#include"Header.h"
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

//TEST(Group1_Q1, CategoryMultiplierAppliedToBasePrice)
//{
//    Ticket vip;
//    vip.setDetails(5, 100.0, "VIP");
//    EXPECT_DOUBLE_EQ(vip.getFinalPrice(), 150.0);
//
//    Ticket regular;
//    regular.setDetails(5, 100.0, "Regular");
//    EXPECT_DOUBLE_EQ(regular.getFinalPrice(), 100.0);
//}
//
//TEST(Group1_Q1, MultipleDiscountsStackMultiplicatively)
//{
//    Ticket t;
//    t.setDetails(5, 100.0, "Regular");   // adjusted = 100
//    t.applyDiscount(20.0);               // 100 * 0.8 = 80
//    t.applyDiscount(50.0);               // 80  * 0.5 = 40
//    EXPECT_DOUBLE_EQ(t.getFinalPrice(), 40.0);
//}
//
//TEST(Group1_Q1, DiscountCannotPushBelowTenPercentFloor)
//{
//    Ticket t;
//    t.setDetails(5, 100.0, "VIP");       // category-adjusted = 150, floor = 15
//    t.applyDiscount(95.0);               // 150 * 0.05 = 7.5 -> clamped to 15
//    EXPECT_DOUBLE_EQ(t.getFinalPrice(), 15.0);
//}
//
//TEST(Group1_Q1, RefundSucceedsOnceWithinWindowThenPermanentlyFails)
//{
//    Ticket t;
//    t.setDetails(5, 100.0, "Regular");
//    EXPECT_TRUE(t.refund(20));           // first refund within 30-min window
//    EXPECT_TRUE(t.isRefunded());
//    EXPECT_FALSE(t.refund(10));          // second refund fails even with small minutes
//    EXPECT_TRUE(t.isRefunded());         // still marked as refunded
//}
//
//TEST(Group1_Q1, RefundFailsAtOrAfterThirtyMinuteWindow)
//{
//    Ticket t;
//    t.setDetails(5, 100.0, "Regular");
//    EXPECT_FALSE(t.refund(30));          // exactly at 30 min - rejected
//    EXPECT_FALSE(t.refund(45));          // after 30 min - rejected
//    EXPECT_FALSE(t.isRefunded());
//}

//TEST(Group1_Q2, ConstructorSetsDueDateLoanDaysFromDayZero)
//{
//    Book b("Test", 14);
//    EXPECT_EQ(b.daysOverdue(14), 0);     // at due date - not overdue
//    EXPECT_EQ(b.daysOverdue(15), 1);     // 1 day overdue
//}
//
//TEST(Group1_Q2, RenewPushesDueDateForwardByExtraDays)
//{
//    Book b("Test", 14);
//    ASSERT_TRUE(b.renew(7));             // due day becomes 21
//    EXPECT_EQ(b.daysOverdue(20), 0);
//    EXPECT_EQ(b.daysOverdue(22), 1);
//}
//
//TEST(Group1_Q2, ThirdRenewAttemptFailsAndChangesNothing)
//{
//    Book b("Test", 14);
//    EXPECT_TRUE(b.renew(7));             // due day = 21
//    EXPECT_TRUE(b.renew(7));             // due day = 28
//    EXPECT_FALSE(b.renew(7));            // third attempt rejected
//    EXPECT_EQ(b.daysOverdue(28), 0);     // still due at 28
//    EXPECT_EQ(b.daysOverdue(29), 1);
//}
//
//TEST(Group1_Q2, DaysOverdueNeverReturnsNegative)
//{
//    Book b("Test", 14);
//    EXPECT_EQ(b.daysOverdue(0), 0);
//    EXPECT_EQ(b.daysOverdue(10), 0);
//    EXPECT_EQ(b.daysOverdue(14), 0);
//    EXPECT_EQ(b.daysOverdue(13), 0);
//}
//
//TEST(Group1_Q2, FineCappedAtTenTimesRatePerDay)
//{
//    Book b("Test", 14);
//    EXPECT_DOUBLE_EQ(b.calculateFine(15, 1.0), 1.0);    // 1 day late, no cap
//    EXPECT_DOUBLE_EQ(b.calculateFine(64, 1.0), 10.0);   // 50 days late -> capped at 10
//    EXPECT_DOUBLE_EQ(b.calculateFine(100, 2.0), 20.0);  // 86 days * 2 = 172 -> capped at 20
//}


//TEST(Group1_Q3, BonusPercentForReturnsCorrectTier)
//{
//    Employee junior{ "Ali",  1000.0, 1 };
//    Employee mid{ "Zara", 1000.0, 3 };
//    Employee senior{ "Omar", 1000.0, 5 };
//
//    EXPECT_DOUBLE_EQ(bonusPercentFor(junior), 5.0);
//    EXPECT_DOUBLE_EQ(bonusPercentFor(mid), 10.0);
//    EXPECT_DOUBLE_EQ(bonusPercentFor(senior), 15.0);
//}
//
//TEST(Group1_Q3, ApplyYearEndBonusAddsPercentageToSalary)
//{
//    Employee arr[1] = { {"Ali", 1000.0, 3} };
//    applyYearEndBonus(arr, 1);
//    EXPECT_DOUBLE_EQ(arr[0].salary, 1100.0);   // 10% bonus
//}
//
//TEST(Group1_Q3, SeniorEmployeesReceiveFlatTopUpOnTopOfBonus)
//{
//    Employee arr[1] = { {"Ali", 1000.0, 5} };
//    applyYearEndBonus(arr, 1);
//    // 15% bonus (150) + flat 2000 top-up = 3150
//    EXPECT_DOUBLE_EQ(arr[0].salary, 3150.0);
//}
//
//TEST(Group1_Q3, CountTopTierDoesNotModifyAnySalary)
//{
//    Employee arr[3] = {
//        {"Ali",  1000.0, 1},
//        {"Zara", 1000.0, 5},
//        {"Omar", 1000.0, 7},
//    };
//    double before = arr[0].salary + arr[1].salary + arr[2].salary;
//    EXPECT_EQ(countTopTier(arr, 3), 2);
//    double after = arr[0].salary + arr[1].salary + arr[2].salary;
//    EXPECT_DOUBLE_EQ(before, after);
//}
//
//TEST(Group1_Q3, TotalPayrollAfterBonusReflectsAllBonusesAndTopUps)
//{
//    Employee arr[2] = {
//        {"Ali",  1000.0, 3},   // +10% = 100   -> 1100
//        {"Zara", 1000.0, 5},   // +15% + 2000  -> 3150
//    };
//    applyYearEndBonus(arr, 2);
//    EXPECT_DOUBLE_EQ(totalPayrollAfterBonus(arr, 2), 4250.0);
//}


TEST(Group1_Q4, AreaComputesCorrectValue)
{
    Circle c(2.0);
    EXPECT_NEAR(c.area(), 12.56636, 0.001);
}

TEST(Group1_Q4, GrowByStepsCompoundsMultiplicatively)
{
    Circle c(2.0);
    c.growBySteps(2, 50.0);              // r = 2 * 1.5 * 1.5 = 4.5
    EXPECT_NEAR(c.area(), 63.6172, 0.001); // pi * 4.5^2
}

TEST(Group1_Q4, ZeroOrNegativeRadiusNeverGrows)
{
    Circle z(0.0);
    z.growBySteps(5, 50.0);
    EXPECT_DOUBLE_EQ(z.area(), 0.0);

    Circle n(-5.0);
    n.growBySteps(5, 50.0);
    EXPECT_DOUBLE_EQ(n.area(), 0.0);
}

TEST(Group1_Q4, StepsToExceedAreaReturnsZeroIfAlreadyExceeds)
{
    Circle c(5.0);                        // area ~ 78.54
    EXPECT_EQ(c.stepsToExceedArea(10.0, 50.0), 0);
    EXPECT_EQ(c.stepsToExceedArea(70.0, 100.0), 0);
}

TEST(Group1_Q4, StepsToExceedAreaSimulatesWithoutChangingStoredRadius)
{
    Circle c(1.0);                        // area ~ 3.14
    int steps = c.stepsToExceedArea(10.0, 100.0); // double each step
    EXPECT_EQ(steps, 1);                  // step 1: r=2, area=12.57 > 10
    EXPECT_NEAR(c.area(), 3.14159, 0.001); // stored radius unchanged
}
