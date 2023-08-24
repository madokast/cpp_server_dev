#include<iostream>
#include<vector>

class object {
public:
    object(){std::cout << "default constructor\n";}
    object(const object&){std::cout << "copy constructor\n";}
    object(object&&) noexcept {std::cout << "move constructor\n";}
    object& operator=(const object& o) {std::cout << "assign op\n";return *this;}
    object& operator=(object&& o) noexcept {std::cout << "move assign op\n";return *this;}
    ~object(){std::cout << "destructor\n";};
};

int main() {
    std::vector<object> v;
    v.emplace_back();
    // default constructor
    // destructor
}