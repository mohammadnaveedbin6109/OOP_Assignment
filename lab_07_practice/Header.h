#pragma once
#include<string>
using namespace std;
//class Ticket {
//private:
//	int seat;
//	double basePrice;
//	string category;
//	double	discountApplied=1.0;
//	bool refunded=false;
//	double categorymultiplier() const {
//		return (category == "VIP") ? 1.5 : 0;
//	}
//public:
//	void setDetails(int seat, double basePrice, string category) {
//		this->seat = seat;
//		this->basePrice = basePrice;
//		this->category = category;
//		discountApplied = 1.0;
//		refunded = false;
//	}
//	double getFinalPrice() {
//		double adjusted = basePrice * categorymultiplier();
//		double price = adjusted * discountApplied;
//		double floorPrice = adjusted * 0.10;   // protected floor
//		return (price < floorPrice) ? floorPrice : price;
//	}
//
//	void applyDiscount(double percent) {
//		if (percent <= 0 || percent > 100) return;   // ignore invalid discounts
//		discountApplied *= (1.0 - percent / 100.0);
//	}
//
//	bool refund(int minutesSincePurchase) {
//		if (refunded || minutesSincePurchase < 0 || minutesSincePurchase >= 30)
//			return false;
//		refunded = true;
//		return true;
//	}
//
//	bool isRefunded() {
//		return refunded;
//	}
//};

//class Book {
//	string title;
//	int dueDay;
//	int renewalused=0;
//public:
//	Book(string title, int loanDays) {
//		this->title = title;
//		dueDay = loanDays;
//	}
//	bool renew(int extraDays) {
//		if (renewalused < 2) {
//			dueDay += extraDays;
//			renewalused++;
//			return true;
//		}
//		return false;
//	}
//	int daysOverdue(int currentDay) {
//		if (currentDay <= dueDay)
//			return 0;
//		else
//		return currentDay - dueDay;
//	}
//	double calculateFine(int currentDay, double ratePerDay) {
//		double price = currentDay * ratePerDay;
//		if (price >= (10 * ratePerDay))
//			return ratePerDay*10;
//		else
//			return price;
//	}
//};

// q3

//class Employee{
//public:
//	string name;
//	double salary;
//	int yearsWorked;
//};
//double bonusPercentFor(const Employee& e) {
//	double perc = 0;
//	if (e.yearsWorked < 2)
//		return perc = 5;
//	if (e.yearsWorked >= 2 && e.yearsWorked <5)
//		return perc = 10;
//	if (e.yearsWorked >= 5)
//		return perc = 15;
//}
//void applyYearEndBonus(Employee arr[], int n) {
//	for (int i = 0;i < n;i++) {
//		double perc=bonusPercentFor(*(arr + i));
//		double addition = ((arr+i)->salary) * (perc / 100);
//		((arr + i)->salary) += addition;
//		if (perc == 15) {
//			((arr + i)->salary) += 2000;
//		}
//	}
//}
//int countTopTier(Employee arr[], int n) {
//	int count = 0;
//	for (int i = 0;i < n;i++) {
//		if ((arr + i)->yearsWorked >= 5)
//			count++;
//	}
//	return count;
//}
//double totalPayrollAfterBonus(Employee arr[], int n) {
//	double sum = 0;
//	for (int i = 0;i < n;i++) {
//		sum += (arr + i)->salary;
//	}
//	return sum;
//}

class Circle{ 
private: 
double radius;
public:
Circle(double r) {
	radius = r;
}
void growBySteps(int steps, double percentPerStep) {
	if (radius <= 0)
		return;
	r += r * (1 + p / 100);
}
double area() const {
	return 3.14 * radius * radius;
}
double Area(double r) const {
	return 3.14 * r * r;
}
int stepsToExceedArea(double targetArea, double percentPerStep) {
	double r = radius;
	int steps = 0;
	while (Area(r) < targetArea) {
		r += r * (1 + percentPerStep / 100);
		steps++;
	}
	return steps;
}

};