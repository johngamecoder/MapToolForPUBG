#include "stdafx.h"
#include "IDisplayObject.h"


IDisplayObject::IDisplayObject()
{
	m_pParent = NULL;
	D3DXMatrixIdentity(&m_matWorld);
}


IDisplayObject::~IDisplayObject()
{
}

void IDisplayObject::AddChild(IDisplayObject * pChild)
{
	if (pChild == NULL) return;

	m_vecChild.push_back(pChild);
	pChild->m_pParent = this;
}

void IDisplayObject::Release()
{
	for (auto child : m_vecChild)
		if ( child ) child->Release();

	IObject::Release();
}
