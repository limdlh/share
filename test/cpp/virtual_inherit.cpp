#include <iostream>

using namespace std;
class Parent
{
public:
	int p;                                           // p将会被所有的子类继承，也将是二义性的根源
	inline Parent()
	{
		p = 10;
	}
};

class Child1 : virtual public Parent
{
public:
	int c1;
	inline Child1()
	{
		p = 12;                           // p在子类Child1中被赋值为12
		c1 = 12;
	}
};

class Child2 : virtual public Parent
{
public:
	int c2;
	inline Child2()
	{
		p = 13;                          // p在子类Child2中被赋值为13
		c2 = 13;
	}
};

class GrandChild : public Child1, public Child2
{
public:
	int grandchild;
	// p显然也存在于GrandChild中，但是到底是12，还是13呢？这就产生了二义性
	inline GrandChild()
	{
		grandchild = 14;
	}
 };

int main(void)
{
	GrandChild* pGC = new GrandChild();
	cout << pGC->p << endl;
	cout << pGC->grandchild << endl;
	
	return 0;
}
 