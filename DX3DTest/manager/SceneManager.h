#pragma once

class SceneManager
{
public:
    static SceneManager*	Get();
    static void				Delete();
private:
    static SceneManager*	instance;

    vector<IScene*> m_vecScene;
    IScene *	m_pCurrentScene;

    SceneManager();
    ~SceneManager();

public:
    void Init();
    void Update();
    void Render();

    void SetCurrentScene(size_t index);
};

