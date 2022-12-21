## 运行环境

windows平台 VS2022

## 运行方式

打开"最小生成树.sln",即可在VS2022上运行该项目
也可直接在"最小生成树\x64\Debug\最小生成树.exe"处直接运行

## 源文件（MinTree.cpp）

本次实验选择了三种情况的图，可在main函数中修改num数组的元素进行改变，每组第一个元素为顶点数，第二个为边数。

```cpp
int num[3][2] = {{10, 40}, {30, 40}, {30, 100}};
for (int j = 1; j <= 3; j++){...}
```

实验结果为100个联通无向图的平均结果

```cpp
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
                t = (double)(t2 - t1) / CLOCKS_PER_SEC;
                time[k] += t;
                maxtime[k] = max(t, maxtime[k]);
                mintime[k] = min(t, mintime[k]);
            }
        }
```

## 输出文件（Prim_i.txt,Kruscal_i.txt)(i=1,2,3)

本次实验将第i种情况下的100次运行结果和图分别输入到Prim_i.txt,Kruscal_i.txt中，可打开文件查看
