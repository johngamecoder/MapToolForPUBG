#include "stdafx.h"
#include "ObjectManager.h"

ObjectManager* ObjectManager::instance = NULL;

ObjectManager * ObjectManager::Get()
{
	if (instance == NULL)
		instance = new ObjectManager();

	return instance;
}

void ObjectManager::Delete()
{
	SAFE_DELETE(instance);
}


ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	for (auto p : m_pParticleList)
		SAFE_RELEASE(p);

	assert(m_setObject.empty() && "생성된 객체 중 삭제되지 않은 객체가 있습니다.");
}

void ObjectManager::AddObject(IObject* pObject)
{
	m_setObject.insert(pObject);
}

void ObjectManager::RemoveObject(IObject* pObject)
{
	m_setObject.erase(pObject);
}

void ObjectManager::AddToTagList(WORD _tag, IDisplayObject * _object)
{
	m_tagList[_tag].push_back(_object);
}

void ObjectManager::RemoveFromTagList(WORD _tag, IDisplayObject* _pObject)
{
	m_tagList[_tag].remove(_pObject);
}

IDisplayObject * ObjectManager::FindObjectByTag(WORD _tag)
{
	if (m_tagList[_tag].empty() == true) return NULL;

	return m_tagList[_tag].front();
}

list<IDisplayObject*> ObjectManager::FindObjectsByTag(WORD _tag)
{
	return m_tagList[_tag];
}

