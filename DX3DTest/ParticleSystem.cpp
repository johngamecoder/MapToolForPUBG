#include "stdafx.h"
#include "ParticleSystem.h"
#include "./manager/TextureManager.h"

//const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

ParticleSystem::ParticleSystem(LPCTSTR _texName)
{
	_pTex = TextureManager::Get()->GetTexture(_texName);
}

ParticleSystem::~ParticleSystem()
{
	SAFE_RELEASE(_pVB);
}


void ParticleSystem::Render()
{
	PreRender();

	DX::GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);
	DX::GetDevice()->SetTexture(0, _pTex);
	DX::GetDevice()->SetFVF(Particle::FVF);
	DX::GetDevice()->SetStreamSource(0, _pVB, 0, sizeof(Particle));
	DX::GetDevice()->DrawPrimitive(D3DPT_POINTLIST, 0, _particles.size());

	PostRender();
}

void ParticleSystem::PreRender()
{
	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DX::GetDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_POINTSIZE, FtoDw(0.4f));
	DX::GetDevice()->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	DX::GetDevice()->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	DX::GetDevice()->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	DX::GetDevice()->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	//DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DX::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DX::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void ParticleSystem::PostRender()
{
	DX::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DX::GetDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	DX::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}


Snow::Snow(LPCTSTR _texName, BoundingBox * boundingBox, int numParticles)
	:ParticleSystem(_texName)
{
	_boundingBox = *boundingBox;
	_numParticle = numParticles;
}

void Snow::Init()
{
	_numParticle = 5000;
	_particles.resize(_numParticle);
	for (int i = 0; i < _numParticle; i++)
	{
		Attribute* att = new Attribute();
		att->_position = D3DXVECTOR3(
			GetRandomFloat(_boundingBox.minPos.x, _boundingBox.maxPos.x),
			GetRandomFloat(_boundingBox.maxPos.y - 15, _boundingBox.maxPos.y + 5),
			GetRandomFloat(_boundingBox.minPos.z, _boundingBox.maxPos.z));
		att->_velocity.x = GetRandomFloat(-0.001f, 0.001f);
		att->_velocity.z = GetRandomFloat(-0.001f, 0.001f);
		att->_velocity.y = -GetRandomFloat(0.01f, 0.02f);
		att->_color = 0xffffffff;
		att->_delay = 0;
		_particles[i] = att;
	}

	DX::GetDevice()->CreateVertexBuffer(_numParticle * sizeof(Particle),
		D3DUSAGE_POINTS | D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		Particle::FVF, D3DPOOL_DEFAULT, &_pVB, 0);
}

void Snow::ResetParticle(Attribute * attribute)
{
}

void Snow::Update()
{
	for (size_t i = 0; i < _particles.size(); i++)
	{
		Attribute* att = _particles[i];
		att->_delay += 1;

		if (att->_delay < 0) continue;

		att->_position += att->_velocity;
		if (att->_position.y < _boundingBox.minPos.y)
			att->_position.y = _boundingBox.maxPos.y + 0.5f;
	}


	Particle* v;
	_pVB->Lock(0, 0, (LPVOID*)&v, D3DLOCK_DISCARD);
	for (auto p : _particles)
	{
		v->position = p->_position;
		v->color = p->_color;
		v++;
	}
}


Aura::Aura(LPCTSTR _texName, D3DXVECTOR3* _pTarget, float _scale)
	:ParticleSystem(_texName)
{
	pTarget = _pTarget;
	scale = _scale;
}

void Aura::Init()
{
	_numParticle = 5000;
	_particles.resize(_numParticle);
	for (int i = 0; i < _numParticle; i++)
	{
		Attribute* att = new Attribute();
		att->_position = D3DXVECTOR3(GetRandomFloat(-0.1f, 0.1f), GetRandomFloat(-0.01f, 0.01f),
			GetRandomFloat(-0.1f, 0.1f));
		att->_color = 0xffff3300 * GetRandomFloat(0, 1);
		att->_delay = -i * 2;
		_particles[i] = att;
	}

	DX::GetDevice()->CreateVertexBuffer(_numParticle * sizeof(Particle),
		D3DUSAGE_POINTS | D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		Particle::FVF, D3DPOOL_DEFAULT, &_pVB, 0);
}

void Aura::ResetParticle(Attribute * attribute)
{
}

void Aura::Update()
{
	for (size_t i = 0; i < _particles.size(); i++)
	{
		Attribute* att = _particles[i];
		att->_delay += 1;

		if (att->_delay < 0) continue;

		att->_age += Time::Get()->GetDeltaTime();
		att->_velocity.x = cosf(att->_age * 4.0f) * 0.2f * scale;
		att->_velocity.z = sinf(att->_age * 4.0f) * 0.2f * scale;
		att->_velocity.y = att->_age * 0.15f * scale;

		att->_position = *pTarget + att->_velocity;
		att->_color.a -= 0.002f;
	}
	

	Particle* v;
	_pVB->Lock(0, 0, (LPVOID*)&v, D3DLOCK_DISCARD);
	for (auto p : _particles)
	{
		v->position = p->_position;
		v->color = p->_color;
		v++;
	}
}

