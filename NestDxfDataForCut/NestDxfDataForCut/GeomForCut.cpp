#include "stdafx.h"
#include "GeomForCut.h"
#define EPSILON 0.018
#define WEIGHT_ORIGIN 0.1//封闭环到机床原点距离的权重
#define WEIGHT_PTP 0.9//封闭环到另一个封闭环之间距离的权重
GeomForCut::GeomForCut()
{
	//BatchHEAD m_batchHead = { 0, NULL };//结构体初始化
	i = 0;
	j = 0;
	a = 0;
	m_nestrsltNode = { NULL, NULL, NULL, false };//用来作为每次存储值的全局变量，但每次之后都会被重写
	m_ceramic_Head = NULL;
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
	NestResultDataNode*temp=NULL;
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
	i++;
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
	j++;
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
			//如果这个不是封闭环的节点和前面找到的节点能够配对，那么就该把封闭环合并
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
					head->m_NumGeomClose--;//封闭环数要减少一个
					free(Htemp);//删除这个结点，经过上面的调整，它前后也已经连接上了，已经没用了
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
	node->m_IfGeomCloseIntact = true;//这找了一圈了，还没有，只能说明它确实没有合拍的，但如果不给它置位将来会一直循环这个值
	return head;//把原来的返回，说明并没有找到与它能配对的，这是最坏的情况！！！
}
//输入排样dxf头结点，输出封闭环头结点中，最后一个被置位的封闭环头结点
GeomCloseHEAD*GeomForCut::FindNotAcceptClosedNode(NestResultDataNode*head)
{
	GeomCloseHEAD*phtemp=NULL ,*phhtemp= NULL;
	phtemp = head->FirstGeomClose;
	//phhtemp = head->FirstGeomClose;
	//while (phhtemp->nextGeomcloseNode)//找到最后一个封闭环节点
	//{
	//	phhtemp = phhtemp->nextGeomcloseNode;
	//}
	if( !(phtemp->m_AcceptGeomcloseHEAD))//头结点没有被置位，那么肯定没有被置位的封闭环头结点了
	{
		return NULL;
	}
	//else if (phhtemp->m_AcceptGeomcloseHEAD)//如果最后一个封闭环节点也是被置位了，那么说明这个双向链表以及处理完了
	//{
	//	return phhtemp;
	//}
	else
	{
		while (phtemp->m_AcceptGeomcloseHEAD)
		{
			phtemp = phtemp->nextGeomcloseNode;
		}
		return phtemp;
	}
}
//寻找已经置位和没有置位的封闭环节点中的最短距离过渡节点
//输入封闭排样结果头结点，和一个没有被置位的封闭环头结点
//输出当前封闭环头结点下，与前面最后一个被置位的封闭头结点中的最短距离的那个图元节点
Mindistant_EleNode* GeomForCut::FindMinDstGeomeleNode(NestResultDataNode*head, GeomCloseHEAD*pGMCLSHead)//输入一个未置位的封闭环节点
{
	Mindistant_EleNode*m_mintempnode=NULL;
	GeomCloseHEAD*phtemp;//前面最后一个被置位的封闭头结点
	GeomCloseHEAD*pMINhtemp;//前面最后一个被置位的封闭头结点
	GeomEleNode* ptempminds;//遍历点
	GeomEleNode* pmindst;//保存头结点
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//上一个封闭环的起止点
	double m_tempGeomCloseEnd_x, m_tempGeomCloseEnd_y;//当前封闭环的暂时起止点
	double m_mindstant;
	double m_tempmindstant;
	ptempminds = pGMCLSHead->FirstGeomele;
	pmindst = pGMCLSHead->FirstGeomele;
	//找整个封闭环头结点双向链表里的最后一个被置位的封闭环头结点
	//phtemp = FindNotAcceptClosedNode(head);
	phtemp = head->FirstGeomClose;
	while (phtemp->m_AcceptGeomcloseHEAD)
		phtemp = phtemp->nextGeomcloseNode;
	if (!phtemp)//说明这个封闭环里面所有的头结点都还是没有被置位的
	{
		m_prevGeomCloseEnd_x = 0.0;//机床原点
		m_prevGeomCloseEnd_y = 0.0;
	}
	else
	{
		m_prevGeomCloseEnd_x = phtemp->m_geomclose_startpoint.colse_start_x0;
		m_prevGeomCloseEnd_y = phtemp->m_geomclose_startpoint.colse_start_y0;
	}
	m_tempGeomCloseEnd_x = pGMCLSHead->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
	m_tempGeomCloseEnd_y = pGMCLSHead->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
	//pMINhtemp = pGMCLSHead;
	m_mindstant = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
	while (ptempminds)//全部遍历完
	{
		m_tempGeomCloseEnd_x = ptempminds->m_GeomStandData.GeoEle_start_x0;
		m_tempGeomCloseEnd_y = ptempminds->m_GeomStandData.GeoEle_start_y0;
		m_tempmindstant = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
		if (m_tempmindstant<m_mindstant)
		{
			m_mindstant = m_tempmindstant;
			pmindst = ptempminds;
			//pMINhtemp=
		}
		else
		{
			;
		}
		ptempminds=ptempminds->nextGeomeleNode;
	}
	m_mintempnode=CreatMindistant_EleNode(pmindst, m_mindstant);
	return m_mintempnode;
}
//对单个封闭环内部进行节点顺序调整
void GeomForCut::ChangeEleNodeForAloneClse(GeomCloseHEAD*pHtemp, GeomEleNode*paimnode)
{
	
	GeomEleNode*phtemp,*ptemp;
	phtemp = pHtemp->FirstGeomele;
	if (paimnode == phtemp)//说明找了一圈发现还是最开始的结点距离最短
	{
		//其实这约等于说明都没干
		pHtemp->FirstGeomele = phtemp;
		pHtemp->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//把头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
		pHtemp->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
	}
	else 
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
		
}
//核心代码，请勿乱动！！
//核心代码，请勿乱动！！
//核心代码，请勿乱动！！
//先按照每个封闭环到机床原点之间的最短距离去寻找封闭环内的头结点
//为下面处理整张图纸封闭环顺序打下基础
void GeomForCut::ChangeEleNodeOfGeomClosed_origin(NestResultDataNode*head)
{
	GeomCloseHEAD*pHtemp;
	GeomEleNode*phtemp, *paimnode = NULL, *ptemp;//重要的三个结点参数
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//上一个封闭环的起止点
	double temp_end_x, temp_end_y;//当前封闭环数据结点的起点
	double m_mindistant, m_tempmindistant;//保存两点之间最短距离

	pHtemp = head->FirstGeomClose;//第一个头结点
	while (pHtemp)//所有封闭环头结点都处理完
	{
		
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
				{
					m_mindistant = m_tempmindistant;//把最小值也要更新
					paimnode = ptemp;//说明现在这结点离上一个封闭环起止点的距离更近些

				}
			}
			ptemp = ptemp->nextGeomeleNode;
		}
		//至此，一个封闭环链表里面的数据中距离最短的那个结点已经找到，接下来要对封闭环内的结点进行数据调换了
		
		ChangeEleNodeForAloneClse(pHtemp, paimnode);
		pHtemp = pHtemp->nextGeomcloseNode;//第一个封闭环处理完之后，进入第二个封闭环，如此循环最后整个排样dxf里面的封闭环都处理完；除了圆
	}
}

//核心代码
//核心代码
//核心代码
//按照封闭环里的起止点到机床原点的距离由近到远
//调整整个排样结果图的封闭环链接顺序
void GeomForCut::ChangClosedNodeOfNRDXF(NestResultDataNode*head)
{
	GeomCloseHEAD*pHtemp;//保存封闭环双向链表的头结点
	GeomCloseHEAD*phnowtemp;//保存封闭环双向链表结点
	GeomCloseHEAD*phstarttemp=NULL;//保存封闭暂时应该是头结点
	GeomCloseHEAD*pntemp = NULL;//保存最后一个封闭环节点
	GeomCloseHEAD*ptemp = NULL;//保存封闭暂时应该是头结点
	GeomCloseHEAD*pptemp = NULL;//保存封闭暂时应该是头结点
	double m_origin_x = 0.0;//机床原点
	double m_origin_y = 0.0;//机床原点
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//上一个封闭环的起止点
	double m_tempGeomCloseEnd_x, m_tempGeomCloseEnd_y;//当前封闭环的起止点
	double m_mindistant_origin;//保存封闭环双向链表中第一个没有被置位的节点到机床原点的最短距离（置位：封闭环被确定放置位置）
	double m_tempmindistant_origin;//暂时保存封闭环双向链表中没有被置位的节点中到机床原点的距离
	double m_mindistant_ptp;//保存封闭环双向链表中最后一个已经被置位的节点到第一个没有被置位的节点之间的距离
	double m_tempmindistant_ptp;//暂时保存封闭环双向链表中最后一个已经被置位的节点到封闭环里没有被置位的节点之间的距离
	double	m_allmindistant;//保存封闭环双向链表中最后一个已经被置位的节点到没有被置位节点之间的距离和没有被置位的封闭环节点到机床原点的距离之和的最小值
	double	m_tempallmindistant;//保存封闭环双向链表中最后一个已经被置位的节点到没有被置位节点之间的距离和没有被置位的封闭环节点到机床原点的距离之和的值
	int m_GeomCloseID = 0;
	int m_TotalCloseID=0;//保存总共的封闭环算，圆的封闭环数已经并进来了。
	pHtemp = head->FirstGeomClose;//第一个封闭环头结点
	pntemp = head->FirstGeomClose;
	while (pntemp)//找到最后一个封闭环节点
	{
		m_TotalCloseID++;
		pntemp = pntemp->nextGeomcloseNode;
	}
	/*while (pHtemp->nextGeomcloseNode)
	{
		pHtemp = pHtemp->nextGeomcloseNode;
	}
	*///重点是m_TotalCloseID的数据要对
	for (m_GeomCloseID = 1; m_GeomCloseID <=m_TotalCloseID; m_GeomCloseID++)//将全部循环封闭环的个数遍
	{
		
		//找到第一个没有还没有被收编的头结点，以它到原点的距离为基准
		ptemp = head->FirstGeomClose;
		//注意要放置出现一个问题，就是当for循环还没有跳出的时候，里面已经全部是被置位的了，那么在下面while里面就是死循环
		//while ((ptemp->m_AcceptGeomcloseHEAD)&&(ptemp!=NULL))//找到第一个没有被置位的封闭环节点,&&(ptemp!=NULL)是破解上面情况死循环的利器
		while (ptemp->m_AcceptGeomcloseHEAD)
		{
			ptemp = ptemp->nextGeomcloseNode;//出现错误，它的下一个还是它本身
			if (!ptemp)//当ptemp为NULL的时候，直接跳出整个循环
			{
				break;
			}
			else
			{
				;
			}
		}
		if (ptemp)//当ptemp不为NULL的时候，才有意义
		{
			if (!(ptemp->prevGeomcloseNode))//如果ptemp是封闭环头结点且没有被置位，那么它前面节点的起始点为机床原点即是0.0，0.0
			{
				m_prevGeomCloseEnd_x = 0.0;
				m_prevGeomCloseEnd_y = 0.0;
			}
			else//说明ptemp 不是封闭环头结点，那么就该有前向节点
			{
				m_prevGeomCloseEnd_x = ptemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_x0;
				m_prevGeomCloseEnd_y = ptemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_y0;
			}
			m_tempGeomCloseEnd_x = ptemp->m_geomclose_startpoint.colse_start_x0;
			m_tempGeomCloseEnd_y = ptemp->m_geomclose_startpoint.colse_start_y0;
			//第一个没被置位的封闭环的起始点到机床原点的距离
			m_mindistant_origin = fabs(sqrt(((m_tempGeomCloseEnd_x - m_origin_x)*(m_tempGeomCloseEnd_x - m_origin_x)) + ((m_tempGeomCloseEnd_y - m_origin_y)*(m_tempGeomCloseEnd_y - m_origin_y))));//这里求的是两点之间的最短距离
			m_mindistant_ptp = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
			m_allmindistant = m_mindistant_origin*WEIGHT_ORIGIN + m_mindistant_ptp*WEIGHT_PTP;//求最短距离
			phstarttemp = ptemp;
			//先要找到这个最后一个没有被置位的封闭环节点
			phnowtemp = head->FirstGeomClose;
			while (phnowtemp)//全部遍历一遍，需求和前一封闭环靠的最近的那个封闭环
			{
				if (!(phnowtemp->prevGeomcloseNode))//说明是头结点
				{
					if (!(phnowtemp->m_AcceptGeomcloseHEAD))//说明这个封闭环还没有被规划好，这段代码的目的是得到一个暂时的最小值
					{
						m_prevGeomCloseEnd_x = 0.0;
						m_prevGeomCloseEnd_y = 0.0;
						m_tempGeomCloseEnd_x = phnowtemp->m_geomclose_startpoint.colse_start_x0;
						m_tempGeomCloseEnd_y = phnowtemp->m_geomclose_startpoint.colse_start_y0;
						//到原点的距离
						m_tempmindistant_origin = fabs(sqrt(((m_tempGeomCloseEnd_x - m_origin_x)*(m_tempGeomCloseEnd_x - m_origin_x)) + ((m_tempGeomCloseEnd_y - m_origin_y)*(m_tempGeomCloseEnd_y - m_origin_y))));//这里求的是两点之间的最短距离
						//两个封闭环之间的距离
						m_tempmindistant_ptp = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
						//求最短距离
						m_tempallmindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
						if (m_tempallmindistant < m_allmindistant)//如果这个比所在的最小还要小，那么当前应该是它最小
						{
							m_allmindistant = m_tempallmindistant;
							phstarttemp = phnowtemp;
						}
					}

				}
				else//说明不是头结点了
				{
					if (!(phnowtemp->m_AcceptGeomcloseHEAD))//说明这个封闭环还没有被规划好
					{
						//找到第一个没有还没有被收编的头结点，以它到原点的距离为基准
						pptemp = head->FirstGeomClose;
						while (pptemp->m_AcceptGeomcloseHEAD)//找到第一个没有被置位的封闭环节点
							pptemp = pptemp->nextGeomcloseNode;
						if (!(pptemp->prevGeomcloseNode))//如果ptemp是封闭环头结点且没有被置位，那么它前面节点的起始点为机床原点即是0.0，0.0
						{
							m_prevGeomCloseEnd_x = 0.0;
							m_prevGeomCloseEnd_y = 0.0;
						}
						else//说明ptemp 不是封闭环头结点，那么就该有前向节点
						{
							m_prevGeomCloseEnd_x = pptemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_x0;
							m_prevGeomCloseEnd_y = pptemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_y0;
						}
						m_tempGeomCloseEnd_x = phnowtemp->m_geomclose_startpoint.colse_start_x0;
						m_tempGeomCloseEnd_y = phnowtemp->m_geomclose_startpoint.colse_start_y0;
						//到原点的距离
						m_tempmindistant_origin = fabs(sqrt(((m_tempGeomCloseEnd_x - m_origin_x)*(m_tempGeomCloseEnd_x - m_origin_x)) + ((m_tempGeomCloseEnd_y - m_origin_y)*(m_tempGeomCloseEnd_y - m_origin_y))));//这里求的是两点之间的最短距离
						//两个封闭环之间的距离
						m_tempmindistant_ptp = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
						//求最短距离
						m_tempallmindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
						if (m_tempallmindistant < m_allmindistant)//如果这个比所在的最小还要小，那么当前应该是它最小
						{
							m_allmindistant = m_tempallmindistant;
							phstarttemp = phnowtemp;
						}
					}
				}

				phnowtemp = phnowtemp->nextGeomcloseNode;
			}
			//以上是循环一遍之后找到的目前来说距离最短的一个封闭环头结点
			if (!(phstarttemp->prevGeomcloseNode))//如果是头结点，找了一遍发现还是头结点为最靠近的封闭环节节点
			{
				head->FirstGeomClose = phstarttemp;//还是它为头结点
				phstarttemp->m_AcceptGeomcloseHEAD = true;//从此该节点就被固定了
			}
			else//说明不是头结点了，那么这个节点要挂的目前最后一个m_AcceptGeomcloseHEAD没有被设置为true的封闭环节点后面
			{
				//先要找到这个最后一个没有被置位的封闭环节点
				phnowtemp = head->FirstGeomClose;
				while (phnowtemp->nextGeomcloseNode->m_AcceptGeomcloseHEAD)//当phnowtemp的下一个封闭环头结点不再是true的时候，那么phnowtemp就应该是最后一个m_AcceptGeomcloseHEAD是true的，也就是已经被安排了顺序的
					phnowtemp = phnowtemp->nextGeomcloseNode;
				//注意区分，这里既没有任何一个节点被置位，第一个最近的点也不是头结点的情况
				if ((!(phnowtemp->prevGeomcloseNode)) && (!(phnowtemp->m_AcceptGeomcloseHEAD)))//说明这就是现在没有一个封闭环被置位了
				{
					if (!(phstarttemp->nextGeomcloseNode))//如果是尾节点
					{
						//还有判断phstarttemp的前一个是不是倒数第二个，如果是则直接挂上去，不要变，否则死循环				
							phstarttemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
							phstarttemp->nextGeomcloseNode = phnowtemp;
							phnowtemp->prevGeomcloseNode = phstarttemp;
							phstarttemp->prevGeomcloseNode = NULL;
							phstarttemp->m_AcceptGeomcloseHEAD = true;
							head->FirstGeomClose = phstarttemp;
					}
					else//一般节点
					{
						phstarttemp->prevGeomcloseNode->nextGeomcloseNode = phstarttemp->nextGeomcloseNode;//它原来前面的后面本来是它的，但它走了，自然就变成了它后面的那个
						phstarttemp->nextGeomcloseNode->prevGeomcloseNode = phstarttemp->prevGeomcloseNode;//它原来后面的前面本来是它的，但它走了，自然就变成了它前面的那个
						phnowtemp->prevGeomcloseNode = phstarttemp;
						phstarttemp->nextGeomcloseNode = phnowtemp;
						phstarttemp->prevGeomcloseNode = NULL;
						phstarttemp->m_AcceptGeomcloseHEAD = true;
						head->FirstGeomClose = phstarttemp;
					}

				}
				//经过上面的寻找，现在phnowtemp是最后一个被置位的节点，而phstarttemp就是要挂到phnowtemp后面
				//本来是每次调整双向链表的节点位置时候，都要考虑该节点是不是头尾还是一般节点的，但这里可以不用考虑头结点了
				else
				{
					if (!(phstarttemp->nextGeomcloseNode))//如果是尾节点
					{
						//还有判断phstarttemp的前一个是不是倒数第二个，如果是则直接挂上去，不要变，否则死循环
						if (phnowtemp->nextGeomcloseNode == phstarttemp)//说明最后一个已经置位的封闭环头结点的后一个节点本身就是phstarttemp
						{
							phstarttemp->m_AcceptGeomcloseHEAD = true;//没有位置给它了，直接置位
						}
						else
						{
							phnowtemp->nextGeomcloseNode->prevGeomcloseNode = phstarttemp;
							phstarttemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
							phstarttemp->nextGeomcloseNode = phnowtemp->nextGeomcloseNode;
							phnowtemp->nextGeomcloseNode = phstarttemp;
							phstarttemp->prevGeomcloseNode = phnowtemp;
							phstarttemp->m_AcceptGeomcloseHEAD = true;
						}
					}
					else//一般节点
					{
						phstarttemp->prevGeomcloseNode->nextGeomcloseNode = phstarttemp->nextGeomcloseNode;//它原来前面的后面本来是它的，但它走了，自然就变成了它后面的那个
						phstarttemp->nextGeomcloseNode->prevGeomcloseNode = phstarttemp->prevGeomcloseNode;//它原来后面的前面本来是它的，但它走了，自然就变成了它前面的那个
						phnowtemp->nextGeomcloseNode->prevGeomcloseNode = phstarttemp;
						phstarttemp->nextGeomcloseNode = phnowtemp->nextGeomcloseNode;
						phnowtemp->nextGeomcloseNode = phstarttemp;
						phstarttemp->prevGeomcloseNode = phnowtemp;
						phstarttemp->m_AcceptGeomcloseHEAD = true;
					}
				}
				
			}
		}
		else//当ptemp为NULL的时候，啥也不干，就耗完for就好了
		{
			;
		}
		
	}
}


//核心代码，请勿乱动！！
//核心代码，请勿乱动！！
//核心代码，请勿乱动！！
//按照已经确定的顺序封闭环顺序去处理每一个封闭环里面的数据，寻找新的头结点
void GeomForCut::ChangeEleNodeOfGeomClosed_order(NestResultDataNode*head)
{
	int a = 0;
	GeomCloseHEAD*pHtemp;
	GeomEleNode*phtemp, *paimnode = NULL, *ptemp;//重要的三个结点参数
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//上一个封闭环的起止点
	double temp_end_x, temp_end_y;//当前封闭环数据结点的起点
	double m_mindistant, m_tempmindistant;//保存两点之间最短距离

	pHtemp = head->FirstGeomClose;//第一个头结点
	while (pHtemp->nextGeomcloseNode)//所有封闭环头结点都处理完
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
				//对于圆来说整个封闭环只有一个结点，且为头结点，那么就需要根据前面封闭环的起止点和现在的圆心的直线和圆求交点。
				if (3==ptemp->m_GeomStandData.m_typegeomele)//圆
				{
					double a,b,r,x0,y0,c;
					double x1,y1,x2,y2;
					double distance_1, distance_2;
					a=ptemp->m_GeomStandData.m_circle.m_Circent_x;
					b =ptemp->m_GeomStandData.m_circle.m_Circent_y;
					r = ptemp->m_GeomStandData.m_circle.m_Circle_r;
					x0 = m_prevGeomCloseEnd_x;
					y0 = m_prevGeomCloseEnd_y;
					if(x0==a)
					{
						x1=a;
						y1=b-r;
						x2=a;
						y2=b+r;
					}
					else if(y0==b)
					{
						x1=a-r;
						y1=b;
						x2=a+r;
						y2=b;
					}
					else
					{
						c=sqrt(pow(r,2)/(1+pow(((y0-b)/(x0-a)),2)));
						x1=a+c;
						y1=b+((y0-b)/(x0-a))*c;
						x2=a-c;
						y2=b-((y0-b)/(x0-a))*c;
					}
					distance_1 = fabs(sqrt(((x1 - m_prevGeomCloseEnd_x)*(x1 - m_prevGeomCloseEnd_x)) + ((y1 - m_prevGeomCloseEnd_y)*(y1 - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
					distance_2 = fabs(sqrt(((x2 - m_prevGeomCloseEnd_x)*(x2 - m_prevGeomCloseEnd_x)) + ((y2 - m_prevGeomCloseEnd_y)*(y2 - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
					m_tempmindistant = distance_1;
					if (distance_2<distance_1)
					{
						x1 = x2;
						y1 = y2;
						m_tempmindistant = distance_2;
					}
					if (m_tempmindistant < m_mindistant)
					{
						ptemp->m_GeomStandData.GeoEle_start_x0 = x1;
						ptemp->m_GeomStandData.GeoEle_start_y0 = y1;
						ptemp->m_GeomStandData.GeoEle_start_x1 = x1;
						ptemp->m_GeomStandData.GeoEle_start_y1 = y1;
						ptemp->m_GeomStandData.m_circle.m_Circle_Start_Angle = m_geomele.ForCircleStartAngle(x1, y1, ptemp->m_GeomStandData.m_circle);
					}
					else
					{
						paimnode = ptemp;//说明现在这结点离上一个封闭环起止点的距离更近些

					}
				}
				else
				{
					paimnode = phtemp;//如果头结点的起止点是整个封闭环与和上一个封闭环起始点的最短距离的时候，头结点为目标结点

				}

			}
			else//如果不是头结点了那么就要进行数据比较了
			{
				//求下一个结点起点和上一个封闭环起止点的最短距离
				temp_end_x = ptemp->m_GeomStandData.GeoEle_start_x0;//保存起点数据
				temp_end_y = ptemp->m_GeomStandData.GeoEle_start_y0;
				m_tempmindistant = fabs(sqrt(((temp_end_x - m_prevGeomCloseEnd_x)*(temp_end_x - m_prevGeomCloseEnd_x)) + ((temp_end_y - m_prevGeomCloseEnd_y)*(temp_end_y - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
				if (m_tempmindistant < m_mindistant)
				{
					m_mindistant = m_tempmindistant;//把最小值也要更新
					paimnode = ptemp;//说明现在这结点离上一个封闭环起止点的距离更近些

				}
			}
			ptemp = ptemp->nextGeomeleNode;
		}
		//至此，一个封闭环链表里面的数据中距离最短的那个结点已经找到，接下来要对封闭环内的结点进行数据调换了
		a++;
		//if (paimnode->prevGeomeleNode)//不是NULL，那么就是说不是从圆那边获得的数据
		if (pHtemp->FirstGeomele->m_GeomStandData.m_typegeomele!=3)//不是圆
		{
			ChangeEleNodeForAloneClse(pHtemp, paimnode);
		}
		pHtemp = pHtemp->nextGeomcloseNode;//第一个封闭环处理完之后，进入第二个封闭环，如此循环最后整个排样dxf里面的封闭环都处理完；除了圆

	}
}
//核心代码，请勿乱动！！
//核心代码，请勿乱动！！
//核心代码，请勿乱动！！
//按照已经确定的顺序封闭环顺序去处理每一个封闭环里面切割控制点，使得避免撞刀
void GeomForCut::ChangeEleNode_Avoid_Impact(NestResultDataNode*head)
{
	int a = 0;
	GeomCloseHEAD*pHtemp;
	GeomEleNode*phtemp, *paimnode = NULL, *ptemp;//重要的三个结点参数
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//上一个封闭环的起止点
	double temp_end_x, temp_end_y;//当前封闭环数据结点的起点
	double m_mindistant, m_tempmindistant;//保存两点之间最短距离

	pHtemp = head->FirstGeomClose;//第一个头结点
	while (pHtemp->nextGeomcloseNode)//所有封闭环头结点都处理完
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
				//对于圆来说整个封闭环只有一个结点，且为头结点，那么就需要根据前面封闭环的起止点和现在的圆心的直线和圆求交点。
				if (3 == ptemp->m_GeomStandData.m_typegeomele)//圆
				{
					double a, b, r, x0, y0, c;
					double x1, y1, x2, y2;
					double distance_1, distance_2;
					a = ptemp->m_GeomStandData.m_circle.m_Circent_x;
					b = ptemp->m_GeomStandData.m_circle.m_Circent_y;
					r = ptemp->m_GeomStandData.m_circle.m_Circle_r;
					x0 = m_prevGeomCloseEnd_x;
					y0 = m_prevGeomCloseEnd_y;
					if (x0 == a)
					{
						x1 = a;
						y1 = b - r;
						x2 = a;
						y2 = b + r;
					}
					else if (y0 == b)
					{
						x1 = a - r;
						y1 = b;
						x2 = a + r;
						y2 = b;
					}
					else
					{
						c = sqrt(pow(r, 2) / (1 + pow(((y0 - b) / (x0 - a)), 2)));
						x1 = a + c;
						y1 = b + ((y0 - b) / (x0 - a))*c;
						x2 = a - c;
						y2 = b - ((y0 - b) / (x0 - a))*c;
					}
					distance_1 = fabs(sqrt(((x1 - m_prevGeomCloseEnd_x)*(x1 - m_prevGeomCloseEnd_x)) + ((y1 - m_prevGeomCloseEnd_y)*(y1 - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
					distance_2 = fabs(sqrt(((x2 - m_prevGeomCloseEnd_x)*(x2 - m_prevGeomCloseEnd_x)) + ((y2 - m_prevGeomCloseEnd_y)*(y2 - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
					m_tempmindistant = distance_1;
					if (distance_2<distance_1)
					{
						x1 = x2;
						y1 = y2;
						m_tempmindistant = distance_1;
					}
					if (m_mindistant<m_tempmindistant)
					{
						ptemp->m_GeomStandData.GeoEle_start_x0 = x1;
						ptemp->m_GeomStandData.GeoEle_start_y0 = y1;
						ptemp->m_GeomStandData.GeoEle_start_x1 = x1;
						ptemp->m_GeomStandData.GeoEle_start_y1 = y1;
						ptemp->m_GeomStandData.m_circle.m_Circle_Start_Angle = m_geomele.ForCircleStartAngle(x1, y1, ptemp->m_GeomStandData.m_circle);
					}
					else
					{
						paimnode = ptemp;//说明现在这结点离上一个封闭环起止点的距离更近些

					}
				}
				else
				{
					paimnode = phtemp;//如果头结点的起止点是整个封闭环与和上一个封闭环起始点的最短距离的时候，头结点为目标结点

				}

			}
			else//如果不是头结点了那么就要进行数据比较了
			{
				//求下一个结点起点和上一个封闭环起止点的最短距离
				temp_end_x = ptemp->m_GeomStandData.GeoEle_start_x0;//保存起点数据
				temp_end_y = ptemp->m_GeomStandData.GeoEle_start_y0;
				m_tempmindistant = fabs(sqrt(((temp_end_x - m_prevGeomCloseEnd_x)*(temp_end_x - m_prevGeomCloseEnd_x)) + ((temp_end_y - m_prevGeomCloseEnd_y)*(temp_end_y - m_prevGeomCloseEnd_y))));//这里求的是两点之间的最短距离
				if (m_mindistant<m_tempmindistant)
				{
					m_mindistant = m_tempmindistant;//把最小值也要更新
					paimnode = ptemp;//说明现在这结点离上一个封闭环起止点的距离更近些

				}
			}
			ptemp = ptemp->nextGeomeleNode;
		}
		//至此，一个封闭环链表里面的数据中距离最短的那个结点已经找到，接下来要对封闭环内的结点进行数据调换了
		a++;
		//if (paimnode->prevGeomeleNode)//不是NULL，那么就是说不是从圆那边获得的数据
		if (pHtemp->FirstGeomele->m_GeomStandData.m_typegeomele != 3)//不是圆
		{
			ChangeEleNodeForAloneClse(pHtemp, paimnode);
		}
		pHtemp = pHtemp->nextGeomcloseNode;//第一个封闭环处理完之后，进入第二个封闭环，如此循环最后整个排样dxf里面的封闭环都处理完；除了圆

	}
}

//创造一个用来存放离最后一个已经被置位的封闭环最近的内图元结点和最短距离
Mindistant_EleNode*GeomForCut::CreatMindistant_EleNode(GeomEleNode* pmindst, double m_mindstant)
{
	Mindistant_EleNode*newNode = (Mindistant_EleNode*)malloc(sizeof(Mindistant_EleNode));
	newNode->m_mindistant = m_mindstant;
	newNode->pminnode = pmindst;
	return newNode;
}
//核心代码！
//核心代码！
//核心代码！
//基于禁忌——贪婪算法的路径规划和优化算法

void GeomForCut::BaseTS_GR_ForCutPathPlan(NestResultDataNode*head)
{

	Mindistant_EleNode*newNode = (Mindistant_EleNode*)malloc(sizeof(Mindistant_EleNode));
	newNode->m_mindistant = 99999.0;
	newNode->pminhead = NULL;
	newNode->pminnode = NULL;
	GeomCloseHEAD*pTempCHead, *ppTempCHead;//用作遍历全部封闭环
	GeomCloseHEAD*pLastAcceptCHead;//用寻找最后一个被置位的封闭环头结点
	GeomCloseHEAD*pTheLastAcceptCHead;//用寻找最后一个被置位的封闭环头结点

	GeomCloseHEAD*pMinDistantCloseHead, *pntemp;//用寻找最后一个被置位的封闭环头结点
	GeomCloseHEAD*pSencond2FirstCloseHead;//将第一个封闭去除，从第二个封闭环开始

	GeomEleNode*pTempGelenode;
	double pre_x, pre_y, temp_x, temp_y;
	double m_temp_mindistant;//保存暂时的最短距离，如果是最终的最短距离，那么就该是给到	newNode->m_mindistant;
	double m_tempmindistant_origin;//到原点距离
	double m_tempmindistant_ptp;//到已经被置位的封闭环的距离
	double m_origin_x = 0.0;//机床原点
	double m_origin_y = 0.0;//机床原点
	int a = 0, b = 0, m_TotalCloseID, m_GeomCloseID;
	//算法开始
	//pSencond2FirstCloseHead = head->FirstGeomClose;
	//pntemp = pSencond2FirstCloseHead->nextGeomcloseNode;//第一个封闭环是800*800的瓷砖，不应该切
	//head->FirstGeomClose = pntemp;
	//将第二个封闭环代替第一个封闭环
	//head = ChangeSencondCH2FH(head);
	pLastAcceptCHead = head->FirstGeomClose;
	pntemp = head->FirstGeomClose;
	m_TotalCloseID = 0;
	while (pntemp)//找到最后一个封闭环节点
	{
		m_TotalCloseID++;
		pntemp = pntemp->nextGeomcloseNode;
	}

	//重点是m_TotalCloseID的数据要对,保证每一次有一个封闭环被置位
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_TotalCloseID; m_GeomCloseID++)//将全部循环封闭环的个数遍
	{


		pTempCHead = head->FirstGeomClose;
		while (pTempCHead)//将这个排样结果图里面的封闭环全部遍历一遍,直到最后一个跳出循环
		{
			a++;
			/*pTempCHead = head->FirstGeomClose;*/
			while (!(pTempCHead->m_AcceptGeomcloseHEAD))//只有当该封闭环还没有被规划的时候，进入才有意义
			{
				//pTempCHead能进来说明它是第一个没有被置位的封闭环，那么遍历该从它开始
				//如果TempCHead是第一个封闭环节点，那么它前面并没有已经被置位的封闭环，所以以工件原点作为上一个封闭环的起止点

				if (NULL == pTempCHead->prevGeomcloseNode)//是头结点
				{
					pre_x = 0.0;
					pre_y = 0.0;
				}
				else//那么要找到最后一个被置位的封闭环头结点
				{
					/*pLastAcceptCHead = head->FirstGeomClose;
					while (pLastAcceptCHead->nextGeomcloseNode->m_AcceptGeomcloseHEAD)
						pLastAcceptCHead = pLastAcceptCHead->nextGeomcloseNode;*/

					//pre_x = pLastAcceptCHead->m_geomclose_startpoint.colse_start_x0;
					//pre_y = pLastAcceptCHead->m_geomclose_startpoint.colse_start_y0;
					pre_x = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
					pre_y = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				}
				//以上已经找到上一个封闭的起止点了。
				//现在要以该起止点来遍历所有的封闭环节点，找到其到上一个封闭环起止点最短距离的那个节点和封闭环
				//要对剩下的封闭环进行遍历
				//temp_x = pTempCHead->m_geomclose_startpoint.colse_start_x0;
				//temp_y = pTempCHead->m_geomclose_startpoint.colse_start_y0;//好像m_geomclose_startpoint已经没有用了。
				temp_x = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
				temp_y = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				//到原点的距离
				m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//这里求的是两点之间的最短距离
				//两个封闭环之间的距离
				m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//这里求的是两点之间的最短距离
				//求最短距离,暂时的最短距离
				newNode->m_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
				//newNode->m_mindistant = 1 / newNode->m_mindistant;
				newNode->pminhead = pTempCHead;
				newNode->pminnode = pTempCHead->FirstGeomele;
				pMinDistantCloseHead = pTempCHead;
				//把剩下的没有被置位的所有遍历一遍
				//以最短距离为准则从这里开始决定性下一个封闭环和头结点是什么
				b = 0;
				while (pMinDistantCloseHead)
				{
					b++;
					pTempGelenode = pMinDistantCloseHead->FirstGeomele;
					//注意要区分圆的封闭环
					if (3 != pTempGelenode->m_GeomStandData.m_typegeomele)//如果不是圆
					{
						while (pTempGelenode)//在这个封闭环里把节点全部遍历一遍
						{
							temp_x = pTempGelenode->m_GeomStandData.GeoEle_start_x0;
							temp_y = pTempGelenode->m_GeomStandData.GeoEle_start_y0;
							//到原点的距离
							m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//这里求的是两点之间的最短距离
							//两个封闭环之间的距离
							m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//这里求的是两点之间的最短距离
							m_temp_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
							//m_temp_mindistant = 1 / m_temp_mindistant;
							if (m_temp_mindistant < newNode->m_mindistant)
							{
								newNode->m_mindistant = m_temp_mindistant;
								newNode->pminhead = pMinDistantCloseHead;
								newNode->pminnode = pTempGelenode;
							}
							pTempGelenode = pTempGelenode->nextGeomeleNode;
						}
					}
					else//如果是圆的话
					{

						double a, b, r, x0, y0, c;
						double x1, y1, x2, y2;
						double distance_1, distance_2;
						a = pTempGelenode->m_GeomStandData.m_circle.m_Circent_x;
						b = pTempGelenode->m_GeomStandData.m_circle.m_Circent_y;
						r = pTempGelenode->m_GeomStandData.m_circle.m_Circle_r;
						x0 = pre_x;
						y0 = pre_y;
						if (x0 == a)
						{
							x1 = a;
							y1 = b - r;
							x2 = a;
							y2 = b + r;
						}
						else if (y0 == b)
						{
							x1 = a - r;
							y1 = b;
							x2 = a + r;
							y2 = b;
						}
						else
						{
							c = sqrt(pow(r, 2) / (1 + pow(((y0 - b) / (x0 - a)), 2)));
							x1 = a + c;
							y1 = b + ((y0 - b) / (x0 - a))*c;
							x2 = a - c;
							y2 = b - ((y0 - b) / (x0 - a))*c;
						}
						distance_1 = fabs(sqrt(((x1 - pre_x)*(x1 - pre_x)) + ((y1 - pre_y)*(y1 - pre_y))));//这里求的是两点之间的最短距离
						distance_2 = fabs(sqrt(((x2 - pre_x)*(x2 - pre_x)) + ((y2 - pre_y)*(y2 - pre_y))));//这里求的是两点之间的最短距离
						m_tempmindistant_ptp = distance_1;
						if (distance_2 < distance_1)
						{
							x1 = x2;
							y1 = y2;
							m_tempmindistant_ptp = distance_2;
						}
						m_temp_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
						//m_temp_mindistant = 1 / m_temp_mindistant;
						if (m_temp_mindistant < newNode->m_mindistant)
						{
							pTempGelenode->m_GeomStandData.GeoEle_start_x0 = x1;
							pTempGelenode->m_GeomStandData.GeoEle_start_y0 = y1;
							pTempGelenode->m_GeomStandData.GeoEle_start_x1 = x1;
							pTempGelenode->m_GeomStandData.GeoEle_start_y1 = y1;
							pTempGelenode->m_GeomStandData.m_circle.m_Circle_Start_Angle = m_geomele.ForCircleStartAngle(x1, y1, pTempGelenode->m_GeomStandData.m_circle);
							//
							newNode->m_mindistant = m_temp_mindistant;
							newNode->pminhead = pMinDistantCloseHead;
							newNode->pminnode = pTempGelenode;
						}
						else
						{
							;
						}
					}
					b;
					pMinDistantCloseHead = pMinDistantCloseHead->nextGeomcloseNode;

				}
				//以上已经找到基于现在这个被置位的封闭环的头结点的全局搜索
				//下面要做到是基于封闭环头结点和图元节点进行双向链表数据调整
				//先是把图元节点调整为封闭环的头结点
				GeomEleNode*phtemp, *ptemp;
				//if (!(pTempCHead->prevGeomcloseNode))//如果pTempCHead是第一个封闭头结点的情况下
				//{
				//	pLastAcceptCHead = pTempCHead;
				//}
				//else
				//{
				//	pLastAcceptCHead = pTempCHead->prevGeomcloseNode;
				//}
				phtemp = newNode->pminhead->FirstGeomele;
				if (newNode->pminnode == phtemp)//说明找了一圈发现还是最开始的结点距离最短
				{
					//其实这约等于说明都没干
					newNode->pminhead->FirstGeomele = phtemp;
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//把头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
				}
				else
				{
					ptemp = phtemp;
					while (ptemp->nextGeomeleNode)//找到最后一个结点
						ptemp = ptemp->nextGeomeleNode;
					ptemp->nextGeomeleNode = phtemp;//原来ptemp作为尾结点，它的后向指针是指向NULL的
					phtemp->prevGeomeleNode = ptemp;//原来phtemp作为头结点，它的前向指针是指向NULL的
					newNode->pminnode->prevGeomeleNode->nextGeomeleNode = NULL;//原来paimnode的前向指针是指向它的，但是从此就作为尾结点了
					newNode->pminnode->prevGeomeleNode = NULL;//从此作为头结点
					newNode->pminhead->FirstGeomele = newNode->pminnode;//作为上面的工作，它已经是这个封闭环里的新头结点了
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = newNode->pminnode->m_GeomStandData.GeoEle_start_x0;//把新头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = newNode->pminnode->m_GeomStandData.GeoEle_start_y0;
				}
				//以上是循环一遍之后找到的目前来说距离最短的一个封闭环头结点
				//接着要对整个封闭环双向链表的顺序进行调整
				//说明newNode->pminnode恰好是头结点，又是最靠近上一封闭环的那个封闭环
				if (!(newNode->pminhead->prevGeomcloseNode))//如果是头结点，找了一遍发现还是头结点为最靠近的封闭环节节点
				{
					head->FirstGeomClose = newNode->pminhead;//还是它为头结点
					newNode->pminhead->m_AcceptGeomcloseHEAD = true;//从此该节点就被固定了
				}
				//newNode->pminhead的前面刚好是被置位的
				//但会存在一种情况，就是恰好pLastAcceptCHead=newNode->pminhead;如此便能解决这个问题
				else if (newNode->pminhead->prevGeomcloseNode->m_AcceptGeomcloseHEAD)
				{
					//那么什么都不用动
					newNode->pminhead->m_AcceptGeomcloseHEAD = true;//从此该节点就被固定了

				}
				else//说明newNode->pminnode不是头结点了，那么这个节点要挂的目前最后一个m_AcceptGeomcloseHEAD没有被设置为true的封闭环节点后面
				{

					//经过上面的寻找，现在pLastAcceptCHead是最后一个被置位的节点，而newNode->pminhead就是要挂到pLastAcceptCHead后面
					//本来是每次调整双向链表的节点位置时候，都要考虑该节点是不是头尾还是一般节点的，但这里可以不用考虑头结点了
					//那么要先找到最后一个没被置位的封闭环，不是头结点也不代表现在前面有了被置位的封闭环
					pLastAcceptCHead = head->FirstGeomClose;
					while (pLastAcceptCHead->m_AcceptGeomcloseHEAD)
						pLastAcceptCHead = pLastAcceptCHead->nextGeomcloseNode;
					//pLastAcceptCHead是没有被置位的
					if (!(newNode->pminhead->nextGeomcloseNode))//如果是尾节点
					{
						//既然已经是尾节点newNode->pminhead，那么直接把他前面一个值得后向节点指向NULL；
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = NULL;
					}
					else//一般节点
					{
						//newNode->pminhead原来前后指向关系的调节
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead->nextGeomcloseNode;
						newNode->pminhead->nextGeomcloseNode->prevGeomcloseNode = newNode->pminhead->prevGeomcloseNode;
					}
					//以上处理完了newNode->pminhead在原来位置的关系，接下来要处理他在被置位时候的关系了
					//如果pLastAcceptCHead还是头节点，说明这一个封闭环都还没有被置位
					//要考虑的一点是当pLastAcceptCHead=newNode->pminhead的时候怎么办
					if (!(pLastAcceptCHead->prevGeomcloseNode))//那么现在情况是，离上一个末尾点最近的那个封闭环不是头结点，而且现在也没有封闭环被置位
					{
						//这个时候pLastAcceptCHead还是封闭环双向链表的头结点
						//应该把newNode->pminhead当成head指向的第一个封闭环节点
						head->FirstGeomClose = newNode->pminhead;
						newNode->pminhead->prevGeomcloseNode = NULL;//首结点
						newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
						pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
						newNode->pminhead->m_AcceptGeomcloseHEAD = true;//置位
					}
					//上一种情况是newNode->pminhead不会与pLastAcceptCHead相等
					else//那么将newNode->pminhead直接挂到pLastAcceptCHead后面去
					{
						//主要pLastAcceptCHead并不是已经被置位的，它是第一个没被置位的，pLastAcceptCHead->prevGeomcloseNode才是最后一个被置位的
						newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
						newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
						pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
						pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
						newNode->pminhead->m_AcceptGeomcloseHEAD = true;//置位
					}
				}
				pTempCHead = newNode->pminhead;//跳出这个循环
			}
			b;
			pTempCHead = pTempCHead->nextGeomcloseNode;
		}
	}
}
//将第二个封闭环头结作为第一个头结点
NestResultDataNode* GeomForCut::ChangeSencondCH2FH(NestResultDataNode*head)
{
	GeomCloseHEAD*pFirstCloseHead;
	pFirstCloseHead = head->FirstGeomClose;
	m_ceramic_Head = pFirstCloseHead;//保存瓷砖的部件
	pFirstCloseHead = pFirstCloseHead->nextGeomcloseNode;
	pFirstCloseHead->prevGeomcloseNode = NULL;
	head->FirstGeomClose = pFirstCloseHead;

	return head;
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//以下是加入切割工艺，即是按照切割过程中，如果有嵌套的先切割内环，再切割外环
//先获取两条直线的参数
void GeomForCut::GetLinePara(Line_Cross *l)
{
	l->a = l->p1.y - l->p2.y;
	l->b = l->p2.x - l->p1.x;
	l->c = l->p1.x*l->p2.y - l->p2.x*l->p1.y;
}
//保留四位小数
double GeomForCut::Retain4Decimals(double a)
{
	//4个0代表保留4个有效数字
	//int c = (a * 1000000 );
	//double b = c / 1000000.0;
	//int d = (b* 100000 );
	//double e = d / 100000.0;
	int f = (a * 10000+0.5 );
	double h = f / 10000.0;
	return h;

}

//输入决定两个直线的八个点，求得两条直线是否有相交点，有就返回交点，没有就返回-1
//这个算法，可以求垂直线和水平线之间的交点
Point GeomForCut::GetCrossPoint(Line_Cross *m_linePoint1, Line_Cross  *m_linePoint2)
{
	Point p;
	GetLinePara(m_linePoint1);
	GetLinePara(m_linePoint2);
	double D = m_linePoint1->a*m_linePoint2->b - m_linePoint2->a*m_linePoint1->b;
	if (0 != D)//当D!=0的时候，两条直线不平行，这时候才有交点的。
	{
		p.x = (m_linePoint1->b*m_linePoint2->c - m_linePoint2->b*m_linePoint1->c) / D;
		p.y = (m_linePoint1->c*m_linePoint2->a - m_linePoint2->c*m_linePoint1->a) / D;
		//对交点保留四位小数
		p.x = Retain4Decimals(p.x);
		p.y = Retain4Decimals(p.y);

	}
	else//当D=0的时候，两条直线平行，这时候是没有交点的。
	{
		//在排样图里面，交点的范围在0-800之间才有意义
		p.x = -1.0;
		p.y = -1.0;
	}
	return p;
}
//输入子封闭环，调整父封闭环的打孔点(更改父封闭环内部的图元节点的顺序)
void GeomForCut::ChangeOutsideGeomEleNode(GeomCloseHEAD*pKidCloseHead, GeomCloseHEAD*pFadClosedHead)
{
	GeomEleNode*pkidGelenode, *pfadGelenode,*pmindistantGelenode;
	double x0, y0, x1, y1;
	double m_distant, m_tempdistant;
	//找到子封闭的最后一个图元节点
	pkidGelenode = pKidCloseHead->FirstGeomele;
	while (pkidGelenode->nextGeomeleNode)
		pkidGelenode = pkidGelenode->nextGeomeleNode;
	x0 = pkidGelenode->m_GeomStandData.GeoEle_start_x1;
	y0 = pkidGelenode->m_GeomStandData.GeoEle_start_y1;
	pmindistantGelenode = pFadClosedHead->FirstGeomele;
	x1 = pmindistantGelenode->m_GeomStandData.GeoEle_start_x0;
	y1 = pmindistantGelenode->m_GeomStandData.GeoEle_start_y0;
	m_distant = fabs(sqrt(((x0 - x1)*(x0 - x1)) + ((y0 - y1)*(y0 - y1))));//这里求的是两点之间的最短距离
	pfadGelenode = pmindistantGelenode;
	while (pmindistantGelenode)//将父节点的图元全部遍历一遍
	{
		x1 = pmindistantGelenode->m_GeomStandData.GeoEle_start_x0;
		y1 = pmindistantGelenode->m_GeomStandData.GeoEle_start_y0;
		m_tempdistant = fabs(sqrt(((x0 - x1)*(x0 - x1)) + ((y0 - y1)*(y0 - y1))));//这里求的是两点之间的最短距离
		if (m_tempdistant<m_distant)
		{
			pfadGelenode = pmindistantGelenode;
			//不加下行代码，就会产生只要符合就进来覆盖却不管是不是真的最小
			m_distant = m_tempdistant;
		}
		pmindistantGelenode = pmindistantGelenode->nextGeomeleNode;
	}
	ChangeEleNodeForAloneClse(pFadClosedHead, pfadGelenode);

}
//输入第一层父封闭环的图元头结点，改变下一个第一层父封闭环的子封闭环的顺序,并把最后一个子封闭环输出
GeomCloseHEAD* GeomForCut::ChangeINsideGeomEleNode(GeomCloseHEAD*pFadClosedHead, GeomCloseHEAD*pNKidCloseHead)
{

	Mindistant_EleNode*newNode = (Mindistant_EleNode*)malloc(sizeof(Mindistant_EleNode));
	newNode->m_mindistant = 99999.0;
	newNode->pminhead = NULL;
	newNode->pminnode = NULL;
	GeomCloseHEAD*pTempCHead, *ppTempCHead;//用作遍历全部封闭环
	GeomCloseHEAD*pLastAcceptCHead;//用寻找最后一个被置位的封闭环头结点
	GeomCloseHEAD*pTheLastKidCHead=NULL;//用寻找最后一个被置位的封闭环头结点

	GeomCloseHEAD*pMinDistantCloseHead, *pntemp;//用寻找最后一个被置位的封闭环头结点

	GeomEleNode*pTempGelenode;
	double pre_x, pre_y, temp_x, temp_y;
	double m_temp_mindistant;//保存暂时的最短距离，如果是最终的最短距离，那么就该是给到	newNode->m_mindistant;
	double m_tempmindistant_origin;//到原点距离
	double m_tempmindistant_ptp;//到已经被置位的封闭环的距离
	double m_origin_x = 0.0;//机床原点
	double m_origin_y = 0.0;//机床原点
	int a = 0, b = 0, m_TotalCloseID, m_GeomCloseID;
	//算法开始
	//要注意第一层封闭环只有一个没有嵌套情况
	if (pNKidCloseHead->prevOutsideGCloseNode)//不是只有一层
	{
		pTempCHead = pNKidCloseHead;
		while (pTempCHead)//将这个子封闭环全部遍历一遍,直到最后一个跳出循环
		{
			a++;
			while (!(pTempCHead->m_KidAcceptGeomCloseHead))//只有当该封闭环还没有被规划的时候，进入才有意义
			{
				//pTempCHead能进来说明它是第一个没有被置位的封闭环，那么遍历该从它开始
				//如果TempCHead是第一个封闭环节点，那么它前面并没有已经被置位的封闭环，所以以工件原点作为上一个封闭环的起止点

				if (NULL == pTempCHead->prevGeomcloseNode)//是头结点
				{
					if (!(pFadClosedHead->prevGeomcloseNode))//如果pNKidCloseHead是第一层封闭环的第一个子封闭环，那么前面没有封闭环
					{
						pre_x = 0.0;
						pre_y = 0.0;
					}
					else//否则就该找到上一个封闭的起始点
					{
						pre_x = pFadClosedHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
						pre_y = pFadClosedHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
					}

				}
				else//那么要找到最后一个被置位的封闭环头结点
				{
					pre_x = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
					pre_y = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				}
				//以上已经找到上一个封闭的起止点了。
				//现在要以该起止点来遍历所有的封闭环节点，找到其到上一个封闭环起止点最短距离的那个节点和封闭环
				//要对剩下的封闭环进行遍历
				temp_x = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
				temp_y = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				//到原点的距离
				m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//这里求的是两点之间的最短距离
				//两个封闭环之间的距离
				m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//这里求的是两点之间的最短距离
				//求最短距离,暂时的最短距离
				newNode->m_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
				//newNode->m_mindistant = 1 / newNode->m_mindistant;
				newNode->pminhead = pTempCHead;
				newNode->pminnode = pTempCHead->FirstGeomele;
				pMinDistantCloseHead = pTempCHead;
				//把剩下的没有被置位的所有遍历一遍
				//以最短距离为准则从这里开始决定性下一个封闭环和头结点是什么
				b = 0;
				while (pMinDistantCloseHead)
				{
					b++;
					pTempGelenode = pMinDistantCloseHead->FirstGeomele;
					//注意要区分圆的封闭环
					if (3 != pTempGelenode->m_GeomStandData.m_typegeomele)//如果不是圆
					{
						while (pTempGelenode)//在这个封闭环里把节点全部遍历一遍
						{
							temp_x = pTempGelenode->m_GeomStandData.GeoEle_start_x0;
							temp_y = pTempGelenode->m_GeomStandData.GeoEle_start_y0;
							//到原点的距离
							m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//这里求的是两点之间的最短距离
							//两个封闭环之间的距离
							m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//这里求的是两点之间的最短距离
							m_temp_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
							//m_temp_mindistant = 1 / m_temp_mindistant;
							if (m_temp_mindistant < newNode->m_mindistant)
							{
								newNode->m_mindistant = m_temp_mindistant;
								newNode->pminhead = pMinDistantCloseHead;
								newNode->pminnode = pTempGelenode;
							}
							pTempGelenode = pTempGelenode->nextGeomeleNode;
						}
					}
					else//如果是圆的话
					{

						double a, b, r, x0, y0, c;
						double x1, y1, x2, y2;
						double distance_1, distance_2;
						a = pTempGelenode->m_GeomStandData.m_circle.m_Circent_x;
						b = pTempGelenode->m_GeomStandData.m_circle.m_Circent_y;
						r = pTempGelenode->m_GeomStandData.m_circle.m_Circle_r;
						x0 = pre_x;
						y0 = pre_y;
						if (x0 == a)
						{
							x1 = a;
							y1 = b - r;
							x2 = a;
							y2 = b + r;
						}
						else if (y0 == b)
						{
							x1 = a - r;
							y1 = b;
							x2 = a + r;
							y2 = b;
						}
						else
						{
							c = sqrt(pow(r, 2) / (1 + pow(((y0 - b) / (x0 - a)), 2)));
							x1 = a + c;
							y1 = b + ((y0 - b) / (x0 - a))*c;
							x2 = a - c;
							y2 = b - ((y0 - b) / (x0 - a))*c;
						}
						distance_1 = fabs(sqrt(((x1 - pre_x)*(x1 - pre_x)) + ((y1 - pre_y)*(y1 - pre_y))));//这里求的是两点之间的最短距离
						distance_2 = fabs(sqrt(((x2 - pre_x)*(x2 - pre_x)) + ((y2 - pre_y)*(y2 - pre_y))));//这里求的是两点之间的最短距离
						m_tempmindistant_ptp = distance_1;
						if (distance_2 < distance_1)
						{
							x1 = x2;
							y1 = y2;
							m_tempmindistant_ptp = distance_2;
						}
						m_temp_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
						//m_temp_mindistant = 1 / m_temp_mindistant;
						if (m_temp_mindistant < newNode->m_mindistant)
						{
							pTempGelenode->m_GeomStandData.GeoEle_start_x0 = x1;
							pTempGelenode->m_GeomStandData.GeoEle_start_y0 = y1;
							pTempGelenode->m_GeomStandData.GeoEle_start_x1 = x1;
							pTempGelenode->m_GeomStandData.GeoEle_start_y1 = y1;
							pTempGelenode->m_GeomStandData.m_circle.m_Circle_Start_Angle = m_geomele.ForCircleStartAngle(x1, y1, pTempGelenode->m_GeomStandData.m_circle);
							//
							newNode->m_mindistant = m_temp_mindistant;
							newNode->pminhead = pMinDistantCloseHead;
							newNode->pminnode = pTempGelenode;
						}
						else
						{
							;
						}
					}
					b;
					pMinDistantCloseHead = pMinDistantCloseHead->nextGeomcloseNode;

				}
				//以上已经找到基于现在这个被置位的封闭环的头结点的全局搜索
				//下面要做到是基于封闭环头结点和图元节点进行双向链表数据调整
				//先是把图元节点调整为封闭环的头结点
				GeomEleNode*phtemp, *ptemp;
				phtemp = newNode->pminhead->FirstGeomele;
				if (newNode->pminnode == phtemp)//说明找了一圈发现还是最开始的结点距离最短
				{
					//其实这约等于说明都没干
					newNode->pminhead->FirstGeomele = phtemp;
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//把头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
				}
				else
				{
					ptemp = phtemp;
					while (ptemp->nextGeomeleNode)//找到最后一个结点
						ptemp = ptemp->nextGeomeleNode;
					ptemp->nextGeomeleNode = phtemp;//原来ptemp作为尾结点，它的后向指针是指向NULL的
					phtemp->prevGeomeleNode = ptemp;//原来phtemp作为头结点，它的前向指针是指向NULL的
					newNode->pminnode->prevGeomeleNode->nextGeomeleNode = NULL;//原来paimnode的前向指针是指向它的，但是从此就作为尾结点了
					newNode->pminnode->prevGeomeleNode = NULL;//从此作为头结点
					newNode->pminhead->FirstGeomele = newNode->pminnode;//作为上面的工作，它已经是这个封闭环里的新头结点了
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = newNode->pminnode->m_GeomStandData.GeoEle_start_x0;//把新头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = newNode->pminnode->m_GeomStandData.GeoEle_start_y0;
				}
				//以上是循环一遍之后找到的目前来说距离最短的一个封闭环头结点
				//接着要对整个封闭环双向链表的顺序进行调整
				//说明newNode->pminnode恰好是头结点，又是最靠近上一封闭环的那个封闭环
				if (!(newNode->pminhead->prevGeomcloseNode))//如果是头结点，找了一遍发现还是头结点为最靠近的封闭环节节点
				{
					pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode = newNode->pminhead;//还是以它为子封闭环的头节点
					newNode->pminhead->prevOutsideGCloseNode = pNKidCloseHead->prevOutsideGCloseNode;
					newNode->pminhead->m_KidAcceptGeomCloseHead = true;//从此该节点就被固定了
				}
				//newNode->pminhead的前面刚好是被置位的
				//但会存在一种情况，就是恰好pLastAcceptCHead=newNode->pminhead;如此便能解决这个问题
				else if (newNode->pminhead->prevGeomcloseNode->m_KidAcceptGeomCloseHead)
				{
					//那么什么都不用动
					newNode->pminhead->m_KidAcceptGeomCloseHead = true;//从此该节点就被固定了

				}
				else//说明newNode->pminnode不是头结点了，那么这个节点要挂的目前最后一个m_AcceptGeomcloseHEAD没有被设置为true的封闭环节点后面
				{

					//经过上面的寻找，现在pLastAcceptCHead是最后一个被置位的节点，而newNode->pminhead就是要挂到pLastAcceptCHead后面
					//本来是每次调整双向链表的节点位置时候，都要考虑该节点是不是头尾还是一般节点的，但这里可以不用考虑头结点了
					//那么要先找到最后一个没被置位的封闭环，不是头结点也不代表现在前面有了被置位的封闭环
					pLastAcceptCHead = pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode;
					while (pLastAcceptCHead->m_KidAcceptGeomCloseHead)
						pLastAcceptCHead = pLastAcceptCHead->nextGeomcloseNode;
					//pLastAcceptCHead是没有被置位的
					if (!(newNode->pminhead->nextGeomcloseNode))//如果是尾节点
					{
						//既然已经是尾节点newNode->pminhead，那么直接把他前面一个值得后向节点指向NULL；
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = NULL;
					}
					else//一般节点
					{
						//newNode->pminhead原来前后指向关系的调节
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead->nextGeomcloseNode;
						newNode->pminhead->nextGeomcloseNode->prevGeomcloseNode = newNode->pminhead->prevGeomcloseNode;
					}
					//以上处理完了newNode->pminhead在原来位置的关系，接下来要处理他在被置位时候的关系了
					//如果pLastAcceptCHead还是头节点，说明这一个封闭环都还没有被置位
					//要考虑的一点是当pLastAcceptCHead=newNode->pminhead的时候怎么办
					if (!(pLastAcceptCHead->prevGeomcloseNode))//那么现在情况是，离上一个末尾点最近的那个封闭环不是头结点，而且现在也没有封闭环被置位
					{
						//这个时候pLastAcceptCHead还是封闭环双向链表的头结点
						//应该把newNode->pminhead当成head指向的第一个封闭环节点
						pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode = newNode->pminhead;
						newNode->pminhead->prevOutsideGCloseNode = pNKidCloseHead->prevOutsideGCloseNode;
						newNode->pminhead->prevGeomcloseNode = NULL;//首结点
						newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
						pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
						newNode->pminhead->m_KidAcceptGeomCloseHead = true;//置位
					}
					//上一种情况是newNode->pminhead不会与pLastAcceptCHead相等
					else//那么将newNode->pminhead直接挂到pLastAcceptCHead后面去
					{
						//主要pLastAcceptCHead并不是已经被置位的，它是第一个没被置位的，pLastAcceptCHead->prevGeomcloseNode才是最后一个被置位的
						newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
						newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
						pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
						pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
						newNode->pminhead->m_KidAcceptGeomCloseHead = true;//置位
					}
				}
				pTempCHead = newNode->pminhead;//跳出这个循环
			}
			b;
			if (!(pTempCHead->nextGeomcloseNode))//说明这是最后一个子封闭环
			{
				pTheLastKidCHead = pTempCHead;
			}
			pTempCHead = pTempCHead->nextGeomcloseNode;
		}
	}
	else
	{
		pTheLastKidCHead = pNKidCloseHead;
	}	
	return pTheLastKidCHead;
}
//核心代码！
//核心代码！
//核心代码！
//将整张图的封闭环是否嵌套全部找出来，并按照是否嵌套的关系挂到不同的封闭环头结点上
void GeomForCut::Find_AdjustNestCloseHead(NestResultDataNode*head)
{
	int a = 0;//找bug使用
	GeomCloseHEAD*pTempCHead, *pTempNextCHead;//用作遍历全部封闭环
	GeomCloseHEAD*pNextCHead;//作为pTempNextCHead被查出是pTempCHead的子封闭环后，要离开大封闭环的交接环
	m_G2CloseHeadNest.m_IfCloseNest= true;//判断第二个封闭环是不是第一个封闭环的子封闭环
	//算法开始
	pTempCHead = head->FirstGeomClose;
	//不要全部遍历一遍，当只有最后一个封闭环的时候就可以跳出去了，因为要嵌套起码要两个封闭环
	//用while (pTempCHead->nextGeomcloseNode)居然出错，原因在于pTempCHead之前的后一个现在变成了它的子封闭环，那么就会让pTempCHead变成最后一个
	//那么它就成了最后一个，那么经过下面的pTempCHead = pTempCHead->nextGeomcloseNode;
	//此时pTempCHead已经是NULL，所以pTempCHead->nextGeomcloseNode是会报错的
	//所以要改为while (pTempCHead)
	pTempCHead->m_Singlelayer = true;//这个是800*800的板材，是第一层也就是单层
	while (pTempCHead)//倒数第二个可以进入循环，最后一个就要跳出循环了
	{
		pNextCHead = head->FirstGeomClose;
		//这是按照每一个都是大环的假设进行对余下的封闭环进行排查
		while (pNextCHead)//遍历完一遍
		{
			//给pNextCHead找一个替身pTempNextCHead,
			pTempNextCHead = pNextCHead;
			////////////用射线的方式判断这两个封闭环有没有嵌套
			////////m_G2CloseHeadNest = EstimateCloseHeadNest(pTempCHead, pTempNextCHead);
			//用包络的方式来判断两个封闭环有没有嵌套
			m_G2CloseHeadNest = JudgeCloseHeadNest(pTempCHead, pTempNextCHead);
			//如果有嵌套，那么就要把第二个封闭环挂到第一个封闭上
			if (m_G2CloseHeadNest.m_IfCloseNest)
			{
				//把第二个封闭环环挂到第一个封闭上
				pTempNextCHead = m_G2CloseHeadNest.KidCloseHead;
				SetInSideClose(pTempCHead, pTempNextCHead);
				pNextCHead = m_G2CloseHeadNest.NextCloseHead;//上面的函数已经对这里进行的 pNextCHead->nextGeomcloseNode类似的工作
			}
			else//如果上面没有嵌套，那么现在的pNextCHead还没有上面变化，还可以继续找到原来的下一个
			{
				pNextCHead = pNextCHead->nextGeomcloseNode;

			}
		}	
		
		a++;
		pTempCHead = pTempCHead->nextGeomcloseNode;
	}
}//以上代码实现了，单双层判断和外包络矩形的控制点记录
//核心代码！
//核心代码！
//核心代码！
//判断两个封闭环是否具有嵌套
//递归方程
Geom2CloseHeadNest  GeomForCut::EstimateCloseHeadNest(GeomCloseHEAD*pTempCHead, GeomCloseHEAD*pTempNextCHead)
{
	GeomEleNode*ptemp1Gelenode, *ptemp2Gelenode;
	double pre_x, pre_y, temp_x, temp_y;
	double x1_min, x1_max, y1_min, y1_max;//直线1的范围
	double x2_min, x2_max, y2_min, y2_max;//直线2的范围

	//从第一个封闭环的第一个图元节点开始
	//要第一个封闭环对三角形，和圆进行区分
	ptemp1Gelenode = pTempCHead->FirstGeomele;
	if (3 != ptemp1Gelenode->m_GeomStandData.m_typegeomele)//如果不是圆的情况下
	{
		//判断是不是三角形
		if (NULL == ptemp1Gelenode->nextGeomeleNode->nextGeomeleNode)//三角形
		{
			//对于三角形而言，要以顶点向对边中点取直线
			;
		}
		else//一般多边形
		{
			//第一个while是要保证第一个封闭环的所有顶点可能产生的对角线找完备。
			while (ptemp1Gelenode->nextGeomeleNode->nextGeomeleNode)//如果当现在的节点的下一个的下一个就是NULL的时候，那么跳出循环，因为那些线以及和前面重复了
			{

				m_line1Point.p1.x = ptemp1Gelenode->m_GeomStandData.GeoEle_start_x0;//当前图元数据的起始点
				m_line1Point.p1.y = ptemp1Gelenode->m_GeomStandData.GeoEle_start_y0;
				m_line1Point.p2.x = ptemp1Gelenode->nextGeomeleNode->nextGeomeleNode->m_GeomStandData.GeoEle_start_x0;//再下一个图元数据的起始点（所以才要判断有没有再下一个数据图元）
				m_line1Point.p2.y = ptemp1Gelenode->nextGeomeleNode->nextGeomeleNode->m_GeomStandData.GeoEle_start_y0;
				//求得直线1的xy范围值
				x1_min = m_line1Point.p1.x;
				x1_max = m_line1Point.p2.x;
				if (x1_max <= x1_min)
				{
					x1_max = x1_min;
					x1_min = m_line1Point.p2.x;
				}
				y1_min = m_line1Point.p1.y;
				y1_max = m_line1Point.p2.y;
				if (y1_max <= y1_min)
				{
					y1_max = y1_min;
					y1_min = m_line1Point.p2.y;
				}
				
				if (pTempNextCHead == pTempCHead)//因为从头开始，肯定会有两者相同的时候
				{
					//当pTempCHead与 pTempNextCHead相等的时候，要注意判断是不是最后一个封闭环
					if (!(pTempNextCHead->nextGeomcloseNode))//最后一个封闭环的时候，自己不可能包含自己
					{
						m_G2CloseHeadNest.m_IfCloseNest = false;
						m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
						return m_G2CloseHeadNest;
					}
					else//不是最后一个的时候
					{
						pTempNextCHead = pTempNextCHead->nextGeomcloseNode;
					}
					
				}
					ptemp2Gelenode = pTempNextCHead->FirstGeomele;
					//要判断这个封闭环是不是圆
					if (3 != ptemp2Gelenode->m_GeomStandData.m_typegeomele)//如果不是圆
					{
						//第二个while是要保证第二个封闭环的边都被检查了一遍
						while (ptemp2Gelenode)//那么要全部遍历一遍
						{
							m_line2Point.p1.x = ptemp2Gelenode->m_GeomStandData.GeoEle_start_x0;
							m_line2Point.p1.y = ptemp2Gelenode->m_GeomStandData.GeoEle_start_y0;
							m_line2Point.p2.x = ptemp2Gelenode->m_GeomStandData.GeoEle_start_x1;
							m_line2Point.p2.y = ptemp2Gelenode->m_GeomStandData.GeoEle_start_y1;

							//求得直线2的xy范围值
							x2_min = m_line2Point.p1.x;
							x2_max = m_line2Point.p2.x;
							if (x2_max <= x2_min)
							{
								x2_max = x2_min;
								x2_min = m_line2Point.p2.x;
							}
							y2_min = m_line2Point.p1.y;
							y2_max = m_line2Point.p2.y;
							if (y2_max <= y2_min)
							{
								y2_max = y2_min;
								y2_min = m_line2Point.p2.y;
							}
								//求得交点
							m_PointCross = GetCrossPoint(&m_line1Point, &m_line2Point);
							//判断返回来x的值在不在m_line1Point->p1.x和m_line1Point->p2.x而且y的值m_line1Point->p1.y和m_line1Point->p2.y的范围内
							//因为当直线垂直x轴的时候，直线与任何直线的交点的x值都在这个范围内，所以只有两个值都在这个范围内，才是嵌套包含的关系
							//要先将这四个数保留四位小数
							x1_min = Retain4Decimals(x1_min);
							x1_max = Retain4Decimals(x1_max);
							y1_min = Retain4Decimals(y1_min);
							y1_max = Retain4Decimals(y1_max);
							x2_min = Retain4Decimals(x2_min);
							x2_max = Retain4Decimals(x2_max);
							y2_min = Retain4Decimals(y2_min);
							y2_max = Retain4Decimals(y2_max);
							if (((x1_min <= m_PointCross.x) && (m_PointCross.x <= x1_max)) && ((y1_min <= m_PointCross.y) && (m_PointCross.y <= y1_max)) && ((x2_min <= m_PointCross.x) && (m_PointCross.x <= x2_max)) && ((y2_min <= m_PointCross.y) && (m_PointCross.y <= y2_max)))
							{
								//如果有交点的情况下，那么应该把这个封闭环2挂到封闭环1中，作为它的子封闭环
								m_G2CloseHeadNest.m_IfCloseNest = true;
								m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
								m_G2CloseHeadNest.NextCloseHead = pTempNextCHead->nextGeomcloseNode;
								return m_G2CloseHeadNest;
							}
							else//不在这个范围内，说明不是交点
							{
								;
							}
							ptemp2Gelenode = ptemp2Gelenode->nextGeomeleNode;
						}
						m_G2CloseHeadNest.m_IfCloseNest = false;
						m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
						return m_G2CloseHeadNest;
					}
					else//如果是嫌疑子封闭环是圆的情况下
					{
						//是圆的情况下，那么就要直接求直线与圆有没有交点就好了。
						;
					}
				ptemp1Gelenode = ptemp1Gelenode->nextGeomeleNode;
			}
		}

	}
	else//封闭环是圆的情况下
	{
		;
	}
}
//核心代码！
//核心代码！
//核心代码！
//把pHNtemp挂到pHtemp的子封闭环上
void  GeomForCut::SetInSideClose(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp)
{
	
	GeomCloseHEAD*pKidHtemp=NULL;
	GeomCloseHEAD*pKidLastClose = NULL;//子封闭环所在的封闭环双向链表中的最后一个封闭环链表
	bool m_IfCloseNest = false;//判断第二个封闭环是不是第一个封闭环的子封闭环
	//先要判断pHtemp有没有子封闭环，如果有的话又得区分pHNtemp和原来的子封闭环有没有内嵌的关系
	if (!(pHtemp->FirstInsideGCloseNode))//如果原来并没有子封闭环
	{
		pHtemp->FirstInsideGCloseNode = pHNtemp;
		//同时，要把pHNtemp这个双向链表的前后结点的指向更改
		//这里要注意pHNtemp是最后一个封闭环的情况，但不可能是第一个封闭环
		if (pHNtemp->nextGeomcloseNode)//如果pHNtemp不是最后的封闭环
		{
			pHNtemp->prevGeomcloseNode->nextGeomcloseNode = pHNtemp->nextGeomcloseNode;//pHNtemp的前面的后面原本是pHNtemp，但现在改是它的后面的那个了
			pHNtemp->nextGeomcloseNode->prevGeomcloseNode = pHNtemp->prevGeomcloseNode;//pHNtemp的后面的前面原本是pHNtemp，但现在改是它的前面的那个了
		}
		else//如果pHNtemp是最后的封闭环节点的情况下
		{
			pHNtemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
		}
		//调整完了之后，pHNtemp就该把前后变为NULL了。
		pHNtemp->prevGeomcloseNode =pHNtemp->nextGeomcloseNode = NULL;
		//指向其父节点
		pHNtemp->prevOutsideGCloseNode = pHtemp;
	}
	else//如果现在pHtemp已经有了子封闭环了，那么要判断现在的子封闭环和原来的子封闭环有没有嵌套关系
	{
		pKidHtemp = pHtemp->FirstInsideGCloseNode;
		//不要全部遍历一遍，当只有最后一个封闭环的时候就可以跳出去了，因为要嵌套起码要两个封闭环
		while (pKidHtemp)//把先有的子封闭环遍历一遍
		{
			//这是按照每一个都是大环的假设进行规划
			//输入这两个封闭环头结点判断是否有嵌套关系
			//////////判断这两个封闭环有没有嵌套
			////////m_G2CloseHeadNest = EstimateCloseHeadNest(pKidHtemp, pHNtemp);
			//用包络的方式来判断两个封闭环有没有嵌套
			m_G2CloseHeadNest = JudgeCloseHeadNest(pKidHtemp, pHNtemp);

			//如果有嵌套，那么就要把第二个封闭环挂到第一个封闭上
			if (m_G2CloseHeadNest.m_IfCloseNest)
			{
				//把第二个封闭环环挂到第一个封闭上
				pHNtemp = m_G2CloseHeadNest.KidCloseHead;
				SetInSideClose(pKidHtemp, pHNtemp);
			}
			else//如果没有嵌套，
			{
				//这里要把pHNtemp原来的关系进行交接
				//这里要注意pHNtemp是最后一个封闭环的情况，但不可能是第一个封闭环
				if (pHNtemp->nextGeomcloseNode)//如果pHNtemp不是最后的封闭环
				{
					pHNtemp->prevGeomcloseNode->nextGeomcloseNode = pHNtemp->nextGeomcloseNode;//pHNtemp的前面的后面原本是pHNtemp，但现在改是它的后面的那个了
					pHNtemp->nextGeomcloseNode->prevGeomcloseNode = pHNtemp->prevGeomcloseNode;//pHNtemp的后面的前面原本是pHNtemp，但现在改是它的前面的那个了
				}
				else//如果pHNtemp是最后的封闭环节点的情况下
				{
					pHNtemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
				}
				//并列封闭环的关系
				//那么就要分pKidHtemp所在的子封闭环双向链表封闭环了，如果没有其他直接把pHNtemp挂到pKidHtemp后面就好了
				if (!(pKidHtemp->nextGeomcloseNode))//如果后面没有其他封闭环了
				{
					pKidHtemp->nextGeomcloseNode = pHNtemp;//pKidHtemp作为新进入另一个领域的封闭环，他已经把原来的关系交接完了之后丢掉了，变成了没有身份的了，此时前后节点都是NULL
					//调整完了之后，pHNtemp就该把这个子封闭环里面的关系建立起来
					pHNtemp->prevGeomcloseNode = pKidHtemp;
					pHNtemp->nextGeomcloseNode = NULL;
				}
				else//pKidHtemp所在的子封闭环双向链表封闭环后面有其他封闭环
				{
					//既然后面还有其他封闭环，那么有可能是跟它后面的封闭环嵌套，而不是跟头结点封闭环
					//判断pHNtemp与pKidHtemp后面的封闭环是否构成嵌套关系，如果构成那么要挂到后面，
					//不要全部遍历一遍，当只有最后一个封闭环的时候就可以跳出去了，因为要嵌套起码要两个封闭环
					pKidHtemp = pKidHtemp->nextGeomcloseNode;
					while (pKidHtemp)//把子封闭环里面余下的并列的封闭环遍历一遍
					{
						//用包络的方式来判断两个封闭环有没有嵌套
						m_G2CloseHeadNest = JudgeCloseHeadNest(pKidHtemp, pHNtemp);

						//如果有嵌套，那么就要把第二个封闭环挂到第一个封闭上
						if (m_G2CloseHeadNest.m_IfCloseNest)
						{
							//把第二个封闭环环挂到第一个封闭上
							pHNtemp = m_G2CloseHeadNest.KidCloseHead;
							SetInSideClose(pKidHtemp, pHNtemp);
							m_IfCloseNest = true;//说明已经嵌套进去了
							break;
						}
						//否则找到最后一个，然后把pHNtemp挂到其后面
						if (!(pKidHtemp->nextGeomcloseNode))//如果pKidHtemp是最后一个
						{
							pKidLastClose = pKidHtemp;//这是最后一个封闭环，会到这来，说明，最后一个封闭环和pHNtemp也没有嵌套的关系
							m_IfCloseNest = false;
						}
						pKidHtemp = pKidHtemp->nextGeomcloseNode;
					}			
					if (!(m_IfCloseNest))
					{
						pKidLastClose->nextGeomcloseNode = pHNtemp;
						//调整完了之后，pHNtemp就该把这个子封闭环里面的关系建立起来
						pHNtemp->prevGeomcloseNode = pKidLastClose;
						pHNtemp->nextGeomcloseNode = NULL;
					}
					
				}
				
			}
			//这个while循环不需要再从下面这个代码去循环，因为这是一个自递归的函数，当去到上面的else时候，整个while其实应该是要停止了
			//pKidHtemp = pKidHtemp->nextGeomcloseNode;
			//如果用上面的代码，将会使得pKidHtemp会跟自己循环一次
			break;
		}
	}
}
//求封闭环的包络矩形xy的极限值

void  GeomForCut::GetLimtofGeomClosed(GeomCloseHEAD*pTempCHead)
{
	GeomEleNode*ptempGelenode;
	double temp_x_min, temp_x_max, temp_y_min, temp_y_max;
	double x_min, x_max, y_min, y_max;//寻找封闭环1的包络矩形的范围
	//从第一个封闭环的第一个图元节点开始
	//要第一个封闭环对三角形，和圆进行区分
	ptempGelenode = pTempCHead->FirstGeomele;
	if (3 != ptempGelenode->m_GeomStandData.m_typegeomele)//如果不是圆的情况下
	{
		//先给以一个初始化的值
		x_min = ptempGelenode->m_GeomStandData.GeoEle_start_x0;
		x_max = ptempGelenode->m_GeomStandData.GeoEle_start_x0;
		y_min = ptempGelenode->m_GeomStandData.GeoEle_start_y0;
		y_max = ptempGelenode->m_GeomStandData.GeoEle_start_y0;

		//要保证封闭环的所有顶点的数据都被遍历
		while (ptempGelenode)//如果当现在的节点的下一个的下一个就是NULL的时候，那么跳出循环，因为那些线以及和前面重复了
		{
			//temp_x1_min与temp_x1_max其实不分大小
			//圆弧也已经变成了这种直线模式
			temp_x_min = ptempGelenode->m_GeomStandData.GeoEle_start_x0;
			temp_x_max = ptempGelenode->m_GeomStandData.GeoEle_start_x1;
			temp_y_min = ptempGelenode->m_GeomStandData.GeoEle_start_y0;
			temp_y_max = ptempGelenode->m_GeomStandData.GeoEle_start_y1;
			//x的limit
			if ((temp_x_min <= x_min) || (temp_x_max <= x_min))
			{
				if (temp_x_min <= temp_x_max)
				{
					x_min = temp_x_min;
				}
				else
				{
					x_min = temp_x_max;
				}

			}
			if ((x_max <= temp_x_min) || (x_max <= temp_x_max))
			{
				if (x_max <= temp_x_max)
				{
					x_max = temp_x_max;
				}
				else
				{
					x_max = temp_x_min;
				}
			}
			//y的limit
			if ((temp_y_min <= y_min) || (temp_y_max <= y_min))
			{
				if (temp_y_min <= temp_y_max)
				{
					y_min = temp_y_min;
				}
				else
				{
					y_min = temp_y_max;
				}

			}
			if ((y_max <= temp_y_min) || (y_max <= temp_y_max))
			{
				if (y_max <= temp_y_max)
				{
					y_max = temp_y_max;
				}
				else
				{
					y_max = temp_y_min;
				}
			}
			ptempGelenode = ptempGelenode->nextGeomeleNode;
		}
	}
	else//封闭是圆的情况下，那么就采用过圆心的水平线和垂直线与圆的交点的两个值
	{
		;
	}
	x_min = Retain4Decimals(x_min);
	x_max = Retain4Decimals(x_max);
	y_min = Retain4Decimals(y_min);
	y_max = Retain4Decimals(y_max);
	pTempCHead->m_GemoClosedLimt.x_min = x_min;
	pTempCHead->m_GemoClosedLimt.x_max = x_max;
	pTempCHead->m_GemoClosedLimt.y_min = y_min;
	pTempCHead->m_GemoClosedLimt.y_max = y_max;
}

//核心算法
//核心算法
//核心算法
//用包络的方法来判断两个封闭环是否嵌套
Geom2CloseHeadNest  GeomForCut::JudgeCloseHeadNest(GeomCloseHEAD*pTempCHead, GeomCloseHEAD*pTempNextCHead)

{
	Envelope_Rect m_GeomClosed1, m_GeomClosed2;
	double x1_min, x1_max, y1_min, y1_max;//寻找封闭环1的包络矩形的范围
	double x2_min, x2_max, y2_min, y2_max;//寻找封闭环1的包络矩形的范围
	if (pTempNextCHead == pTempCHead)//因为从头开始，肯定会有两者相同的时候
	{
		//当pTempCHead与 pTempNextCHead相等的时候，要注意判断是不是最后一个封闭环
		if (!(pTempNextCHead->nextGeomcloseNode))//最后一个封闭环的时候，自己不可能包含自己
		{
			m_G2CloseHeadNest.m_IfCloseNest = false;
			m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
			m_G2CloseHeadNest.NextCloseHead = NULL;
			return m_G2CloseHeadNest;
		}
		else//不是最后一个的时候
		{
			pTempNextCHead = pTempNextCHead->nextGeomcloseNode;
		}

	}
	//求两个封闭环的包络矩形的范围
	GetLimtofGeomClosed(pTempCHead);
	GetLimtofGeomClosed(pTempNextCHead);
	m_GeomClosed1 =pTempCHead->m_GemoClosedLimt;
	m_GeomClosed2 =pTempNextCHead->m_GemoClosedLimt;
	//对数值保留四位小数
	x1_min = m_GeomClosed1.x_min;
	x1_max = m_GeomClosed1.x_max;
	y1_min = m_GeomClosed1.y_min;
	y1_max = m_GeomClosed1.y_max;
	x2_min = m_GeomClosed2.x_min;
	x2_max = m_GeomClosed2.x_max;
	y2_min = m_GeomClosed2.y_min;
	y2_max = m_GeomClosed2.y_max;
	//以上已经把包络矩形的范围求出来了
	//下面进行对两个矩形是否嵌套进行判断
	//主要是判断后面那个封闭环是不是前面封闭环的子封闭环
	//这里默认同个封闭环里的数值，min<max
	if (((x1_min <=x2_min ) && (x2_max <= x1_max)) && ((y1_min <= y2_min) && (y2_max<=y1_max)))
	{
		//如果有交点的情况下，那么应该把这个封闭环2挂到封闭环1中，作为它的子封闭环
		m_G2CloseHeadNest.m_IfCloseNest = true;
		m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
		//根据父封闭环是单双层来判断子封闭环的单双层
		if (pTempCHead->m_Singlelayer)//能进来说明是单层，因为默认是单层
		{
			pTempNextCHead->m_Singlelayer = false;//因为它已经是人家的子封闭环，而他爹是单层，他就得是双层
		}
		else//pTempCHead->m_Singlelayer如果父封闭环是双层，那么子封闭环就得是单层，也就是不用变
		{
			pTempNextCHead->m_Singlelayer = true;//父是双层的时候，这里应该是单层了
		}
		m_G2CloseHeadNest.NextCloseHead = pTempNextCHead->nextGeomcloseNode;
		
	}
	else//不在这个范围内，说明不是交点
	{
		m_G2CloseHeadNest.m_IfCloseNest = false;
		m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
		m_G2CloseHeadNest.NextCloseHead = pTempNextCHead->nextGeomcloseNode;
	}
	return m_G2CloseHeadNest;
}
//核心算法
//核心算法
//核心算法
//蚁群算法调整第一层封闭环
void GeomForCut::Base_ACO_ForFirstCHead(NestResultDataNode*head)
{
	;
}
//核心算法
//核心算法
//核心算法
//最终的处理，把子封闭环的顺序调整，把父封闭环的打孔点也调整
//void GeomForCut::BaseTS_GR_ForKidCHead(NestResultDataNode*head)
//{
//	GeomCloseHEAD*pKidCloseHead, *pFadClosedHead, *pFirstClosedHead, *tempCloseHead, *pTheLastKidCHead;
//	double x_C0, y_C0;//上一个第一层封闭环的尾节点，用作寻找下一个第一层封闭环子封闭环的打孔点，或下一个第一层封闭环的打孔点的选择
//	
//	pFirstClosedHead = head->FirstGeomClose;
//	//把第一层封闭环循环完就可以了
//	tempCloseHead = pFirstClosedHead;
//	while (tempCloseHead)
//	{
//		pKidCloseHead = tempCloseHead->FirstInsideGCloseNode;
//		while (pKidCloseHead->FirstInsideGCloseNode)//寻找到最后一层子封闭环
//			pKidCloseHead = pKidCloseHead->FirstInsideGCloseNode;
//			pFadClosedHead = tempCloseHead;
//		//输入第一层封闭环和第一层下一个封闭环的子封闭环，输出最后一个子封闭环
//		pTheLastKidCHead = ChangeINsideGeomEleNode(pFadClosedHead, pKidCloseHead);
//		if (tempCloseHead)//只要不是NULL，都应该根据最后一个子封闭环来调整上一层父封闭环的头结点
//		{
//			ChangeOutsideGeomEleNode(pTheLastKidCHead, tempCloseHead);
//		}
//		tempCloseHead = tempCloseHead->nextGeomcloseNode;
//	}
//}


//第一个就是离原点最近的封闭环群组
void GeomForCut::BaseTS_GR_ForKidCHead(NestResultDataNode*head)
{
	GeomCloseHEAD*pKidCloseHead, *pFadClosedHead, *pFirstClosedHead, *tempCloseHead, *pTheLastKidCHead;
	GeomCloseHEAD*tempfadCloseHead=NULL;
	GeomCloseHEAD*pFindTheFCH;
	double x_C0, y_C0;//上一个第一层封闭环的尾节点，用作寻找下一个第一层封闭环子封闭环的打孔点，或下一个第一层封闭环的打孔点的选择

	pFirstClosedHead = head->FirstGeomClose;
	//把第一层封闭环循环完就可以了
	while (pFirstClosedHead)
	{
		
		pKidCloseHead =  pFirstClosedHead->FirstInsideGCloseNode;
		//当第一层封闭环只有一层的时候，pKidCloseHead为NULL
		if (!(pKidCloseHead))
		{
			//当没有子封闭环时候，就以自身
			pKidCloseHead = pFirstClosedHead;
		}
		else
		{
			while (pKidCloseHead->FirstInsideGCloseNode)//寻找到最后一层子封闭环
				pKidCloseHead = pKidCloseHead->FirstInsideGCloseNode;
		}
		
		pFadClosedHead = pFirstClosedHead;
		//输入第一层封闭环和第一层下一个封闭环的子封闭环，输出最后一个子封闭环
		pTheLastKidCHead = ChangeINsideGeomEleNode(pFirstClosedHead, pKidCloseHead);
		//对上一层的封闭环节点调节
		pFindTheFCH = pTheLastKidCHead;
		while (pFindTheFCH->prevGeomcloseNode)//找到最下一层封闭环的头结点
			pFindTheFCH = pFindTheFCH->prevGeomcloseNode;
		//倒数第二层
			tempfadCloseHead = pFindTheFCH->prevOutsideGCloseNode;
			//从倒数第二层开始到第二层进行切割路径规划和打孔点规划
		while (tempfadCloseHead)//全部遍历一遍
		{
			
			//输入子封闭环的最后一个节点，调节上一层的父封闭环
			ChangeOutsideGeomEleNode(pTheLastKidCHead, tempfadCloseHead);
			//如果这一层已经是第一层的封闭环，那么不需要规划，之前已经用ACO规划过了，直接跳出
			if (tempfadCloseHead == pFirstClosedHead)
			{
				break;
			}
			//如果这层的父封闭环有兄弟封闭环，而且不是第一层的封闭环；
			//那么就该按照这个父封闭环为基准调整它所在的这一层
			else
			{
				//tempfadCloseHead是每一层封闭环的头结点
				//调整这一层的切割顺序，并输出最后一个封闭环
				pTheLastKidCHead = ChangeGeomCHead(tempfadCloseHead);
			}
			tempfadCloseHead = tempfadCloseHead->prevOutsideGCloseNode;
			
		}
		//以上只是把第一层的第一个处理完了。
		//下面应该是根据现在的切割控制点，寻找下一个第一层封闭环，
		//输入第一层，然后调整第一层的顺序
		pFirstClosedHead = Find_Next_TheFirstLevel(pFirstClosedHead);
		pFirstClosedHead = pFirstClosedHead->nextGeomcloseNode;
	}
}
GeomCloseHEAD*GeomForCut::ChangeGeomCHead(GeomCloseHEAD*pNKidCloseHead)
{

	Mindistant_EleNode*newNode = (Mindistant_EleNode*)malloc(sizeof(Mindistant_EleNode));
	newNode->m_mindistant = 99999.0;
	newNode->pminhead = NULL;
	newNode->pminnode = NULL;
	GeomCloseHEAD*pTempCHead, *ppTempCHead;//用作遍历全部封闭环
	GeomCloseHEAD*pLastAcceptCHead;//用寻找最后一个被置位的封闭环头结点
	GeomCloseHEAD*pTheLastKidCHead = NULL;//用寻找最后一个被置位的封闭环头结点

	GeomCloseHEAD*pMinDistantCloseHead, *pntemp;//用寻找最后一个被置位的封闭环头结点

	GeomEleNode*pTempGelenode;
	double pre_x, pre_y, temp_x, temp_y;
	double m_temp_mindistant;//保存暂时的最短距离，如果是最终的最短距离，那么就该是给到	newNode->m_mindistant;
	double m_tempmindistant_origin;//到原点距离
	double m_tempmindistant_ptp;//到已经被置位的封闭环的距离
	double m_origin_x = 0.0;//机床原点
	double m_origin_y = 0.0;//机床原点
	int a = 0, b = 0, m_TotalCloseID, m_GeomCloseID;
	//算法开始
	if (pNKidCloseHead->nextGeomcloseNode)//如果有兄弟封闭环的话
	{
		pTempCHead = pNKidCloseHead;
		pTempCHead->m_KidAcceptGeomCloseHead = true;
		while (pTempCHead)//将这个子封闭环全部遍历一遍,直到最后一个跳出循环
		{
			a++;
			while (!(pTempCHead->m_KidAcceptGeomCloseHead))//只有当该封闭环还没有被规划的时候，进入才有意义
			{
				//pTempCHead能进来说明它是这个封闭环双向链表的第一个没有被置位的封闭环，
				//前面已经有一个封闭环被置位了

					pre_x = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
					pre_y = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				
				//以上已经找到上一个封闭的起止点了。
				//现在要以该起止点来遍历所有的封闭环节点，找到其到上一个封闭环起止点最短距离的那个节点和封闭环
				//要对剩下的封闭环进行遍历
				temp_x = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
				temp_y = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				//到原点的距离
				m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//这里求的是两点之间的最短距离
				//两个封闭环之间的距离
				m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//这里求的是两点之间的最短距离
				//求最短距离,暂时的最短距离
				newNode->m_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
				newNode->pminhead = pTempCHead;
				newNode->pminnode = pTempCHead->FirstGeomele;
				pMinDistantCloseHead = pTempCHead;
				//把剩下的没有被置位的所有遍历一遍
				//以最短距离为准则从这里开始决定性下一个封闭环和头结点是什么
				b = 0;
				while (pMinDistantCloseHead)
				{
					b++;
					pTempGelenode = pMinDistantCloseHead->FirstGeomele;
					//注意要区分圆的封闭环
					if (3 != pTempGelenode->m_GeomStandData.m_typegeomele)//如果不是圆
					{
						while (pTempGelenode)//在这个封闭环里把节点全部遍历一遍
						{
							temp_x = pTempGelenode->m_GeomStandData.GeoEle_start_x0;
							temp_y = pTempGelenode->m_GeomStandData.GeoEle_start_y0;
							//到原点的距离
							m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//这里求的是两点之间的最短距离
							//两个封闭环之间的距离
							m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//这里求的是两点之间的最短距离
							m_temp_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
							if (m_temp_mindistant < newNode->m_mindistant)
							{
								newNode->m_mindistant = m_temp_mindistant;
								newNode->pminhead = pMinDistantCloseHead;
								newNode->pminnode = pTempGelenode;
							}
							pTempGelenode = pTempGelenode->nextGeomeleNode;
						}
					}
					else//如果是圆的话
					{

						double a, b, r, x0, y0, c;
						double x1, y1, x2, y2;
						double distance_1, distance_2;
						a = pTempGelenode->m_GeomStandData.m_circle.m_Circent_x;
						b = pTempGelenode->m_GeomStandData.m_circle.m_Circent_y;
						r = pTempGelenode->m_GeomStandData.m_circle.m_Circle_r;
						x0 = pre_x;
						y0 = pre_y;
						if (x0 == a)
						{
							x1 = a;
							y1 = b - r;
							x2 = a;
							y2 = b + r;
						}
						else if (y0 == b)
						{
							x1 = a - r;
							y1 = b;
							x2 = a + r;
							y2 = b;
						}
						else
						{
							c = sqrt(pow(r, 2) / (1 + pow(((y0 - b) / (x0 - a)), 2)));
							x1 = a + c;
							y1 = b + ((y0 - b) / (x0 - a))*c;
							x2 = a - c;
							y2 = b - ((y0 - b) / (x0 - a))*c;
						}
						distance_1 = fabs(sqrt(((x1 - pre_x)*(x1 - pre_x)) + ((y1 - pre_y)*(y1 - pre_y))));//这里求的是两点之间的最短距离
						distance_2 = fabs(sqrt(((x2 - pre_x)*(x2 - pre_x)) + ((y2 - pre_y)*(y2 - pre_y))));//这里求的是两点之间的最短距离
						m_tempmindistant_ptp = distance_1;
						if (distance_2 < distance_1)
						{
							x1 = x2;
							y1 = y2;
							m_tempmindistant_ptp = distance_2;
						}
						m_temp_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;

						if (m_temp_mindistant < newNode->m_mindistant)
						{
							pTempGelenode->m_GeomStandData.GeoEle_start_x0 = x1;
							pTempGelenode->m_GeomStandData.GeoEle_start_y0 = y1;
							pTempGelenode->m_GeomStandData.GeoEle_start_x1 = x1;
							pTempGelenode->m_GeomStandData.GeoEle_start_y1 = y1;
							pTempGelenode->m_GeomStandData.m_circle.m_Circle_Start_Angle = m_geomele.ForCircleStartAngle(x1, y1, pTempGelenode->m_GeomStandData.m_circle);
							//
							newNode->m_mindistant = m_temp_mindistant;
							newNode->pminhead = pMinDistantCloseHead;
							newNode->pminnode = pTempGelenode;
						}
						else
						{
							;
						}
					}
					b;
					pMinDistantCloseHead = pMinDistantCloseHead->nextGeomcloseNode;

				}
				//以上已经找到基于现在这个被置位的封闭环的头结点的全局搜索
				//下面要做到是基于封闭环头结点和图元节点进行双向链表数据调整
				//先是把图元节点调整为封闭环的头结点
				GeomEleNode*phtemp, *ptemp;
				b++;
				phtemp = newNode->pminhead->FirstGeomele;
				if (newNode->pminnode == phtemp)//说明找了一圈发现还是最开始的结点距离最短
				{
					//其实这约等于说明都没干
					newNode->pminhead->FirstGeomele = phtemp;
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//把头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
				}
				else
				{
					ptemp = phtemp;
					while (ptemp->nextGeomeleNode)//找到最后一个结点
						ptemp = ptemp->nextGeomeleNode;
					ptemp->nextGeomeleNode = phtemp;//原来ptemp作为尾结点，它的后向指针是指向NULL的
					phtemp->prevGeomeleNode = ptemp;//原来phtemp作为头结点，它的前向指针是指向NULL的
					newNode->pminnode->prevGeomeleNode->nextGeomeleNode = NULL;//原来paimnode的前向指针是指向它的，但是从此就作为尾结点了
					newNode->pminnode->prevGeomeleNode = NULL;//从此作为头结点
					newNode->pminhead->FirstGeomele = newNode->pminnode;//作为上面的工作，它已经是这个封闭环里的新头结点了
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = newNode->pminnode->m_GeomStandData.GeoEle_start_x0;//把新头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = newNode->pminnode->m_GeomStandData.GeoEle_start_y0;
				}
				//以上是循环一遍之后找到的目前来说距离最短的一个封闭环头结点
				//接着要对整个封闭环双向链表的顺序进行调整
				//如果前面一个节点就是最后一个被置位的节点，那么直接挂上去
				if (newNode->pminhead->prevGeomcloseNode->m_KidAcceptGeomCloseHead)
				{
					//那么什么都不用动
					newNode->pminhead->m_KidAcceptGeomCloseHead = true;//从此该节点就被固定了

				}
				else//否则要寻找到最后被置位的节点，挂到其后面
				{
					pLastAcceptCHead = pNKidCloseHead;
					while (pLastAcceptCHead->m_KidAcceptGeomCloseHead)
						pLastAcceptCHead = pLastAcceptCHead->nextGeomcloseNode;
					//pLastAcceptCHead是没有被置位的
					if (!(newNode->pminhead->nextGeomcloseNode))//如果是尾节点
					{
						//既然已经是尾节点newNode->pminhead，那么直接把他前面一个值得后向节点指向NULL；
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = NULL;
					}
					else//一般节点
					{
						//newNode->pminhead原来前后指向关系的调节
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead->nextGeomcloseNode;
						newNode->pminhead->nextGeomcloseNode->prevGeomcloseNode = newNode->pminhead->prevGeomcloseNode;
					}
					//以上处理完了newNode->pminhead在原来位置的关系，接下来要处理他在被置位时候的关系了
					//主要pLastAcceptCHead并不是已经被置位的，它是第一个没被置位的，pLastAcceptCHead->prevGeomcloseNode才是最后一个被置位的
					newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
					newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
					pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
					pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
					newNode->pminhead->m_KidAcceptGeomCloseHead = true;//置位
				}
				//因为pNKidCloseHead一定是所在层的头结点，且已经被固定了，所以下面代码不需要
				
				
				////说明newNode->pminnode恰好是头结点，又是最靠近上一封闭环的那个封闭环
				//if (!(newNode->pminhead->prevGeomcloseNode))//如果是头结点，找了一遍发现还是头结点为最靠近的封闭环节节点
				//{
				//	pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode = newNode->pminhead;//还是以它为子封闭环的头节点
				//	newNode->pminhead->m_KidAcceptGeomCloseHead = true;//从此该节点就被固定了
				//}
				//newNode->pminhead的前面刚好是被置位的
				//但会存在一种情况，就是恰好pLastAcceptCHead=newNode->pminhead;如此便能解决这个问题
				// if (newNode->pminhead->prevGeomcloseNode->m_KidAcceptGeomCloseHead)
				//{
				//	//那么什么都不用动
				//	newNode->pminhead->m_KidAcceptGeomCloseHead = true;//从此该节点就被固定了

				//}
				//else//说明newNode->pminnode不是头结点了，那么这个节点要挂的目前最后一个m_AcceptGeomcloseHEAD没有被设置为true的封闭环节点后面
				//{

				//	//经过上面的寻找，现在pLastAcceptCHead是最后一个被置位的节点，而newNode->pminhead就是要挂到pLastAcceptCHead后面
				//	//本来是每次调整双向链表的节点位置时候，都要考虑该节点是不是头尾还是一般节点的，但这里可以不用考虑头结点了
				//	//那么要先找到最后一个没被置位的封闭环，不是头结点也不代表现在前面有了被置位的封闭环
				//	
				//	pLastAcceptCHead = pNKidCloseHead;
				//	while (pLastAcceptCHead->m_KidAcceptGeomCloseHead)
				//		pLastAcceptCHead = pLastAcceptCHead->nextGeomcloseNode;
				//	//pLastAcceptCHead是没有被置位的
				//	if (!(newNode->pminhead->nextGeomcloseNode))//如果是尾节点
				//	{
				//		//既然已经是尾节点newNode->pminhead，那么直接把他前面一个值得后向节点指向NULL；
				//		newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = NULL;
				//	}
				//	else//一般节点
				//	{
				//		//newNode->pminhead原来前后指向关系的调节
				//		newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead->nextGeomcloseNode;
				//		newNode->pminhead->nextGeomcloseNode->prevGeomcloseNode = newNode->pminhead->prevGeomcloseNode;
				//	}
				//	//以上处理完了newNode->pminhead在原来位置的关系，接下来要处理他在被置位时候的关系了
				//	//如果pLastAcceptCHead还是头节点，说明这一个封闭环都还没有被置位
				//	//要考虑的一点是当pLastAcceptCHead=newNode->pminhead的时候怎么办
				//	if (!(pLastAcceptCHead->prevGeomcloseNode))//那么现在情况是，离上一个末尾点最近的那个封闭环不是头结点，而且现在也没有封闭环被置位
				//	{
				//		//这个时候pLastAcceptCHead还是封闭环双向链表的头结点
				//		//应该把newNode->pminhead当成head指向的第一个封闭环节点
				//		pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode = newNode->pminhead;
				//		newNode->pminhead->prevGeomcloseNode = NULL;//首结点
				//		newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
				//		pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
				//		newNode->pminhead->m_KidAcceptGeomCloseHead = true;//置位
				//	}
				//	//上一种情况是newNode->pminhead不会与pLastAcceptCHead相等
				//	else//那么将newNode->pminhead直接挂到pLastAcceptCHead后面去
				//	{
				//		//主要pLastAcceptCHead并不是已经被置位的，它是第一个没被置位的，pLastAcceptCHead->prevGeomcloseNode才是最后一个被置位的
				//		newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
				//		newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
				//		pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
				//		pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
				//		newNode->pminhead->m_KidAcceptGeomCloseHead = true;//置位
				//	}
				//}
				pTempCHead = newNode->pminhead;//跳出这个循环
			}
			b;
			if (!(pTempCHead->nextGeomcloseNode))//说明这是最后一个子封闭环
			{
				pTheLastKidCHead = pTempCHead;
			}
			pTempCHead = pTempCHead->nextGeomcloseNode;
		}
	}
	else//如果没有其他封闭环，那么最后的封闭环节点还是其本身
	{
		pTheLastKidCHead = pNKidCloseHead;
	}
	

	return pTheLastKidCHead;
}
//寻找下一个第一层封闭环
GeomCloseHEAD*GeomForCut::Find_Next_TheFirstLevel(GeomCloseHEAD*pFirstClosedHead)
{
	Mindistant_EleNode*newNode = (Mindistant_EleNode*)malloc(sizeof(Mindistant_EleNode));
	newNode->m_mindistant = 99999.0;
	newNode->pminhead = NULL;
	newNode->pminnode = NULL;
	GeomCloseHEAD*pFirstl, *pHtemp, *pMinDistantCloseHead;
	GeomCloseHEAD*pLastAcceptCHead;//用寻找最后一个被置位的封闭环头结点
	GeomCloseHEAD*pTheLastKidCHead = NULL;//用寻找最后一个被置位的封闭环头结点

	double pre_x, pre_y, temp_x, temp_y;
	double m_temp_mindistant;//保存暂时的最短距离，如果是最终的最短距离，那么就该是给到	newNode->m_mindistant;
	double m_tempmindistant_origin;//到原点距离
	double m_tempmindistant_ptp;//到已经被置位的封闭环的距离
	double m_origin_x = 0.0;//机床原点
	double m_origin_y = 0.0;//机床原点
	GeomEleNode*pTempGelenode;
	if (pFirstClosedHead->nextGeomcloseNode)//如果有兄弟封闭环的话
	{
		pHtemp = pFirstClosedHead->nextGeomcloseNode;//从下一个开始
		pFirstl = pFirstClosedHead->nextGeomcloseNode;
		while (pFirstl)//遍历完一遍,将所有的第一层封闭环离开禁忌表
		{
			pFirstl->m_AcceptGeomcloseHEAD = false;
			pFirstl = pFirstl->nextGeomcloseNode;
		}
		//算法开始
		pre_x = pFirstClosedHead->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
		pre_y = pFirstClosedHead->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
		while (pHtemp)//将这个子封闭环全部遍历一遍,直到最后一个跳出循环
		{
			a++;
			while (!(pHtemp->m_AcceptGeomcloseHEAD))//只有当该封闭环还没有被规划的时候，进入才有意义
			{
				//pTempCHead能进来说明它是这个封闭环双向链表的第一个没有被置位的封闭环，
				//前面已经有一个封闭环被置位了	
				//现在要以该起止点来遍历所有的封闭环节点，找到其到上一个封闭环起止点最短距离的那个节点和封闭环
				//要对剩下的封闭环进行遍历
				temp_x = pHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
				temp_y = pHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				//到原点的距离
				m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//这里求的是两点之间的最短距离
				//两个封闭环之间的距离
				m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//这里求的是两点之间的最短距离
				//求最短距离,暂时的最短距离
				newNode->m_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
				//newNode->m_mindistant = 1 / newNode->m_mindistant;
				newNode->pminhead = pHtemp;
				newNode->pminnode = pHtemp->FirstGeomele;
				pMinDistantCloseHead = pHtemp;
				//把剩下的没有被置位的所有遍历一遍
				//以最短距离为准则从这里开始决定性下一个封闭环和头结点是什么
				while (pMinDistantCloseHead)
				{
					pTempGelenode = pMinDistantCloseHead->FirstGeomele;
					//注意要区分圆的封闭环
					if (3 != pTempGelenode->m_GeomStandData.m_typegeomele)//如果不是圆
					{
						while (pTempGelenode)//在这个封闭环里把节点全部遍历一遍
						{
							temp_x = pTempGelenode->m_GeomStandData.GeoEle_start_x0;
							temp_y = pTempGelenode->m_GeomStandData.GeoEle_start_y0;
							//到原点的距离
							m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//这里求的是两点之间的最短距离
							//两个封闭环之间的距离
							m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//这里求的是两点之间的最短距离
							m_temp_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
							//m_temp_mindistant = 1 / m_temp_mindistant;
							if (m_temp_mindistant < newNode->m_mindistant)
							{
								newNode->m_mindistant = m_temp_mindistant;
								newNode->pminhead = pMinDistantCloseHead;
								newNode->pminnode = pTempGelenode;
							}
							pTempGelenode = pTempGelenode->nextGeomeleNode;
						}
					}
					else//如果是圆的话
					{

						double a, b, r, x0, y0, c;
						double x1, y1, x2, y2;
						double distance_1, distance_2;
						a = pTempGelenode->m_GeomStandData.m_circle.m_Circent_x;
						b = pTempGelenode->m_GeomStandData.m_circle.m_Circent_y;
						r = pTempGelenode->m_GeomStandData.m_circle.m_Circle_r;
						x0 = pre_x;
						y0 = pre_y;
						if (x0 == a)
						{
							x1 = a;
							y1 = b - r;
							x2 = a;
							y2 = b + r;
						}
						else if (y0 == b)
						{
							x1 = a - r;
							y1 = b;
							x2 = a + r;
							y2 = b;
						}
						else
						{
							c = sqrt(pow(r, 2) / (1 + pow(((y0 - b) / (x0 - a)), 2)));
							x1 = a + c;
							y1 = b + ((y0 - b) / (x0 - a))*c;
							x2 = a - c;
							y2 = b - ((y0 - b) / (x0 - a))*c;
						}
						distance_1 = fabs(sqrt(((x1 - pre_x)*(x1 - pre_x)) + ((y1 - pre_y)*(y1 - pre_y))));//这里求的是两点之间的最短距离
						distance_2 = fabs(sqrt(((x2 - pre_x)*(x2 - pre_x)) + ((y2 - pre_y)*(y2 - pre_y))));//这里求的是两点之间的最短距离
						m_tempmindistant_ptp = distance_1;
						if (distance_2 < distance_1)
						{
							x1 = x2;
							y1 = y2;
							m_tempmindistant_ptp = distance_2;
						}
						m_temp_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
						//m_temp_mindistant = 1 / m_temp_mindistant;
						if (m_temp_mindistant < newNode->m_mindistant)
						{
							pTempGelenode->m_GeomStandData.GeoEle_start_x0 = x1;
							pTempGelenode->m_GeomStandData.GeoEle_start_y0 = y1;
							pTempGelenode->m_GeomStandData.GeoEle_start_x1 = x1;
							pTempGelenode->m_GeomStandData.GeoEle_start_y1 = y1;
							pTempGelenode->m_GeomStandData.m_circle.m_Circle_Start_Angle = m_geomele.ForCircleStartAngle(x1, y1, pTempGelenode->m_GeomStandData.m_circle);
							//
							newNode->m_mindistant = m_temp_mindistant;
							newNode->pminhead = pMinDistantCloseHead;
							newNode->pminnode = pTempGelenode;
						}
						else
						{
							;
						}
					}
					pMinDistantCloseHead = pMinDistantCloseHead->nextGeomcloseNode;

				}
				//以上已经找到基于现在这个被置位的封闭环的头结点的全局搜索
				//下面要做到是基于封闭环头结点和图元节点进行双向链表数据调整
				//先是把图元节点调整为封闭环的头结点
				GeomEleNode*phtemp, *ptemp;
				phtemp = newNode->pminhead->FirstGeomele;
				if (newNode->pminnode == phtemp)//说明找了一圈发现还是最开始的结点距离最短
				{
					//其实这约等于说明都没干
					newNode->pminhead->FirstGeomele = phtemp;
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//把头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
				}
				else
				{
					ptemp = phtemp;
					while (ptemp->nextGeomeleNode)//找到最后一个结点
						ptemp = ptemp->nextGeomeleNode;
					ptemp->nextGeomeleNode = phtemp;//原来ptemp作为尾结点，它的后向指针是指向NULL的
					phtemp->prevGeomeleNode = ptemp;//原来phtemp作为头结点，它的前向指针是指向NULL的
					newNode->pminnode->prevGeomeleNode->nextGeomeleNode = NULL;//原来paimnode的前向指针是指向它的，但是从此就作为尾结点了
					newNode->pminnode->prevGeomeleNode = NULL;//从此作为头结点
					newNode->pminhead->FirstGeomele = newNode->pminnode;//作为上面的工作，它已经是这个封闭环里的新头结点了
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = newNode->pminnode->m_GeomStandData.GeoEle_start_x0;//把新头结点的起点赋给封闭环头结点里保存，作为下一个封闭环判断的依据
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = newNode->pminnode->m_GeomStandData.GeoEle_start_y0;
				}
				//以上是循环一遍之后找到的目前来说距离最短的一个封闭环头结点
				//接着要对整个封闭环双向链表的顺序进行调整
				//如果前面一个节点就是最后一个被置位的节点，那么直接挂上去
				if (newNode->pminhead->prevGeomcloseNode->m_AcceptGeomcloseHEAD)
				{
					//那么什么都不用动
					newNode->pminhead->m_AcceptGeomcloseHEAD = true;//从此该节点就被固定了

				}
				else//否则要寻找到最后被置位的节点，挂到其后面
				{
					pLastAcceptCHead = pFirstClosedHead;
					while (pLastAcceptCHead->m_AcceptGeomcloseHEAD)
						pLastAcceptCHead = pLastAcceptCHead->nextGeomcloseNode;
					//pLastAcceptCHead是没有被置位的
					if (!(newNode->pminhead->nextGeomcloseNode))//如果是尾节点
					{
						//既然已经是尾节点newNode->pminhead，那么直接把他前面一个值得后向节点指向NULL；
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = NULL;
					}
					else//一般节点
					{
						//newNode->pminhead原来前后指向关系的调节
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead->nextGeomcloseNode;
						newNode->pminhead->nextGeomcloseNode->prevGeomcloseNode = newNode->pminhead->prevGeomcloseNode;
					}
					//以上处理完了newNode->pminhead在原来位置的关系，接下来要处理他在被置位时候的关系了
					//主要pLastAcceptCHead并不是已经被置位的，它是第一个没被置位的，pLastAcceptCHead->prevGeomcloseNode才是最后一个被置位的
					newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
					newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
					pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
					pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
					newNode->pminhead->m_AcceptGeomcloseHEAD = true;//置位
				}
				pHtemp = newNode->pminhead;//跳出这个循环
			}

			
				pTheLastKidCHead = pFirstClosedHead;
			
			break;//不用循环，只要找到下一个就好了
			//pHtemp = pHtemp->nextGeomcloseNode;
		}
	}


	else//如果没有其他封闭环，那么最后的封闭环节点还是其本身
	{
		pTheLastKidCHead = pFirstClosedHead;
	}


	return pTheLastKidCHead;
	
}
////////////////////////////////////添加切割引刀线//////////////////////////////////////////
////添加切割引刀线
//void GeomForCut::Add_CutGuideLine(NestResultDataNode*head)
//{
//
//}
//求解首图元和尾图元的直线的斜率，用此斜率来确定切割引刀线的斜率
double GeomForCut::CalculateSlope(GeomEleNode*pGnode)
{
	double slope;//斜率
	double x0, x1, y0, y1;//基本图元的四个控制点
	x0 = pGnode->m_GeomStandData.GeoEle_start_x0;
	y0 = pGnode->m_GeomStandData.GeoEle_start_y0;
	x1 = pGnode->m_GeomStandData.GeoEle_start_x1;
	y1 = pGnode->m_GeomStandData.GeoEle_start_y1;
	//判断是否为垂直直线
	if (x0 == x1)
	{
		slope = -987654;//给予一个固定的值，作为垂直直线的标识
	}
	//判断是否为水平直线
	else if (y1 == y0)
	{
		slope = 0.0;
	}
	//一般直线
	else
	{
		slope = (y1 - y0) / (x1 - x0);
	}
	return slope;
}
//生成切割引刀线
//要区分直线，圆，圆弧的不同
CutGuideLine* GeomForCut::CreatCutGuideLine(GeomCloseHEAD*Phead) //生成切割引刀线，输入切割控制点的起点，输入第一条基本图元的斜率，最后一条直线的斜率，输入是判断是内环还是外环。
{
	CutGuideLine*newNode = (CutGuideLine*)malloc(sizeof(CutGuideLine));
	//判断该封闭环是不是圆，如果是圆那么求切割引刀线的方式与其他的不一样
	GeomEleNode*First_node;//封闭环第一个基本图元
	bool m_bilayer;//判断是否双层
	m_bilayer = Phead->m_Singlelayer;
	First_node = Phead->FirstGeomele;
	if (First_node->m_GeomStandData.m_typegeomele == 3)//说明是圆
	{
		newNode = CreatCutGuideLine_Circle(First_node,m_bilayer);
	}
	else//说明是其他多边形的时候
	{
		newNode = CreatCutGuideLine_Polygon(First_node, m_bilayer);
	}
	return newNode;
}
//求圆的切割引刀线
CutGuideLine*GeomForCut::CreatCutGuideLine_Circle(GeomEleNode*Phead, bool m_bilayer)
{
	CutGuideLine*newNode = (CutGuideLine*)malloc(sizeof(CutGuideLine));

	return newNode;
}
//求多边形（非圆）的切割引刀线
CutGuideLine*GeomForCut::CreatCutGuideLine_Polygon(GeomEleNode*Phead, bool m_bilayer)
{
	//既然是多边形，那么肯定有不止一个基本图元
	CutGuideLine*newNode = (CutGuideLine*)malloc(sizeof(CutGuideLine));
	GeomEleNode*End_node;
	double x0, y0, x1, y1;
	double slope_1;//封闭环第一个基本图元的斜率
	double slope_2;//封闭环最后一个基本图元的斜率
	double a, b, r;//圆心，半径
	End_node = Phead->nextGeomeleNode;
	while (End_node->nextGeomeleNode)//找到最后一个基本图元
		End_node = End_node->nextGeomeleNode;	
	x1 = Phead->m_GeomStandData.GeoEle_start_x0;//以封闭环的第一个基本图元作为切割引刀线的末尾
	y1 = Phead->m_GeomStandData.GeoEle_start_y0;//以封闭环的第一个基本图元作为切割引刀线的末尾
	//要区分首尾节点中，是否有圆弧这种特殊的图元存在，
	//如果首图元是圆弧，那么切割引刀线最好是相切的方式（单层的时候，往外切）
	//（双层的时候，往里切），那么就是求弦的斜率
	if (Phead->m_GeomStandData.m_typegeomele==2||End_node->m_GeomStandData.m_typegeomele==2)//只要有一个是，都要分开处理
	{
		if (Phead->m_GeomStandData.m_typegeomele == 2)//首图元是圆弧
		{
			if (m_bilayer)//是双层，那么就该是往内切，求首，尾节点的斜率，与普通多边形一样求法，所以只求斜率，和后面一起处理
			{
				slope_1 = CalculateSlope(Phead);
				slope_2 = CalculateSlope(End_node);
			}
			else//单层，切割引刀线往外切，这个时候，应该是求圆弧的相切线
			{
				//求圆弧的切线
			

			}
		}
	}
	//newNode->x0 = x0;
	//newNode->y0 = y0;
	//newNode->x1 = x1;
	//newNode->y1 = y1;
	//newNode->if_CutGuideLine = true;//作为后续的是否切割引刀线判断的标准
	//newNode->nextGeomcloseNode = Phead;//切割引刀线后面就是封闭环的第一个图元节点
	//newNode->prevGeomcloseNode = NULL;//前面没有指向了，这里其实不要也应该可以的
	return newNode;
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//新的添加切割引刀线的方法//
//添加切割引刀线，上述代码已经把封闭环按照逻辑顺序挂靠完了。
void GeomForCut::Add_CutGuideLine(NestResultDataNode*head)
{
	//这里就要把所有的封闭环按照逻辑顺序读一遍，这是坑爹啊
	GeomCloseHEAD*ptemp;//封闭环首结点。
	bool m_HaveKidClosed;//判断是否有子封闭环，有子封闭环则需要由最里面的子封闭环开始挂靠

	ptemp = head->FirstGeomClose;
	while (ptemp)//将封闭环全部遍历，这里是对第一层封闭环进行遍历
	{
		//判断是否有子封闭环
		m_HaveKidClosed = IfIncludeKidClose(ptemp);
		if (m_HaveKidClosed)//如果有，那么调用专门为有子封闭环的切割引刀线添加函数
		{
			Add_KidCloseCutLine(ptemp);
		}
		else
		{
			CreatCutGuideLINE(ptemp);//添加引刀线
		}
		//如果没有子封闭环，那么就可以按照每个封闭环的外包络矩形来进行引刀线添加
		
		ptemp = ptemp->nextGeomcloseNode;
	}

}
//判断封闭环是否有子封闭环
bool GeomForCut::IfIncludeKidClose(GeomCloseHEAD*Phead)
{
	if (Phead->FirstInsideGCloseNode)//有子封闭环
	{
		return true;
	}
	else
	{
		return false;
	}
}
//核心代码，处理含有子封闭环的封闭环数据
//含有子封闭环，那么就可能存在一串的封闭环，
//因为子封闭环可能有兄弟封闭环，兄弟封闭环可能有子封闭环。
//在这个函数里，要把子封闭环的数据处理完，然后还要把封闭环本身的数据处理完
//自递归方程
void GeomForCut::Add_KidCloseCutLine(GeomCloseHEAD*Phead)//输入一个含有子封闭环的封闭环，给它添加切割引刀线
{
	GeomCloseHEAD*pktemp;//子封闭环结点。
	GeomCloseHEAD*pkNtemp;//子兄弟封闭环结点。
	GeomCloseHEAD*pFtemp;//上一层父封闭环结点。
	GeomCloseHEAD*pFNtemp;//上一层父兄弟封闭环结点。
	bool m_HaveKidClosed;//判断是否有子封闭环，有子封闭环则需要由最里面的子封闭环开始挂靠
	pktemp = Phead->FirstInsideGCloseNode;//子封闭环
	m_HaveKidClosed = IfIncludeKidClose(pktemp);
	if (m_HaveKidClosed)//如果有,则会一直判断，进入，直到最后一次没有子封闭环的封闭环
	{
		Add_KidCloseCutLine(pktemp);
	}
	else//没有子封闭环，那么就可以将该子封闭环添加切割引刀线。
	{
		CreatCutGuideLINE(pktemp);
	}
	//以上只是把含有子封闭环的第一层链表的封闭环的最底层封闭环处理完了
	//余下要处理最底层的兄弟封闭环
	//和上一级父封闭环
	//剩余封闭环处理开始
	//如果有兄弟封闭环,那么就要把该层的兄弟环处理完
	if (pktemp->nextGeomcloseNode)
	{
		pkNtemp = pktemp->nextGeomcloseNode;
		//遍历该层的兄弟封闭环
		while (pkNtemp)
		{
			//如果兄弟封闭环有子封闭环的情况下
			if (pkNtemp->FirstInsideGCloseNode)
			{
				Add_KidCloseCutLine(pkNtemp);
			}
			else//如果没有的情况下,直接处理
			{
				CreatCutGuideLINE(pkNtemp);
			}
			pkNtemp = pkNtemp->nextGeomcloseNode;
		}
	}
	//如果没有兄弟封闭环，或者兄弟封闭环已经处理完了
	//一定会有父封闭环的
	pFtemp = pktemp->prevOutsideGCloseNode;
	//重要代码
	while (pFtemp != Phead)//如果上一层父封闭环还不是第一层父封闭环的时候，那么就要继续往上处理
	{
		CreatCutGuideLINE(pFtemp);
		//判断是否有兄弟封闭环
		if (pFtemp->nextGeomcloseNode)
		{
			pFNtemp = pFtemp->nextGeomcloseNode;
			//把这一层兄弟封闭环处理完
			while (pFNtemp)
			{
				//兄弟封闭环是否有子封闭环
				if (pFNtemp->FirstInsideGCloseNode)
				{
					Add_KidCloseCutLine(pFNtemp);
				}
				else
				{
					CreatCutGuideLINE(pFNtemp);
				}
				pFNtemp = pFNtemp->nextGeomcloseNode;
			}

		}

		pFtemp = pFtemp->prevOutsideGCloseNode;

	}
	//以上已经把由第一层的那个封闭环带领的子封闭环除第一层子封闭环之外的封闭环全部处理完
		CreatCutGuideLINE(Phead);
}
//最核心的代码，这个代码将是要对每一个封闭环进行求切割引刀线
//要求判断该切割引导线不会与其他线有交点，也就是不会存在把切割引刀线开到要保留的材料中
//还要把切割引刀线加入到子封闭环中，并作为头结点
//如果不合适的话，还要对封闭环的切割控制点进行重新选择
CutGuideLine*GeomForCut::CreatCutGuideLINE(GeomCloseHEAD*Phead)//输入每一个封闭环，然后对其进行切割 引刀线的添加。
{
	CutGuideLine*A;
	return A;
}