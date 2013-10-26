/************************************
 * file enc : ASCII
 * author   : wuyanyi09@gmail.com
************************************/
#ifndef CPPJIEBA_MPSEGMENT_H
#define CPPJIEBA_MPSEGMENT_H

#include <algorithm>
#include <set>
#include <logger.hpp>
#include "Trie.h"
#include "globals.h"
#include "ISegment.hpp"
#include "SegmentBase.hpp"

namespace CppJieba
{

    typedef vector<SegmentChar> SegmentContext;

    class MPSegment: public SegmentBase
    {
        private:
            Trie _trie;
            
        public:
            MPSegment();
            virtual ~MPSegment();
        public:
            bool init(const char* const filePath);
            bool dispose();
        public:
            //bool cut(const string& str, vector<TrieNodeInfo>& segWordInfos)const;
            bool cut(const string& str, vector<string>& res)const;
            bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const;
            bool cut(const string& str, vector<TrieNodeInfo>& segWordInfos)const;
            bool cut(Unicode::const_iterator begin , Unicode::const_iterator end, vector<TrieNodeInfo>& segWordInfos)const;
            //virtual bool cut(const string& str, vector<string>& res)const;

        private:
            bool _calcDAG(SegmentContext& segContext)const;
            bool _calcDP(SegmentContext& segContext)const;
            bool _cut(SegmentContext& segContext, vector<TrieNodeInfo>& res)const;


    };
}

#endif
