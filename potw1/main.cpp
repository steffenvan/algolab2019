#include <iostream>
#include <vector> 
#include <algorithm> 

void testcase() {
  long n;
  long k; 
  long value;
  std::cin >> n >> k;
  std::vector<int> card_list; 
  for (int i = 0; i < n; ++i) {
    std::cin >> value;
    card_list.push_back(value);
  }
  // std::sort(card_list.begin(), card_list.end());

  int i = 0; int j = 0; int g = 0; int h = 0;
  int smallest_diff = 1000000;
  int curr_diff = 1000000;
  int answer = card_list[0];
  while (j < n) {
    curr_diff = std::abs(answer - k);
    if (answer == k) {
      // std::cout << i << " " << j << std::endl;
      g = i;
      h = j;
      break;
    } else if (curr_diff < smallest_diff) {
      smallest_diff = curr_diff;
      g = i;
      h = j;
    }
    if (answer < k) {
      // std::cout << "answer (before j++): " << answer << " j: " << j;
      j += 1; 
      answer += card_list[j];
    } else {
      // std::cout << "answer (before i--): " << answer << " i: " << i;
        answer -= card_list[i];
        i += 1;
        if (i > j) {
          j = i;
          answer = card_list[i];
        } 
      }
  }
  std::cout << g << " " << h << std::endl;
}

int main() {
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}