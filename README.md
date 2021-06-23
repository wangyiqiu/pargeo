## Pargeo: A Library for Parallal Algorithms in Computational Geometry

Pargeo is being developted at MIT. It hosts a growing set of multi-core implementations for parallel algorithms in computational geometry:

* Graph Generators
  * Euclidean MST
  * K-Nearest Neighbor Graph
  * Planar Beta-Skeleton
  * Planar Delaunay Graph
  * Planar Gabriel Graph
  * T-Spanner
* Spatial Query
  * K-Nearest Neighbor
  * Range Search
* Closest Pair
* 2D & 3D Z-Order Sort
* Planar Delaunay Triangulation
* Well-Separated Pair Decomposition

## Depdendencies

Pargeo runs on any modern x86-based multicore machines. To compile, it requires g++ 5.4.0 or later. The build system is [CMake](https://cmake.org/install/). 

Pargeo uses the [parlaylib](https://github.com/cmuparlay/parlaylib), a wonderful library developed at CMU for multi-core parallel programming. It and other dependencies are included in the project as submodules - initialize them before compiling the code:

```
git submodule init
git submodule update
```

## Compilation

Starting from the project root directory:

```
mkdir build
cd build
cmake ..
make -j # this will take a while
```

## Usage

After the build completes, navigate to `<root>/build/executable`, where the executables are generated. Running `./<executable>` in the terminal displays short usage instructions. As an example, to compute the Euclidean MST of a point data set `<data-file>`, input the following, and the MST edges will be written to `edges.txt`.

```
./emst -o edges.txt <data-file>
```

Example data sets can be found [here](https://github.com/wangyiqiu/pargeo/tree/main/test/datasets). Pargeo automatically parses CSV-like point data files with or without header, as long as the delimiters (space, comma, etc) in between numbers are one character each in length.

## References

[1] [Yiqiu Wang, Shangdi Yu, Laxman Dhulipala, Yan Gu, and Julian Shun. 2021. GeoGraph: A Framework for Graph Processing on Geometric Data. SIGOPS Operating Systems Review 55, 1 (July 2021), 38–46.](https://dl.acm.org/doi/abs/10.1145/3469379.3469384)

[2] [Yiqiu Wang, Shangdi Yu, Yan Gu, and Julian Shun. 2021. Fast Parallel Algorithms for Euclidean Minimum Spanning Tree and Hierarchical Spatial Clustering. In Proceedings of the 2021 International Conference on Management of Data (SIGMOD/PODS '21). Association for Computing Machinery, New York, NY, USA, 1982–1995.](https://arxiv.org/abs/2104.01126)

[3] [Yiqiu Wang, Shangdi Yu, Yan Gu, and Julian Shun. 2021. A Parallel Batch-Dynamic Data Structure for the Closest Pair Problem. 37th International Symposium on Computational Geometry (SoCG 2021).](https://arxiv.org/abs/2010.02379)
