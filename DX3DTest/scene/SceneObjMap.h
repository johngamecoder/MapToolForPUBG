#pragma once

class ObjMap;
class Cubeman;

class SceneObjMap : public IScene
{
public:
	ObjMap *				m_pObjMap;
	Cubeman*				m_pCubeman;

	SceneObjMap();
	~SceneObjMap();
	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

