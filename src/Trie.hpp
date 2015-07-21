#ifndef CPPJIEBA_TRIE_HPP
#define CPPJIEBA_TRIE_HPP

#include "Limonp/StdExtension.hpp"
#include <vector>
#include <queue>

namespace CppJieba {
using namespace std;

struct DictUnit {
  Unicode word;
  double weight;
  string tag;
};

// for debugging
inline ostream & operator << (ostream& os, const DictUnit& unit) {
  string s;
  s << unit.word;
  return os << string_format("%s %s %.3lf", s.c_str(), unit.tag.c_str(), unit.weight);
}

typedef LocalVector<std::pair<size_t, const DictUnit*> > DagType;

struct SegmentChar {
  uint16_t uniCh;
  DagType dag;
  const DictUnit * pInfo;
  double weight;
  size_t nextPos;
  SegmentChar() : uniCh(), pInfo(NULL), weight(0.0), nextPos(0) {}
  ~SegmentChar() {}
};

typedef Unicode::value_type TrieKey;

class TrieNode {
 public :
  TrieNode(): next(NULL), ptValue(NULL) {}
 public:
  typedef unordered_map<TrieKey, TrieNode*> NextMap;
  NextMap *next;
  const DictUnit *ptValue;
};

class Trie {
 public:
  static const size_t BASE_SIZE = (1 << (8 * (sizeof(TrieKey))));
 public:
  Trie(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers) {
    _createTrie(keys, valuePointers);
  }
  const DictUnit* find(Unicode::const_iterator begin, Unicode::const_iterator end) const {
    if (begin == end) {
      return NULL;
    }

    const TrieNode* ptNode = _base + (*(begin++));
    TrieNode::NextMap::const_iterator citer;
    for (Unicode::const_iterator it = begin; it != end; it++) {
      if (NULL == ptNode->next) {
        return NULL;
      }
      citer = ptNode->next->find(*it);
      if (ptNode->next->end() == citer) {
        return NULL;
      }
      ptNode = citer->second;
    }
    return ptNode->ptValue;
  }

  void find(
    Unicode::const_iterator begin,
    Unicode::const_iterator end,
    vector<struct SegmentChar>& res
  ) const {
    res.resize(end - begin);

    const TrieNode *ptNode = NULL;
    TrieNode::NextMap::const_iterator citer;
    for (size_t i = 0; i < size_t(end - begin); i++) {
      Unicode::value_type ch = *(begin + i);
      ptNode = _base + ch;
      res[i].uniCh = ch;
      assert(res[i].dag.empty());

      res[i].dag.push_back(DagType::value_type(i, ptNode->ptValue));

      for (size_t j = i + 1; j < size_t(end - begin); j++) {
        if (ptNode->next == NULL) {
          break;
        }
        citer = ptNode->next->find(*(begin + j));
        if (ptNode->next->end() == citer) {
          break;
        }
        ptNode = citer->second;
        if (NULL != ptNode->ptValue) {
          res[i].dag.push_back(DagType::value_type(j, ptNode->ptValue));
        }
      }
    }
  }
  bool find(
    Unicode::const_iterator begin,
    Unicode::const_iterator end,
    DagType & res,
    size_t offset = 0) const {
    if (begin == end) {
      return !res.empty();
    }

    const TrieNode* ptNode = _base + (*(begin++));
    if (ptNode->ptValue != NULL && res.size() == 1) {
      res[0].second = ptNode->ptValue;
    } else if (ptNode->ptValue != NULL) {
      res.push_back(DagType::value_type(offset, ptNode->ptValue));
    }

    TrieNode::NextMap::const_iterator citer;
    for (Unicode::const_iterator itr = begin; itr != end; itr++) {
      if (NULL == ptNode->next) {
        break;
      }
      citer = ptNode->next->find(*itr);
      if (citer == ptNode->next->end()) {
        break;
      }
      ptNode = citer->second;
      if (NULL != ptNode->ptValue) {
        res.push_back(DagType::value_type(itr - begin + offset, ptNode->ptValue));
      }
    }
    return !res.empty();
  }
  ~Trie() {
    for (size_t i = 0; i < BASE_SIZE; i++) {
      if (_base[i].next == NULL) {
        continue;
      }
      for (TrieNode::NextMap::iterator it = _base[i].next->begin(); it != _base[i].next->end(); it++) {
        _deleteNode(it->second);
        it->second = NULL;
      }
      delete _base[i].next;
      _base[i].next = NULL;
    }
  }

  void insertNode(const Unicode& key, const DictUnit* ptValue) {
    if (key.begin() == key.end()) {
      return;
    }

    TrieNode::NextMap::const_iterator kmIter;
    Unicode::const_iterator citer= key.begin();
    TrieNode *ptNode = _base + (*(citer++));
    for (; citer != key.end(); citer++) {
      if (NULL == ptNode->next) {
        ptNode->next = new TrieNode::NextMap;
      }
      kmIter = ptNode->next->find(*citer);
      if (ptNode->next->end() == kmIter) {
        TrieNode *nextNode = new TrieNode;

        (*(ptNode->next))[*citer] = nextNode;
        ptNode = nextNode;
      } else {
        ptNode = kmIter->second;
      }
    }
    ptNode->ptValue = ptValue;
  }

 private:
  void _createTrie(const vector<Unicode>& keys, const vector<const DictUnit*>& valuePointers) {
    if (valuePointers.empty() || keys.empty()) {
      return;
    }
    assert(keys.size() == valuePointers.size());

    for (size_t i = 0; i < keys.size(); i++) {
      insertNode(keys[i], valuePointers[i]);
    }
  }

  void _deleteNode(TrieNode* node) {
    if (NULL == node) {
      return;
    }
    if (NULL != node->next) {
      TrieNode::NextMap::iterator it;
      for (it = node->next->begin(); it != node->next->end(); it++) {
        _deleteNode(it->second);
      }
      delete node->next;
      node->next = NULL;
    }
    delete node;
  }

  TrieNode _base[BASE_SIZE];
};
}

#endif
