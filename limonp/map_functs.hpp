/************************************
 * file enc : ascii
 * author   : wuyanyi09@gmail.com
 ************************************/


#ifndef LIMONP_MAP_FUNCTS_H
#define LIMONP_MAP_FUNCTS_H

#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include "typedefs.h"

namespace Limonp
{
    using namespace std;


    template <typename T>
        string setToString(const set<T>& st)
        {
            if(st.empty())
            {
                return "{}";
            }
            stringstream ss;
            ss<<'{';
            typename set<T>::const_iterator it = st.begin();
            ss<<*it;
            it++;
            while(it != st.end())
            {
                ss<<", "<<*it;
                it++;
            }
            ss<<'}';
            return ss.str();
        }

    template<typename T1, typename T2>
        string mapToString(const map<T1, T2>& mp)
        {
            if(mp.empty())
            {
                return "{}";
            }
            stringstream ss;
            ss<<'{';
            typename map<T1, T2>::const_iterator it = mp.begin();
            ss<<it->first<<": "<<it->second;
            it++;
            while(it != mp.end())
            {
                ss<<", "<<it->first<<": "<<it->second;
                it++;
            }
            ss<<'}';
            return ss.str();
        }

    template<typename T1, typename T2>
        string HashMapToString(const HashMap<T1, T2>& mp)
        {
            if(mp.empty())
            {
                return "{}";
            }
            stringstream ss;
            ss<<'{';
            typename HashMap<T1, T2>::const_iterator it = mp.begin();
            ss<<it->first<<": "<<it->second;
            it++;
            while(it != mp.end())
            {
                ss<<", "<<it->first<<": "<<it->second;
                it++;
            }
            ss<<'}';
            return ss.str();
        }
    template<typename T1, typename T2>
        string pairToString(const pair<T1, T2>& p)
        {
            stringstream ss;
            ss<<p.first<<":"<<p.second;
            return ss.str();
        }

    template<class kT, class vT>
        void printMap(const map<kT, vT>& mp)
        {
            for(typename map<kT, vT>::const_iterator it = mp.begin(); it != mp.end(); it++)
            {
                cout<<it->first<<' '<<it->second<<endl;
            }
        }

    template<class kT, class vT>
        vT getMap(const map<kT, vT>& mp, const kT & key, const vT & defaultVal)
        {
            typename map<kT, vT>::const_iterator it;
            it = mp.find(key);
            if(mp.end() == it)
            {
                return defaultVal;
            }
            return it->second;
        }

    template<class kT, class vT>
        void map2Vec(const map<kT, vT>& mp, vector<pair<kT, vT> > & res)
        {
            typename map<kT, vT>::const_iterator it = mp.begin();
            for(; it != mp.end(); it++)
            {
                res.push_back(*it);
            }
        }
}

#endif
