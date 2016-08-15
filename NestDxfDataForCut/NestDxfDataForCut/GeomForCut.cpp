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
	newNode->AcceptNestResultflag = false;//表明没有被收录
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
//将在一个双向链表的整张排样DXF里面的基本图元数据划分成不同的封闭环,每次把不是同个闭环的基本图元迁到另一个链表中。如此循环，不能再有新的封闭环F头结点产生
void GeomForCut::SepDiffGeomCls(GeomCloseHEAD*head)
{
	//划分的原则是统一数据类型中后一个直线的起止点与前一个直线的起止点没有任何的联系，没遇到一个就新生产一个封闭环结点，同时将该结点从本链表中移除，挂到新的双向链表中
	GeomEleNode*temp;//用来遍历整体初始双向链表
	double m_temp_x0, m_temp_y0, m_temp_x1, m_temp_y1;//存储每一个封闭环的起始结点
	m_temp_x0 = head->FirstGeomele->m_GeomStandData.GeoEle_start_x0;//将已经转换过后保存在统一数据类型里面的起止点用来以后判断依据
	m_temp_y0 = head->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
	m_temp_x1 = head->FirstGeomele->m_GeomStandData.GeoEle_start_x1;
	m_temp_y1 = head->FirstGeomele->m_GeomStandData.GeoEle_start_y1;

	temp = head->FirstGeomele->nextGeomeleNode;//从整个无序双向链表中的第二个结点开始

	//while (temp)//遍历一遍，到尾结点才结束
	//{
	//	//LINE ARC CIRCLE这三种类型中，CIRCLE就是一个独立的封闭环，所以要在判断这些结点时候一起考虑是不是CIRCLE，是则直接剔除出去
	//	if (m_temp_x0 != temp->m_GeomStandData.GeoEle_start_x0) && (m_temp_x0 != temp->m_GeomStandData.GeoEle_start_x0)
	//}
}