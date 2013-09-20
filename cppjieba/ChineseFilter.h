#ifndef CPPJIEBA_CHINESEFILTER_H
#define CPPJIEBA_CHINESEFILTER_H

#include "globals.h"
#include "TransCode.h"

namespace CppJieba
{

    class ChFilterIterator;
    class ChineseFilter
    {
        public:
            typedef ChFilterIterator  iterator;
        public:
            ChineseFilter();
            ~ChineseFilter();
        public:
            bool feed(const string& str);
            iterator begin();
            iterator end();
        private:
            Unicode _unico;
        private:
            ChFilterIterator _get(UniConIter iter);
        private:
            bool _isChWord(uint16_t x)
            {
                return (x >= 0x4e00 && x <= 0x9f);
            }
            friend class ChFilterIterator;
    };
    class ChFilterIterator
    {
        public:
            ChineseFilter * ptChFilter;
            UniConIter begin;
            UniConIter end;
            bool isChWord;
            ChFilterIterator& operator++()
            {
                if(end == ptChFilter->_unico.end())
                {
                    return *this;
                }
                 *this = ptChFilter->_get(end+1);
                 return *this;
            }
            bool operator==(const ChFilterIterator& iter)
            {
                return end == iter.end;
            }
            ChFilterIterator& operator=(const ChFilterIterator& iter)
            {
                ptChFilter = iter.ptChFilter;
                begin = iter.begin;
                end = iter.end;
                isChWord = iter.isChWord;
                return *this;
            }

            ChFilterIterator(ChineseFilter * ptcf, UniConIter be, UniConIter en, bool is):ptChFilter(ptcf), begin(be), end(en), isChWord(is){};
            ChFilterIterator(ChineseFilter * ptcf);
        private:
            ChFilterIterator(){}

    };
}

#endif
