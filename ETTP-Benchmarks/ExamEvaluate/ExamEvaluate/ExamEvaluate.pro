#-------------------------------------------------
#
# Project created by QtCreator 2013-04-22T13:42:17
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ExamEvaluate
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../MOEA/include/boost_1_52_0/

SOURCES += \
    ../Timetabling.cpp \
    ../StudentList.cpp \
    ../ExamEvaluate.cpp \
    ../EventList.cpp

HEADERS += \
    ../Timetabling.h \
    ../StudentList.h \
    ../EventList.h
