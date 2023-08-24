#include<iostream>
#include<memory>

class object {
public:
    object(){std::cout << "default constructor\n";}
    object(int a){std::cout << "constructor" << a << std::endl;}
    object(const object&){std::cout << "copy constructor\n";}
    object(object&&) noexcept {std::cout << "move constructor\n";}
    object& operator=(const object& o) {std::cout << "assign op\n";return *this;}
    object& operator=(object&& o) noexcept {std::cout << "move assign op\n";return *this;}
    ~object(){std::cout << "destructor\n";};
};

int main() {
    std::unique_ptr<object> p {std::make_unique<object>(1)};
    p.reset(new object(2));
}