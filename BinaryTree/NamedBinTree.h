#pragma once

#include "BinTree.h"
#include <tchar.h>
#include <string>


template<typename InfType> class NamedBinTree : public BinTree<InfType>
{
protected:
    std::basic_string<TCHAR> binTreeName;

public:
    NamedBinTree(const std::basic_string<TCHAR> str)
    {
        binTreeName = str;
    }

    ~NamedBinTree()
    {
        binTreeName.clear();
    }

    std::basic_string<TCHAR> getBinTreeName() const
    {
        return binTreeName;
    }

    void setBinTreeName(const std::basic_string<TCHAR> str)
    {
        binTreeName = str;
    }

    NamedBinTree& operator=(NamedBinTree& right)
    {
        BinTree::operator=(right);
        binTreeName = right.binTreeName;
        return *this;
    }

    bool operator==(NamedBinTree& right)
    {
        return (binTreeName == right.binTreeName);
    }

    bool operator!=(NamedBinTree& right)
    {
        return !(binTreeName == right.binTreeName);
    }

    bool operator>(NamedBinTree& right)
    {
        return (binTreeName.compare(right) > 0);
    }

    bool operator<(NamedBinTree& right)
    {
        return (binTreeName.compare(right) < 0);
    }

    bool operator>=(NamedBinTree& right)
    {
        return (binTreeName.compare(right) >= 0);
    }

    bool operator<=(NamedBinTree& right)
    {
        return (binTreeName.compare(right) <= 0);
    }
};