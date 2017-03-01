#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include "ofxVpTree.h"

typedef std::chrono::high_resolution_clock hi_clock;

namespace ch = std::chrono;

/*
struct Vector {
  Vector(double x, double y) : x(x), y(y) {}

  inline double length() const { return std::sqrt(length_squared()); }

  inline double length_squared() const { return x * x + y * y; }

  double x, y;
};

inline Vector operator-(const Vector &v1, const Vector &v2) {
  return Vector(v1.x - v2.x, v1.y - v2.y);
}

struct Particle {
  Particle(double x, double y) : pos(x, y) {}

  Particle(const Vector &pos) : pos(pos) {}

  Vector pos;
};

inline double dist(const Particle &p1, const Particle &p2) {
  return (p1.pos - p2.pos).length();
}
*/

/*
 * To use your own points, you need to define the point type
 * and a metric function.
 * In the following example, we define a metric function over
 * strings, the Hamming distance
 */

size_t ham_distance(const std::string& a, const std::string& b) {
  size_t dist = 0;
  for (size_t i = 0; i < std::min(a.size(), b.size()); i++) {
    if (a[i] != b[i]) dist++;
  }
  dist += std::abs((int)a.size() - (int)b.size());
  return dist;
}

int main() {
  const size_t DIMS = 4096;

  ofxVpTree::Default tree;

  std::vector<ofxVpTree::item> particles;

  double dx = 0.1;
  double dy = 0.1;

  auto start = hi_clock::now();
  for (int x = 0; x < 600; x++) {
    ofxVpTree::item stuff(DIMS);

    for (size_t i = 0; i < DIMS; i++) {
      stuff[i] = (float)rand() + (float)rand() * 500 + (float)rand();
    }

    particles.push_back(stuff);
  }
  auto end = hi_clock::now();
  std::cout << "creating particles took: "
            << ch::duration_cast<ch::milliseconds>((end - start)).count()
            << " milliseconds" << std::endl;
  start = hi_clock::now();
  tree.create(particles);
  end = hi_clock::now();
  std::cout << "creating tree took: "
            << ch::duration_cast<ch::milliseconds>((end - start)).count()
            << " milliseconds" << std::endl;

  std::cout << particles.size() << " particles inserted" << std::endl;

  int k = 10;

  start = hi_clock::now();
  //#pragma omp parallel for
  size_t N = 100;
  for (size_t i = 0; i < N; i++) {
    // std::vector<float> distances;
    std::vector<ofxVpTree::item> neighbors;
    neighbors = tree.search(particles[i], k);
  }
  end = hi_clock::now();
  std::cout
      << "searching neighbors for " << N << " particles took: "
      << ch::duration_cast<ch::milliseconds>((end - start)).count() << " ("
      << (float)ch::duration_cast<ch::milliseconds>((end - start)).count() / N
      << " avg per particle)"
      << " milliseconds" << std::endl;

  /*
   * format: VpTree<point type, scalar type, metric function>
   */
  ofxVpTree::VpTree<std::string, size_t, ham_distance> string_tree;

  std::vector<std::string> strings{"hello", "bye", "abcd", "aman"};

  string_tree.create(strings);

  auto results_2 = string_tree.search(strings[0], 2);
  for (const auto& res : results_2) {
    std::cout << res << " ";
  }
  std::cout << std::endl;
  return 0;
}
