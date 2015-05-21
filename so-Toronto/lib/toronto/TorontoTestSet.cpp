
#include "TorontoTestSet.h"




///////////////////////////////////////////////////////////////////////////////
//  Helper function reading a file into a string
///////////////////////////////////////////////////////////////////////////////
std::string read_from_file(char const* infile)
{
    std::ifstream instream(infile);
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << infile << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);      // No white space skipping!
    return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
                       std::istreambuf_iterator<char>());
}




////////////////////////////////////////////////////////////
// Toronto Dataset Methods
//
////////////////////////////////////////////////////////////

/**
 * @brief TorontoTestSet::load Overriden method
 * @param _testSetName
 * @param _rootDir
 */
void TorontoTestSet::load()
{

#ifdef TORONTO_TESTSET_DEBUG
    cout << "TorontoTestSet::load" << endl;
#endif

    // Load course student counts from '.crs' file
    loadCourseCounts();
    // Compute student count from '.stu' file
    computeStudentCount();
    // Load conflict matrix and build adjacency list
    loadConflictMatrix();
    // Load course names
    loadCourseNames();
}


/**
 * @brief TorontoTestSet::loadCourseCounts
 * This method loads the Toronto .crs file containing the counts for each course.
 * It computes the following information:
 *    - numEnrolments
 *    - numExams
 *    - vector courseStudentCounts, containing the course counts for each exam
 * Exams numbering start at 0
 */
void TorontoTestSet::loadCourseCounts() {
    string filename = this->getRootDirectory() + "/" + this->getName() + ".crs";

#ifdef TORONTO_TESTSET_DEBUG
    cout << filename << endl;
#endif

    string wholeFile = read_from_file(filename.c_str());
    // Process whole file
    tokenizer<> tok(wholeFile);
    // Exam index and course count
    int idx, val;
    // Course count vector
    boost::shared_ptr<vector<int> > courseStudentCounts(new vector<int>());
    // # exams
    int numExams = 0;
    // # enrolments
    int numEnrolments = 0;
    // Fill course counts info
    for (tokenizer<>::iterator beg = tok.begin(); beg != tok.end();) {
        idx = atoi((*beg++).c_str());
        val = atoi((*beg++).c_str());

#ifdef TORONTO_TESTSET_DEBUG
//        cout << idx << " " << val << endl;
#endif

        // Insert entry into course count vector
        courseStudentCounts->push_back(val); // We don't use index here. The first count is for exam 0, etc.
        // Increment # exams
        numExams++;
        // Update number of enrolments
        numEnrolments += val;
    }

#ifdef TORONTO_TESTSET_DEBUG
//    copy(courseStudentCounts->begin(), courseStudentCounts->end(), ostream_iterator<int>(cout, "\n"));
//    cin.get();
#endif

    // Set # exams
    this->getTimetableProblemData()->setNumExams(numExams);
    // Set # enrolments
    this->getTimetableProblemData()->setNumEnrolments(numEnrolments);
    // Set course student counts
    this->getTimetableProblemData()->setCourseStudentCounts(courseStudentCounts);
}



void TorontoTestSet::computeStudentCount() {
    string filename = this->getRootDirectory() + "/" + this->getName() + ".stu";

#ifdef TORONTO_TESTSET_DEBUG
    cout << filename << endl;
#endif

///
/// ATTENTION:
///
/// Reading the files using a tokenizer also reads empty lines in the end of file.
/// We have to IGNORE ALL EMPTY LINES AT THE END IN THE ORIGINAL DATASETS
///
    string wholeFile = read_from_file(filename.c_str());
    int numLines = 0, numBlankLines = 0;
    // Process whole file escaping '\n'
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(
      "\n", // dropped delimiters
      "",  // kept delimiters
      boost::/*drop_empty_tokens*/keep_empty_tokens); // empty token policy
    tokenizer tok(wholeFile, sep);
    for (tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg, ++numLines) {
#ifdef TORONTO_TESTSET_DEBUG
        if (*beg == "") {
            cout << "Found blank line at index " << numLines << endl;
            ++numBlankLines;
        }
//        cout << *beg << endl;
#endif
    }

    // The number of students is equal to the number of records in '.stu' file
    int numStudents = numLines;
///
/// NOTE: pur93 and ute92 have blank lines.
/// pur93 has 3 blank lines but these are not ignored so # students = 30032 instead 30029
/// ute92 has 1 blank lines.
/// If we *ignore* the blank lines the # students = 2049 instead 2050 -> blank line 921
//    if (this->getName() == "ute-s-92")
//        --numStudents;

///
/// ATTENTION:
///
/// The original data: ftp://ftp.mie.utoronto.ca/pub/carter/testprob (version I).
/// To maintain the original values published by Carter, don't ignore blank lines in Pur93 and ute92 datasets

#ifdef TORONTO_TESTSET_DEBUG
    cout << "num students = " << numStudents << endl;
    cout << "num blank lines  = " << numBlankLines << endl;
    cout << "num students given by Carter  = " << this->datasetNumStudentsMap.at(this->getName()) << endl;
#endif


    // Set # students
//    this->getTimetableProblemData()->setNumStudents(numStudents);
    // Set # students to constant values given by Carter
    this->getTimetableProblemData()->setNumStudents(this->datasetNumStudentsMap.at(this->getName()));
}


// Load the conflict matrix for this test set
void TorontoTestSet::loadConflictMatrix() {

#ifdef TORONTOTESTSET_LOAD_CONFLICT_MATRIX_FILE
    string filename = this->getRootDirectory() + "/" + this->getName() + ".cm";

//    cout << filename << endl;

    string wholeFile = read_from_file(filename.c_str());
    // Get number of columns
    int ncols = 0;
    string firstLine;
    ifstream file(filename.c_str());
    getline(file, firstLine);
    file.close();
    tokenizer<> firstLineTok(firstLine);
    for (tokenizer<>::iterator beg = firstLineTok.begin(); beg != firstLineTok.end(); ++beg){
        ++ncols;
    }
    ////////////////////////////
    // Create shared ptr to manage Conflict Matrix
    boost::shared_ptr<Matrix<int> > ptrConflictMatrix(new Matrix<int>(ncols, ncols));
    // Instantiate graph with ncols vertices
    boost::shared_ptr<AdjacencyList> ptrExamGraph(new AdjacencyList(ncols));
    // Process whole file
    tokenizer<> tok(wholeFile);
    int v1 = 0, v2 = 0; // Exams (vertices) start at 0
    int cost, nonZeroElements = 0;
    for (tokenizer<>::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
        cost = atoi((*beg).c_str());
        // Also save arc's cost in conflict matrix
        ptrConflictMatrix->setVal(v1, v2, cost);
        if (cost != 0) {
            // Increment the number of non-zero elements
            ++nonZeroElements;
            if (v2 > v1) {
                add_edge(v1, v2, *ptrExamGraph.get());
            }
        }
        // Update line and column index
        if (v2 == ncols-1) {
            ++v1;
            v2 = 0;
        }
        else
            ++v2;
    }
    // The ‘conflict’ density is the ratio of the number of non-zero elements
    // in the conflict matrix to the total number of conflict matrix elements.
    double matrixElements = ncols*ncols;
    double conflictMatrixDensity = nonZeroElements / matrixElements;
    // Set conflict matrix
    this->getTimetableProblemData()->setConflictMatrix(ptrConflictMatrix);
    // Set conflict matrix density
    this->getTimetableProblemData()->setConflictMatrixDensity(conflictMatrixDensity);
    // Set exam graph
    this->getTimetableProblemData()->setExamGraph(ptrExamGraph);


//#ifdef TORONTO_TESTSET_DEBUG
//    cout << "Print conflict matrix contents: " << endl;
//    Matrix<int> const& mat = *ptrConflictMatrix.get();
//    int numLines = mat.getNumLines();
//    int numCols = mat.getNumCols();
//    cout << "numLines = " << numLines << ", numCols = " << numCols << endl;
//    if (numLines != numCols) {
//        cout << "numLines != numCols" << endl;
//        cin.get();
//    }
//    for (int i = 0; i < numLines; ++i) {
//        for (int j = 0; j < numCols; ++j) {
//            cout << mat.getVal(i,j) << " ";
//        }
//        cout << endl;
//    }
//    cin.get();
//#endif

#else
    /// Read .stu file and build conflict matrix and exam graph
    string filename = this->getRootDirectory() + "/" + this->getName() + ".stu";
//    cout << filename << endl;

    // For checking the # enrolments
    int numEnrolments1 = 0;
    // # exams enrolled by the current student
    int numEnrolledExams;

    // Get # exams previously read from .crs file
    const int numExams = this->getTimetableProblemData()->getNumExams();
    // Create shared ptr to manage Conflict Matrix
    boost::shared_ptr<IntMatrix> ptrConflictMatrix(new IntMatrix(numExams, numExams));
    // Instantiate graph with ncols vertices
    boost::shared_ptr<AdjacencyList> ptrExamGraph(new AdjacencyList(numExams));
    // Get exam graph
    AdjacencyList &examGraph = *ptrExamGraph.get();
    // Get Conflict Matrix
    IntMatrix &conflictMatrix = *ptrConflictMatrix.get();
    // Process .stu file one line at a time filling the conflict matrix and the exam graph
    // Each line corresponds to a student enrolled exams
    ifstream file(filename.c_str());
    if (file) {
        typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;
        boost::char_separator<char> sep(" ");
        string line;

        while (getline(file, line)) {
            Tokenizer info(line, sep);   // Tokenize the line of data
            vector<int> values;
            for (Tokenizer::iterator it = info.begin(); it != info.end(); ++it) {
                // Convert data into int value, and store
                values.push_back(atoi(it->c_str()));
#ifdef TORONTO_TESTSET_DEBUG
//                cout << it->c_str() << " ";
#endif
/// TODO
/// SHOULD USE strtol instead of atoi?
///
            }
#ifdef TORONTO_TESTSET_DEBUG
//            cout << endl;
//            cin.get();
#endif
            // Set # exams enrolled by the current student
            numEnrolledExams = values.size();
            // Sum to the total # enrolments
            numEnrolments1 += numEnrolledExams;

            // Now, fill the conflict matrix and exam graph for the current student
            // (each line corresponds to one student enrolment data)
            int v1, v2;
            for (int i = 0; i < values.size(); ++i) {
                for (int j = i+1; j < values.size(); ++j) {
                    // Get vertices.
                    /// Toronto exam indexes start at 1, but we use zero based indexes.
                    v1 = values[i]-1;
                    v2 = values[j]-1;
                    // Increment by one (one student is enrolled in v1 and v2) arc's cost in
                    // the conflict matrix for vertices v1 and v2
                    conflictMatrix.incVal(v1, v2);
                    conflictMatrix.incVal(v2, v1); // Conflict matrix is symmetric
                }
            }
        }
    }
    else
    {
        cerr << "Error: Unable to open file " << filename.c_str() << endl;
        exit(-1);
    }
    // Close file
    file.close();

    //
    // Build exam graph from conflict matrix
    //
    // # of non zero elements in the conflict matrix
    int nonZeroElements = 0;
    for (int i = 0; i < conflictMatrix.getNumLines(); ++i) {
        for (int j = 0; j < conflictMatrix.getNumCols(); ++j) { // Square and symmetric matrix
            if (conflictMatrix.getVal(i, j) > 0) { // There is an arc between vertices i and j
                // Add arc in the exam graph
                // If j > i
                if (j > i)
                    add_edge(i, j, examGraph);
                // Increment the number of non-zero elements
                ++nonZeroElements;
            }
        }
    }

    // The ‘conflict’ density is the ratio of the number of non-zero elements
    // in the conflict matrix to the total number of conflict matrix elements.
    double numMatrixElements = numExams*numExams;
    // Consider diagonal elements
    double conflictMatrixDensity = (nonZeroElements) / (numMatrixElements - numExams);

    // Set conflict matrix
    this->getTimetableProblemData()->setConflictMatrix(ptrConflictMatrix);
    // Set conflict matrix density
    this->getTimetableProblemData()->setConflictMatrixDensity(conflictMatrixDensity);
    // Set exam graph
    this->getTimetableProblemData()->setExamGraph(ptrExamGraph);

#ifdef TORONTO_TESTSET_DEBUG
//    cout << "numEnrolments1 = " << numEnrolments1 << ", this->getTimetableProblemData()->getNumEnrolments() = "
//         << this->getTimetableProblemData()->getNumEnrolments() << endl;

//    //////////////////////////////////////////////////////////////////////////
//    //
//    // Verification of exam graph integrity
//    //
//    //////////////////////////////////////////////////////////////////////////
//    // # conflicts between exams. Should be equal to the number of non-zero elements in the conflict matrix
//    int countNumConflicts = 0;
//    property_map<AdjacencyList, vertex_index_t>::type index_map = get(vertex_index, examGraph);
//    graph_traits<AdjacencyList>::adjacency_iterator ai, a_end;
//    // Iterate over all exams and check if the number of edges in the exam graph
//    // correspond to the edeges in the conflict matrix
//    for (int ei = 0; ei < numExams; ++ei) {
//        // Get ei adjacent exams
//        boost::tie(ai, a_end) = adjacent_vertices(ei, examGraph);
//        for (; ai != a_end; ++ai) {
//            // Get adjacent exam
//            int ej = get(index_map, *ai);
//            // Verify if there's a conflict between ei and ej
//            if (conflictMatrix.getVal(ei, ej) > 0)
//                ++countNumConflicts;
//        }
//    }
//    cout << "nonZeroElements = " << nonZeroElements << ", countNumConflicts = " << countNumConflicts << endl;

#endif


#endif

}


// Load course names
void TorontoTestSet::loadCourseNames() {
    //
    // For the Toronto datasets the course names are not considered
    //
}


ostream& operator<<(ostream& _os, const TestSet& _t) {
    _os << "Test set\t # exams\t # students\t # enrolments\t conflict density\t # time slots" << endl;
    _os << _t.getName() << "\t "
        << _t.getTimetableProblemData()->getNumExams() << "\t "
        << _t.getTimetableProblemData()->getNumStudents() << "\t "
        << _t.getTimetableProblemData()->getNumEnrolments() << "\t "
        << _t.getTimetableProblemData()->getConflictMatrixDensity() << "\t "
        << _t.getTimetableProblemData()->getNumPeriods() << endl;

    return _os;
}









