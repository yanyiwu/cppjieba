/*
 * UndirectWeightedGraph.hpp
 *
 *  Created on: 2015年7月7日
 *      Author: oliverlwang
 */

#ifndef UNDIRECTWEIGHTEDGRAPH_H_
#define UNDIRECTWEIGHTEDGRAPH_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

using namespace std;

namespace CppJieba
{

/* edge type */
struct edge_t
{
	string start;
	string end;
	double weight;
};

class UndirectWeightedGraph
{
public:
	UndirectWeightedGraph(): _dampingFactor(0.85){};
	virtual ~UndirectWeightedGraph(){};

    /**
	 * @brief add an edge for the UndirectedWeighted Graph
	 *
	 * @param string &start
	 * @param  string &end
	 * @param
	 *
     * @retval
     */
	void addEdge(const string &start, const string &end, double weight)
	{
		/* add an out edge for vertex start */
		edge_t _edge;
		_edge.start = start;
		_edge.end = end;
		_edge.weight = weight;

		_graph[start].push_back(_edge);

		/* add an out edge for vertex end */
		_edge.start = end;
		_edge.end = start;

		_graph[end].push_back(_edge);
	}

    /**
	 * @brief rank the words according to its score
	 *
	 * @param none
	 *
     * @retval none
     */
	void rank()
	{
		map<string, double> outSum;

		/* initialize words score */
		double wsdef = (_graph.size() > 0) ? (1.0 / _graph.size()) : 1.0;

		for(map<string, vector<edge_t> >::iterator it = _graph.begin(); it != _graph.end(); ++it)
		{
	        ws[it->first] = wsdef;
	        outSum[it->first] = weightOutSum(it->second);
		}

		/* iterator 10 times */
		for(int i = 0; i < 10; ++i)
		{
			/* stl map is sorted by key by default */
			for(map<string, vector<edge_t> >::iterator i = _graph.begin(); i != _graph.end(); ++i)
			{
	            double score = 0.0;
	            for(vector<edge_t>::iterator j = i->second.begin(); j != i->second.end(); ++j)
	            {
	                score += j->weight / outSum[j->end] * ws[j->end];
	            }

	            ws[i->first] = (1.0 - _dampingFactor) + _dampingFactor * score;
			}
		}

		/* normalize */
		double max_rank = max_element(ws.begin(), ws.end())->second;
		double min_rank = min_element(ws.begin(), ws.end())->second;

		for(map<string, double>::iterator m = ws.begin(); m != ws.end(); ++m)
		{
			m->second = (m->second - min_rank / 10.0) / (max_rank - min_rank / 10.0);
		}
	}

public:
	/* words score */
	map<string, double> ws;

private:
	/* Graph, key: vertex which is a words or term, value: In(vertex) */
    map<string, vector<edge_t> > _graph;

    /* d is the damping factor that can be set between 0 and 1, always set to 0.85 */
    double _dampingFactor;

private:
    /* calculate the weight sum of out edge */
    double weightOutSum(const vector<edge_t>& v)
    {
    	double sum = 0.0;
        for(vector<edge_t>::const_iterator it = v.begin(); it != v.end(); ++it)
        {
        	sum += it->weight;
        }
        return sum;
    }

};

}/* namespace CppJieba */

#endif /* UNDIRECTWEIGHTEDGRAPH_H_ */




