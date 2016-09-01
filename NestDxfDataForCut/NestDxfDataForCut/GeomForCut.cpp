#include "stdafx.h"
#include "GeomForCut.h"
#define EPSILON 0.018

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
////将在一个双向链表的整张排样DXF里面的基本图元数据划分成不同的封闭环,每次把不是同个闭环的基本图元迁到另一个链表中。如此循环，不能再有新的封闭环F头结点产生
//void GeomForCut::SepDiffGeomCls(GeomCloseHEAD*head)
//{
//	//划分的原则是统一数据类型中后一个直线的起止点与前一个直线的起止点没有任何的联系，没遇到一个就新生产一个封闭环结点，同时将该结点从本链表中移除，挂到新的双向链表中
//	GeomEleNode*temp;//用来遍历整体初始双向链表
//	double m_temp_x0, m_temp_y0, m_temp_x1, m_temp_y1;//存储每一个封闭环的起始结点
//	m_temp_x0 = head->FirstGeomele->m_GeomStandData.GeoEle_start_x0;//将已经转换过后保存在统一数据类型里面的起止点用来以后判断依据
//	m_temp_y0 = head->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
//	m_temp_x1 = head->FirstGeomele->m_GeomStandData.GeoEle_start_x1;
//	m_temp_y1 = head->FirstGeomele->m_GeomStandData.GeoEle_start_y1;
//
//	temp = head->FirstGeomele->nextGeomeleNode;//从整个无序双向链表中的第二个结点开始
//
//	//while (temp)//遍历一遍，到尾结点才结束
//	//{
//	//	//LINE ARC CIRCLE这三种类型中，CIRCLE就是一个独立的封闭环，所以要在判断这些结点时候一起考虑是不是CIRCLE，是则直接剔除出去
//	//	if (m_temp_x0 != temp->m_GeomStandData.GeoEle_start_x0) && (m_temp_x0 != temp->m_GeomStandData.GeoEle_start_x0)
//	//}
//}
//寻找不是同一个封闭环的结点,要具有通用性
GeomEleNode*GeomForCut::FindDiffGeomCloseNode(NestResultDataNode*head)
{
	GeomCloseHEAD*Htemp=NULL;
	GeomEleNode*temp = NULL;
	int m_CloseID=-1;//初始化
	if (!(head->FirstGeomClose->nextGeomcloseNode))//如果排样结果dxf头结点指向的封闭环双向链表中只有一个封闭环结点
	{
		Htemp = head->FirstGeomClose;//第一个封闭环双向链表结点
		temp = Htemp->FirstGeomele;//第一个数据点的双向链表
		m_CloseID = temp->m_NumGeomCloseID; //第一个数据点的封闭环ID,这个ID就是用来与其他封闭环区分的标志
		while (temp)//全部遍历完
		{
			if (m_CloseID != temp->m_NumGeomCloseID)//找到第一个不是同一个封闭环ID的结点
			{
				temp->prevGeomeleNode->nextGeomeleNode = NULL;//从temp开始后面所有的结点都将移走，temp前面的结点从此后继无人。。。
				temp->prevGeomeleNode = NULL;//同理，temp从此也没有领头人了。
				return temp;
			}
				
			temp = temp->nextGeomeleNode;
		}
		return NULL;//说明这里全部都是同一个封闭环的
	}
	else//如果同一个排样dxf头结点指向的封闭环双向链表中不止一个封闭环结点
	{
		Htemp = head->FirstGeomClose;//第一个封闭环双向链表结点
		while (Htemp->nextGeomcloseNode)//找到最后一个封闭环双向链表结点
			Htemp = Htemp->nextGeomcloseNode;
		temp = Htemp->FirstGeomele;//最后一个封闭环头结点所指向的第一个数据结点
		m_CloseID = temp->m_NumGeomCloseID;//重要的初始化
		while (temp)//全部遍历完
		{
			if (m_CloseID != temp->m_NumGeomCloseID)//找到第一个不是同一个封闭环ID的结点
			{
				temp->prevGeomeleNode->nextGeomeleNode = NULL;//从temp开始后面所有的结点都将移走，temp前面的结点从此后继无人。。。
				temp->prevGeomeleNode = NULL;//同理，temp从此也没有领头人了。
				return temp;
			}
			temp = temp->nextGeomeleNode;
		}
		return NULL;//说明这里全部都是同一个封闭环的
	}
}
//将上面需找到的结点输入到这里
void GeomForCut::InsertGeomCloseHEAD(NestResultDataNode*head, GeomEleNode*node)
{
	GeomCloseHEAD*Ftemp;//寻找最后一个结点
	GeomCloseHEAD*Fnode;
	//要判断node是不是NULL
	if (node)//node不是NULL,说插入才有意义
	{
		Fnode = m_geomclose.CreatGeomCloseHEAD(node->m_NumGeomCloseID);//新产生一个F头结点
		Ftemp = head->FirstGeomClose;//Ftemp为封闭环双向链表的头结点
		while (Ftemp->nextGeomcloseNode)//需找封闭环双向链表的尾结点
			Ftemp = Ftemp->nextGeomcloseNode;
		Ftemp->nextGeomcloseNode = Fnode;//把新产生的封闭环双向链表结点挂到尾结点上
		Fnode->prevGeomcloseNode = Ftemp;
		Fnode->GEOMCLOSE_ID = node->m_NumGeomCloseID;
		Fnode->FirstGeomele = node;//把node挂到新的封闭环头结点上
		Fnode->m_geomclose_startpoint.colse_start_x0 = node->m_GeomStandData.GeoEle_start_x0;//将node上的第一个点挂到封闭环结点上，将来用作过渡线的一个点
		Fnode->m_geomclose_startpoint.colse_start_y0 = node->m_GeomStandData.GeoEle_start_y0;//将node上的第一个点挂到封闭环结点上，将来用作过渡线的一个点
		Fnode->m_AcceptGeomcloseHEAD = false;
	}	
}
//判断封闭环内结点是否完整,标志是封闭环组成的数据头结点的起点和尾结点的终点重合
GeomCloseHEAD* GeomForCut::JudgeGeomCloseIntact(NestResultDataNode*head)
{//这个函数只能保证每次进来要么确定一个完整封闭环，要么就是返回一个不完整的封闭环，不能保证是否已经完全判断完的标志
	GeomCloseHEAD*Htemp;
	GeomEleNode*temp;
	double x0, y0, x1, y1;
	Htemp = head->FirstGeomClose;//第一个封闭环结点
	while (Htemp)//每次都遍历完全
	{
		if (!(Htemp->m_IfGeomCloseIntact))//如果这个是还没被置位为完整封闭环的封闭环结点
		{
			temp = Htemp->FirstGeomele;
			x0 = temp->m_GeomStandData.GeoEle_start_x0;
			y0 = temp->m_GeomStandData.GeoEle_start_y0;
			while (temp->nextGeomeleNode)//找到最后一个结点
				temp = temp->nextGeomeleNode;
			x1 = temp->m_GeomStandData.GeoEle_start_x1;
			y1 = temp->m_GeomStandData.GeoEle_start_y1;
			if ((fabs(x0 - x1) < EPSILON) && (fabs(y0 - y1) < EPSILON))//说明两点相同
				Htemp->m_IfGeomCloseIntact = true;//说明这个封闭环是完整的。将来用作不再判断的标志
			else
				return Htemp;//后面的是否完整就不知道了，所以要再次进入
		}
		else
		 Htemp = Htemp->nextGeomcloseNode;
	}
}
//判断跟哪个结点是完整的配对,这必须要找得到，否则就出现不封闭的图形了。
NestResultDataNode*GeomForCut::Find_Change_GeomCloseHEAD(NestResultDataNode*head, GeomCloseHEAD*node)
{
	double x0,y0, x1, y1;
	GeomCloseHEAD*Htemp;
	GeomEleNode*temp;
	temp = node->FirstGeomele;
	x0 = temp->m_GeomStandData.GeoEle_start_x0;
	y0 = temp->m_GeomStandData.GeoEle_start_y0;
	while (temp->nextGeomeleNode)//找到最后一个结点
		temp = temp->nextGeomeleNode;
	x1 = temp->m_GeomStandData.GeoEle_start_x1;
	y1 = temp->m_GeomStandData.GeoEle_start_y1;
	Htemp = head->FirstGeomClose;
	while (Htemp)
	{
		if (!(Htemp->m_IfGeomCloseIntact))//如果没有被置位，说明这个可能是不完整的
		{
			//这里用if居然出错，因为没有else，如果不进入if，接下来就不知道怎么走了。
			while ((fabs(x1 - Htemp->FirstGeomele->m_GeomStandData.GeoEle_start_x0) < EPSILON) && (fabs(y1 - Htemp->FirstGeomele->m_GeomStandData.GeoEle_start_y0) < EPSILON))//说明Htemp与node配对,那么Htemp就没用了
			{
				temp->nextGeomeleNode = Htemp->FirstGeomele;//node的尾结点指向Htemp的头结点
				Htemp->FirstGeomele->prevGeomeleNode = temp;// Htemp的头结点指向node的尾结点,
				//对封闭环头结点进行调整的时候要区分Htemp是头结点，尾结点还是中间结点
				//头结点
				if (!(Htemp->prevGeomcloseNode))
				{
					Htemp->nextGeomcloseNode->prevGeomcloseNode = NULL;
					head->FirstGeomClose = Htemp->nextGeomcloseNode;//从此第二个结点才是头结点
					head->m_NumGeomClose--;//封闭环数要减少一个
					free(Htemp);//删除这个结点，经过上面的调整，它前后也已经连接上了，已经没用了
				}
				//尾结点
				else if (!(Htemp->nextGeomcloseNode))
				{
					Htemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
					head->FirstGeomClose = Htemp->nextGeomcloseNode;//从此第二个结点才是头结点
					head->m_NumGeomClose--;//封闭环数要减少一个
				}
				else if (Htemp->nextGeomcloseNode)//一般结点
				{
					Htemp->prevGeomcloseNode->nextGeomcloseNode = Htemp->nextGeomcloseNode;//也要把封闭环头结点进行调整
					Htemp->nextGeomcloseNode->prevGeomcloseNode = Htemp->prevGeomcloseNode;//没有考虑到Htemp是头结点，尾结点和中间结点的情况
					free(Htemp);//删除这个结点，经过上面的调整，它前后也已经连接上了，已经没用了
				}
				while (temp->nextGeomeleNode)//找到新加进来的数据结点的最后一个结点
					temp = temp->nextGeomeleNode;
				x1 = temp->m_GeomStandData.GeoEle_start_x1;
				y1 = temp->m_GeomStandData.GeoEle_start_y1;
				if ((fabs(x0 - x1) < EPSILON) && (fabs(y0 - y1) < EPSILON))//说明如今这两点相同
					node->m_IfGeomCloseIntact = true;//说明这个封闭环是完整的。将来用作不再判断的标志
				return head;//数据结点和封闭环头结点调整完毕
			}
			//如果上面没有跳出去，那么只能说明Htemp代表的这个封闭环和不完整的封闭环不是一个整体


		}
		//else//有这个else反而出错了
		Htemp = Htemp->nextGeomcloseNode;
	}
	return head;//把原来的返回，说明并没有找到与它能配对的，这是最坏的情况！！！
}

//核心代码，请勿乱动！！
//核心代码，请勿乱动！！
//核心代码，请勿乱动！！
//处理每一个封闭环里面的数据，寻找新的头结点
void GeomForCut::ChangeEleNodeOfGeomClosed(NestResultDataNode*head)
{
	GeomCloseHEAD*pHtemp;
	GeomEleNode*phtemp,*paimnode=NULL,*ptemp;//重要的三个结点参数
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//上一个封闭环的起止点
	double temp_end_x, temp_end_y;//当前封闭环数据结点的起点
	double m_mindistant, m_tempmindistant;//保存两点之间最短距离

	pHtemp = head->FirstGeomClose;//第一个头结点
	while (pHtemp)//所有封闭环头结点都处理完
	{
		if (!(pHtemp->prevGeomcloseNode))//如果这是第一个封闭环
		{
			//说明Htemp没有前一个封闭环，把机床原点赋给它
			m_prevGeomCloseEnd_x = 0.0;//以机床原点为最开始的起点，以（0,0）为机床原点
			m_prevGeomCloseEnd_y = 0.0;
		}
		else//说明不是头结点，那么就有前封闭环结点了，那么把前一个封闭环头结点保存的起止点数据传给它
		{
			m_prevGeomCloseEnd_x = pHtemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_x0;
			m_prevGeomCloseEnd_y = pHtemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_y0;

		}
		m_prevGeomCloseEnd_x = 0.0;//以机床原点为最开始的起点，以（0,0）为机床原点
		m_prevGeomCloseEnd_y = 0.0;

		//对于单个封闭环处理
		//寻找所有数据结点的起点中与上一个封闭环链表中的起止点最靠近的那个结点
		phtemp = pHtemp->FirstGeomele;//保存头结点
		ptemp = pHtemp->FirstGeomele;//用来遍历的结点
		//求下一个结点起点和上一个封闭环起止点的最短距离
		//以头结点起点和上一个封闭环起止点的最短距离为首先参考
		temp_end_x = phtemp->m_GeomStandData.GeoEle_start_x0;//保存起点数据
		temp_end_y = phtemp->m_GeomStandData.GeoEle_start_y0;
		m_mindistant = fabs(sqrt(((temp_end_x - m_prevGeomCloseEnd_x)*(temp_end_x - m_prevGeomCloseEnd_x)) + ((temp_end_y - m_prevGeomCloseEnd_y)*(temp_end_y - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
		while (ptemp)//把封闭环内所有的数据结点遍历一般,找出最理想的数据结点
		{
			if (!(ptemp->prevGeomeleNode))//如果ptemp是头结点那么目标结点就是头结点
			{
				paimnode = phtemp;//如果头结点的起止点是整个封闭环与和上一个封闭环起始点的最短距离的时候，头结点为目标结点

			}
			else//如果不是头结点了那么就要进行数据比较了
			{
				//求下一个结点起点和上一个封闭环起止点的最短距离
				temp_end_x = ptemp->m_GeomStandData.GeoEle_start_x0;//保存起点数据
				temp_end_y = ptemp->m_GeomStandData.GeoEle_start_y0;
				m_tempmindistant = fabs(sqrt(((temp_end_x - m_prevGeomCloseEnd_x)*(temp_end_x - m_prevGeomCloseEnd_x)) + ((temp_end_y - m_prevGeomCloseEnd_y)*(temp_end_y - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
				if (m_tempmindistant < m_mindistant)
					paimnode = ptemp;//说明现在这结点离上一个封闭环起止点的距离更近些
			}
			ptemp = ptemp->nextGeomeleNode;
		}
		//至此，一个封闭环链表里面的数据中距离最短的那个结点已经找到，接下来要对封闭环内的结点进行数据调换了
		if (paimnode==phtemp)//说明找了一圈发现还是最开始的结点距离最短
		{
			//其实这约等于说明都没干
			pHtemp->FirstGeomele = phtemp;
			pHtemp->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//把头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
			pHtemp->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
		}
		else//说明并不是头结点为最理想的结点
		{
			ptemp = phtemp;
			while (ptemp->nextGeomeleNode)//找到最后一个结点
				ptemp = ptemp->nextGeomeleNode;
			ptemp->nextGeomeleNode = phtemp;//原来ptemp作为尾结点，它的后向指针是指向NULL的
			phtemp->prevGeomeleNode = ptemp;//原来phtemp作为头结点，它的前向指针是指向NULL的
			paimnode->prevGeomeleNode->nextGeomeleNode = NULL;//原来paimnode的前向指针是指向它的，但是从此就作为尾结点了
			paimnode->prevGeomeleNode = NULL;//从此作为头结点
			pHtemp->FirstGeomele = paimnode;//作为上面的工作，它已经是这个封闭环里的新头结点了
			pHtemp->m_geomclose_startpoint.colse_start_x0 = paimnode->m_GeomStandData.GeoEle_start_x0;//把新头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
			pHtemp->m_geomclose_startpoint.colse_start_y0 = paimnode->m_GeomStandData.GeoEle_start_y0;
		}
		pHtemp = pHtemp->nextGeomcloseNode;//第一个封闭环处理完之后，进入第二个封闭环，如此循环最后整个排样dxf里面的封闭环都处理完；除了圆
	}
}