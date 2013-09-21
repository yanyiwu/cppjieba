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
            //friend class ChFilterIterator;
    };

    class ChFilterIterator
    {
        public:
            enum CHAR_TYPE { CHWORD = 0, DIGIT_OR_LETTER = 1, OTHERS = 2};
        public:
            const Unicode * ptUnico;
            UniConIter begin;
            UniConIter end;
            CHAR_TYPE charType;
            ChFilterIterator& operator++()
            {
                return *this = _get(end);
            }
            ChFilterIterator operator++(int)
            {
                ChFilterIterator res = *this;
                *this = _get(end);
                return res;
            }
            bool operator==(const ChFilterIterator& iter)
            {
                return begin == iter.begin && end == iter.end;
            }
            bool operator!=(const ChFilterIterator& iter)
            {
                return !(*this == iter);
            }
            ChFilterIterator& operator=(const ChFilterIterator& iter)
            {
                ptUnico = iter.ptUnico;
                begin = iter.begin;
                end = iter.end;
                charType = iter.charType;
                return *this;
            }
        
        public:
            ChFilterIterator(const Unicode * ptu, UniConIter be, UniConIter en, CHAR_TYPE is):ptUnico(ptu), begin(be), end(en), charType(is){};
            ChFilterIterator(const Unicode * ptu):ptUnico(ptu){*this = _get(ptUnico->begin());};
        private:
            ChFilterIterator(){}
        private:
            CHAR_TYPE _charType(uint16_t x)const
            {
                if((0x0030 <= x && x<= 0x0039) || (0x0041 <= x && x <= 0x005a ) || (0x0061 <= x && x <= 0x007a))
                {
                    return DIGIT_OR_LETTER;
                }
                if(x >= 0x4e00 && x <= 0x9fff)
                {
                    return CHWORD;
                }
                return OTHERS;
            }
            ChFilterIterator _get(UniConIter iter)
            {
                UniConIter _begin = iter;
                const UniConIter& _end = ptUnico->end();
                if(iter == _end)
                {
                    return ChFilterIterator(ptUnico, end, end, OTHERS);
                }
                CHAR_TYPE charType = _charType(*iter);
                iter ++;
                //cout<<charType<<endl;
                //getchar();
                while(iter != _end &&charType ==  _charType(*iter))
                {
                    //cout<<__LINE__<<endl;
                    iter++;
                }
                return ChFilterIterator(ptUnico, _begin, iter, charType);
            }

    };
}

#endif
