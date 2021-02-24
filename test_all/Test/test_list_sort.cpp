#include "test_list_sort.h"

#include <list>
#include <iostream>


int test_sort()
{
	std::list<test_list_sort> ltls;
	ltls.push_back(std::string("1"));
	ltls.push_back(std::string("2"));
	ltls.push_back(std::string("3"));
	ltls.push_back(std::string("11"));
	ltls.push_back(std::string("22"));
	ltls.push_back(std::string("33"));
	ltls.sort();
	std::cout << "default sort" << std::endl;
	for (auto& it : ltls)
	{
		std::cout << it.value() << " < ";
	}
	std::cout << std::endl;

	// sort 
	ltls.sort([](const test_list_sort& a, const test_list_sort& b)->bool {
		return a.value() > b.value();
		});
	std::cout << "my sort" << std::endl;
	for (auto& it : ltls)
	{
		std::cout << it.value() << " > ";
	}
	std::cout << std::endl;

	return 0;
}
