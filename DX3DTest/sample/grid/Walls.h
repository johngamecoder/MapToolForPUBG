#pragma once

class Walls : public IDisplayObject
{
private:
	vector<VERTEX_PN> m_vecVertex;
	vector<VERTEX_PN> m_vecWall0;
	vector<VERTEX_PN> m_vecWall1;
	vector<VERTEX_PN> m_vecWall2;
	vector<VERTEX_PN> m_vecFloor0;
	vector<VERTEX_PN> m_vecFloor1;

public:
	Walls();
	~Walls();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void MakePlane(vector<VERTEX_PN> &vecOut, vector<D3DXVECTOR3> &vecPos, D3DXMATRIXA16 &mat);
	void MakePlaneGroup(vector<VERTEX_PN> &vecOut, vector<D3DXVECTOR3> vecWall);
};

