#include "MojingTextureBaker.h"
//#include "Base/MojingLog.h"
#include <string.h>
#include <GLES3/gl3.h>
namespace Baofeng
{
	namespace Mojing
	{
		MojingTextureBaker::MojingTextureBaker()
		{
			m_iMaxSize = 1024*1024;
			m_iBinWidth = 0;
			m_iBinHeight = 0;
			root.left = NULL;
			root.right = NULL;
		}


		MojingTextureBaker::~MojingTextureBaker()
		{
			if (root.left)
			{
				Destroy(root.left);
			}
			if (root.right)
			{
				Destroy(root.right);
			}
		}

		void MojingTextureBaker::Init()
		{
			int size = GetMaxSize();
			m_iBinWidth = sqrt(size);
			m_iBinHeight = m_iBinWidth;
			root.left = root.right = 0;
			root.x = root.y = 0;
			root.width = m_iBinWidth;
			root.height = m_iBinWidth;
		}

		void MojingTextureBaker::Init(int width, int height)
		{
			m_iBinWidth = width;
			m_iBinHeight = height;
			root.left = root.right = 0;
			root.x = root.y = 0;
			root.width = width;
			root.height = height;
		}

		int MojingTextureBaker::DynamicAddSize()
		{
			root.left = NULL;
			root.right = NULL;
			m_iBinHeight *= 2;
			root.height = m_iBinHeight;
			m_iBinWidth *= 2;
			root.width = m_iBinWidth;
			return m_iBinHeight * m_iBinWidth;
		}

		Node* MojingTextureBaker::Insert(int width, int height, int index)
		{
			return Insert(&root, width, height, index);
		}

		Node *MojingTextureBaker::Insert(Node *node, int width, int height, int index)
		{

			if (node->left || node->right)
			{
				if (node->left)
				{
					Node *newNode = Insert(node->left, width, height, index);
					if (newNode)
						return newNode;
				}
				if (node->right)
				{
					Node *newNode = Insert(node->right, width, height, index);
					if (newNode)
						return newNode;
				}
				return 0; 
			}

			// This node is a leaf, but can we fit the new rectangle here?
			if (width > node->width || height > node->height)
				return 0; // Too bad, no space.

			// The new cell will fit, split the remaining space along the shorter axis,
			// that is probably more optimal.
			int w = node->width - width;
			int h = node->height - height;
			node->left = new Node();
			node->right = new Node();
			if (w <= h) // Split the remaining space in horizontal direction.
			{
				node->index = index;
				node->left->x = node->x + width;
				node->left->y = node->y;
				node->left->width = w;
				node->left->height = height;

				node->right->x = node->x;
				node->right->y = node->y + height;
				node->right->width = node->width;
				node->right->height = h;
			}
			else // Split the remaining space in vertical direction.
			{
				node->index = index;
				node->left->x = node->x;
				node->left->y = node->y + height;
				node->left->width = width;
				node->left->height = h;

				node->right->x = node->x + width;
				node->right->y = node->y;
				node->right->width = w;
				node->right->height = node->height;
			}
			// Note that as a result of the above, it can happen that node->left or node->right
			// is now a degenerate (zero area) rectangle. No need to do anything about it,
			// like remove the nodes as "unnecessary" since they need to exist as children of
			// this node (this node can't be a leaf anymore).

			// This node is now a non-leaf, so shrink its area - it now denotes
			// *occupied* space instead of free space. Its children spawn the resulting
			// area of free space.
			node->width = width;
			node->height = height;
			return node;
		}

		void MojingTextureBaker::GetBakerResult(char** lpResult, int& size)
		{
			PreOrderTraversal(&root, lpResult, size);
		}

		void MojingTextureBaker::PreOrderTraversal(Node *root, char** lpResult, int& size)
		{
			static int i = 0;
			if (root)
			{
				i++;

				if (root->left && root->right)
				{
					int len = sprintf(*lpResult + size,
						"{\n\rSubTextureID:%d,\n\rPosition:[%d,%d],\n\rSize:[%d,%d]\n\r},\n\r",
						root->index, root->x, root->y, root->width, root->height);
					len = len < 0 ? 0 : len;
					size += len;

					GLuint uiFramebuffer;
					glGenFramebuffers(1, &uiFramebuffer);
					glBindFramebuffer(GL_READ_FRAMEBUFFER, uiFramebuffer);
					glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetBakerResultTextureId(), 0);
					glBindTexture(GL_TEXTURE_2D, root->index);
					glCopyTexSubImage2D(GL_TEXTURE_2D, 0, root->x, root->y, 0, 0, root->width, root->height);
					
					glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
					glDeleteBuffers(1, &uiFramebuffer);
					//size = size < 0 ? 0 : size;
					//LOGI("MojingBaker:GetBakerResult result%d = %s, node size=%d", i, *lpResult, size);
					//*lpResult = *lpResult + strlen(*lpResult) - 1;
				}
//				PreOrderTraversal(root->left, lpResult, size);
//				PreOrderTraversal(root->right, lpResult, size);
			}
		}

		void MojingTextureBaker::Destroy(Node* &node)
		{
			if (node)
			{
				Destroy(node->left);
				Destroy(node->right);
				delete node;
				node = NULL;
			}
		}

		void MojingTextureBaker::SetMaxSize(int size)
		{
			m_iMaxSize = TableSizeFor(size);
		}

		int MojingTextureBaker::GetMaxSize()
		{
			if (!m_iMaxSize)
			{
				if (m_lTextureNodeList.size() > 0)
				{
					return m_lTextureNodeList.begin()->rect * m_lTextureNodeList.begin()->rect;
				}
			}
			return m_iMaxSize;
		}

		void MojingTextureBaker::AddNode(int iTextureID, int iX, int iY, int iWidth, int iHeight)
		{
			TextureNode tempNode;
			tempNode.height = iHeight;
			tempNode.width = iWidth;
			tempNode.x = iX;
			tempNode.y = iY;
			tempNode.id = iTextureID;
			m_lTextureNodeList.push_back(tempNode);
		}
		int MojingTextureBaker::RemoveNode(int iTextureID)
		{
			int iRet = 0;
			MojingTextureNodeList::iterator iter = m_lTextureNodeList.begin();
			while (iter != m_lTextureNodeList.end())
			{
				if (iter->id == iTextureID)
				{
					m_lTextureNodeList.erase(iter);
					iRet = 1;
					break;
				}
				iter++;
			}
			return iRet;
		}

		int MojingTextureBaker::GetBakerResultTextureId()
		{
			return m_iResultTextureID;
		}

		void MojingTextureBaker::SetBakerResultTextureId(int id)
		{
			m_iResultTextureID = id;
		}

		int MojingTextureBaker::GetNodeSize()
		{
			return m_lTextureNodeList.size();
		}

		int MojingTextureBaker::GetBakerResultSize()
		{
			return m_iBinHeight * m_iBinWidth;
		}

		int MojingTextureBaker::StartRunTextureBaker()
		{
			Init();
			m_lTextureNodeList = m_lTextureNodeList;
			MojingTextureNodeList::iterator iter = m_lTextureNodeList.begin();
			int max_width;
			int max_height;
			int re_rect;
			while (iter != m_lTextureNodeList.end())
			{
				max_width = iter->width > max_width ? iter->width : max_width;
				max_height = iter->height > max_height ? iter->height : max_height;

				if (iter->height > iter->width)
				{
					re_rect = TableSizeFor(iter->height);
				}
				else
				{
					re_rect = TableSizeFor(iter->width);
				}
				iter->rect = re_rect;
				//LOGI("MojingBaker:StartRunTextureBaker, node id=%d, rect=%d", iter->id, iter->rect);
				iter++;
			}
			m_lTextureNodeList.sort();

			iter = m_lTextureNodeList.begin();
			int iCurrentSize;
			while (iter != m_lTextureNodeList.end())
			{
				if (0 == Insert(iter->rect, iter->rect, iter->id))
				{
					iCurrentSize = DynamicAddSize();
					iter = m_lTextureNodeList.begin();
				}
				else
				{
					iter++;
				}
			}
			return 1;
		}

		int MojingTextureBaker::TableSizeFor(int cap)
		{
			int n = cap;
			if (n >= 1)
			{
				n = n - 1;
				n = n | (n >> 1);
				n = n | (n >> 2);
				n = n | (n >> 4);
				n = n | (n >> 8);
				n = n | (n >> 16);
				return n + 1;
			}
			else
			{
				return 1;
			}
		}
	}
}
