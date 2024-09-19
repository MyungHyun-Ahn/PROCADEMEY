#pragma once

// 상위 17비트가 논다.
constexpr ULONG_PTR ADDR_MASK = 0x00007FFFFFFFFFFF;
constexpr ULONG_PTR IDENTIFIER_MASK = ~ADDR_MASK;
constexpr INT ZERO_BIT = 17;
constexpr INT NON_ZERO_BIT = 64 - ZERO_BIT;
constexpr ULONG_PTR MAX_IDENTIFIER = 131071;

struct DebugInfo
{
	// Push 0
	// Pop 1
	LONG64 num;
	DWORD pushOrPop;
	USHORT threadId;
	USHORT data;
	void *tPtr;
	void *newTPtr;
};

template<typename Data>
struct SNode
{
	Data data;
	ULONG_PTR identifier;
	SNode *next;
};

template<typename DATA>
struct MPNode
{
#ifdef SAFE_MODE
	void *poolPtr;
#endif
	DATA data;
	ULONG_PTR identifier; // 새로 TOP이 될 수 있는 부분에만 발급
	MPNode *next;

#ifdef SAFE_MODE
	int safeWord = 0xFFFF;
#endif
};

BOOL CheckZeroBitCount();

ULONG_PTR GetNodePtr(ULONG_PTR ptr);

ULONG_PTR GetNodeIdentifier(ULONG_PTR ptr);

ULONG_PTR CombinePtrAndIdnetifier(ULONG_PTR ident, ULONG_PTR ptr);