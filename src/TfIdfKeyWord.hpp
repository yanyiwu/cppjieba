#ifndef CPPJIEBA_TFIDF_H
#define CPPJIEBA_TFIDF_H

#include "MPSegment.hpp"

namespace CppJieba
{
    using namespace Limonp;

    class TfIdfKeyWord
    {
        private:
            MPSegment _segment;
        public:
            TfIdfKeyWord(const char* dictFile): _segment(dictFile){};
            ~TfIdfKeyWord(){};
        public:
            bool init(){return _segment.init();};
            bool dispose(){return _segment.dispose();};
        public:
            bool extract(const string& str, vector<string>& words, uint topN)
            {
                return _segment.cut(words);
                return true;
            }
            
    };
}

#endif


