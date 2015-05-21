// Timetabling.cpp: implementation of the Timetabling class.
//////////////////////////////////////////////////////////////////////

//#include "StdAfx.h"
#include "Timetabling.h"
//#include "string.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <cstdio>


using namespace std;
using namespace boost;


Timetabling::Timetabling()
{
	for(int i = 0; i < max_events; i++)
		constraint[i] = new EventList;
	events = new int[max_events];

	event = new eventelement[max_events];
    for(int i = 0; i < max_events; i++)
	{
		event[i].ei = -1;		//slot event i scheduled in
		event[i].enrollment = 0;
	}

	numStudents = 0;
	numEvents = 0;
	numSlots = 0;
}

Timetabling::~Timetabling()
{
	for(int i = 0; i < max_events; i++)
		delete constraint[i];

	delete []event;
}

// delta cost of scheduling ei into pi
long double Timetabling::canSchedule(int pi, int ei)
{
	int j, con;
	long double v;
	EventList *c;
	double cost = 0.0;

	c = constraint[ei];
	con = c->count();
	for(j = 0; j < con; j++)
	{
		v = eventInPeriodNumber(pi, c->getEvent(j));
		if(v == -1) continue;
		if(v == conflicted)
			return conflicted;
		cost += (double)(c->getEnroll(j) * pow(2, 4 - v)) / (double)numStudents;
	}
	return cost;
}

// individual cost of event n2 in period pi
long double Timetabling::eventInPeriodNumber(int pi, int n2)
{
	if(event[n2].ei == pi)
		return conflicted;

	for(int i = 0; i < 5; i++)
	{
		if(((pi > i) && (event[n2].ei == pi - i - 1))
			|| ((pi < numSlots - i - 1) && (event[n2].ei == pi + i + 1)))
			return i;
	}
	return -1;
}

long double Timetabling::calculate()
{
	double sum = 0.0;
	int i;	

	for (i = 1; i < numEvents + 1; i++)
	{
		if(event[i].ei == -1)
			return conflicted;
		sum += canSchedule(event[i].ei, i);
	}
	sum = sum / 2;

	printf("penalty for the input timetable: %f\n", sum);
	return sum;
}

void Timetabling::readTimetable(char *name)
{
	int e, slot;
	FILE *tt;
	char nameT[32];

	strcpy(nameT, name);
	strcat(nameT, ".sol");
	if ((tt = fopen(nameT, "r")) == NULL)
	{
		printf("%s %s", "Error.  Cannot open file: ", nameT);
		exit(10);
	}

	for(int j = 0; j < numEvents; j++)
	{
		fscanf(tt, "%d %d", &e, &slot);

//        printf("Read: %d %d\n", e, slot);

		event[e].ei = slot;
	}
	fclose(tt);
}

void Timetabling::evaluate(char *name)
{
	readSlot(name);
	readEvents(name);
	readStudents(name);
	processData();

	readTimetable(name);

	calculate();
}

void Timetabling::readSlot(char *name)
{
	FILE *fp;
	char nameS[32];

	strcpy(nameS, name);
	strcat(nameS, ".RES");
	if ((fp = fopen(nameS, "r")) == NULL)
	{
		printf("%s %s", "Error.  Cannot open file: ", nameS);
		exit(10);
	}

	fscanf(fp,"%d", &numSlots);

//    printf("Read numslots = %d\n", numSlots);

	fclose(fp);
}

void Timetabling::readEvents(char *name)
{
	int i, j, e, enroll;
	FILE *fp;
	char nameE[32];

	strcpy(nameE, name);
	strcat(nameE, ".exm");
	if ((fp = fopen(nameE, "r")) == NULL)
	{
		printf("%s %s", "Error.  Cannot open file: ", nameE);
		exit(1);
	}

	for(i = 0; ; i++)
	{
		if((j = fscanf(fp, "%d %d", &e, &enroll)) == EOF)
		{
			fclose(fp);
			break;
		}
//        printf("Read Event: %d %d\n", e, enroll);

		events[e] = enroll;
	}
	numEvents = i;
}


void Timetabling::readStudents(char *name)
{
	FILE *fp;
	int i, e;
	char id[32], oldId[32], nameS[32];
	studInfo *ds;

	strcpy(nameS, name);
	strcat(nameS, ".stu");
//	if ((fp = fopen(nameS, "r")) == NULL)
//	{
//		printf("Error.  Cannot open file: ", nameS);
//		exit(1);
//	}

    ifstream file(nameS);

    string line;

//    for(i = 0; ; i++)
    int k;
    for(i = 0, k = 1; ; ++k)
    {
        getline(file, line);

        tokenizer<> lineTok(line);
        sprintf(id, "%d", k); // Student id
//        cout << id << " - ";

        // Read exam enrolment data
        for (tokenizer<>::iterator beg = lineTok.begin(); beg != lineTok.end(); ++beg, ++i) {
            e = atoi((*beg).c_str());
            cout << e << " ";
            if ((i == 0) || (strcmp(id, oldId) != 0))
            {
//                cout << "i = " << i << ", k = " << k << endl;
//                cout << "Create new student entry " << id << endl;

//                cin.get();

                ds = new studInfo;
                ds->event.clear();
                ds->event.insert(e);
                strcpy(ds->stud_ID, id);
                ds->next = NULL;
                student.insert(ds);
                strcpy(oldId, id);
                delete ds;
                continue;
            }
            else
            {
                if((i >= 1) && (strcmp(id, oldId) == 0))
                    student.insert(e);
            }
        }
        cout << endl;
        if (!file.good())
            break;


//		if(fscanf(fp, "%s %d", &id, &e) == EOF)
//			break;
//		if((i == 0) || (strcmp(id, oldId) != 0))
//		{
//			ds = new studInfo;
//			ds->event.clear();
//			ds->event.insert(e);
//			strcpy(ds->stud_ID, id);
//			ds->next = NULL;
//			student.insert(ds);
//			strcpy(oldId, id);
//			delete ds;
//			continue;
//		}
//		else
//		{
//			if((i >= 1) && (strcmp(id, oldId) == 0))
//				student.insert(e);
//		}

	}
	numStudents = student.count();

    cout << "numStudents = " << numStudents << endl;
//    cout << "Press any key = " << endl;

//    cin.get();

//	fclose(fp);
    file.close();
}


void Timetabling::processData()
{
	int i, j, k, con, e1, e2;
	studInfo *ds;

	for(i = 0; i < student.count(); i++)
	{
		ds = student.get(i);
		con = ds->event.count();
		for(j = 0; j < con; j++)
		{
			for(k = 0; k < con; k++)
			{
				if(k != j)
				{
					e1 = ds->event.getEvent(j);
					e2 = ds->event.getEvent(k);
					if(constraint[e1]->eventIn(e2))
						continue;
					else
						constraint[e1]->insert(e2);
				}
			}
		}
	}
}
