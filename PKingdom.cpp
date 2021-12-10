#include <bits/stdc++.h>
#define int long long
using namespace std; // SCC 蝮桅?霈AG
// reference: https://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/#main
int n;
int sccid[100010];             //閮?瘥?撅祆?芸CC
bool sinknotfound;             //蝚砌?甇亦??
bool vis[100010];              //蝚砌?甇亦??
int Count = 1;                 //閮?SCCID
int Max = -1;                  // SCC?賊? 蝮格?DAG???賊?
bool sink[100010] = {false};   // ?臭??盎ink
bool source[100010] = {false}; //?臭??盎ource
bool marksink[100010] = {false};
vector<int> unmarksource;
vector<int> unpairsource;
vector<int> unpairsink;
vector<pair<int, int>> wv;
int record = 0;
void init()
{
    Count = 1, Max = -1, unmarksource.clear(), wv.clear(), unpairsink.clear(), unpairsource.clear();
}
void Init(int n)
{
    for (int i = 1; i <= n; i++)
    {
        sink[i] = false;
        source[i] = false;
        vis[i] = false;
        sccid[i] = 0;
        marksink[i] = 0;
    }
}
void addEdge(vector<int> adj[], int u, int v) // u->v
{
    adj[u].push_back(v);
}

void AddEdge(vector<int> dag[], int u, int v) // u->v
{
    dag[u].push_back(v);
}

void printGraph(vector<int> dag[], int V)
{
    for (int v = 1; v <= V; v++)
    {
        cout << v << "->";
        for (auto x : dag[v])
            cout << x << " ";
        printf("\n");
    }
}

void SCCUtil(int u, int disc[], int low[], stack<int> *st, bool stackMember[], vector<int> adj[])
{
    static int time = 0;
    disc[u] = low[u] = ++time;
    st->push(u);
    stackMember[u] = true;
    for (auto &i : adj[u])
    {
        int v = i;
        if (disc[v] == -1)
        {
            SCCUtil(v, disc, low, st, stackMember, adj);
            low[u] = min(low[u], low[v]);
        }
        else if (stackMember[v] == true)
        {
            low[u] = min(low[u], disc[v]);
        }
    }
    int w = 0;
    if (low[u] == disc[u])
    {
        while (st->top() != u)
        {
            w = (int)st->top();
            sccid[w] = Count;
            if (Count >= Max)
                Max = Count;
            stackMember[w] = false;
            st->pop();
        }
        w = (int)st->top();
        sccid[w] = Count;
        if (Count >= Max)
            Max = Count;
        stackMember[w] = false;
        st->pop();
        Count++;
    }
}

void SCC(vector<int> adj[])
{
    int *disc = new int[n + 10];
    int *low = new int[n + 10];
    bool *stackMember = new bool[n + 10];
    stack<int> *st = new stack<int>();
    for (int i = 1; i <= n; i++)
    {
        disc[i] = -1;
        low[i] = -1;
        stackMember[i] = false;
    }
    for (int i = 1; i <= n; i++)
    {
        if (disc[i] == -1)
        {
            SCCUtil(i, disc, low, st, stackMember, adj);
        }
    }
    delete disc;
    delete low;
    delete stackMember;
}

void SEARCH(int x, vector<int> dag[])
{
    if (vis[x] == 0)
    {
        if (sink[x] == 1)
        {
            record = x;
            sinknotfound = false;
        }
        vis[x] = 1;
        for (auto y : dag[x])
        {
            if (sinknotfound == true)
                SEARCH(y, dag);
        }

        return;
    }
}

void STCORRECT(vector<int> dag[])
{
    for (int u = 1; u <= Max; u++)
    {
        vis[u] = 0;
    }
    for (int v = 1; v <= Max; v++)
    {
        if (source[v] == 0 || vis[v] == 1)
        {
            continue;
        }
        record = 0;
        sinknotfound = true;
        SEARCH(v, dag);
        if (record != 0) // pair
        {
            wv.push_back(make_pair(v, record));
            marksink[record] = true;
        }
        else // not pair
        {
            unpairsource.push_back(v);
        }
    }
}

signed main()
{
    int t, flag;
    cin >> t >> flag;
    for (int i = 0; i < t; i++)
    {
        init();
        int sourcenumber = 0, sinknumber = 0;
        int maxofsourceandsink;
        int m;
        cin >> n >> m;
        Init(n);
        vector<int> adj[n + 10];
        int u, v;
        for (int j = 1; j <= m; j++)
        {
            cin >> u >> v;
            addEdge(adj, u, v);
        }
        // cout << "ans=\n";
        SCC(adj);
        vector<int> dag[Max + 2];
        int indegree[Max + 2] = {0};
        int outdegree[Max + 2] = {0};
        int re[n + 2] = {0};
        for (int i = 1; i <= n; i++)
        {
            int SCCID = sccid[i];
            re[SCCID] = i;
        }
        for (int i = 1; i <= n; i++)
        {
            for (auto x : adj[i])
            {
                if (sccid[x] != sccid[i])
                {
                    AddEdge(dag, sccid[i], sccid[x]);
                    indegree[sccid[x]]++;
                    outdegree[sccid[i]]++;
                }
            }
        }
        for (int i = 1; i <= Max; i++)
        {
            if (indegree[i] == 0)
            {
                source[i] = true;
                sourcenumber++;
            }
            if (outdegree[i] == 0)
            {
                sink[i] = true;
                sinknumber++;
            }
        }
        maxofsourceandsink = sourcenumber;
        if (sinknumber > maxofsourceandsink)
            maxofsourceandsink = sinknumber;
        /*for (int i = 1; i <= n; i++)
        {
            cout << sccid[i] << " ";
        }*/
        // cout << "-----------\n";
        //  printGraph(dag, Max);
        //  cout << "-----------------\n";
        STCORRECT(dag);
        for (int i = 1; i <= Max; i++)
        {
            if (sink[i] == 1 && marksink[i] == false)
            {
                unpairsink.push_back(i);
            }
        }
        // unpairsink unpairsource wv source sink
        if (Max == 1)
            cout << "0\n";
        else
        {
            cout << maxofsourceandsink << "\n";
            /*cout << "whichedge\n";
            cout << "unpairsink " << unpairsink.size() << "\n";
            // re[unpairsink[0]] << endl;
            cout << "unpairsource " << unpairsource.size() << "\n";
            // re[unpairsource[0]] << endl;
            cout << "pairsize " << wv.size() << "\n";
            // re[wv[0].first] << " " << re[wv[0].second] << endl;*/
            if (flag == 1)
            {
                if (unpairsink.size() == 0 && unpairsource.size() == 0)
                {
                }
                else if (unpairsink.size() == 0) // no unpairsink
                {
                    for (int i = 0; i < unpairsource.size(); i++)
                        cout << re[wv[0].second] << " " << unpairsource[i] << "\n";
                }
                else if (unpairsource.size() == 0) // no unpairsource
                {
                    for (int i = 0; i < unpairsink.size(); i++)
                        cout << re[unpairsink[i]] << " " << re[wv[0].first] << "\n";
                }
                else if (unpairsink.size() != 0 && unpairsource.size() != 0)
                {
                    if (unpairsink.size() >= unpairsource.size()) // 6 4        0 1 2 3 4 5 0 1 2 3
                    {
                        for (int i = 0; i < unpairsource.size(); i++)
                        {
                            cout << re[unpairsink[i]] << " " << re[unpairsource[i]] << "\n";
                        }
                        for (int i = unpairsource.size(); i < unpairsink.size(); i++)
                        {
                            cout << re[unpairsink[i]] << " " << re[wv[0].first] << "\n";
                        }
                    }
                    else
                    {
                        for (int i = 0; i < unpairsink.size(); i++)
                        {
                            cout << re[unpairsink[i]] << " " << re[unpairsource[i]] << "\n";
                        }
                        for (int i = unpairsink.size(); i < unpairsource.size(); i++)
                        {
                            cout << re[wv[0].second] << " " << re[unpairsource[i]] << "\n";
                        }
                    }
                }
                if (wv.size() != 0)
                {
                    for (int i = 0; i < wv.size() - 1; i++)
                    {
                        cout << re[wv[i].second] << " " << re[wv[i + 1].first] << "\n";
                    }
                    cout << re[wv.back().second] << " " << re[wv[0].first] << "\n";
                }
            }
        }
        /*cout << "unpairsink is\n";
        for (auto &x : unpairsink)
            cout << x << " ";
        cout << "unpairsource is\n";
        for (auto &x : unpairsource)
            cout << x << " ";
        cout << "pairsink\n";
        for (auto &x : wv)
        {
            cout << x.first << " " << x.second << " ";
            cout << "\n";
        }*/
    }
}
