#include <iostream>
#include <string>
#include <memory>
using namespace std;

int main() {
	auto_ptr<string> films[5] ={
		auto_ptr<string> (new string("Fowl Balls")),
		auto_ptr<string> (new string("Duck Walks")),
		auto_ptr<string> (new string("Chicken Runs")),
		auto_ptr<string> (new string("Turkey Errors")),
		auto_ptr<string> (new string("Goose Eggs"))
	};
    auto_ptr<string> pwin;
    pwin = films[2]; // films[2] loses ownership. 将所有权从films[2]转让给pwin，此时films[2]不再引用该字符串从而变成空指针

	cout << "The nominees for best avian baseballl film are\n";
	for(int i = 0; i < 5; ++i) {
		cout << *films[i] << endl;
	}
 	cout << "The winner is " << *pwin << endl;
	return 0;
}
