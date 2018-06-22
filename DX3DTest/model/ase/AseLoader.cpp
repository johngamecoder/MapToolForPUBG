#include "stdafx.h"
#include "AseLoader.h"
#include "AseObject.h"
#include "Asciitok.h"

AseLoader::AseLoader()
{
}


AseLoader::~AseLoader()
{
}

AseObject * AseLoader::Load(LPCTSTR filepath, LPCTSTR filename, OUT ASE_SCENE * pScene)
{
	m_filepath = filepath;

	CString fullpath = filepath;
	fullpath.Append(filename);

	m_fin.open(fullpath);
	
	if (m_fin.is_open() == false)
		assert(false && "File loading is failed");

	AseObject* m_pRootObj = NULL;

	while (!m_fin.eof() )
	{
		char* szToken = GetToken();
		
		if (CompareStr(szToken, ID_SCENE))
		{
			if (pScene) ProcessSCENE(pScene);
		}
		else if (CompareStr(szToken, ID_MATERIAL_LIST))
		{
			ProcessMATERIAL_LIST();
		}
		else if (CompareStr(szToken, ID_GEOMETRY))
		{
			AseObject* pObj = new AseObject();
			ProcessGEOMOBJECT(pObj);
			if (m_pRootObj == NULL)
			{
				pObj->AddRef();
				m_pRootObj = pObj;
			}
			pObj->Release();
		}
	}
  
	m_fin.close();

	return m_pRootObj;
}

char* AseLoader::GetToken()
{
	bool isQuot = false;
	char szToken[512];
	
	do
	{
		m_fin >> m_szToken;
		//" 가 나오면 " 가 다시 나올 때까지 문장을 묶는다
		if (m_szToken[0] == '\"' || m_szToken[strlen(m_szToken) - 1] == '\"')
		{
			if (isQuot == true)
			{
				isQuot = false;
				strncat_s(szToken, " ", 1);
				strncat_s(szToken, m_szToken, strlen(m_szToken));
			}
			else
			{
				if (m_szToken[strlen(m_szToken) - 1] != '\"')
				{
					isQuot = true;
				}
				strcpy_s(szToken, m_szToken);
			}
		}
		else
		{
			if (isQuot == true)
			{
				strncat_s(szToken, " ", 1);
				strncat_s(szToken, m_szToken, strlen(m_szToken));
			}
			else
			{
				strcpy_s(szToken, m_szToken);
			}
		}
	} while (isQuot == true);

	strcpy_s(m_szToken, szToken);
	
	return m_szToken;
}

int AseLoader::GetTokenAsInt()
{
	return atoi(GetToken());
}

float AseLoader::GetTokenAsFloat()
{
	return atof(GetToken());
}

bool AseLoader::CompareStr(char * str1, const char * str2)
{
	return strcmp(str1, str2) == 0;
}

void AseLoader::SkipBlock()
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessSCENE(OUT ASE_SCENE * pScene)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_FIRSTFRAME))
		{
			pScene->numFirstFrame = GetTokenAsInt();
		}
		else if (CompareStr(szToken, ID_LASTFRAME))
		{
			pScene->numLastFrame = GetTokenAsInt();
		}
		else if (CompareStr(szToken, ID_FRAMESPEED))
		{
			pScene->FrameSpeed = GetTokenAsInt();
		}
		else if (CompareStr(szToken, ID_TICKSPERFRAME))
		{
			pScene->TicksPerFrame = GetTokenAsInt();
		}
	} while (nLevel > 0);

}

void AseLoader::ProcessMATERIAL_LIST()
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_MATERIAL_COUNT))
		{
			int size = GetTokenAsInt();

			m_vecMtl.resize(size);
			m_vecTexName.resize(size);
		}
		else if (CompareStr(szToken, ID_MATERIAL))
		{
			ProcessMATERIAL(GetTokenAsInt());
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMATERIAL(int index)
{
	D3DMATERIAL9* mtl = &m_vecMtl[index];
	ZeroMemory(mtl, sizeof(D3DMATERIAL9));

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_AMBIENT))
		{
			mtl->Ambient.r = GetTokenAsFloat();
			mtl->Ambient.g = GetTokenAsFloat();
			mtl->Ambient.b = GetTokenAsFloat();
			mtl->Ambient.a = 1.0f;
		}
		else if (CompareStr(szToken, ID_DIFFUSE))
		{
			mtl->Diffuse.r = GetTokenAsFloat();
			mtl->Diffuse.g = GetTokenAsFloat();
			mtl->Diffuse.b = GetTokenAsFloat();
			mtl->Diffuse.a = 1.0f;
		}
		else if (CompareStr(szToken, ID_SPECULAR))
		{
			mtl->Specular.r = GetTokenAsFloat();
			mtl->Specular.g = GetTokenAsFloat();
			mtl->Specular.b = GetTokenAsFloat();
			mtl->Specular.a = 1.0f;
		}
		else if (CompareStr(szToken, ID_MAP_DIFFUSE))
		{
			ProcessMAP_DIFFUSE(index);
		}
    
	} while (nLevel > 0);
}

void AseLoader::ProcessMAP_DIFFUSE(int index)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_BITMAP))
		{
			char* filename = GetToken();
			
			filename = &filename[1];  //문자열의 "" 제거
			filename[strlen(filename) - 1] = '\0';

			CString fullpath = m_filepath;
			fullpath.Append(CA2W(filename));
			
			m_vecTexName[index] = fullpath;
		}
	} while (nLevel > 0);
}

AseObject * AseLoader::ProcessGEOMOBJECT(OUT AseObject* &pObj)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_NODE_NAME))
		{
			char* nodeName = GetToken();
			m_mapAseObject[nodeName] = pObj;
		}
		else if (CompareStr(szToken, ID_NODE_PARENT))
		{
			char* parentName = GetToken();
			pObj->AddRef();
			m_mapAseObject[parentName]->AddChild(pObj);
		}
		else if (CompareStr(szToken, ID_NODE_TM))
		{
			ProcessNODE_TM(pObj);
		}
		else if (CompareStr(szToken, ID_MESH))
		{
			ProcessMESH(pObj);
		}
		else if (CompareStr(szToken, ID_TM_ANIMATION))
		{
			ProcessTM_ANIMATION(pObj);
		}
		else if (CompareStr(szToken, ID_MATERIAL_REF))
		{
			int index = GetTokenAsInt();

			pObj->m_mtl = m_vecMtl[index];
			pObj->m_texName = m_vecTexName[index];
		}
	} while (nLevel > 0);

	return pObj;
}

void AseLoader::ProcessNODE_TM(OUT AseObject * pObj)
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_TM_ROW0))
		{
			matWorld._11 = GetTokenAsFloat();
			matWorld._13 = GetTokenAsFloat();
			matWorld._12 = GetTokenAsFloat();
			matWorld._14 = 0.0f;
		}
		else if (CompareStr(szToken, ID_TM_ROW1))
		{
			matWorld._31 = GetTokenAsFloat();
			matWorld._33 = GetTokenAsFloat();
			matWorld._32 = GetTokenAsFloat();
			matWorld._34 = 0.0f;
		}
		else if (CompareStr(szToken, ID_TM_ROW2))
		{
			matWorld._21 = GetTokenAsFloat();
			matWorld._23 = GetTokenAsFloat();
			matWorld._22 = GetTokenAsFloat();
			matWorld._24 = 0.0f;
		}
		else if (CompareStr(szToken, ID_TM_ROW3))
		{
			matWorld._41 = GetTokenAsFloat();
			matWorld._43 = GetTokenAsFloat();
			matWorld._42 = GetTokenAsFloat();
			matWorld._44 = 1.0f;
		}
	} while (nLevel > 0);

	pObj->m_matWorldTM = matWorld;
}

void AseLoader::ProcessMESH(OUT AseObject * pObj)
{
	vector<D3DXVECTOR3>	vecP;
	vector<D3DXVECTOR2>	vecT;
	vector<VERTEX_PNT>	vecVertex;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_MESH_NUMVERTEX))
		{
			vecP.resize(GetTokenAsInt());
		}
		else if (CompareStr(szToken, ID_MESH_NUMFACES))
		{
			vecVertex.resize(3 * GetTokenAsInt());
		}
		else if (CompareStr(szToken, ID_MESH_VERTEX_LIST))
		{
			ProcessMESH_VERTEX_LIST(vecP);
		}
		else if (CompareStr(szToken, ID_MESH_FACE_LIST))
		{
			ProcessMESH_FACE_LIST(vecP, vecVertex);
		}
		else if (CompareStr(szToken, ID_MESH_NUMTVERTEX))
		{
			vecT.resize(GetTokenAsInt());
		}
		else if (CompareStr(szToken, ID_MESH_TVERTLIST))
		{
			ProcessMESH_TVERTLIST(vecT);
		}
		else if (CompareStr(szToken, ID_MESH_NUMTVFACES))
		{
			GetToken();
		}
		else if (CompareStr(szToken, ID_MESH_TFACELIST))
		{
			ProcessMESH_TFACELIST(vecT, vecVertex);
		}
		else if (CompareStr(szToken, ID_MESH_NORMALS))
		{
			ProcessMESH_NORMALS(vecVertex);
		}

	} while (nLevel > 0);

	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &pObj->m_matWorldTM);

	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}

	pObj->m_vecVertex = vecVertex;
}

void AseLoader::ProcessMESH_VERTEX_LIST(OUT std::vector<D3DXVECTOR3>& vecV)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_MESH_VERTEX))
		{
			int index = GetTokenAsInt();
			vecV[index].x = GetTokenAsFloat();
			vecV[index].z = GetTokenAsFloat();
			vecV[index].y = GetTokenAsFloat();
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMESH_FACE_LIST(IN std::vector<D3DXVECTOR3>& vecV, OUT std::vector<VERTEX_PNT>& vecVertex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_MESH_FACE))
		{
			int faceIndex = GetTokenAsInt();
			GetToken(); // A:
			int nA = GetTokenAsInt();

			GetToken(); // B:
			int nB = GetTokenAsInt();

			GetToken(); // C:
			int nC = GetTokenAsInt();

			vecVertex[faceIndex * 3 + 0].p = vecV[nA];
			vecVertex[faceIndex * 3 + 1].p = vecV[nC];
			vecVertex[faceIndex * 3 + 2].p = vecV[nB];
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMESH_TVERTLIST(OUT std::vector<D3DXVECTOR2>& vecVT)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_MESH_TVERT))
		{
			int index = GetTokenAsInt();
			vecVT[index].x = GetTokenAsFloat();
			vecVT[index].y = 1.0f - GetTokenAsFloat();
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMESH_TFACELIST(IN std::vector<D3DXVECTOR2>& vecVT, OUT std::vector<VERTEX_PNT>& vecVertex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_MESH_TFACE))
		{
			int faceIndex = GetTokenAsInt();
			int nA = GetTokenAsInt();
			int nB = GetTokenAsInt();
			int nC = GetTokenAsInt();

			vecVertex[faceIndex * 3 + 0].t = vecVT[nA];
			vecVertex[faceIndex * 3 + 1].t = vecVT[nC];
			vecVertex[faceIndex * 3 + 2].t = vecVT[nB];
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessMESH_NORMALS(OUT std::vector<VERTEX_PNT>& vecVertex)
{
	int faceIndex = 0;
	int vertexCount = 0;
	int aVertexIndex[] = { 0, 2, 1 };

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_MESH_FACENORMAL))
		{
			faceIndex = GetTokenAsInt();
			vertexCount = 0;
		}
		else if (CompareStr(szToken, ID_MESH_VERTEXNORMAL))
		{
			GetToken();
			vecVertex[faceIndex * 3 + aVertexIndex[vertexCount]].n.x = GetTokenAsFloat();
			vecVertex[faceIndex * 3 + aVertexIndex[vertexCount]].n.z = GetTokenAsFloat();
			vecVertex[faceIndex * 3 + aVertexIndex[vertexCount]].n.y = GetTokenAsFloat();
			vertexCount++;
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessTM_ANIMATION(OUT AseObject * pObj)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_POS_TRACK))
		{
			ProcessPOS_TRACK(pObj);
		}
		else if (CompareStr(szToken, ID_ROT_TRACK))
		{
			ProcessROT_TRACK(pObj);
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessPOS_TRACK(OUT AseObject * pObj)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_POS_SAMPLE))
		{
			ASE_POS_SAMPLE posSample;
			posSample.tick = GetTokenAsInt();
			posSample.position.x = GetTokenAsFloat();
			posSample.position.z = GetTokenAsFloat();
			posSample.position.y = GetTokenAsFloat();

			pObj->m_vecPosTrack.push_back(posSample);
		}
	} while (nLevel > 0);
}

void AseLoader::ProcessROT_TRACK(OUT AseObject * pObj)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (CompareStr(szToken, "{"))
		{
			++nLevel;
		}
		else if (CompareStr(szToken, "}"))
		{
			--nLevel;
		}
		else if (CompareStr(szToken, ID_ROT_SAMPLE))
		{
			ASE_ROT_SAMPLE rotSample;

			rotSample.tick = GetTokenAsInt();

			rotSample.quaternion.x = GetTokenAsFloat();
			rotSample.quaternion.z = GetTokenAsFloat();
			rotSample.quaternion.y = GetTokenAsFloat();
			rotSample.quaternion.w = GetTokenAsFloat();
					  
			rotSample.quaternion.x *= sinf(rotSample.quaternion.w / 2.0f);
			rotSample.quaternion.y *= sinf(rotSample.quaternion.w / 2.0f);
			rotSample.quaternion.z *= sinf(rotSample.quaternion.w / 2.0f);
			rotSample.quaternion.w = cosf(rotSample.quaternion.w / 2.0f);

			if (!pObj->m_vecRotTrack.empty())
			{
				rotSample.quaternion = pObj->m_vecRotTrack.back().quaternion * rotSample.quaternion;
			}

			pObj->m_vecRotTrack.push_back(rotSample);
		}
	} while (nLevel > 0);
}
