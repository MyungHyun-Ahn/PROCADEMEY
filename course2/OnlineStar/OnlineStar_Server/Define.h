#pragma once

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 3000

#define dfSCREEN_WIDTH (80 + 2)
#define dfSCREEN_HEIGHT (23 + 1)

#define RECV_BUFFER_SIZE 1024

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

struct stPos
{
	int m_iX;
	int m_iY;
};