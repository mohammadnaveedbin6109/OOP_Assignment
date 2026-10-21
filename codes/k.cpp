#include<iostream>
using namespace std;
void func(int n){
    if (n<=0)
    return;
    if(n%2==0)
    cout<<n<<"  ";
    func(n-2);
    if(n%3==0)
    func(n-1);
    else
    func(n-2);
}
int main(){
    func(8);
}