#pragma once
//#include "c:\Users\ÇÑÂùÀÀ\source\repos\MapToolForPUBG\DX3DTest\object\interface\IDisplayObject.h"


class Church :
    public IDisplayObject
{
private:
    EffectMesh* m_pEffectMesh;
    //BoxCollider* pBoxCollider;
    //BoxColliderRenderer* pBoxColliderRenderer;

public:
    Church();
    ~Church();

    // Inherited via IDisplayObject
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render() override;
};

