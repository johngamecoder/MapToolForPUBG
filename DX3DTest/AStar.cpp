#include "stdafx.h"
#include "AStar.h"
#include "AStarNode.h"
#include "Heap.h"
#include "Ray.h"


AStar::AStar()
{
}


AStar::~AStar()
{
	SAFE_RELEASE(m_pSphere);
	SAFE_DELETE(m_pOpenNodeHeap);
	for (auto p : m_vecNode)
		SAFE_RELEASE(p);
}

void AStar::Init()
{
	D3DXCreateSphere(DX::GetDevice(), 0.2f, 10, 10, &m_pSphere, NULL);
	m_pOpenNodeHeap = new Heap;
}

void AStar::Render()
{
	D3DXMATRIXA16 mat;

	for (auto p : m_vecNode)
	{
		switch (p->m_nodeState)
		{
		case STATE_NONE:
			DX::GetDevice()->SetMaterial(&DXUtil::WHITE_MTRL);
			break;
		case STATE_OPEN:
			DX::GetDevice()->SetMaterial(&DXUtil::GREEN_MTRL);
			break;
		case STATE_CLOSE:
			DX::GetDevice()->SetMaterial(&DXUtil::RED_MTRL);
			break;
		case STATE_PATH:
			DX::GetDevice()->SetMaterial(&DXUtil::YELLOW_MTRL);
			break;
		case STATE_WALL:
			DX::GetDevice()->SetMaterial(&DXUtil::BLUE_MTRL);
			break;
		}
		
		D3DXMatrixTranslation(&mat, p->GetLocation().x, p->GetLocation().y, p->GetLocation().z);
		DX::GetDevice()->SetTransform(D3DTS_WORLD, &mat);
		DX::GetDevice()->SetTexture(0, NULL);
		m_pSphere->DrawSubset(0);

		D3DXMatrixIdentity(&mat);
		DX::GetDevice()->SetTransform(D3DTS_WORLD, &mat);
		DX::GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, p->GetVecLines().size() / 2,
			p->GetVecLines()[0], sizeof(D3DXVECTOR3));
	}
}

void AStar::InitNodes(IMap* pMap)
{
	int nodeDim = 30;
	float interval = pMap->GetSize().x / (float)(nodeDim - 0.99);

	for (int posZ = 0; posZ < nodeDim; ++posZ)
	{
		for (int posX = 0; posX < nodeDim; ++posX)
		{
			D3DXVECTOR3 location(posX * interval, 0, posZ * interval);
			pMap->GetHeight(location.y, location);

			AStarNode* pNode = new AStarNode(location, m_vecNode.size());
			m_vecNode.push_back(pNode);
			if (posX == 2  && posZ < 5)
				pNode->m_nodeState = STATE_WALL;
		}
	}

	for (size_t i = 0; i < m_vecNode.size(); ++i)
	{
		//제일 우측 열을 제외하고 자신의 우측 노드와 서로 Edge 등록
		if (i % nodeDim != nodeDim - 1)
		{
			m_vecNode[i]->AddEdge(m_vecNode[i + 1]);
			m_vecNode[i + 1]->AddEdge(m_vecNode[i]);
		}
		//제일 윗 횡을 제외하고 자신의 윗 노드와 서로 Edge 등록
		if (i < m_vecNode.size() - nodeDim)
		{
			m_vecNode[i]->AddEdge(m_vecNode[i + nodeDim]);
			m_vecNode[i + nodeDim]->AddEdge(m_vecNode[i]);
		}
	}
}


void AStar::FindPath(D3DXVECTOR3 startPos, D3DXVECTOR3 destPos, OUT vector<int>& vecIndex)
{
	ResetNodes();

	int startNodeIndex = FindClosestNode(startPos);
	int destNodeIndex = FindClosestNode(destPos);
	if (startNodeIndex == -1 || destNodeIndex == -1)
	{
		return;
	}

	//출발지 노드를 초기화 해주고 열린 상태로 만든다
	float G = 0.0f;
	float H = GetManhattanDistance(startNodeIndex, destNodeIndex);
	float F = G + H;

	m_vecNode[startNodeIndex]->SetValues(STATE_OPEN, G, H, F, startNodeIndex);
	m_pOpenNodeHeap->Insert(m_vecNode[startNodeIndex]);
	//m_pOpenNodeList.push_back(m_vecNode[startNodeIndex]);

	//목적지 노드가 닫힌 상태가 될 때까지 출발지 노드부터 확장해 나간다
	//확장을 완료한 노드는 닫힌 상태로 만든다
	while (m_vecNode[destNodeIndex]->m_nodeState != STATE_CLOSE)
	{
		int nCurrIndex = GetMinimumFNode();
		if (nCurrIndex == -1) return;
		Extend(nCurrIndex, destNodeIndex);
		m_vecNode[nCurrIndex]->m_nodeState = (STATE_CLOSE);
	}

	//목적지 노드부터 출발지 노드 이전 까지 경유 노드들의 인덱스를 Path 목록에 추가한다
	//목록에 추가한 노드들은 사용 상태로 만든다
	int nCurrIndex = destNodeIndex;
	while (nCurrIndex != startNodeIndex)
	{
		m_vecNode[nCurrIndex]->m_nodeState = (STATE_PATH);
		vecIndex.push_back(nCurrIndex);
		nCurrIndex = m_vecNode[nCurrIndex]->m_via;
	}
	//출발지 노드의 인덱스도 Path 목록에 추가하고 사용 상태로 만든다
	m_vecNode[nCurrIndex]->m_nodeState = (STATE_PATH);
	vecIndex.push_back(nCurrIndex);
	
	//목적지 -> 경유지... -> 출발지 인덱스 목록 완성됨
	
	m_pOpenNodeHeap->Clear();
	//m_pOpenNodeList.clear();
}


void AStar::ResetNodes()
{
	for (int i = 0; i < m_vecNode.size(); ++i)
	{
		if (m_vecNode[i]->m_nodeState != STATE_WALL)
			m_vecNode[i]->m_nodeState = (STATE_NONE);
	}
}

int AStar::FindClosestNode(const D3DXVECTOR3 & pos)
{
	float minDist = FLT_MAX;
	int closestNodeIndex = -1;
	
	for (int i = 0; i < m_vecNode.size(); ++i)
	{
		if (m_vecNode[i]->m_nodeState == STATE_WALL)
			continue;

		D3DXVECTOR3 subtract = pos - m_vecNode[i]->GetLocation();
		float dist = D3DXVec3Length(&subtract);
		if (dist < minDist)
		{
			minDist = dist;
			closestNodeIndex = i;
		}
	}
	
	return closestNodeIndex;
}


float AStar::GetManhattanDistance(int from, int to)
{
	float offsetX = fabs(m_vecNode[from]->GetLocation().x - m_vecNode[to]->GetLocation().x);
	float offsetZ = fabs(m_vecNode[from]->GetLocation().z - m_vecNode[to]->GetLocation().z);
	return offsetX + offsetZ;
}


int AStar::GetMinimumFNode()
{
	/*AStarNode* node = NULL;
	float minValue = FLT_MAX;
	for (auto p : m_pOpenNodeList)
	{
		if (p->m_f < minValue)
		{
			minValue = p->m_f;
			node = p;
		}
	}
	if (node == NULL) return -1;
	m_pOpenNodeList.remove(node);
	*/

	AStarNode* node = m_pOpenNodeHeap->ExtractMin();
	if (node == NULL) return -1;
	return node->m_index;
}


void AStar::Extend(int targetIdx, int destIdx)
{
	vector<EDGE_INFO*> vecEdge = m_vecNode[targetIdx]->GetEdgeInfos();

	for (int i = 0; i < vecEdge.size(); ++i)
	{
		int index = vecEdge[i]->index;
		AStarNode* currNode = m_vecNode[index];

		if (currNode->m_nodeState == STATE_CLOSE) continue;
		if (currNode->m_nodeState == STATE_WALL) continue;
		
		float G = m_vecNode[targetIdx]->m_g + vecEdge[i]->edgeCost;
		float H = GetManhattanDistance(index, destIdx);
		float F = G + H;
		if (currNode->m_nodeState == STATE_OPEN && G >= currNode->m_g) continue;
		
		currNode->SetValues(STATE_OPEN, G, H, F, targetIdx);
		m_pOpenNodeHeap->Insert(currNode);
		//m_pOpenNodeList.push_back(currNode);
	}
}


void AStar::MakeDirectPath(const D3DXVECTOR3& startPos, const D3DXVECTOR3& destPos, OUT vector<int>& vecIndex)
{
	if (m_vecObstacle.empty())
	{
		//장애물이 없으면 destPos 까지 직선으로 이동
		vecIndex.clear();
	}
	else
	{
		Debug::Get()->InitRetainStr();
		Debug::Get()->UsingRetainStr();
		for (auto p : vecIndex)
		{
			Debug::Get()->AddText(p); Debug::Get()->AddText(", ");
		}
		Debug::Get()->EndLine();
		int numNodeToErase = 0;
		//시작 지점부터 장애물 교차 지점까지 노드 삭제
		CalcEraseCount(startPos, vecIndex, true, numNodeToErase);
		if (numNodeToErase > 0) vecIndex.erase(vecIndex.end() - numNodeToErase, vecIndex.end());
		Debug::Get()->AddText("EraseCount = ");
		Debug::Get()->AddText(numNodeToErase);
		Debug::Get()->EndLine();
		for (auto p : vecIndex)
		{
			Debug::Get()->AddText(p); Debug::Get()->AddText(", ");
		}
		Debug::Get()->EndLine();
		//마지막 지점부터 장애물 교차 지점까지 노드 삭제
		CalcEraseCount(destPos, vecIndex, false, numNodeToErase);
		if (numNodeToErase == vecIndex.size()) numNodeToErase--;
		if (numNodeToErase > 0) vecIndex.erase(vecIndex.begin(), vecIndex.begin() + numNodeToErase);
		Debug::Get()->AddText("EraseCount = ");
		Debug::Get()->AddText(numNodeToErase);
		Debug::Get()->EndLine();
		for (auto p : vecIndex)
		{
			Debug::Get()->AddText(p); Debug::Get()->AddText(", ");
		}
		Debug::Get()->EndLine();
		Debug::Get()->UsingRetainStr(false);
	}
}


void AStar::CalcEraseCount(const D3DXVECTOR3 &pos, const vector<int>& vecIndex, bool bByStartPos, OUT int & numNodeForErase)
{
	numNodeForErase = 0;
	Ray ray;
	ray.m_pos = pos;
	ray.m_pos.y += 0.3f;

	for (size_t indexCnt = 0; indexCnt < vecIndex.size(); indexCnt++)
	{
		if (bByStartPos == true)
		{
			ray.m_dir = m_vecNode[vecIndex[vecIndex.size() - 1 - indexCnt]]->GetLocation() - pos;
		}
		else
		{
			ray.m_dir = m_vecNode[vecIndex[indexCnt]]->GetLocation() - pos;
		}
		float nodeDist = D3DXVec3Length(&ray.m_dir);
		D3DXVec3Normalize(&ray.m_dir, &ray.m_dir);
		float intersectionDist;
		bool isIntersected = false;
		for (size_t i = 0; i < m_vecObstacle.size(); i += 3)
		{
			if (ray.CalcIntersectTri(&m_vecObstacle[i], &intersectionDist))
			{
				if (intersectionDist < nodeDist)
				{
					isIntersected = true;
					break;
				}
			}
		}
		if (isIntersected == true)
		{
			numNodeForErase--;
			break;
		}
		else
		{
			numNodeForErase++;
		}
	}
}
