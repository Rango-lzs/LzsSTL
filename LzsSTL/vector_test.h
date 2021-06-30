#ifndef _VECTOR_TEST_HH
#define _VECTOR_TEST_HH

#include <vector>
#include "vector.h"
#include "test.h"
using namespace std;

namespace test {

void vector_test()
{
	std::cout << "===================================" << endl;
	std::cout << "------------Run container test: vector test--------" << endl;
	std::cout << "--------------------API test---------------------------" << endl;

	int a[] = { 1,2,3,4,5 };
	vector<int> v1;
	vector<int> v2(10);
	vector<int> v3(10, 1);
	vector<int> v4(a, a + 5);
	vector<int> v5(v2);
	vector<int> v6(std::move(v2));
	//vector<int> v7{ 1,2,3,4,5,6,7,8,9 };
	vector<int> v8, v9, v10;
}

} //end of test

#endif