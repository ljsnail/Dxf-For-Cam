#include "stdafx.h"
#include "GeomForCut.h"
#define EPSILON 0.018

GeomForCut::GeomForCut()
{
	//BatchHEAD m_batchHead = { 0, NULL };//�ṹ���ʼ��
	m_nestrsltNode = { NULL, NULL, NULL, false };//������Ϊÿ�δ洢ֵ��ȫ�ֱ�������ÿ��֮�󶼻ᱻ��д
}


GeomForCut::~GeomForCut()
{
}
//ʵ����BatchHEAD������ֵ��
BatchHEAD* GeomForCut::CreatBatchHEAD(int BatchHEAD_ID)//ʵ������ʱ��͸�ע�����ǵڼ����Σ�Ӧ��Ҫ��ȫ�ֱ���
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
	newNode->AcceptNestResultflag = false;//����û�б���¼
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
	//�����Ҫ�Ƿ�ձ�
	//����ձ���ôֱ�����Ϲ�
	if (!(head->FirstGeomClose))
	{
		head->FirstGeomClose = node;
		head->m_NumGeomClose++;//�����ķ�ջ�����һ��
		return head;
	}
	else//���ǿձ�������
	{
		temp = head->FirstGeomClose;
		while (temp->nextGeomcloseNode)//˵����β���
		{
			temp = temp->nextGeomcloseNode;
		}
		temp->nextGeomcloseNode = node;
		node->prevGeomcloseNode = temp;
		head->m_NumGeomClose++;
		return head;
	}
}
//��һ�������ж������������ͼ���뵽��������ָ���˫��������
BatchHEAD*GeomForCut::AddNestRsltDtNode(BatchHEAD*head, NestResultDataNode*node)
{
	NestResultDataNode*temp;
	//���Ƿ�Ϊ�ձ�
	//����ǿձ�
	if (!(head->FirstNestRsltNode))
	{
		head->FirstNestRsltNode = node;
		head->m_NumNestrsltDtNode++;
		return head;
	}
	else
	{
		temp = head->FirstNestRsltNode;
		while (temp->nextNestResultDataNode)//˵����β���
		{
			temp = temp->nextNestResultDataNode;
		}
		temp->nextNestResultDataNode = node;
		node->prevNestResultDataNode = temp;
		head->m_NumNestrsltDtNode++;
		return head;
	}
}
////����һ��˫���������������DXF����Ļ���ͼԪ���ݻ��ֳɲ�ͬ�ķ�ջ�,ÿ�ΰѲ���ͬ���ջ��Ļ���ͼԪǨ����һ�������С����ѭ�������������µķ�ջ�Fͷ������
//void GeomForCut::SepDiffGeomCls(GeomCloseHEAD*head)
//{
//	//���ֵ�ԭ����ͳһ���������к�һ��ֱ�ߵ���ֹ����ǰһ��ֱ�ߵ���ֹ��û���κε���ϵ��û����һ����������һ����ջ���㣬ͬʱ���ý��ӱ��������Ƴ����ҵ��µ�˫��������
//	GeomEleNode*temp;//�������������ʼ˫������
//	double m_temp_x0, m_temp_y0, m_temp_x1, m_temp_y1;//�洢ÿһ����ջ�����ʼ���
//	m_temp_x0 = head->FirstGeomele->m_GeomStandData.GeoEle_start_x0;//���Ѿ�ת�����󱣴���ͳһ���������������ֹ�������Ժ��ж�����
//	m_temp_y0 = head->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
//	m_temp_x1 = head->FirstGeomele->m_GeomStandData.GeoEle_start_x1;
//	m_temp_y1 = head->FirstGeomele->m_GeomStandData.GeoEle_start_y1;
//
//	temp = head->FirstGeomele->nextGeomeleNode;//����������˫�������еĵڶ�����㿪ʼ
//
//	//while (temp)//����һ�飬��β���Ž���
//	//{
//	//	//LINE ARC CIRCLE�����������У�CIRCLE����һ�������ķ�ջ�������Ҫ���ж���Щ���ʱ��һ�����ǲ���CIRCLE������ֱ���޳���ȥ
//	//	if (m_temp_x0 != temp->m_GeomStandData.GeoEle_start_x0) && (m_temp_x0 != temp->m_GeomStandData.GeoEle_start_x0)
//	//}
//}
//Ѱ�Ҳ���ͬһ����ջ��Ľ��,Ҫ����ͨ����
GeomEleNode*GeomForCut::FindDiffGeomCloseNode(NestResultDataNode*head)
{
	GeomCloseHEAD*Htemp=NULL;
	GeomEleNode*temp = NULL;
	int m_CloseID=-1;//��ʼ��
	if (!(head->FirstGeomClose->nextGeomcloseNode))//����������dxfͷ���ָ��ķ�ջ�˫��������ֻ��һ����ջ����
	{
		Htemp = head->FirstGeomClose;//��һ����ջ�˫��������
		temp = Htemp->FirstGeomele;//��һ�����ݵ��˫������
		m_CloseID = temp->m_NumGeomCloseID; //��һ�����ݵ�ķ�ջ�ID,���ID����������������ջ����ֵı�־
		while (temp)//ȫ��������
		{
			if (m_CloseID != temp->m_NumGeomCloseID)//�ҵ���һ������ͬһ����ջ�ID�Ľ��
			{
				temp->prevGeomeleNode->nextGeomeleNode = NULL;//��temp��ʼ�������еĽ�㶼�����ߣ�tempǰ��Ľ��Ӵ˺�����ˡ�����
				temp->prevGeomeleNode = NULL;//ͬ��temp�Ӵ�Ҳû����ͷ���ˡ�
				return temp;
			}
				
			temp = temp->nextGeomeleNode;
		}
		return NULL;//˵������ȫ������ͬһ����ջ���
	}
	else//���ͬһ������dxfͷ���ָ��ķ�ջ�˫�������в�ֹһ����ջ����
	{
		Htemp = head->FirstGeomClose;//��һ����ջ�˫��������
		while (Htemp->nextGeomcloseNode)//�ҵ����һ����ջ�˫��������
			Htemp = Htemp->nextGeomcloseNode;
		temp = Htemp->FirstGeomele;//���һ����ջ�ͷ�����ָ��ĵ�һ�����ݽ��
		while (temp)//ȫ��������
		{
			if (m_CloseID != temp->m_NumGeomCloseID)//�ҵ���һ������ͬһ����ջ�ID�Ľ��
			{
				temp->prevGeomeleNode->nextGeomeleNode = NULL;//��temp��ʼ�������еĽ�㶼�����ߣ�tempǰ��Ľ��Ӵ˺�����ˡ�����
				temp->prevGeomeleNode = NULL;//ͬ��temp�Ӵ�Ҳû����ͷ���ˡ�
				return temp;
			}
			temp = temp->nextGeomeleNode;
		}
		return NULL;//˵������ȫ������ͬһ����ջ���
	}
}
//���������ҵ��Ľ�����뵽����
void GeomForCut::InsertGeomCloseHEAD(NestResultDataNode*head, GeomEleNode*node)
{
	GeomCloseHEAD*Ftemp;//Ѱ�����һ�����
	GeomCloseHEAD*Fnode;
	//Ҫ�ж�node�ǲ���NULL
	if (node)//node����NULL,˵�����������
	{
		Fnode = m_geomclose.CreatGeomCloseHEAD(node->m_NumGeomCloseID);//�²���һ��Fͷ���
		Ftemp = head->FirstGeomClose;//FtempΪ��ջ�˫�������ͷ���
		while (Ftemp->nextGeomcloseNode)//���ҷ�ջ�˫�������β���
			Ftemp = Ftemp->nextGeomcloseNode;
		Ftemp->nextGeomcloseNode = Fnode;//���²����ķ�ջ�˫��������ҵ�β�����
		Fnode->prevGeomcloseNode = Ftemp;
		Fnode->GEOMCLOSE_ID = node->m_NumGeomCloseID;
		Fnode->FirstGeomele = node;//��node�ҵ��µķ�ջ�ͷ�����
		Fnode->m_geomclose_startpoint.colse_start_x0 = node->m_GeomStandData.GeoEle_start_x0;//��node�ϵĵ�һ����ҵ���ջ�����ϣ��������������ߵ�һ����
		Fnode->m_geomclose_startpoint.colse_start_y0 = node->m_GeomStandData.GeoEle_start_y0;//��node�ϵĵ�һ����ҵ���ջ�����ϣ��������������ߵ�һ����
		Fnode->m_AcceptGeomcloseHEAD = false;
	}	
}
//�жϷ�ջ��ڽ���Ƿ�����,��־�Ƿ�ջ���ɵ�����ͷ��������β�����յ��غ�
GeomCloseHEAD* GeomForCut::JudgeGeomCloseIntact(NestResultDataNode*head)
{//�������ֻ�ܱ�֤ÿ�ν���Ҫôȷ��һ��������ջ���Ҫô���Ƿ���һ���������ķ�ջ������ܱ�֤�Ƿ��Ѿ���ȫ�ж���ı�־
	GeomCloseHEAD*Htemp;
	GeomEleNode*temp;
	double x0, y0, x1, y1;
	Htemp = head->FirstGeomClose;//��һ����ջ����
	while (Htemp)//ÿ�ζ�������ȫ
	{
		if (!(Htemp->m_IfGeomCloseIntact))//�������ǻ�û����λΪ������ջ��ķ�ջ����
		{
			temp = Htemp->FirstGeomele;
			x0 = temp->m_GeomStandData.GeoEle_start_x0;
			y0 = temp->m_GeomStandData.GeoEle_start_y0;
			while (temp->nextGeomeleNode)//�ҵ����һ�����
				temp = temp->nextGeomeleNode;
			x1 = temp->m_GeomStandData.GeoEle_start_x1;
			y1 = temp->m_GeomStandData.GeoEle_start_y1;
			if ((fabs(x0 - x1) < EPSILON) && (fabs(y0 - y1) < EPSILON))//˵��������ͬ
				Htemp->m_IfGeomCloseIntact = true;//˵�������ջ��������ġ��������������жϵı�־
			else
				return Htemp;//������Ƿ������Ͳ�֪���ˣ�����Ҫ�ٴν���
		}
		else
		 Htemp = Htemp->nextGeomcloseNode;
	}
}
//�жϸ��ĸ���������������,�����Ҫ�ҵõ�������ͳ��ֲ���յ�ͼ���ˡ�
NestResultDataNode*GeomForCut::Find_Change_GeomCloseHEAD(NestResultDataNode*head, GeomCloseHEAD*node)
{
	double x0,y0, x1, y1;
	GeomCloseHEAD*Htemp;
	GeomEleNode*temp;
	temp = node->FirstGeomele;
	x0 = temp->m_GeomStandData.GeoEle_start_x0;
	y0 = temp->m_GeomStandData.GeoEle_start_y0;
	while (temp->nextGeomeleNode)//�ҵ����һ�����
		temp = temp->nextGeomeleNode;
	x1 = temp->m_GeomStandData.GeoEle_start_x1;
	y1 = temp->m_GeomStandData.GeoEle_start_y1;
	Htemp = head->FirstGeomClose;
	while (Htemp)
	{
		if (!(Htemp->m_IfGeomCloseIntact))//���û�б���λ��˵����������ǲ�������
		{

			if ((fabs(x1 - Htemp->FirstGeomele->m_GeomStandData.GeoEle_start_x0) < EPSILON) && (fabs(y1 - Htemp->FirstGeomele->m_GeomStandData.GeoEle_start_y0) < EPSILON))//˵��Htemp��node���,��ôHtemp��û����
			{
				temp->nextGeomeleNode = Htemp->FirstGeomele;//node��β���ָ��Htemp��ͷ���
				Htemp->FirstGeomele->prevGeomeleNode = temp;// Htemp��ͷ���ָ��node��β���,
				Htemp->prevGeomcloseNode->nextGeomcloseNode = Htemp->nextGeomcloseNode;//ҲҪ�ѷ�ջ�ͷ�����е���
				Htemp->nextGeomcloseNode->prevGeomcloseNode = Htemp->prevGeomcloseNode;
				free(Htemp);//ɾ�������㣬��������ĵ�������ǰ��Ҳ�Ѿ��������ˣ��Ѿ�û����
				while (temp->nextGeomeleNode)//�ҵ��¼ӽ��������ݽ������һ�����
					temp = temp->nextGeomeleNode;
				x1 = temp->m_GeomStandData.GeoEle_start_x1;
				y1 = temp->m_GeomStandData.GeoEle_start_y1;
				if ((fabs(x0 - x1) < EPSILON) && (fabs(y0 - y1) < EPSILON))//˵�������������ͬ
					node->m_IfGeomCloseIntact = true;//˵�������ջ��������ġ��������������жϵı�־
				return head;//���ݽ��ͷ�ջ�ͷ���������
			}
			//�������û������ȥ����ôֻ��˵��Htemp����������ջ��Ͳ������ķ�ջ�����һ������

			
		}
		else
			Htemp = Htemp->nextGeomcloseNode;
	}
	return head;//��ԭ���ķ��أ�˵����û���ҵ���������Եģ�����������������
}
