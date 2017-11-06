#pragma once
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
using namespace std;
#ifndef WIN32

#include <algorithm> // new , delete...
#if (defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))) || defined(__MACOS__)
#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
typedef long HANDLE;
#else
#include <GLES3/gl3.h>
typedef long HANDLE;
#endif
typedef int TEXTURE_ID;
#else
#include <wtypes.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")  
#include <string>
#ifndef TEXTURE_ID
typedef struct __tagTEXTURE_ID
{
	__tagTEXTURE_ID(int i)
	{
		m_pImage = (Bitmap *)i;
		m_szFileName = "";
	}
	__tagTEXTURE_ID(const __tagTEXTURE_ID&Other)
	{
		m_pImage = Other.m_pImage;
		m_szFileName = Other.m_szFileName;
	}
	__tagTEXTURE_ID(const char* szFileName , Bitmap* pImage)
	{
		m_pImage = pImage;
		m_szFileName = szFileName;
	}

	__tagTEXTURE_ID& operator=(const __tagTEXTURE_ID&Other)
	{
		m_pImage = Other.m_pImage;
		m_szFileName = Other.m_szFileName;
		return *this;
	}
	Bitmap *m_pImage;
	string m_szFileName;
	const char *c_str()const
	{
		return m_szFileName.c_str();
	}
	bool operator ==(const __tagTEXTURE_ID &Other)
	{
		return m_pImage == Other.m_pImage && m_szFileName == Other.m_szFileName;
	}
}TEXTURE_ID;
extern bool operator ==(const TEXTURE_ID &A, const TEXTURE_ID &B);
#endif // TEXTURE_ID

#endif

typedef struct __tagProgressInfo
{
	__tagProgressInfo()
	{
		m_iProgressNow = *m_szProgressText = 0;
		m_iProgressMax = 1;
	}
	char m_szProgressText[512];
	int m_iProgressNow;
	int m_iProgressMax;
}ProgressInfo, *LPProgressInfo;
typedef void(*FP_UpdateProgressInfo)(LPProgressInfo  pProgressInfo);

// #define _DEBUG
#define JSON_IO(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	virtual void _NAME_##ToJson(JSON* pJson){ pJson->AddNumberItem(#_NAME_, _PREV_NAME_##_NAME_); }; \
	virtual bool _NAME_##FromJson(JSON* pJson)\
	{\
	JSON* pSubJson = pJson->GetItemByName(#_NAME_); \
if (pSubJson) \
		{\
		Set##_NAME_((_TYPE_)pSubJson->GetDoubleValue()); \
		return true; \
		}; \
		return false; \
	};

#define JSON_IO_ARRAY(_TYPE_ ,_PREV_NAME_, _NAME_, _SIZE_)\
	virtual void _NAME_##ToJson(JSON* pJson){ JSON* pArray = JSON::CreateArray(); for (int i = 0; i < _SIZE_; i++)pArray->AddArrayNumber(_PREV_NAME_##_NAME_[i]); pJson->AddItem(#_NAME_, pArray); }; \
	virtual bool _NAME_##FromJson(JSON* pJson)\
	{\
	bool bRet = true; \
	JSON* pJsonOBJ = pJson->GetItemByName(#_NAME_); \
if (pJsonOBJ &&pJsonOBJ->HasItems() && pJsonOBJ->GetArraySize() == _SIZE_) \
		{\
for (int i = 0; i < _SIZE_; i++)\
			{\
			_PREV_NAME_##_NAME_[i] = (_TYPE_)pJsonOBJ->GetArrayNumber(i); \
			}\
			return true; \
		}\
		return false; \
	}

#define JSON_IO_V2( _PREV_NAME_ , _NAME_ )\
	virtual void _NAME_##ToJson(JSON* pJson){ JSON* pJsonOBJ = pJson->CreateObject();  pJsonOBJ->AddNumberItem("x", _PREV_NAME_##_NAME_.x); pJsonOBJ->AddNumberItem("y", _PREV_NAME_##_NAME_.y); pJson->AddItem(#_NAME_, pJsonOBJ); }; \
	virtual bool _NAME_##FromJson(JSON* pJson)\
	{\
	JSON* pJsonOBJ = pJson->GetItemByName(#_NAME_); \
if (pJsonOBJ) \
		{\
		JSON* pJsonOBJ_X = pJsonOBJ->GetItemByName("x"); \
		JSON* pJsonOBJ_Y = pJsonOBJ->GetItemByName("y"); \
if (pJsonOBJ_X && pJsonOBJ_Y && pJsonOBJ_Z)\
			{\
			_PREV_NAME_##_NAME_.x = pJsonOBJ_X->GetDoubleValue(); \
			_PREV_NAME_##_NAME_.y = pJsonOBJ_Y->GetDoubleValue(); \
			return true; \
			}\
		}; \
		return false; \
	};

#define JSON_IO_V3( _PREV_NAME_ , _NAME_ )\
	virtual void _NAME_##ToJson(JSON* pJson){ JSON* pJsonOBJ = pJson->CreateArray(); pJsonOBJ->AddArrayNumber(_PREV_NAME_##_NAME_.x); pJsonOBJ->AddArrayNumber(_PREV_NAME_##_NAME_.y); pJsonOBJ->AddArrayNumber(_PREV_NAME_##_NAME_.z); pJson->AddItem(#_NAME_, pJsonOBJ); }; \
	virtual bool _NAME_##FromJson(JSON* pJson)\
	{\
	JSON* pJsonOBJ = pJson->GetItemByName(#_NAME_); \
if (pJsonOBJ && pJsonOBJ->Type == JSON_Array&& pJsonOBJ->GetArraySize() == 3) \
		{\
		_PREV_NAME_##_NAME_.x = pJsonOBJ->GetArrayNumber(0); \
		_PREV_NAME_##_NAME_.y = pJsonOBJ->GetArrayNumber(1); \
		_PREV_NAME_##_NAME_.z = pJsonOBJ->GetArrayNumber(2); \
		return true; \
		} \
		return false; \
	};


#define JSON_IO_V4(_PREV_NAME_ , _NAME_ )\
	virtual void _NAME_##ToJson(JSON* pJson){ JSON* pJsonOBJ = pJson->CreateArray(); pJsonOBJ->AddArrayNumber(_PREV_NAME_##_NAME_.x); pJsonOBJ->AddArrayNumber(_PREV_NAME_##_NAME_.y); pJsonOBJ->AddArrayNumber(_PREV_NAME_##_NAME_.z); pJsonOBJ->AddArrayNumber(_PREV_NAME_##_NAME_.w); pJson->AddItem(#_NAME_, pJsonOBJ); }; \
	virtual bool _NAME_##FromJson(JSON* pJson)\
	{\
	JSON* pJsonOBJ = pJson->GetItemByName(#_NAME_); \
if (pJsonOBJ && pJsonOBJ->Type == JSON_Array&& pJsonOBJ->GetArraySize() == 4) \
		{\
		_PREV_NAME_##_NAME_.x = pJsonOBJ->GetArrayNumber(0); \
		_PREV_NAME_##_NAME_.y = pJsonOBJ->GetArrayNumber(1); \
		_PREV_NAME_##_NAME_.z = pJsonOBJ->GetArrayNumber(2); \
		_PREV_NAME_##_NAME_.w = pJsonOBJ->GetArrayNumber(3); \
		return true; \
		}; \
		return false; \
	};


#define JSON_IO_M4(_PREV_NAME_ , _NAME_ )\
	virtual void _NAME_##ToJson(JSON* pJson)\
	{\
	JSON* pJsonOBJ = pJson->CreateArray(); \
	int i = 0, j; \
while (i < 4)\
		{\
		j = 0; \
		JSON* pArray = pJsonOBJ->CreateArray(); \
while (j < 4)\
			{\
			pArray->AddArrayNumber(_PREV_NAME_##_NAME_.M[i][j]); \
			j++; \
			}\
			pJsonOBJ->AddArrayElement(pArray); \
			i++; \
		}\
		pJson->AddItem(#_NAME_, pJsonOBJ); \
	}\
	virtual bool _NAME_##FromJson(JSON* pJson)\
	{\
	JSON* pJsonOBJ = pJson->GetItemByName(#_NAME_); \
if (pJsonOBJ)\
		{\
		int i = 0, j; \
while (i < 4)\
			{\
			j = 0; \
			JSON* pArray = pJsonOBJ->GetItemByIndex(i); \
while (j < 4)\
				{\
				_PREV_NAME_##_NAME_.M[i][j] = pArray->GetArrayNumber(j); \
				j++; \
				}\
				i++; \
			}\
			return true; \
		}\
		return  false; \
	}




#define JSON_IO_STR( _PREV_NAME_ , _NAME_ )\
	virtual void _NAME_##ToJson(JSON* pJson){ pJson->AddStringItem(#_NAME_, _PREV_NAME_##_NAME_); }; \
	virtual bool _NAME_##FromJson(JSON* pJson)\
	{\
	JSON* pSubJson = pJson->GetItemByName(#_NAME_); \
if (pSubJson) \
		{\
		Set##_NAME_(pSubJson->GetStringValue()); \
		return true; \
		}; \
		return false; \
	};


#define CLASS_MEMBER(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	protected:\
	_TYPE_ _PREV_NAME_##_NAME_; \
	public:\
	virtual void Set##_NAME_(_TYPE_ V){ _PREV_NAME_##_NAME_ = V; }; \
	virtual _TYPE_ Get##_NAME_()const{ return _PREV_NAME_##_NAME_; };

#define CLASS_MEMBER_REF(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	protected:\
	_TYPE_ _PREV_NAME_##_NAME_; \
	public:\
	virtual _TYPE_ & Get##_NAME_(){ return _PREV_NAME_##_NAME_; };

#define CLASS_MEMBER_STR(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	protected:\
	_TYPE_ _PREV_NAME_##_NAME_; \
	public:\
	virtual void Set##_NAME_(_TYPE_ V){ _PREV_NAME_##_NAME_ = V; }; \
	virtual void Set##_NAME_(const char*  V){ _PREV_NAME_##_NAME_ = ((V != NULL) ? (V) : ""); }; \
	virtual const char* Get##_NAME_()const{ return _PREV_NAME_##_NAME_.c_str(); };

#define CLASS_MEMBER_ARRAY(_TYPE_ , _PREV_NAME_ , _NAME_ , _SIZE_)\
	protected:\
	_TYPE_ _PREV_NAME_##_NAME_[_SIZE_]; \
	public:\
	virtual void Set##_NAME_(_TYPE_ V){ memcpy(_PREV_NAME_##_NAME_, V, sizeof(_TYPE_)* _SIZE_); }; \
	virtual _TYPE_ Get##_NAME_(int index)const{ return _PREV_NAME_##_NAME_[index]; };

#define CLASS_INTERFACE(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	protected:\
	_TYPE_ _PREV_NAME_##_NAME_; \
	virtual void Set##_NAME_(_TYPE_ V){ _PREV_NAME_##_NAME_ = V; }; \
	public:\
	virtual _TYPE_ Get##_NAME_()const{ return _PREV_NAME_##_NAME_; };

#define CLASS_INTERFACE_STR(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	protected:\
	_TYPE_ _PREV_NAME_##_NAME_; \
	virtual void Set##_NAME_(_TYPE_ V){ _PREV_NAME_##_NAME_ = V; }; \
	virtual void Set##_NAME_(const char*  V){ _PREV_NAME_##_NAME_ = V; }; \
	public:\
	virtual const char* Get##_NAME_()const{ return _PREV_NAME_##_NAME_; };


#define CLASS_MEMBER_J(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	CLASS_MEMBER(_TYPE_, _PREV_NAME_, _NAME_)\
	JSON_IO(_TYPE_, _PREV_NAME_, _NAME_)

#define CLASS_MEMBER_V2_J(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	CLASS_MEMBER(_TYPE_, _PREV_NAME_, _NAME_)\
	JSON_IO_V2(_PREV_NAME_, _NAME_)

#define CLASS_MEMBER_V3_J(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	CLASS_MEMBER(_TYPE_, _PREV_NAME_, _NAME_)\
	JSON_IO_V3(_PREV_NAME_, _NAME_)

#define CLASS_MEMBER_V4_J(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	CLASS_MEMBER(_TYPE_, _PREV_NAME_, _NAME_)\
	JSON_IO_V4(_PREV_NAME_, _NAME_)

#define CLASS_MEMBER_M4_J(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	CLASS_MEMBER(_TYPE_, _PREV_NAME_, _NAME_)\
	JSON_IO_M4(_PREV_NAME_, _NAME_)

#define CLASS_MEMBER_STR_J(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	CLASS_MEMBER_STR(_TYPE_, _PREV_NAME_, _NAME_)\
	JSON_IO_STR(_PREV_NAME_, _NAME_)

#define CLASS_INTERFACE_J(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	CLASS_INTERFACE(_TYPE_, _PREV_NAME_, _NAME_)\
	JSON_IO(_TYPE_, _PREV_NAME_, _NAME_)

#define CLASS_INTERFACE_STR_J(_TYPE_ , _PREV_NAME_ , _NAME_ )\
	CLASS_INTERFACE_STR(_TYPE_, _PREV_NAME_, _NAME_)\
	JSON_IO_STR(_PREV_NAME_, _NAME_)

