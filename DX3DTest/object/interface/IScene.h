#pragma once

class IDisplayObject;

class IScene : public IObject
{
protected:
	set<IDisplayObject*> m_simpleDisplayList;

	IScene() {}
	virtual ~IScene() {}
public:
	
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	
	void AddSimpleObj(IDisplayObject* p) { m_simpleDisplayList.insert(p); }
	void RemoveSimpleObj(IDisplayObject* p) { m_simpleDisplayList.erase(p); }
	void OnUpdateIScene() { for (auto p : m_simpleDisplayList) SAFE_UPDATE(p); }
	void OnRenderIScene() { for (auto p : m_simpleDisplayList) SAFE_RENDER(p); }
	void OnDestructIScene() { for (auto p : m_simpleDisplayList) SAFE_RELEASE(p); }
};

