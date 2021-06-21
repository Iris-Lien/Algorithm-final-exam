#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

#define m 100				//螞蟻個數
#define n 20				//城市數量

const int NC_max = 100;		//最大迭代次數
const double Alpha = 1;		//決定費洛蒙與距離相對重要性參數
const double Beta = 5;
const double Rho = 0.1;		//費洛蒙衰退參數
const double Q = 100;		//費洛蒙加強度係數
const double C[n][2] =		//各城市座標數據
{
    { 37, 52 },
	{ 49, 49 },
	{ 52, 64 },
	{ 20, 26 },
	{ 40, 30 },
	{ 21, 47 },
	{ 17, 63 },
	{ 31, 62 },
	{ 52, 33 },
	{ 51, 21 },
	{ 42, 41 },
	{ 31, 32 },
	{ 5, 25 },
	{ 12, 42 },
	{ 36, 16 },
	{ 52, 41 },
	{ 27, 23 },
	{ 17, 33 },
	{ 13, 13 },
	{ 57, 58 },

};

double D[n][n];			//表示完全圖的鄰接矩阵
double Eta[n][n];		//表示啟發式因子，為D中距離的倒數
double DeltaTau[n][n];	//表示啟發式因子的變化量
double Tau[n][n];		//路徑上面信息素的濃度
int Tabu[m][n];			//儲存走過的路径表

double L_best[NC_max];		//儲存每次迭代的路的最短長度
double L_ave[NC_max];		//儲存每次迭代的路的平均長度
int R_best[NC_max][n];		//儲存每次迭代的最佳路線


void ValueInit(void)		//變量初始化函數
{
    for (int i = 0; i < n; i++)			//初始化 D[n][n]
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
                D[i][j] = pow(pow((C[i][0] - C[j][0]), 2) + pow((C[i][1] - C[j][1]), 2), 0.5);
            else
                D[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++)			//初始化 Eta[n][n]
        for (int j = 0; j < n; j++)
            Eta[i][j] = 1.0 / D[i][j];

    for (int i = 0; i < n; i++)			//初始化 DeltaEta[n][n]
        for (int j = 0; j < n; j++)
            DeltaTau[i][j] = 0;

    for (int i = 0; i < n; i++)			//初始化 Tau[n][n]
        for (int j = 0; j < n; j++)
            Tau[i][j] = 1.0;

    for (int i = 0; i < m; i++)			//初始化 Tabu[m][n]
        for (int j = 0; j < n; j++)
            Tabu[i][j] = 0;
}

void ValueDisplayTabu(int (*p)[n])	//儲存走過的路徑表
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << *(*(p + i) + j) << ' ';
        }
        cout << endl;
    }
}

void ValueDisplayTau(double(*p)[n])		//信息素的濃度
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << *(*(p + i) + j) << ' ';
        }
        cout << endl;
    }
}

double rnd(double lower, double uper)	//生成lower和uper之间的一个double類型隨機數
{
    return  (rand() / (double)RAND_MAX) * (uper - lower) + lower;
}

int main()
{
    //第一步：進行變量的初始化
    ValueInit();

    int NC = 0;
    while(NC < NC_max)
    {
        //第二步：將m只螞蟻随機放到n个城市上
        vector<int> temp;
        for (int i = 0; i < ceil((double)m / (double)n); i++)
        {
            for (int j = 0; j < n; j++)
                temp.push_back(j);
        }

        random_shuffle(temp.begin(), temp.end());	//打亂temp中元素的順序

        for (int i = 0; i < m; i++)
        {
            Tabu[i][0] = temp[i];
        }

        //第三步：m只螞蟻按照機率函數選擇n中的下一座城市，完成周游
        for (int j = 1; j < n; j++)
        {
            for (int i = 0; i < m; i++)
            {
                vector<int> visited;	//第i只螞蟻已訪問過的城市
                vector<int> J;			//第i只螞蟻待訪問的城市
                vector<double> P;		//第i只螞蟻待訪問的城市的機率

                double Psum = 0.0;		//機率值和
                double rate = 0.0;		//随機數
                double choose = 0.0;	//輪盤算法累加值
                int to_visit;			//下一個要去的城市

                for (int k = 0; k < j; k++)
                    visited.push_back(Tabu[i][k]);	//visited初始化

                for (int k = 0; k < n; k++)
                {
                    if (find(visited.begin(), visited.end(), k) == visited.end())	//在visited中没有找到t
                    {
                        J.push_back(k);				//J初始化
                        P.push_back(0.0);			//P初始化
                    }
                }

                for (int k = 0; k < P.size(); k++)	//計算去下一座城市的機率
                {
                    P[k] = pow(Tau[visited.back()][J[k]], Alpha) * pow(Eta[visited.back()][J[k]], Beta);
                    Psum += P[k];
                }

                rate = rnd(0.0, Psum);				//使用輪盤算法，挑選下一座要去的城市
                for (int k = 0; k < P.size(); k++)
                {
                    choose += P[k];
                    if (choose > rate)
                    {
                        to_visit = J[k];
                        break;
                    }
                }

                Tabu[i][j] = to_visit;				//更新table
            }
        }

        //第四步：紀錄本次迭代螞蟻行走的路線數據
        double L[m];	//紀錄本代每只螞蟻走的路程、初始化
        for (int i = 0; i < m; i++)
        {
            L[i] = 0.0;
        }
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n - 1; j++)
            {
                L[i] += D[Tabu[i][j]][Tabu[i][j + 1]];
            }
            L[i] += D[Tabu[i][0]][Tabu[i][n - 1]];
        }

        double min_value = L[0];	//每次迭代所有螞蟻行走距離最小值的臨時變量
        double sum_value = L[0];	//每次迭代所有螞蟻行走距離總值的臨時變量
        int min_index = 0;			//每次迭代所有螞蟻行走距離最小值的下限
        for (int i = 1; i < m; i++)
        {
            sum_value += L[i];
            if (L[i] < min_value)
            {
                min_value = L[i];
                min_index = i;
            }
        }

        L_best[NC] = min_value;						//每次迭代中路徑的最短長度
        L_ave[NC] = sum_value / m;					//每次迭代中路徑的平均長度

        for (int i = 0; i < n; i++)
        {
            R_best[NC][i] = Tabu[min_index][i];		//每次迭代最短的路徑數據
        }

       //cout << NC << ": Best is " << L_best[NC] << ' ' << "Average is " << L_ave[NC] << endl;	//輸出每次迭代距離信息

        NC++;	//迭代继续

        //第五步：更新信息素
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n - 1; j++)
            {
                DeltaTau[Tabu[i][j]][Tabu[i][j + 1]] += Q / L[i];	//此次循環在整個路徑上的信息素增量
            }
            DeltaTau[Tabu[i][n - 1]][Tabu[i][0]] += Q / L[i];
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                Tau[i][j] = (1 - Rho) * Tau[i][j] + DeltaTau[i][j];	//考慮信息素揮發，更新後的信息素
            }
        }

        for (int i = 0; i < m; i++)			//初始化table
            for (int j = 0; j < n; j++)
                Tabu[i][j] = 0;
    }

    //第六步：把结果輸出出来
    double min_L = L_best[0];			//所有迭代中最短距离
    int min_L_index = 0;				//所有迭代中最優路径的下限
    int Shortest_Route[n];				//所有迭代中的最優路径
    for (int i = 0; i < NC; i++)
    {
        if (L_best[i] < min_L)
        {
            min_L = L_best[i];
            min_L_index = i;
        }
    }

    cout << min_L << endl;  //輸出最短路徑長

    return 0;
}
