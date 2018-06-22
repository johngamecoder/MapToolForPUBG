#pragma once

class SceneTest : public IScene
{
public:
	LPDIRECT3DCUBETEXTURE9 m_pCubemap;

	SceneTest();
	~SceneTest();

	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

