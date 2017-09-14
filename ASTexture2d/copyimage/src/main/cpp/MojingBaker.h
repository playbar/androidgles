#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <map>
#include "MojingTextureBaker.h"

#define HANDLE_MAX 65535
#define TEXTURE_RESULE_NODE_SIZE 80
#define TEXTURE_RESULT_HEAD_SIZE 100
using namespace std;
using namespace Baofeng::Mojing;


typedef map<int, MojingTextureBaker*> MOJING_TEXTURE_BAKER_MAP_TYPE;


class MojingBaker 
{
public:
	MojingBaker();
	~MojingBaker();
public:
	static MojingBaker* GetInstance();
	int BeginBaker(int iMaxSize);
	int AddTexture(int hHandle, int iTextureID, int iX, int iY, int iWidth, int iHeight);
	int RemoveTexture(int hHandle, int iTextureID, int iX = 0, int iY = 0, int iWidth = 0, int iHeight = 0);
	int GetBakerTextureSize(int hHandle);
	char* BakerTexture(int hHandle, int iTextureID);
	void EndBaker(int hHandle);
private:
	static MojingBaker* _instance;
	MOJING_TEXTURE_BAKER_MAP_TYPE m_mTextureBakerMap;
	int m_hHandle;
	char *m_lpTextureBakerResult;
};
