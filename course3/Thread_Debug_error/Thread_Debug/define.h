#ifndef __DEFINE__
#define __DEFINE__


struct st_SESSION
{
	int SessionID;
};


struct st_PLAYER
{
	int SessionID;
	int Content[3];
};


// THREAD °³¼ö Àß¸øµÊ
#define dfTHREAD_NUM	4
#endif