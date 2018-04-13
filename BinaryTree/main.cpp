#include "stdafx.h"
#include <iostream>
#define DEPTH_SEARCH_RIGHT 1
#define DEPTH_SEARCH_LEFT 0

using namespace std;

template <typename InfType>
struct TElem {
	int ID;
	InfType inf;
	TElem *leftPtr, *rightPtr,
		*nextPtr, *prevPtr;

	TElem<InfType>(int key, InfType data)
	{
		this->inf = data;
		this->ID = key;
		this->leftPtr = this->rightPtr = this->nextPtr = this->prevPtr = NULL;
	}
};

template <typename InfType>
class BaseAction
{
public:

	virtual void operator() (int id, InfType &val) = 0;

};

template <typename InfType>
class Print : public BaseAction<InfType>
{
public:

	virtual void operator() (int id, InfType &val)// ����������� ����� ��������� 
	{
		cout << "(key: " << id << ", value: " << val << ")\n";
	}
};

template <typename InfType>
class BTree
{
private:

	TElem<InfType> *root;

	struct TLevel
	{
		TElem<InfType> *firstNode;
		TLevel *next;
		TLevel() : firstNode(NULL), next(NULL) {}
		TLevel(TElem<InfType>* ptr) : firstNode(ptr), next(NULL) {}
	};

	class Levels
	{
	private:

		TLevel *firstLevel;

	public:

		Levels() : firstLevel(new TLevel) {}

		void setFirstLevel(TElem<InfType> *p)
		{
			firstLevel->firstNode = p;
		}

		void newLevel(TElem<InfType> *elem)
		{
			TLevel *temp = firstLevel;
			if (temp->firstNode != NULL)
			{
				while (temp->next != NULL)
					temp = temp->next;
				temp->next = new TLevel(elem);
			}
			else
				temp->firstNode = elem;
		}

		void insertIntoLevel(TElem<InfType> *newElem, int depth)
		{
			if (newElem != NULL)
			{
				TLevel* curLevel = selectLevel(depth);
				if (curLevel != NULL)
				{
					TElem<InfType>* curElem = curLevel->firstNode;
					while (curElem->nextPtr != NULL && newElem->ID > curElem->ID) // ������� �� ������� ��������
					{
						curElem = curElem->nextPtr;
					}

					if (newElem->ID < curElem->ID)
					{
						if (curElem->prevPtr == NULL) // ���������� � ������ ������
						{
							newElem->nextPtr = curElem;
							curElem->prevPtr = newElem;
							curLevel->firstNode = newElem; // ������ ����������� ������� ������� ������
						}
						else // ������� � �������
						{
							curElem->prevPtr->nextPtr = newElem;
							newElem->prevPtr = curElem->prevPtr;
							newElem->nextPtr = curElem;
							curElem->prevPtr = newElem;
						}
					}
					else if (curElem->nextPtr == NULL) // ���������� � ����� ������
					{
						curElem->nextPtr = newElem;
						newElem->prevPtr = curElem;
					}
				}
				else
				{
					newLevel(newElem);
				}
			}
		}

		TLevel* selectLevel(int number)
		{
			TLevel* temp = firstLevel;
			while (temp != NULL && number != 1)
			{
				temp = temp->next;
				--number;
			}
			return temp;
		}

		~Levels()
		{
			TLevel *temp = firstLevel;
			while (temp != NULL)
			{
				firstLevel = firstLevel->next;
				delete temp;
				temp = firstLevel;
			}
		}

	};

	Levels levels;

	bool searchKey(int key, int &depth, TElem<InfType> *&parent) // ����� �� �����
	{
		TElem<InfType> *cur = root;
		while (cur != NULL) // ���� �� ������ �� ����� 
		{
			if (key == cur->ID) // ���� ����� ���������, �� ������� ������
				return true;
			parent = cur; // ���������� ����� ��������
			++depth;
			cur = key > cur->ID ? cur->rightPtr : cur->leftPtr;
		}
		return false; // ���� ����� �� ����� � �� ����� �������
	}

	// ����� � ������� � ����� �����������
	TElem<InfType>* depthSearchLeft(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent)
	{
		if (cur == NULL || cur->inf == val) // ������� ������
			return cur;
		TElem<InfType> *res;
		parent = cur;
		return  (NULL != (res = depthSearchLeft(val, cur->leftPtr, parent))) ? res : depthSearchLeft(val, cur->rightPtr, parent);

	}

	// ����� � ������� � ������ �����������
	TElem<InfType>* depthSearchRight(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent)
	{
		if (cur == NULL || cur->inf == val)
			return cur;
		TElem<InfType> *res;
		parent = cur;
		return  (NULL != (res = depthSearchRight(val, cur->rightPtr, parent))) ? res : depthSearchRight(val, cur->leftPtr, parent);

	}

	void inOrderAction(BaseAction<InfType>* act, TElem<InfType> *elem)
	{
		if (elem == NULL)
			return;
		inOrderAction(act, elem->leftPtr);
		(*act)(elem->ID, elem->inf);
		inOrderAction(act, elem->rightPtr);
	}

	void delAll(TElem<InfType> *elem)
	{
		if (elem != NULL)
		{
			delAll(elem->leftPtr);
			delAll(elem->rightPtr);
			delete elem;
		}
	}

public:
	BTree() : root(NULL) 
	{ 
		levels.setFirstLevel(root);
	}

	const TElem<InfType>* getRoot()
	{
		return root;
	}

	bool isEmpty()
	{
		return (root == NULL) ? true : false;
	}

	/*BTree& operator=(BTree& bt)
	{
		if (bt.isEmpty() || *this == &bt)
			return *this;

		if (!isEmpty()) // ������� ������� ������
			delAll(this->root);
		
		TElem<InfType> *copyRoot = bt.getRoot();
		copyTree(copyRoot);
	}*/

	BTree& copyTree(TElem<InfType> *elem)
	{
		addNode(elem->ID, elem->inf);
		copyTree(elem->leftPtr);
		copyTree(elem->rightPtr);
	}

	/*void printLevel() // ��� ������
	{
	TLevel *p = levels.selectLevel(1);
	TElem<InfType> *temp = root;
	while (p != NULL)
	{
	temp = p->firstNode;
	while (temp != NULL)
	{
	cout << temp->ID << " ";
	temp = temp->nextPtr;
	}
	p = p->next;
	cout << endl;
	}
	}*/

	bool addNode(int key, InfType value) // ��������� �������� ������
	{
		TElem<InfType> *cur = new TElem<InfType>(key, value);

		if (this->root == NULL) // ���� ������ ������
		{
			this->root = cur; // ����� ������
			levels.setFirstLevel(this->root);
		}
		else // ����� ������� �� ���������������� ����� � ��������� ��� �������
		{
			int depth = 1; // ������� ������������ ��������
			TElem<InfType>*	parent = NULL;

			if (searchKey(key, depth, parent)) // ���� ����� ������� ��� ����������
			{
				delete cur;
				return false;
			}

			(key > parent->ID) ? parent->rightPtr = cur : parent->leftPtr = cur;

			levels.insertIntoLevel(cur, depth);
		}
		return true;
	}

	bool searchElemForKey(int key) // ����� �� �����
	{
		return searchKey(key, NULL, NULL);
	}

	bool depthSearch(InfType val, bool priority = DEPTH_SEARCH_LEFT) // ����� � ������� �� ��� �����
	{
		return (priority == DEPTH_SEARCH_LEFT) ? depthSearchLeft(val, root, NULL) != NULL : depthSearchRight(val, root, NULL) != NULL;
	}

	bool breadthSearch(InfType val) // ����� � ������
	{
		TElem<InfType> *p = root;
		TLevel *level;
		int i = 0;

		while (level = levels.selectLevel(++i))
		{
			p = level->firstNode;
			while (p != NULL)
			{
				if (p->inf == val)
					return true;
				p = p->nextPtr;
			}
		}

		return false;
	}

	void actionOnElement(BaseAction<InfType>* act) // �������� ��� ���������
	{
		inOrderAction(act, root);
	}

	int getDepth()
	{
		TLevel *temp = levels.firstLevel;
		int d = 0;
		while (temp != NULL)
		{
			d++;
			temp = temp->next;
		}
		return d;
	}

	~BTree()
	{
		delAll(this->root);
	}
};

int main()
{
	setlocale(0, "rus");

	BTree<int> t1;
	//BTree<int> t2;
	int key = 0;
	int data;

	while (key != -1)
	{
		cout << "Enter key: ";
		cin >> key;
		cout << "Enter data: ";
		cin >> data;
		if (key != -1)
			t1.addNode(key, data);
	}

	//t1.printLevel();
	BaseAction<int> *p = new Print<int>;
	t1.actionOnElement(p);
	delete p;

	return 0;
}