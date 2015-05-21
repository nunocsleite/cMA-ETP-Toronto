#ifndef COMMON_H
#define COMMON_H


#include <boost/graph/adjacency_list.hpp>
#include <boost/random.hpp>

// Use a vector for vertices and a linked list for edges
//typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> AdjacencyList;

// Use a vector for vertices and a linked list for edges
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> AdjacencyList;

typedef AdjacencyList::vertex_descriptor Vertex;


/**
 * @brief RNGType Random type
 */
typedef boost::mt19937 RNGType;



#endif // COMMON_H
