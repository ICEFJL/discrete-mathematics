## 运行环境

windows平台 VS2022

## 运行方式

打开"欧拉回路.sln",即可在VS2022上运行该项目
也可直接在"欧拉回路\x64\Debug\欧拉回路.exe"处直接运行

## 源文件（MinTree.cpp）

本次实验选择了三种情况的无向图和有向图，前三个组为无向，后三个组为有向，可在main函数中修改num数组的元素进行改变，每组第一个元素为顶点数，第二个为边数。

```cpp
int num[2][3][2] = {{{10, 20}, {40, 80}, {60, 80}}, {{10, 20} ,{60, 80}, {60, 200}}};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {...}
```

实验结果为100个欧拉图的平均结果

```cpp
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
                    t = (double)(t2 - t1) / CLOCKS_PER_SEC;
                    time[k] += t;
                }
            }
```

## 输出文件（Fleury_无向/有向_i.txt,Hierholzer_无向/有向_i.txt)(i=0,1,2)

本次实验将无向/有向图的第i种情况下的100次运行结果和图分别输入到Fleury_无向/有向_i.txt,Hierholzer_无向/有向_i.txt中，可打开文件查看


