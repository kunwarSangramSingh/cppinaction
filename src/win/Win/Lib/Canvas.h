#if !defined (CANVAS_H)
#define CANVAS_H
// (c) Bartosz Milewski 2000
#include <windows.h>

namespace Win
{
	class Canvas
	{
	public:
		operator HDC () 
		{ 
			return _hdc; 
		}

		// Point
		void SetPixel (int x, int y, COLORREF color = RGB (0, 0, 0))
		{
			::SetPixel (_hdc, x, y, color);
		}

		void MoveTo (int x, int y)
		{
			::MoveToEx (_hdc, x, y, 0);
		}

		// Line
		void LineTo (int x, int y)
		{
			::LineTo (_hdc, x, y);
		}

		void Line (int x1, int y1, int x2, int y2)
		{
			::MoveToEx (_hdc, x1, y1, 0);
			::LineTo (_hdc, x2, y2);
		}

		// Text
		void Text (int x, int y, char const * buf, int count)
		{
			::TextOut (_hdc, x, y, buf, count);
		}

	protected:
		Canvas(HDC hdc) :_hdc(hdc) {}

		HDC  _hdc;
	};

	// Use for painting after WM_PAINT

	class PaintCanvas: public Canvas
	{
	public:
		PaintCanvas (HWND hwnd)
			:   Canvas (::BeginPaint (hwnd, &_paint)),
			_hwnd (hwnd)
		{}

		~PaintCanvas ()
		{
			::EndPaint(_hwnd, &_paint);
		}

		int Top () const    { return _paint.rcPaint.top; }
		int Bottom () const { return _paint.rcPaint.bottom; }
		int Left () const   { return _paint.rcPaint.left; }
		int Right () const  { return _paint.rcPaint.right; }

	protected:
                 
		PAINTSTRUCT _paint;
		HWND        _hwnd;
	};

	// Device Context
	// Use for painting other than WM_PAINT

	class UpdateCanvas: public Canvas
	{
	public:
		UpdateCanvas (HWND hwnd)
			:   Canvas (::GetDC(hwnd)),
			_hwnd(hwnd)
		{}
    
		~UpdateCanvas ()
		{
			::ReleaseDC (_hwnd, _hdc);
		}

	protected:

		HWND _hwnd;
	};
}

#endif
