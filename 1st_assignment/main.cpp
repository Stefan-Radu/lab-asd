// By Stefan Radu

#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <cmath>

using namespace std;
using namespace std::chrono;

void minSort(vector < int > &v) {/*{{{*/
  for (int i = 0; i < (int) v.size(); ++ i) {
    for (int j = i + 1; j < (int) v.size(); ++ j) {
      if (v[i] > v[j]) swap(v[i], v[j]);
    }
  }
}/*}}}*/

void bubbbleSort(vector < int > &v) {/*{{{*/

  bool sorted = false;
  while (not sorted) {
    sorted = true;
    for (int i = 1; i < (int) v.size(); ++ i) {
      if (v[i] < v[i - 1]) {
        swap(v[i], v[i - 1]);
        sorted = false;
      }
    }
  }
}/*}}}*/

void countSort(vector < int > &v, int maxVal) {/*{{{*/

  vector < int > frecv(maxVal + 1);
  for (auto x : v) {
    ++ frecv[x];
  }

  int index = 0;
  for (int i = 0; i <= maxVal; ++ i) {
    for (int j = 1; j <= frecv[i]; ++ j) {
      v[index ++] = i;
    }
  }
}/*}}}*/

void radixSort(vector < int > &v) {/*{{{*/

  vector < vector < int > > buckets(256);

  for (int buck = 0; buck <= 31; buck += 8) {

    for (auto &x : v) {
      buckets[(x >> buck) & 255].push_back(x);
    }

    int index = 0;
    for (auto &x : buckets) {
      for (auto y : x) {
        v[index ++] = y;
      }
      x.clear();
    }
  }
}/*}}}*/

void radixSort(vector < long long > &v, int base) {/*{{{*/

  if (__builtin_popcount(base) != 1) {
    cerr << "The base is not a power of 2";
  }

  int step = log2(base);

  vector < vector < int > > buckets(base);

  for (int buck = 0; buck < 63; buck += step) {

    for (auto &x : v) {
      buckets[(x >> buck) & (base - 1)].push_back(x);
    }

    int index = 0;
    for (auto &x : buckets) {
      for (auto y : x) {
        v[index ++] = y;
      }
      x.clear();
    }
  }
}/*}}}*/

class SkipList {/*{{{*/
public:

  SkipList() {
    root = new Node(-INF, MAX_H);
  }

  void insert(int val) {

    int newH = getNewHeight();
    int curLvl = MAX_H - 1;
    Node *curNode = root;

    vector < Node* > prevNodes(newH);

    while (true) {
      if (not curNode -> nextNodeAtLvl[curLvl]) {
        if (curLvl) {
          if (curLvl < newH) {
            prevNodes[curLvl] = curNode;
          }
          -- curLvl;
          continue;
        }
        prevNodes[0] = curNode;
        break;
      }

      if (curNode -> nextNodeAtLvl[curLvl] -> val <= val) {
        curNode = curNode -> nextNodeAtLvl[curLvl];
      }
      else {
        if (curLvl) {
          if (curLvl < newH) {
            prevNodes[curLvl] = curNode;
          }
          -- curLvl;
          continue;
        }
        prevNodes[0] = curNode;
        break;
      }
    }

    Node *newNode = new Node(val, newH);
    for (int i = 0; i < newH; ++ i) {
      newNode -> nextNodeAtLvl[i] = prevNodes[i] -> nextNodeAtLvl[i];
      prevNodes[i] -> nextNodeAtLvl[i] = newNode;
    }
  }

  void erase(int val) {

    int curLvl = MAX_H - 1;
    Node *curNode = root;
    vector < Node* > prevNodes(MAX_H);

    while (true) {
      if (not curNode -> nextNodeAtLvl[curLvl]) {
        if (curLvl) {
          prevNodes[curLvl] = curNode;
          -- curLvl;
          continue;
        }
        prevNodes[0] = curNode;
        break;
      }

      if (curNode -> nextNodeAtLvl[curLvl] -> val < val) {
        curNode = curNode -> nextNodeAtLvl[curLvl];
      }
      else {
        if (curLvl) {
          prevNodes[curLvl] = curNode;
          -- curLvl;
          continue;
        }
        prevNodes[0] = curNode;
        break;
      }
    }

    int h = 0;
    if (prevNodes[0] -> nextNodeAtLvl[0]) {
      if (prevNodes[0] -> nextNodeAtLvl[0] -> val != val) return;
      h = prevNodes[0] -> nextNodeAtLvl[0] -> nextNodeAtLvl.size();
    }

    for (int i = 0; i < h; ++ i) {
      if (prevNodes[i] -> nextNodeAtLvl[i]) {
        prevNodes[i] -> nextNodeAtLvl[i] = prevNodes[i] -> nextNodeAtLvl[i] -> nextNodeAtLvl[i];
      }
    }
  }

  void replaceV(vector < int > &v) {
    int index = 0;
    Node *curNode = root;
    while (curNode -> nextNodeAtLvl[0]) {
      v[index ++] = curNode -> nextNodeAtLvl[0] -> val;
      curNode = curNode -> nextNodeAtLvl[0];
    }
  }

private:

  struct Node {

    Node(int v, int height): val(v) {
      nextNodeAtLvl.resize(height);
      for (auto &x : nextNodeAtLvl) x = nullptr;
    }

    int val;
    vector < Node* > nextNodeAtLvl;
  };

  static const int INF = (1ll << 31) - 1;
  static const int MAX_H = 27;

  Node *root;

  int getNewHeight() {
    int k = 1;
    while (rand() & 1) ++ k;
    return min(k, MAX_H);
  }
};/*}}}*/

void skipListSort(vector < int > &v) {/*{{{*/
  SkipList s;
  for (auto x : v) s.insert(x);
  s.replaceV(v);
}/*}}}*/

void merge(int st, int dr, vector < int > &v) {/*{{{*/

  int mid = (st + dr) >> 1;
  int i = st, j = mid + 1, k = 0;
  vector < int > aux(dr - st + 1);

  while (i <= mid and j <= dr) {
    if (v[i] < v[j]) aux[k ++] = v[i ++];
    else aux[k ++] = v[j ++];
  }

  while (i <= mid) aux[k ++] = v[i ++];
  while (j <= dr) aux[k ++] = v[j ++];

  for (int i = 0; i < (int) aux.size(); ++ i) {
    v[st + i] = aux[i];
  }
}/*}}}*/

void mergeSort(int st, int dr, vector < int > &v) {/*{{{*/

  if (st == dr) return;

  int mid = (st + dr) >> 1;
  mergeSort(st, mid, v);
  mergeSort(mid + 1, dr, v);

  merge(st, dr, v);
}/*}}}*/

int partitionWithRand(int st, int dr, vector < int > &v) {/*{{{*/

  swap(v[st + rand() % (dr - st + 1)], v[dr]);
  int pivot = v[dr], i = st;

  for (int j = st; j <= dr; ++ j) {
    if (v[j] < pivot) swap(v[j], v[i ++]);
  }

  swap(v[i], v[dr]);
  return i;
}/*}}}*/

void quickSort(int st, int dr, vector < int > &v) {/*{{{*/

  if (st >= dr) return;

  int p = partitionWithRand(st, dr, v);
  quickSort(st, p - 1, v);
  quickSort(p + 1, dr, v);
}/*}}}*/

int partitionWithMedian5(int st, int dr, vector < int > &v) {/*{{{*/

  vector < int > aux;
  for (int i = 1; i <= 5; ++ i) {
    aux.push_back(st + rand() % (dr - st + 1));
  }

  sort(aux.begin(), aux.end(), [&](int a, int b) -> bool {return v[a] < v[b];});
  swap(v[dr], v[aux[2]]);

  int pivot = v[dr], i = st;

  for (int j = st; j <= dr; ++ j) {
    if (v[j] < pivot) swap(v[j], v[i ++]);
  }

  swap(v[i], v[dr]);
  return i;
}/*}}}*/

void quickSort2(int st, int dr, vector < int > &v) {/*{{{*/

  if (st >= dr) return;

  int p = partitionWithMedian5(st, dr, v);
  quickSort2(st, p - 1, v);
  quickSort2(p + 1, dr, v);
}/*}}}*/

class MinHeap {/*{{{*/
public:

  MinHeap() {}

  void sort(vector < int > &v) {
    sz = (int) v.size();
    tree = v;
    heapify();

    int ind = 0;
    while (sz) {
      v[ind ++] = removeTop();
    }
  }

private:

  int sz;
  vector < int > tree;

  void heapify() { 
    for (int i = sz - 1; i >= 0; -- i) {
      downShift(i);
    }
  }

  void downShift(int ind) {
    int left = ind << 1, right = left | 1;
    while (right < sz) {
      if (min(tree[left], tree[right]) < tree[ind]) {
        if (tree[left] < tree[right]) {
          swap(tree[left], tree[ind]);
          ind = left;
        }
        else {
          swap(tree[right], tree[ind]);
          ind = right;
        }
      }
      else {break;}
      left = ind << 1, right = left | 1;
    }

    if (left < sz and tree[left] < tree[ind]) {
      swap(tree[left], tree[ind]);
    }
  }

  int removeTop() {

    int ret = tree[0];
    swap(tree[0], tree[sz - 1]);
    -- sz;

    downShift(0);
    return ret;
  }
};/*}}}*/

void heapSort(vector < int > &v) {/*{{{*/
  MinHeap h;
  h.sort(v);
}/*}}}*/

struct TreapNode {/*{{{*/
  TreapNode(int v, TreapNode *l = 0, TreapNode *r = 0):
    val(v), priority(rand()), left(l), right(r) {}
  int val, priority;
  TreapNode *left, *right;
};/*}}}*/

class Treap {/*{{{*/ public: Treap() {
    root = nullptr;
  }

  void sort(vector < int > &v) {
    for (auto &x : v) insert(x);
    int ind = 0;
    storeValues(ind, v, root);
  }

private:

  TreapNode *root;

  pair < TreapNode*, TreapNode* > split(int val, TreapNode *curNode) {

    if (not curNode) return {nullptr, nullptr};

    if (curNode -> val < val) {
      auto pa = split(val, curNode -> right);
      curNode -> right = pa.first;
      return {curNode, pa.second};
    }
    else {
      auto pa = split(val, curNode -> left);
      curNode -> left = pa.second;
      return {pa.first, curNode};
    }
  }

  TreapNode* join(TreapNode *left, TreapNode *right) {

    if (not left) return right;
    if (not right) return left;

    if (left -> priority > right -> priority) {
      left -> right = join(left -> right, right);
      return left;
    }
    else {
      right -> left = join(left, right -> left);
      return right;
    }
  }

  void insert(int val) {
    auto pa = split(val, root);
    auto aux = new TreapNode(val);
    root = join(join(pa.first, aux), pa.second);
  }

  void storeValues(int &ind, vector < int > &v, TreapNode *curNode) {
    if (not curNode) return;
    storeValues(ind, v, curNode -> left);
    v[ind ++] = curNode -> val;
    storeValues(ind, v, curNode -> right);
  }
};/*}}}*/

void treapSort(vector < int > &v) {/*{{{*/
  Treap t;
  t.sort(v);
}/*}}}*/

void testSortMethods(const int nrOfElements) {/*{{{*/

  if (nrOfElements > 5e7) {
    cerr << '\n' << nrOfElements << " are too many elements\n";
    return;
  }

  cerr << "\nTesting sorting methods for " << nrOfElements << " elements\n\n";

  vector < int > toBeSorted(nrOfElements);
  for (int i = 0; i < nrOfElements; ++ i) toBeSorted[i] = i;
  random_shuffle(toBeSorted.begin(), toBeSorted.end());

  vector < int > refference = toBeSorted;

  auto startTime = chrono::high_resolution_clock::now();

  // std::sort

  sort(refference.begin(), refference.end());

  auto endTime = chrono::high_resolution_clock::now();
  auto timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  auto timeElapsed = duration_cast < milliseconds > (endTime - startTime);
  cerr << "std::sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";

  // Count Sort

  vector < int > aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  countSort(aux, nrOfElements - 1);
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Count Sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";


  // Radix Sort

  aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  radixSort(aux);
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Radix Sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";

  
  if (nrOfElements <= 1e6) {

    // SkipList Sort

    aux = toBeSorted;

    startTime = chrono::high_resolution_clock::now();
    skipListSort(aux);
    endTime = chrono::high_resolution_clock::now();
    timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
    timeElapsed = duration_cast < milliseconds > (endTime - startTime);

    assert(aux == refference);
    cerr << "SkipList Sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";
  }
  else {
    cerr << "SkipList Sort will take too long to execute\n";
  }


  // Merge Sort

  aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  mergeSort(0, nrOfElements - 1, aux);
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Marge Sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";


  // Quick Sort - random

  aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  quickSort(0, nrOfElements - 1, aux);
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Quick Sort (with random) has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";


  // Quick Sort - random

  aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  quickSort2(0, nrOfElements - 1, aux);
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Quick Sort (with median of 5) has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";


  // Heap Sort

  aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  heapSort(aux);
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Heap Sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";



  if (nrOfElements <= 1e6) {

    // Treap Sort

    aux = toBeSorted;

    startTime = chrono::high_resolution_clock::now();
    treapSort(aux);
    endTime = chrono::high_resolution_clock::now();
    timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
    timeElapsed = duration_cast < milliseconds > (endTime - startTime);

    assert(aux == refference);
    cerr << "Treap Sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";
  }
  else {
    cerr << "Treap Sort will take too long to execute\n";
  }


  if (nrOfElements > 3e4) {
    cerr << "minSort and Bubble Sort will take too long to execute\n";
    return;
  }

  // minSort

  aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  minSort(aux);
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Min Sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";


  // Bubble Sort

  aux = toBeSorted;
  startTime = chrono::high_resolution_clock::now();
  bubbbleSort(aux);
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Bubble Sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";
}/*}}}*/

void testRadixSort(int nrOfElements, long long maxVal) {/*{{{*/

  /* if (nrOfElements > 5e7) { */
  /*   cerr << '\n' << nrOfElements << " are too many elements\n"; */
  /*   return; */
  /* } */

  cerr << "\nTesting sorting methods for " << nrOfElements << " elements and the max value " << maxVal << "\n\n";

  vector < long long > toBeSorted(nrOfElements);
  for (auto &x : toBeSorted) x = abs((long long) (rand() * rand()) % (maxVal + 1));

  vector < long long > refference = toBeSorted;

  auto startTime = chrono::high_resolution_clock::now();

  // std::sort

  sort(refference.begin(), refference.end());

  auto endTime = chrono::high_resolution_clock::now();
  auto timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  auto timeElapsed = duration_cast < milliseconds > (endTime - startTime);
  cerr << "std::sort has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";


  // Radix Sort - base 2^4

  auto aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  radixSort(aux, (1 << 4));
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Radix Sort (base 2^4) has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";


  // Radix Sort - base 2 ^ 8

  aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  radixSort(aux, (1 << 8));
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Radix Sort (base 2^8) has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";


  // Radix Sort - base 2 ^ 16

  aux = toBeSorted;

  startTime = chrono::high_resolution_clock::now();
  radixSort(aux, (1 << 16));
  endTime = chrono::high_resolution_clock::now();
  timeElapsed2 = duration_cast < microseconds > (endTime - startTime);
  timeElapsed = duration_cast < milliseconds > (endTime - startTime);

  assert(aux == refference);
  cerr << "Radix Sort (base 2^16) has executed in: " << timeElapsed.count() << " milliseconds & " << timeElapsed2.count() << " microseconds\n";
}/*}}}*/

int main() {

  srand(time(0));

  testSortMethods(10);
  testSortMethods(100);
  testSortMethods(1000);
  testSortMethods(3e4);
  testSortMethods(1e6);
  testSortMethods(1e7);
  testSortMethods(5e7);
  testSortMethods(1e8);

  testRadixSort(1e6, 1e6);
  testRadixSort(1e6, 1e9);
  testRadixSort(1e6, 1e18);
  testRadixSort(1e7, 1e6);
  testRadixSort(1e7, 1e9);
  testRadixSort(1e7, 1e18);
  testRadixSort(5e7, 1e6);
  testRadixSort(5e7, 1e12);
  testRadixSort(5e7, 1e18);
}
