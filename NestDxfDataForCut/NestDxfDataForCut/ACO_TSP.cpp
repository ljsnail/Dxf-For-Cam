// acotest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include<iostream>
#include<math.h>
#include<time.h>
//输出相关
#include <fstream>

#include <iostream>

#include <conio.h>
#include<iosfwd>
using namespace std;

//该程序是以蚁群系统为模型写的蚁群算法程序(强调：非蚂蚁周模型)，以三个著名的TSP问题为测试对象
//通过微调参数，都可以获得较好的解

/*
//----------(1)问题一：Oliver 30 城市 TSP 问题 best_length = 423.7406; ------------------------
//该程序最好的结果是423.741，可运行多次获得
//城市节点数目
#define N 30
//城市坐标
double C[N][2]={
	{2,99},{4,50},{7,64},{13,40},{18,54},{18,40},{22,60},{24,42},{25,62},{25,38},
	{37,84},{41,94},{41,26},{44,35},{45,21},{54,67},{54,62},{58,35},{58,69},{62,32},
	{64,60},{68,58},{71,44},{71,71},{74,78},{82,7},{83,46},{83,69},{87,76},{91,38}
};
//----------上面参数是固定的，下面的参数是可变的-----------
//蚂蚁数量
#define M 30
//最大循环次数NcMax
int NcMax = 500;
//信息启发因子，期望启发式因子，全局信息素挥发参数，局部信息素挥发参数, 状态转移公式中的q0
double alpha = 2, beta = 3, rou = 0.1, alpha1 = 0.1,  qzero = 0.01;
//-----------问题一结束------------------------------------------------------------------------
*/


//----------(2)问题二：Elion50 城市 TSP 问题 best_length = 427.96; ----------------------------
//该程序最好的结果是428.468，可运行多次获得
//城市节点数目
#define N 50
//城市坐标
double C[N][2]={
	{5,64}, {5,25}, {5,6}, {7,38}, {8,52}, {10,17},
	{12,42}, {13,13}, {16,57}, {17,33}, {17,63},
	{20,26}, {21,47}, {21,10}, {25,32}, {25,55},
	{27,68}, {27,23}, {30,48}, {30,15}, {31,62},
	{31,32}, {32,22}, {32,39}, {36,16}, {37,69},
	{37,52}, {38,46}, {39,10}, {40,30}, {42,57},
	{42,41}, {43,67}, {45,35}, {46,10}, {48,28},
	{49,49}, {51,21}, {52,33}, {52,41}, {52,64},
	{56,37}, {57,58}, {58,27}, {58,48}, {59,15},
	{61,33}, {62,42}, {62,63}, {63,69}
};
//----------上面参数是固定的，下面的参数是可变的-----------
//蚂蚁数量
#define M 50
//最大循环次数NcMax
int NcMax = 100;
//信息启发因子，期望启发式因子，全局信息素挥发参数，局部信息素挥发参数, 状态转移公式中的q0
double alpha = 2, beta = 4, rou = 0.1, alpha1 = 0.1,  qzero = 0.01;
//-----------问题二结束------------------------------------------------------------------------


//----------(3)问题三：Elion75 城市 TSP 问题 best_length = 542.31;
//该程序最好的结果是542.309，可运行多次获得 
//城市节点数目
//#define N 5
//城市坐标
//double C[N][2]={
//{6,25}, {7,43}, {9,56}, {10,70}, {11,28}
//};
//----------上面参数是固定的，下面的参数是可变的-----------
//蚂蚁数量
//#define M 5
//最大循环次数NcMax
//int NcMax =10;
//信息启发因子，期望启发式因子，全局信息素挥发参数，局部信息素挥发参数, 状态转移公式中的q0
//double alpha = 2, beta = 5, rou = 0.1, alpha1 = 0.1,  qzero = 0.1;
//-----------问题三结束------------------------------------------------------------------------


//===========================================================================================================
//局部更新时候使用的的常量，它是由最近邻方法得到的一个长度
//什么是最近邻方法?:)就是从源节点出发，每次选择一个距离最短的点来遍历所有的节点得到的路径
//每个节点都可能作为源节点来遍历
double Lnn;
//矩阵表示两两城市之间的距离
double allDistance[N][N];

//计算两个城市之间的距离
double calculateDistance(int i, int j)
{
	return sqrt(pow((C[i][0]-C[j][0]),2.0) + pow((C[i][1]-C[j][1]),2.0));
}

//由矩阵表示两两城市之间的距离
void calculateAllDistance()
{
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			if (i != j)
			{
				allDistance[i][j] = calculateDistance(i, j);
				allDistance[j][i] = allDistance[i][j];
			}
		}
	}
}

//获得经过n个城市的路径长度
double calculateSumOfDistance(int* tour)
{
	double sum = 0;
	for(int i = 0; i< N ;i++)
	{
		int row = *(tour + 2 * i);
		int col = *(tour + 2* i + 1);
		sum += allDistance[row][col];
	}
	return sum;
}

class ACSAnt;

class AntColonySystem
{
private:	
	double info[N][N], visible[N][N];//节点之间的信息素强度,节点之间的能见度
public:	
	AntColonySystem()
	{
	}
	//计算当前节点到下一节点转移的概率
	double Transition(int i, int j);	
	//局部更新规则
	void UpdateLocalPathRule(int i, int j);	
	//初始化
	void InitParameter(double value);	
	//全局信息素更新
	void UpdateGlobalPathRule(int* bestTour, int globalBestLength);
};

//计算当前节点到下一节点转移的概率
double AntColonySystem::Transition(int i, int j)
{
	if (i != j)
	{
		return (pow(info[i][j],alpha) * pow(visible[i][j], beta));
	}
	else
	{
		return 0.0;
	}	
}
//局部更新规则
void AntColonySystem::UpdateLocalPathRule(int i, int j)
{
	info[i][j] = (1.0 - alpha1) * info[i][j] + alpha1 * (1.0 / (N * Lnn));
	info[j][i] = info[i][j];
}
//初始化
void AntColonySystem::InitParameter(double value)
{
	//初始化路径上的信息素强度tao0
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{				
			info[i][j] = value;
			info[j][i] = value;
			if (i != j)
			{
				visible[i][j] = 1.0 / allDistance[i][j];
				visible[j][i] = visible[i][j];
			}
		}
	}	
}

//全局信息素更新
void AntColonySystem::UpdateGlobalPathRule(int* bestTour, int globalBestLength)
{
	for(int i = 0; i < N; i++)
	{
		int row = *(bestTour + 2 * i);
		int col = *(bestTour + 2* i + 1);
		info[row][col] = (1.0 - rou) * info[row][col] + rou * (1.0 / globalBestLength);
		info[col][row] =info[row][col];
	}
}

class ACSAnt
{
private:
	AntColonySystem* antColony;
protected:
	int startCity, cururentCity;//初始城市编号，当前城市编号
	int allowed[N];//禁忌表	
	int Tour[N][2];//当前路径
	int currentTourIndex;//当前路径索引，从0开始，存储蚂蚁经过城市的编号
public:	
	ACSAnt(AntColonySystem* acs, int start)
	{
		antColony = acs; 
		startCity = start;
	}	
	//开始搜索
	int* Search();
	//选择下一节点
	int Choose();
	//移动到下一节点
	void MoveToNextCity(int nextCity);

};

//开始搜索
int* ACSAnt::Search()
{
	cururentCity = startCity;
	int toCity;
	currentTourIndex = 0;
	for(int i  = 0; i < N; i++)
	{
		allowed[i] = 1;
	}
	allowed[cururentCity] = 0;
	int endCity;
	int count = 0;
	do
	{
		count++;
		endCity = cururentCity;
		toCity = Choose();		
		if (toCity >= 0)
		{			
			MoveToNextCity(toCity);
			antColony->UpdateLocalPathRule(endCity, toCity);
			cururentCity = toCity;
		}		
	}while(toCity >= 0);
	MoveToNextCity(startCity);
	antColony->UpdateLocalPathRule(endCity, startCity);

	return *Tour;
}

//选择下一节点
int ACSAnt::Choose()
{
	int nextCity = -1;		
	double q = rand()/(double)RAND_MAX;
	//如果 q <= q0,按先验知识，否则则按概率转移，
	if (q <= qzero)
	{
		double probability = -1.0;//转移到下一节点的概率
		for(int i = 0; i < N; i++)
		{
			//去掉禁忌表中已走过的节点,从剩下节点中选择最大概率的可行节点
			if (1 == allowed[i])
			{
				double prob = antColony->Transition(cururentCity, i);
				if (prob  > probability)
				{
					nextCity = i;
					probability = prob;
				}
			}
		}
	}
	else
	{
		//按概率转移			
		double p = rand()/(double)RAND_MAX;//生成一个随机数,用来判断落在哪个区间段
		double sum = 0.0;			
		double probability = 0.0;//概率的区间点，p 落在哪个区间段，则该点是转移的方向
		//计算概率公式的分母的值
		for(int i = 0; i < N; i++)
		{
			if (1 == allowed[i])
			{
				sum += antColony->Transition(cururentCity, i);
			}
		}
		for(int j = 0; j < N; j++)
		{
			if (1 == allowed[j] && sum > 0)
			{
				probability += antColony->Transition(cururentCity, j)/sum;
				if (probability >= p || (p > 0.9999 && probability > 0.9999))
				{
					nextCity = j;
					break;
				}
			}
		}	
	}	
	return nextCity;
}

//移动到下一节点
void ACSAnt::MoveToNextCity(int nextCity)
{
	allowed[nextCity]=0;
	Tour[currentTourIndex][0] = cururentCity;
	Tour[currentTourIndex][1] = nextCity;
	currentTourIndex++;
	cururentCity = nextCity;
}

//------------------------------------------
//选择下一个节点，配合下面的函数来计算的长度
int ChooseNextNode(int currentNode, int visitedNode[])
{
	int nextNode = -1;		
	double shortDistance = 0.0;
	for(int i = 0; i < N; i++)
	{
		//去掉已走过的节点,从剩下节点中选择距离最近的节点
		if (1 == visitedNode[i])
		{			
			if (shortDistance == 0.0)
			{
				shortDistance = allDistance[currentNode][i];
				nextNode = i;
			}
			if(shortDistance < allDistance[currentNode][i])
			{
				nextNode = i;
			}
		}
	}
	return nextNode;
}

//给一个节点由最近邻距离方法计算长度
double CalAdjacentDistance(int node)
{
	double sum = 0.0;
	int visitedNode[N];
	for(int j = 0; j < N; j++)
	{
		visitedNode[j] = 1; 
	}
	visitedNode[node] = 0;
	int currentNode = node;
	int nextNode;
	do
	{
		nextNode = ChooseNextNode(currentNode, visitedNode);
		if (nextNode >= 0)
		{
			sum += allDistance[currentNode][nextNode];
			currentNode= nextNode;
			visitedNode[currentNode] = 0;
		}		
	}while(nextNode >= 0);
	sum += allDistance[currentNode][node];
	return sum;
}

//---------------------------------结束---------------------------------------------

//--------------------------主函数--------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	int a=3;
	time_t timer,timerl;
ofstream outfile("I:\\MATLAB\\DXF\\TSP01.txt");
	time(&timer);
	unsigned long seed = timer;
	seed %= 56000;
	srand((unsigned int)seed);

	//由矩阵表示两两城市之间的距离
	calculateAllDistance();
	//蚁群系统对象
	AntColonySystem* acs = new AntColonySystem();
	ACSAnt* ants[M];
	//蚂蚁均匀分布在城市上
	for(int k = 0; k < M; k++)
	{
		ants[k] = new ACSAnt(acs, (int)(k%N));
	}
	calculateAllDistance();
	//随机选择一个节点计算由最近邻方法得到的一个长度
	int node = rand() % N;
	Lnn = CalAdjacentDistance(node);
	
	//各条路径上初始化的信息素强度
	double initInfo = 1 / (N * Lnn);
	acs->InitParameter(initInfo);	
	
	//全局最优路径
	int globalTour[N][2];
	//全局最优长度
	double globalBestLength = 0.0;	
	for(int i = 0; i < NcMax; i++)
	{
		//局部最优路径
		int localTour[N][2];
		//局部最优长度
		double localBestLength = 0.0;
		//当前路径长度
		double tourLength;
		for(int j = 0; j < M; j++)
		{
			int* tourPath = ants[j]->Search();
			tourLength = calculateSumOfDistance(tourPath);				
			//局部比较，并记录路径和长度
			if(tourLength < localBestLength || abs(localBestLength - 0.0) < 0.000001)
			{				
				for(int m = 0; m< N; m++)
				{
					int row = *(tourPath + 2 * m);
					int col = *(tourPath + 2* m + 1);
					localTour[m][0] = row;
					localTour[m][1] = col;
				}
				localBestLength = tourLength;			
			}
		}
		//全局比较，并记录路径和长度
		if(localBestLength < globalBestLength || abs(globalBestLength - 0.0) < 0.000001)
		{				
			for(int m = 0; m< N; m++)
			{
				globalTour[m][0] = localTour[m][0];
				globalTour[m][1] = localTour[m][1];
			}
			globalBestLength = localBestLength;	
		}
		acs->UpdateGlobalPathRule(*globalTour, globalBestLength);
		//输出所有蚂蚁循环一次后的迭代最优路径
		//cout<<"第 "<<i + 1<<" 迭代最优路径:"<<localBestLength<<"."<<endl;
		int b=0;
		for(int m = 0; m< N-1; m++)
		{
			int i=localTour[m][0];
			int j=0;
			j=localTour[m+1][0];
			
          b++;
			outfile<<1<<"    "<<C[i][0] << "    "<<C[i][1]<< "    "<<C[j][0] << "    "<<C[j][1]<<endl;
			if((m+1)==N-1)
			{
			int i=localTour[0][0];
			
			j=localTour[N-1][0];
			outfile<< 1<<"    "<<C[j][0] << "    "<<C[j][1]<< "    "<<C[i][0] << "    "<<C[i][1]<<endl;
			}
		}
		outfile<<a++<<endl;		
	}	
	//输出全局最优路径
	/*outfile<<"全局最优路径长度:"<<globalBestLength<<endl;	
	outfile<<"全局最优路径:";*/
	for(int m = 0; m< N-1; m++)
	{
		//outfile<<globalTour[m][0]<<".";
			int i=globalTour[m][0];
			int j=0;
			
			j=globalTour[m+1][0];
			
			
			outfile<< 2<<"    "<<C[i][0] << "    "<<C[i][1]<< "    "<<C[j][0] << "    "<<C[j][1]<<endl;
			if((m+1)==N-1)
			{
			int i=globalTour[0][0];
			j=globalTour[N-1][0];
			outfile<< 2<<"    "<<C[j][0] << "    "<<C[j][1]<< "    "<<C[i][0] << "    "<<C[i][1]<<endl;
			}
		
	}
	outfile<<a++<<endl;
	time(&timerl);
	int t = timerl - timer;
	return 0;
}
//--------------------------主函数结束------------------------------------

