// EventList.cpp: implementation of the EventList class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "EventList.h"


#define NULL 0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EventList::EventList()
{
	Count = 0;
	Top = NULL;
	Last = NULL;
}

EventList::~EventList()
{
	clear();
}

// insert an element in the list
void EventList::insert(int ei, int enroll)
{
	if( !Top )
	{
		Top = new eventelement;
		Top->ei = ei;
		Top->enrollment = enroll;
		Top->next = NULL;
		Last = Top;
	}else
	{
		Last->next = new eventelement;
		Last = Last->next;
		Last->ei = ei;
		Last->enrollment = enroll;
		Last->next = NULL;
	}
	Count++;
}

//get the counter of element i
int EventList::getEnroll(int i)
{
	eventelement *ptr;
	int x;

	ptr = Top;
	//#############for(x = 0; x < i - 1; x++)
	for(x = 0; x < i; x++)
		ptr = ptr->next;

	return ptr->enrollment;
}

//get the event of elelemt i
int EventList::getEvent(int i)
{
	eventelement *ptr;
	int x;

	ptr = Top;
	for(x = 0; x < i; x++)
		ptr = ptr->next;

	return ptr->ei;
}

void EventList::clear()
{
	while(Top)
	{
		Last = Top->next;
		delete Top;
		Top = Last;
	}
	Top = NULL;
	Last = NULL;
	Count = 0;
}

int EventList::count()
{
	return Count;
}

//if event ei int the list, increase the counter
int EventList::eventIn(int ei)
{
	eventelement *ptr;
	int x;

	ptr = Top;
	for(x = 0; x < Count; x++)
	{
		if(ptr->ei == ei)
		{
			ptr->enrollment++;
			return 1;
		}
		ptr = ptr->next;
	}
	return 0;
}
