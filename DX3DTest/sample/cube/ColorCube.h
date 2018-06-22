#pragma once

class ColorCube : public IDisplayObject
{
private:
	vector<VERTEX_PC>		m_vecVertex;
	vector<WORD>			m_vecIndex;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	bool					m_useIndex;
	bool					m_useBuffer;

public:
	ColorCube();
	~ColorCube();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetVertex(vector<VERTEX_PC> &vecOut, vector<D3DXVECTOR3> vecPos);
	void SetVertex(vector<VERTEX_PC> &vecOut, vector<WORD> &vecIndex, vector<D3DXVECTOR3> vecPos);
	void SetBuffer(LPDIRECT3DVERTEXBUFFER9 &pVb, vector<VERTEX_PC> &vecVertex);
	void SetBuffer(LPDIRECT3DVERTEXBUFFER9 &pVb, LPDIRECT3DINDEXBUFFER9 &pIb,
		vector<VERTEX_PC> &vecVertex, vector<WORD> &vecIndex);
};

