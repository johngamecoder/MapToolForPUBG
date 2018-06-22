#pragma once
/*
typedef enum {
	//면의 차례를 변경해, 사용되지 않은 정점와 면을 삭제한다.
	D3DXMESHOPT_COMPACT = 0x01000000, 

	//속성 번들 스테이트의 변경을 줄여, ID3DXBaseMesh::DrawSubset 의 퍼포먼스를 
	//올리기(위해)때문에, 면의 차례를 변경해 최적화를 실시한다.
	D3DXMESHOPT_ATTRSORT = 0x02000000, 

	//면의 차례를 변경해, 정점 캐쉬의 캐쉬 히트율을 향상시킨다.
	D3DXMESHOPT_VERTEXCACHE = 0x04000000,
	
	//면의 차례를 변경해, 인접하는 삼각형의 길이를 최대로 한다.
	D3DXMESHOPT_STRIPREORDER = 0x08000000,

	//면만을 최적화해, 정점은 최적화하지 않는다.
	D3DXMESHOPT_IGNOREVERTS = 0x10000000,

	//속성의 소트중에, 속성 그룹간에 공유하고 있는 정점을 분할하지 않는다.
	D3DXMESHOPT_DONOTSPLIT = 0x20000000,

	//정점 캐쉬의 사이즈에 영향을 준다. 이 플래그를 사용하면 
	//종래의 하드웨어로 문제 없게 기능하는 디폴트의 정점 캐쉬 사이즈가 지정된다.
	D3DXMESHOPT_DEVICEINDEPENDENT = 0x40000000
} D3DXMESHOPT;

//최적화 플래그의 D3DXMESHOPT_STRIPREORDER 및 D3DXMESHOPT_VERTEXCACHE 는 각각 서로 배타적이다.
*/

//////////
//FVF
//////////
//	1. Position 정점의 좌표 x,y,z(float)				: D3DFVF_XYZ
// 	2. RHW (float)                                  : D3DFVF_XYZRHW (D3DFVF_XYZ 또는 D3DFVF_NORMAL과 같이 사용불가)
// 	3. Blending Weight Data 결합 가중치 (float)		: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 	4. Vertex Normal 정점의 법선 벡터 x,y,z(float)	: D3DFVF_NORMAL
// 	5. Vertex Point Size 정점의 점 크기 (float)		: D3DFVF_PSIZE
// 	6. Diffuse Color (DWORD)						: D3DFVF_DIFFUSE
// 	7. Specular Color (DWORD)						: D3DFVF_SPECULAR
// 	8. Texture Coordinate Set 1 (float)             : D3DFVF_TEX0 - D3DFVF_TEX8


//////////
//RENDER STATE
//////////
/*
DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
DX::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
DX::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

DX::GetDevice()->SetRenderState(D3DRS_ZENABLE, true);

DX::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
*/