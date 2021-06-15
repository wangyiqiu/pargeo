#pragma once

#include "pargeo/point.h"
#include "parlay/utilities.h"
#include "parlay/sequence.h"

namespace pargeo {
  namespace dataGen {
    double randDouble(size_t i) {
      return double(parlay::hash64(i)) / double(std::numeric_limits<size_t>::max());
    }
  }

  template<int dim>
  parlay::sequence<pargeo::point<dim>> uniformInPolyPoints(size_t n, size_t shape);

  template<int dim>
  parlay::sequence<pargeo::point<dim>> uniformOnPolyPoints(size_t n, size_t shape, double thickness);

}
