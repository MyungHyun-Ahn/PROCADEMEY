// Thread_Debug.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "define.h"

////////////////////////////////////////////////////////
// 컨텐츠 부
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// 접속요청 목록. 
//
// IOThread 에서 주기적으로 삽입, 
// AcceptThread 에서 이 리스트의 값(SessionID 값)을 뽑아서  새로운 Session 을 만든다.
// 이미 존재하는 SessionID 가 나올경우 무시 해야 함
////////////////////////////////////////////////////////
CRITICAL_SECTION	g_Accept_cs;
list<DWORD>			g_AcceptPacketList;

#define				LockAccept()	EnterCriticalSection(&g_Accept_cs)
#define 			UnlockAccept()	LeaveCriticalSection(&g_Accept_cs)

////////////////////////////////////////////////////////
// 액션 요청 목록.
//
// IOThread 에서 주기적으로 삽입(SessionID),
// UpdateThread 에서는 이 값을 뽑아서 해당 플레이어의 Content + 1 을 한다.
// 존재하지 않는 SessionID 가 나오면 무시해야 함.
////////////////////////////////////////////////////////
CRITICAL_SECTION	g_Action_cs;
list<DWORD>			g_ActionPacketList;

#define				LockAction()	EnterCriticalSection(&g_Action_cs)
#define				UnlockAction()	LeaveCriticalSection(&g_Action_cs)

////////////////////////////////////////////////////////
// 접속종료 요청 목록
//
// IOThread 에서 주기적으로 삽입, 
// AcceptThread 에서 이 리스트의 값을 뽑아서 (SessionID 값) 해당 Session 을 종료 시킨다.
////////////////////////////////////////////////////////
CRITICAL_SECTION	g_Disconnect_cs;
list<DWORD>			g_DisconnectPacketList;

#define				LockDisconnect()	EnterCriticalSection(&g_Disconnect_cs)
#define				UnlockDisconnect()	LeaveCriticalSection(&g_Disconnect_cs)






////////////////////////////////////////////////////////
// Session 목록.
//
// 접속이 완료시 (Accept 처리 완료)  st_SESSION 를 동적 생성하여, SessionList 에 포인터를 넣는다.
// 그리고 접속이 끊어질 시 해당 세션을 삭제 한다.
////////////////////////////////////////////////////////
CRITICAL_SECTION		g_Session_cs;
list<st_SESSION *>		g_SessionList;

#define	LockSession()	EnterCriticalSection(&g_Session_cs)
#define UnlockSession()	LeaveCriticalSection(&g_Session_cs)

////////////////////////////////////////////////////////
// Player 목록.
//
// Session 이 생성 후, 생성 될때 (Accept 처리 완료시)  st_PLAYER 객체도 함께 생성되어 여기에 등록 된다.
////////////////////////////////////////////////////////
CRITICAL_SECTION		g_Player_cs;
list<st_PLAYER *>		g_PlayerList;

void LockPlayer()
{
	EnterCriticalSection(&g_Player_cs);
}

void UnlockPlayer()
{
	LeaveCriticalSection(&g_Player_cs);
}


HANDLE	g_hExitThreadEvent;

HANDLE	g_hAcceptThreadEvent;
HANDLE	g_hUpdateThreadEvent;


WCHAR *g_szDebug;

void NewSession(DWORD dwSessionID)
{
	st_SESSION *pSession = new st_SESSION;
	pSession->SessionID = dwSessionID;
	
	LockSession();
	g_SessionList.push_back(pSession);
	UnlockSession();


	// ABCD BUG : 1
	// 힙 깨짐
	st_PLAYER *pPlayer = new st_PLAYER;
	pPlayer->SessionID = dwSessionID;
	memset(pPlayer->Content, 0, sizeof(int) * 3); // int 3개

	LockPlayer();
	g_PlayerList.push_back(pPlayer);
	UnlockPlayer();

}

void DeleteSession(DWORD dwSessionID)
{
	LockSession();

	list<st_SESSION *>::iterator SessionIter = g_SessionList.begin();
	for ( ; SessionIter != g_SessionList.end(); SessionIter++ )
	{
		if ( dwSessionID == (*SessionIter)->SessionID )
		{
			delete *SessionIter;
			g_SessionList.erase(SessionIter);
			break;
		}
	}
	UnlockSession();

	LockPlayer();
	list<st_PLAYER *>::iterator PlayerIter = g_PlayerList.begin();
	for ( ; PlayerIter != g_PlayerList.end(); PlayerIter++ )
	{
		if ( dwSessionID == (*PlayerIter)->SessionID )
		{
			delete *PlayerIter;
			g_PlayerList.erase(PlayerIter);

			// ABCD BUG 2 - Unlock 안함

			UnlockPlayer();
			return;
		}
	}
	UnlockPlayer();
}


bool FindSessionList(DWORD dwSessionID)
{
	LockSession();

	list<st_SESSION *>::iterator SessionIter = g_SessionList.begin();
	for ( ; SessionIter != g_SessionList.end(); SessionIter++ )
	{
		if ( dwSessionID == (*SessionIter)->SessionID )
		{
			// ABCD BUG 3 : UnLock 하지 않음
			UnlockSession();
			return true;
		}
	}
	UnlockSession();

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// Accept Thread
/////////////////////////////////////////////////////////////////////////////////////
unsigned int WINAPI AcceptThread(LPVOID lpParam)
{
	HANDLE hEvent[2] = {g_hExitThreadEvent, g_hAcceptThreadEvent};
	DWORD dwError;
	DWORD dwSessionID;
	bool bLoop = true;
	bool bFindIt = false;

	wprintf(L"Accept Thread Start\n");
	srand(GetCurrentThreadId());

	while (bLoop)
	{
		dwError = WaitForMultipleObjects(2, hEvent, FALSE, INFINITE);

		if ( dwError == WAIT_FAILED )
 		{
			wprintf(L"Accept Thread Event Error\n");
			wprintf(L"Accept Thread Event Error\n");
			wprintf(L"Accept Thread Event Error\n");
			wprintf(L"Accept Thread Event Error\n");
			break;
		}

		if ( dwError == WAIT_OBJECT_0 )
			break;


		//----------------------------------------------------------
		//----------------------------------------------------------
		// 정상 로직처리 
		//----------------------------------------------------------
		//----------------------------------------------------------

		//----------------------------------------------------------
		// 접속요청 처리
		//----------------------------------------------------------
		while ( !g_AcceptPacketList.empty() )
		{
			dwSessionID = *g_AcceptPacketList.begin();
			LockAccept();
			g_AcceptPacketList.pop_front();
			UnlockAccept();

			//----------------------------------------------------------
			// SessionList 에 이미 존재하는 SessionID 인지 확인.  없는 경우만 등록.
			//----------------------------------------------------------
			if ( !FindSessionList(dwSessionID) )
			{
				NewSession(dwSessionID);
				wprintf(L"AcceptThread - New Session[%d]\n", dwSessionID);
			}
		}

		//----------------------------------------------------------
		// 접속해제 처리
		//----------------------------------------------------------
		while ( !g_DisconnectPacketList.empty() )
		{
			LockDisconnect();
			dwSessionID = *g_DisconnectPacketList.begin();
			g_DisconnectPacketList.pop_front();
			UnlockDisconnect();

			//----------------------------------------------------------
			// SessionList 에 존재하는 SessionID 인지 확인.  있는 경우만 삭제
			//----------------------------------------------------------
			if ( FindSessionList(dwSessionID) )
			{
				DeleteSession(dwSessionID);
				wprintf(L"AcceptThread - Delete Session[%d]\n", dwSessionID);
			}
		}
	}

	wprintf(L"Accept Thread Exit\n");
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// IO Thread
/////////////////////////////////////////////////////////////////////////////////////
unsigned int WINAPI IOThread(LPVOID lpParam)
{
	DWORD dwError;
	bool bLoop = true;
	int iRand;
	DWORD dwSessionID;

	srand(GetCurrentThreadId());
	wprintf(L"IO Thread Start\n");

	while ( bLoop )
	{
		dwError = WaitForSingleObject(g_hExitThreadEvent, 10);
		if ( dwError != WAIT_TIMEOUT )
			break;

		// ABCD BUG 4 == WAIT의 성공 여부 판단하지 않음
		if ( dwError == WAIT_OBJECT_0 )
			break;

		//----------------------------------------------------------
		// 정상 로직처리 
		//----------------------------------------------------------
		iRand = rand() % 3;
		dwSessionID = rand() % 5000;
		

		switch ( iRand )
		{
		case 0:			// Accept 추가
			wsprintf(g_szDebug, L"# IOThread AcceptPacket Insert [%d] \n", dwSessionID);
			LockAccept();
			g_AcceptPacketList.push_back(dwSessionID);
			UnlockAccept();
			SetEvent(g_hAcceptThreadEvent);
			break;

		case 1:			// Disconnect 추가
			wsprintf(g_szDebug, L"# IOThread DisconnetPacket Insert [%d] \n", dwSessionID); // 문자열 사이즈 41
			LockDisconnect();
			g_DisconnectPacketList.push_back(dwSessionID);
			UnlockDisconnect();
			SetEvent(g_hAcceptThreadEvent);
			break;

		case 2:			// Action 추가
			wsprintf(g_szDebug, L"# IOThread ActionPacket Insert [%d] \n", dwSessionID);
			LockAction();
			g_ActionPacketList.push_back(dwSessionID);
			UnlockAction();
			SetEvent(g_hUpdateThreadEvent);
			break;
		}
		OutputDebugString(g_szDebug);
	}


	wprintf(L"IO Thread Exit\n");
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// Update Thread
/////////////////////////////////////////////////////////////////////////////////////
unsigned int WINAPI UpdateThread(LPVOID lpParam)
{
	HANDLE hEvent[2] = {g_hExitThreadEvent, g_hUpdateThreadEvent};
	DWORD dwError;
	DWORD dwSessionID;
	st_PLAYER *pPlayer;
	bool bLoop = true;

	srand(GetCurrentThreadId());

	wprintf(L"Update Thread Start\n");

	while ( bLoop )
	{
		dwError = WaitForMultipleObjects(2, hEvent, FALSE, INFINITE);

		if ( dwError == WAIT_FAILED )
 		{
			wprintf(L"Update Thread Event Error\n");
			wprintf(L"Update Thread Event Error\n");
			wprintf(L"Update Thread Event Error\n");
			wprintf(L"Update Thread Event Error\n");
			break;
		}

		if ( dwError == WAIT_OBJECT_0 )
			break;


		//----------------------------------------------------------
		// 정상 로직처리 
		//----------------------------------------------------------
		//----------------------------------------------------------
		// 플레이어 액션 처리
		//----------------------------------------------------------
		while ( !g_ActionPacketList.empty() )
		{
			LockAction();
			dwSessionID = *g_ActionPacketList.begin();
			g_ActionPacketList.pop_front();

			//----------------------------------------------------------
			// PlayerList 에 이미 존재하는 SessionID 인지 확인. 있는 경우만 해당 플레이어 찾아서 + 1
			//----------------------------------------------------------
			LockPlayer();
			list<st_PLAYER *>::iterator PlayerIter = g_PlayerList.begin();
			for ( ; PlayerIter != g_PlayerList.end(); PlayerIter++ )
			{
				pPlayer = *PlayerIter;
				if ( dwSessionID == pPlayer->SessionID )
				{
					// 컨텐츠 업데이트 - Content 배열마다 + 1 후 출력
					// ABCD BUG 5 : 인덱스 범위 에러
					for ( int iCnt = 0; iCnt < 3; iCnt++ )
					{
						pPlayer->Content[iCnt]++;
					}

					wprintf(L"UpdateThread - Session[%d] Content[%d] \n", dwSessionID, pPlayer->Content[0]);
					break;
				}
			}
			UnlockPlayer();
			UnlockAction();

		}

	}

	wprintf(L"Update Thread Exit\n");
	return 0;
}




/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// Initial
/////////////////////////////////////////////////////////////////////////////////////
void Initial()
{
	// ABCD BUG 6 : 사이즈를 넘어서 Write 하는 부분이 있음 40 -> 50
	g_szDebug = new WCHAR[50];

	//------------------------------------------------
	// 각각의 스레드를 깨울 이벤트
	//------------------------------------------------
	g_hAcceptThreadEvent	= CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hUpdateThreadEvent	= CreateEvent(NULL, FALSE, FALSE, NULL);

	//------------------------------------------------
	// 모든 스레드를 종료 시킬 이벤트
	//------------------------------------------------

	// ABCD BUG 7 : 메뉴얼 리셋
	g_hExitThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	InitializeCriticalSection(&g_Accept_cs);
	InitializeCriticalSection(&g_Action_cs);
	InitializeCriticalSection(&g_Disconnect_cs);
	InitializeCriticalSection(&g_Player_cs);
	InitializeCriticalSection(&g_Session_cs);

}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// Release
/////////////////////////////////////////////////////////////////////////////////////
void Release()
{
	g_AcceptPacketList.clear();
	g_ActionPacketList.clear();
	g_DisconnectPacketList.clear();


	// ABCD BUG : 8
	// iterator 갱신하지 않음
	list<st_SESSION *>::iterator SessionIter = g_SessionList.begin();
	while ( SessionIter != g_SessionList.end() )
	{
		delete *SessionIter;
		SessionIter = g_SessionList.erase(SessionIter);
	}

	list<st_PLAYER *>::iterator PlayerIter = g_PlayerList.begin();
	while ( PlayerIter != g_PlayerList.end() )
	{
		delete *PlayerIter;
		PlayerIter = g_PlayerList.erase(PlayerIter);
	}

	delete[] g_szDebug;

	DeleteCriticalSection(&g_Accept_cs);
	DeleteCriticalSection(&g_Action_cs);
	DeleteCriticalSection(&g_Disconnect_cs);
	DeleteCriticalSection(&g_Player_cs);
	DeleteCriticalSection(&g_Session_cs);


}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hIOThread;			// 접속요청, 끊기요청, 액션요청 발생.  (IO 시뮬레이션)
	HANDLE hAcceptThread;		// 접속요청, 끊기에 대한 처리	
	HANDLE hUpdateThread;		// 액션요청 처리.

	DWORD dwThreadID;

	Initial();



	//------------------------------------------------
	// 스레드 생성.
	//------------------------------------------------
	hAcceptThread	= (HANDLE)_beginthreadex(NULL, 0, AcceptThread, (LPVOID)0, 0, (unsigned int *)&dwThreadID);
	if (hAcceptThread == 0)
		return 1;
	hIOThread		= (HANDLE)_beginthreadex(NULL, 0, IOThread,	(LPVOID)0, 0, (unsigned int *)&dwThreadID);
	if (hIOThread == 0)
		return 1;
	hUpdateThread	= (HANDLE)_beginthreadex(NULL, 0, UpdateThread, (LPVOID)0, 0, (unsigned int *)&dwThreadID);
	if (hUpdateThread == 0)
		return 1;


	WCHAR ControlKey;

	//------------------------------------------------
	// 종료 컨트롤...
	//------------------------------------------------
	while ( 1 )
	{	
		ControlKey = _getwch();
		if ( ControlKey == L'q' || ControlKey == L'Q' )
		{
			//------------------------------------------------
			// 종료처리
			//------------------------------------------------
			SetEvent(g_hExitThreadEvent);
			break;
		}
	}


	//------------------------------------------------
	// 스레드 종료 대기
	//------------------------------------------------

	HANDLE hThread[3] = {hAcceptThread, hIOThread, hUpdateThread};
	// ABCD BUG 9 : COUNT 오류 - 리턴값 확인
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);


	Release();


	//------------------------------------------------
	// 디버깅용 코드  스레드 정상종료 확인.
	//------------------------------------------------
	DWORD ExitCode;

	wprintf(L"\n\n--- THREAD CHECK LOG -----------------------------\n\n");

	GetExitCodeThread(hAcceptThread, &ExitCode);
	if ( ExitCode != 0 )
		wprintf(L"error - Accept Thread not exit\n");

	GetExitCodeThread(hIOThread, &ExitCode);
	if ( ExitCode != 0 )
		wprintf(L"error - IO Thread not exit\n");

	GetExitCodeThread(hUpdateThread, &ExitCode);
	if ( ExitCode != 0 )
		wprintf(L"error - Update Thread not exit\n");

	return 0;
}

