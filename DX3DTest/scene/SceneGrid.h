#pragma once

class SceneGrid : public IScene
{
public:
	SceneGrid();
	~SceneGrid();

	// IScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetLight();
};

