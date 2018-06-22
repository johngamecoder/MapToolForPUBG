#pragma once

class AStarNode;

class Heap
{
public:
	vector<AStarNode*> m_vecHeap;

	Heap();
	~Heap();

	void		Insert(AStarNode* pNode);
	AStarNode*	ExtractMin();
	void		Clear();
private:
	void		Upheap(int targetIdx);
	void		Downheap(int targetIdx);
	void		Swap(int idx1, int idx2);
};

