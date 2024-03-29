#include <iostream>

class Complex
{
private:
	double    m_real;
	double    m_imag;

public:
	//    无参数构造函数
	// 如果创建一个类你没有写任何构造函数,则系统会自动生成默认的无参构造函数，函数为空，什么都不做
	// 只要你写了一个下面的某一种构造函数，系统就不会再自动生成这样一个默认的构造函数，如果希望有一个这样的无参构造函数，则需要自己显示地写出来
	Complex(void)
	{
		 m_real = 0.0;
		 m_imag = 0.0;
	}

	//    一般构造函数（也称重载构造函数）
	// 一般构造函数可以有各种参数形式,一个类可以有多个一般构造函数，前提是参数的个数或者类型不同（基于c++的重载函数原理）
	// 例如：你还可以写一个 Complex( int num)的构造函数出来
	// 创建对象时根据传入的参数不同调用不同的构造函数
	Complex(double real, double imag)
	{
		 m_real = real;
		 m_imag = imag;
	 }

	//    复制构造函数（也称为拷贝构造函数）
	//    复制构造函数参数为类对象本身的引用，用于根据一个已存在的对象复制出一个新的该类的对象，一般在函数中会将已存在对象的数据成员的值复制一份到新创建的对象中
	//    若没有显示的写复制构造函数，则系统会默认创建一个复制构造函数，但当类中有指针成员时，由系统默认创建该复制构造函数会存在风险，具体原因请查询 有关 “浅拷贝” 、“深拷贝”的文章论述
	Complex(const Complex & c)
	{
		// 将对象c中的数据成员值复制过来
		m_real = c.m_real;
		m_imag = c.m_imag;
	}

	// 类型转换构造函数，根据一个指定的类型的对象创建一个本类的对象
	// 例如：下面将根据一个double类型的对象创建了一个Complex对象
	Complex(double r)
	{
		m_real = r;
		m_imag = 0.0;
	}

	// 等号运算符重载
	// 注意，这个类似复制构造函数，将=右边的本类对象的值复制给等号左边的对象，它不属于构造函数，等号左右两边的对象必须已经被创建
	// 若没有显示的写=运算符重载，则系统也会创建一个默认的=运算符重载，只做一些基本的拷贝工作
	Complex &operator=( const Complex &rhs )
	{
		// 首先检测等号右边的是否就是左边的对象本，若是本对象本身,则直接返回
		if ( this == &rhs )
		{
			return *this;
		}

		// 复制等号右边的成员到左边的对象中
		this->m_real = rhs.m_real;
		this->m_imag = rhs.m_imag;

		// 把等号左边的对象再次传出
		// 目的是为了支持连等 eg:    a=b=c 系统首先运行 b=c
		// 然后运行 a= ( b=c的返回值,这里应该是复制c值后的b对象)
		return *this;
	}
};

Complex test1(const Complex& c)
{
	return c;
}

Complex test2(const Complex c)
{
	return c;
}

Complex test3()
{
	static Complex c(1.0,5.0);
	return c;
}

Complex& test4()
{
	static Complex c(1.0,5.0);
	return c;
}

int main()
{
	Complex a,b;

	// 下面函数执行过程中各会调用几次构造函数，调用的是什么构造函数？
	test1(a);
	test2(a);

	b = test3();
	b = test4();

	test2(1.2);
	// 下面这条语句会出错吗？
	test1(1.2);     //test1( Complex(1.2 )) 呢？
	test1(Complex(1.2 ));
	return 0;
}