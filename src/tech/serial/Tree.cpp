//------------------------------------
//  tree.cpp
//  (c) Bartosz Milewski, 1994
//------------------------------------

#include "tree.h"
#include "store.h"
#include <cmath>
#include <iostream>
using std::cerr;

double NumNode::Calc () const
{
    return _num;
}

double VarNode::Calc () const
{
	double x = 0.0;
    if (_store.IsInit (_id ))
        x = _store.Value (_id);
	else
		cerr << "Use of uninitialized variable\n";
    return x;
}

void VarNode::Assign (double val)
{
    _store.SetValue (_id, val);
}

bool VarNode::IsLvalue () const
{
    return true;
}

double UMinusNode::Calc () const
{
    return - _pNode->Calc();
}

double FunNode::Calc () const
{
    double val = 0;
    assert (_pFun != 0);
    val = (*_pFun)(_pNode->Calc());
    return val;
}

double AssignNode::Calc () const
{
    double x = _pRight->Calc ();
    _pLeft->Assign (x);
    return x;
}

double SumNode::Calc () const
{
    double sum = 0.0;
	// Version using iterator
	auto_vector<Node>::const_iterator childIt = _aChild.begin ();
	std::vector<bool>::const_iterator isPosIt = _aIsPositive.begin ();
	for (; childIt != _aChild.end (); ++childIt, ++isPosIt)
	{
		assert (isPosIt != _aIsPositive.end ());
        double val = childIt->Calc ();
        if (*isPosIt)
            sum += val;
        else
            sum -= val;
    }
	assert (isPosIt == _aIsPositive.end ());
    return sum;
}

// Notice: the calculation is left associative
double ProductNode::Calc () const
{
    double prod = 1.0;
	// Version using array access
    for (int i = 0; i < _aChild.size (); ++i)
    {
        double val = _aChild [i]->Calc ();
        if (_aIsPositive [i])
            prod *= val;
        else if (val != 0.0)
        {
            prod /= val;
        }
        else
        {
            cerr << "Error: division by zero\n";
            return HUGE_VAL;
        }
    }
    return prod;
}
