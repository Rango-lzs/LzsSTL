#include "list.h"

void list_test()
{
	Lzs::list<int> l1;
	Lzs::list<int> l2(10);
	Lzs::list<int> l3((size_t)5, 2); // ������Ҫ��size_t �������� ���ĸ����캯�������أ� list(n,x) list(ite, ite);����ô�Ľ�
}