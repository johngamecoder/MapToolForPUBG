#pragma once


class Bandage : public IDisplayObject
{
private:
    EffectMesh * m_pEffectMesh;

public:
    Bandage();
    ~Bandage();
    // Inherited via IDisplayObject
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render() override;
};