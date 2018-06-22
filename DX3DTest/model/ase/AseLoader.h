#pragma once

class AseObject;

class AseLoader
{
private:
	LPCTSTR					m_filepath;
	ifstream				m_fin;
	char					m_szToken[512];
	vector<D3DMATERIAL9>	m_vecMtl;
	vector<CString>			m_vecTexName;
	map<string, AseObject*>	m_mapAseObject;

public:
	AseLoader();
	~AseLoader();

	AseObject*	Load(LPCTSTR filepath, LPCTSTR filename, OUT ASE_SCENE* pScene);
	char*		GetToken();
	int			GetTokenAsInt();
	float		GetTokenAsFloat();
	bool		CompareStr(char* str1, const char* str2);
	void		SkipBlock();

	void		ProcessSCENE(OUT ASE_SCENE* pScene);
	void		ProcessMATERIAL_LIST();
	void		ProcessMATERIAL(int index);
	void		ProcessMAP_DIFFUSE(int index);
	AseObject*	ProcessGEOMOBJECT(OUT AseObject* &pObj);
	void		ProcessNODE_TM(OUT AseObject* pObj);
	void		ProcessMESH(OUT AseObject* pObj);
	void		ProcessMESH_VERTEX_LIST(OUT std::vector<D3DXVECTOR3>& vecV);
	void		ProcessMESH_FACE_LIST(IN std::vector<D3DXVECTOR3>& vecV, OUT std::vector<VERTEX_PNT>&	vecVertex);
	void		ProcessMESH_TVERTLIST(OUT std::vector<D3DXVECTOR2>& vecVT);
	void		ProcessMESH_TFACELIST(IN std::vector<D3DXVECTOR2>& vecVT, OUT std::vector<VERTEX_PNT>&	vecVertex);
	void		ProcessMESH_NORMALS(OUT std::vector<VERTEX_PNT>&	vecVertex);
	void		ProcessTM_ANIMATION(OUT AseObject* pObj);
	void		ProcessPOS_TRACK(OUT AseObject* pObj);
	void		ProcessROT_TRACK(OUT AseObject* pObj);
};

