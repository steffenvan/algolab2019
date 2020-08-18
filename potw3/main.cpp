#include <iostream>
#include <vector> 
#include <algorithm> 
#include <array>
#include <map>

using namespace std;

typedef std::vector<long> VI;
typedef std::vector<VI> VVI;

long add_cost(long a, long b) {
  if (a == -1 || b == -1) {
    return -1;
  } else {
    return a + b;
  }
}

long minimum_cost(long a, long b) {
  if (a == -1) {
    return b;
  } else if (b == -1) {
    return a;
  } else {
    return min(a, b);
  }
}

// map<tuple<long, long, bool, bool>, long> cache;
vector<long> cache[3][100000];
// 

long min_cost(long child_index, long parent, bool is_parent_covered, bool is_parent_included, vector<long> &costs, VVI &adj_list) {
  // go through all nodes when the current node is included
  if (child_index == adj_list[parent].size()) {
    if (is_parent_covered) {
      return 0;
    } else {
      return -1;
    }
  }
  // tuple<long, long, bool, bool> key = make_tuple(child_index, parent, is_parent_covered, is_parent_included);
  // her har vi kun kigget på de nuværende børn. 
  int key = is_parent_included ? 2 : is_parent_covered ? 1 : 0;

  if (cache[key][parent][child_index] == -2) {
    long node = adj_list[parent][child_index];
    long best_cost = -1;

    long include_cost = min_cost(0, node, true, true, costs, adj_list);
    include_cost = add_cost(include_cost, min_cost(child_index + 1, parent, true, is_parent_included, costs, adj_list));
    include_cost = add_cost(include_cost, costs[node]);

    // if we exclude the node
    long exclude_cost = min_cost(0, node, is_parent_included, false, costs, adj_list);
    // is_parent_covered -> hvis den allerede er covered af en knude, skal den ikke blive covered igen. 
    exclude_cost = add_cost(exclude_cost, min_cost(child_index + 1, parent, is_parent_covered, is_parent_included, costs, adj_list));

    best_cost = minimum_cost(include_cost, exclude_cost);
    cache[key][parent][child_index] = best_cost;
  }

  return cache[key][parent][child_index];

}

void testcase() {
  int n;
  cin >> n; 
  vector<long> costs;
  VVI adj_list(n);
  int start, end; 
  long cost;
  for (int i = 0; i < n - 1; i++) {
    cin >> start >> end; 
    adj_list[start].push_back(end);
  }
  for (int i = 0; i < n; i++) {
    cin >> cost; 
    costs.push_back(cost);
  }
  for (int i = 0; i < n; i++) { 
    for (int k = 0; k < 3; k++) {
      cache[k][i].assign(adj_list[i].size(), -2);
    }
  }
  long best_cost = -1;
  long include_zero_cost = min_cost(0, 0, true, true, costs, adj_list);
  include_zero_cost = add_cost(include_zero_cost, costs[0]);

  long exclude_zero_cost = min_cost(0, 0, false, false, costs, adj_list);

  best_cost = minimum_cost(include_zero_cost, exclude_zero_cost);
  cout << best_cost << "\n";

}

int main() {
  std::ios_base::sync_with_stdio(false);
  // cout << setiosflags(ios::fixed) << setprecision(0);
  int t; cin >> t; 
  for (int i = 0; i < t; i++) {
    testcase();
  }
}  