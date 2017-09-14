#pragma once
#include <stdio.h>
#include <math.h>
#include <list>

namespace Baofeng
{
	namespace Mojing
	{
		struct Node
		{
			Node* left;
			Node* right;
			int x;
			int y;
			int width;
			int height;
			int index;
		};
		struct TextureNode
		{
			int x;
			int y;
			int width;
			int height;
			int rect;
			int id;
			bool operator < (const struct TextureNode& t)
			{
				return rect > t.rect;
			}
		};
		typedef std::list<TextureNode> MojingTextureNodeList;
		class MojingTextureBaker
		{
		public:
			MojingTextureBaker();
			~MojingTextureBaker();
		public:
			
			void Init();
			void Init(int width, int height);
			Node *Insert(int width, int height, int index);
			int StartRunTextureBaker();
			int GetBakerResultSize();
			void GetBakerResult(char** lpResult, int& size);
			void AddNode(int iTextureID, int iX, int iY, int iWidth, int iHeight);
			int RemoveNode(int iTextureID); 
			int GetNodeSize();
			void SetMaxSize(int size);
			int GetMaxSize();
			int GetBakerResultTextureId();
			void SetBakerResultTextureId(int id);
		private:
			int DynamicAddSize();
			Node *Insert(Node *node, int width, int height, int index);
			int TableSizeFor(int cap);
			void PreOrderTraversal(Node *root, char** lpResult, int& size);
			void Destroy(Node* &node);
		private:
			Node root;
			int m_iBinWidth;
			int m_iBinHeight;
			int m_iMaxSize;
			int m_iResultTextureID;
			MojingTextureNodeList m_lTextureNodeList;
		};
	}
}