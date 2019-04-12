#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <deque>

using namespace std;

vector<int> find_max_sliding_window(vector<int>& v, int window_size) {
  if (v.empty()) return {};
  
  vector<int> result;
  
  // subset of indices for current window
  // Invariant: window.front() contains max element for current 
  // window. New indices are added to back
  deque<int> window;
  
  // find max for first window
  for (int idx = 0; idx < window_size; ++idx) {
    while (!window.empty() && v[idx] >= v[window.back()]) {
      window.pop_back();
    }
    window.push_back(idx);
  }
  result.push_back(v[window.front()]);
  
  // process remaining windows
  for (int idx = window_size, N = v.size(); idx < N; ++idx) {
    // remove all indices from back of deque that 
    // are less than or equal to current element
    while (!window.empty() &&
           v[idx] >= v[window.back()]) {
      window.pop_back();
    }
            
    // remove front index if it has fallen outside window range
    if (!window.empty() && window.front() <= idx - window_size) {
      window.pop_front();
    }
    
    // add current idx
    window.push_back(idx);
    // add current max
    result.push_back(v[window.front()]);
  }
  
  return result;
}


void print(const vector<int>& v, string name)
{
	cout << name << " = {";
	for (int n : v) cout << n << ' ';
	cout << "}\n";
}

int solution(const vector<int>& A, int K, int L)
{
	const int N = A.size();
	if (K + L > N) return -1;

	vector<int> k_right_max(N, 0);
	vector<int> k_left_max(N, 0);
	for (int i = 0; i+K <= N; ++i) {
		int sum = accumulate(A.begin() + i, A.begin() + i + K, 0);
		k_right_max[i+K-1] = sum;
		k_left_max[i] = sum;
	}

	vector<int> l_right_max(N, 0);
	vector<int> l_left_max(N, 0);
	for (int i = 0; i+L <= N; ++i) {
		int sum = accumulate(A.begin() + i, A.begin() + i + L, 0);
		l_right_max[i+L-1] = sum;
		l_left_max[i] = sum;
	}

	k_left_max = find_max_sliding_window(k_left_max, K);
	l_left_max = find_max_sliding_window(l_left_max, L);
	k_right_max = find_max_sliding_window(k_right_max, K);
	l_right_max = find_max_sliding_window(l_right_max, L);
	

	// append K-1 0's to end of k_left_max,
	// L-1 0's to end of l_left_max
	k_left_max.resize(k_left_max.size() + K - 1, 0);
	l_left_max.resize(l_left_max.size() + L - 1, 0);

	// prepend K-1 0's to beginning of k_right_max,
	// L-1 0's to beginning of l_right_max
	reverse(k_right_max.begin(), k_right_max.end());
	k_right_max.resize(k_right_max.size() + K - 1, 0);
	reverse(k_right_max.begin(), k_right_max.end());
	reverse(l_right_max.begin(), l_right_max.end());
	l_right_max.resize(l_right_max.size() + L - 1, 0);
	reverse(l_right_max.begin(), l_right_max.end());


	int ans = 0;
	for (int i = 1; i < N-1; ++i) {
		int sum1 = k_right_max[i] + l_left_max[i+1];
		int sum2 = k_right_max[i-1] + l_left_max[i];

		int sum3 = l_right_max[i] + k_left_max[i+1];
		int sum4 = l_right_max[i-1] + k_left_max[i];

		ans = max(ans, max(max(sum1, sum2), max(sum3, sum4)));
	}

	return res;
}


int main()
{
	vector<int> A = {6,1,4,6,3,2,7,4};
	vector<int> B = {5,3,3,7,8,9,1,3,6,8,9,3,4,7,2,6,2};
	print(A, "A");
	int K = 3, L = 2;
	cout << "Answer: " << solution(A,K,L) << endl;
	print(B, "B");
	cout << "\nAnswer: " << solution(B,K,L) << endl;
}
