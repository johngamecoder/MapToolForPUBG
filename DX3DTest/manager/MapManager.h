#pragma once
#define g_pMapManager MapManager::GetInstance()
#define g_pCurrentMap MapManager::GetInstance()->GetCurrentMap()

class IMap;

class MapManager
{
	SINGLETON(MapManager)
public:
	IMap *				m_pCurrentMap;
	map<string, IMap*>	m_mapMap;

	void	AddMap(string mapName, IMap* pMap);
	void	SetCurrentMap(string mapName);
	IMap*	GetCurrentMap();
	void	Destroy();
	
};
