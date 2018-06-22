#pragma once

class IObject
{
private:
	int m_nRefCount;

public:
	IObject();
	virtual ~IObject();

	virtual void AddRef();
	virtual void Release();
};

