#ifndef LIMONP_HASH_MAP_HPP
#define LIMONP_HASH_MAP_HPP

#include <utility>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>

namespace limonp {

static size_t PRIME_NUMBERS[] = {3, 7, 17, 37, 79, 163, 331, 
 673, 1361, 2729, 471, 10949, 
 21911, 43853, 87719, 175447, 350899,
 701819, 1403641, 2807303, 5614657, 11229331, 
 22458671, 44917381, 89834777, 179669557, 359339171, 
 718678369, 1437356741, 2147483647
};

template <class KeyT, class MappedT>
class HashMap {
 private:
  typedef std::pair<KeyT, MappedT> ValueT;

  class LightList {
   public:
    struct Node {
      ValueT value;
      struct Node* next;
    }; // struct Node
  
    LightList()
      : head_(NULL) {
    }
    LightList(const LightList& ll)
      : head_(NULL) {
      struct Node* node = ll.head_;
      while (node != NULL) {
        UniqAppend(node->value);
        node = node->next;
      }
    }
    ~LightList() {
      while (head_ != NULL) {
        Node* x = head_;
        head_ = head_->next;
        delete x;
      }
    }
  
    //O(n)
    std::pair<ValueT&, bool> UniqAppend(const ValueT& x) {
      struct Node** pp = &head_;
      while ((*pp) != NULL) {
        if ((*pp)->value.first == x.first) {
          (*pp)->value.second = x.second;
          return std::pair<ValueT&, bool>((*pp)->value, false);
        }
        pp = &(*pp)->next;
      }
      struct Node* node = new Node;
      node->value = x;
      node->next = NULL;
      (*pp) = node;
      return std::pair<ValueT&, bool>(node->value, true);
    }

    //O(1)
    void PushFront(const ValueT& x) {
      Node* node = new Node;
      node->value = x;
      node->next = head_;
      head_ = node;
    }
   private:
    LightList& operator = (const LightList& ll);

    friend class HashMap;
    friend class Iterator;

    Node* head_;
  }; // class LightList

  template<class BucketsT, class NodeT, class ValueT>
  class Iterator {
   public:
    Iterator() {
    }

    Iterator& operator ++() {
      assert(buckets_ != NULL);
      assert(node_ != NULL);
      node_ = node_->next;
      while (node_ == NULL) {
        ++bucketid_;
        if (bucketid_ >= buckets_->size()) {
          break;
        }
        node_ = (*buckets_)[bucketid_].head_;
      }
      return *this;
    }

    bool operator == (const Iterator& iter) const {
      return node_ == iter.node_;
    }

    bool operator != (const Iterator& iter) const {
      return node_ != iter.node_;
    }

    ValueT* operator -> () {
      assert(node_ != NULL);
      assert(bucketid_ < buckets_->size());
      return &node_->value;
    }
    ValueT& operator * () {
      assert(node_ != NULL);
      assert(bucketid_ < buckets_->size());
      return node_->value;
    }
   private:
    friend class HashMap;
    Iterator(size_t bucketid, BucketsT buckets, NodeT node)
     : bucketid_(bucketid), buckets_(buckets), node_(node) {
    }
    size_t bucketid_;
    BucketsT buckets_;
    NodeT node_;
  }; // class Iterator
  
 public:
  typedef Iterator<const std::vector<LightList>*, const struct LightList::Node*, const ValueT> const_iterator;

  HashMap()
   : size_(0) {
  }
  ~HashMap() {
  }

  size_t Size() const {
    return size_;
  }

  size_t BucketSize() const {
    return buckets_.size();
  }

  bool Insert(const ValueT& v) {
    size_ ++;
    if (size_ > buckets_.size()) {
      const size_t* begin = PRIME_NUMBERS;
      const size_t* end = PRIME_NUMBERS + sizeof(PRIME_NUMBERS)/sizeof(*PRIME_NUMBERS);
      const size_t* cur = std::lower_bound(begin, end, size_);
      if (end != cur) {
        Rehash(*cur);
      } else {
        Rehash(size_);
      }
    }

    assert(buckets_.size() >= size_ && size_ > 0);
    size_t bucketid = Hash(v.first) % buckets_.size();
    bool ok = buckets_[bucketid].UniqAppend(v).second;
    if (!ok) {
      size_ --;
    }
    return ok;
  }

  const_iterator Find(const KeyT& key) const {
    if (size_ == 0) {
      return End();
    }
    assert(buckets_.size() > 0);
    size_t bucketid = Hash(key) % buckets_.size();
    const struct LightList::Node* node = buckets_[bucketid].head_;
    while (node != NULL) {
      if (node->value.first == key) {
        return const_iterator(bucketid, &buckets_, node);
      }
      node = node->next;
    }
    return End();
  }

  const_iterator Begin() const {
    if (buckets_.empty()) {
      return End();
    }
    size_t bucketid = 0;
    assert(bucketid < buckets_.size());
    const struct LightList::Node* node = buckets_[bucketid].head_;
    while (node == NULL && bucketid < buckets_.size()) {
      bucketid ++;
      node = buckets_[bucketid].head_;
    }
    return const_iterator(bucketid, &buckets_, node);
  }
  const_iterator End() const {
    return const_iterator(buckets_.size(), &buckets_, NULL);
  }

  void Rehash(size_t maxsize) {
    assert(maxsize > 0);
    std::vector<LightList> newbuckets(maxsize);
    for (size_t i = 0; i < buckets_.size(); ++i) {
      struct LightList::Node* oldnode = buckets_[i].head_;
      while (oldnode != NULL) {
        size_t bucketid = Hash(oldnode->value.first) % maxsize; //TODO
        newbuckets[bucketid].PushFront(oldnode->value);
        oldnode = oldnode->next;
      }
    }
    buckets_.swap(newbuckets);
  }
 private:
  size_t Hash(KeyT key) const {
    return key;
  }

  std::vector<LightList> buckets_;
  size_t size_;
}; // class HashMap

} // namespace limonp

#endif // LIMONP_HASH_MAP_HPP
