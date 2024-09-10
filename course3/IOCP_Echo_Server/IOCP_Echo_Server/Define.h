#pragma once

// ¼­¹ö Á¤º¸
constexpr const CHAR *SERVER_IP = "0.0.0.0";
constexpr USHORT SERVER_PORT = 6000;

constexpr USHORT PACKET_HEADER_SIZE = 2;

enum class IOOperation
{
	RECV,
	SEND
};

struct OverlappedEx
{
	OverlappedEx(IOOperation oper) : m_Operation(oper) {}

	OVERLAPPED m_Overlapped;
	IOOperation m_Operation;
};

// ½Ì±ÛÅæ
#define SINGLE(type)		private:										\
								type();										\
								~type();									\
								inline static type *m_instPtr = nullptr;	\
							public:											\
								static type *GetInstance()					\
								{											\
									if (m_instPtr == nullptr)				\
									{										\
										m_instPtr = new type();				\
										atexit(Destory);					\
									}										\
																			\
									return m_instPtr;						\
								}											\
																			\
								static void Destory()						\
								{											\
									delete m_instPtr;						\
									m_instPtr = nullptr;					\
								}