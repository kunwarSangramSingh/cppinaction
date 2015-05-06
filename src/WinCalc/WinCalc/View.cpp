#include "View.h"
// Bartosz Milewski (c) 2000
#include "calc.h"
#include "params.h"
#include "Tools.h"

View::View (Win::Dow win, Cmd::Vector * cmdVector, Calculator & calc)
		: _menu (Menu::barItems, *cmdVector),
		  _calc (calc)
{
	Win::StatusBarMaker status (win, 0);
	_status.Init (status.Create ());
	// Create child windows
	Win::ListBoxMaker listBox1 (win, 1);
	_historyView.Init (listBox1.Create ());
	Win::ListBoxMaker listBox2 (win, 2);
	_memoryView.Init (listBox2.Create ());
	Win::EditMaker edit (win, 3);
	_edit.Init (edit.Create ());
	Win::EditMaker result (win, 4);
	result.MakeReadOnly ();
	_result.Init (result.Create ());

	_edit.SubClass (&_editCtrl);

	Font::OemFixed fixed;
	fixed.GetTextSize (_charWidth, _charHeight);
	_wValue = VALUE_CHARS * _charWidth;

	_historyView.SetFont (fixed);
	_memoryView.SetFont (fixed);
	_result.SetFont (fixed);
	_edit.SetFont (fixed);
}

void View::Init (Win::Dow win)
{
	_menu.AttachToWindow (win);
}

void View::Size (int width, int height)
{
	// total width is wMem + wEdit
	// wMem consists of variable name and value
	// wEdit is twice as wide as variable name in memory

	int wVarName = (width - _wValue) / 3;
	// How many characters will fit in variable name?
	_varNameChars = wVarName / _charWidth - 1;
	int wMem = wVarName + _wValue;
	int wEdit = width - wMem;
	int editHeight = _charHeight + 6;
	int statusHeight = _status.Height ();

	_edit.Move    (0,     0,          wEdit, editHeight);
	_result.Move  (wEdit, 0,	      wMem,  editHeight);

	_historyView.Move (0,     editHeight, wEdit, height - editHeight - statusHeight);
	_memoryView.Move  (wEdit, editHeight, wMem,  height - editHeight - statusHeight);

	_status.Move  (0, height - statusHeight, width, statusHeight);   
}

void View::SetResult (char const * str)
{
	_result.SetText (str);
	_edit.Clear ();
}

void View::SetStatus (char const * str)
{
	_status.SetText (str);
}

void View::UpdateHistory (std::string const & str, double result)
{
	_historyView.InsertString (0, str);
}

std::string View::FormatMemoryString (int id)
{
	std::string varStr = _calc.GetVariableName (id);
	double x = _calc.GetVariableValue (id);
	varStr.resize (_varNameChars, ' ');
	varStr += ' ';
	varStr.append (ToString (x));
	return varStr;
}

void View::UpdateMemory ()
{
	int count = _memoryView.GetCount ();
	for (int i = 0; i < count; ++i)
	{
		int id = _memoryView.GetData (i);
		if (_calc.HasValueChanged (id))
		{
			_calc.ResetChange (id);
			std::string varStr = FormatMemoryString (id);
			_memoryView.ReplaceString (i, varStr);
			_memoryView.SetData (i, id);
		}
	}
	int iNew;
	while ((iNew = _calc.FindNewVariable ()) != idNotFound)
	{
		_calc.ResetChange (iNew);
		std::string varStr = FormatMemoryString (iNew);
		int i = _memoryView.AddString (varStr);
		_memoryView.SetData (i, iNew);
	}
}

void View::ReDisplayMemory ()
{
	int count = _memoryView.GetCount ();
	for (int i = 0; i < count; ++i)
	{
		int id = _memoryView.GetData (i);
		std::string varStr = FormatMemoryString (id);
		_memoryView.ReplaceString (i, varStr);
		_memoryView.SetData (i, id);
	}
}

// Turn the return key into IDOK and send to parent
bool EditController::OnKeyDown (int vKey, int flags) throw ()
{
	if (vKey == VK_RETURN)
	{
		_h.GetParent ().SendMessage (WM_COMMAND, 
			MAKEWPARAM (IDOK, 0),
			reinterpret_cast<LPARAM> (static_cast<HWND> (_h)));
		return true;
	}
	return false;
};
