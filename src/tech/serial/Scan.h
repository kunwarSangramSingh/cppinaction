#if !defined SCAN_H
#define SCAN_H
//------------------------------------
//  scan.h
//  (c) Bartosz Milewski, 1994
//------------------------------------
#pragma warning (disable:4786)
#include <istream>
#include <string>
#include <cassert>

enum EToken
{
    tEnd,
    tError,
    tNumber,
    tPlus,
    tMult,
    tMinus,
    tDivide,
    tLParen,
    tRParen,
    tAssign,
    tIdent,
	tExclam
};

class Scanner
{
public:
	explicit Scanner (std::istream & in);
	bool IsDone () const { return _token == tEnd; }
	bool IsEmpty () const { return _isEmpty; }
	bool IsCommand () const { return _token == tExclam; }
	EToken	Token () const { return _token; }
	void	Accept ();
	void	AcceptCommand ();
	std::string GetSymbolName ();
	double Number ()
	{
		assert (_token == tNumber);
		return _number;
	}
private:
	void ReadChar ();

	std::istream  & _in;
	int			_look;	// lookahead character
	bool		_isEmpty;
	EToken		_token;
	double		_number;
	std::string	_symbol;
};
#endif
