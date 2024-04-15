#pragma once
enum class COLOR
{
	BLACK = 0,
	RED
};

template<typename Key>
struct RBNode
{
	RBNode() {}
	// 신규 노드는 무조건 RED
	RBNode(Key k) : key(k), color(COLOR::RED) {}

	Key key;
	COLOR color = COLOR::RED;
	RBNode *pParent = nullptr;
	RBNode *pLeft = nullptr;
	RBNode *pRight = nullptr;
};

template<typename Key>
class RBTree
{
	using Node = RBNode<Key>;
public:
	RBTree()
	{
		m_pNil = new Node;
		m_pNil->color = COLOR::BLACK;
		m_pNil->pLeft = m_pNil;
		m_pNil->pRight = m_pNil;
	}

	~RBTree()
	{

	}

	bool Insert(Key key)
	{
		Node *newNode = new Node(key);

		// 트리가 비어있는 상황
		if (m_pRoot == nullptr)
		{
			newNode->color = COLOR::BLACK;
			newNode->pLeft = m_pNil;
			newNode->pRight = m_pNil;
			m_pRoot = newNode;
			return true;
		}

		Node *curNode = m_pRoot;
		// L == 0, R == 1
		int flagLR = -1;
		// 노드 자리 찾기
		while (true)
		{
			if (key < curNode->key)
			{
				// 왼쪽 자식이 Nil 노드
				if (curNode->pLeft == m_pNil)
				{
					curNode->pLeft = newNode;
					newNode->pParent = curNode;
					newNode->pLeft = m_pNil;
					newNode->pRight = m_pNil;
					flagLR = 0;
					break;
				}

				curNode = curNode->pLeft;
				continue;
			}

			if (key > curNode->key)
			{
				// 오른쪽 자식이 Nil 노드
				if (curNode->pRight == m_pNil)
				{
					curNode->pRight = newNode;
					newNode->pParent = curNode;
					newNode->pLeft = m_pNil;
					newNode->pRight = m_pNil;
					flagLR = 1;
					break;
				}

				curNode = curNode->pRight;
				continue;
			}

			std::cout << "키 중복" << '\n';
			delete newNode;
			return false;
		}

		// 삽입한 노드의 부모가 BLACK이면 삽입 성공
		if (newNode->pParent->color == COLOR::BLACK)
			return true;

		// 삽입한 노드의 부모가 RED인 경우

		while (newNode->pParent->color == COLOR::RED)
		{
			newNode = InsertBalancing(newNode);

			if (newNode->pParent == nullptr)
			{
				m_pRoot = newNode;
				m_pRoot->color = COLOR::BLACK;
				break;
			}
		}

		return true;
	}

	bool Print()
	{
		if (m_pRoot == nullptr)
		{
			std::cout << "트리가 비었음" << '\n';
			return false;
		}

		Print(m_pRoot);
		return true;
	}

private:
	Node *InsertBalancing(Node *newNode)
	{
		// 여기까지 들어왔으면 부모가 RED 나도 RED인 경우

		// case 1 : 부모 레드, 삼촌 레드
		// 부모와 삼촌을 블랙으로 바꾸고 할아버지는 레드로 바꾼다.
		// 자신이 부모의 왼쪽인 경우
		Node *parent = newNode->pParent;

		if (parent->pLeft == newNode)
		{
			// 삼촌 체크
			// 할아버지의 왼쪽이 부모면 삼촌은 할아버지의 오른쪽
			if (parent->pParent->pLeft == parent)
			{
				if (parent->pParent->pRight->color == COLOR::RED)
				{
					parent->color = COLOR::BLACK;
					parent->pParent->pRight->color = COLOR::BLACK;
					// 할아버지 노드 반환
					return parent->pParent;
				}

				RotateRight(parent->pParent);
				return newNode->pParent;
			}

			if (parent->pParent->pLeft->color == COLOR::RED)
			{
				parent->color = COLOR::BLACK;
				parent->pParent->pLeft->color = COLOR::BLACK;
				// 할아버지 노드 반환
				return parent->pParent;
			}

			// 왼쪽 레드, 부모 레드, 삼촌 블랙
			parent->color = COLOR::BLACK;
			parent->pParent->color = COLOR::RED;

			RotateRight(parent->pParent);
			return newNode->pParent;
		}

		// 자신이 부모의 오른쪽인 경우
		if (parent->pRight == newNode)
		{
			// 부모가 할아버지의 왼쪽 자식
			if (parent->pParent->pLeft == parent)
			{
				if (parent->pParent->pRight->color == COLOR::RED)
				{
					parent->color = COLOR::BLACK;
					parent->pParent->pRight->color = COLOR::BLACK;
					// 할아버지 노드 반환
					return parent->pParent;
				}

				// case 2 부모의 오른쪽, 삼촌은 블랙
				// 부모 기준으로 좌회전
				RotateLeft(parent);
				// parent가 끝단 노드가 된 상황
				return parent;
			}

			if (parent->pParent->pLeft->color == COLOR::RED)
			{
				parent->color = COLOR::BLACK;
				parent->pParent->pLeft->color = COLOR::BLACK;

				// 할아버지 노드 반환
				return parent->pParent;
			}


			parent->color = COLOR::BLACK;
			parent->pParent->color = COLOR::RED;
			RotateLeft(parent->pParent);
			// parent가 끝단 노드가 된 상황
			return newNode->pParent;
		}
	}

	// 우회전
	void RotateRight(Node *node)
	{
		Node *grandPa = node->pParent;
		Node *a = node;
		Node *b = node->pLeft;

		Node *br = b->pRight;
		b->pRight = a;
		b->pParent = grandPa;

		a->pParent = b;
		a->pLeft = br;

		br->pParent = a;
	}

	// 좌회전
	void RotateLeft(Node *node)
	{
		Node *grandPa = node->pParent;
		Node *a = node;
		Node *c = node->pRight;

		Node *cl = c->pLeft;
		c->pLeft = a;
		c->pParent = grandPa;

		a->pParent = c;
		a->pRight = cl;

		cl->pParent = a;
	}

	void Print(Node *node)
	{
		if (node == m_pNil)
			return;

		Print(node->pLeft);
		std::cout << node->key << '\n';
		Print(node->pRight);
	}

private:
	Node *m_pRoot = nullptr;
	Node *m_pNil = nullptr;
};

