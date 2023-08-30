#include <BC-Tree/ball_tree.h>
#include <BC-Tree/bc_tree.h>

namespace p2h {

template<class DType>
Ball_Tree<DType>* build_ball_tree(const std::size_t kDataNum, const std::size_t kDataDim, const unsigned leaf,
                                  const float *kData) {
    Ball_Tree<DType> *tree = new Ball_Tree<DType>(kDataNum, kDataDim, leaf, kData);
    return tree;
}

template<class DType>
void run_ball_tree() {

}

}

