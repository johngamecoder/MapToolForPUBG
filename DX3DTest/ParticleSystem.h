#pragma once

struct Particle
{
	D3DXVECTOR3 position;
	D3DCOLOR    color;

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};

struct Attribute
{
	Attribute()
	{
		_lifeTime = 0.0f;
		_age = 0.0f;
		_delay = 0.0f;
		_isAlive = true;
	}

	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;
	D3DXVECTOR3 _acceleration;
	float       _lifeTime;     // how long the particle lives for before dying  
	float       _age;          // current age of the particle  
	D3DXCOLOR   _color;        // current color of the particle   
	D3DXCOLOR   _colorFade;    // how the color fades with respect to time
	bool        _isAlive;
	float		_delay;
};

class ParticleSystem : public  IDisplayObject
{
protected:
	D3DXVECTOR3             _origin;
	BoundingBox				_boundingBox;
	float                   _emitRate;   // rate new particles are added to system
	float                   _size;       // size of particles
	LPDIRECT3DTEXTURE9      _pTex;
	LPDIRECT3DVERTEXBUFFER9 _pVB;
	vector<Attribute*>			_particles;
	int                     _maxParticles; // max allowed particles system can have
	size_t  _numParticle;
	//
	// Following three data elements used for rendering the p-system efficiently
	//

	DWORD _vbSize;      // size of vb
	DWORD _vbOffset;    // offset in vb to lock   
	DWORD _vbBatchSize; // number of vertices to lock starting at _vbOffset

	ParticleSystem(LPCTSTR _texName);

public:
	virtual ~ParticleSystem();
	virtual void Init() = 0;
	virtual void ResetParticle(Attribute* attribute) = 0;
	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();
	
	/*
	virtual void addParticle();
	bool isEmpty();
	bool isDead();
	virtual void reset();
	*/
};

class Snow : public ParticleSystem
{
public:
	Snow(LPCTSTR _texName, BoundingBox* boundingBox, int numParticles);
	void Init() override;
	void ResetParticle(Attribute* attribute);
	void Update();
};

class Aura : public ParticleSystem
{
private:
	D3DXVECTOR3 * pTarget;
	float scale;

public:
	Aura(LPCTSTR _texName, D3DXVECTOR3* _pTarget, float _scale);
	void Init() override;
	void ResetParticle(Attribute* attribute);
	void Update();
};