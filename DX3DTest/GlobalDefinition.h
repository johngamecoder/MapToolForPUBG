#pragma once

extern CString ASSET_PATH;
extern CString SHADER_PATH;


extern vector<D3DXVECTOR3> g_vecCubeVertex;
extern vector<WORD> g_vecCubeIndex;

extern vector<D3DXVECTOR3> g_vecQuadVertex;
extern vector<WORD> g_vecQuadIndex;
extern vector<D3DXVECTOR2> g_vecQuadUV;


#define SINGLETON(class_name)\
private: \
	class_name(); \
	~class_name(); \
public:\
	static class_name* GetInstance()\
	{\
		static class_name instance;\
		return &instance;\
	}


#define SAFE_UPDATE(p) if(p) { (p)->Update(); }
#define SAFE_RENDER(p) if(p) { (p)->Render(); }
#define SAFE_WNDPROC(p) if (p) {(p)->WndProc(hWnd, message, wParam, lParam);}
#define SAFE_RELEASE(p) if(p) {(p)->Release(); (p) = NULL;}
#define SAFE_DELETE(p) if(p) {delete (p); (p) = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p) {delete [] (p); (p) = NULL;}
#define SAFE_ADDREF(p) if(p) { (p)->AddRef(); }

#define VARIATION(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SCREEN_POINT(lParam) LOWORD(lParam), HIWORD(lParam)

#define WHITE	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define BLACK	D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
#define RED		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
#define GREEN	D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE	D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)
#define YELLOW	D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)

enum SCENE_INDEX {
	SCENE_GRID,
	SCENE_OBJ,
	SCENE_HEIGHTMAP,
	SCENE_TEST,
};

enum TAG_DISPLAYOBJECT {
	TAG_PLAYER
};


struct VERTEX_PC
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;

	VERTEX_PC() {}
	VERTEX_PC(D3DXVECTOR3 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE, };
};

struct VERTEX_PT
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;

	VERTEX_PT() {}
	VERTEX_PT(D3DXVECTOR3 _p, D3DXVECTOR2 _t) : p(_p), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1, };
};

struct VERTEX_PCT
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;
	D3DXVECTOR2 t;
	D3DXVECTOR2 t1;

	VERTEX_PCT() {}
	VERTEX_PCT(D3DXVECTOR3 _p, D3DCOLOR _c, D3DXVECTOR2 _t) : p(_p), c(_c), t(_t), t1(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2, };
};

struct VERTEX_RHWC
{
	VERTEX_RHWC() {}
	VERTEX_RHWC(D3DXVECTOR4 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	D3DXVECTOR4 p;
	D3DCOLOR	c;
	
	enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE, };
};

struct VERTEX_PN
{
	D3DXVECTOR3 p;
	D3DXVECTOR3	n;

	VERTEX_PN() {}
	VERTEX_PN(D3DXVECTOR3 _p, D3DXVECTOR3 _n) : p(_p), n(_n) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct VERTEX_PNT
{
	D3DXVECTOR3 p;
	D3DXVECTOR3	n;
	D3DXVECTOR2	t;

	VERTEX_PNT() {}
	VERTEX_PNT(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t) : p(_p), n(_n), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, };
};

struct VERTEX_BLENDING
{
	D3DXVECTOR3 p;
	float		weights[3];
	DWORD		index;
	//D3DXCOLOR	c;
	D3DXVECTOR2	t;

	VERTEX_BLENDING() {}
	VERTEX_BLENDING(D3DXVECTOR3 _p, float* _weights, DWORD _i, D3DXCOLOR _c, D3DXVECTOR2 _t)
		: p(_p), index(_i), t(_t) {
		weights[0] = _weights[0];
		weights[1] = _weights[1];
		weights[2] = _weights[2];
	}

	enum { FVF = D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_TEX1 };
};


struct MTLTEX
{
	D3DMATERIAL9		material;
	LPDIRECT3DTEXTURE9	pTexture;
	int					id;

	MTLTEX() { ZeroMemory(&material, sizeof(D3DMATERIAL9)); pTexture = NULL; }

	void SetMaterial(const D3DMATERIAL9 &_material) { material = _material; }
	D3DMATERIAL9 GetMaterial() { return material; }
	void SetTexture(LPDIRECT3DTEXTURE9 _ptexture) { pTexture = _ptexture; }
	LPDIRECT3DTEXTURE9 GetTexture() { return pTexture; }
};

struct ASE_POS_SAMPLE
{
	int				tick;
	D3DXVECTOR3		position;
};

struct ASE_ROT_SAMPLE
{
	int				tick;
	D3DXQUATERNION	quaternion;
};

struct ASE_SCENE
{
	int numFirstFrame;
	int numLastFrame;
	int FrameSpeed;
	int TicksPerFrame;
};


struct BoundingSphere
{
	D3DXVECTOR3 center;
	float		radius;
	bool		isPicked;

	BoundingSphere():isPicked(false) {}
	BoundingSphere(D3DXVECTOR3 _center, float _radius):center(_center), radius(_radius), isPicked(false) {}
};


struct BoundingBox
{
	D3DXVECTOR3 minPos;
	D3DXVECTOR3 maxPos;
	D3DXVECTOR3 oriPos;

	D3DXVECTOR3 GetMinPos() { return D3DXVECTOR3(oriPos.x + minPos.x, oriPos.y + minPos.y, oriPos.z + minPos.z); }
	D3DXVECTOR3 GetMaxPos() { return D3DXVECTOR3(oriPos.x + maxPos.x, oriPos.y + maxPos.y, oriPos.z + maxPos.z); }

	bool IsIntersected(BoundingBox &box)
	{
		if (GetMinPos().x > box.GetMaxPos().x || GetMaxPos().x < box.GetMinPos().x) return false;
		if (GetMinPos().y > box.GetMaxPos().y || GetMaxPos().y < box.GetMinPos().y) return false;
		if (GetMinPos().z > box.GetMaxPos().z || GetMaxPos().z < box.GetMinPos().z) return false;
		return true;
	}

	bool IsPointInside(D3DXVECTOR3& p)
	{
		if (GetMinPos().x > p.x || GetMaxPos().x < p.x) return false;
		if (GetMinPos().y > p.y || GetMaxPos().y < p.y) return false;
		if (GetMinPos().z > p.z || GetMaxPos().z < p.z) return false;
		return true;
	}
};

DWORD FtoDw(float f);

float GetRandomFloat(float lowBound, float highBound);

namespace DXUtil
{
D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 8.0f);
const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 8.0f);
const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 8.0f);
const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 8.0f);
const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 8.0f);

void ComputeNormal(D3DXVECTOR3* out, D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2);

D3DLIGHT9 InitDirectional(D3DXVECTOR3* dir, D3DXCOLOR* c);
D3DLIGHT9 InitPoint(D3DXVECTOR3* pos, D3DXCOLOR* c);
D3DLIGHT9 InitSpot(D3DXVECTOR3* dir, D3DXVECTOR3* pos, D3DXCOLOR* c);

}