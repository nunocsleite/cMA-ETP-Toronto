// EventList.h: interface for the EventList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTLIST_H__1C2E5D44_9A9C_436C_8E0C_CDD813F29FD9__INCLUDED_)
#define AFX_EVENTLIST_H__1C2E5D44_9A9C_436C_8E0C_CDD813F29FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct eventelement
{
	int ei;
	int enrollment;
	eventelement *next;
};

class EventList  
{
public:
	EventList();
	virtual ~EventList();
	
	int count();

	int getEvent(int i);
	int getEnroll(int i);
	int eventIn(int ei);

	void clear();
	void insert(int ei, int enroll = 1);

private:

	int Count;
	eventelement* Top;
	eventelement* Last;
};

#endif // !defined(AFX_EVENTLIST_H__1C2E5D44_9A9C_436C_8E0C_CDD813F29FD9__INCLUDED_)
