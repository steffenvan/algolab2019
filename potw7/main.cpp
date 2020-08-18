#include<iostream>
#include <algorithm>
// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> flow_graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;


class edge_adder {
  flow_graph &G;
  
public:
  explicit edge_adder(flow_graph &G) : G(G) {}

  void edge_add(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int zones, jobs; cin >> zones >> jobs;
  // + 2 for source and sink. 
  const int graph_size = zones + jobs; 
  flow_graph G(graph_size);
  edge_adder adder(G);

  const int source = boost::add_vertex(G);
  const int sink = boost::add_vertex(G);

  int max_cost = 10000;

  for (int zone = 0; zone < zones; zone++) {
    int zone_cost; 
    cin >> zone_cost;
    adder.edge_add(jobs + zone, sink, zone_cost); 
  }

  int total_reward = 0;

  for (int job = 0; job < jobs; job++) {
    int job_reward; 
    cin >> job_reward;
    total_reward += job_reward;
    adder.edge_add(source, job, job_reward);
  }

  for (int i = 0; i < jobs; i++) {
    int zonesNeeded;
    cin >> zonesNeeded;
    for (int j = 0; j < zonesNeeded; j++) {
      int zone;
      cin >> zone;
      adder.edge_add(i, zone + jobs, max_cost);
    }
  }

  long cost = push_relabel_max_flow(G, source, sink);
  cout << total_reward - cost << "\n"; 

}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t; 
  for (int i = 0; i < t; i++) {
    testcase();
  }
}

