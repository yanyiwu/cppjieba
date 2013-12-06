#ifndef CPPJIEBA_CHINESEFILTER_H
#define CPPJIEBA_CHINESEFILTER_H

#include "TransCode.hpp"

namespace CppJieba
{

    //enum CHAR_TYPE { CHWORD = 0, DIGIT_OR_LETTER = 1};
    //typedef Unicode::const_iterator UniConIter;
    //class ChineseFilter;
    //class ChFilterIterator
    //{
    //    public:
    //        const Unicode * ptUnico;
    //        UniConIter begin;
    //        UniConIter end;
    //        CHAR_TYPE charType;
    //        ChFilterIterator& operator++()
    //        {
    //            return *this = _get(end);
    //        }
    //        ChFilterIterator operator++(int)
    //        {
    //            ChFilterIterator res = *this;
    //            *this = _get(end);
    //            return res;
    //        }
    //        bool operator==(const ChFilterIterator& iter)
    //        {
    //            return begin == iter.begin && end == iter.end;
    //        }
    //        bool operator!=(const ChFilterIterator& iter)
    //        {
    //            return !(*this == iter);
    //        }
    //        ChFilterIterator& operator=(const ChFilterIterator& iter)
    //        {
    //            ptUnico = iter.ptUnico;
    //            begin = iter.begin;
    //            end = iter.end;
    //            charType = iter.charType;
    //            return *this;
    //        }
    //    
    //    public:
    //        ChFilterIterator(const Unicode * ptu, UniConIter be, UniConIter en, CHAR_TYPE is):ptUnico(ptu), begin(be), end(en), charType(is){};
    //        ChFilterIterator(const Unicode * ptu):ptUnico(ptu){*this = _get(ptUnico->begin());};
    //    private:
    //        ChFilterIterator(){}
    //    private:
    //        CHAR_TYPE _charType(uint16_t x)const
    //        {
    //            if(x < 0x0080)
    //            {
    //                return DIGIT_OR_LETTER;
    //            }
    //            return CHWORD;
    //        }
    //        ChFilterIterator _get(UniConIter iter)
    //        {
    //            UniConIter _begin = iter;
    //            const UniConIter& _end = ptUnico->end();
    //            if(iter == _end)
    //            {
    //                return ChFilterIterator(ptUnico, end, end, DIGIT_OR_LETTER);
    //            }
    //            CHAR_TYPE charType = _charType(*iter);
    //            iter ++;
    //            while(iter != _end &&charType ==  _charType(*iter))
    //            {
    //                iter++;
    //            }
    //            return ChFilterIterator(ptUnico, _begin, iter, charType);
    //        }

    //};
    //class ChineseFilter
    //{
    //    private:
    //        Unicode _unico;
    //    public:
    //        typedef ChFilterIterator  iterator;
    //    public:
    //        ChineseFilter(){};
    //        ~ChineseFilter(){};
    //    public:
    //        bool feed(const string& str)
    //        {
    //            return TransCode::decode(str, _unico);
    //        }
    //        iterator begin()
    //        {
    //            return iterator(&_unico);
    //        }
    //        iterator end()
    //        {
    //            return iterator(&_unico, _unico.end(), _unico.end(), DIGIT_OR_LETTER);
    //        }
    //};
    
    /*
     * if char is ascii, count the ascii string's length and return 0;
     * else count the nonascii string's length and return 1;
     * if errors, return -1;
     * */
    inline int filterAscii(const char* str, uint len, uint& resLen)
    {
        if(!str || !len)
        {
            return -1;
        }
        char x = 0x80;
        int resFlag = (str[0] & x ? 1 : 0);
        resLen = 1;
        if(!resFlag)
        {
            while(resLen < len && !(str[resLen] & x))
            {
                resLen ++;
            }
        }
        else
        {
            while(resLen < len && (str[resLen] & x))
            {
                resLen ++;
            }
        }
        return resFlag;
    }
}

#endif
