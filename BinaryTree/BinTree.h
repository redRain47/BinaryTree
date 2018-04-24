#pragma once

#define DEPTH_SEARCH_RIGHT 1
#define DEPTH_SEARCH_LEFT 0

// ��������� �������� ������
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
        TLevel *curLevel;
        int amountOfLevels;

        Levels() : firstLevel(new TLevel) 
        {
        
        } // ����������� ������ �������

        void setFirstLevel(TElem<InfType> *p) // ���������� ������ ������� ������
        {
            firstLevel->firstNode = p;
            curLevel = firstLevel;
            amountOfLevels = 1;
        }

        void addLevel(TElem<InfType> *elem) // �������� ����� �������
        {
            TLevel *temp = firstLevel;
            if (temp->firstNode != NULL)
            {
                while (temp->next != NULL)
                    temp = temp->next;
                temp->next = new TLevel(elem);
                ++amountOfLevels;
            }
            else
                temp->firstNode = elem;
        }

        void insertIntoLevel(TElem<InfType> *newElem) // ��������� ����������� � ������ ������� � ��������������� �������
        {
                if (curLevel != NULL)
                {
                    TElem<InfType>* curElem = curLevel->firstNode;
                    while (curElem->nextPtr != NULL && newElem->ID > curElem->ID) // ������� �� ������� ��������
                    {
                        curElem = curElem->nextPtr;
                    }

                    if (newElem->ID < curElem->ID)
                    {
                        // ��������� ������� � ����������� ��������
                        newElem->nextPtr = curElem;
                        curElem->prevPtr = newElem;

                        if (curElem->prevPtr == NULL) // ���������� � ������ ������
                        {
                            curLevel->firstNode = newElem; // ������ ����������� ������� ������� ������
                        }
                        else // ������� � �������
                        {
                            curElem->prevPtr->nextPtr = newElem;
                            newElem->prevPtr = curElem->prevPtr;
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
                    addLevel(newElem);
                }
        }

        bool delFromLevel(TElem<InfType> *elem)
        {
            if ((elem != NULL) && (curLevel != NULL))
            {
                TElem<InfType> *next = elem->nextPtr;
                TElem<InfType> *prev = elem->prevPtr;

                if (next != NULL)
                {
                    if (prev == NULL) // ���� ��������� ������� ������ �� ������
                    {
                        next->prevPtr = NULL;
                        curLevel->firstNode = next;
                    }
                    else  // ���� ��������� ������� ����� ���� �������
                    {
                        prev->nextPtr = next;
                        next->prevPtr = prev;
                    }
                }
                else if (prev != NULL) // ���� ��������� ������� ��������� �� ������
                {
                    prev->nextPtr = NULL;
                }
                else // ���� �� ������ ��� ������ ���������, ����� ����������
                {
                    curLevel->firstNode = NULL;
                    //delete curLevel;
                }

                return true;
            }

            return false;
        }

        bool operator++ ()
        {
            return (curLevel != NULL) ? curLevel = curLevel->next : false;
        }

        ~Levels() // ���������� ������ �������
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

    bool searchKey(int key, TElem<InfType> *&parent); // ����� �� �����

    TElem<InfType>* depthSearchLeft(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // ����� � ������� � ����� �����������

    TElem<InfType>* depthSearchRight(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // ����� � ������� � ������ �����������

    void inOrderAction(BaseAction<InfType>* act, TElem<InfType> *elem); // ������������ ����� ������ � �������������� ���������� ��������

    void delAll(TElem<InfType> *elem); // �������� ���� ��������� ������

public:

    BinTree() : root(NULL) {} // ����������� ������

    BinTree(BinTree &bt) : root(NULL) // ����������� ����������� ������
    {
        *this = bt;
    }

    inline bool isEmpty() // �������� ������ �� �������
    {
        return (root == NULL);
    }

    BinTree& operator=(BinTree& bt); // ������������� �������� ������������ ��� ����������� ������

    void copyTree(TElem<InfType> *elem); // ����������� ��������� ������ � ���� ��� ���������� ������

    bool addNode(int key, InfType value); // ��������� �������� � ������

    bool searchElemForKey(int key); // ����� �� ����� (������������ �����)

    bool depthSearch(InfType val, bool priority = DEPTH_SEARCH_LEFT); // ����� � ������� �� �������������� ����� (������������ �����)

    bool breadthSearch(InfType val); // ����� � ������

    void actionOnElement(BaseAction<InfType>* act); // ������������ ������������� �������� ��� ��������� (��� �������������� ������)

    bool delElemForKey(int key); // �������� �������� ������ �� �����

    bool delElemForData(InfType data); // �������� ��������(��) ������ �� �������������� �����

    void delAllElems(); // ������� ��� ��������

    int getDepth(); // ������ ������� ������

    ~BinTree(); // ���������� ������
};


// ������ ������ BinTree //

// ��������� ������
//----------------------------------------------------------------------------------------------------
template <typename InfType>
bool BinTree<InfType>::searchKey(int key, TElem<InfType> *&parent) // ����� �� �����
{
    TElem<InfType> *cur = root;
    levels.curLevel = levels.firstLevel;
    while (cur != NULL) // ���� �� ������ �� ����� 
    {
        if (key == cur->ID) // ���� ����� ���������, �� ������� ������
            return true;
        parent = cur; // ���������� ����� ��������
        if (levels.curLevel != NULL)
            //levels.curLevel = levels.curLevel->next;
            ++levels;

        cur = key > cur->ID ? cur->rightPtr : cur->leftPtr;
    }
    return false; // ���� ����� �� ����� � �� ����� �������
}


template <typename InfType>
TElem<InfType>* BinTree<InfType>::depthSearchLeft(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent)
{
    if (cur == NULL || cur->inf == val) // ������� ������
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
void BinTree<InfType>::inOrderAction(BaseAction<InfType>* act, TElem<InfType> *elem) // ������������ ����� ������ � �������������� ���������� ��������
{
    if (elem != NULL)
    {
        inOrderAction(act, elem->leftPtr);
        (*act)(elem->ID, elem->inf);
        inOrderAction(act, elem->rightPtr);
    }
}


template <typename InfType>
void BinTree<InfType>::delAll(TElem<InfType> *elem) // �������� ���� ��������� ������
{
    if (elem != NULL)
    {
        delAll(elem->leftPtr);
        delAll(elem->rightPtr);
        delete elem;
    }
}
//----------------------------------------------------------------------------------------------------

// ��������� ������
//----------------------------------------------------------------------------------------------------
template <typename InfType>
BinTree<InfType>& BinTree<InfType>::operator=(BinTree& bt) // ������������� �������� ������������ ��� ����������� ������
{
    if (this == &bt) // �������� �� ����������������
        return *this;

    delAllElems();

    levels.curLevel = levels.firstLevel;
    copyTree(bt.root); // ����������, ���� �����������

    return *this;
}

template <typename InfType>
void BinTree<InfType>::copyTree(TElem<InfType> *elem) // ����������� ��������� ������ � ���� ��� ���������� ������
{
    if (elem != NULL)
    {
        //addNode(elem->ID, elem->inf);

        copyTree(elem->leftPtr);
        copyTree(elem->rightPtr);
    }
}


template <typename InfType>
bool BinTree<InfType>::addNode(int key, InfType value)
{
    TElem<InfType>*	parent = NULL;
    if (!searchKey(key, parent))
    {
        TElem<InfType> *cur = new TElem<InfType>(key, value);

        if (this->root != NULL)// ������� �� ���������������� ����� � ��������� ��� �������
        {
            (key > parent->ID) ? parent->rightPtr = cur : parent->leftPtr = cur;
            levels.insertIntoLevel(cur);
        }
        else // ���� ������ ������
        {
            this->root = cur; // ����� ������
            levels.setFirstLevel(this->root);
        }

        return true;
    }

    return false;
}


template <typename InfType>
bool BinTree<InfType>::searchElemForKey(int key) // ����� �� ����� (������������ �����)
{
    TElem<InfType> *p;
    return searchKey(key, p); 
}


template <typename InfType>
bool BinTree<InfType>::depthSearch(InfType val, bool priority = DEPTH_SEARCH_LEFT) // ����� � ������� �� �������������� ����� (������������ �����)
{
    TElem<InfType> *temp;
    return (priority == DEPTH_SEARCH_LEFT) ? depthSearchLeft(val, root, temp) != NULL : depthSearchRight(val, root, temp) != NULL;
}


template <typename InfType>
bool BinTree<InfType>::breadthSearch(InfType val) // ����� � ������
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
void BinTree<InfType>::actionOnElement(BaseAction<InfType>* act) // ������������ ������������� �������� ��� ��������� (��� �������������� ������)
{
    inOrderAction(act, root);
}


template <typename InfType>
bool BinTree<InfType>::delElemForKey(int key) // �������� �������� ������ �� �����
{
    TElem<InfType> *parent = NULL;

    if (!searchKey(key, parent)) // ���� ������� �� ������
        return false;

    if (parent == NULL)
    {
        //delRoot();
        return true;
    }

    TElem<InfType> *cur = NULL;

    if (key > parent->ID) // ���� �������� � ������ ��������
    {
        cur = parent->rightPtr; 
        if ((cur->leftPtr || cur->rightPtr) == NULL) // ���� � ���������� �������� ��� ��������
        {
            parent->rightPtr = NULL; 
        }
        else if ((cur->leftPtr != NULL) && (cur->rightPtr == NULL)) // ���� � ���������� �������� ���� ������ ����� �������
        {
            parent->rightPtr = cur->leftPtr;
        }
        else if ((cur->leftPtr == NULL) && (cur->rightPtr != NULL)) // ���� � ���������� �������� ���� ������ ������ �������
        {
            parent->rightPtr = cur->rightPtr;
        }
        else if ((cur->leftPtr && cur->rightPtr) != NULL) // ���� ���� ��� �������
        {
            /*TElem<InfType> *parentMin = cur, *minNode;
            // ���� ����������� ������� � ������ ���������
            minNode = minElem(cur->rightPtr, parentMin, depth = 1); 
            // �������������� ��� �������� � ��������� �������
            cur->ID = minNode->ID;
            cur->inf = minNode->inf;
            // ������ ������, ���� � ������ ��������� ��������� ���������; ������ - ���� ����
            (parentMin != cur) ? parentMin->leftPtr = minNode->rightPtr : parentMin->rightPtr = NULL;
            // ������� � ��������
            cur = minNode;*/
        }
    }
    else // ���� �������� � ����� ��������
    {
        cur = parent->leftPtr;
        if ((cur->leftPtr || cur->rightPtr) == NULL) // ���� � ���������� �������� ��� ��������
        {
            parent->leftPtr = NULL;
        }
        else if ((cur->leftPtr != NULL) && (cur->rightPtr == NULL)) // ���� � ���������� �������� ���� ������ ����� �������
        {
            parent->leftPtr = cur->leftPtr;
        }
        else if ((cur->leftPtr == NULL) && (cur->rightPtr != NULL)) // ���� � ���������� �������� ���� ������ ������ �������
        {
            parent->leftPtr = cur->rightPtr;
        }
        else if ((cur->leftPtr && cur->rightPtr) != NULL) // ���� ���� ��� �������
        {
            /*TElem<InfType> *parentMin = cur, *minNode;
            // ���� ����������� ������� � ������ ���������
            minNode = minElem(cur->rightPtr, parentMin, depth = 1);
            // �������������� ��� �������� � ��������� �������
            cur->ID = minNode->ID;
            cur->inf = minNode->inf;
            // ������ ������, ���� � ������ ��������� ��������� ���������; ������ - ���� ����
            (parentMin != cur) ? parentMin->leftPtr = minNode->rightPtr : parentMin->rightPtr = NULL;
            // ������� � ��������
            cur = minNode;*/
        }
    }
   
    levels.delFromLevel(cur, depth);
    delete cur;
    return true;
}


template <typename InfType>
bool BinTree<InfType>::delElemForData(InfType data) // �������� ��������(��) ������ �� �������������� �����
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
void BinTree<InfType>::delAllElems() // ������� ��� ��������
{
    if (root != NULL)
    {
        delAll(root);
        root = NULL;
    }
}


template <typename InfType>
int BinTree<InfType>::getDepth() // ������ ������� ������
{
    return levels.amountOfLevels;
}


template <typename InfType>
BinTree<InfType>::~BinTree() // ����������
{
    delAll(this->root);
}
//----------------------------------------------------------------------------------------------------
