#pragma once

class Pyramid;

class Grid : public IDisplayObject
{
private:
	vector<VERTEX_PC>	m_vecVertex;
	vector<Pyramid*>	m_vecPyramid;

    //bool    m_bShowGrid =false;
	
public:
	Grid();
	~Grid();

	void Init();
    void Update();
	void Render();
};

