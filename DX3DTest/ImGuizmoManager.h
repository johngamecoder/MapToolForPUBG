#pragma once
#include "ImGuizmo.h"
//#include <fstream>
class IDisplayObject;
class Camera;
class BoxCollider;

enum ObjList {
    BANDAGE,
    CHURCH,
    //TREE,
    //ROCK,
    //WAREHOUSE,
    COUNT
};
///여기에 add 할때, 아래에 ***count 변수 생성도 해주고 init에서 0으로 최기화
//ComboObjectList[] 너어주기 
//ContainObject 추가해 주기
///ObjectLoader(int index) switch 문 추가해주기
///Open Scene 바꾸어 주기 
///+ Init() 초기화 할때에 switch 문에 넣어주기 (이부분은 숫자 맞추게 하기 위해서 넣은 코드)
struct ObjInfo 
{
    int                 ID;
    ObjList             list;
    string              m_ObjName;
    D3DXVECTOR3         m_Position;
    D3DXVECTOR3         m_Rotation;
    D3DXVECTOR3         m_Scale;
    D3DXMATRIXA16       m_matTransform;
    IDisplayObject*     objPtr;
    vector<BoxCollider*> m_vecBoxCollider;
    
    ObjInfo()
        :  m_Position(0.0f, 0.0f, 0.0f), 
           m_Rotation(0.0f, 0.0f, 0.0f), 
           m_Scale(1.0f, 1.0f, 1.0f)
    {
        D3DXMatrixIdentity(&m_matTransform);
    }
    ObjInfo(int id, ObjList _list)
        : ObjInfo()
    {
        ID = id;
        list = _list;
    }

};

class ImGuizmoManager 
{
public:
    string m_currentSceneName;
    Camera* m_pCamera;

    //IDisplayObject* m_pBoxCollider;

    vector<IDisplayObject*> m_vecObjectContainer;   //contains Displayable Objects
    map<ObjList, int> m_mapCount;                   //contains number of Objects
    map<string, ObjInfo*> m_mapObject;
    //vector<> m_vecSavedScene;
    ObjInfo* m_pCurrentObject;
    
    LPDIRECT3DTEXTURE9 m_pButtonTexture_Handle;
    LPDIRECT3DTEXTURE9 m_pButtonTexture_Translation;
    LPDIRECT3DTEXTURE9 m_pButtonTexture_Rotation;
    LPDIRECT3DTEXTURE9 m_pButtonTexture_Scale;

    ImGuizmo::OPERATION mCurrentGizmoOperation;
    ImGuizmo::MODE mCurrentGizmoMode;

    int hierarchySelectedObjIndex;  //Hierarchy obj tab
    int hierarchySelectedColliderIndex;  //Hierarchy obj tab
    int comboSelect;
    //int bandageCount;
    //int churchCount;
    //int treeCount;
    //int rockCount;
    //int wareHouseCount;

    bool isRenderCollider;

    // Camera view & projection
    bool isPerspective;
    float viewWidth;
    float cameraProjection[16];
    float cameraView[16];
    float objectMatrix[16];

public:
    ImGuizmoManager();
    ~ImGuizmoManager();

    void Init();
    void Update();
    void Render();

    void SetCurrentObject(ObjInfo* obj)
    {
        m_pCurrentObject = obj;
        //resetting the position to origin
        MatChangeDX2Float(objectMatrix, &m_pCurrentObject->m_matTransform);
    }

    void MenuBarImGui();
    void HierarchyImGui();
    void LoadObjectImGui();
    void InspectorImGui();

    void NewScene();
    void OpenScene(string& fileName );
    void SaveScene(string& fileName );
    void ContainObject();
    void EditTransform(const float *cameraView, float *cameraProjection, float* matrix);
    void ObjectLoader(int index);
    void AddBoxCollider();
    void DeleteObject();

    void MouseHandleMove();
    void MatChangeDX2Float(OUT float * m16, IN D3DXMATRIXA16 * mat);
    void MatChangeFloat2DX(OUT D3DXMATRIXA16 * mat, IN float * m16);
};

