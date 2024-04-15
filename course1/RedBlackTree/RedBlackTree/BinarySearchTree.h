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
			// 작으면 왼쪽으로
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

			// 크면 오른쪽으로
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
			// 키 중복
			std::cout << "키 중복" << '\n';
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

		// 자신이 어느쪽 자식인지
		// 0 - left, 1 - right
		int flagLR = -1;

		while (curNode != nullptr)
		{
			// 작으면 왼쪽으로
			if (key < curNode->key)
			{
				parentNode = curNode;
				curNode = curNode->left;
				flagLR = 0;
				continue;
			}

			// 크면 오른쪽으로
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
			std::cout << "삭제 대상 노드 없음" << '\n';
			return false;
		}

		// 자식이 없는 상황
		if (targetNode->left == nullptr && targetNode->right == nullptr)
		{
			// 자식이 없고 루트인 경우
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

		// 오른쪽 자식 1개만 있는 경우
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

		// 왼쪽 자식 1개만 있는 경우
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

		// 여기까지 왔으면 양쪽 모두 자식이 있는 것
		// 왼쪽에서 가장 큰 노드를 찾아서 자신으로 올리고 그 노드를 삭제

		Node *targetNode2 = targetNode->left;
		Node *parentNode2 = targetNode;

		while (true)
		{
			// 왼쪽에서 오른쪽으로 내려간다.
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
			std::cout << "트리가 비어 있음" << '\n';
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

