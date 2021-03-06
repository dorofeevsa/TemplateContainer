// TemplateContainer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <vector>
#include "cArray.h"
#include <algorithm>


int main()
{
	cArray<uint32_t> arr;
	for (short i = 0; i < 10; i++)
	{
		uint32_t value = rand() % 10;
		arr.push_back(value);
		std::cout << value<<std::endl;
	}

	

	cArray<uint32_t>::iterator it = arr.begin();
	std::cout <<"iterator check: "<< *it << std::endl;

	cArray<uint32_t>::iterator itMin = std::min_element(it, arr.end());
	std::cout << "min elem: " << std::endl;
	std::cout << *itMin << 
		std::endl;
	
	std::sort(arr.begin(), arr.end());
	std::cout << "after sort: " << std::endl;
	for (uint32_t i = 0; i < arr.size(); i++)
	{
		std::cout << arr[i] << std::endl;
	}
	/**/

	system("pause");
}

