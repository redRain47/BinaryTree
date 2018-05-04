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
    public:
        TLevel *firstLevel;
        TLevel *lastLevel;
        TLevel *curLevel;
        int amountOfLevels;

        Levels() : firstLevel(new TLevel) 
        {
        
        } // конструктор списка уровней

        void setFirstLevel(TElem<InfType> *p) // установить первый уровень дерева
        {
            firstLevel->firstNode = p;
            curLevel = firstLevel;
            lastLevel = firstLevel;
            amountOfLevels = 1;
        }

        void addLevel(TElem<InfType> *elem) // добавить новый уровень
        {
            if (lastLevel->firstNode != NULL)
            {
                lastLevel->next = new TLevel(elem);
                ++amountOfLevels;
                lastLevel = lastLevel->next;
            }
            else
            {
                lastLevel->firstNode = elem;
            }
        }

        void insertIntoLevel(TElem<InfType> *newElem) // поместить добавляемый к дереву элемент в соответствующий уровень
        {
                if (curLevel != NULL)
                {
                    TElem<InfType>* curElem = curLevel->firstNode;
                    while (curElem->nextPtr != NULL && newElem->ID > curElem->ID) // доходим до нужного элемента
                    {
                        curElem = curElem->nextPtr;
                    }

                    if (newElem->ID < curElem->ID)
                    {
                        // связываем текущий и добавляемый элементы
                        newElem->nextPtr = curElem;

                        if (curElem->prevPtr == NULL) // добавление в начало уровня
                        {
                            curLevel->firstNode = newElem; // делаем добавляемый элемент головой уровня
                            curElem->prevPtr = newElem;
                        }
                        else // вставка в уровень
                        {
                            curElem->prevPtr->nextPtr = newElem;
                            newElem->prevPtr = curElem->prevPtr;
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
                    curLevel = lastLevel;
                }
        }

        bool delFromLevel(TElem<InfType> *elem)
        {
            if (elem != NULL)
            {
                TElem<InfType> *next = elem->nextPtr;
                TElem<InfType> *prev = elem->prevPtr;

                if (next != NULL)
                {
                    if (prev == NULL) // если удаляемый элемент первый на уровне
                    {
                        next->prevPtr = NULL;
                        lastLevel->firstNode = next;
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
                    TLevel *prevLevel = curLevel;

                    while (prevLevel->next != lastLevel)
                    {
                        prevLevel = prevLevel->next;
                    }
                    
                    delete lastLevel;
                    prevLevel->next = NULL;
                    lastLevel = prevLevel;
                }

                return true;
            }

            return false;
        }

        bool operator++ ()
        {
            return (curLevel != NULL) ? curLevel = curLevel->next : false;
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

    inline TElem<InfType>* newElem(int key, InfType data); // выделение памяти под элемент

    bool searchKey(int key, TElem<InfType> *&parent); // поиск по ключу

    TElem<InfType>* depthSearchLeft(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // поиск в глубину с левым приоритетом

    TElem<InfType>* depthSearchRight(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // поиск в глубину с правым приоритетом

    void inOrderAction(BaseAction<InfType>* act, TElem<InfType> *elem); // симметричный обход дерева с осуществлением указанного действия

    void copyTree(TElem<InfType> *elem); // копирование структуры дерева и всех его внутренних связей

    void delElem(TElem<InfType> *elem, TElem<InfType> *parent); // удаление элемента (рекурсивное)

    void delAll(TElem<InfType> *elem); // удаление всех элементов дерева

public:

    BinTree() : root(NULL) {} // конструктор дерева

    BinTree(BinTree &bt) : root(NULL) // конструктор копирования дерева
    {
        *this = bt;
    }

    inline bool isEmpty() // проверка дерева на пустоту
    {
        return (root == NULL);
    }

    BinTree& operator=(BinTree& bt); // перегруженный оператор присваивания для копирования дерева

    bool addNode(int key, InfType value); // добаление элемента к дереву

    bool searchElemForKey(int key); // поиск по ключу (интерфейсный метод)

    bool depthSearch(InfType val, bool priority = DEPTH_SEARCH_LEFT); // поиск в глубину по информационной части (интерфейсный метод)

    bool breadthSearch(InfType val); // поиск в ширину

    void actionOnElement(BaseAction<InfType>* act); // определенное пользователем действие над элементом (его информационной частью)

    bool delElemForKey(int key); // удаление элемента дерева по ключу

    bool delElemForData(InfType data); // удаление элемента(ов) дерева по информационной части

    void delAllElems(); // удалить все элементы

    int getDepth(); // геттер глубины дерева

    ~BinTree(); // деструктор дерева
};


// Методы класса BinTree //

// Приватные методы
//----------------------------------------------------------------------------------------------------
template <typename InfType>
inline TElem<InfType>* BinTree<InfType>::newElem(int key, InfType data) // выделение памяти под элемент
{
    //TElem<InfType>* elem = new TElem<InfType>(key, data);
    return new TElem<InfType>(key, data);
}


template <typename InfType>
bool BinTree<InfType>::searchKey(int key, TElem<InfType> *&parent) // поиск по ключу
{
    TElem<InfType> *cur = root;
    levels.curLevel = levels.firstLevel;
    
    while (cur != NULL) // пока не дойдем до листа 
    {
        if (key == cur->ID) // если ключи совпадают, то элемент найден
            return true;

        parent = cur; // запоминаем адрес родителя
        ++levels;
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
    if (elem != NULL)
    {
        inOrderAction(act, elem->leftPtr);
        (*act)(elem->ID, elem->inf);
        inOrderAction(act, elem->rightPtr);
    }
}

//--------------------- вынести new в отдельный метод и сделать его inline -------------------------------
template <typename InfType>
void BinTree<InfType>::copyTree(TElem<InfType> *elem) // копирование структуры дерева и всех его внутренних связей
{
    if (elem != NULL)
    {
        TElem<InfType> *desc = NULL;
        TElem<InfType> *descCopy = NULL;
        TElem<InfType> *parent = NULL;
        TElem<InfType> *nextParent = NULL;
        TElem<InfType> *parentCopy = NULL;
        TElem<InfType> *nextParentCopy;

        //root = new TElem<InfType>(elem->ID, elem->inf);
        root = newElem(elem->ID, elem->inf);
        parent = root;
        levels.setFirstLevel(root);

        parentCopy = elem;
        descCopy = (elem->leftPtr) ? elem->leftPtr : elem->rightPtr;

        while (descCopy != NULL) // перемещение по списку уровней на уровень ниже на каждой итерации
        {
            // копируем первый элемент на уровне
            //desc = new TElem<InfType>(descCopy->ID, descCopy->inf);
            desc = newElem(descCopy->ID, descCopy->inf);
            levels.addLevel(desc);
            (desc->ID < parent->ID) ? parent->leftPtr = desc : parent->rightPtr = desc;
            nextParent = desc;
            nextParentCopy = descCopy;

            // идем по остальным
            while (descCopy->nextPtr != NULL) // перемещение на один элемент по уровню на каждой итерации
            {
                descCopy = descCopy->nextPtr;
                //desc->nextPtr = new TElem<InfType>(descCopy->ID, descCopy->inf);
                desc->nextPtr = newElem(descCopy->ID, descCopy->inf);
                desc->nextPtr->prevPtr = desc;
                desc = desc->nextPtr;
                while (parent != NULL) // подбираем родителя P.S. условие - чистая формальность, прокатило бы и true
                {
                    if (desc->ID < parent->ID)
                    {
                        parent->leftPtr = desc;
                        break;
                    }
                    else if (descCopy == parentCopy->rightPtr)
                    {
                        parent->rightPtr = desc;
                        break;
                    }

                    parent = parent->nextPtr;
                    parentCopy = parentCopy->nextPtr;
                }
            }

            parent = nextParent; // перемещаем уровень родителей текущего дерева
            parentCopy = nextParentCopy; // перемещаем уровень родителей копируемого дерева
            while (parentCopy != NULL)
            {
                if (descCopy = (parentCopy->leftPtr) ? parentCopy->leftPtr : parentCopy->rightPtr) // перемещаем уровень потомков копируемого дерева
                {
                    break;
                }
                parentCopy = parentCopy->nextPtr;
                parent = parent->nextPtr;
            }
            desc = NULL;
        }
    }
}


template <typename InfType>
void BinTree<InfType>::delElem(TElem<InfType> *elem, TElem<InfType> *parent) // удаление элемента (рекурсивное)
{
    if ((elem->leftPtr || elem->rightPtr) == NULL) // если у удаляемого элемента нет потомков
    {
        levels.delFromLevel(elem);
        if (parent != NULL)
        {
            (elem == parent->rightPtr) ? parent->rightPtr = NULL : parent->leftPtr = NULL;
        }
        delete elem;
    }
    else if ((elem->leftPtr && elem->rightPtr) != NULL) // если есть оба потомка
    {
        TElem<InfType> *cur = elem->rightPtr;
        TElem<InfType> *parentCur = elem;
        while (cur->leftPtr != NULL)
        {
            parentCur = cur;
            cur = cur->leftPtr;
        }
        elem->ID = cur->ID;
        elem->inf = cur->inf;
        delElem(cur, parentCur);
    }
    else if (elem->rightPtr != NULL) // если у удаляемого элемента есть правый потомок
    {
        elem->ID = elem->rightPtr->ID;
        elem->inf = elem->rightPtr->inf;
        delElem(elem->rightPtr, elem);
    }
    else if (elem->leftPtr != NULL) // если у удаляемого элемента есть левый потомок
    {
        elem->ID = elem->leftPtr->ID;
        elem->inf = elem->leftPtr->inf;
        delElem(elem->leftPtr, elem);
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
    if (this == &bt) // проверка на самоприсваивание
        return *this;

    delAllElems(); // очищаем текущее дерево

    copyTree(bt.root); // собственно, само копирование

    return *this;
}


template <typename InfType>
bool BinTree<InfType>::addNode(int key, InfType value)
{
    TElem<InfType>*	parent = NULL;
    if (!searchKey(key, parent))
    {
        TElem<InfType> *cur = newElem(key, value);//new TElem<InfType>(key, value);

        if (this->root != NULL)// доходим до соответствующего листа и добавляем ему потомка
        {
            (key > parent->ID) ? parent->rightPtr = cur : parent->leftPtr = cur;
            levels.insertIntoLevel(cur);
        }
        else // если дерево пустое
        {
            this->root = cur; // садим дерево
            levels.setFirstLevel(this->root);
        }

        return true;
    }

    return false;
}


template <typename InfType>
bool BinTree<InfType>::searchElemForKey(int key) // поиск по ключу (интерфейсный метод)
{
    TElem<InfType> *p;
    return searchKey(key, p); 
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
    if (root != NULL)
    {
        TElem<InfType> *p = root;
        TLevel *curLevel = levels.firstLevel;

        while (curLevel != NULL)
        {
            p = curLevel->firstNode;
            while (p != NULL)
            {
                if (p->inf == val)
                    return true;
                p = p->nextPtr;
            }
            curLevel = curLevel->next;
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

    if (searchKey(key, parent)) // если элемент найден
    {
        if (parent != NULL) 
        {
            TElem<InfType> *cur = (key > parent->ID) ? parent->rightPtr : parent->leftPtr;
            delElem(cur, parent);
        }
        else // если удаляемый элемент - корень
        {
            delElem(root, parent); 
        }

        return true;
    }

    return false;
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
void BinTree<InfType>::delAllElems() // удалить все элементы
{
    if (root != NULL)
    {
        delAll(root);
        root = NULL;
        levels.~Levels();
    }
}


template <typename InfType>
int BinTree<InfType>::getDepth() // геттер глубины дерева
{
    return levels.amountOfLevels;
}


template <typename InfType>
BinTree<InfType>::~BinTree() // деструктор
{
    delAll(this->root);
}
//----------------------------------------------------------------------------------------------------
