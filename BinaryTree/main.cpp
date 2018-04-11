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

	virtual void operator() (int id, InfType &val)// рекурсивный вывод элементов 
	{
		cout << "(key: " << id << ", value: " << val << ")\n";
	}
};

template <typename InfType>
class ChangeInf : public BaseAction<InfType>
{
public:

	virtual void operator() (int id, InfType &val)// рекурсивный вывод элементов 
	{
		val *= rand();
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
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = new TLevel(elem);
		}

		int getDepth()
		{
			TLevel *temp = firstLevel;
			int d = 0;
			while (temp != NULL)
			{
				d++;
				temp = temp->next;
			}
			return d;
		}

		TLevel* selectLevel(int number)
		{
			TLevel* temp = firstLevel;
			while (temp->next != NULL && number != 1)
			{
				temp = temp->next;
				--number;
			}
			return (number == 1) ? temp : NULL;
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

	bool searchKey(int key, TElem<InfType> *parent = NULL) // поиск по ключу
	{
		TElem<InfType> *cur = root;
		while (cur != NULL) // пока не дойдем до листа 
		{
			if (key == cur->ID) // если ключи совпадают, то элемент найден
				return true;
			parent = cur; // запоминаем адрес родителя
			cur = key > cur->ID ? cur->rightPtr : cur->leftPtr;
		}
		return false; // если дошли до листа и не нашли элемент
	}

	// поиск в глубину с левым приоритетом
	TElem<InfType>* depthSearchLeft(InfType val, TElem<InfType> *cur, TElem<InfType> *parent = NULL)
	{
		if (cur == NULL || cur->inf == val) // базовый случай
			return cur;
		TElem<InfType> *res;
		parent = cur;
		return  (NULL != (res = depthSearchLeft(val, cur->leftPtr, parent))) ? res : depthSearchLeft(val, cur->rightPtr, parent);

	}

	// поиск в глубину с правым приоритетом
	TElem<InfType>* depthSearchRight(InfType val, TElem<InfType> *cur, TElem<InfType> *parent = NULL)
	{
		if (cur == NULL || cur->inf == val)
			return cur;
		TElem<InfType> *res;
		parent = cur;
		return  (NULL != (res = depthSearchRight(val, cur->rightPtr, parent))) ? res : depthSearchRight(val, cur->leftPtr, parent);

	}

	TElem<InfType>* getLastLevelElem(TLevel *level)
	{
		if (level != NULL)
		{
			TElem<InfType>* elem = level->firstNode;
			while (elem->nextPtr != NULL)
			{
				elem = elem->nextPtr;
			}
			return elem;
		}
		return NULL;
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
	BTree() : root(NULL) { levels.setFirstLevel(root); }

	/*void printLevel() // для дебуга
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

	bool addNode(int key, InfType value) // добаление элемента дерева
	{
		if (searchElemForKey(key)) // если такой элемент уже существует
			return false;
		TElem<InfType> *p = new TElem<InfType>(key, value);
		int depth = 1; // глубина добавляемого элемента
		if (this->root == NULL) // если дерево пустое
		{
			this->root = p; // садим дерево
			TLevel *temp = levels.selectLevel(depth);
			temp->firstNode = root;

		}
		else // иначе доходим до соответствующего листа и добавляем ему потомка
		{
			TElem<InfType> *cur = this->root,
				*parent;
			do {
				parent = cur;
				cur = key > cur->ID ? cur->rightPtr : cur->leftPtr;
				++depth;
			} while (cur != NULL);

			TLevel *curLevel = levels.selectLevel(depth); // указатель на уровень добавляемого элемента
			if (key > parent->ID) // для правого потомка
			{
				parent->rightPtr = p;
				if (parent->leftPtr) // если существует левый потомок, то связываем его с добавляемым
				{
					parent->rightPtr->prevPtr = parent->leftPtr;
					if (parent->leftPtr->nextPtr) // если левый потомок уже указывает на следующий элемент уровня
					{
						parent->rightPtr->nextPtr = parent->leftPtr->nextPtr;
						parent->leftPtr->nextPtr = parent->rightPtr;
					}
					else parent->leftPtr->nextPtr = parent->rightPtr;
				}
				else if (curLevel != NULL) // есть ли на уровне еще елементы
				{
					if (key < curLevel->firstNode->ID) // если эти элементы находятся в правом поддереве
					{
						parent->rightPtr->nextPtr = curLevel->firstNode;
						curLevel->firstNode->prevPtr = parent->rightPtr;
						curLevel->firstNode = parent->rightPtr;
					}
					else // а если в левом
					{
						TElem<InfType>* elem = getLastLevelElem(curLevel); // получаем текущий последний элемент данного уровня
																	   // и связываем его с добавляемым
						elem->nextPtr = parent->rightPtr;
						parent->rightPtr->prevPtr = elem;
					}

				}
				else levels.newLevel(parent->rightPtr); // если уровень пуст, то создаем для добавляемого новый уровень

			}
			else // для левого потомка
			{
				parent->leftPtr = p;
				if (parent->rightPtr) // если существует правый потомок, то связываем его с добавляемым
				{
					parent->leftPtr->nextPtr = parent->rightPtr;
					parent->rightPtr->prevPtr = parent->leftPtr;
					curLevel->firstNode = parent->leftPtr;
				}
				else if (curLevel != NULL) // есть ли на уровне еще елементы
				{
					if (key < curLevel->firstNode->ID)
					{
						parent->leftPtr->nextPtr = curLevel->firstNode;
						curLevel->firstNode->prevPtr = parent->leftPtr;
						curLevel->firstNode = parent->leftPtr;
					}
					else
					{
						TElem<InfType>* elem = getLastLevelElem(curLevel); // получаем текущий последний элемент данного уровня
						elem->nextPtr = parent->leftPtr;
						parent->leftPtr->prevPtr = elem;
					}
				}
				else levels.newLevel(parent->leftPtr); // если уровень пуст, то создаем для добавляемого новый уровень
			}
		}
		return true;
	}

	bool searchElemForKey(int key) // поиск по ключу
	{
		return searchKey(key, NULL);
	}

	bool depthSearch(InfType val, bool priority = DEPTH_SEARCH_LEFT) // поиск в глубину по инф части
	{
		return (priority == DEPTH_SEARCH_LEFT) ? depthSearchLeft(val, root) != NULL : depthSearchRight(val, root) != NULL;
	}

	bool breadthSearch(InfType val) // поиск в ширину
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
				else p = p->nextPtr;
			}
		}

		return false;
	}

	void actionOnElement(BaseAction<InfType>* act) // действие над элементом
	{
		inOrderAction(act, root);
	}

	void inOrderAction(BaseAction<InfType>* act, TElem<InfType> *elem)
	{
		if (elem == NULL)
			return;
		inOrderAction(act, elem->leftPtr);
		(*act)(elem->ID, elem->inf);
		inOrderAction(act, elem->rightPtr);
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

	/*BaseAction<int> *p = new Print<int>;
	BaseAction<int> *g = new ChangeInf<int>;
	t1.printLevel();
	t1.actionOnElement(g);
	t1.actionOnElement(p);
	delete p;
	delete q;*/

	return 0;
}