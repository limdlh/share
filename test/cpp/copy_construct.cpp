#include <iostream>

using namespace std;
class A
{
public:
    A():m_ptr(new int(0)){cout << "construct" << endl;}
    A(const A& a):m_ptr(new int(*a.m_ptr)) //深拷贝的拷贝构造函数
    {
        cout << "copy construct" << endl;
    }
    ~A(){ delete m_ptr;}
private:
    int* m_ptr;
};

A GetA()
{
    return A();
}

int main() {
    A a = GetA();
	
	int n = 123;
	int m = std::move(n);
	cout << "m: " << m << endl;
	cout << "n: " << n << endl;
    return 0;
}