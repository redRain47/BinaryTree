#pragma once

template <typename InfType>
class BaseAction
{
public:

	virtual void operator() (int id, InfType &val) = 0;

};