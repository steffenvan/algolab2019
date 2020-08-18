#include<iostream>
#include<vector>
#include<limits>
#include<algorithm>
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/dijkstra_shortest_paths.hpp>
#include<boost/graph/strong_components.hpp>
#include<boost/graph/transpose_graph.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace boost;
using namespace std;
typedef boost::adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::property_map<weighted_graph, edge_weight_t>::type weight_maps;
typedef property_map<weighted_graph, vertex_index_t>::type index_maps;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;


void print_graph(weighted_graph G, weight_maps weights) {
  edge_it e_beg, e_end;
  for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; ++e_beg) {
    std::cout << boost::source(*e_beg, G) << " --> " << boost::target(*e_beg, G) << " " << weights[*e_beg] << "\n";
  }
}
     
void testcase() {
  int n, m, k, T;
  cin >> n >> m >> k >> T; 
  weighted_graph G(n);
  weight_maps edge_weights = boost::get(edge_weight, G);
  std::vector<int> scc_map(n);
  std::vector<int> tele_net;

  std::unordered_map<int, int> tele_costs(T);
  std::unordered_map<int, int> node_to_scc(T);

  for (int i = 0; i < T; i++) {
    int t; cin >> t; 
    tele_net.push_back(t);
  }
 
  for (int i = 0; i < m; i ++) {
    int from, to, cost; 
    cin >> from >> to >> cost; 
    bool success;
    edge_desc edge;
    boost::tie(edge, success) = boost::add_edge(from, to, G); edge_weights[edge] = cost;
  }

  boost::strong_components(G,
    boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));

  // Count number of nodes in each SCC, i.e the cost for each scc group.
  for (auto& tele_node : tele_net) {
    tele_costs[scc_map[tele_node]]++;
  }

  // Add teleportation node to its scc group. 
  for (int i = 0; i < T; i++) {
    int tele_node = tele_net[i];
    int scc_group = scc_map[tele_node];
    node_to_scc[tele_node] = scc_group;
  }

  // loop over all teleport nodes and add teleport nodes within same SCC group to a proxy node. 
  for (int i = 0; i < T; i++) {
    int tele_node = tele_net[i];
    if (scc_map[tele_node] == node_to_scc[tele_node]) {
      bool success;
      edge_desc edge;
      boost::tie(edge, success) = boost::add_edge(tele_node, n + scc_map[tele_node], G); edge_weights[edge] = tele_costs[scc_map[tele_node]] - 1;
      boost::tie(edge, success) = boost::add_edge(n + scc_map[tele_node], tele_node, G); edge_weights[edge] = 0;
    }
  }

  weighted_graph G_T;
  transpose_graph(G, G_T);
  int n_T = num_vertices(G_T);
  std::vector<int> dist_map(n_T);

  boost::dijkstra_shortest_paths(G_T, n - 1,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G_T))));

  int best_dist = numeric_limits<int>::max();
  for (int i = 0; i < k; i++) {
    best_dist = min(best_dist, dist_map[i]);
  }

  if (best_dist < 1000000) {
    cout << best_dist << '\n';
  } else {
    cout << "no" << '\n';
  }
}

int main() {
  ios_base::sync_with_stdio(false);  
  int t; cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}