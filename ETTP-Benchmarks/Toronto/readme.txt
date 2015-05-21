README.TXT
------------------

The file contains a list of test data used in the paper called

   "Examination Timetabling: Algorithmic Strategies and Applications". J. Op.
   Res. Soc. 47, 373 - 383. 

Each set of test data come in two files, one file contains the list of 
courses and the other a list of student course selections. All course 
files end with the extension .crs and all student files end with a .stu
extension. All the data files are located under the directory /paper/testprob.
Refer to the file UPDATE.TXT for the latest changes made to the files on the 
ftp site.


The directory contains the following files:

Name          Description
----          -------------

car-s-91.crs  Carleton University (course data)
car-s-91.stu  Carleton University (student.data)
car-f-92.crs  Carleton University (course data)
car-f-92.stu  Carleton University (student data)
ear-f-83.crs  Earl Haig Collegiate (course data)
ear-f-83.stu  Earl Haig Collegiate
hec-s-92.crs  Ecole des Hautes Etudes Commerciales (course data)
hec-s-92.stu  Ecloe des Hautes Etudes Commercilaes (student data)
kfu-s-93.crs  King Fahd University (course data)
kfu-s-93.stu  King Fahd University (student data)
lse-f-91.crs  London School of Economics (course data)
lse-f-91.stu  London School of Economics (student data)
pur-s-93.crs  Purdue University (course data)
pur-s-93.stu  Purdue University (student data)
rye-f-92.crs  Ryerson University (course data)
rye-f-92-stu  Ryerson University (student data)
sta-f-83.crs  St. Andrews High school (course data)
sta-f-83.stu  St. Andrews High school (student data)
tre-s-92.crs  Trent University (course data)
tre-s-92.stu  Trent University (student data)
uta-s-92.crs  University of Toronto, Arts & Science (course data)
uta-s-92.stu  University of Toronto, Arts & Science (student data)
ute-s-92.crs  University of Toronto, Engineering (course data)
ute-s-92.stu  University of Toronto, Engineering (student data)
yor-s-92.crs  York Mills Collegiate (course data)
yor-s-92.stu  York Mills Collegiate (student data)

all_file.tar.Z  a compresses file of all the data
all_file.tar.gz a compress version of all the data in gunzip format
all_file.zip    a compress version of all the data in zip format

readme.txt  this file


File Format Description
----------------------------

The course data file is in the following format:

course      enrolment
-------     ------------
0001        10
0002        9
.
.
.

The course file is sorted in ascending order.


The student data file is in the following format:

course  course  course ... ... course
------  ------  ------         ------
0001    0005    0010           0100
.
.
.

The student course selections are sort in ascending order.

