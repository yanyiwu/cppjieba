#ifndef CPPJIEBA_TRIE_HPP
#define CPPJIEBA_TRIE_HPP

#include <vector>
#include <queue>
#include "HashMap.hpp"

namespace cppjieba {
using namespace std;

const size_t MAX_WORD_LENGTH = 512;

struct DictUnit {
  Unicode word;
  double weight;
  string tag;
};

// for debugging
inline ostream & operator << (ostream& os, const DictUnit& unit) {
  string s;
  s << unit.word;
  return os << StringFormat("%s %s %.3lf", s.c_str(), unit.tag.c_str(), unit.weight);
}

struct Dag {
  uint16_t rune;
  LocalVector<pair<size_t, const DictUnit*> > nexts;
  const DictUnit * pInfo;
  double weight;
  size_t nextPos;
  Dag():rune(0), pInfo(NULL), weight(0.0), nextPos(0) {
  }
};

typedef Rune TrieKey;

class TrieNode {
 public :
  TrieNode(): next(NULL), ptValue(NULL) {
  }
 public:
  typedef HashMap<TrieKey, TrieNode*> NextMap;
  NextMap *next;
  const DictUnit *ptValue;
};

class Trie {
 public:
  Trie(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers)
   : root_(new TrieNode) {
    CreateTrie(keys, valuePointers);
  }
  ~Trie() {
    DeleteNode(root_);
  }

  const DictUnit* Find(Unicode::const_iterator begin, Unicode::const_iterator end) const {
    if (begin == end) {
      return NULL;
    }

    const TrieNode* ptNode = root_;
    TrieNode::NextMap::const_iterator citer;
    for (Unicode::const_iterator it = begin; it != end; it++) {
      if (NULL == ptNode->next) {
        return NULL;
      }
      citer = ptNode->next->Find(*it);
      if (ptNode->next->End() == citer) {
        return NULL;
      }
      ptNode = citer->second;
    }
    return ptNode->ptValue;
  }

  void Find(Unicode::const_iterator begin, 
        Unicode::const_iterator end, 
        vector<struct Dag>&res, 
        size_t max_word_len = MAX_WORD_LENGTH) const {
    assert(root_ != NULL);
    res.resize(end - begin);

    const TrieNode *ptNode = NULL;
    TrieNode::NextMap::const_iterator citer;
    for (size_t i = 0; i < size_t(end - begin); i++) {
      Rune rune = *(begin + i);
      res[i].rune = rune;

      if (root_->next != NULL && root_->next->End() != (citer = root_->next->Find(rune))) {
        ptNode = citer->second;
      } else {
        ptNode = NULL;
      }
      if (ptNode != NULL) {
        res[i].nexts.push_back(pair<size_t, const DictUnit*>(i, ptNode->ptValue));
      } else {
        res[i].nexts.push_back(pair<size_t, const DictUnit*>(i, static_cast<const DictUnit*>(NULL)));
      }

      for (size_t j = i + 1; j < size_t(end - begin) && (j - i + 1) <= max_word_len; j++) {
        if (ptNode == NULL || ptNode->next == NULL) {
          break;
        }
        citer = ptNode->next->Find(*(begin + j));
        if (ptNode->next->End() == citer) {
          break;
        }
        ptNode = citer->second;
        if (NULL != ptNode->ptValue) {
          res[i].nexts.push_back(pair<size_t, const DictUnit*>(j, ptNode->ptValue));
        }
      }
    }
  }

  void InsertNode(const Unicode& key, const DictUnit* ptValue) {
    if (key.begin() == key.end()) {
      return;
    }

    TrieNode::NextMap::const_iterator kmIter;
    TrieNode *ptNode = root_;
    for (Unicode::const_iterator citer = key.begin(); citer != key.end(); ++citer) {
      if (NULL == ptNode->next) {
        ptNode->next = new TrieNode::NextMap;
      }
      kmIter = ptNode->next->Find(*citer);
      if (ptNode->next->End() == kmIter) {
        TrieNode *nextNode = new TrieNode;

        ptNode->next->Insert(make_pair(*citer, nextNode));
        ptNode = nextNode;
      } else {
        ptNode = kmIter->second;
      }
    }
    assert(ptNode != NULL);
    ptNode->ptValue = ptValue;
  }

 private:
  void CreateTrie(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers) {
    if (valuePointers.empty() || keys.empty()) {
      return;
    }
    assert(keys.size() == valuePointers.size());

    for (size_t i = 0; i < keys.size(); i++) {
      InsertNode(keys[i], valuePointers[i]);
    }
  }

  void DeleteNode(const TrieNode* node) {
    if (NULL == node) {
      return;
    }
    if (NULL != node->next) {
      for (TrieNode::NextMap::const_iterator it = node->next->Begin(); it != node->next->End(); ++it) {
        DeleteNode(it->second);
      }
      delete node->next;
    }
    delete node;
  }

  TrieNode* root_;
}; // class Trie
} // namespace cppjieba

#endif // CPPJIEBA_TRIE_HPP
