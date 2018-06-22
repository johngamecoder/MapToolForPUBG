#pragma once
#include "stdafx.h"

class ObjectManager
{
public:
	static ObjectManager*	Get();
	static void		Delete();
private:
	static ObjectManager*	instance;

	set<IObject*> m_setObject;
	map<WORD, list<IDisplayObject*>> m_tagList;
	list<ParticleSystem*> m_pParticleList;

	ObjectManager();
	~ObjectManager();
public:
	void AddObject(IObject* pObject);
	void RemoveObject(IObject* pObject);

	void AddToTagList(WORD _tag, IDisplayObject* _pObject);
	void RemoveFromTagList(WORD _tag, IDisplayObject* _pObject);
	IDisplayObject* FindObjectByTag(WORD _tag);
	list<IDisplayObject*> FindObjectsByTag(WORD _tag);

	void AddParticle(ParticleSystem* pParticle) { m_pParticleList.push_back(pParticle); }
	list<ParticleSystem*>& GetParticles() { return m_pParticleList; }

};

