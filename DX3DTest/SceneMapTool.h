#pragma once
//#include "c:\Users\������\source\repos\MapToolForPUBG\DX3DTest\object\interface\IScene.h"

class ImGuizmoManager;
class HeightMap;
class SceneMapTool :public IScene
{
    ImGuizmoManager*    m_pImGuizmoManager;
    HeightMap* m_pHeightMap;
    
public:
    SceneMapTool();
    ~SceneMapTool();

    // Inherited via IScene
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render() override;
};

