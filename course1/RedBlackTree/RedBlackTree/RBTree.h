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
	// �ű� ���� ������ RED
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

		// Ʈ���� ����ִ� ��Ȳ
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
		// ��� �ڸ� ã��
		while (true)
		{
			if (key < curNode->key)
			{
				// ���� �ڽ��� Nil ���
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
				// ������ �ڽ��� Nil ���
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

			std::cout << "Ű �ߺ�" << '\n';
			delete newNode;
			return false;
		}

		// ������ ����� �θ� BLACK�̸� ���� ����
		if (newNode->pParent->color == COLOR::BLACK)
			return true;

		// ������ ����� �θ� RED�� ���

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
			std::cout << "Ʈ���� �����" << '\n';
			return false;
		}

		Print(m_pRoot);
		return true;
	}

private:
	Node *InsertBalancing(Node *newNode)
	{
		// ������� �������� �θ� RED ���� RED�� ���

		// case 1 : �θ� ����, ���� ����
		// �θ�� ������ ������ �ٲٰ� �Ҿƹ����� ����� �ٲ۴�.
		// �ڽ��� �θ��� ������ ���
		Node *parent = newNode->pParent;

		if (parent->pLeft == newNode)
		{
			// ���� üũ
			// �Ҿƹ����� ������ �θ�� ������ �Ҿƹ����� ������
			if (parent->pParent->pLeft == parent)
			{
				if (parent->pParent->pRight->color == COLOR::RED)
				{
					parent->color = COLOR::BLACK;
					parent->pParent->pRight->color = COLOR::BLACK;
					// �Ҿƹ��� ��� ��ȯ
					return parent->pParent;
				}

				RotateRight(parent->pParent);
				return newNode->pParent;
			}

			if (parent->pParent->pLeft->color == COLOR::RED)
			{
				parent->color = COLOR::BLACK;
				parent->pParent->pLeft->color = COLOR::BLACK;
				// �Ҿƹ��� ��� ��ȯ
				return parent->pParent;
			}

			// ���� ����, �θ� ����, ���� ��
			parent->color = COLOR::BLACK;
			parent->pParent->color = COLOR::RED;

			RotateRight(parent->pParent);
			return newNode->pParent;
		}

		// �ڽ��� �θ��� �������� ���
		if (parent->pRight == newNode)
		{
			// �θ� �Ҿƹ����� ���� �ڽ�
			if (parent->pParent->pLeft == parent)
			{
				if (parent->pParent->pRight->color == COLOR::RED)
				{
					parent->color = COLOR::BLACK;
					parent->pParent->pRight->color = COLOR::BLACK;
					// �Ҿƹ��� ��� ��ȯ
					return parent->pParent;
				}

				// case 2 �θ��� ������, ������ ��
				// �θ� �������� ��ȸ��
				RotateLeft(parent);
				// parent�� ���� ��尡 �� ��Ȳ
				return parent;
			}

			if (parent->pParent->pLeft->color == COLOR::RED)
			{
				parent->color = COLOR::BLACK;
				parent->pParent->pLeft->color = COLOR::BLACK;

				// �Ҿƹ��� ��� ��ȯ
				return parent->pParent;
			}


			parent->color = COLOR::BLACK;
			parent->pParent->color = COLOR::RED;
			RotateLeft(parent->pParent);
			// parent�� ���� ��尡 �� ��Ȳ
			return newNode->pParent;
		}
	}

	// ��ȸ��
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

	// ��ȸ��
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

