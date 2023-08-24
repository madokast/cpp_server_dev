#include<functional>
#include<iostream>

class Substractor {
public:
    int sub(int a, int b) {return a-b;}
};

int main() {
    Substractor s;
    std::function<int(int, int)> rsub = std::bind(&Substractor::sub, &s, std::placeholders::_2, std::placeholders::_1); // add2(&a, _1)
    std::cout << rsub(3, 5) << std::endl;
}