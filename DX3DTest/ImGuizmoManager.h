#pragma once
#include "ImGuizmo.h"
#include "TagResource.h"
#include "BoxCollider.h"

class IDisplayObject;
class Camera;
class BoxCollider;
extern char* ComboObjectList[static_cast<int>(TAG_RES_STATIC::COUNT)];
extern char* ComboTerrainFeaturesList[63];
extern char* ComboItemsList[13];

struct BoxColliderInFile
{
    D3DXMATRIXA16  m_transform;

    BoxColliderInFile();
};

struct ObjectInFile
{
    TAG_RES_STATIC m_tagResStatic;
    std::string    m_name;
    D3DXVECTOR3    m_position;
    D3DXVECTOR3    m_rotation;
    D3DXVECTOR3    m_scale;

    std::vector<BoxColliderInFile> m_boxColliders;

    ObjectInFile();
};


struct ObjInfo 
{
    //int                 ID;
    TAG_RES_STATIC      list;
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
    ObjInfo(const TAG_RES_STATIC _list, const string& userInputName)
        : ObjInfo()
    {
        list = _list;
        m_ObjName = userInputName;
    }

};

class ImGuizmoManager 
{
public:

    string m_currentSceneName;
    Camera* m_pCamera;
    D3DXVECTOR3 m_currentObjPos;

    IDisplayObject* m_vecObjectContainer[static_cast<int>(TAG_RES_STATIC::COUNT)];
    map<TAG_RES_STATIC, int> m_mapObjCount;                   //contains number of Objects
    map<string, ObjInfo*> m_mapObject;
    //deque<pair<string, ObjInfo*>> m_mapObject;
    //vector<> m_vecSavedScene;
    ObjInfo* m_pCurrentObject;

    LPDIRECT3DTEXTURE9 m_pButtonTexture[5];
    
    ImGuizmo::OPERATION mCurrentGizmoOperation;
    ImGuizmo::MODE mCurrentGizmoMode;

    int hierarchySelectedObjIndex;  //Hierarchy obj tab
    int hierarchySelectedColliderIndex;  //Hierarchy obj tab
    int comboTerrainFeatureSelect;    //selected number of object on hierarchy
    int comboItemSelect;
    bool boundSizing;   //selecting to use boundsizing or not


    // Camera view & projection & Guizmo Matrix
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
        m_pCamera->SetCurrentObjectPos(&m_pCurrentObject->m_Position);//this is for camera, when press F, camera focus into the current object;
        //m_currentOBJName = &obj->m_ObjName;
        //resetting the position to origin
        MatChangeDX2Float(objectMatrix, &m_pCurrentObject->m_matTransform);
    }

    void MenuBarImGui();
    void HierarchyImGui();
    void LoadObjectImGui();
    void InspectorImGui();

    void NewScene();
    //void OpenScene(const string& fileName );
    void OpenScene2(const string& fileName );
    void SaveScene(string& fileName );
    void ConstructComboObjectList();
    void ContainObject();
    void EditTransform(const float *cameraView, float *cameraProjection, float* matrix);
    void ObjectLoader(const int index, const string& userInputName);
    void AddBoxCollider();
    void DeleteObject();
    void DeleteBoxCollider(int index);

    void MouseHandleMove();
    void MatChangeDX2Float(OUT float * m16, IN D3DXMATRIXA16 * mat);
    void MatChangeFloat2DX(OUT D3DXMATRIXA16 * mat, IN float * m16);

    void ObjectLoaderButton(char* ComboList[], int selectedCombo);
    void ObjectItemLoaderButton(char* ComboList[], int selectedCombo);

    HRESULT parseObjectInFile(std::ifstream& fin, ObjectInFile* Out);
    HRESULT parseBoxColliderInFile(
        std::ifstream& fin, BoxColliderInFile* Out);
};

