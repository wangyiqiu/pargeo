/* This data generator is partially adapted from the
   Problem Based Benchmark Suite
   https://github.com/cmuparlay/pbbsbench
*/

#include <string>
#include <limits>

#include "parlay/parallel.h"
#include "parlay/utilities.h"
#include "pargeo/point.h"
#include "pargeo/pointIO.h"
#include "pargeo/parseCommandLine.h"

using namespace parlay;
using namespace pargeo;
using namespace pargeo::pointIO;

namespace pargeo {

  namespace dataGen {

    using floatT = double;

    double randDouble(size_t i) {
      return double(parlay::hash64(i)) / double(std::numeric_limits<size_t>::max());
    }

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
      return (v/v.dist(origin))*scale;
    }

  } // End namespace dataGen

  template<int dim>
  sequence<point<dim>> uniformPoints(size_t n, size_t type) {
    using namespace dataGen;

    auto P = sequence<point<dim>>(n);
    parallel_for (0, n, [&](size_t i) {
			  if (type == 0) P[i] = randInUnitSphere<dim>(i, sqrt(floatT(n)));
			  else if (type == 1) P[i] = randOnUnitSphere<dim>(i, n);
			  else if (type == 2) P[i] = randNd<dim>(i, sqrt(double(n)));
			  else throw std::runtime_error("generator not implemented yet");
			});

    // data should be already permuted
    return P;
  }

} // End namespace pargeo

template<int dim>
void uniformGenerator(size_t n, size_t type, char* fName) {
  auto P = pargeo::uniformPoints<dim>(n, type);
  pargeo::pointIO::writePointsToFile(P, fName);
}

int main(int argc, char* argv[]) {
  string text = "[-s] [-S] [-c] [-C] [-d {2--9}] n <outFile>";
  text += "\n -s: in sphere";
  text += "\n -S: on sphere";
  text += "\n -c: in cube";
  text += "\n -C: on cube";
  text += "\n default : in cube";
  commandLine P(argc, argv, text);

  pair<size_t, char*> in = P.sizeAndFileName();
  size_t n = in.first;
  char* fName = in.second;

  int dims = P.getOptionIntValue("-d", 2);
  bool inSphere = P.getOption("-s");
  bool onSphere = P.getOption("-S");
  bool inCube = P.getOption("-c");
  bool onCube = P.getOption("-C");

  size_t type;
  if (inSphere) type = 0;
  else if (onSphere) type = 1;
  else if (inCube) type = 2;
  else if (onCube) type = 3;
  else type = -1;

  if (dims == 2) uniformGenerator<2>(n, type, fName);
  else if (dims == 3) uniformGenerator<3>(n, type, fName);
  else if (dims == 4) uniformGenerator<4>(n, type, fName);
  else if (dims == 5) uniformGenerator<5>(n, type, fName);
  else if (dims == 6) uniformGenerator<6>(n, type, fName);
  else if (dims == 7) uniformGenerator<7>(n, type, fName);
  else if (dims == 8) uniformGenerator<8>(n, type, fName);
  else if (dims == 9) uniformGenerator<9>(n, type, fName);
  else throw std::runtime_error("dimensionality not yet supported");

  return 0;
}
