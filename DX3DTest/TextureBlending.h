#pragma once

class TextureBlending : public IDisplayObject
{
private:
	vector<VERTEX_PCT> m_vecVertex;
	LPDIRECT3DTEXTURE9 m_pTex;
	LPDIRECT3DTEXTURE9 m_pTex1;
	
public:
	TextureBlending();
	~TextureBlending();

	// IDisplayObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

