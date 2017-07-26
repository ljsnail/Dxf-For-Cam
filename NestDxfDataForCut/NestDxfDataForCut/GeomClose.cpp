#include "stdafx.h"
#include "GeomClose.h"
#include "math.h"
#define EPSILON 0.018
GeomClose::GeomClose()
{
	m_NumGeomClose = 0;
}


GeomClose::~GeomClose()
{
}
//初始化一块内存来存放整个图元标准的数据格式GeomStandData
//最底层的一个链表
GeomEleNode* GeomClose::CreatGeomEleNode(GeomStandData m_geomstandData)
{
	GeomEleNode*newNode = (GeomEleNode*)malloc(sizeof(GeomEleNode));
	newNode->m_GeomStandData = m_geomstandData;//把数据保存起来
	newNode->prevGeomeleNode = NULL;
	newNode->nextGeomeleNode = NULL;
	newNode->m_AccptGeomEleNode = false;
	newNode->m_NumGeomEleID=m_geomstandData.m_GeomEleID;//这是这张排样图的第几个基本图元
	newNode->m_NumGeomCloseID = 0;
	m_NumGeomEle++;
	return newNode;

}
GeomCloseHEAD*GeomClose::CreatGeomCloseHEAD(int GEOMCLOSE_ID)
{
	GeomCloseHEAD*newNode = (GeomCloseHEAD*)malloc(sizeof(GeomCloseHEAD));
	newNode->m_tranline = { 0.0, 0.0, 0.0, 0.0 };
	newNode->m_geomclose_startpoint = { 0.0, 0.0 };
	newNode->GEOMCLOSE_ID = GEOMCLOSE_ID;
	newNode->m_NumGeomele = 0;
	newNode->FirstGeomele = NULL;
	newNode->FirstInsideGCloseNode = NULL;
	newNode->prevOutsideGCloseNode = NULL;
	newNode->prevGeomcloseNode = NULL;
	newNode->nextGeomcloseNode = NULL;
	newNode->m_AcceptGeomcloseHEAD = false;//起初时候并没有收录
	newNode->m_IfGeomCloseIntact = false;//默认为不完整。
	newNode->m_KidAcceptGeomCloseHead = false;//默认为没有读取
	newNode->m_Singlelayer = true; //默认是单层，最外边的框框不能算一层，从里面排列的第一层开始算起为单层
	//newNode->m_GemoClosedLimt = { 0.0, 0.0, 0.0, 0.0 };//初始化的值
	return newNode;
}
GeomCloseHEAD*GeomClose::InsertGeomEleNode(GeomCloseHEAD*head, GeomEleNode *node, GeomStandData m_geomstandData)
{
	GeomEleNode*pStart;//起起相同
	GeomEleNode*pEnd;//止止相同
	GeomEleNode*pEStrat;//现在数据的终点和原来数据的起点是相同的
	GeomEleNode*pSEnd;//现在数据的起点和原来数据的终点是相同的
	GeomStandData NewData;//调换起点终点的位置
	GeomEleNode*temp;//过渡结点
	//结点挂上来的时候分空表和已经有数据的表。
	//如果是空表，那么就直接把数据挂上去
	if (!(head->FirstGeomele))
	{
		head->FirstGeomele = node;
		head->m_geomclose_startpoint.colse_start_x0 = node->m_GeomStandData.GeoEle_start_x0;//既然是空表，那么这个就是头结点，所以就把头结点的起始点存放first结点里。
		head->m_geomclose_startpoint.colse_start_y0 = node->m_GeomStandData.GeoEle_start_y0;
		head->m_NumGeomele++;//表示现在有了一个图元
		m_NumGeomClose++;//每新挂一个空表，说明是一个新的封闭环
		head->FirstGeomele->m_NumGeomCloseID = m_NumGeomClose;
		return head;
	}
	/*如果不是空表，那么要根据现在的数据和原来的数据之间的关系进行数据调换和找到插入点。
	判断是不是起起重合，往前插，如果是那么要将现有的数据前后调换，并插入到原有结点的前面
	往前插*/
	pStart = FindRelatGmElNd_xyStart(head, m_geomstandData);//单单是只有起起重合的关系
	if (pStart)
	{
		if (!(pStart->prevGeomeleNode))//如果pstart是头结点的情况下,就要把head->first改变
		{
			NewData = DataSwap(m_geomstandData);
			node->m_GeomStandData = NewData;
			head->FirstGeomele = node;
			node->nextGeomeleNode = pStart;
			pStart->prevGeomeleNode = node;
			head->m_NumGeomele++;//表示现在有了一个图元
			node->m_NumGeomCloseID = pStart->m_NumGeomCloseID;//这是重点，既然现在的数据跟原来的数据有相同的关系，那么就该是同属一个闭环
			return head;
		}
		else//如果pstart不是头结点
		{
			NewData = DataSwap(m_geomstandData);
			node->m_GeomStandData = NewData;//覆盖原来的数据
			node->prevGeomeleNode = pStart->prevGeomeleNode;
			pStart->prevGeomeleNode->nextGeomeleNode = node;//这一行不能与下一行调换顺序
			pStart->prevGeomeleNode = node;
			node->nextGeomeleNode = pStart;
			head->m_NumGeomele++;//表示现在有了一个图元
			node->m_NumGeomCloseID = pStart->m_NumGeomCloseID;//这是重点，既然现在的数据跟原来的数据有相同的关系，那么就该是同属一个闭环
			return head;
		}
	}
	//如果不是空表
	//如果现在的数据和原来的数据是终点和终点重合，
	//往后插，新数据要前后换位。这里要注意是不是尾结点
	//往后插
	pEnd = FindRelatGmElNd_xyEnd(head, m_geomstandData);
	if (pEnd)
	{
		if (!(pEnd->nextGeomeleNode))//是尾结点
		{
			NewData = DataSwap(m_geomstandData);
			node->m_GeomStandData = NewData;
			node->prevGeomeleNode = pEnd;
			pEnd->nextGeomeleNode = node;
			head->m_NumGeomele++;//表示现在有了一个图元
			node->m_NumGeomCloseID = pEnd->m_NumGeomCloseID;//这是重点，既然现在的数据跟原来的数据有相同的关系，那么就该是同属一个闭环
			return head;
		}
		else//不是尾结点
		{
			NewData = DataSwap(m_geomstandData);
			node->m_GeomStandData = NewData;
			node->prevGeomeleNode = pEnd;
			node->nextGeomeleNode = pEnd->nextGeomeleNode;
			pEnd->nextGeomeleNode->prevGeomeleNode = node;
			pEnd->nextGeomeleNode = node;
			head->m_NumGeomele++;//表示现在有了一个图元
			node->m_NumGeomCloseID = pEnd->m_NumGeomCloseID;//这是重点，既然现在的数据跟原来的数据有相同的关系，那么就该是同属一个闭环
			return head;
		}
	}
	//判断是不是现在数据的终点和原来数据的起点是相同的情况
	//往前插，要注意头结点
	pEStrat = FindRelatGmElNd_xyEStart(head,m_geomstandData);
	if (pEStrat)
	{
		if (!(pEStrat->prevGeomeleNode))
		{
			head->FirstGeomele = node;
			node->nextGeomeleNode = pEStrat;
			pEStrat->prevGeomeleNode = node;
			head->m_NumGeomele++;//表示现在有了一个图元
			node->m_NumGeomCloseID = pEStrat->m_NumGeomCloseID;//这是重点，既然现在的数据跟原来的数据有相同的关系，那么就该是同属一个闭环
			return head;
		}
		else//如果不是头结点
		{
			node->nextGeomeleNode = pEStrat;
			node->prevGeomeleNode = pEStrat->prevGeomeleNode;
			pEStrat->prevGeomeleNode->nextGeomeleNode = node;//与下面行不可调换
			pEStrat->prevGeomeleNode = node;
			head->m_NumGeomele++;//表示现在有了一个图元
			node->m_NumGeomCloseID = pEStrat->m_NumGeomCloseID;//这是重点，既然现在的数据跟原来的数据有相同的关系，那么就该是同属一个闭环
			return head;
		}
	}
	//如果不是空表
	//判断是不是现有数据的起点和原有数据的终点相同，往后插，不同调换数据位置
	//往后插
	pSEnd = FindRelatGmElNd_xySEnd(head, m_geomstandData);
	if (pSEnd)
	{
		//往后插的都要判断尾结点
		if (!(pSEnd->nextGeomeleNode))
		{
			node->prevGeomeleNode = pSEnd;
			pSEnd->nextGeomeleNode = node;
			head->m_NumGeomele++;//表示现在有了一个图元
			node->m_NumGeomCloseID = pSEnd->m_NumGeomCloseID;//这是重点，既然现在的数据跟原来的数据有相同的关系，那么就该是同属一个闭环
			return head;
		}
		else
		{
			node->prevGeomeleNode = pSEnd;
			node->nextGeomeleNode = pSEnd->nextGeomeleNode;
			pSEnd->nextGeomeleNode->prevGeomeleNode = node;
			pSEnd->nextGeomeleNode = node;
			head->m_NumGeomele++;//表示现在有了一个图元
			node->m_NumGeomCloseID = pSEnd->m_NumGeomCloseID;//这是重点，既然现在的数据跟原来的数据有相同的关系，那么就该是同属一个闭环
			return head;
		}
	}
	//如果不是空表，且数据也没有什么关系，那么就该是另外一条链表了。（暂时还挂在同一条链表上，但是应该在封闭环的ID上加1.）
	else//那么就不是空表，这时候不用再把起点赋值出去，而是要找到尾结点挂上去
	{
		temp = head->FirstGeomele;
		while (temp->nextGeomeleNode)
		{
			temp = temp->nextGeomeleNode;//遍历寻找最后一个结点
		}
		node->prevGeomeleNode = temp;
		temp->nextGeomeleNode = node;//原来它的后向结点是NULL
		head->m_NumGeomele++;//在原来的基础上加1，只有单条链表的时候这里跟node->m_GeomStandData.m_GeomCloseID是相等的。但是一旦有多条链表的时候这就不一样了。
		//head->FirstGeomele->m_NumGeomCloseID;//这样加就是错的。
		m_NumGeomClose++;//那么应该是另一个封闭环,但注意是在node上加，而不是头结点上加
		node->m_NumGeomCloseID = m_NumGeomClose;
		return head;
	}
}
//判断是不是起点相同
GeomEleNode*GeomClose::FindRelatGmElNd_xyStart(GeomCloseHEAD*head, GeomStandData m_geomstandData)
{
	GeomEleNode*temp;//用来遍历整个链表
	temp = head->FirstGeomele;//初始化为头结点
	while (temp)//整个链表都遍历完，如果是temp->nextGeomeleNode,那么在最后一个结点处就停了，即不会对最后一个比较
	{
		
		if ((fabs(temp->m_GeomStandData.GeoEle_start_x0 - m_geomstandData.GeoEle_start_x0)<EPSILON) && (fabs(temp->m_GeomStandData.GeoEle_start_y0 - m_geomstandData.GeoEle_start_y0)<EPSILON))//说明起点相同
			return temp;
		temp = temp->nextGeomeleNode;//遍历
	}
	return NULL;
}
//判断是不是止止相同
GeomEleNode*GeomClose::FindRelatGmElNd_xyEnd(GeomCloseHEAD*head, GeomStandData m_geomstandData)
{
	GeomEleNode*temp;//用来遍历整个链表
	temp = head->FirstGeomele;//初始化为头结点
	while (temp)
	{
		if ((fabs(temp->m_GeomStandData.GeoEle_start_x1 - m_geomstandData.GeoEle_start_x1)<EPSILON) && (fabs(temp->m_GeomStandData.GeoEle_start_y1 - m_geomstandData.GeoEle_start_y1)<EPSILON))//说明终点相同
				return temp;
		temp = temp->nextGeomeleNode;//遍历
	}
	return NULL;
}
//判断是不是现在的起点和原来的终点相同
GeomEleNode*GeomClose::FindRelatGmElNd_xySEnd(GeomCloseHEAD*head, GeomStandData m_geomstandData)
{
	GeomEleNode*temp;//用来遍历整个链表
	temp = head->FirstGeomele;//初始化为头结点
	while (temp)
	{
		if ((fabs(temp->m_GeomStandData.GeoEle_start_x1 - m_geomstandData.GeoEle_start_x0)<EPSILON) && (fabs(temp->m_GeomStandData.GeoEle_start_y1 - m_geomstandData.GeoEle_start_y0)<EPSILON))//说明现在起点和原来的终点相同
			return temp;
		temp = temp->nextGeomeleNode;//遍历
	}
	return NULL;
}
//判断是不是现在数据的终止点和原来数据的起点相同
GeomEleNode*GeomClose::FindRelatGmElNd_xyEStart(GeomCloseHEAD*head, GeomStandData m_geomstandData)
{
	GeomEleNode*temp;//用来遍历整个链表
	temp = head->FirstGeomele;//初始化为头结点
	while (temp)
	{
		if ((fabs(temp->m_GeomStandData.GeoEle_start_x0 - m_geomstandData.GeoEle_start_x1)<EPSILON) && (fabs(temp->m_GeomStandData.GeoEle_start_y0 - m_geomstandData.GeoEle_start_y1)<EPSILON))//说明现在的终点和原来的起点相同
			return temp;
		temp = temp->nextGeomeleNode;//遍历
	}
	return NULL;
}
//调换数据
GeomStandData GeomClose::DataSwap(GeomStandData m_geomstandData)
{
	GeomStandData m_tempData;
	m_tempData = m_geomstandData;//相当于初始化，把现有的值给传过来
	//对于需要调换的数据，下面就相当于二次赋值
	m_tempData.GeoEle_start_x0 = m_geomstandData.GeoEle_start_x1;
	m_tempData.GeoEle_start_x1 = m_geomstandData.GeoEle_start_x0;
	m_tempData.GeoEle_start_y0 = m_geomstandData.GeoEle_start_y1;
	m_tempData.GeoEle_start_y1 = m_geomstandData.GeoEle_start_y0;
	//m_tempData.m_arc.m_ArcAngle_start = m_geomstandData.m_arc.m_ArcAngle_end;//对于ARC而言，数据转换了，那么起始角和终止角要调转，否则切割的过程会跳到另一头开始切。
	//m_tempData.m_arc.m_ArcAngle_end = m_geomstandData.m_arc.m_ArcAngle_start;//相当于二次赋值
	m_tempData.m_line.x0 = m_geomstandData.m_line.x1;
	m_tempData.m_line.x1 = m_geomstandData.m_line.x0;
	m_tempData.m_line.y0 = m_geomstandData.m_line.y1;
	m_tempData.m_line.y1 = m_geomstandData.m_line.y0;
	m_tempData.m_IsTranData = true;
	//其他不用变了
	return m_tempData;
}
