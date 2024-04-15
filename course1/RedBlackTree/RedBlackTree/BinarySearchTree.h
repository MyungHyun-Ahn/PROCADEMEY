#pragma once

template<typename Key>
struct BNode
{
	BNode() {}
	BNode(Key k) : key(k) {}
	BNode(Key k, BNode *l, BNode *r) : key(k), left(l), right(r) {}

	Key key;

	BNode *left = nullptr;
	BNode *right = nullptr;
};

template<typename Key>
class BinarySearchTree
{
	using Node = BNode<Key>;

public:
	BinarySearchTree()
	{

	}
	~BinarySearchTree()
	{
		Clear();
	}

	bool Insert(Key key)
	{
		Node *newNode = new Node(key);
		if (m_pRoot == nullptr)
		{
			m_pRoot = newNode;
			return true;
		}

		Node *curNode = m_pRoot;
		while (true)
		{
			// ������ ��������
			if (key < curNode->key)
			{
				if (curNode->left == nullptr)
				{
					curNode->left = newNode;
					return true;
				}

				curNode = curNode->left;
				continue;
			}

			// ũ�� ����������
			if (key > curNode->key)
			{
				if (curNode->right == nullptr)
				{
					curNode->right = newNode;
					return false;
				}

				curNode = curNode->right;
				continue;
			}
			// Ű �ߺ�
			std::cout << "Ű �ߺ�" << '\n';
			delete newNode;
			return false;
		}

		return false;
	}

	bool Delete(Key key)
	{
		Node *targetNode = nullptr;
		Node *parentNode = nullptr;
		Node *curNode = m_pRoot;

		// �ڽ��� ����� �ڽ�����
		// 0 - left, 1 - right
		int flagLR = -1;

		while (curNode != nullptr)
		{
			// ������ ��������
			if (key < curNode->key)
			{
				parentNode = curNode;
				curNode = curNode->left;
				flagLR = 0;
				continue;
			}

			// ũ�� ����������
			if (key > curNode->key)
			{
				parentNode = curNode;
				curNode = curNode->right;
				flagLR = 1;
				continue;
			}

			targetNode = curNode;
			break;
		}

		if (targetNode == nullptr)
		{
			std::cout << "���� ��� ��� ����" << '\n';
			return false;
		}

		// �ڽ��� ���� ��Ȳ
		if (targetNode->left == nullptr && targetNode->right == nullptr)
		{
			// �ڽ��� ���� ��Ʈ�� ���
			if (targetNode == m_pRoot)
			{
				delete targetNode;
				m_pRoot = nullptr;
				return true;
			}

			if (flagLR == 0)
			{
				parentNode->left = nullptr;
				delete targetNode;
				return true;
			}

			if (flagLR == 1)
			{
				parentNode->right = nullptr;
				delete targetNode;
				return true;
			}
		}

		// ������ �ڽ� 1���� �ִ� ���
		if (targetNode->left == nullptr && targetNode->right != nullptr)
		{
			if (flagLR == 0)
			{
				parentNode->right = targetNode->left;
				delete targetNode;
				return true;
			}

			if (flagLR == 1)
			{
				parentNode->right = targetNode->right;
				delete targetNode;
				return true;
			}
		}

		// ���� �ڽ� 1���� �ִ� ���
		if (targetNode->left != nullptr && targetNode->right == nullptr)
		{
			if (flagLR == 0)
			{
				parentNode->left = targetNode->left;
				delete targetNode;
				return true;
			}

			if (flagLR == 1)
			{
				parentNode->left = targetNode->right;
				delete targetNode;
				return true;
			}
		}

		// ������� ������ ���� ��� �ڽ��� �ִ� ��
		// ���ʿ��� ���� ū ��带 ã�Ƽ� �ڽ����� �ø��� �� ��带 ����

		Node *targetNode2 = targetNode->left;
		Node *parentNode2 = targetNode;

		while (true)
		{
			// ���ʿ��� ���������� ��������.
			if (targetNode2->right == nullptr)
				break;

			parentNode2 = targetNode2;
			targetNode2 = targetNode2->right;
		}
		
		targetNode->key = targetNode2->key;

		if (parentNode2 == targetNode)
		{
			parentNode2->left = nullptr;
			delete targetNode2;
			return true;
		}


		parentNode2->right = nullptr;
		delete targetNode2;
		return true;
	}

	bool Print()
	{
		if (m_pRoot == nullptr)
		{
			std::cout << "Ʈ���� ��� ����" << '\n';
			return false;
		}

		Print(m_pRoot);
		return true;
	}

	bool Clear()
	{
		if (m_pRoot == nullptr)
			return false;

		Clear(m_pRoot);
		return true;
	}

private:
	void Print(Node *node)
	{
		if (node == nullptr)
			return;

		Print(node->left);
		std::cout << node->key << '\n';
		Print(node->right);
	}

	void Clear(Node *node)
	{
		if (node == nullptr)
			return;

		Clear(node->left);
		Clear(node->right);
		delete node;
	}

private:
	Node *m_pRoot = nullptr;
};

