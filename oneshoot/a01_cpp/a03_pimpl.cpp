class Client {
public:
    Client();
    void init();
    void close();
    void send();
private:
    class Impl;
    Impl* m_impl;
};

int main() {
    Client c;
    c.init();
    c.send();
    c.close();
}

// 一下内容已提前编译用户不可见
#include<iostream>

class Client::Impl {
public:
    void init()  {std::cout<<"init\n";}
    void close() {std::cout<<"close\n";}
    void send()  {std::cout<<"send\n";}
};

Client::Client() {m_impl = new Impl;}
void Client::init() {m_impl->init();}
void Client::close() {m_impl->close();}
void Client::send() {m_impl->send();}