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
        TLevel *lastLevel;
        TLevel *curLevel;
        int amountOfLevels;

        Levels() : firstLevel(new TLevel) 
        {
        
        } // ����������� ������ �������

        void setFirstLevel(TElem<InfType> *p) // ���������� ������ ������� ������
        {
            firstLevel->firstNode = p;
            curLevel = firstLevel;
            lastLevel = firstLevel;
            amountOfLevels = 1;
        }

        void addLevel(TElem<InfType> *elem) // �������� ����� �������
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

                        if (curElem->prevPtr == NULL) // ���������� � ������ ������
                        {
                            curLevel->firstNode = newElem; // ������ ����������� ������� ������� ������
                            curElem->prevPtr = newElem;
                        }
                        else // ������� � �������
                        {
                            curElem->prevPtr->nextPtr = newElem;
                            newElem->prevPtr = curElem->prevPtr;
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
                    if (prev == NULL) // ���� ��������� ������� ������ �� ������
                    {
                        next->prevPtr = NULL;
                        lastLevel->firstNode = next;
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

    inline TElem<InfType>* newElem(int key, InfType data); // ��������� ������ ��� �������

    bool searchKey(int key, TElem<InfType> *&parent); // ����� �� �����

    TElem<InfType>* depthSearchLeft(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // ����� � ������� � ����� �����������

    TElem<InfType>* depthSearchRight(InfType val, TElem<InfType> *cur, TElem<InfType> *&parent); // ����� � ������� � ������ �����������

    void inOrderAction(BaseAction<InfType>* act, TElem<InfType> *elem); // ������������ ����� ������ � �������������� ���������� ��������

    void copyTree(TElem<InfType> *elem); // ����������� ��������� ������ � ���� ��� ���������� ������

    void delElem(TElem<InfType> *elem, TElem<InfType> *parent); // �������� �������� (�����������)

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
inline TElem<InfType>* BinTree<InfType>::newElem(int key, InfType data) // ��������� ������ ��� �������
{
    //TElem<InfType>* elem = new TElem<InfType>(key, data);
    return new TElem<InfType>(key, data);
}


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

//--------------------- ������� new � ��������� ����� � ������� ��� inline -------------------------------
template <typename InfType>
void BinTree<InfType>::copyTree(TElem<InfType> *elem) // ����������� ��������� ������ � ���� ��� ���������� ������
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

        while (descCopy != NULL) // ����������� �� ������ ������� �� ������� ���� �� ������ ��������
        {
            // �������� ������ ������� �� ������
            //desc = new TElem<InfType>(descCopy->ID, descCopy->inf);
            desc = newElem(descCopy->ID, descCopy->inf);
            levels.addLevel(desc);
            (desc->ID < parent->ID) ? parent->leftPtr = desc : parent->rightPtr = desc;
            nextParent = desc;
            nextParentCopy = descCopy;

            // ���� �� ���������
            while (descCopy->nextPtr != NULL) // ����������� �� ���� ������� �� ������ �� ������ ��������
            {
                descCopy = descCopy->nextPtr;
                //desc->nextPtr = new TElem<InfType>(descCopy->ID, descCopy->inf);
                desc->nextPtr = newElem(descCopy->ID, descCopy->inf);
                desc->nextPtr->prevPtr = desc;
                desc = desc->nextPtr;
                while (parent != NULL) // ��������� �������� P.S. ������� - ������ ������������, ��������� �� � true
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

            parent = nextParent; // ���������� ������� ��������� �������� ������
            parentCopy = nextParentCopy; // ���������� ������� ��������� ����������� ������
            while (parentCopy != NULL)
            {
                if (descCopy = (parentCopy->leftPtr) ? parentCopy->leftPtr : parentCopy->rightPtr) // ���������� ������� �������� ����������� ������
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
void BinTree<InfType>::delElem(TElem<InfType> *elem, TElem<InfType> *parent) // �������� �������� (�����������)
{
    if ((elem->leftPtr || elem->rightPtr) == NULL) // ���� � ���������� �������� ��� ��������
    {
        levels.delFromLevel(elem);
        if (parent != NULL)
        {
            (elem == parent->rightPtr) ? parent->rightPtr = NULL : parent->leftPtr = NULL;
        }
        delete elem;
    }
    else if ((elem->leftPtr && elem->rightPtr) != NULL) // ���� ���� ��� �������
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
    else if (elem->rightPtr != NULL) // ���� � ���������� �������� ���� ������ �������
    {
        elem->ID = elem->rightPtr->ID;
        elem->inf = elem->rightPtr->inf;
        delElem(elem->rightPtr, elem);
    }
    else if (elem->leftPtr != NULL) // ���� � ���������� �������� ���� ����� �������
    {
        elem->ID = elem->leftPtr->ID;
        elem->inf = elem->leftPtr->inf;
        delElem(elem->leftPtr, elem);
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

    delAllElems(); // ������� ������� ������

    copyTree(bt.root); // ����������, ���� �����������

    return *this;
}


template <typename InfType>
bool BinTree<InfType>::addNode(int key, InfType value)
{
    TElem<InfType>*	parent = NULL;
    if (!searchKey(key, parent))
    {
        TElem<InfType> *cur = newElem(key, value);//new TElem<InfType>(key, value);

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

    if (searchKey(key, parent)) // ���� ������� ������
    {
        if (parent != NULL) 
        {
            TElem<InfType> *cur = (key > parent->ID) ? parent->rightPtr : parent->leftPtr;
            delElem(cur, parent);
        }
        else // ���� ��������� ������� - ������
        {
            delElem(root, parent); 
        }

        return true;
    }

    return false;
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
        levels.~Levels();
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
