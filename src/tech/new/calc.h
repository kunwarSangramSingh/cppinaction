#if !defined CALC_H
#define CALC_H
//------------------------------------
//  (c) Bartosz Milewski, 1999
//------------------------------------
#include "Symtab.h"
#include "Funtab.h"
#include "Store.h"
#include "serial.h"

class Calculator: public Serializable
{
	friend class Parser;
public:
	Calculator ()
		: _funTab (_symTab),
		  _store (_symTab)
	{}
	void Serialize (Serializer & out) const;
	void DeSerialize (DeSerializer & in);
	
private:
	Store & GetStore () { return _store; }
	PFun GetFun (int id) const { return _funTab.GetFun (id); }
	bool IsFunction (int id) const { return id < _funTab.Size (); }
	int AddSymbol (std::string const & str)
	{
		return _symTab.ForceAdd (str);
	}
	int FindSymbol (std::string const & str) const
	{
		return _symTab.Find (str);
	}

	SymbolTable 	_symTab;
	Function::Table _funTab;
	Store			_store;
};

#endif
