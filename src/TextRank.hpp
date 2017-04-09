/*
 * TextRank.hpp
 *
 *  Created on: 2015年7月7日
 *      Author: oliverlwang
 */
#ifndef TEXTRANK_H_
#define TEXTRANK_H_

#include "UndirectWeightedGraph.hpp"

namespace CppJieba
{

class TextRank
{
public:
	TextRank() : _span(2) {};
	virtual ~TextRank(){};

    /**
	 * @brief get the TopN Keywords.
	 *
	 * @param vector words input words
	 * @param vector keywords keywords with score
	 * @param int topN how many keywords do you want
	 *
     * @retval
     */
	int textRank(vector<string>& words, map<string, double>& wordmap)
	{
		try
		{
			UndirectWeightedGraph graph;
			map< pair<string, string>, double> cm;

			for(size_t i = 0; i < words.size(); ++i)
			{
				/* syntactic filter */

				/* ngram, when span=2, f-measure gets best result */
				for(size_t j = i + 1; j < i + _span; ++j)
				{
					if(j >= words.size())
						break;
					/* using std::pair as union key */
					pair<string, string> key = make_pair(words[i], words[j]);
					cm[key] += 1.0;
				}
			}

			/* add edge */
			for(map< pair<string, string>, double>::iterator it = cm.begin(); it != cm.end(); ++it)
			{
				/* do not add edge between the same vertex */
				if(it->first.first == it->first.second)
					continue;

				graph.addEdge(it->first.first, it->first.second, it->second);
			}

			/* rank */
			graph.rank();

			wordmap.clear();
			wordmap = graph.ws;
		}
		catch(exception &e)
		{
			cerr << e.what() << endl;
			return -1;
		}
		return 0;
	}

private:
    int _span;             /* scanning span */


};

} /* namespace CppJieba */
#endif /* TEXTRANK_H_ */


