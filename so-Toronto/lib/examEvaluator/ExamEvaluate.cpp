
//#include "stdafx.h"
#include "Timetabling.h"

#include <iostream>
#include <string>

using namespace std;


void runExamEvaluator(string const& _torontoDatasetPath, string const& _outputDir, string const& _torontoDatasetName) {
    Timetabling ttp(_torontoDatasetPath, _outputDir, _torontoDatasetName);

    std::cout << "R. Qu's Exam Evaluator" << std::endl;

    ttp.evaluate((char*)_torontoDatasetName.c_str());
}

/*
int main(int argc, char* argv[])
{
	Timetabling ttp;

    std::cout << "R. Qu's Exam Evaluator" << std::endl;

//	if (argc != 2)
//        printf("Please provide the file name of the solution timetable (.sln):\n");
//	else
//        ttp.evaluate(argv[1]);

    if (argc != 2)
        std::cout << "Please provide the path and dataset name to evaluate. There must exist a solution file (.sln): " << std::endl;
    else
        ttp.evaluate(argv[1]);


//        ttp.evaluate("./../Toronto/car-f-92");
//        ttp.evaluate("./../Toronto/hec-s-92");
//    ttp.evaluate("./../Toronto/pur-s-93");
    //    ttp.evaluate("./../Toronto/sta-f-83");
    //    ttp.evaluate("./../Toronto/uta-s-92");
    //    ttp.evaluate("./../Toronto/yor-f-83");



	//ttp.evaluate("EdHEC92");
	//ttp.evaluate("Pur93");
    //ttp.evaluate("St.Andrews83");
	//ttp.evaluate("sta-m-83-gxk");
	//ttp.evaluate("YorkMills83");
	//ttp.evaluate("TorontoE92");
	//ttp.evaluate("EarlHaig83");
	//ttp.evaluate("Trent92");
	//ttp.evaluate("LSE91");
	//ttp.evaluate("KingFahd93");
	//ttp.evaluate("Carleton92");
	//ttp.evaluate("TorontoAS92");
	//ttp.evaluate("Carleton91");
	//ttp.evaluate("yor-f-83-3-IIc");
	//ttp.evaluate("ear-f-83-3-IIc");

    return 0;
}
*/
