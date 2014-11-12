#ifndef CPPJIEBA_TRIE_HPP
#define CPPJIEBA_TRIE_HPP

#include "Limonp/StdExtension.hpp"
#include <vector>
#include <queue>

namespace CppJieba
{
    using namespace std;

    struct DictUnit
    {
        Unicode word;
        double weight; 
        string tag;
    };

    inline ostream & operator << (ostream& os, const DictUnit& unit)
    {
        string s;
        s << unit.word;
        return os << string_format("%s %s %.3lf", s.c_str(), unit.tag.c_str(), unit.weight);
    }

    typedef std::vector<std::pair<size_t, const DictUnit*> > DagType;

    struct SegmentChar 
    {
        uint16_t uniCh;
        DagType dag;
        const DictUnit * pInfo;
        double weight;
        size_t nextPos;
        SegmentChar():uniCh(0), pInfo(NULL), weight(0.0), nextPos(0)
        {}
    };

    template <class KeyType, class ValueType>
        class TrieNode
        {
            public:
                typedef unordered_map<KeyType, TrieNode<KeyType, ValueType>* > NextMap;
            public:
                TrieNode * fail;
                NextMap * next;
                const ValueType * ptValue;
            public:
                TrieNode(): fail(NULL), next(NULL), ptValue(NULL) {
                }
        };

    template <class KeyType, class ValueType, class KeyContainerType = vector<KeyType>, class KeysContainerType = vector<KeyContainerType>, class ValueContainerType = vector<const ValueType* > >
        class Trie
        {
            public:
                typedef TrieNode<KeyType, ValueType> TrieNodeType;
            private:
                TrieNodeType* _root;
            public:
                Trie(const KeysContainerType& keys, const ValueContainerType& valuePointers)
                {
                    _root = new TrieNodeType;
                    _createTrie(keys, valuePointers);
                    _build();// build automation
                }
                ~Trie()
                {
                    if(_root)
                    {
                        _deleteNode(_root);
                    }
                }
            public:
                const ValueType* find(typename KeyContainerType::const_iterator begin, typename KeyContainerType::const_iterator end) const
                {
                    typename TrieNodeType::NextMap::const_iterator citer;
                    const TrieNodeType* ptNode = _root;
                    for(typename KeyContainerType::const_iterator it = begin; it != end; it++)
                    {// build automation
                        assert(ptNode);
                        if(NULL == ptNode->next || ptNode->next->end() == (citer = ptNode->next->find(*it)))
                        {
                            return NULL;
                        }
                        ptNode = citer->second;
                    }
                    return ptNode->ptValue;
                }
                // aho-corasick-automation 
                void find(
                            typename KeyContainerType::const_iterator begin, 
                            typename KeyContainerType::const_iterator end, 
                            vector<struct SegmentChar>& res
                            ) const
                {
                    res.resize(end - begin);
                    const TrieNodeType * now = _root;
                    typename TrieNodeType::NextMap::const_iterator iter;
                    for (size_t i = 0; i < end - begin; i++) 
                    {
                        res[i].uniCh = *(begin + i);
                        assert(res[i].dag.empty());
                        res[i].dag.reserve(2);
                        res[i].dag.push_back(pair<typename KeysContainerType::size_type, const ValueType* >(i, NULL));
                        while( now != _root && (now->next == NULL || (iter = now->next->find(res[i].uniCh)) == now->next->end())) 
                        {
                            now = now->fail;
                        }
                        if(now->next == NULL || (iter = now->next->find(res[i].uniCh)) == now->next->end()) 
                        {
                            now = _root;
                        } 
                        else 
                        {
                            now = iter->second;
                            const TrieNodeType * temp = now;
                            while(temp != _root) 
                            {
                                if (temp->ptValue) 
                                {
                                    size_t pos = i - temp->ptValue->word.size() + 1;
                                    res[pos].dag.push_back(pair<typename KeysContainerType::size_type, const ValueType* >(i, temp->ptValue));
                                    if(pos == i) 
                                    {
                                        res[pos].dag[0].second = temp->ptValue;
                                    }
                                }
                                temp = temp->fail;
                                assert(temp);
                            }
                        }
                    }
                }
                bool find(
                            typename KeyContainerType::const_iterator begin, 
                            typename KeyContainerType::const_iterator end, 
                            std::vector<std::pair<typename KeyContainerType::size_type, const ValueType* > >& res, 
                            size_t offset = 0) const
                {
                    const TrieNodeType * ptNode = _root;
                    typename TrieNodeType::NextMap::const_iterator citer;
                    for(typename KeyContainerType::const_iterator itr = begin; itr != end ; itr++)
                    {
                        assert(ptNode);
                        if(NULL == ptNode->next || ptNode->next->end() == (citer = ptNode->next->find(*itr)))
                        {
                            break;
                        }
                        ptNode = citer->second;
                        if(ptNode->ptValue)
                        {
                            if(itr == begin && res.size() == 1) // first singleword
                            {
                                res[0].second = ptNode->ptValue;
                            }
                            else
                            {
                                res.push_back(pair<typename KeysContainerType::size_type, const ValueType* >(itr - begin + offset, ptNode->ptValue));
                            }
                        }
                    }
                    return !res.empty();
                }
            private:
                void _build()
                {
                    queue<TrieNodeType*> que;
                    assert(_root->ptValue == NULL);
                    assert(_root->next);
                    _root->fail = NULL;
                    for(typename TrieNodeType::NextMap::iterator iter = _root->next->begin(); iter != _root->next->end(); iter++) {
                        iter->second->fail = _root;
                        que.push(iter->second);
                    }
                    TrieNodeType* back = NULL;
                    typename TrieNodeType::NextMap::iterator backiter;
                    while(!que.empty()) {
                        TrieNodeType * now = que.front();
                        que.pop();
                        if(now->next == NULL) {
                            continue;
                        }
                        for(typename TrieNodeType::NextMap::iterator iter = now->next->begin(); iter != now->next->end(); iter++) {
                            back = now->fail;
                            while(back != NULL) {
                                if(back->next && (backiter = back->next->find(iter->first)) != back->next->end()) 
                                {
                                    iter->second->fail = backiter->second;
                                    break;
                                }
                                back = back->fail;
                            }
                            if(back == NULL) {
                                iter->second->fail = _root;
                            }
                            que.push(iter->second);
                        }
                    }
                }
            private:
                void _createTrie(const KeysContainerType& keys, const ValueContainerType& valuePointers)
                {
                    if(valuePointers.empty() || keys.empty())
                    {
                        return;
                    }
                    assert(keys.size() == valuePointers.size());

                    for(size_t i = 0; i < keys.size(); i++)
                    {
                        _insertNode(keys[i], valuePointers[i]);
                    }
                }
            private:
                void _insertNode(const KeyContainerType& key, const ValueType* ptValue)
                {
                    TrieNodeType* ptNode  = _root;

                    typename TrieNodeType::NextMap::const_iterator kmIter;

                    for(typename KeyContainerType::const_iterator citer = key.begin(); citer != key.end(); citer++)
                    {
                        if(NULL == ptNode->next)
                        {
                            ptNode->next = new typename TrieNodeType::NextMap;
                        }
                        kmIter = ptNode->next->find(*citer);
                        if(ptNode->next->end() == kmIter)
                        {
                            TrieNodeType * nextNode = new TrieNodeType;
                            nextNode->next = NULL;
                            nextNode->ptValue = NULL;

                            (*ptNode->next)[*citer] = nextNode;
                            ptNode = nextNode;
                        }
                        else
                        {
                            ptNode = kmIter->second;
                        }
                    }
                    ptNode->ptValue = ptValue;
                }
                void _deleteNode(TrieNodeType* node)
                {
                    if(!node)
                    {
                        return;
                    }
                    if(node->next)
                    {
                        typename TrieNodeType::NextMap::iterator it;
                        for(it = node->next->begin(); it != node->next->end(); it++)
                        {
                            _deleteNode(it->second);
                        }
                        delete node->next;
                    }
                    delete node;
                }
        };
}

#endif
