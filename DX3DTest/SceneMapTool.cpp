#include "stdafx.h"
#include "SceneMapTool.h"
#include "./sample/grid/Grid.h"
#include "ImGuizmoManager.h"

SceneMapTool::SceneMapTool()
{
    m_pImGuizmoManager = NULL;
}


SceneMapTool::~SceneMapTool()
{
    OnDestructIScene();
    SAFE_DELETE(m_pImGuizmoManager);
}

void SceneMapTool::Init()
{
    m_pImGuizmoManager = new ImGuizmoManager(); m_pImGuizmoManager->Init();

    IDisplayObject* pObj = NULL;
    pObj = new Grid(); pObj->Init(); AddSimpleObj(pObj);

}

void SceneMapTool::Update()
{
    OnUpdateIScene();
    SAFE_UPDATE(m_pImGuizmoManager);
}

void SceneMapTool::Render()
{
    OnRenderIScene();
    SAFE_RENDER(m_pImGuizmoManager);
}

