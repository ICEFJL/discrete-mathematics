#include <limits.h>

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

#define MAX_V 1000

enum { Undirected_graph, Directed_graph, Undirected_net, Directed_net };

struct Edge {
  string from;
  string to;
  int value;
  int s, e;
  Edge(string f, string t, int v = 1) : from(f), to(t), value(v){};
};

struct Vex {
  string code;
  Vex(string c) : code(c){};
};

struct Graph {
  int type;
  int vexnum, arcnum;
  vector<Vex> vexs;
  vector<Edge> arc;
  int arcs[MAX_V][MAX_V] = {{0}};
  map<string, int> str_int;
  Graph(int t) : type(t) { vexnum = arcnum = 0; }
};

struct {
  int pre;
  int cost;
} Close[MAX_V];

class UF {
 private:
  int *parent;

 public:
  UF(int n) {
    parent = new int[n];
    for (int i = 0; i < n; i++) {
      parent[i] = i;
    }
  }

  int Find(int x) {
    if (parent[x] != x) parent[x] = Find(parent[x]);
    return parent[x];
  }

  bool Union(int p, int q) {
    int rootp = Find(p);
    int rootq = Find(q);
    if (rootp == rootq) return false;
    parent[rootp] = rootq;
    return true;
  }

  ~UF() {
    if (parent) delete parent;
  }
};

class MinTree {
 private:
  Graph *G;

 public:
  MinTree() { G = nullptr; }

  void RandCreate(int vex, int arc) {
    vector<Vex> V;
    string v, w, t;
    for (int i = 0; i < vex; i++) {
      v = "V" + to_string(i);
      V.push_back({v});
    }
    Create(Undirected_net, V, arc);
  }

  void Create(int type, vector<Vex> V, int arc) {
    G = new Graph(type);
    if (type == Undirected_net || type == Directed_net)
      for (int i = 0; i < MAX_V; i++)
        for (int j = 0; j < MAX_V; j++) G->arcs[i][j] = INT_MAX;
    for (Vex &a : V) Insert(a);
    string v, w;
    while (G->arcnum < arc) {
      v = "V" + to_string(rand() % G->vexnum);
      w = "V" + to_string(rand() % G->vexnum);
      Insert({v, w, rand() % 100 + 1});
    }
  }

  void Destroy() {
    if (G) delete G;
    G = NULL;
  }

  bool Judge() {
    if (G == NULL) return false;
    int v[MAX_V] = {0};
    DFS(0, v);
    for (int i = 0; i < G->vexnum; i++)
      if (!v[i]) return false;
    return true;
  }

  void DFS(int i, int *v) {
    v[i] = true;
    for (int j = 0; j < G->vexnum; j++)
      if (G->arcs[i][j] != 0 && G->arcs[i][j] != INT_MAX && !v[j]) DFS(j, v);
  }

  void Insert(Vex V) {
    if (G->str_int.find(V.code) == G->str_int.end()) {
      G->vexs.push_back({V.code});
      G->str_int.insert({V.code, G->vexnum});
      G->vexnum++;
    }
  }

  void Insert(Edge E) {
    int from = G->str_int.find(E.from)->second;
    int to = G->str_int.find(E.to)->second;
    E.s = from;
    E.e = to;
    if (from != to && G->arcs[from][to] == INT_MAX) {
      G->arcnum++;
      G->arc.push_back(E);
      G->arcs[from][to] = E.value;
      if (G->type == Undirected_graph || G->type == Undirected_net)
        G->arcs[to][from] = E.value;
    }
  }

  void Traverse(fstream &f) {
    if (G == nullptr) {
      cout << "图不存在" << endl;
      return;
    }
    f << "   ";
    for (int i = 0; i < G->vexnum; i++) {
      f << " " << setw(3) << G->vexs[i].code;
    }
    f << endl;
    for (int i = 0; i < G->vexnum; i++) {
      f << setw(3) << G->vexs[i].code << " ";
      for (int j = 0; j < G->vexnum; j++) {
        if (G->arcs[i][j] == INT_MAX)
          f << setw(4) << "X ";
        else
          f << setw(3) << G->arcs[i][j] << " ";
      }
      f << endl;
    }
  }

  void Prim(fstream &f) {
    for (int i = 1; i < G->vexnum; i++) {
      Close[i].cost = G->arcs[0][i];
      Close[i].pre = 0;
    }
    int weight = 0;
    Close[0].cost = 0;
    for (int i = 1; i < G->vexnum; i++) {
      int min = INT_MAX;
      int k = 0;
      for (int j = 1; j < G->vexnum; j++)
        if (Close[j].cost && Close[j].cost < min) {
          min = Close[j].cost;
          k = j;
        }
      string v = G->vexs[Close[k].pre].code;
      string w = G->vexs[k].code;
      f << "(" << v << "-" << w << "," << min << ") ";
      Close[k].cost = 0;
      weight += min;
      for (int j = 1; j < G->vexnum; j++) {
        if (G->arcs[k][j] < Close[j].cost) {
          Close[j].cost = G->arcs[k][j];
          Close[j].pre = k;
        }
      }
    }
    f << "最小生成树总权值为" << weight << endl;
  }

  void Kruscal(fstream &f) {
    UF u(G->vexnum);
    sort(G->arc.begin(), G->arc.end(),
         [](Edge &a, Edge &b) -> int { return a.value < b.value; });
    int ret = 0, num = 1;
    for (auto &t : G->arc) {
      if (u.Union(t.s, t.e)) {
        ret += t.value;
        num++;
        f << "(" << t.from << "-" << t.to << "," << t.value << ")";
        if (num == G->vexnum) break;
      }
    }
    f << "最小生成树总权值为" << ret << endl;
  }
};

int main() {
  MinTree G;
  fstream f[2];
  vector<string> t = {"Prim", "Kruscal"};
  int num[3][2] = {{10, 40}, {30, 40}, {30, 100}};  // 测试的顶点数和边数
  for (int j = 1; j <= 3; j++) {
    cout << num[j - 1][0] << "阶 " << num[j - 1][1] << "边 " << endl;
    for (int k = 0; k <= 1; k++) {
      string temp = t[k] + "_" + to_string(j) + ".txt";
      f[k].open(temp, ios::out);
      if (!f[k].is_open()) exit(0);
    }
    double time[2] = {0};
    double maxtime[2] = {0}, mintime[2] = {1, 1};
    for (int i = 0; i < 100; i++) {
      double t = 0;
      do {
        G.Destroy();
        G.RandCreate(num[j - 1][0], num[j - 1][1]);
      } while (!G.Judge());
      for (int k = 0; k <= 1; k++) {
        G.Traverse(f[k]);
        clock_t t1 = clock();
        if (!k)
          G.Prim(f[k]);
        else
          G.Kruscal(f[k]);
        clock_t t2 = clock();
        t = (double)(t2 - t1);
        time[k] += t;
        maxtime[k] = max(t, maxtime[k]);
        mintime[k] = min(t, mintime[k]);
      }
    }
    for (int k = 0; k <= 1; k++) {
      cout << t[k] << "算法平均执行时间：" << time[k] / 100
           << "ms 最大执行时间: " << maxtime[k]
           << "ms 最小执行时间: " << mintime[k] << "ms" << endl
           << endl;
      f[k].close();
    }
  }
  system("pause");
}