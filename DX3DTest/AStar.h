#pragma once
#include "AStarNode.h"

class AStarNode;
class Heap;

class AStar : public IObject
{
private:
	vector<AStarNode*>	m_vecNode;
	Heap*				m_pOpenNodeHeap;
	list<AStarNode*>	m_pOpenNodeList;
	LPD3DXMESH			m_pSphere;
	vector<D3DXVECTOR3> m_vecObstacle;
public:
	AStar();
	~AStar();

	void	Init();
	void	Render();
	void	InitNodes(IMap* pMap);
	const vector<AStarNode*>& GetNodes() { return m_vecNode; }
	void	SetObstacle(vector<D3DXVECTOR3> &vecObstacle) { m_vecObstacle = vecObstacle; }

	void	FindPath(D3DXVECTOR3 startPos, D3DXVECTOR3 destPos, OUT vector<int>& vecIndex);
private:
	void	ResetNodes();
	int		FindClosestNode(const D3DXVECTOR3& pos);
	float	GetManhattanDistance(int from, int to);
	int		GetMinimumFNode();
	void	Extend(int targetIdx, int destIdx);

public:
	void	MakeDirectPath(const D3DXVECTOR3 &startPos, const D3DXVECTOR3 &destPos, OUT vector<int>& vecIndex);
private:
	void	CalcEraseCount(const D3DXVECTOR3 &pos, const vector<int>& vecIndex, bool bByStartPos, OUT int &numNodeForErase);
	
};

