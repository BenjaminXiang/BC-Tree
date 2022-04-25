#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <numeric>
#include <cstddef>

#include "def.h"
#include "util.h"
#include "pri_queue.h"
#include "bc_node.h"

namespace p2h {

// -----------------------------------------------------------------------------
//  BC_Tree is a data structure that maintains a ball structure for internal 
//  nodes and a joint structure of ball & cone for leaf nodes.
//  
//  It can be used for Point-to-Hyperplane NNS.
// -----------------------------------------------------------------------------
template<class DType>
class BC_Tree {
public:
    int   n_;                       // number of data points
    int   d_;                       // dimension of data points
    int   leaf_;                    // leaf size of bc-tree
    const DType *data_;             // data points
    
    int   *index_;                  // data index
    BC_Node<DType> *root_;          // root node of bc-tree
    
    // -------------------------------------------------------------------------
    BC_Tree(                        // constructor
        int   n,                        // number of data points
        int   d,                        // dimension of data points
        int   leaf,                     // leaf size of bc-tree
        const DType *data);             // data points
    
    // -------------------------------------------------------------------------
    ~BC_Tree();                     // destructor
    
    // -------------------------------------------------------------------------
    void display();                 // display bc-tree
    
    // -------------------------------------------------------------------------
    int nns(                        // point-to-hyperplane search on bc-tree
        int   top_k,                    // top_k value
        int   cand,                     // number of candidates
        float c,                        // approximation ratio
        const float *query,             // input query
        MinK_List *list);               // top-k results (return)
    
    // -------------------------------------------------------------------------
    void traversal(                 // traversal bc-tree to get leaf info
        std::vector<int> &leaf_size,    // leaf size (return)
        std::vector<int> &index);       // data index with leaf order (return)
    
    // -------------------------------------------------------------------------
    u64 get_memory_usage() {
        u64 ret = 0UL;
        ret += sizeof(*this);
        ret += sizeof(int)*n_; // index_
        ret += root_->get_memory_usage(); // memory of bc-nodes
        return ret;
    }

protected:
    // -------------------------------------------------------------------------
    BC_Node<DType>* build(          // build a bc-node
        int n,                          // number of data points
        int *index);                    // data index (return)

    // -------------------------------------------------------------------------
    int find_furthest_id(           // find furthest data id
        int from,                       // input data id
        int n,                          // number of data index
        int *index);                    // data index
};

// -----------------------------------------------------------------------------
template<class DType>
BC_Tree<DType>::BC_Tree(            // constructor
    int   n,                            // number of data points
    int   d,                            // dimension of data points
    int   leaf,                         // leaf size of bc-tree
    const DType *data)                  // data points
    : n_(n), d_(d), leaf_(leaf), data_(data)
{
    index_ = new int[n];
    
    int i = 0;
    std::iota(index_, index_+n, i++);
    root_ = build(n, index_);
}

// -----------------------------------------------------------------------------
template<class DType>
BC_Node<DType>* BC_Tree<DType>::build(// build a bc-node 
    int n,                              // number of data points
    int *index)                         // data index (return)
{
    BC_Node<DType> *cur = nullptr;
    if (n <= leaf_) {
        // build leaf node
        cur = new BC_Node<DType>(n, d_, true, nullptr, nullptr, index, data_);
    }
    else {
        DType *w = new DType[d_];
        int left = 0, right = n-1, cnt=0;
        do {
            // build internal node
            int x_p = rand() % n;
            int l_p = find_furthest_id(x_p, n, index);
            int r_p = find_furthest_id(l_p, n, index);
            assert(l_p != r_p);
            // printf("x=%d, l=%d, r=%d\n", x_p, l_p, r_p);
            
            // note: we use l_p and r_p as two pivots
            const DType *l_pivot = data_ + (u64) index[l_p]*d_;
            const DType *r_pivot = data_ + (u64) index[r_p]*d_;
            float l_sqr = 0.0f, r_sqr = 0.0f;
            for (int i = 0; i < d_; ++i) {
                DType l_v = l_pivot[i], r_v = r_pivot[i];
                w[i] = r_v - l_v; l_sqr += SQR(l_v); r_sqr += SQR(r_v);
            }
            float b = 0.5f * (l_sqr - r_sqr);
    
            left = 0, right = n-1;
            while (left <= right) {
                const DType *x = data_ + (u64) index[left]*d_;
                float val = calc_inner_product<DType>(d_, w, x) + b;
                if (val < 0.0f) ++left;
                else { SWAP(index[left], index[right]); --right; }
            }
            // if (left <= 0 || left >= n) {
            //     printf("n=%d, left=%d, right=%d\n", n, left, right);
            // }
            ++cnt;
        } while ((left <= 0 || left >= n) && (cnt <= 3)); // ensure split into 2
        if (cnt > 3) left = n/2; // meet the case that cannot split, force split
        delete[] w;

        BC_Node<DType> *lc = build(left,   index);
        BC_Node<DType> *rc = build(n-left, index+left);
        cur = new BC_Node<DType>(n, d_, false, lc, rc, index, data_);
    }
    return cur;
}

// -----------------------------------------------------------------------------
template<class DType>
int BC_Tree<DType>::find_furthest_id(// find furthest data id
    int from,                           // input id
    int n,                              // number of data index
    int *index)                         // data index
{
    int   far_id   = -1;
    float far_dist = -1.0f;

    const DType *query = data_ + (u64) index[from]*d_;
    for (int i = 0; i < n; ++i) {
        if (i == from) continue;
        
        const DType *point = data_ + (u64) index[i]*d_;
        float dist = calc_l2_sqr<DType>(d_, point, query);
        if (far_dist < dist) { far_dist = dist; far_id = i; }
    }
    return far_id;
}

// -----------------------------------------------------------------------------
template<class DType>
BC_Tree<DType>::~BC_Tree()          // desctructor
{
    if (root_  != nullptr) { delete   root_;  root_  = nullptr; }
    if (index_ != nullptr) { delete[] index_; index_ = nullptr; }
}

// -----------------------------------------------------------------------------
template<class DType>
void BC_Tree<DType>::display()      // display bc-tree
{
    std::vector<int> leaf_size;
    root_->traversal(leaf_size);
        
    printf("Parameters of BC_Tree:\n");
    printf("n        = %d\n", n_);
    printf("d        = %d\n", d_);
    printf("leaf     = %d\n", leaf_);
    printf("# leaves = %d\n", (int) leaf_size.size());
    // for (int leaf : leaf_size) printf("%d\n", leaf);
    printf("\n");
    std::vector<int>().swap(leaf_size);
}

// -----------------------------------------------------------------------------
template<class DType>
int BC_Tree<DType>::nns(            // point-to-hyperplane search on bc-tree
    int   top_k,                        // top_k value
    int   cand,                         // number of candidates
    float c,                            // approximation ratio
    const float *query,                 // input query
    MinK_List *list)                    // top-k results (return)
{
    cand = std::min(cand+top_k-1, n_);
    
    float norm_q = sqrt(calc_inner_product<float>(d_, query, query));
    float ip = calc_inner_product<float>(d_, root_->center_, query);
    
    int total = cand;
    root_->nns(c, ip, norm_q, query, cand, list);
    return total - cand;
}

// -----------------------------------------------------------------------------
template<class DType>
void BC_Tree<DType>::traversal(     // traversal bc-tree to get leaf info
    std::vector<int> &leaf_size,        // leaf size (return)
    std::vector<int> &index)            // data index with leaf order (return)
{
    for (int i = 0; i < n_; ++i) index[i] = index_[i];
    root_->traversal(leaf_size);
}

} // end namespace p2h
