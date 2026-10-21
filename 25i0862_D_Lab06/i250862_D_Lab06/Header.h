#pragma once
#include<iostream>
#include<cmath>
// q1
struct Rectangle {
    int* width;
    int height;
};

void setWidth(Rectangle& r, int* newWidth) {
    r.width = newWidth;
}

int computeArea(const Rectangle& r) {
    return (*(r.width)) * r.height;
}

void displayRectangle(const Rectangle& r) {
    std::cout << "Width: " << *r.width << ", Height: " << r.height << std::endl;
}

// q2
struct Point {
    int x;
    int y;
};

void movePoint(Point* p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

void resetPoint(Point* p) {
    p->x = 0;
    p->y = 0;
}

int distanceFromOrigin(const Point* p) {
    return static_cast<int>(std::sqrt((p->x * p->x) + (p->y * p->y)));
}

// q3
struct Rectangle {
    double width;
    double height;
    double area;
    double perimeter;
};

Rectangle makeRectangle(double width, double height) {
    double area = width * height;
    double perimeter = 2 * (width + height);
    return Rectangle{ width, height, area, perimeter };
}

bool isLarger(const Rectangle& r1, const Rectangle& r2) {
    return r1.area > r2.area;
}

//q4
struct Product {
    std::string name;
    int id;
    double price;
};

bool isEqual(const Product& a, const Product& b) {
    return a.name == b.name && a.id == b.id && a.price == b.price;
}

bool isSameId(const Product& a, const Product& b) {
    return a.id == b.id;
}

Product cloneProduct(const Product& original) {
    Product copy = original;
    return copy;
}

// q5

struct StudentRecord {
    std::string name;
    int marks[5];
    double average;

    void printAverage(); 
};

void StudentRecord::printAverage() {
    std::cout << "Student: " << name << std::endl;
    std::cout << "Average: " << average << std::endl;
}
 
double computeAverage(const StudentRecord& s) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += s.marks[i];
    }
    return static_cast<double>(sum) / 5;
}
