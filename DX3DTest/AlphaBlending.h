#pragma once

class AlphaBlending : public IDisplayObject
{
private:
	vector<VERTEX_PC> m_vecVertex;
	vector<VERTEX_PC> m_vecVertex1;
	
public:
	AlphaBlending();
	~AlphaBlending();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

