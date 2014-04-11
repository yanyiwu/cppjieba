#ifndef CPPJIEBA_TRIE_HPP
#define CPPJIEBA_TRIE_HPP

#include "Limonp/std_outbound.hpp"
#include <vector>

namespace CppJieba
{
    using namespace std;
    template <class KeyType, class ValueType>
        class TrieNode
        {
            public:
                typedef unordered_map<KeyType, TrieNode<KeyType, ValueType>* > KeyMapType;
            public:
                KeyMapType * ptKeyMap;
                const ValueType * ptValue;
        };

    template <class KeyType, class ValueType>
        class Trie
        {
            public:
                typedef TrieNode<KeyType, ValueType> TrieNodeType;
            private:
                TrieNodeType* _root;
            public:
                Trie(const vector<vector<KeyType> >& keys, const vector<const ValueType* >& valuePointers)
                {
                    _root = new TrieNodeType;
                    _root->ptKeyMap = NULL;
                    _root->ptValue = NULL;

                    _createTrie(keys, valuePointers);
                }
                ~Trie()
                {
                    if(_root)
                    {
                        _deleteNode(_root);
                    }
                }
            public:
                const ValueType* find(typename vector<KeyType>::const_iterator begin, typename vector<KeyType>::const_iterator end) const
                {
                    typename TrieNodeType::KeyMapType::const_iterator citer;
                    const TrieNodeType* ptNode = _root;
                    for(typename vector<KeyType>::const_iterator it = begin; it != end; it++)
                    {
                        assert(ptNode);
                        if(NULL == ptNode->ptKeyMap || ptNode->ptKeyMap->end() == (citer = ptNode->ptKeyMap->find(*it)))
                        {
                            return NULL;
                        }
                        ptNode = citer->second;
                    }
                    return ptNode->ptValue;
                }
                bool find(typename vector<KeyType>::const_iterator begin, typename vector<KeyType> ::const_iterator end, map<typename vector<KeyType>::size_type, const ValueType* >& ordererMap, size_t offset = 0) const
                {
                    const TrieNodeType * ptNode = _root;
                    typename TrieNodeType::KeyMapType::const_iterator citer;
                    ordererMap.clear();
                    for(typename vector<KeyType>::const_iterator itr = begin; itr != end ; itr++)
                    {
                        assert(ptNode);
                        if(NULL == ptNode->ptKeyMap || ptNode->ptKeyMap->end() == (citer = ptNode->ptKeyMap->find(*itr)))
                        {
                            break;
                        }
                        ptNode = citer->second;
                        if(ptNode->ptValue)
                        {
                            ordererMap[itr - begin + offset] = ptNode->ptValue;
                        }
                    }
                    return ordererMap.size();
                }
            private:
                void _createTrie(const vector<vector<KeyType> >& keys, const vector<const ValueType*>& valuePointers)
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
                void _insertNode(const vector<KeyType>& key, const ValueType* ptValue)
                {
                    TrieNodeType* ptNode  = _root;

                    typename TrieNodeType::KeyMapType::const_iterator kmIter;

                    for(typename vector<KeyType>::const_iterator citer = key.begin(); citer != key.end(); citer++)
                    {
                        if(NULL == ptNode->ptKeyMap)
                        {
                            ptNode->ptKeyMap = new typename TrieNodeType::KeyMapType;
                        }
                        kmIter = ptNode->ptKeyMap->find(*citer);
                        if(ptNode->ptKeyMap->end() == kmIter)
                        {
                            TrieNodeType * nextNode = new TrieNodeType;
                            nextNode->ptKeyMap = NULL;
                            nextNode->ptValue = NULL;

                            (*ptNode->ptKeyMap)[*citer] = nextNode;
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
                    if(node->ptKeyMap)
                    {
                        typename TrieNodeType::KeyMapType::iterator it;
                        for(it = node->ptKeyMap->begin(); it != node->ptKeyMap->end(); it++)
                        {
                            _deleteNode(it->second);
                        }
                        delete node->ptKeyMap;
                    }
                    delete node;
                }
        };
}

#endif
