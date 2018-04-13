#pragma once

#define DEPTH_SEARCH_RIGHT 1
#define DEPTH_SEARCH_LEFT 0

// структура элемента дерева
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
class BinTree
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

		Levels() : firstLevel(new TLevel) {} // конструктор списка уровней

		/*void setFirstLevel(TElem<InfType> *p); // установить первый уровень дерева

		void addLevel(TElem<InfType> *elem); // добавить новый уровень

		void insertIntoLevel(TElem<InfType> *newElem, int depth); // поместить добавляемый к дереву элемент в соответствующий уровень

		TLevel* selectLevel(int number); // выбрать заданный уровень

		~Levels(); // деструктор списка уровней*/

		void setFirstLevel(TElem<InfType> *p) // установить первый уровень дерева
		{
			firstLevel->firstNode = p;
		}

		void addLevel(TElem<InfType> *elem) // добавить новый уровень
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

		void insertIntoLevel(TElem<InfType> *newElem, int depth) // поместить добавляемый к дереву элемент в соответствующий уровень
		{
			if (newElem != NULL)
			{
				TLevel* curLevel = selectLevel(depth);
				if (curLevel != NULL)
				{
					TElem<InfType>* curElem = curLevel->firstNode;
					while (curElem->nextPtr != NULL && newElem->ID > curElem->ID) // доходим до нужного элемента
					{
						curElem = curElem->nextPtr;
					}

					if (newElem->ID < curElem->ID)
					{
						if (curElem->prevPtr == NULL) // добавление в начало уровня
						{
							newElem->nextPtr = curElem;
							curElem->prevPtr = newElem;
							curLevel->firstNode = newElem; // делаем добавляемый элемент головой уровня
						}
						else // вставка в уровень
						{
							curElem->prevPtr->nextPtr = newElem;
							newElem->prevPtr = curElem->prevPtr;
							newElem->nextPtr = curElem;
							curElem->prevPtr = newElem;
						}
					}
					else if (curElem->nextPtr == NULL) // добавление в конец уровня
					{
						curElem->nextPtr = newElem;
						newElem->prevPtr = curElem;
					}
				}
				else
				{
					addLevel(newElem);
				}
			}
		}

		TLevel* selectLevel(int number) // выбрать заданный уровень
		{
			TLevel* temp = firstLevel;
			while (temp != NULL && number != 1)
			{
				temp = temp->next;
				--number;
			}
			return temp;
		}

		~Levels() // деструктор списка уровней
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

	bool searchKey(int key, int &depth, TElem<InfType> *&parent); // поиск по ключу

	TElem<InfType>* depthSearchLeft(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // поиск в глубину с левым приоритетом

	TElem<InfType>* depthSearchRight(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // поиск в глубину с правым приоритетом

	void inOrderAction(BaseAction<InfType>* act, TElem<InfType> *elem); // симметричный обход дерева с осуществлением указанного действия

	void delAll(TElem<InfType> *elem); // удаление всех элементов дерева

public:
	BinTree() : root(NULL) {} // конструктор дерева

	inline bool isEmpty() // проверка дерева на пустоту
	{
		return (root == NULL) ? true : false;
	}

	/*BinTree& operator=(BinTree& bt)
	{
	if (bt.isEmpty() || *this == &bt)
	return *this;

	if (!isEmpty()) // очищаем текущее дерево
	delAll(this->root);

	TElem<InfType> *copyRoot = bt.getRoot();
	copyTree(copyRoot);
	}

	BinTree& copyTree(TElem<InfType> *elem)
	{
		addNode(elem->ID, elem->inf);
		copyTree(elem->leftPtr);
		copyTree(elem->rightPtr);
	}*/

	bool addNode(int key, InfType value); // добаление элемента к дереву

	bool searchElemForKey(int key); // поиск по ключу (интерфейсный метод)

	bool depthSearch(InfType val, bool priority = DEPTH_SEARCH_LEFT); // поиск в глубину по информационной части (интерфейсный метод)

	bool breadthSearch(InfType val); // поиск в ширину

	void actionOnElement(BaseAction<InfType>* act); // определенное пользователем действие над элементом (его информационной частью)

	int getDepth(); // геттер глубины дерева

	~BinTree(); // деструктор дерева
};

// Методы класса BinTree //

// Приватные методы
//----------------------------------------------------------------------------------------------------
template <typename InfType>
bool BinTree<InfType>::searchKey(int key, int &depth, TElem<InfType> *&parent) // поиск по ключу
{
	TElem<InfType> *cur = root;
	while (cur != NULL) // пока не дойдем до листа 
	{
		if (key == cur->ID) // если ключи совпадают, то элемент найден
			return true;
		parent = cur; // запоминаем адрес родителя
		++depth;
		cur = key > cur->ID ? cur->rightPtr : cur->leftPtr;
	}
	return false; // если дошли до листа и не нашли элемент
}


template <typename InfType>
TElem<InfType>* BinTree<InfType>::depthSearchLeft(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent)
{
	if (cur == NULL || cur->inf == val) // базовый случай
		return cur;
	TElem<InfType> *res;
	parent = cur;
	return  (NULL != (res = depthSearchLeft(val, cur->leftPtr, parent))) ? res : depthSearchLeft(val, cur->rightPtr, parent);

}


template <typename InfType>
TElem<InfType>* BinTree<InfType>::depthSearchRight(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent)
{
	if (cur == NULL || cur->inf == val)
		return cur;
	TElem<InfType> *res;
	parent = cur;
	return  (NULL != (res = depthSearchRight(val, cur->rightPtr, parent))) ? res : depthSearchRight(val, cur->leftPtr, parent);

}


template <typename InfType>
void BinTree<InfType>::inOrderAction(BaseAction<InfType>* act, TElem<InfType> *elem) // симметричный обход дерева с осуществлением указанного действия
{
	if (elem == NULL)
		return;
	inOrderAction(act, elem->leftPtr);
	(*act)(elem->ID, elem->inf);
	inOrderAction(act, elem->rightPtr);
}


template <typename InfType>
void BinTree<InfType>::delAll(TElem<InfType> *elem) // удаление всех элементов дерева
{
	if (elem != NULL)
	{
		delAll(elem->leftPtr);
		delAll(elem->rightPtr);
		delete elem;
	}
}
//----------------------------------------------------------------------------------------------------

// Публичные методы
//----------------------------------------------------------------------------------------------------
template <typename InfType>
bool BinTree<InfType>::addNode(int key, InfType value)
{
	TElem<InfType> *cur = new TElem<InfType>(key, value);

	if (this->root == NULL) // если дерево пустое
	{
		this->root = cur; // садим дерево
		levels.setFirstLevel(this->root);
	}
	else // иначе доходим до соответствующего листа и добавляем ему потомка
	{
		int depth = 1; // глубина добавляемого элемента
		TElem<InfType>*	parent = NULL;

		if (searchKey(key, depth, parent)) // если такой элемент уже существует
		{
			delete cur;
			return false;
		}

		(key > parent->ID) ? parent->rightPtr = cur : parent->leftPtr = cur;

		levels.insertIntoLevel(cur, depth);
	}
	return true;
}


template <typename InfType>
bool BinTree<InfType>::searchElemForKey(int key) // поиск по ключу (интерфейсный метод)
{
	return searchKey(key, NULL, NULL);
}


template <typename InfType>
bool BinTree<InfType>::depthSearch(InfType val, bool priority = DEPTH_SEARCH_LEFT) // поиск в глубину по информационной части (интерфейсный метод)
{
	return (priority == DEPTH_SEARCH_LEFT) ? depthSearchLeft(val, root, NULL) != NULL : depthSearchRight(val, root, NULL) != NULL;
}


template <typename InfType>
bool BinTree<InfType>::breadthSearch(InfType val) // поиск в ширину
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


template <typename InfType>
void BinTree<InfType>::actionOnElement(BaseAction<InfType>* act) // определенное пользователем действие над элементом (его информационной частью)
{
	inOrderAction(act, root);
}


template <typename InfType>
int BinTree<InfType>::getDepth() // геттер глубины дерева
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


template <typename InfType>
BinTree<InfType>::~BinTree() // деструктор
{
	delAll(this->root);
}
//----------------------------------------------------------------------------------------------------
