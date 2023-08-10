#include <iostream>

class resource {
 private:
  int* a;

 public:
  resource() { a = new int{}; };
  ~resource() {
    std::cout << "delete\n";
    if (a != nullptr) {
      delete a;
      a = nullptr;
    }
  };
  resource(const resource&) = delete;
  resource(resource&&) = delete;
  resource& operator=(const resource&) = delete;
  resource& operator=(resource&&) = delete;
};

int main() {
  resource r;
  if (false) {
    return 0;
  }
  return 1;
}