// acotest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include<iostream>
#include<math.h>
#include<time.h>
//������
#include <fstream>

#include <iostream>

#include <conio.h>
#include<iosfwd>
using namespace std;

//�ó���������ȺϵͳΪģ��д����Ⱥ�㷨����(ǿ������������ģ��)��������������TSP����Ϊ���Զ���
//ͨ��΢�������������Ի�ýϺõĽ�

/*
//----------(1)����һ��Oliver 30 ���� TSP ���� best_length = 423.7406; ------------------------
//�ó�����õĽ����423.741�������ж�λ��
//���нڵ���Ŀ
#define N 30
//��������
double C[N][2]={
	{2,99},{4,50},{7,64},{13,40},{18,54},{18,40},{22,60},{24,42},{25,62},{25,38},
	{37,84},{41,94},{41,26},{44,35},{45,21},{54,67},{54,62},{58,35},{58,69},{62,32},
	{64,60},{68,58},{71,44},{71,71},{74,78},{82,7},{83,46},{83,69},{87,76},{91,38}
};
//----------��������ǹ̶��ģ�����Ĳ����ǿɱ��-----------
//��������
#define M 30
//���ѭ������NcMax
int NcMax = 500;
//��Ϣ�������ӣ���������ʽ���ӣ�ȫ����Ϣ�ػӷ��������ֲ���Ϣ�ػӷ�����, ״̬ת�ƹ�ʽ�е�q0
double alpha = 2, beta = 3, rou = 0.1, alpha1 = 0.1,  qzero = 0.01;
//-----------����һ����------------------------------------------------------------------------
*/


//----------(2)�������Elion50 ���� TSP ���� best_length = 427.96; ----------------------------
//�ó�����õĽ����428.468�������ж�λ��
//���нڵ���Ŀ
#define N 50
//��������
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
//----------��������ǹ̶��ģ�����Ĳ����ǿɱ��-----------
//��������
#define M 50
//���ѭ������NcMax
int NcMax = 100;
//��Ϣ�������ӣ���������ʽ���ӣ�ȫ����Ϣ�ػӷ��������ֲ���Ϣ�ػӷ�����, ״̬ת�ƹ�ʽ�е�q0
double alpha = 2, beta = 4, rou = 0.1, alpha1 = 0.1,  qzero = 0.01;
//-----------���������------------------------------------------------------------------------


//----------(3)��������Elion75 ���� TSP ���� best_length = 542.31;
//�ó�����õĽ����542.309�������ж�λ�� 
//���нڵ���Ŀ
//#define N 5
//��������
//double C[N][2]={
//{6,25}, {7,43}, {9,56}, {10,70}, {11,28}
//};
//----------��������ǹ̶��ģ�����Ĳ����ǿɱ��-----------
//��������
//#define M 5
//���ѭ������NcMax
//int NcMax =10;
//��Ϣ�������ӣ���������ʽ���ӣ�ȫ����Ϣ�ػӷ��������ֲ���Ϣ�ػӷ�����, ״̬ת�ƹ�ʽ�е�q0
//double alpha = 2, beta = 5, rou = 0.1, alpha1 = 0.1,  qzero = 0.1;
//-----------����������------------------------------------------------------------------------


//===========================================================================================================
//�ֲ�����ʱ��ʹ�õĵĳ���������������ڷ����õ���һ������
//ʲô������ڷ���?:)���Ǵ�Դ�ڵ������ÿ��ѡ��һ��������̵ĵ����������еĽڵ�õ���·��
//ÿ���ڵ㶼������ΪԴ�ڵ�������
double Lnn;
//�����ʾ��������֮��ľ���
double allDistance[N][N];

//������������֮��ľ���
double calculateDistance(int i, int j)
{
	return sqrt(pow((C[i][0]-C[j][0]),2.0) + pow((C[i][1]-C[j][1]),2.0));
}

//�ɾ����ʾ��������֮��ľ���
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

//��þ���n�����е�·������
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
	double info[N][N], visible[N][N];//�ڵ�֮�����Ϣ��ǿ��,�ڵ�֮����ܼ���
public:	
	AntColonySystem()
	{
	}
	//���㵱ǰ�ڵ㵽��һ�ڵ�ת�Ƶĸ���
	double Transition(int i, int j);	
	//�ֲ����¹���
	void UpdateLocalPathRule(int i, int j);	
	//��ʼ��
	void InitParameter(double value);	
	//ȫ����Ϣ�ظ���
	void UpdateGlobalPathRule(int* bestTour, int globalBestLength);
};

//���㵱ǰ�ڵ㵽��һ�ڵ�ת�Ƶĸ���
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
//�ֲ����¹���
void AntColonySystem::UpdateLocalPathRule(int i, int j)
{
	info[i][j] = (1.0 - alpha1) * info[i][j] + alpha1 * (1.0 / (N * Lnn));
	info[j][i] = info[i][j];
}
//��ʼ��
void AntColonySystem::InitParameter(double value)
{
	//��ʼ��·���ϵ���Ϣ��ǿ��tao0
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

//ȫ����Ϣ�ظ���
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
	int startCity, cururentCity;//��ʼ���б�ţ���ǰ���б��
	int allowed[N];//���ɱ�	
	int Tour[N][2];//��ǰ·��
	int currentTourIndex;//��ǰ·����������0��ʼ���洢���Ͼ������еı��
public:	
	ACSAnt(AntColonySystem* acs, int start)
	{
		antColony = acs; 
		startCity = start;
	}	
	//��ʼ����
	int* Search();
	//ѡ����һ�ڵ�
	int Choose();
	//�ƶ�����һ�ڵ�
	void MoveToNextCity(int nextCity);

};

//��ʼ����
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

//ѡ����һ�ڵ�
int ACSAnt::Choose()
{
	int nextCity = -1;		
	double q = rand()/(double)RAND_MAX;
	//��� q <= q0,������֪ʶ�������򰴸���ת�ƣ�
	if (q <= qzero)
	{
		double probability = -1.0;//ת�Ƶ���һ�ڵ�ĸ���
		for(int i = 0; i < N; i++)
		{
			//ȥ�����ɱ������߹��Ľڵ�,��ʣ�½ڵ���ѡ�������ʵĿ��нڵ�
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
		//������ת��			
		double p = rand()/(double)RAND_MAX;//����һ�������,�����ж������ĸ������
		double sum = 0.0;			
		double probability = 0.0;//���ʵ�����㣬p �����ĸ�����Σ���õ���ת�Ƶķ���
		//������ʹ�ʽ�ķ�ĸ��ֵ
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

//�ƶ�����һ�ڵ�
void ACSAnt::MoveToNextCity(int nextCity)
{
	allowed[nextCity]=0;
	Tour[currentTourIndex][0] = cururentCity;
	Tour[currentTourIndex][1] = nextCity;
	currentTourIndex++;
	cururentCity = nextCity;
}

//------------------------------------------
//ѡ����һ���ڵ㣬�������ĺ���������ĳ���
int ChooseNextNode(int currentNode, int visitedNode[])
{
	int nextNode = -1;		
	double shortDistance = 0.0;
	for(int i = 0; i < N; i++)
	{
		//ȥ�����߹��Ľڵ�,��ʣ�½ڵ���ѡ���������Ľڵ�
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

//��һ���ڵ�������ھ��뷽�����㳤��
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

//---------------------------------����---------------------------------------------

//--------------------------������--------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	int a=3;
	time_t timer,timerl;
ofstream outfile("I:\\MATLAB\\DXF\\TSP01.txt");
	time(&timer);
	unsigned long seed = timer;
	seed %= 56000;
	srand((unsigned int)seed);

	//�ɾ����ʾ��������֮��ľ���
	calculateAllDistance();
	//��Ⱥϵͳ����
	AntColonySystem* acs = new AntColonySystem();
	ACSAnt* ants[M];
	//���Ͼ��ȷֲ��ڳ�����
	for(int k = 0; k < M; k++)
	{
		ants[k] = new ACSAnt(acs, (int)(k%N));
	}
	calculateAllDistance();
	//���ѡ��һ���ڵ����������ڷ����õ���һ������
	int node = rand() % N;
	Lnn = CalAdjacentDistance(node);
	
	//����·���ϳ�ʼ������Ϣ��ǿ��
	double initInfo = 1 / (N * Lnn);
	acs->InitParameter(initInfo);	
	
	//ȫ������·��
	int globalTour[N][2];
	//ȫ�����ų���
	double globalBestLength = 0.0;	
	for(int i = 0; i < NcMax; i++)
	{
		//�ֲ�����·��
		int localTour[N][2];
		//�ֲ����ų���
		double localBestLength = 0.0;
		//��ǰ·������
		double tourLength;
		for(int j = 0; j < M; j++)
		{
			int* tourPath = ants[j]->Search();
			tourLength = calculateSumOfDistance(tourPath);				
			//�ֲ��Ƚϣ�����¼·���ͳ���
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
		//ȫ�ֱȽϣ�����¼·���ͳ���
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
		//�����������ѭ��һ�κ�ĵ�������·��
		//cout<<"�� "<<i + 1<<" ��������·��:"<<localBestLength<<"."<<endl;
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
	//���ȫ������·��
	/*outfile<<"ȫ������·������:"<<globalBestLength<<endl;	
	outfile<<"ȫ������·��:";*/
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
//--------------------------����������------------------------------------

