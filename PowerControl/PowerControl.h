#pragma once

#include "resource.h"
#include "targetver.h"
#define DONNYLIBUSEDLL
#include <Donny/DonnyLib.h>
#include <Donny/DonnyWindowsLib.h>
using namespace Donny;
using namespace DWindow;

inline bool IsKeyDown(int vKey)
{
	return GetBinaryBit(GetAsyncKeyState(VK_SHIFT), 16);
}
#define GetStringAndSizeFromStr(Str) Str,wcslen(Str)
const int nHButtonW = 100;//Horizontal
const int nHButtonH = 140;//Horizontal
const int nHBTextY = 100;//Horizontal
const int nVButtonW = 270;//Vertical
const int nVButtonH = 50;//Vertical
const int nVButtonPadding = 3;//Vertical
const int nVBTextX = 70;//Vertical
const DFont BTextFont(L"Microsoft Sans Serif", -1, (nHButtonH - nHBTextY) / 2);
bool bHorizontal = false;
enum VerticalStyle {
	VStyle1,
	VStyle2
};
VerticalStyle nVStyle = VStyle1;//Vertical
inline bool InitializeImage(DImage& Image, WORD ImageID)
{
	/*
	DImage OriginalImage;
	OriginalImage.Load(GetModuleHandle(NULL), MAKEINTRESOURCE(ImageID), L"PNG");
	OriginalImage.bTransparent = true;
	Image.Create(OriginalImage.GetWidth(), OriginalImage.GetHeight());
	FillRect(Image, DRect(0, 0, Image.GetWidth(), Image.GetHeight()), GetStockBrush(WHITE_BRUSH));
	return OriginalImage.Draw(Image);
	*/
	Image.Load(GetModuleHandle(NULL), MAKEINTRESOURCE(ImageID), L"PNG");
	Image.bTransparent = true;
	return true;
}
inline bool InitializeForPaint(PDWnd pDWnd, HDC hdc, WRect& Rect, UINT& Format, DImage& Image)
{
	HBRUSH hBrush = CreateSolidBrush(pDWnd->GetBackgroundColor());
	if (bHorizontal)
	{
		Rect = WRect(0, nHBTextY, nHButtonW, nHButtonH);
		Format = DT_CENTER | DT_VCENTER;// | DT_BOTTOM;
		DDDImage CBImage(Image.GetWidth(), Image.GetHeight());
		FillRect(CBImage, DRect(0, 0, Image.GetWidth(), Image.GetHeight()), hBrush);
		Image.Draw(CBImage);
		SetStretchBltMode(hdc, HALFTONE);
		SetBrushOrgEx(hdc, 0, 0, nullptr);
		CBImage.StretchBlt(hdc, DRect(0, 0, nHButtonW, nHBTextY), DRect(0, 0, -1, -1), SRCCOPY);
		//Image.Draw(hdc, DRect(0, 0, nHButtonW, nHBTextY));
	}
	else
	{
		Rect = WRect(nVBTextX, 0, nVButtonW, nVButtonH);
		Format = DT_SINGLELINE | DT_VCENTER | DT_LEFT;
		DDDImage CBImage(Image.GetWidth(), Image.GetHeight());
		FillRect(CBImage, DRect(0, 0, Image.GetWidth(), Image.GetHeight()), hBrush);
		Image.Draw(CBImage);
		SetStretchBltMode(hdc, HALFTONE);
		SetBrushOrgEx(hdc, 0, 0, nullptr);
		CBImage.StretchBlt(hdc, DRect(0, 0, nVButtonH, nVButtonH), DRect(0, 0, -1, -1), SRCCOPY);
		if (nVStyle == VStyle2)
		{
			const int nLineWidth = 1;
			const int nOffsetY = nVButtonH - 1;
			MoveToEx(hdc, 0, nOffsetY, NULL);
			LineTo(hdc, nVButtonW, nOffsetY);
		}
		//Image.Draw(hdc, DRect(0, 0, nVButtonH, nVButtonH));
	}
	DeleteObject(hBrush);
	return true;
}
inline void ExitAfterClick()
{
	PostQuitMessage(0);
}
inline HINSTANCE Shutdown(DStringA Command)
{
	return ShellExecuteA(NULL, "open", "shutdown.exe", Command, NULL, SW_HIDE);
	//return system("start \"\" //b \"shutdown.exe\" " + Command + "&pause");
}
bool bPressingShift = false;
inline bool IsKeyShiftDown()
{
	return bPressingShift;
}
const int DWM_PROCESSSHOTCUT = WM_USER + 1;
inline LRESULT ProcessShotcut(HWND pProcessor, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
	{
					   switch (wParam)
					   {
					   case 0x41://A
					   case 0x43://C
					   case 0x48://H
					   case 0x52://R
					   case 0x53://S
					   case 0x54://T
					   case 0x5A://Z
					   case VK_ESCAPE://ESC
						   return SendMessage(pProcessor, DWM_PROCESSSHOTCUT, wParam, lParam);
					   }
					   break;
	}
	}
	return 0;
}

class DWBShutdown : public DWButton
{
protected:
	virtual LRESULT OnCreate()
	{
		SetBkColor(RGB(50, 150, 255), RGB(70, 190, 250), RGB(255, 255, 255));
		InitializeImage(Image, IDB_SHUTDOWN);
		return 0;
	}
	virtual LRESULT OnClick(WPARAM wParam, LPARAM lParam)
	{
		if (IsKeyShiftDown()) Shutdown("/s /t 0");//system("shutdown /s /t 0");
		else Shutdown("/s /hybrid /t 0");//system("shutdown /s /hybrid /t 0");
		ExitAfterClick();
		return 0;
	}
	virtual LRESULT OnPaint(HDC hdc)
	{
		WRect Rect;
		UINT Format = 0;
		InitializeForPaint(this, hdc, Rect, Format, Image);
		::SetBkColor(hdc, GetBackgroundColor());
		SelectObject(hdc, BTextFont);
		if (IsKeyShiftDown())
		{
			if (bHorizontal) DrawTextW(hdc, GetStringAndSizeFromStr(L"Totally\nShutdown"), &Rect, Format);
			else DrawTextW(hdc, GetStringAndSizeFromStr(L"Totally Shutdown(T)"), &Rect, Format);
		}
		else DrawTextW(hdc, GetStringAndSizeFromStr(L"Shutdown(S)"), &Rect, DT_SINGLELINE | Format);
		return 0;
	}
	virtual LRESULT Proc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		ProcessShotcut(hParent(), message, wParam, lParam);
		/*
		switch (message)
		{
		case WM_KEYDOWN:
		{
		if (wParam == VK_SHIFT) SendMessage(hParent(), message, wParam, lParam);
		break;
		}
		case WM_KEYUP:
		{
		if (wParam == VK_SHIFT) SendMessage(hParent(), message, wParam, lParam);
		break;
		}
		}*/
		return DWButton::Proc(message, wParam, lParam);
	}
private:
	DImage Image;
};
class DWBRestart : public DWButton
{
protected:
	virtual LRESULT OnCreate()
	{
		SetBkColor(RGB(50, 150, 255), RGB(70, 190, 250), RGB(255, 255, 255));
		InitializeImage(Image, IDB_RESTART);
		return 0;
	}
	virtual LRESULT OnClick(WPARAM wParam, LPARAM lParam)
	{
		if (IsKeyShiftDown()) Shutdown("/r /o /t 0");//system("shutdown /r /o /t 0");
		else Shutdown("/r /t 0");//system("shutdown /r /t 0");
		ExitAfterClick();
		return 0;
	}
	virtual LRESULT OnPaint(HDC hdc)
	{
		WRect Rect;
		UINT Format = 0;
		InitializeForPaint(this, hdc, Rect, Format, Image);
		::SetBkColor(hdc, GetBackgroundColor());
		SelectObject(hdc, BTextFont);
		if (IsKeyShiftDown())
		{
			if (bHorizontal) DrawTextW(hdc, GetStringAndSizeFromStr(L"Advance\nRestart"), &Rect, Format);
			else DrawTextW(hdc, GetStringAndSizeFromStr(L"Advance Restart(A)"), &Rect, Format);
		}
		else DrawTextW(hdc, GetStringAndSizeFromStr(L"Restart(R)"), &Rect, DT_SINGLELINE | Format);
		return 0;
	}
	virtual LRESULT Proc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		ProcessShotcut(hParent(), message, wParam, lParam);
		/*
		switch (message)
		{
		case WM_KEYDOWN:
		{
		if (wParam == VK_SHIFT) SendMessage(hParent(), message, wParam, lParam);
		break;
		}
		case WM_KEYUP:
		{
		if (wParam == VK_SHIFT) SendMessage(hParent(), message, wParam, lParam);
		break;
		}
		}*/
		return DWButton::Proc(message, wParam, lParam);
	}
private:
	DImage Image;
};
class DWBSleep : public DWButton
{
protected:
	virtual LRESULT OnCreate()
	{
		SetBkColor(RGB(50, 150, 255), RGB(70, 190, 250), RGB(255, 255, 255));
		InitializeImage(Image, IDB_SLEEP);
		return 0;
	}
	virtual LRESULT OnClick(WPARAM wParam, LPARAM lParam)
	{
		if (IsKeyShiftDown())
			Shutdown("/h");//system("shutdown /h");
		else SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
		ExitAfterClick();
		return 0;
	}
	virtual LRESULT OnPaint(HDC hdc)
	{
		WRect Rect;
		UINT Format = 0;
		InitializeForPaint(this, hdc, Rect, Format, Image);
		::SetBkColor(hdc, GetBackgroundColor());
		SelectObject(hdc, BTextFont);
		if (IsKeyShiftDown())
			DrawTextW(hdc, GetStringAndSizeFromStr(L"Hibernate(H)"), &Rect, DT_SINGLELINE | Format);
		else DrawTextW(hdc, GetStringAndSizeFromStr(L"Standby(Z)"), &Rect, DT_SINGLELINE | Format);
		return 0;
	}
	virtual LRESULT Proc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		ProcessShotcut(hParent(), message, wParam, lParam);
		/*
		switch (message)
		{
		case WM_KEYDOWN:
		{
		if (wParam == VK_SHIFT) SendMessage(hParent(), message, wParam, lParam);
		break;
		}
		case WM_KEYUP:
		{
		if (wParam == VK_SHIFT) SendMessage(hParent(), message, wParam, lParam);
		break;
		}
		}*/
		return DWButton::Proc(message, wParam, lParam);
	}
private:
	DImage Image;
};
class DWBCancel : public DWButton
{
protected:
	virtual LRESULT OnCreate()
	{
		SetBkColor(RGB(50, 150, 255), RGB(70, 190, 250), RGB(255, 255, 255));
		InitializeImage(Image, IDB_CANCEL);
		return 0;
	}
	virtual LRESULT OnClick(WPARAM wParam, LPARAM lParam)
	{
		PostQuitMessage(0);
		return 0;
	}
	virtual LRESULT OnPaint(HDC hdc)
	{
		WRect Rect;
		UINT Format = 0;
		InitializeForPaint(this, hdc, Rect, Format, Image);
		::SetBkColor(hdc, GetBackgroundColor());
		SelectObject(hdc, BTextFont);
		DrawTextW(hdc, GetStringAndSizeFromStr(L"Cancel(C)"), &Rect, DT_SINGLELINE | Format);
		return 0;
	}
	virtual LRESULT Proc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		ProcessShotcut(hParent(), message, wParam, lParam);
		/*
		switch (message)
		{
		case WM_KEYDOWN:
		{
		if (wParam == VK_SHIFT) SendMessage(hParent(), message, wParam, lParam);
		break;
		}
		case WM_KEYUP:
		{
		if (wParam == VK_SHIFT) SendMessage(hParent(), message, wParam, lParam);
		break;
		}
		}*/
		return DWButton::Proc(message, wParam, lParam);
	}
private:
	DImage Image;
};

class DWPC : public DWnd
{
public:
	DResult Create(PDWnd Parent);
protected:
	virtual LRESULT OnDestroy();
	virtual LRESULT Proc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	DWBShutdown ShutdownButton;
	DWBRestart RestartButton;
	DWBSleep SleepButton;
	DWBCancel CancelButton;
};

class DWPCBK : public DWnd
{
public:
	DResult Create(int nCmdShow);
protected:
	virtual LRESULT OnDestroy();
	virtual LRESULT Proc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	DWPC PowerControl;
};