#include "MojingBaker.h"
#include <GLES3/gl3.h>

MojingBaker::MojingBaker()
{
	m_hHandle = 0;
}

MojingBaker::~MojingBaker()
{
	
}

MojingBaker* MojingBaker::_instance = NULL;

MojingBaker* MojingBaker::GetInstance()
{
	if (NULL == _instance)
	{
		_instance = new MojingBaker();
	}
	return _instance;
}

int MojingBaker::BeginBaker(int iMaxSize)
{
	if (m_hHandle >= HANDLE_MAX - 1)
		m_hHandle = 0;
	MojingTextureBaker* baker = new MojingTextureBaker();
	int size = TEXTURE_RESULE_NODE_SIZE * baker->GetNodeSize() + TEXTURE_RESULT_HEAD_SIZE;
	m_lpTextureBakerResult = new char[size*10];
	memset(m_lpTextureBakerResult, 0, size*10);
	baker->SetMaxSize(iMaxSize);
	m_mTextureBakerMap.insert(map<int, MojingTextureBaker*>::value_type(++m_hHandle, baker));
	return m_hHandle;
}

int MojingBaker::AddTexture(int hHandle, int iTextureID, int iX, int iY, int iWidth, int iHeight)
{
	MOJING_TEXTURE_BAKER_MAP_TYPE::iterator iter = m_mTextureBakerMap.find(hHandle);

	if (iter != m_mTextureBakerMap.end())
	{
		MojingTextureBaker* baker = iter->second;
		if (baker)
		{
			//LOGI("MojingBaker:AddTexture---TextureID Add Success");
			baker->AddNode(iTextureID, iX, iY, iWidth, iHeight);
			return 0;
		}
		else
		{
			return -1;
			//LOGI("MojingBaker:AddTexture---TextureID Add Failed");
		}
	}
	return -1;
}

int MojingBaker::RemoveTexture(int hHandle, int iTextureID, int iX, int iY, int iWidth, int iHeight)
{
	MOJING_TEXTURE_BAKER_MAP_TYPE::iterator iter = m_mTextureBakerMap.find(hHandle);

	if (iter != m_mTextureBakerMap.end())
	{
		MojingTextureBaker* baker = iter->second;
		if (baker)
		{
			if (baker->RemoveNode(iTextureID))
				return 1;
		}
	}
	return 0;
}

int MojingBaker::GetBakerTextureSize(int hHandle)
{
	MOJING_TEXTURE_BAKER_MAP_TYPE::iterator iter = m_mTextureBakerMap.find(hHandle);

	if (iter != m_mTextureBakerMap.end())
	{
		MojingTextureBaker* baker = iter->second;
		if (baker)
		{
			return baker->GetBakerResultSize();
		}
	}
	return 0;
}

char* MojingBaker::BakerTexture(int hHandle, int iTextureID)
{
	MOJING_TEXTURE_BAKER_MAP_TYPE::iterator iter = m_mTextureBakerMap.find(hHandle);
	//char tempSize1[] = "{SubTextureCount:N,\n\rBakedTextureSize:XXXX,\n\rBakedTextureID:XXXX,\n\rBakedTexture:[]\n\r}";
	//char tempSize[] = "{\n\rSubTextureID:XXXX,\n\rPosition:[XXXX,XXXX],\n\rSize:[XXXX,XXXX]\n\r}";
	//int node_size = sizeof(tempSize);
	//LOGI("MojingBaker:BakerTexture Start");
	if (iter != m_mTextureBakerMap.end())
	{
		MojingTextureBaker* baker = iter->second;
		if (baker)
		{
			baker->SetBakerResultTextureId(iTextureID);
			if (baker->StartRunTextureBaker())
			{
				int count = sprintf(m_lpTextureBakerResult,
					"{\n\rSubTextureCount:%d,\n\rBakedTextureSize:%d,\n\rBakedTextureID:%d,\n\rBakedTexture:[\n\r",
					baker->GetNodeSize(),
					baker->GetBakerResultSize(),
					baker->GetBakerResultTextureId());
				baker->GetBakerResult(&m_lpTextureBakerResult, count);
				sprintf(m_lpTextureBakerResult + count, "]\n\r}");
				return m_lpTextureBakerResult;
			}
		}
	}
	return NULL;
}

void MojingBaker::EndBaker(int hHandle)
{
	if (hHandle)
	{
		MOJING_TEXTURE_BAKER_MAP_TYPE::iterator iter = m_mTextureBakerMap.find(hHandle);
		if (iter != m_mTextureBakerMap.end())
		{
			MojingTextureBaker* baker = iter->second;
			if (baker)
			{
				//LOGI("MojingBaker:EndBaker---HandleID:%d", hHandle);
				delete baker;
				baker = NULL;
			}
			m_mTextureBakerMap.erase(iter);
			delete[] m_lpTextureBakerResult;
		}
	}
	else
	{
		for (MOJING_TEXTURE_BAKER_MAP_TYPE::iterator iter = m_mTextureBakerMap.begin(); iter != m_mTextureBakerMap.end(); iter++)
		{
			MojingTextureBaker* baker = iter->second;
			if (baker)
			{
				//LOGI("MojingBaker:EndBaker---HandleID:%d", hHandle);
				delete baker;
				baker = NULL;
			}
			m_mTextureBakerMap.clear();
			delete[] m_lpTextureBakerResult;
		}
	}
	
}