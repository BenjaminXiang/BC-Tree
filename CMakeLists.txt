file(GLOB_RECURSE srcs CONFIGURE_DEPENDS include/*.h src/*.cc)

add_library(BC-Tree STATIC ${srcs})
target_include_directories(BC-Tree PUBLIC include)
target_compile_options(BC-Tree PUBLIC -std=c++17 -fopenmp -march=native -ftree-vectorize -ftree-vectorizer-verbose=0)
# target_link_libraries(hnswlib PUBLIC utils)


# add_executable(bc-main main.cc)
# target_link_libraries(bc-main BC-Tree)
# target_link_options(bc-main PUBLIC -fopenmp)
# target_compile_options(bc-main PUBLIC -std=c++17 -fopenmp -march=native -ftree-vectorize -ftree-vectorizer-verbose=0)
