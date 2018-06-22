#pragma once

struct EDGE_INFO
{
	int		index;
	float	edgeCost;
};

enum NODE_STATE
{
	STATE_NONE,
	STATE_OPEN,
	STATE_CLOSE,
	STATE_PATH,
	STATE_WALL
};

class AStarNode :	public IObject
{
public:
	D3DXVECTOR3	m_location;
	int			m_index;
	NODE_STATE	m_nodeState;
	float		m_g;
	float		m_h;
	float		m_f;
	int			m_via;
	
	vector<EDGE_INFO*>	m_vecEdgeInfo;
	vector<D3DXVECTOR3>	m_vecLine;

	AStarNode(D3DXVECTOR3 vLoc, int nIndex);
	~AStarNode();

	void SetValues(NODE_STATE _nodeState, float _g, float _h, float _f, int _via);
	void AddEdge(AStarNode* pNode);
	const D3DXVECTOR3& GetLocation() { return m_location; }
	const vector<D3DXVECTOR3>& GetVecLines() { return m_vecLine; }
	const vector<EDGE_INFO*>& GetEdgeInfos() { return m_vecEdgeInfo; }

};

