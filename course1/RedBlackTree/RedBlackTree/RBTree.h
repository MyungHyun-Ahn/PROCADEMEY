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
		m_pNil->pParent = m_pNil;
		m_pRoot = m_pNil;
	}

	~RBTree()
	{

	}

	bool Insert(Key key)
	{
		Node *newNode = new Node(key);

		// 트리가 비어있는 상황
		if (m_pRoot == m_pNil)
		{
			newNode->color = COLOR::BLACK;
			newNode->pLeft = m_pNil;
			newNode->pRight = m_pNil;
			newNode->pParent = m_pNil;
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

		InsertBalancing(newNode);

		return true;
	}

	bool Delete(Key key)
	{
		if (m_pRoot == m_pNil)
		{
			std::cout << "트리가 비었음" << std::endl;
			return false;
		}

		Node *target = Search(key);

		if (target == m_pNil || target == nullptr)
		{
			std::cout << "삭제 대상 노드 없음" << '\n';
			return false;
		}

		Node *y = target;
		COLOR originColor = y->color;

		Node *x = nullptr;

		if (target->pLeft == m_pNil)
		{
			x = target->pRight;
			Transplant(target, target->pRight);
		}
		else if (target->pRight == m_pNil)
		{
			x = target->pLeft;
			Transplant(target, target->pLeft);
		}
		else
		{
			y = Minimum(target->pRight);

			originColor = y->color;
			x = y->pRight;

			if (y != target->pRight)
			{
				Transplant(y, y->pRight);
				y->pRight = target->pRight;
				y->pRight->pParent = y;
			}
			else
				x->pParent = y;

			Transplant(target, y);
			y->pLeft = target->pLeft;
			y->pLeft->pParent = y;
			y->color = target->color;
		}

		if (originColor == COLOR::BLACK)
			deleteBalancing(x);

		delete target;
		return true;
	}

	Node *Search(Key key)
	{
		Node *targetNode = m_pRoot;

		while (targetNode != m_pNil && key != targetNode->key)
		{
			// 작으면 왼쪽으로
			if (key < targetNode->key)
				targetNode = targetNode->pLeft;
			else
				targetNode = targetNode->pRight;
		}

		if (targetNode == m_pNil || targetNode == nullptr)
			return nullptr;

		return targetNode;
	}

	bool Print()
	{
		if (m_pRoot == m_pNil)
		{
			std::cout << "트리가 비었음" << '\n';
			return false;
		}

		Print(m_pRoot);
		return true;
	}

	bool Print(std::map<int, int>::iterator it)
	{
		m_it = it;
		m_useIt = true;

		if (m_pRoot == m_pNil)
		{
			std::cout << "트리가 비었음" << '\n';
			return false;
		}

		Print(m_pRoot);
		m_useIt = false;
		return true;
	}

private:
	void InsertBalancing(Node *newNode)
	{
		Node *uncle = nullptr;
		while (newNode->pParent->color == COLOR::RED)
		{
			Node *grandPa = newNode->pParent->pParent;
			if (newNode->pParent == grandPa->pLeft)
			{
				uncle = grandPa->pRight;

				if (uncle->color == COLOR::RED)
				{
					newNode->pParent->color = COLOR::BLACK;
					uncle->color = COLOR::BLACK;
					grandPa->color = COLOR::RED;
					newNode = grandPa;
				}
				else
				{
					if (newNode == newNode->pParent->pRight)
					{
						newNode = newNode->pParent;
						RotateLeft(newNode);
					}

					newNode->pParent->color = COLOR::BLACK;
					grandPa->color = COLOR::RED;
					RotateRight(grandPa);
				}
			}
			else
			{
				uncle = grandPa->pLeft;
				if (uncle->color == COLOR::RED)
				{
					newNode->pParent->color = COLOR::BLACK;
					uncle->color = COLOR::BLACK;
					grandPa->color = COLOR::RED;
					newNode = grandPa;
				}
				else
				{
					if (newNode == newNode->pParent->pLeft)
					{
						newNode = newNode->pParent;
						RotateRight(newNode);
					}

					newNode->pParent->color = COLOR::BLACK;
					grandPa->color = COLOR::RED;
					RotateLeft(grandPa);
				}
			}
		}
		m_pRoot->color = COLOR::BLACK;
	}
	// Node *InsertBalancing(Node *newNode)
	// {
	// 	// 여기까지 들어왔으면 부모가 RED 나도 RED인 경우
	// 
	// 	// case 1 : 부모 레드, 삼촌 레드
	// 	// 부모와 삼촌을 블랙으로 바꾸고 할아버지는 레드로 바꾼다.
	// 	// 자신이 부모의 왼쪽인 경우
	// 	Node *parent = newNode->pParent;
	// 
	// 	Node *uncle;
	// 	if (parent->pParent->pLeft == parent)
	// 		uncle = parent->pParent->pRight;
	// 	else
	// 		uncle = parent->pParent->pLeft;
	// 
	// 	if (uncle->color == COLOR::RED)
	// 	{
	// 		parent->color = COLOR::BLACK;
	// 		uncle->color = COLOR::BLACK;
	// 		parent->pParent->color = COLOR::RED;
	// 		// 할아버지 노드 반환
	// 		return parent->pParent;
	// 	}
	// 
	// 	if (parent->pLeft == newNode)
	// 	{
	// 		if (parent->pParent->pLeft == parent)
	// 		{
	// 			parent->color = COLOR::BLACK;
	// 			parent->pParent->color = COLOR::RED;
	// 
	// 			RotateRight(parent->pParent);
	// 			return newNode->pParent;
	// 		}
	// 
	// 		RotateRight(parent);
	// 		return parent;
	// 	}
	// 
	// 	// 자신이 부모의 오른쪽인 경우
	// 	if (parent->pRight == newNode)
	// 	{
	// 		// 부모가 할아버지의 왼쪽 자식
	// 		if (parent->pParent->pRight == parent)
	// 		{
	// 			parent->color = COLOR::BLACK;
	// 			parent->pParent->color = COLOR::RED;
	// 
	// 			RotateLeft(parent->pParent);
	// 
	// 			return parent;
	// 		}
	// 
	// 		RotateLeft(parent);
	// 		return parent;
	// 	}
	// }

	void deleteBalancing(Node *deleteNode)
	{
		Node *sibling = nullptr;

		while (deleteNode != m_pRoot && deleteNode->color == COLOR::BLACK)
		{
			if (deleteNode == deleteNode->pParent->pLeft)
			{
				sibling = deleteNode->pParent->pRight;
				// 형제 레드
				if (sibling->color == COLOR::RED)
				{
					sibling->color = COLOR::BLACK;
					deleteNode->pParent->color = COLOR::RED;
					RotateLeft(deleteNode->pParent);
					sibling = deleteNode->pParent->pRight;
				}

				// 형제 자식 모두 블랙
				if (sibling->pLeft->color == COLOR::BLACK && sibling->pRight->color == COLOR::BLACK)
				{
					sibling->color = COLOR::RED;
					deleteNode = deleteNode->pParent;
				}
				else
				{
					// 형제 오른쪽 블랙
					if (sibling->pRight->color == COLOR::BLACK)
					{
						sibling->pLeft->color = COLOR::BLACK;
						sibling->color = COLOR::RED;
						RotateRight(sibling);
						sibling = deleteNode->pParent->pRight;
					}
					
					// 형제 오른쪽 레드
					sibling->color = deleteNode->pParent->color;
					deleteNode->pParent->color = COLOR::BLACK;
					sibling->pRight->color = COLOR::BLACK;

					RotateLeft(deleteNode->pParent);

					deleteNode = m_pRoot;
				}
			}
			else
			{
				sibling = deleteNode->pParent->pLeft;
				// 형제 레드
				if (sibling->color == COLOR::RED)
				{
					sibling->color = COLOR::BLACK;
					deleteNode->pParent->color = COLOR::RED;
					RotateRight(deleteNode->pParent);
					sibling = deleteNode->pParent->pLeft;
				}

				// 형제 자식 모두 블랙
				if (sibling->pRight->color == COLOR::BLACK && sibling->pLeft->color == COLOR::BLACK)
				{
					sibling->color = COLOR::RED;
					deleteNode = deleteNode->pParent;
				}
				else
				{
					// 형제 왼쪽 블랙
					if (sibling->pLeft->color == COLOR::BLACK)
					{
						sibling->pRight->color = COLOR::BLACK;
						sibling->color = COLOR::RED;
						RotateLeft(sibling);
						sibling = deleteNode->pParent->pLeft;
					}

					// 형제 왼쪽 레드
					sibling->color = deleteNode->pParent->color;
					deleteNode->pParent->color = COLOR::BLACK;
					sibling->pLeft->color = COLOR::BLACK;

					RotateRight(deleteNode->pParent);

					deleteNode = m_pRoot;
				}
			}
		}

		deleteNode->color = COLOR::BLACK;
	}

	void Transplant(Node *u, Node *v)
	{
		if (u->pParent == m_pNil)
			m_pRoot = v;
		else if (u == u->pParent->pLeft)
			u->pParent->pLeft = v;
		else
			u->pParent->pRight = v;

		v->pParent = u->pParent;
	}

	Node *Minimum(Node *node)
	{
		if (node == m_pNil)
			return m_pNil;

		Node *minNode = node;
		while (minNode->pLeft != m_pNil)
			minNode = minNode->pLeft;

		return minNode;
	}

	// 좌회전
	void RotateLeft(Node *node)
	{
		Node *rightChild = node->pRight;
		node->pRight = rightChild->pLeft;
		rightChild->pParent = node->pParent;
	
		if (rightChild->pLeft != m_pNil)
		{
			rightChild->pLeft->pParent = node;
		}
	
		if (node->pParent == m_pNil)
			m_pRoot = rightChild;
		else if (node == node->pParent->pLeft)
			node->pParent->pLeft = rightChild;
		else
			node->pParent->pRight = rightChild;
	
		rightChild->pLeft = node;
		node->pParent = rightChild;
	}

	// 우회전
	void RotateRight(Node *node)
	{
		Node *leftChild = node->pLeft;
		node->pLeft = leftChild->pRight;
		leftChild->pParent = node->pParent;

		if (leftChild->pRight != m_pNil)
		{
			leftChild->pRight->pParent = node;
		}

		if (node->pParent == m_pNil)
			m_pRoot = leftChild;
		else if (node == node->pParent->pLeft)
			node->pParent->pLeft = leftChild;
		else
			node->pParent->pRight = leftChild;

		leftChild->pRight = node;
		node->pParent = leftChild;
	}

	void Print(Node *node)
	{
		if (node == m_pNil || node == nullptr)
			return;

		Print(node->pLeft);
		std::string color = (node->color == COLOR::BLACK) ? "BLACK" : "RED";

		if (m_useIt)
		{
			if (node->key != m_it->first)
			{
				std::cout << "잘못됨" << std::endl;
			}
		}


		std::cout << "My RB Tree : " << node->key << " " << color << '\n';

		if (m_useIt)
		{
			std::cout << "std::map : " << m_it->first << " " << '\n';
			++m_it;
		}
		Print(node->pRight); 
	}

private:
	Node *m_pRoot;
	Node *m_pNil = nullptr;
	std::map<int, int>::iterator m_it;
	bool m_useIt = false;
};

