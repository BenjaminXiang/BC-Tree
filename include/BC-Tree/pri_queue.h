#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>

#include "def.h"

namespace p2h {

// -----------------------------------------------------------------------------
//  struct Result
// -----------------------------------------------------------------------------
struct Result {                     // basic structure
    float key_;                         // distance / random projection value
    int   id_;                          // object id
};

// -----------------------------------------------------------------------------
inline int cmp(                     // cmp func for lower_bound (ascending)
    const Result &a,                    // 1st element
    const Result &b)                    // 2nd element
{
    return a.key_ < b.key_;
}

// -----------------------------------------------------------------------------
int ResultComp(                     // compare function for qsort (ascending)
    const void *e1,                     // 1st element
    const void *e2);                    // 2nd element

// -----------------------------------------------------------------------------
int ResultCompDesc(                 // compare function for qsort (descending)
    const void *e1,                     // 1st element
    const void *e2);                    // 2nd element


// -----------------------------------------------------------------------------
//  MinK_List: a structure which maintains the smallest k values (of type float)
//  and associated object id (of type int).
//
//  This structure is used for ANNS
// -----------------------------------------------------------------------------
class MinK_List {
public:
    MinK_List(int max);             // constructor (given max size)
    ~MinK_List();                   // destructor

    // -------------------------------------------------------------------------
    inline void reset() { num_ = 0; }

    // -------------------------------------------------------------------------
    inline float min_key() const { return num_ > 0 ? list_[0].key_ : MAXREAL; }

    // -------------------------------------------------------------------------
    inline float max_key() const { return num_ >= k_ ? list_[k_-1].key_ : MAXREAL; }

    // -------------------------------------------------------------------------
    inline float ith_key(int i) const { return i < num_ ? list_[i].key_ : MAXREAL; }

    // -------------------------------------------------------------------------
    inline int ith_id(int i) const { return i < num_ ? list_[i].id_ : MININT; }

    // -------------------------------------------------------------------------
    inline int size() const { return num_; }

    // -------------------------------------------------------------------------
    inline bool isFull() const { if (num_ >= k_) return true; else return false; }

    // -------------------------------------------------------------------------
    float insert(                   // insert item
        float key,                      // key of item
        int   id);                      // id of item

protected:
    int    k_;                      // max numner of keys
    int    num_;                    // number of key current active
    Result *list_;                  // the list itself
};


// -----------------------------------------------------------------------------
//  MaxK_List: An MaxK_List structure is one which maintains the largest k 
//  values (of type float) and associated object id (of type int).
//
//  This structure is used for MIPS
// -----------------------------------------------------------------------------
class MaxK_List {
public:
    MaxK_List(int max);             // constructor (given max size)
    ~MaxK_List();                   // destructor

    // -------------------------------------------------------------------------
    inline void reset() { num_ = 0; }

    // -------------------------------------------------------------------------
    inline float max_key() { return num_ > 0 ? list_[0].key_ : MINREAL; }

    // -------------------------------------------------------------------------
    inline float min_key() { return num_ >= k_ ? list_[k_-1].key_ : MINREAL; }

    // -------------------------------------------------------------------------
    inline float ith_key(int i) { return i < num_ ? list_[i].key_ : MINREAL; }

    // -------------------------------------------------------------------------
    inline int ith_id(int i) { return i < num_ ? list_[i].id_ : MININT; }

    // -------------------------------------------------------------------------
    inline int size() { return num_; }

    // -------------------------------------------------------------------------
    inline bool isFull() { if (num_ >= k_) return true; else return false; }

    // -------------------------------------------------------------------------
    float insert(                   // insert item
        float key,                      // key of item
        int   id);                      // id of item

protected:
    int    k_;                      // max numner of keys
    int    num_;                    // number of key current active
    Result *list_;                  // the list itself
};

} // end namespace p2h
