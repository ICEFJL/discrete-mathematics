#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#define MAX_V 1000

enum { Undirected_graph, Directed_graph, Undirected_net, Directed_net };

struct Edge {
  string from;
  string to;
  int value;
  Edge(string f, string t, int v = 1) : from(f), to(t), value(v){};
};

class RandomizedSet {
 public:
  vector<int> nums;
  unordered_map<int, int> v;
  RandomizedSet(){};

  RandomizedSet(int n) {
    for (int i = 0; i < n; i++) {
      insert(i);
    }
  }

  bool find(int val) { return v.count(val); }

  bool insert(int val) {
    if (v.count(val)) return 0;
    v[val] = nums.size();
    nums.push_back(val);
    return 1;
  }

  bool remove(int val) {
    if (!v.count(val)) return 0;
    int index = v[val];
    v[nums.back()] = index;
    swap(nums[index], nums.back());
    nums.pop_back();
    v.erase(val);
    return 1;
  }

  int getRandom() { return nums[rand() % nums.size()]; }
};

struct Vex {
  string code;
  int in, out;
  RandomizedSet f;
  Vex(string c) : code(c) { in = out = 0; };
};

struct Graph {
  int type;
  int vexnum, arcnum;
  vector<Vex> vexs;
  map<string, int> str_int;
  Graph(int t) : type(t) { vexnum = arcnum = 0; }
};

class Euler {
 private:
  Graph *G;
  vector<Vex> vexs;
  int visited[MAX_V];

 public:
  Euler() { G = nullptr; }

  bool RandCreate(int type, int vex, int arc) {
    G = new Graph(type);
    string v;
    for (int i = 0; i < vex; i++) {
      v = "V" + to_string(i);
      Insert(v);
    }
    memset(visited, 0, sizeof(visited));
    int pre = 0, t = 0;
    vector<RandomizedSet> s(G->vexnum, {G->vexnum});
    for (int i = 0; i < vex; i++) s[i].remove(i);
    RandomizedSet vt(G->vexnum);
    vt.remove(0);
    for (int i = 1; i < G->vexnum; i++) {
      t = vt.getRandom();
      vt.remove(t);
      s[pre].remove(t);
      InsertEdge(pre, t);
      pre = t;
    }
    while (G->arcnum < arc - 1) {
      t = s[pre].getRandom();
      if ((G->type == Undirected_graph || G->type == Undirected_net) &&
          s[t].nums.size() == 1)
        continue;
      s[pre].remove(t);
      InsertEdge(pre, t);
      pre = t;
    }
    if (s[pre].find(0)) {
      InsertEdge(pre, 0);
      return true;
    } else
      return false;
  }

  void Destroy() {
    if (G) delete G;
    G = NULL;
  }

  void Insert(string &code) {
    G->vexs.push_back({code});
    G->str_int.insert({code, G->vexnum});
    G->vexnum++;
  }

  void InsertEdge(int from, int to) {
    if (from == to) return;
    if (Insert(from, to)) {
      G->arcnum++;
      if (G->type == Undirected_graph || G->type == Undirected_net)
        Insert(to, from);
    }
  }

  bool Insert(int &from, int &to) {
    if (G->vexs[from].f.insert(to)) {
      G->vexs[from].out++;
      G->vexs[to].in++;
      return true;
    }
    return false;
  }

  void Traverse(fstream &f) {
    for (int i = 0; i < G->vexnum; i++) {
      f << G->vexs[i].code << ":";
      int s = G->vexs[i].f.nums.size();
      for (int j = 0; j < s; j++) {
        int v = G->vexs[i].f.nums[j];
        f << " " << G->vexs[v].code;
      }
      f << endl;
    }
  }
  bool Judge() {
    if (G == NULL) return false;
    memset(visited, 0, sizeof(visited));
    DFS(0);
    for (int i = 0; i < G->vexnum; i++)
      if (!visited[i]) return false;
    if (G->type == Undirected_graph || G->type == Undirected_net) {
      for (int i = 0; i < G->vexnum; i++)
        if (G->vexs[i].in % 2) return false;
    } else {
      for (int i = 0; i < G->vexnum; i++)
        if (G->vexs[i].in != G->vexs[i].out) return false;
    }
    return true;
  }

  void DFS(int t) {
    visited[t] = true;
    int s = G->vexs[t].f.nums.size();
    for (int i = 0; i < s; i++) {
      int v = G->vexs[t].f.nums[i];
      if (!visited[v]) DFS(v);
    }
  }

  bool DFS(int t, int pre, int anc) {
    visited[t] = true;
    int s = vexs[t].f.nums.size();
    for (int i = 0; i < s; i++) {
      int v = vexs[t].f.nums[i];
      if ((G->type == Undirected_graph || G->type == Undirected_net) &&
          v == pre)
        continue;
      if (v == anc) return false;
      if (!visited[v] && !DFS(v, t, anc)) return false;
    }
    return true;
  }

  void Fleury(fstream &f) {
    vexs = G->vexs;
    vector<int> cir;
    int v = 0, w = 0;
    for (int i = 0; i < G->arcnum; i++) {
      int s = vexs[v].f.nums.size();
      for (int j = 0; j < s; j++) {
        int t = vexs[v].f.nums[j];
        w = t;
        memset(visited, 0, sizeof(visited));
        if (!DFS(t, v, v) || s == 1) break;
      }
      DeleteArc(v, w);
      v = w;
      cir.push_back(v);
    }
    f << "欧拉回路：V0";
    for (auto &t : cir) {
      f << "->" << G->vexs[t].code;
    }
    f << endl << endl;
  }

  void FIND(int t, vector<int> &cir) {
    for (int i = 0; i < vexs[t].f.nums.size();) {
      int v = vexs[t].f.nums[i];
      DeleteArc(t, v);
      FIND(v, cir);
    }
    cir.push_back(t);
  }

  void Hierholzer(fstream &f) {
    vexs = G->vexs;
    vector<int> cir;
    FIND(0, cir);
    f << "欧拉回路：V0";
    for (int i = G->arcnum - 1; i >= 0; i--) {
      f << "->" << G->vexs[cir[i]].code;
    }
    f << endl << endl;
  }

  void DeleteArc(int &v, int &w) {
    vexs[v].f.remove(w);
    if (G->type == Undirected_graph || G->type == Undirected_net)
      vexs[w].f.remove(v);
  }
};

int main() {
  Euler E;
  fstream f[2];
  vector<string> al = {"Fleury", "Hierholzer"};
  vector<string> tp = {"无向图", "有向图"};
  int num[2][3][2] = {{{10, 20}, {40, 80}, {60, 80}},
                      {{10, 20}, {60, 80}, {60, 200}}};

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      int a = num[i][j][0];
      int b = num[i][j][1];
      cout << a << "阶 " << b << "边 " << tp[i] << endl;

      for (int k = 0; k <= 1; k++) {
        string temp = al[k] + "_" + tp[i] + "_" + to_string(j) + ".txt";
        f[k].open(temp, ios::out);
        if (!f[k]) exit(0);
      }

      double time[2] = {0};

      for (int n = 0; n < 100; n++) {
        double t = 0;
        do {
          E.Destroy();
        } while (!E.RandCreate(i, a, b) || !E.Judge());
        for (int k = 0; k <= 1; k++) {
          E.Traverse(f[k]);
          clock_t t1 = clock();
          if (!k)
            E.Fleury(f[k]);
          else
            E.Hierholzer(f[k]);
          clock_t t2 = clock();
          t = (double)(t2 - t1);
          time[k] += t;
        }
      }

      for (int k = 0; k <= 1; k++) {
        cout << al[k] << "算法平均执行时间：" << time[k] / 100 << "ms" << endl
             << endl;
        f[k].close();
      }
    }
  }
  system("pause");
}