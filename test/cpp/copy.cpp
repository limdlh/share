#include <iostream>
#include <string.h>

using namespace std;

class Person
{
public :
	// 构造函数
	Person(char * pN)
	{
		m_pName = new char[strlen(pN) + 1];
		//在堆中开辟一个内存块存放pN所指的字符串
		if(m_pName != NULL)
		{
			//如果m_pName不是空指针，则把形参指针pN所指的字符串复制给它
			strcpy(m_pName ,pN);
	    }
		cout << "一般构造函数被调用 ! " << m_pName << std::endl;
	}

	// 系统创建的默认复制构造函数，只做位模式拷贝
	/*Person(Person & p)
	{
		//使两个字符串指针指向同一地址位置
		m_pName = p.m_pName;
		cout << "系统创建的默认复制构造函数 被调用 ! " << m_pName << std::endl;
	}*/

    Person(Person & chs)
	{
		// 用运算符new为新对象的指针数据成员分配空间
		m_pName = new char[strlen(chs.m_pName)+ 1];

		if(m_pName)
		{
			 // 复制内容
			strcpy(m_pName, chs.m_pName);
		}
		// 则新创建的对象的m_pName与原对象chs的m_pName不再指向同一地址了
		cout << "用户创建的复制构造函数 被调用 ! " << m_pName << std::endl;
	}

	~Person()
	{
		delete m_pName;
	}

	private :
	char * m_pName;
};

// 下面自己设计复制构造函数，实现“深拷贝”，即不让指针指向同一地址，而是重新申请一块内存给新的对象的指针数据成员
/*Person::Person(Person & chs)
{
	// 用运算符new为新对象的指针数据成员分配空间
	m_pName = new char[strlen(chs.m_pName)+ 1];

	if(m_pName)
	{
		 // 复制内容
		strcpy(m_pName, chs.m_pName);
	}
	// 则新创建的对象的m_pName与原对象chs的m_pName不再指向同一地址了
}*/

int main()
{
	Person man("lujun");
	Person woman(man);
	// 结果导致   man 和    woman 的指针都指向了同一个地址

	// 函数结束析构时
	// 同一个地址被delete两次
	return 1;
}


