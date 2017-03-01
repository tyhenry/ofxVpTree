# ofxVpTree
----
Turning [vp-tree](https://github.com/gregorburger/vp-tree) into an openFrameworks addon.

----
## Usage
```c++
const size_t DIMS = 1024;

ofxVpTree::Default vp_tree;

/* ofxVpTree::item == Eigen::VectorXf */
std::vector<ofxVpTree::item> items;

for(size_t i = 0; i < 1024 * 10; i++) {
	ofxVpTree::item temp(DIMS);
	for(size_t j = 0; j < DIMS; j++) {
		temp[j] = /* some values */
	}
}

vp_tree.create(items);

/* vp_tree.search(search_point, num_neighbors) */
std::vector<ofxVpTree::item> results = vp_tree.search(items[0], 16);

/*
 * To use your own points, you need to define the point type
 * and a metric function.
 * In the following example, we define a metric function over
 * strings, the Hamming distance
 */

size_t ham_distance(const std::string& a, const std::string& b) {
	size_t dist = 0;
	for(size_t i = 0; i < std::min(a.size(), b.size()); i++) {
		if(a[i] != b[i]) dist++;
	}
	dist += std::max(a.size(), b.size()) - std::min(a.size(), b.size());
	return dist;
}

/*
 * format: VpTree<point type, scalar type, metric function>
 * Default == ofxVpTree::<Eigen::VectorXf, Eigen::VectorXf::Scalar, ofxVpTree::vec_euclidean_distance 
 */
 
ofxVpTree::VpTree<std::string, size_t, ham_distance> string_tree;

std::vector<std::string> strings {"hello", "bye", "abcd", "aman"};

string_tree.create(strings);

auto results_2 = string_tree.search("bUh", 1);
