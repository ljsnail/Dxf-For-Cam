#include "stdafx.h"
#include "GeomClose.h"


GeomClose::GeomClose()
{
	
}


GeomClose::~GeomClose()
{
}
//初始化一块内存来存放整个图元标志的数据格式GeomStandData
//最底层的一个链表
GeomEleNode* GeomClose::CreatGeomEleNode(GeomStandData m_geomstandData)
{
	GeomEleNode*newNode = (GeomEleNode*)malloc(sizeof(GeomEleNode));
	newNode->m_GeomStandData = m_geomstandData;//把数据保存起来
	newNode->prevGeomeleNode = NULL;
	newNode->nextGeomeleNode = NULL;
	newNode->m_AccptGeomEleNode = false;
	newNode->m_NumGeomEleID=m_geomstandData.m_GeomEleID;//这是这张排样图的第几个基本图元
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
	newNode->prevGeomcloseNode = NULL;
	newNode->nextGeomcloseNode = NULL;
	newNode->m_AcceptGeomcloseHEAD = false;//起初时候并没有收录
	return newNode;
}
GeomCloseHEAD*GeomClose::AddGeomEleNode(GeomCloseHEAD*head, GeomEleNode *node)
{
	GeomEleNode*temp;//过渡结点
	//结点挂上来的时候分空表和已经有数据的表。
	//如果是空表，那么就直接把数据挂上去
	if (!(head->FirstGeomele))
	{
		head->FirstGeomele = node;
		head->m_geomclose_startpoint.colse_start_x0 = node->m_GeomStandData.GeoEle_start_x0;//既然是空表，那么这个就是头结点，所以就把头结点的起始点存放first结点里。
		head->m_geomclose_startpoint.colse_start_y0 = node->m_GeomStandData.GeoEle_start_y0;
		head->m_NumGeomele++;//表示有了一个图元
		return head;
	}
	else//那么就不是空表，这时候不用再把起点赋值出去，而是要找到尾结点挂上去
	{
		temp = head->FirstGeomele;
		while (temp->nextGeomeleNode)
		{
			temp = temp->nextGeomeleNode;//遍历寻找最后一个结点
		}
		node->prevGeomeleNode = temp;
		temp->nextGeomeleNode = node;//原来它的后向结点是NULL
		head->m_NumGeomele++;//在原来的基础上加1，只有单根链表的时候这里跟node->m_GeomStandData.m_GeomCloseID是相等的。但是一旦有多条链表的时候这就不一样了。
		return head;
	}
}