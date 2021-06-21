
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std ;

typedef struct pix   //Define structure store data
{
    double  x, y ;
} Pix;

Pix Date[100000];
double Distance(int a,int b)
{
    return double (sqrt((Date[a].x-Date[b].x)*(Date[a].x-Date[b].x) + (Date[a].y-Date[b].y)*(Date[a].y-Date[b].y)));
}

int main ()
{
    int n ;
    cin >> n;


    for (int i=1 ; i <= n ; i++) //Enter the data, because the title data is already sorted by x, so there is no need to sort
    {
        cin >> Date[i].x >> Date[i].y;
    }

    double dp[n+1][n+1]; //State transition equation
    //dp[i][j] i<=j means the shortest path of i->1 1-j
    dp[1][2] = Distance (1,2);//initialization

    for (int j = 3 ; j<=n; j++) //(All are gradually recursive processes)
    {
        // when i <j-1
        for (int i=1 ; i<=j-2; i++)
        {
            dp[i][j] = dp[i][j-1] + Distance(j,j-1);
        }
        dp[j-1][j] = 1<<30;
        //i == j-1 calculate the smallest dp[j-1][j] from j=3 to j=n
        for (int k = 1 ; k <= j-2; k++)
        {
            dp[j-1][j] = min(dp[j-1][j], dp[k][j-1] + Distance(k,j) );
        }
    }
    //Finally calculate the answer
    dp[n][n] = dp[n-1][n] + Distance(n-1,n);
    printf ("%.3f\n", dp[n][n]);

    return 0;
}

/*

37 52
49 49
52 64
20 26
40 30
21 47
17 63
31 62
52 33
51 21
42 41
31 32
5 25
12 42
36 16
52 41
27 23
17 33
13 13
57 58

*/
