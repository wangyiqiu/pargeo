/* This data generator is partially adapted from the
   Problem Based Benchmark Suite
   https://github.com/cmuparlay/pbbsbench
*/

#include <string>
#include <limits>

#include "dataset/dataset.h"
#include "parlay/parallel.h"
#include "parlay/utilities.h"
#include "pargeo/point.h"
#include "pargeo/pointIO.h"
#include "pargeo/parseCommandLine.h"

using namespace parlay;

namespace pargeo {

  namespace dataGen {

    using floatT = double;

    template<int dim>
    point<dim> randNd(size_t i, floatT scale=1) {
      size_t s[dim];
      s[0] = i;
      for (int j=1; j<dim; ++j) {
	s[j] = j*i + parlay::hash64(s[j-1]);
      }
      floatT ss[dim];
      for (int j=0; j<dim; ++j) {
	ss[j] = 2 * randDouble(s[j]) - 1;
      }
      return point<dim>(ss) * scale;
    }

    template<int dim>
    point<dim> randInUnitSphere(size_t i, floatT scale=1) {
      auto origin = point<dim>();
      for(int j=0; j<dim; ++j) origin[j] = 0;
      size_t j = 0;
      point<dim> p;
      do {
	size_t o = parlay::hash64(j++);
	p = randNd<dim>(o+i);
      } while (p.dist(origin) > 1.0);
      return p*scale;
    }

    template<int dim>
    point<dim> randOnUnitSphere(size_t i, floatT scale=1) {
      auto origin = point<dim>();
      for(int j=0; j<dim; ++j) origin[j] = 0;
      point<dim> v = randInUnitSphere<dim>(i);
      return (v / v.dist(origin)) * scale;
    }

  } // End namespace dataGen

  template<int dim>
  sequence<point<dim>> uniformInPolyPoints(size_t n, size_t shape) {
    using namespace dataGen;

    auto P = sequence<point<dim>>(n);
    parallel_for (0, n, [&](size_t i) {
			  if (shape == 0) P[i] = randInUnitSphere<dim>(i, sqrt(floatT(n)));
			  else if (shape == 1) P[i] = randNd<dim>(i, sqrt(double(n)));
			  else throw std::runtime_error("generator not implemented yet");
			});

    return P; // data should be already permuted
  }

  template<int dim>
  sequence<point<dim>> uniformOnPolyPoints(size_t n, size_t shape, double thickness) {
    using namespace dataGen;

    auto P = sequence<point<dim>>(n);

    if (shape == 0) {
      floatT r1 = floatT(n) * (1 + thickness);
      floatT r2 = floatT(n) * (1 - thickness);
      floatT a1 = 1; for (int d = 0; d < dim - 1; ++ d) a1 *= r1;
      floatT a2 = 1; for (int d = 0; d < dim - 1; ++ d) a2 *= r2;
      size_t n1 = a1 * n / (a1 + a2);
      size_t n2 = n - n1;
      floatT t1 = 1 - 1 / (1 + thickness);
      floatT t2 = 1 / (1 - thickness) - 1;

      // Outer
      parallel_for (0, n1, [&](size_t i) {
			     floatT s = 1 - t1 * randDouble(i);
			     P[i] = randOnUnitSphere<dim>(i, r1) * s;
			   });

      // Inner
      parallel_for (n1, n, [&](size_t i) {
			     floatT s = t2 * randDouble(i) + 1;
			     P[i] = randOnUnitSphere<dim>(i, r2) * s;
			   });

    } else throw std::runtime_error("generator not implemented yet");

    return P; // data should be already permuted
  }

} // End namespace pargeo

template<int dim>
void uniformGenerator(size_t n, size_t shape, double thickness, char* fName) {
  if (thickness < 0) {
    auto P = pargeo::uniformInPolyPoints<dim>(n, shape);
    pargeo::pointIO::writePointsToFile(P, fName);
  } else {
    auto P = pargeo::uniformOnPolyPoints<dim>(n, shape, thickness);
    pargeo::pointIO::writePointsToFile(P, fName);
  }
}

int main(int argc, char* argv[]) {
  using namespace pargeo;
  std::string text = "[-s] [-c] [-t {double}] [-d {2--9}] n <outFile>";
  text += "\n polygon type: -s sphere -c cube";
  text += "\n generate point on surface: -t thickness";
  text += "\n  o.w. default to generate in polygon";
  commandLine P(argc, argv, text);

  pair<size_t, char*> in = P.sizeAndFileName();
  size_t n = in.first;
  char* fName = in.second;

  int dim = P.getOptionIntValue("-d", 2);
  bool sphere = P.getOption("-s");
  bool cube = P.getOption("-c");
  double thickness = P.getOptionDoubleValue("-t", -1);

  size_t shape = sphere ? 0 : 1;

  if (dim == 2) uniformGenerator<2>(n, shape, thickness, fName);
  else if (dim == 3) uniformGenerator<3>(n, shape, thickness, fName);
  else if (dim == 4) uniformGenerator<4>(n, shape, thickness, fName);
  else if (dim == 5) uniformGenerator<5>(n, shape, thickness, fName);
  else if (dim == 6) uniformGenerator<6>(n, shape, thickness, fName);
  else if (dim == 7) uniformGenerator<7>(n, shape, thickness, fName);
  else if (dim == 8) uniformGenerator<8>(n, shape, thickness, fName);
  else if (dim == 9) uniformGenerator<9>(n, shape, thickness, fName);
  else throw std::runtime_error("dimensionality not yet supported");

  return 0;
}
