#include <iostream> 
#include <set> 
#include <algorithm>
#include <vector>
#include <deque> 
#include <cmath> 
#include <numeric>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

VVI memo_table(2501, VI(2501, -1)); 
vector<int> values;
int k; 
int m;

int best_coins(int start, int end, int player) {
  if (start > end) {
    return 0;
  }

  if (memo_table[start][end] == -1) {
    player %= m;
    if (player == k) {
      memo_table[start][end] = max(best_coins(start + 1, end, player + 1) + values[start], 
                                   best_coins(start, end - 1, player + 1) + values[end]);
    } else {
      memo_table[start][end] = min(best_coins(start + 1, end, player + 1), 
                                   best_coins(start, end - 1, player + 1));
    }
  }
  
  return  memo_table[start][end];
}

void testcase() {
  int n, v; 
  values.clear();
  for (int i = 0; i < memo_table.size(); i++) {
    for (int j = 0; j < memo_table.size(); j++) {
      memo_table[i][j] = -1;
    }
  }

  cin >> n >> m >> k;
  for (int i = 0; i < n; i++) {
    cin >> v; 
    values.push_back(v);
  }

  int best_value = 0;
  int start = 0;
  int end = values.size() - 1;
  int player = 0;
  best_value = best_coins(start, end, player);
  cout << best_value << endl;
}



int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t; 
  for (int i = 0; i < t; i++) {
    testcase();
    // VVI memo_table(501, VI(1001, -1)); 
  }
}