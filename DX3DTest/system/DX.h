#pragma once

struct D3DInfo
{
	CString appName;
	HINSTANCE hInstance;
	HWND hWnd;
	UINT screenWidth;
	UINT screenHeight;
	bool isFullScreen;
};


class DX
{
private:
	static DX*					instance;
	static LPDIRECT3DDEVICE9	m_pD3DDevice;
	static D3DInfo				d3dInfo;
    //static D3DPRESENT_PARAMETERS d3dpp;

	DX();
	~DX();

public:
	static DX* Get();
	static void Delete();
	static LPDIRECT3DDEVICE9 GetDevice() {return m_pD3DDevice; }
	static D3DInfo& GetInfo() { return d3dInfo; }
    //static D3DPRESENT_PARAMETERS& GetDPP() { return d3dpp; }
	static void SetInfo(D3DInfo info) { d3dInfo = info; }

	HRESULT				Init();
};
/*
http://telnet.or.kr/directx/graphics/dxgraphics.htm
P73 ~P99
Direct3D 는 3D 가속 하드웨어를 이용해 3D 세계 표현을 도와주는 그래픽 API 이다.
Application - Direct3D - HAL(Hardware Abstraction Layer) - Graphics Hardware
Device 에서 지원하지 않지만 D3D에서 제공하는 기능을 REF 장치를 통해 이용할 수 있다.
D3DDEVTYPE_HAL or D3DDEVTYPE_REF
REF 장치는 순수 개발 목적으로 제공된다.DirectX SDK 에만 포함됨.상당히 느림.

COM(Component Object Model)
IUnknown Interface 를 상속받음.세부적인 부분들은 내부적으로 작동하며 자신의 메모리 관리를 스스로 수행한다.
특수 함수를 통해 객체의 포인터를 얻고 Release Method 를 통해 해제한다.(new, delete 키워드 사용 안함)
*/