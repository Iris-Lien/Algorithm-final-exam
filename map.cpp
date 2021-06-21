#include<bits/stdc++.h>

using namespace std;
int dx[4]= {-1,1,0,0};
int dy[4]= {0,0,-1,1};
int maxMinefield(vector<vector<int>>&Map)
{

    int n=Map.size(),m=Map[0].size();

    vector<vector<int>>vis(n,vector<int>(m,0));

    int res=0;

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            if(Map[i][j]==1 && vis[i][j]==0)
            {
                int count=0;
                stack<pair<int,int>>s;
                s.push({i,j});
                vis[i][j]=1;
                while(s.size()>0)
                {
                    count++;
                    int x=s.top().first;
                    int y=s.top().second;
                    s.pop();
                    for(int l=0; l<4; l++)
                    {
                        int u=x+dx[l];
                        int v=y+dy[l];
                        if(u>=0 && u<n && v>=0 && v<m && Map[u][v]==1 && vis[u][v]==0)
                        {
                            vis[u][v]=1;
                            s.push({u,v});
                        }
                    }
                }
                res=max(res,count);
            }
        }
    }
    return res;
}

int main()

{
    int n,m;
    //cout << "輸入Map大小 (row,column): ";
    cin >> n >> m;
    vector<vector<int>>Map(n,vector<int>(m,0));
    //cout << "輸入資料" << endl;
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            cin >> Map[i][j];
        }
    }
    cout << maxMinefield(Map) << endl;
}
