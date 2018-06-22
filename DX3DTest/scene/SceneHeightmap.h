#pragma once

class HeightMap;
class AseUnit;

class SceneHeightmap :	public IScene
{
public:
	HeightMap*	m_pHeightMap;
	AseUnit *	m_pAseUnit;

	SceneHeightmap();
	~SceneHeightmap();
	
	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

