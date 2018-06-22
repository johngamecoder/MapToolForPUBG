#pragma once
//#include "c:\Users\ÇÑÂùÀÀ\source\repos\MapToolForPUBG\DX3DTest\object\interface\IScene.h"

class ImGuizmoManager;
class SceneMapTool :public IScene
{
    ImGuizmoManager*    m_pImGuizmoManager;
    D3DXVECTOR3         m_vTranslation;
public:
    SceneMapTool();
    ~SceneMapTool();

    // Inherited via IScene
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render() override;
};

