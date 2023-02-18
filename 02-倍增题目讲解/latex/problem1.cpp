#include <iostream>
#include <cstring>
#include <set>

using namespace std;

typedef long long LL;

const int N = 100010, M = N * 2;

int n, m;
int h[N], e[M], w[M], ne[M], idx; //邻接表
int dep[N], fa[N][17]; //dep[i] 表示节点 i 的深度，fa[i][j] 表示节点 i 往上跳 2^j 步到达的节点
LL dist[N]; //dist[i] 表示节点 i 到根节点的路径长度
int q[N]; //队列
//dfn[i] 表示节点 i 的时间戳(dfs序)
//pos[i] 表示时间戳(dfs序)对应的节点编号
//timestamp 表示当前用到的时间戳
int dfn[N], pos[N], timestamp;
set<int> S; //存储当前存在异象石的时间戳
LL res; //记录当前的答案

void add(int a, int b, int c) //添加边
{
e[idx] = b, w[idx] = c, ne[idx] = h[a], h[a] = idx++;
}

void bfs() //预处理 dep[], fa[][]
{
dep[1] = 1;

int hh = 0, tt = 0;
q[0] = 1;

while(hh <= tt)
{
int t = q[hh++];

for(int i = h[t]; i != -1; i = ne[i])
{
int j = e[i];
if(dep[j]) continue;

dep[j] = dep[t] + 1;
dist[j] = (LL)dist[t] + w[i];

fa[j][0] = t;
for(int k = 1; k <= 16; k++)
fa[j][k] = fa[fa[j][k - 1]][k - 1];

q[++tt] = j;
}
}
}

void dfs(int u, int father) //预处理 dfn[], pos[]
{
dfn[u] = ++timestamp;
pos[timestamp] = u;

for(int i = h[u]; i != -1; i = ne[i])
{
int j = e[i];
if(j == father) continue; //防止往上回搜

dfs(j, u);
}
}

int lca(int a, int b) //求公共祖先节点
{
if(dep[a] < dep[b]) swap(a, b);

for(int k = 16; k >= 0; k--)
if(dep[fa[a][k]] >= dep[b])
a = fa[a][k];

if(a == b) return a;

for(int k = 16; k >= 0; k--)
if(fa[a][k] != fa[b][k])
{
a = fa[a][k];
b = fa[b][k];
}

return fa[a][0];
}

LL path(int a, int b) //求 a, b 之间的路径长度
{
return dist[a] + dist[b] - 2 * dist[lca(a, b)];
}

int main()
{
scanf("%d", &n);

memset(h, -1, sizeof h); //初始化邻接表

for(int i = 0; i < n - 1; i++)
{
int a, b, c;
scanf("%d%d%d", &a, &b, &c);
add(a, b, c), add(b, a, c); //无向边
}

bfs(); //预处理 dep[], fa[][]
dfs(1, -1); //预处理 dfn[], pos[]

scanf("%d", &m);

char op[2];
int x;
while(m--)
{
scanf("%s", op);
if(op[0] == '+') //添加异象石
{
scanf("%d", &x);
S.insert(dfn[x]); //将当前异象石对应的时间戳加入序列
auto it = S.find(dfn[x]); //找到序列中当前异象石对应得时间戳所在得位置(迭代器)

if(it-- == S.begin()) it = S.end(), it--; //迭代器左移(处理边界，头尾相连)
int l = pos[*it]; //记录当前异象石左边位置的节点
if(++it == S.end()) it = S.begin(); //迭代器右移，回到原位(处理边界，头尾相连)
if(++it == S.end()) it = S.begin(); //迭代器右移(处理边界，头尾相连)
int r = pos[*it]; //记录当前异象石右边位置的节点

res = res - path(l, r) + path(l, x) + path(x, r); //更新答案
}
else if(op[0] == '-') //删除异象石
{
scanf("%d", &x);
auto it = S.find(dfn[x]);

if(it-- == S.begin()) it = S.end(), it--; //迭代器左移(处理边界，头尾相连)
int l = pos[*it]; //记录当前异象石左边位置的节点
if(++it == S.end()) it = S.begin(); //迭代器右移，回到原位(处理边界，头尾相连)
if(++it == S.end()) it = S.begin(); //迭代器右移(处理边界，头尾相连)
int r = pos[*it]; //记录当前异象石右边位置的节点

if(it-- == S.begin()) it = S.end(), it--; //迭代器左移，回到原位(处理边界，头尾相连)
S.erase(it); //删除当前异象石

res = res + path(l, r) - path(l, x) - path(x, r); //更新答案
}
else printf("%lld\n", res / 2); //求答案，当前记录的答案是2倍
}
return 0;
}