// StudentList.h: interface for the StudentList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STUDENTLIST_H__F16F262B_4441_4395_8211_817444DA9E06__INCLUDED_)
#define AFX_STUDENTLIST_H__F16F262B_4441_4395_8211_817444DA9E06__INCLUDED_

#include "StdAfx.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EventList.h"
#include "string.h"

struct studInfo
{
	char stud_ID[32];
	EventList event;
	studInfo *next;
};

class StudentList  
{
public:

	int count();
	studInfo* get(int i);

	void clear();
	void insert(int ei);
	void insert(studInfo *si);
	
	StudentList();
	virtual ~StudentList();

private:
	int Count;
	studInfo* Last;
	studInfo* Top;
};

#endif // !defined(AFX_STUDENTLIST_H__F16F262B_4441_4395_8211_817444DA9E06__INCLUDED_)
