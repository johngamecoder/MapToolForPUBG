#include "stdafx.h"
#include "Debug.h"

Debug* Debug::instance = NULL;

Debug * Debug::Get()
{
	if (instance == NULL)
		instance = new Debug();

	return instance;
}

void Debug::Delete()
{
	SAFE_DELETE(instance);
}

Debug::Debug()
{
	D3DXCreateFont(DX::GetDevice(), 12, 6, FW_NORMAL, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("³ª´®°íµñ"), &m_pFont);

	m_bUseRetainStr = false;
}

Debug::~Debug()
{
	SAFE_RELEASE(m_pFont);
}

void Debug::InitText()
{
	m_str.SetString(_T(""));
}


void Debug::_AddText(CString& str)
{
	if (m_bUseRetainStr)
	{
		m_retainStr.Append(str);
	}
	else
	{
		m_str.Append(str);
	}
}

void Debug::AddText(CString & str)
{
	_AddText((str));
}

void Debug::AddText(LPCTSTR str)
{
	_AddText(CString(str));
}

void Debug::AddText(string str)
{
	_AddText(CString(str.c_str()));
}

void Debug::AddText(float str)
{
	_AddText(CString(to_string(str).c_str()));
}

void Debug::AddText(double str)
{
	_AddText(CString(to_string(str).c_str()));
}

void Debug::AddText(int str)
{
	_AddText(CString(to_string(str).c_str()));
}

void Debug::AddText(DWORD str)
{
	_AddText(CString(to_string(str).c_str()));
}

void Debug::AddText(D3DXVECTOR3 str)
{
	string tmp = string(to_string(str.x) + " , " + to_string(str.y) + " , " + to_string(str.z));
	_AddText(CString(tmp.c_str()));
}

void Debug::EndLine()
{
	AddText(_T("\n"));
}

void Debug::Print()
{
	m_str.Append(_T("\n"));
	m_str.Append(m_retainStr);
	
	RECT rc;
	SetRect(&rc, 8, 8, 8, 8);
	m_pFont->DrawText(NULL, m_str, m_str.GetLength(), &rc,
		DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 255));

	if (GetAsyncKeyState('M') & 0x0001)
	{
		ShowMessageBox();
	}
}

void Debug::ShowMessageBox()
{
	MessageBox(NULL, (m_str), _T("Debug"), MB_OK);
}

void Debug::InitRetainStr()
{
	m_retainStr.SetString(_T(""));
}
