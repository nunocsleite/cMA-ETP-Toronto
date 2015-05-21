// StudentList.cpp: implementation of the StudentList class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "StudentList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StudentList::StudentList()
{
	Top = NULL;
	Last = NULL;
	Count = 0;
}

StudentList::~StudentList()
{
	clear();
}

void StudentList::clear()
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

//get the ith element
studInfo* StudentList::get(int i)
{
	studInfo *ptr;
	int x;

	ptr = Top;
	for(x = 0; x < i; x++)
		ptr = ptr->next;
	return ptr;
}

void StudentList::insert(studInfo *si)
{
	int con = si->event.count();
	int i;

	if( !Top )
	{
		Top = new studInfo;
		for(i = con - 1; i >=0; i--)
			Top->event.insert(si->event.getEvent(i));
		strcpy(Top->stud_ID, si->stud_ID);
		Top->next = NULL;
		Last = Top;
	}else
	{
		Last->next = new studInfo;
		Last = Last->next;
		for(i = 0; i < con; i++)
			Last->event.insert(si->event.getEvent(i));
		strcpy(Last->stud_ID, si->stud_ID);
		Last->next = NULL;
	}
	Count++;
}

int StudentList::count()
{
	return Count;
}

void StudentList::insert(int ei)
{
	Last->event.insert(ei);
}
