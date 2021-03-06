#if !defined SYMTAB_H
#define SYMTAB_H
//------------------------------------
//  symtab.h
//  (c) Bartosz Milewski, 1994
//------------------------------------
#include <cstring>

const int maxIdFun = 16;

typedef double (*PFun) (double);

class FunctionEntry
{
public:
    PFun pFun;
    char* strFun;
};

extern FunctionEntry funArr [];

class SymbolTable;

class FunctionTable
{
public:
    FunctionTable (SymbolTable& symTab, FunctionEntry funArr []);
    int Size () const { return _size; }
    PFun GetFun (int id) { return _pFun [id]; }
private:
    PFun  _pFun [maxIdFun];
    int   _size;
};


// stores integer id in a link
class Link
{
public:
    Link (Link * pNext, int id)
    : _pNext (pNext), _id (id) {}

    Link *  Next () const { return _pNext; }
    int     Id () const { return _id; }
private:
    Link *  _pNext;
    int     _id;
};

// Linked list of id's stored in links

class List
{
public:
    List ();
    ~List ();
    void Add (int id);
    Link const * GetHead () const { return _pHead; }
private:
    Link * _pHead;
};

// Hash table of strings

class HTable
{
public:
    explicit HTable (int size): _size(size)
    {
        _aList = new List[size];
    }

    ~HTable ()
    {
        delete [] _aList;
    }

    List const & Find (char const * str, int len) const;
    List const & Find (char const * str) const
	{
		return Find (str, strlen (str));
	}
    void Add (char const * str, int len, int id);
    void Add (char const * str, int id)
	{
		Add (str, strlen (str), id);
	}
private:
    int hash (char const * str, int len) const;

    List * _aList;
    int    _size;
};

const int idNotFound = -1;

// String table maps strings to ints
// and ints to strings

class SymbolTable
{
public:
    explicit SymbolTable (int size);
    ~SymbolTable ();
    int ForceAdd (char const * str, int len);
    int Find (char const * str, int len) const;
    char const * GetString (int id) const;
private:
    HTable  _htab;
    int *   _offStr; // offsets of strings in buffer
    int     _size;
    int     _curId;
    char *  _strBuf;
    int     _bufSize;
    int     _curStrOff;
};
#endif
