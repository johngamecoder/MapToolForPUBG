#pragma once

class Debug
{
public:
	static Debug*	Get();
	static void		Delete();
private:
	static Debug*	instance;

	LPD3DXFONT	m_pFont;
	CString		m_str;
	CString		m_retainStr;
	bool		m_bUseRetainStr;

	void _AddText(CString& str);

	Debug();
	~Debug();

public:
	void InitText();
	void AddText(CString& str);
	void AddText(LPCTSTR str);
	void AddText(string str);
	void AddText(float str);
	void AddText(double str);
	void AddText(int str);
	void AddText(DWORD str);
	void AddText(D3DXVECTOR3 str);
	void EndLine();
	void Print();
	void ShowMessageBox();
	void InitRetainStr();
	void UsingRetainStr(bool bUse = true) { m_bUseRetainStr = bUse; }
};

