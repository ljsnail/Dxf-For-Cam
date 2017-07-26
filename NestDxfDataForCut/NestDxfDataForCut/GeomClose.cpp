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
//��ʼ��һ���ڴ����������ͼԪ��׼�����ݸ�ʽGeomStandData
//��ײ��һ������
GeomEleNode* GeomClose::CreatGeomEleNode(GeomStandData m_geomstandData)
{
	GeomEleNode*newNode = (GeomEleNode*)malloc(sizeof(GeomEleNode));
	newNode->m_GeomStandData = m_geomstandData;//�����ݱ�������
	newNode->prevGeomeleNode = NULL;
	newNode->nextGeomeleNode = NULL;
	newNode->m_AccptGeomEleNode = false;
	newNode->m_NumGeomEleID=m_geomstandData.m_GeomEleID;//������������ͼ�ĵڼ�������ͼԪ
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
	newNode->m_AcceptGeomcloseHEAD = false;//���ʱ��û����¼
	newNode->m_IfGeomCloseIntact = false;//Ĭ��Ϊ��������
	newNode->m_KidAcceptGeomCloseHead = false;//Ĭ��Ϊû�ж�ȡ
	newNode->m_Singlelayer = true; //Ĭ���ǵ��㣬����ߵĿ������һ�㣬���������еĵ�һ�㿪ʼ����Ϊ����
	//newNode->m_GemoClosedLimt = { 0.0, 0.0, 0.0, 0.0 };//��ʼ����ֵ
	return newNode;
}
GeomCloseHEAD*GeomClose::InsertGeomEleNode(GeomCloseHEAD*head, GeomEleNode *node, GeomStandData m_geomstandData)
{
	GeomEleNode*pStart;//������ͬ
	GeomEleNode*pEnd;//ֹֹ��ͬ
	GeomEleNode*pEStrat;//�������ݵ��յ��ԭ�����ݵ��������ͬ��
	GeomEleNode*pSEnd;//�������ݵ�����ԭ�����ݵ��յ�����ͬ��
	GeomStandData NewData;//��������յ��λ��
	GeomEleNode*temp;//���ɽ��
	//����������ʱ��ֿձ���Ѿ������ݵı�
	//����ǿձ���ô��ֱ�Ӱ����ݹ���ȥ
	if (!(head->FirstGeomele))
	{
		head->FirstGeomele = node;
		head->m_geomclose_startpoint.colse_start_x0 = node->m_GeomStandData.GeoEle_start_x0;//��Ȼ�ǿձ���ô�������ͷ��㣬���ԾͰ�ͷ������ʼ����first����
		head->m_geomclose_startpoint.colse_start_y0 = node->m_GeomStandData.GeoEle_start_y0;
		head->m_NumGeomele++;//��ʾ��������һ��ͼԪ
		m_NumGeomClose++;//ÿ�¹�һ���ձ�˵����һ���µķ�ջ�
		head->FirstGeomele->m_NumGeomCloseID = m_NumGeomClose;
		return head;
	}
	/*������ǿձ���ôҪ�������ڵ����ݺ�ԭ��������֮��Ĺ�ϵ�������ݵ������ҵ�����㡣
	�ж��ǲ��������غϣ���ǰ�壬�������ôҪ�����е�����ǰ������������뵽ԭ�н���ǰ��
	��ǰ��*/
	pStart = FindRelatGmElNd_xyStart(head, m_geomstandData);//������ֻ�������غϵĹ�ϵ
	if (pStart)
	{
		if (!(pStart->prevGeomeleNode))//���pstart��ͷ���������,��Ҫ��head->first�ı�
		{
			NewData = DataSwap(m_geomstandData);
			node->m_GeomStandData = NewData;
			head->FirstGeomele = node;
			node->nextGeomeleNode = pStart;
			pStart->prevGeomeleNode = node;
			head->m_NumGeomele++;//��ʾ��������һ��ͼԪ
			node->m_NumGeomCloseID = pStart->m_NumGeomCloseID;//�����ص㣬��Ȼ���ڵ����ݸ�ԭ������������ͬ�Ĺ�ϵ����ô�͸���ͬ��һ���ջ�
			return head;
		}
		else//���pstart����ͷ���
		{
			NewData = DataSwap(m_geomstandData);
			node->m_GeomStandData = NewData;//����ԭ��������
			node->prevGeomeleNode = pStart->prevGeomeleNode;
			pStart->prevGeomeleNode->nextGeomeleNode = node;//��һ�в�������һ�е���˳��
			pStart->prevGeomeleNode = node;
			node->nextGeomeleNode = pStart;
			head->m_NumGeomele++;//��ʾ��������һ��ͼԪ
			node->m_NumGeomCloseID = pStart->m_NumGeomCloseID;//�����ص㣬��Ȼ���ڵ����ݸ�ԭ������������ͬ�Ĺ�ϵ����ô�͸���ͬ��һ���ջ�
			return head;
		}
	}
	//������ǿձ�
	//������ڵ����ݺ�ԭ�����������յ���յ��غϣ�
	//����壬������Ҫǰ��λ������Ҫע���ǲ���β���
	//�����
	pEnd = FindRelatGmElNd_xyEnd(head, m_geomstandData);
	if (pEnd)
	{
		if (!(pEnd->nextGeomeleNode))//��β���
		{
			NewData = DataSwap(m_geomstandData);
			node->m_GeomStandData = NewData;
			node->prevGeomeleNode = pEnd;
			pEnd->nextGeomeleNode = node;
			head->m_NumGeomele++;//��ʾ��������һ��ͼԪ
			node->m_NumGeomCloseID = pEnd->m_NumGeomCloseID;//�����ص㣬��Ȼ���ڵ����ݸ�ԭ������������ͬ�Ĺ�ϵ����ô�͸���ͬ��һ���ջ�
			return head;
		}
		else//����β���
		{
			NewData = DataSwap(m_geomstandData);
			node->m_GeomStandData = NewData;
			node->prevGeomeleNode = pEnd;
			node->nextGeomeleNode = pEnd->nextGeomeleNode;
			pEnd->nextGeomeleNode->prevGeomeleNode = node;
			pEnd->nextGeomeleNode = node;
			head->m_NumGeomele++;//��ʾ��������һ��ͼԪ
			node->m_NumGeomCloseID = pEnd->m_NumGeomCloseID;//�����ص㣬��Ȼ���ڵ����ݸ�ԭ������������ͬ�Ĺ�ϵ����ô�͸���ͬ��һ���ջ�
			return head;
		}
	}
	//�ж��ǲ����������ݵ��յ��ԭ�����ݵ��������ͬ�����
	//��ǰ�壬Ҫע��ͷ���
	pEStrat = FindRelatGmElNd_xyEStart(head,m_geomstandData);
	if (pEStrat)
	{
		if (!(pEStrat->prevGeomeleNode))
		{
			head->FirstGeomele = node;
			node->nextGeomeleNode = pEStrat;
			pEStrat->prevGeomeleNode = node;
			head->m_NumGeomele++;//��ʾ��������һ��ͼԪ
			node->m_NumGeomCloseID = pEStrat->m_NumGeomCloseID;//�����ص㣬��Ȼ���ڵ����ݸ�ԭ������������ͬ�Ĺ�ϵ����ô�͸���ͬ��һ���ջ�
			return head;
		}
		else//�������ͷ���
		{
			node->nextGeomeleNode = pEStrat;
			node->prevGeomeleNode = pEStrat->prevGeomeleNode;
			pEStrat->prevGeomeleNode->nextGeomeleNode = node;//�������в��ɵ���
			pEStrat->prevGeomeleNode = node;
			head->m_NumGeomele++;//��ʾ��������һ��ͼԪ
			node->m_NumGeomCloseID = pEStrat->m_NumGeomCloseID;//�����ص㣬��Ȼ���ڵ����ݸ�ԭ������������ͬ�Ĺ�ϵ����ô�͸���ͬ��һ���ջ�
			return head;
		}
	}
	//������ǿձ�
	//�ж��ǲ����������ݵ�����ԭ�����ݵ��յ���ͬ������壬��ͬ��������λ��
	//�����
	pSEnd = FindRelatGmElNd_xySEnd(head, m_geomstandData);
	if (pSEnd)
	{
		//�����Ķ�Ҫ�ж�β���
		if (!(pSEnd->nextGeomeleNode))
		{
			node->prevGeomeleNode = pSEnd;
			pSEnd->nextGeomeleNode = node;
			head->m_NumGeomele++;//��ʾ��������һ��ͼԪ
			node->m_NumGeomCloseID = pSEnd->m_NumGeomCloseID;//�����ص㣬��Ȼ���ڵ����ݸ�ԭ������������ͬ�Ĺ�ϵ����ô�͸���ͬ��һ���ջ�
			return head;
		}
		else
		{
			node->prevGeomeleNode = pSEnd;
			node->nextGeomeleNode = pSEnd->nextGeomeleNode;
			pSEnd->nextGeomeleNode->prevGeomeleNode = node;
			pSEnd->nextGeomeleNode = node;
			head->m_NumGeomele++;//��ʾ��������һ��ͼԪ
			node->m_NumGeomCloseID = pSEnd->m_NumGeomCloseID;//�����ص㣬��Ȼ���ڵ����ݸ�ԭ������������ͬ�Ĺ�ϵ����ô�͸���ͬ��һ���ջ�
			return head;
		}
	}
	//������ǿձ�������Ҳû��ʲô��ϵ����ô�͸�������һ�������ˡ�����ʱ������ͬһ�������ϣ�����Ӧ���ڷ�ջ���ID�ϼ�1.��
	else//��ô�Ͳ��ǿձ���ʱ�����ٰ���㸳ֵ��ȥ������Ҫ�ҵ�β������ȥ
	{
		temp = head->FirstGeomele;
		while (temp->nextGeomeleNode)
		{
			temp = temp->nextGeomeleNode;//����Ѱ�����һ�����
		}
		node->prevGeomeleNode = temp;
		temp->nextGeomeleNode = node;//ԭ�����ĺ�������NULL
		head->m_NumGeomele++;//��ԭ���Ļ����ϼ�1��ֻ�е��������ʱ�������node->m_GeomStandData.m_GeomCloseID����ȵġ�����һ���ж��������ʱ����Ͳ�һ���ˡ�
		//head->FirstGeomele->m_NumGeomCloseID;//�����Ӿ��Ǵ�ġ�
		m_NumGeomClose++;//��ôӦ������һ����ջ�,��ע������node�ϼӣ�������ͷ����ϼ�
		node->m_NumGeomCloseID = m_NumGeomClose;
		return head;
	}
}
//�ж��ǲ��������ͬ
GeomEleNode*GeomClose::FindRelatGmElNd_xyStart(GeomCloseHEAD*head, GeomStandData m_geomstandData)
{
	GeomEleNode*temp;//����������������
	temp = head->FirstGeomele;//��ʼ��Ϊͷ���
	while (temp)//�������������꣬�����temp->nextGeomeleNode,��ô�����һ����㴦��ͣ�ˣ�����������һ���Ƚ�
	{
		
		if ((fabs(temp->m_GeomStandData.GeoEle_start_x0 - m_geomstandData.GeoEle_start_x0)<EPSILON) && (fabs(temp->m_GeomStandData.GeoEle_start_y0 - m_geomstandData.GeoEle_start_y0)<EPSILON))//˵�������ͬ
			return temp;
		temp = temp->nextGeomeleNode;//����
	}
	return NULL;
}
//�ж��ǲ���ֹֹ��ͬ
GeomEleNode*GeomClose::FindRelatGmElNd_xyEnd(GeomCloseHEAD*head, GeomStandData m_geomstandData)
{
	GeomEleNode*temp;//����������������
	temp = head->FirstGeomele;//��ʼ��Ϊͷ���
	while (temp)
	{
		if ((fabs(temp->m_GeomStandData.GeoEle_start_x1 - m_geomstandData.GeoEle_start_x1)<EPSILON) && (fabs(temp->m_GeomStandData.GeoEle_start_y1 - m_geomstandData.GeoEle_start_y1)<EPSILON))//˵���յ���ͬ
				return temp;
		temp = temp->nextGeomeleNode;//����
	}
	return NULL;
}
//�ж��ǲ������ڵ�����ԭ�����յ���ͬ
GeomEleNode*GeomClose::FindRelatGmElNd_xySEnd(GeomCloseHEAD*head, GeomStandData m_geomstandData)
{
	GeomEleNode*temp;//����������������
	temp = head->FirstGeomele;//��ʼ��Ϊͷ���
	while (temp)
	{
		if ((fabs(temp->m_GeomStandData.GeoEle_start_x1 - m_geomstandData.GeoEle_start_x0)<EPSILON) && (fabs(temp->m_GeomStandData.GeoEle_start_y1 - m_geomstandData.GeoEle_start_y0)<EPSILON))//˵����������ԭ�����յ���ͬ
			return temp;
		temp = temp->nextGeomeleNode;//����
	}
	return NULL;
}
//�ж��ǲ����������ݵ���ֹ���ԭ�����ݵ������ͬ
GeomEleNode*GeomClose::FindRelatGmElNd_xyEStart(GeomCloseHEAD*head, GeomStandData m_geomstandData)
{
	GeomEleNode*temp;//����������������
	temp = head->FirstGeomele;//��ʼ��Ϊͷ���
	while (temp)
	{
		if ((fabs(temp->m_GeomStandData.GeoEle_start_x0 - m_geomstandData.GeoEle_start_x1)<EPSILON) && (fabs(temp->m_GeomStandData.GeoEle_start_y0 - m_geomstandData.GeoEle_start_y1)<EPSILON))//˵�����ڵ��յ��ԭ���������ͬ
			return temp;
		temp = temp->nextGeomeleNode;//����
	}
	return NULL;
}
//��������
GeomStandData GeomClose::DataSwap(GeomStandData m_geomstandData)
{
	GeomStandData m_tempData;
	m_tempData = m_geomstandData;//�൱�ڳ�ʼ���������е�ֵ��������
	//������Ҫ���������ݣ�������൱�ڶ��θ�ֵ
	m_tempData.GeoEle_start_x0 = m_geomstandData.GeoEle_start_x1;
	m_tempData.GeoEle_start_x1 = m_geomstandData.GeoEle_start_x0;
	m_tempData.GeoEle_start_y0 = m_geomstandData.GeoEle_start_y1;
	m_tempData.GeoEle_start_y1 = m_geomstandData.GeoEle_start_y0;
	//m_tempData.m_arc.m_ArcAngle_start = m_geomstandData.m_arc.m_ArcAngle_end;//����ARC���ԣ�����ת���ˣ���ô��ʼ�Ǻ���ֹ��Ҫ��ת�������и�Ĺ��̻�������һͷ��ʼ�С�
	//m_tempData.m_arc.m_ArcAngle_end = m_geomstandData.m_arc.m_ArcAngle_start;//�൱�ڶ��θ�ֵ
	m_tempData.m_line.x0 = m_geomstandData.m_line.x1;
	m_tempData.m_line.x1 = m_geomstandData.m_line.x0;
	m_tempData.m_line.y0 = m_geomstandData.m_line.y1;
	m_tempData.m_line.y1 = m_geomstandData.m_line.y0;
	m_tempData.m_IsTranData = true;
	//�������ñ���
	return m_tempData;
}
