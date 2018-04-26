#include "stdafx.h"

#include <iostream>
#include "BaseAction.h"
#include "BinTree.h"

using namespace std;

template <typename InfType>
class Print : public BaseAction<InfType>
{
public:

	virtual void operator() (int id, InfType &val)// рекурсивный вывод элементов 
	{
		cout << "(key: " << id << ", value: " << val << ")\n";
	}
};

int main()
{
	setlocale(0, "rus");

    BinTree<int> t1;
	
	int key = 0;
	int data = 0;

	while (key != -1)
	{
		cout << "Enter key: ";
		cin >> key;
		cout << "Enter data: ";
		cin >> data;
		if (key != -1)
			t1.addNode(key, data);
	}
    BaseAction<int> *p = new Print<int>;
    cout << endl;
    t1.actionOnElement(p);

    //t1.delElemForKey(9);

    BinTree<int> t2 = t1;
    cout << endl;
    t2.actionOnElement(p);
    delete p;
    if (t2.breadthSearch(96)) cout << "\nNICE!\n";

	return 0;
}