#include "list.h"

void list_test()
{
	Lzs::list<int> l1;
	Lzs::list<int> l2(10);
	Lzs::list<int> l3((size_t)5, 2); // 这里需要用size_t 才能区分 是哪个构造函数的重载， list(n,x) list(ite, ite);给怎么改进
}