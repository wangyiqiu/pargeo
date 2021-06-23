pybind11_add_module(pypargeo pypargeo.cpp)

target_link_libraries(pypargeo PRIVATE
  pargeoLib
  spatialGraphLib
  delaunay2dLib
  emstLib)
