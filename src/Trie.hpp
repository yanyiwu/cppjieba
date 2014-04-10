#ifndef CPPJIEBA_TRIE_HPP
#define CPPJIEBA_TRIE_HPP

#include "Limonp/std_outbound.hpp"
#include <vector>

namespace CppJieba
{
    template <class KeyType, class ValueType>
        class TrieNode
        {
            public:
                typedef unordered_map<KeyType, TrieNode*> TrieNodeMapType;
            public:
                TrieNodeMap * ptKeyMap;
                const ValueType * ptValue;
        };

    template <class KeyType, class ValueType>
        class Trie
        {
            private:
                TrieNode* _root;
            private:
            public:
                Trie(const vector<KeyType>& keys, const vector<ValueType* >& valuePointers)
                {
                    _root = new TrieNode;
                    _root->ptKeyMap = NULL;
                    _root->ptValue = NULL;

                    _createTrie(keys, valuePointers);
                }
                ~Trie()
                {
                }
            public:
                const ValueType* find(KeyType::const_iterator begin; KeyType::const_iterator end) const
                {
                    TrieNodeMapType::const_iterator citer;
                    const TrieNode* ptNode = _root;
                    for(KeyType::const_iterator it = begin; it != end; it++)
                    {
                        citer = ptNode->ptKeyMap->find(*it);
                        if(ptNode->ptKeyMap->end() == citer)
                        {
                            return NULL;
                        }
                        ptNode= citer->second;
                    }
                    return ptNode->ptValue;
                }
                bool find(KeyType::const_iterator begin, KeyType::const_iterator end, map<KeyType::size_type, const ValueType* >& ordererMap) const
                {
                    const TrieNode * ptNode = _root;
                    TrieNodeMapType::const_iterator citer;
                    for(KeyType::const_iterator itr = begin; itr != end ; itr++)
                    {
                        citer = ptNode->ptKeyMap->find(*itr);
                        if(ptNode->ptKeyMap->end() == citer)
                        {
                            break;
                        }
                        ptNode = citer->second;
                        if(ptNode->ptValue)
                        {
                            ordererMap[itr - begin] = ptNode->ptValue;
                        }
                    }
                }
            private:
                void _createTrie(const vector<KeyType>& keys, const vector<ValueType*>& valuePointers)
                {
                    if(values.empty() || keys.empty())
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
                void _insertNode(const KeyType& key, const Value* ptValue)
                {
                    TrieNode* ptNode  = _root;

                    TrieNode::KeyMapType::const_iterator kmIter;

                    for(KeyType::const_iterator citer = key.begin(); citer != key.end(); citer++)
                    {
                        if(NULL == ptNode->ptKeyMap)
                        {
                            ptNode->ptKeyMap = new TrieNode::KeyMapType;
                        }
                        kmIter = ptNode->ptKeyMap->find(*citer);
                        if(ptNode->ptKeyMap->end() == kmIter)
                        {
                            TrieNode * nextNode = new TrieNode;
                            nextNode->ptKeyMap = NULL;
                            nextNode->ptValue = NULL;

                            ptNode->ptKeyMap[*citer] = nextNode;
                            ptNode = next;
                        }
                        else
                        {
                            ptNode = kmIter->second;
                        }
                    }
                    ptNode->ptValue = ptValue;
                }
                void _deleteNode(TrieNode* node)
                {
                    if(!node)
                    {
                        return;
                    }
                    for(TrieNodeMapType::iterator it = node->ptKeyMap->begin(); it != node->ptKeyMap->end(); it++)
                    {
                        _deleteNode(it->second);
                    }
                    delete node->ptKeyMap;
                    delete node;
                }
        }
}

#endif
