#if !defined FUNTAB_H
#define FUNTAB_H
//------------------------------------
//  funtab.h
//  (c) Bartosz Milewski, 1994
//------------------------------------
class SymbolTable;

typedef double (*PFun) (double);

namespace Function
{
	class Table
	{
	public:
		Table (SymbolTable & symTab);
		~Table () { delete []_pFun; }
		int Size () const { return _size; }
		PFun GetFun (int id) { return _pFun [id]; }
	private:
		PFun * _pFun;
		int    _size;
	};
}

#endif
