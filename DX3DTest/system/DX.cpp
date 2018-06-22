#include "StdAfx.h"
#include "DX.h"


DX* DX::instance = NULL;
LPDIRECT3DDEVICE9 DX::m_pD3DDevice = NULL;
D3DInfo DX::d3dInfo;

DX * DX::Get()
{
	if (instance == NULL)
		instance = new DX();

	return instance;
}

void DX::Delete()
{
	SAFE_DELETE(instance);
}

DX::DX()
{
}

DX::~DX()
{
	if (m_pD3DDevice)
	{
		ULONG ul = m_pD3DDevice->Release();
		assert(ul == 0 && "디바이스를 이용해서 생성한 객체 중 소멸되지 않은 객체가 있습니다.");
	}
}

HRESULT DX::Init()
{
	//버전 정보를 통해 IDirect3D9 Interface 의 포인터 획득
	LPDIRECT3D9 pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	assert(pD3d != NULL && "헤더 오류");

	D3DCAPS9	caps;	// 장치에 대한 정	보를 얻어옴
	int			vp;		// 버텍스를 처리 하는 방식
	
	//주 그래픽카드의 정보를 D3DCAPS9 에 받아온다.
	if (FAILED(pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
		return E_FAIL;
	
	//하드웨어가 정점처리를 지원하는지 확인
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//생성할 IDirect3DDevice9 interface 의 특성을 지정해준다.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	//Present 시 Swap 된 BackBuffer 의 데이터를 버리고 새로 씀.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//깊이 버퍼 자동 관리
	d3dpp.EnableAutoDepthStencil = TRUE;
	//깊이버퍼 포맷. 지정 비트 값이 높을 수록 정확한 깊이 체크 가능
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	
	DWORD level;
	if (SUCCEEDED(pD3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, D3DFMT_D24S8, TRUE, D3DMULTISAMPLE_4_SAMPLES, &level)))
	{
		d3dpp.MultiSampleQuality = level - 1;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	}
	
	d3dpp.BackBufferCount = 1;
	d3dpp.hDeviceWindow = d3dInfo.hWnd;
	d3dpp.Windowed = !d3dInfo.isFullScreen;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.BackBufferWidth = d3dInfo.screenWidth;
	d3dpp.BackBufferHeight = d3dInfo.screenHeight;

	pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		d3dInfo.hWnd, vp, &d3dpp, &m_pD3DDevice);
	assert(m_pD3DDevice != NULL && "장치생성 실패");
	
	SAFE_RELEASE(pD3d);
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui_ImplWin32_Init(DX::GetInfo().hWnd);
    ImGui_ImplDX9_Init(m_pD3DDevice);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
	return S_OK;
}

