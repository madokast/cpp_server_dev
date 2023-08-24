#include<iostream>
#include<map>

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
    std::map<int, int> m;
    m.insert_or_assign(1, 2);
    for (auto [k,v] : m) {
        std::cout << k << "->" << v << std::endl;
    }
    std::cout << "-------\n";
    m.insert_or_assign(1, 3);
    for (auto [k,v] : m) {
        std::cout << k << "->" << v << std::endl;
    }
    // 1->2
    // -------------
    // 1->3
}