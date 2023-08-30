#pragma once

#include "ball_tree.h"
#include "bc_tree.h"
#include <utils/timer.h>

namespace p2h {

template<class DType>
class RunBallTree {
public:
    std::size_t data_num_;
    std::size_t data_dim_;
    float *data_ptr_;
    std::size_t query_num_;
    std::size_t query_dim_;
    float *query_ptr_;

    Ball_Tree<DType> *tree_;
    int leaf_num_;

    RunBallTree(const std::size_t kDataN, const std::size_t kDataD, float *data_p,
                const std::size_t kQueryN, const std::size_t kQueryD, float *query_p, const int kLeafN) : 
                data_num_(kDataN), data_dim_(kDataD), data_ptr_(data_p),
                query_num_(kQueryN), query_dim_(kQueryD), query_ptr_(query_p), leaf_num_(kLeafN) {}

    void build() {
        tree_ = new Ball_Tree(data_num_, data_dim_, leaf_num_, data_ptr_);
    }

    void search (unsigned topk, unsigned cand_num, float ratio) {
        utils::Timer<std::chrono::microseconds> timer;
        timer.reset();
        MinK_List *list = new MinK_List(topk);
        for (std::size_t i=0; i<query_num_; ++i) {
            list->reset();
            int check_k = tree->nns(topk, cand_num, ratio, &query_ptr_[i*query_dim_], list);
        }
        
    }

};

}