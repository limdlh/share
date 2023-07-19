#include <iostream>

int main() {
	auto num = 5;
	printf("num: %d\n", num);
	int arr[10] = {0};
	auto auto_arr = arr;
	for(auto &i : arr) {    
		i = num++;
	}
	//auto auto_arr2[10] = arr;
	printf("num: %d\n", num);

	for(auto &i : arr) {    
		std::cout << i << std::endl;
	}

 return 0;
}