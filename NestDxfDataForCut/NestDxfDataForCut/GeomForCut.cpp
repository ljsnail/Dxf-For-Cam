#include "stdafx.h"
#include "GeomForCut.h"


GeomForCut::GeomForCut()
{
	//BatchHEAD m_batchHead = { 0, NULL };//结构体初始化
	m_nestrsltNode = { NULL, NULL, NULL, false };//用来作为每次存储值的全局变量，但每次之后都会被重写
}


GeomForCut::~GeomForCut()
{
}
//实例化BatchHEAD并赋初值。
BatchHEAD* GeomForCut::CreatBatchHEAD(int BatchHEAD_ID)//实例化的时候就该注明这是第几批次，应该要有全局变量
{
	BatchHEAD*newNode = (BatchHEAD*)malloc(sizeof(BatchHEAD));
	newNode->BatchHEAD_ID = BatchHEAD_ID;
	newNode->m_NumNestrsltDtNode = 0;
	newNode->FirstNestRsltNode = NULL;
	return newNode;
}
NestResultDataNode*GeomForCut::CreatNestResultNode(int NestResult_ID)
{
	NestResultDataNode*newNode = (NestResultDataNode*)malloc(sizeof(NestResultDataNode));
	newNode->AcceptNestResultflag = false;//表面没有被收录
	newNode->FirstGeomClose = NULL;
	newNode->nextNestResultDataNode = NULL;
	newNode->prevNestResultDataNode = NULL;
	newNode->NestResult_ID = NestResult_ID;
	newNode->m_NumGeomClose = 0;
	return newNode;
}
NestResultDataNode*GeomForCut::AddGeomCloseHeadNode(NestResultDataNode*head, GeomCloseHEAD*node)
{
	GeomCloseHEAD*temp;
	//这里的要是否空表
	//如果空表，那么直接网上挂
	if (!(head->FirstGeomClose))
	{
		head->FirstGeomClose = node;
		head->m_NumGeomClose++;//包含的封闭环多了一个
		return head;
	}
	else//不是空表的情况下
	{
		temp = head->FirstGeomClose;
		while (temp->nextGeomcloseNode)//说明是尾结点
		{
			temp = temp->nextGeomcloseNode;
		}
		temp->nextGeomcloseNode = node;
		node->prevGeomcloseNode = temp;
		head->m_NumGeomClose++;
		return head;
	}
}
//把一个批次有多少张排样结果图插入到生产批次指向的双向链表中
BatchHEAD*GeomForCut::AddNestRsltDtNode(BatchHEAD*head, NestResultDataNode*node)
{
	NestResultDataNode*temp;
	//分是否为空表
	//如果是空表
	if (!(head->FirstNestRsltNode))
	{
		head->FirstNestRsltNode = node;
		head->m_NumNestrsltDtNode++;
		return head;
	}
	else
	{
		temp = head->FirstNestRsltNode;
		while (temp->nextNestResultDataNode)//说明是尾结点
		{
			temp = temp->nextNestResultDataNode;
		}
		temp->nextNestResultDataNode = node;
		node->prevNestResultDataNode = temp;
		head->m_NumNestrsltDtNode++;
		return head;
	}
}