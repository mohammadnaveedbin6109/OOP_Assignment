#include<iostream>
#include<string>
#include<cmath>
using namespace std;
// class book{
//     char title[50];
//     float price;
//     int pages;
//     public:
//     book(){
//         char cpy[]="unknown";
//         int i=0;
//         while(cpy[i]!='\0'){
//             title[i]=cpy[i];
//             i++;
//         }
//         title[i]='\0';
//         price=0;
//         pages=0;
//     }
//     book(char*a,int b,int c){
//         int i=0;
//         while(a[i]!='\0'){
//             title[i]=a[i];
//             i++;
//         }
//         title[i]='\0';
//         price=b;
//         pages=c;
//     }
//     void display();
//     int getprice(){
//         return price;
//     }
// };
// void book :: display(){
//     cout<<"title  : "<<title;
//     cout<<"price  : "<<price;
//     cout<<"pages  : "<<pages;
// }
// int highestprice(int n,book*object){
//     float price=object[0].getprice();
//     int j=0;
//     for(int i=0;i<n;i++){
//         if(object[i].getprice()>price){
//             price=object[i].getprice();
//             j=i;    
//         }
//     }
//     object[j].display();

// }
// int main(){
//     int n;
//     cin>>n;
//     book*object=new book[n];
//     for(int i=0;i<n;i++){
//         char arr[50];
//         cin>>arr;
//         int a,int b;
//         cin>>a;
//         cin>>b;
//         object[i]=book(arr,a,b);


//     }

// }

// struct Student {
//     int id;
//     int numSubjects;
//     float* marks;      // dynamically allocated array
// };
// Student createStudent(int id, int n){
//     Student newstudent;
//     newstudent.id=id;
//     newstudent.marks=new float[n];
//     for(int i=0;i<n;i++){
//         cin>>newstudent.marks[i];
//     }
//     return newstudent;

// }
// float sumMarks(float* marks, int n){
//     if(n<=0)
//     return 0;
//     return marks[n-1]+sumMarks(marks,n-1);
// }
// float maxMarks(float* marks, int n){
//     if(n==1)
//     return marks[0];
//     float hmarks=maxMarks(marks,n-1);
//     if(marks[n-1]>hmarks){
//         return marks[n-1];
//     }
//     else
//     return hmarks;
// }
// float average(const Student* s){
//     float sum=sumMarks(s->marks,s->numSubjects);
//     return sum/s->numSubjects;
// }
// void printStudent(const Student& s){
//     cout<<s.id<<"  "<<s.numSubjects<<endl;
//     for(int i=0;i<s.numSubjects;i++){
//         cout<<s.marks[i]<<" ";
//     }
// }

// class Matrix {
//     int rows;
//     int cols;
//     int** data;     // 2D dynamic array
// public:
//     Matrix(int r, int c){
//         rows=r;
//         cols=c;
//         data=new int *[r];
//         for(int i=0;i<r;i++){
//             data[i]=new int [c];
//         }
//     }
//     ~Matrix(){
//         for(int i=0;i<rows;i++){
//             delete[] data[i];
//         }
//         delete[] data;
//     }
// };

int count(const char*digit){
    if(*digit=='\0')
    return 0;
    return 1+count(digit+1);
}
int stringToIntRecursive(const char* digits){
    if(*digits=='\0')
    return 0;
    int c=count(digits)-1;
    int a=int(*digits-'0')*(pow(10,c));
    return a+stringToIntRecursive(digits+1);

}
int main(){
    int a=stringToIntRecursive("507");
    cout<<a;
}