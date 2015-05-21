#ifndef TORONTOTESTSET_H
#define TORONTOTESTSET_H

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <boost/tokenizer.hpp>
#include "graphColouring/GraphColouringHeuristics.h"
#include "containers/Matrix.h"
#include <boost/unordered_map.hpp>
#include "testset/TestSet.h"
#include <boost/unordered_map.hpp>



// For debugging purposes
//#define TORONTO_TESTSET_DEBUG



/////////////////////////////
// Toronto Dataset
//
/////////////////////////////
class TorontoTestSet : public TestSet {

public:
    /**
     * @brief TorontoTestSet Constructor
     * @param _testSetName
     * @param _description
     * @param _rootDir
     */
    TorontoTestSet(std::string _testSetName, std::string _description, std::string _rootDir)
        : TestSet(_testSetName, _description, _rootDir,
                  boost::shared_ptr<TimetableProblemData>(new TimetableProblemData())) {
#ifdef TORONTOTESTSET_DEBUG
        std::cout << "TorontoTestSet ctor" << std::endl;
#endif
        // From Carter's update.txt file
        //
        // 19/06/1996
        // ----------
        //
        // Updated data files and a revised Table 1.
        //
        //
        //   Table 1: Characteristics of Real Problems

        // Institutions    No. of  No. of    No. of Student  Problem
        //                 Exams   Students  Exams           Density
        //
        // car-f-92        543     18419     55522           13.8%
        // car-s-91        682     16925     56877           12.8%
        // ear-f-83        190     1125      8109            26.7%
        // hec-s-92        81      2823      10632           42%
        // kfu-s-93        461     5349      25113           5.6%
        // lse-f-91        381     2726      10918           6.3%
        // pur-s-93        2419    30032     120681          2.9%  /// <- ERROR # STUDENTS =  30029
        /// FILE IS NAMED rye-s-93 NOT rye-f-92
        // rye-f-92        486     11483     45051           7.5%
        // sta-f-83        139     611       5751            14.4%
        // tre-s-92        261     4360      14901           5.8%  /// <- ERROR CONFLICT MATRIX DENSITY
        // uta-s-92        622     21267     58979           12.6% /// <- ERROR # STUDENTS = 21266
        // ute-s-92        184     2750      11793           8.5%  /// <- ERROR # STUDENTS = 2749
        // yor-f-83        181     941       6034            28.9%
        ///
        /// READ VALUES WITH ORIGINAL # STUDENTS
        ///
        /// car-f-92	 543	 18419	 55522	 0.137986	 32
        /// car-s-91	 682	 16925	 56877	 0.128386	 35
        /// ear-f-83	 190	 1125	 8109	 0.266945	 24
        /// hec-s-92	 81      2823	 10632	 0.420679	 18
        /// kfu-s-93	 461	 5349	 25113	 0.0555786	 20
        /// lse-f-91	 381	 2726	 10918	 0.0625915	 18
        /// pur-s-93	 2419	 30032	 120681	 0.0294953	 42  <- CARTER'S ERROR # STUDENTS =  30029, Pillay has 43 periods
        /// rye-s-93	 486	 11483	 45051	 0.0752789	 23  <- R.Qu's has # exams = 482 and CD = 0.07
        /// sta-f-83	 139	 611	 5751	 0.143989	 13
        /// tre-s-92	 261	 4360	 14901	 0.180696	 23  <- CARTER'S ERROR CONFLICT MATRIX DENSITY
        /// uta-s-92	 622	 21267	 58979	 0.125557	 35  <- CARTER'S ERROR # STUDENTS = 21266
        /// ute-s-92	 184	 2750	 11793	 0.084937	 10  <- CARTER'S ERROR # STUDENTS = 2749, R.Qu's has # students = 2750
        /// yor-f-83	 181	 941	 6034	 0.288889	 21

        // Initialise datasetNumStudentsMap
        /// Original # students
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("car-f-92", 18419));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("car-s-91", 16925));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("ear-f-83", 1125));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("hec-s-92", 2823));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("kfu-s-93", 5349));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("lse-f-91", 2726));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("pur-s-93", 30032));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("rye-s-93", 11483));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("sta-f-83", 611));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("tre-s-92", 4360));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("uta-s-92", 21267));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("ute-s-92", 2750));
//        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("yor-f-83", 941));

        /// With corrected # students
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("car-f-92", 18419));
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("car-s-91", 16925));
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("ear-f-83", 1125));
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("hec-s-92", 2823));
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("kfu-s-93", 5349));
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("lse-f-91", 2726));
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("pur-s-93", 30029)); // Not 30032
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("rye-s-93", 11483));
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("sta-f-83", 611));
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("tre-s-92", 4360));
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("uta-s-92", 21266)); // Not 21267
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("ute-s-92", 2749));  // Not 2750
        datasetNumStudentsMap.insert(std::make_pair<std::string, int>("yor-f-83", 941));



#ifdef TORONTO_TESTSET_DEBUG
//        for (auto const& x : datasetNumStudentsMap)
//            std::cout << x.first << ": " << x.second << std::endl;
//        std::cout << std::endl;
#endif


        // After creating the instance, the caller should invoke the load method
    }


public:
    // Overriden method
    virtual void load() override;


protected:
    /**
     * Protected members
     */

    virtual void loadConflictMatrix();
    virtual void loadCourseCounts();
    virtual void computeStudentCount();
    virtual void loadCourseNames();
    /**
     * @brief datasetNumStudentsMap
     */
    boost::unordered_map<std::string,int> datasetNumStudentsMap;
};



#endif // TORONTOTESTSET_H
