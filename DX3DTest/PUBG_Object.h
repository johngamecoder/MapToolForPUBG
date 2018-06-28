#pragma once
//#include "c:\Users\ÇÑÂùÀÀ\source\repos\MapToolForPUBG\DX3DTest\object\interface\IDisplayObject.h"
class PUBG_Object : public IDisplayObject
{
private:
    EffectMesh *    m_pEffectMesh;

    string          m_Path;
    string          m_FileName;

public:
    PUBG_Object(string& path, string& fileName);
    ~PUBG_Object();

    // Inherited via IDisplayObject
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render() override;
};

