// TemplateContainer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <vector>
#include "cArray.h"
#include <algorithm>


int main()
{
	cArray<int> arr;
	arr.push_back(4);
	arr.push_back(7);
	arr.push_back(1);
	arr.push_back(4);
	arr.push_back(2);

	std::cout << arr[(unsigned int)0] <<std::endl;
	std::cout << arr[(unsigned int)1] <<std::endl;
	std::cout << arr[(unsigned int)2] <<std::endl;
	std::cout << arr[(unsigned int)3] <<std::endl;
	std::cout << arr[(unsigned int)4] <<std::endl;

	cArray<int>::iterator it = arr.begin();
	std::cout <<"iterator check: "<< *it << std::endl;

	cArray<int>::iterator itMin = std::min_element(it, arr.end());
	std::cout << "min elem: " << std::endl;
	std::cout << *itMin << std::endl;
	
	std::sort(arr.begin(), arr.end());
	std::cout << "after sort: " << std::endl;
	std::cout << arr[(unsigned int)0] << std::endl;
	std::cout << arr[(unsigned int)1] << std::endl;
	std::cout << arr[(unsigned int)2] << std::endl;
	std::cout << arr[(unsigned int)3] << std::endl;
	std::cout << arr[(unsigned int)4] << std::endl;
	/**/

	system("pause");
}

