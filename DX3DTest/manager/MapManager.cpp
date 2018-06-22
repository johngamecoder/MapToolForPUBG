#include "stdafx.h"
#include "MapManager.h"

MapManager::MapManager()
	:m_pCurrentMap(NULL)
{
}


MapManager::~MapManager()
{
}

void MapManager::AddMap(string mapName, IMap * pMap)
{
	if (m_mapMap.count(mapName) == 1)
		return;

	m_mapMap[mapName] = pMap;
}

void MapManager::SetCurrentMap(string mapName)
{
	m_pCurrentMap = m_mapMap[mapName];
}

IMap * MapManager::GetCurrentMap()
{
	return m_pCurrentMap;
}

void MapManager::Destroy()
{
	/*
	for (auto p : m_mapMap)
	{
		SAFE_RELEASE(p.second);
	}
	*/
	m_mapMap.clear();
}
