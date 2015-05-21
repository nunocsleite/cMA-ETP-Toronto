#
# project
#
project(SOlib)

# Include "/lib" directory
include_directories(${PROJECT_SOURCE_DIR})
# Include ParadisEO directories
include_directories(${PROJECT_SOURCE_DIR}/../../ext/include/ParadisEO-2.0/eo/src/)
include_directories(${PROJECT_SOURCE_DIR}/../../ext/include/ParadisEO-2.0/mo/src/)
include_directories(${PROJECT_SOURCE_DIR}/../../ext/include/ParadisEO-2.0/moeo/src/)




add_subdirectory(algorithms)
add_subdirectory(chromosome)
add_subdirectory(containers)
add_subdirectory(data)
add_subdirectory(eval)
add_subdirectory(graphColouring)
add_subdirectory(init)
add_subdirectory(kempeChain)
add_subdirectory(neighbourhood)
add_subdirectory(toronto)
add_subdirectory(utils)


#
# Set public header list (add your headers and source files here))
#
set(${PROJECT_NAME}_headers
        # algorithms/eo
        algorithms/eo/Crossover.h
        algorithms/eo/eoAlgoPointer.h
        algorithms/eo/eoCellularEA.h
        algorithms/eo/eoCellularEAMatrix.h
        algorithms/eo/eoCellularEARing.h
        algorithms/eo/eoDeterministicTournamentSelectorPointer.h
        algorithms/eo/eoGenerationContinue.h   # Not used
        algorithms/eo/eoGenerationContinuePopVector.h
        algorithms/eo/eoSelectBestOne.h        # Not used
        algorithms/eo/Mutation.h
        # algorithms/mo
        algorithms/mo/moSimpleCoolingSchedule.h
        algorithms/mo/moTA.h
        algorithms/mo/moTAexplorer.h
        # algorithms/mo/statistics
        algorithms/mo/statistics/moTAWithStatistics.h
        algorithms/mo/statistics/moTAexplorerWithStatistics.h
        # chromosome
        chromosome/eoChromosome.h
        # containers
        containers/ColumnMatrix.h
        containers/IntMatrix.h
        containers/Matrix.h
        containers/TimetableContainer.h
        containers/TimetableContainerMatrix.h
        containers/VertexPriorityQueue.h
        # data
        data/Data.h
        data/Exam.h
        data/Period.h
        data/TimetableProblemData.hpp
        # eval
        eval/eoETTPEval.h
        eval/eoNumberEvalsCounter.h
        # eval/statistics
        eval/statistics/eoETTPEvalWithStatistics.h
        # examEvaluator
        examEvaluator/EventList.h
        examEvaluator/StdAfx.h
        examEvaluator/StudentList.h
        examEvaluator/Timetabling.h
        # graphColouring
        graphColouring/GraphColouringHeuristics.h
        # init
        init/ETTPInit.h
        # kempeChain
        kempeChain/ETTPKempeChain.h
        kempeChain/ETTPKempeChainHeuristic.h
        # kempeChain/statistics
        kempeChain/statistics/ETTPKempeChainHeuristicWithStatistics.h
        # neighbourhood
        neighbourhood/ETTPneighbor.h
        neighbourhood/ETTPneighborEval.h
        neighbourhood/ETTPneighborhood.h
        neighbourhood/ETTPNeighborhoodExplorer.h
        # neighbourhood/statistics
        neighbourhood/statistics/ETTPneighborEvalNumEvalsCounter.h
        neighbourhood/statistics/ETTPneighborEvalWithStatistics.h
        neighbourhood/statistics/ETTPneighborhoodWithStatistics.h
        neighbourhood/statistics/ETTPneighborWithStatistics.h
        # statistics
        statistics/ExamInfo.h
        statistics/ExamMoveStatistics.h
        statistics/MovedExam.h
        # testset
        testset/TestSet.h
        testset/TestSetDescription.h
        # toronto
        toronto/TorontoTestSet.h
        # utils
        utils/Common.h
        utils/CurrentDateTime.h
)


set(${PROJECT_NAME}_sources
        # algorithms/eo
        algorithms/eo/Crossover.cpp
        algorithms/eo/Mutation.cpp
        # chromosome
        chromosome/eoChromosome.cpp
        # containers
        containers/TimetableContainerMatrix.cpp
        containers/VertexPriorityQueue.cpp
        # data
        data/TimetableProblemData.cpp
        # examEvaluator
        examEvaluator/EventList.cpp
        examEvaluator/ExamEvaluate.cpp
        examEvaluator/StdAfx.cpp
        examEvaluator/StudentList.cpp
        examEvaluator/Timetabling.cpp
        # graphColouring
        # statistics
        statistics/ExamInfo.cpp
        statistics/ExamMoveStatistics.cpp
        # testset
        testset/TestSetDescription.cpp
        # toronto
        toronto/MainAppTorontoDatasets.cpp
        toronto/TorontoTestSet.cpp
        # utils
        utils/CurrentDateTime.cpp
        utils/Utils.cpp
        utils/Common.cpp
        # lib
        MainApp.cpp
)



add_library(SOlib SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})


# Include ParadisEO, Boost Regex, Armadillo, ncurses5-dev libs
#target_link_libraries(${PROJECT_NAME} boost_regex eo es moeo cma eoutils ga armadillo ncurses)
target_link_libraries(${PROJECT_NAME} boost_regex eo es moeo cma eoutils ga armadillo)

