#include<bits/stdc++.h>
using namespace std;
#define rep(i,a,b) for(int i = a ; i <= b ; i++ )
#define per(i,a,b) for(int i = a ; i >= b ; i-- )
typedef long long ll;
const int N = 1e5+7;

int n;
struct Node {
	ll num,val;
};
vector<Node>tree[N];
int cnt = 1;
ll pos[N],d[N],dfn[N];
void dfs(int root) {
	for(auto &son:tree[root]) {
		if(dfn[son.num])
			continue;
		dfn[son.num] = ++cnt;
		pos[ dfn[son.num] ] = son.num;
		d[son.num] = d[root] + son.val;

		dfs(son.num);
	}
	return ;
}

int dp[N][21],depth[N];
void init(int root) {
	rep(i,1,n) {
		depth[i] = 0x3f3f3f3f;
	}
	queue<int>q;
	q.push(root);
	depth[root] = 1;
	while(q.size()) {
		auto x = q.front();
		q.pop();
		for(auto &son:tree[x]) {
			if(depth[son.num] > depth[x]+1) {
				depth[son.num] = depth[x]+1;
				q.push(son.num);

				dp[son.num][0] = x;
				rep(i,1,20) {
					dp[son.num][i] = dp[ dp[son.num][i-1] ][i-1];
				}
			}
		}
	}
}

int lca(int a,int b) {
	if(depth[a] > depth[b])
		swap(a,b);
	// b is below a
	per(i,20,0) {
		if(depth[dp[b][i]] >= depth[a]) {
			b = dp[b][i];
		}
	}
	if(a==b)
		return a;
	per(i,20,0) {
		if(dp[a][i] != dp[b][i]) {
			a = dp[a][i];
			b = dp[b][i];
		}
	}
	return dp[a][0];
}	

ll path(int x,int y) {
	return d[x]+d[y]-d[lca(x,y)]*2;
}

ll ans = 0;
set<int>s;
void work(ll flag,int x) {
	auto it = s.find(x);
	auto l = it;
	if(l==s.begin()) {
		l = s.end();
	}
	l--;

	auto r = it;
	r++;
	if(r==s.end())
		r = s.begin();

	// 1 8 9
	int ll = pos[*l], rr = pos[*r];
	ans -= path(ll,rr)*flag;
	ans += (path(ll,pos[x])+path(pos[x],rr))*flag;
}

int main() {
	scanf("%d",&n);
	rep(i,1,n-1) {
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		tree[x].push_back({y,z});
		tree[y].push_back({x,z});
	}
	init(1);
	pos[1] = 1,dfn[1] = 1;
	dfs(1);
	int m;
	scanf("%d",&m);
	rep(i,1,m) {
		char ch;
		cin >> ch;
		if(ch=='?') {
			printf("%lld\n",ans/2);
		} else {
			int x;
			scanf("%d",&x);
			if(ch=='+') {
				s.insert(dfn[x]);
				work(1,dfn[x]);
			} else {
				work(-1,dfn[x]);
				s.erase(dfn[x]);
			}
		}
	}
}
