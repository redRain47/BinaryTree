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
            TLevel* cur = firstLevel;
            while (cur != NULL && number != 1)
            {
                cur = cur->next;
                --number;
            }
            return cur;
        }

        bool delFromLevel(TElem<InfType> *elem, int depth)
        {
            TLevel *curLevel = selectLevel(depth);
            if ((elem != NULL) && (curLevel != NULL))
            {
                TElem<InfType> *next = elem->nextPtr;
                TElem<InfType> *prev = elem->prevPtr;

                if (next != NULL)
                {
                    if (prev == NULL) // если удаляемый элемент первый на уровне
                    {
                        next->prevPtr = NULL;
                        curLevel->firstNode = next;
                    }
                    else  // если удаляемый элемент имеет двух соседей
                    {
                        prev->nextPtr = next;
                        next->prevPtr = prev;
                    }
                }
                else if (prev != NULL) // если удаляемый элемент последний на уровне
                {
                    prev->nextPtr = NULL;
                }
                else // если на уровне нет других элементов, кроме удаляемого
                {
                    curLevel->firstNode = NULL;
                    //delete curLevel;
                }

                return true;
            }

            return false;
        }

        ~Levels() // деструктор списка уровней
        {
            TLevel *cur = firstLevel;
            while (cur != NULL)
            {
                firstLevel = firstLevel->next;
                delete cur;
                cur = firstLevel;
            }
        }

    };

    Levels levels;

    bool searchKey(int key, int &depth, TElem<InfType> *&parent); // поиск по ключу

    TElem<InfType>* depthSearchLeft(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // поиск в глубину с левым приоритетом

    TElem<InfType>* depthSearchRight(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // поиск в глубину с правым приоритетом

    void inOrderAction(BaseAction<InfType>* act, TElem<InfType> *elem); // симметричный обход дерева с осуществлением указанного действия

    TElem<InfType>* minElem(TElem<InfType> *root, TElem<InfType> *&parent, int &depth); // минимальный элемент поддерева с указанной вершиной

    void delRoot(); // удаление корня

    void delAll(TElem<InfType> *elem); // удаление всех элементов дерева

public:

    BinTree() : root(NULL) {} // конструктор дерева

    inline bool isEmpty() // проверка дерева на пустоту
    {
        return (root == NULL) ? true : false;
    }

    BinTree& operator=(BinTree& bt); // перегруженный оператор присваивания для копирования дерева

    void copyTree(TElem<InfType> *elem); // копирование структуры дерева и всех его внутренних связей

    bool addNode(int key, InfType value); // добаление элемента к дереву

    bool searchElemForKey(int key); // поиск по ключу (интерфейсный метод)

    bool depthSearch(InfType val, bool priority = DEPTH_SEARCH_LEFT); // поиск в глубину по информационной части (интерфейсный метод)

    bool breadthSearch(InfType val); // поиск в ширину

    void actionOnElement(BaseAction<InfType>* act); // определенное пользователем действие над элементом (его информационной частью)

    bool delElemForKey(int key); // удаление элемента дерева по ключу

    bool delElemForData(InfType data); // удаление элемента(ов) дерева по информационной части

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
TElem<InfType>* BinTree<InfType>::minElem(TElem<InfType> *root, TElem<InfType> *&parent, int &depth) // минимальный элемент поддерева с указанной вершиной
{
    if (root != NULL)
    {
        while (root->leftPtr)
        {
            parent = root;
            ++depth;
            root = root->leftPtr;
        }
    }
    return root;
}

template <typename InfType>
void BinTree<InfType>::delRoot() // удаление корня
{
    TElem<InfType> *temp = root;
    if ((root->leftPtr || root->rightPtr) == NULL) // если у корня нет потомков
    {
        root = NULL;
        levels.setFirstLevel(NULL);
        delete temp;
    }
    else if ((root->leftPtr != NULL) && (root->rightPtr == NULL)) // если у корня есть только левый потомок
    {
        root->ID = root->leftPtr->ID;
        root->inf = root->leftPtr->inf;
        temp = root->leftPtr;
        levels.delFromLevel(temp, 2);
        delete temp;
        root->leftPtr = NULL;
    }
    else if ((root->leftPtr == NULL) && (root->rightPtr != NULL)) // если у корня есть только правый потомок
    {
        root->ID = root->rightPtr->ID;
        root->inf = root->rightPtr->inf;
        temp = root->rightPtr;
        levels.delFromLevel(temp, 2);
        delete temp;
        root->rightPtr = NULL;
    }
    else if ((root->leftPtr && root->rightPtr) != NULL) // если есть оба потомка
    {
        TElem<InfType> *parentMin = root, *minNode;
        int depth = 1;
        // ищем минимальный элемент в правом поддереве
        minNode = minElem(root->rightPtr, parentMin, depth);
        // перезаписываем его значения в корня
        root->ID = minNode->ID;
        root->inf = minNode->inf;
        // переводим стрелку либо зануляем
        if (parentMin != root)
        {
            parentMin->leftPtr = minNode->rightPtr;
        }
        else
        {
            root->rightPtr = NULL;
            root->leftPtr->nextPtr = NULL;
        }
        
        // убираем элемент с уровня
        levels.delFromLevel(minNode, depth);
        // грохаем элемент
        delete minNode;
    }
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
BinTree<InfType>& BinTree<InfType>::operator=(BinTree& bt) // перегруженный оператор присваивания для копирования дерева
{
    if (bt.isEmpty() || this == &bt) // проверка на самоприсваивание
        return *this;

    if (!isEmpty()) // очищаем текущее дерево
        delAll(this->root);

    copyTree(bt.root); // собственно, само копирование
}

template <typename InfType>
void BinTree<InfType>::copyTree(TElem<InfType> *elem) // копирование структуры дерева и всех его внутренних связей
{
    if (elem == NULL)
        return;

    addNode(elem->ID, elem->inf);

    copyTree(elem->leftPtr);
    copyTree(elem->rightPtr);
}


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
    int t = 0;
    TElem<InfType> *p;
    return searchKey(key, t, p); 
}


template <typename InfType>
bool BinTree<InfType>::depthSearch(InfType val, bool priority = DEPTH_SEARCH_LEFT) // поиск в глубину по информационной части (интерфейсный метод)
{
    TElem<InfType> *temp;
    return (priority == DEPTH_SEARCH_LEFT) ? depthSearchLeft(val, root, temp) != NULL : depthSearchRight(val, root, temp) != NULL;
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
bool BinTree<InfType>::delElemForKey(int key) // удаление элемента дерева по ключу
{
    TElem<InfType> *parent = NULL;
    int depth = 1;

    if (!searchKey(key, depth, parent)) // если элемент не найден
        return false;

    if (parent == NULL)
    {
        delRoot();
        return true;
    }

    TElem<InfType> *cur = NULL;

    if (key > parent->ID) // если работаем с правым потомком
    {
        cur = parent->rightPtr; 
        if ((cur->leftPtr || cur->rightPtr) == NULL) // если у удаляемого элемента нет потомков
        {
            parent->rightPtr = NULL; 
        }
        else if ((cur->leftPtr != NULL) && (cur->rightPtr == NULL)) // если у удаляемого элемента есть только левый потомок
        {
            parent->rightPtr = cur->leftPtr;
        }
        else if ((cur->leftPtr == NULL) && (cur->rightPtr != NULL)) // если у удаляемого элемента есть только правый потомок
        {
            parent->rightPtr = cur->rightPtr;
        }
        else if ((cur->leftPtr && cur->rightPtr) != NULL) // если есть оба потомка
        {
            TElem<InfType> *parentMin = cur, *minNode;
            // ищем минимальный элемент в правом поддереве
            minNode = minElem(cur->rightPtr, parentMin, depth = 1); 
            // перезаписываем его значения в удаляемый элемент
            cur->ID = minNode->ID;
            cur->inf = minNode->inf;
            // первый случай, если в правом поддереве несколько элементов; второй - если один
            (parentMin != cur) ? parentMin->leftPtr = minNode->rightPtr : parentMin->rightPtr = NULL;
            // готовим к удалению
            cur = minNode;
        }
    }
    else // если работаем с левым потомком
    {
        cur = parent->leftPtr;
        if ((cur->leftPtr || cur->rightPtr) == NULL) // если у удаляемого элемента нет потомков
        {
            parent->leftPtr = NULL;
        }
        else if ((cur->leftPtr != NULL) && (cur->rightPtr == NULL)) // если у удаляемого элемента есть только левый потомок
        {
            parent->leftPtr = cur->leftPtr;
        }
        else if ((cur->leftPtr == NULL) && (cur->rightPtr != NULL)) // если у удаляемого элемента есть только правый потомок
        {
            parent->leftPtr = cur->rightPtr;
        }
        else if ((cur->leftPtr && cur->rightPtr) != NULL) // если есть оба потомка
        {
            TElem<InfType> *parentMin = cur, *minNode;
            // ищем минимальный элемент в правом поддереве
            minNode = minElem(cur->rightPtr, parentMin, depth = 1);
            // перезаписываем его значения в удаляемый элемент
            cur->ID = minNode->ID;
            cur->inf = minNode->inf;
            // первый случай, если в правом поддереве несколько элементов; второй - если один
            (parentMin != cur) ? parentMin->leftPtr = minNode->rightPtr : parentMin->rightPtr = NULL;
            // готовим к удалению
            cur = minNode;
        }
    }
   
    levels.delFromLevel(cur, depth);
    delete cur;
    return true;
}


template <typename InfType>
bool BinTree<InfType>::delElemForData(InfType data) // удаление элемента(ов) дерева по информационной части
{
    TElem<InfType> *cur, *parent;
    bool isFound = false;
    while (cur = depthSearchLeft(data, root, parent))
    {
        isFound = true;
        delElemForKey(cur->ID);
    }
    return isFound;
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
