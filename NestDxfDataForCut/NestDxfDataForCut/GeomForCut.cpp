#include "stdafx.h"
#include "GeomForCut.h"
#define EPSILON 0.018
#define WEIGHT_ORIGIN 0.1//��ջ�������ԭ������Ȩ��
#define WEIGHT_PTP 0.9//��ջ�����һ����ջ�֮������Ȩ��
GeomForCut::GeomForCut()
{
	//BatchHEAD m_batchHead = { 0, NULL };//�ṹ���ʼ��
	i = 0;
	j = 0;
	a = 0;
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
	NestResultDataNode*temp=NULL;
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
	i++;
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
		m_CloseID = temp->m_NumGeomCloseID;//��Ҫ�ĳ�ʼ��
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
	j++;
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
			//������if��Ȼ������Ϊû��else�����������if���������Ͳ�֪����ô���ˡ�
			//���������Ƿ�ջ��Ľڵ��ǰ���ҵ��Ľڵ��ܹ���ԣ���ô�͸ðѷ�ջ��ϲ�
			while ((fabs(x1 - Htemp->FirstGeomele->m_GeomStandData.GeoEle_start_x0) < EPSILON) && (fabs(y1 - Htemp->FirstGeomele->m_GeomStandData.GeoEle_start_y0) < EPSILON))//˵��Htemp��node���,��ôHtemp��û����
			{
				temp->nextGeomeleNode = Htemp->FirstGeomele;//node��β���ָ��Htemp��ͷ���
				Htemp->FirstGeomele->prevGeomeleNode = temp;// Htemp��ͷ���ָ��node��β���,
				//�Է�ջ�ͷ�����е�����ʱ��Ҫ����Htemp��ͷ��㣬β��㻹���м���
				//ͷ���
				if (!(Htemp->prevGeomcloseNode))
				{
					Htemp->nextGeomcloseNode->prevGeomcloseNode = NULL;
					head->FirstGeomClose = Htemp->nextGeomcloseNode;//�Ӵ˵ڶ���������ͷ���
					head->m_NumGeomClose--;//��ջ���Ҫ����һ��
					free(Htemp);//ɾ�������㣬��������ĵ�������ǰ��Ҳ�Ѿ��������ˣ��Ѿ�û����
				}
				//β���
				else if (!(Htemp->nextGeomcloseNode))
				{
					Htemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
					head->m_NumGeomClose--;//��ջ���Ҫ����һ��
					free(Htemp);//ɾ�������㣬��������ĵ�������ǰ��Ҳ�Ѿ��������ˣ��Ѿ�û����
				}
				else if (Htemp->nextGeomcloseNode)//һ����
				{
					Htemp->prevGeomcloseNode->nextGeomcloseNode = Htemp->nextGeomcloseNode;//ҲҪ�ѷ�ջ�ͷ�����е���
					Htemp->nextGeomcloseNode->prevGeomcloseNode = Htemp->prevGeomcloseNode;//û�п��ǵ�Htemp��ͷ��㣬β�����м�������
					free(Htemp);//ɾ�������㣬��������ĵ�������ǰ��Ҳ�Ѿ��������ˣ��Ѿ�û����
				}
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
		//else//�����else����������
		Htemp = Htemp->nextGeomcloseNode;
	}
	node->m_IfGeomCloseIntact = true;//������һȦ�ˣ���û�У�ֻ��˵����ȷʵû�к��ĵģ��������������λ������һֱѭ�����ֵ
	return head;//��ԭ���ķ��أ�˵����û���ҵ���������Եģ�����������������
}
//��������dxfͷ��㣬�����ջ�ͷ����У����һ������λ�ķ�ջ�ͷ���
GeomCloseHEAD*GeomForCut::FindNotAcceptClosedNode(NestResultDataNode*head)
{
	GeomCloseHEAD*phtemp=NULL ,*phhtemp= NULL;
	phtemp = head->FirstGeomClose;
	//phhtemp = head->FirstGeomClose;
	//while (phhtemp->nextGeomcloseNode)//�ҵ����һ����ջ��ڵ�
	//{
	//	phhtemp = phhtemp->nextGeomcloseNode;
	//}
	if( !(phtemp->m_AcceptGeomcloseHEAD))//ͷ���û�б���λ����ô�϶�û�б���λ�ķ�ջ�ͷ�����
	{
		return NULL;
	}
	//else if (phhtemp->m_AcceptGeomcloseHEAD)//������һ����ջ��ڵ�Ҳ�Ǳ���λ�ˣ���ô˵�����˫�������Լ���������
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
//Ѱ���Ѿ���λ��û����λ�ķ�ջ��ڵ��е���̾�����ɽڵ�
//�������������ͷ��㣬��һ��û�б���λ�ķ�ջ�ͷ���
//�����ǰ��ջ�ͷ����£���ǰ�����һ������λ�ķ��ͷ����е���̾�����Ǹ�ͼԪ�ڵ�
Mindistant_EleNode* GeomForCut::FindMinDstGeomeleNode(NestResultDataNode*head, GeomCloseHEAD*pGMCLSHead)//����һ��δ��λ�ķ�ջ��ڵ�
{
	Mindistant_EleNode*m_mintempnode=NULL;
	GeomCloseHEAD*phtemp;//ǰ�����һ������λ�ķ��ͷ���
	GeomCloseHEAD*pMINhtemp;//ǰ�����һ������λ�ķ��ͷ���
	GeomEleNode* ptempminds;//������
	GeomEleNode* pmindst;//����ͷ���
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//��һ����ջ�����ֹ��
	double m_tempGeomCloseEnd_x, m_tempGeomCloseEnd_y;//��ǰ��ջ�����ʱ��ֹ��
	double m_mindstant;
	double m_tempmindstant;
	ptempminds = pGMCLSHead->FirstGeomele;
	pmindst = pGMCLSHead->FirstGeomele;
	//��������ջ�ͷ���˫������������һ������λ�ķ�ջ�ͷ���
	//phtemp = FindNotAcceptClosedNode(head);
	phtemp = head->FirstGeomClose;
	while (phtemp->m_AcceptGeomcloseHEAD)
		phtemp = phtemp->nextGeomcloseNode;
	if (!phtemp)//˵�������ջ��������е�ͷ��㶼����û�б���λ��
	{
		m_prevGeomCloseEnd_x = 0.0;//����ԭ��
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
	m_mindstant = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
	while (ptempminds)//ȫ��������
	{
		m_tempGeomCloseEnd_x = ptempminds->m_GeomStandData.GeoEle_start_x0;
		m_tempGeomCloseEnd_y = ptempminds->m_GeomStandData.GeoEle_start_y0;
		m_tempmindstant = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
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
//�Ե�����ջ��ڲ����нڵ�˳�����
void GeomForCut::ChangeEleNodeForAloneClse(GeomCloseHEAD*pHtemp, GeomEleNode*paimnode)
{
	
	GeomEleNode*phtemp,*ptemp;
	phtemp = pHtemp->FirstGeomele;
	if (paimnode == phtemp)//˵������һȦ���ֻ����ʼ�Ľ��������
	{
		//��ʵ��Լ����˵����û��
		pHtemp->FirstGeomele = phtemp;
		pHtemp->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//��ͷ������㸳����ջ�ͷ����ﱣ�棬��Ϊ��һ����ջ��жϵ�����
		pHtemp->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
	}
	else 
		{
		ptemp = phtemp;
		while (ptemp->nextGeomeleNode)//�ҵ����һ�����
			ptemp = ptemp->nextGeomeleNode;
		ptemp->nextGeomeleNode = phtemp;//ԭ��ptemp��Ϊβ��㣬���ĺ���ָ����ָ��NULL��
		phtemp->prevGeomeleNode = ptemp;//ԭ��phtemp��Ϊͷ��㣬����ǰ��ָ����ָ��NULL��
		paimnode->prevGeomeleNode->nextGeomeleNode = NULL;//ԭ��paimnode��ǰ��ָ����ָ�����ģ����ǴӴ˾���Ϊβ�����
		paimnode->prevGeomeleNode = NULL;//�Ӵ���Ϊͷ���
		pHtemp->FirstGeomele = paimnode;//��Ϊ����Ĺ��������Ѿ��������ջ������ͷ�����
		pHtemp->m_geomclose_startpoint.colse_start_x0 = paimnode->m_GeomStandData.GeoEle_start_x0;//����ͷ������㸳����ջ�ͷ����ﱣ�棬��Ϊ��һ����ջ��жϵ�����
		pHtemp->m_geomclose_startpoint.colse_start_y0 = paimnode->m_GeomStandData.GeoEle_start_y0;
		}
		
}
//���Ĵ��룬�����Ҷ�����
//���Ĵ��룬�����Ҷ�����
//���Ĵ��룬�����Ҷ�����
//�Ȱ���ÿ����ջ�������ԭ��֮�����̾���ȥѰ�ҷ�ջ��ڵ�ͷ���
//Ϊ���洦������ͼֽ��ջ�˳����»���
void GeomForCut::ChangeEleNodeOfGeomClosed_origin(NestResultDataNode*head)
{
	GeomCloseHEAD*pHtemp;
	GeomEleNode*phtemp, *paimnode = NULL, *ptemp;//��Ҫ������������
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//��һ����ջ�����ֹ��
	double temp_end_x, temp_end_y;//��ǰ��ջ����ݽ������
	double m_mindistant, m_tempmindistant;//��������֮����̾���

	pHtemp = head->FirstGeomClose;//��һ��ͷ���
	while (pHtemp)//���з�ջ�ͷ��㶼������
	{
		
		m_prevGeomCloseEnd_x = 0.0;//�Ի���ԭ��Ϊ�ʼ����㣬�ԣ�0,0��Ϊ����ԭ��
		m_prevGeomCloseEnd_y = 0.0;
	   //���ڵ�����ջ�����
		//Ѱ���������ݽ������������һ����ջ������е���ֹ��������Ǹ����
		phtemp = pHtemp->FirstGeomele;//����ͷ���
		ptemp = pHtemp->FirstGeomele;//���������Ľ��
		//����һ�����������һ����ջ���ֹ�����̾���
		//��ͷ���������һ����ջ���ֹ�����̾���Ϊ���Ȳο�
		temp_end_x = phtemp->m_GeomStandData.GeoEle_start_x0;//�����������
		temp_end_y = phtemp->m_GeomStandData.GeoEle_start_y0;
		m_mindistant = fabs(sqrt(((temp_end_x - m_prevGeomCloseEnd_x)*(temp_end_x - m_prevGeomCloseEnd_x)) + ((temp_end_y - m_prevGeomCloseEnd_y)*(temp_end_y - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
		while (ptemp)//�ѷ�ջ������е����ݽ�����һ��,�ҳ�����������ݽ��
		{
			if (!(ptemp->prevGeomeleNode))//���ptemp��ͷ�����ôĿ�������ͷ���
			{
				paimnode = phtemp;//���ͷ������ֹ����������ջ������һ����ջ���ʼ�����̾����ʱ��ͷ���ΪĿ����

			}
			else//�������ͷ�������ô��Ҫ�������ݱȽ���
			{
				//����һ�����������һ����ջ���ֹ�����̾���
				temp_end_x = ptemp->m_GeomStandData.GeoEle_start_x0;//�����������
				temp_end_y = ptemp->m_GeomStandData.GeoEle_start_y0;
				m_tempmindistant = fabs(sqrt(((temp_end_x - m_prevGeomCloseEnd_x)*(temp_end_x - m_prevGeomCloseEnd_x)) + ((temp_end_y - m_prevGeomCloseEnd_y)*(temp_end_y - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
				if (m_tempmindistant < m_mindistant)
				{
					m_mindistant = m_tempmindistant;//����СֵҲҪ����
					paimnode = ptemp;//˵��������������һ����ջ���ֹ��ľ������Щ

				}
			}
			ptemp = ptemp->nextGeomeleNode;
		}
		//���ˣ�һ����ջ���������������о�����̵��Ǹ�����Ѿ��ҵ���������Ҫ�Է�ջ��ڵĽ��������ݵ�����
		
		ChangeEleNodeForAloneClse(pHtemp, paimnode);
		pHtemp = pHtemp->nextGeomcloseNode;//��һ����ջ�������֮�󣬽���ڶ�����ջ������ѭ�������������dxf����ķ�ջ��������ꣻ����Բ
	}
}

//���Ĵ���
//���Ĵ���
//���Ĵ���
//��װ��ջ������ֹ�㵽����ԭ��ľ����ɽ���Զ
//���������������ͼ�ķ�ջ�����˳��
void GeomForCut::ChangClosedNodeOfNRDXF(NestResultDataNode*head)
{
	GeomCloseHEAD*pHtemp;//�����ջ�˫�������ͷ���
	GeomCloseHEAD*phnowtemp;//�����ջ�˫��������
	GeomCloseHEAD*phstarttemp=NULL;//��������ʱӦ����ͷ���
	GeomCloseHEAD*pntemp = NULL;//�������һ����ջ��ڵ�
	GeomCloseHEAD*ptemp = NULL;//��������ʱӦ����ͷ���
	GeomCloseHEAD*pptemp = NULL;//��������ʱӦ����ͷ���
	double m_origin_x = 0.0;//����ԭ��
	double m_origin_y = 0.0;//����ԭ��
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//��һ����ջ�����ֹ��
	double m_tempGeomCloseEnd_x, m_tempGeomCloseEnd_y;//��ǰ��ջ�����ֹ��
	double m_mindistant_origin;//�����ջ�˫�������е�һ��û�б���λ�Ľڵ㵽����ԭ�����̾��루��λ����ջ���ȷ������λ�ã�
	double m_tempmindistant_origin;//��ʱ�����ջ�˫��������û�б���λ�Ľڵ��е�����ԭ��ľ���
	double m_mindistant_ptp;//�����ջ�˫�����������һ���Ѿ�����λ�Ľڵ㵽��һ��û�б���λ�Ľڵ�֮��ľ���
	double m_tempmindistant_ptp;//��ʱ�����ջ�˫�����������һ���Ѿ�����λ�Ľڵ㵽��ջ���û�б���λ�Ľڵ�֮��ľ���
	double	m_allmindistant;//�����ջ�˫�����������һ���Ѿ�����λ�Ľڵ㵽û�б���λ�ڵ�֮��ľ����û�б���λ�ķ�ջ��ڵ㵽����ԭ��ľ���֮�͵���Сֵ
	double	m_tempallmindistant;//�����ջ�˫�����������һ���Ѿ�����λ�Ľڵ㵽û�б���λ�ڵ�֮��ľ����û�б���λ�ķ�ջ��ڵ㵽����ԭ��ľ���֮�͵�ֵ
	int m_GeomCloseID = 0;
	int m_TotalCloseID=0;//�����ܹ��ķ�ջ��㣬Բ�ķ�ջ����Ѿ��������ˡ�
	pHtemp = head->FirstGeomClose;//��һ����ջ�ͷ���
	pntemp = head->FirstGeomClose;
	while (pntemp)//�ҵ����һ����ջ��ڵ�
	{
		m_TotalCloseID++;
		pntemp = pntemp->nextGeomcloseNode;
	}
	
	//�ص���m_TotalCloseID������Ҫ��
	for (m_GeomCloseID = 1; m_GeomCloseID <=m_TotalCloseID; m_GeomCloseID++)//��ȫ��ѭ����ջ��ĸ�����
	{
		
		//�ҵ���һ��û�л�û�б��ձ��ͷ��㣬������ԭ��ľ���Ϊ��׼
		ptemp = head->FirstGeomClose;
		//ע��Ҫ���ó���һ�����⣬���ǵ�forѭ����û��������ʱ�������Ѿ�ȫ���Ǳ���λ���ˣ���ô������while���������ѭ��
		//while ((ptemp->m_AcceptGeomcloseHEAD)&&(ptemp!=NULL))//�ҵ���һ��û�б���λ�ķ�ջ��ڵ�,&&(ptemp!=NULL)���ƽ����������ѭ��������
		while (ptemp->m_AcceptGeomcloseHEAD)
		{
			ptemp = ptemp->nextGeomcloseNode;//���ִ���������һ������������
			if (!ptemp)//��ptempΪNULL��ʱ��ֱ����������ѭ��
			{
				break;
			}
			else
			{
				;
			}
		}
		if (ptemp)//��ptemp��ΪNULL��ʱ�򣬲�������
		{
			if (!(ptemp->prevGeomcloseNode))//���ptemp�Ƿ�ջ�ͷ�����û�б���λ����ô��ǰ��ڵ����ʼ��Ϊ����ԭ�㼴��0.0��0.0
			{
				m_prevGeomCloseEnd_x = 0.0;
				m_prevGeomCloseEnd_y = 0.0;
			}
			else//˵��ptemp ���Ƿ�ջ�ͷ��㣬��ô�͸���ǰ��ڵ�
			{
				m_prevGeomCloseEnd_x = ptemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_x0;
				m_prevGeomCloseEnd_y = ptemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_y0;
			}
			m_tempGeomCloseEnd_x = ptemp->m_geomclose_startpoint.colse_start_x0;
			m_tempGeomCloseEnd_y = ptemp->m_geomclose_startpoint.colse_start_y0;
			//��һ��û����λ�ķ�ջ�����ʼ�㵽����ԭ��ľ���
			m_mindistant_origin = fabs(sqrt(((m_tempGeomCloseEnd_x - m_origin_x)*(m_tempGeomCloseEnd_x - m_origin_x)) + ((m_tempGeomCloseEnd_y - m_origin_y)*(m_tempGeomCloseEnd_y - m_origin_y))));//�������������֮�����̾���
			m_mindistant_ptp = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
			m_allmindistant = m_mindistant_origin*WEIGHT_ORIGIN + m_mindistant_ptp*WEIGHT_PTP;//����̾���
			phstarttemp = ptemp;
			//��Ҫ�ҵ�������һ��û�б���λ�ķ�ջ��ڵ�
			phnowtemp = head->FirstGeomClose;
			while (phnowtemp)//ȫ������һ�飬�����ǰһ��ջ�����������Ǹ���ջ�
			{
				if (!(phnowtemp->prevGeomcloseNode))//˵����ͷ���
				{
					if (!(phnowtemp->m_AcceptGeomcloseHEAD))//˵�������ջ���û�б��滮�ã���δ����Ŀ���ǵõ�һ����ʱ����Сֵ
					{
						m_prevGeomCloseEnd_x = 0.0;
						m_prevGeomCloseEnd_y = 0.0;
						m_tempGeomCloseEnd_x = phnowtemp->m_geomclose_startpoint.colse_start_x0;
						m_tempGeomCloseEnd_y = phnowtemp->m_geomclose_startpoint.colse_start_y0;
						//��ԭ��ľ���
						m_tempmindistant_origin = fabs(sqrt(((m_tempGeomCloseEnd_x - m_origin_x)*(m_tempGeomCloseEnd_x - m_origin_x)) + ((m_tempGeomCloseEnd_y - m_origin_y)*(m_tempGeomCloseEnd_y - m_origin_y))));//�������������֮�����̾���
						//������ջ�֮��ľ���
						m_tempmindistant_ptp = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
						//����̾���
						m_tempallmindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
						if (m_tempallmindistant < m_allmindistant)//�����������ڵ���С��ҪС����ô��ǰӦ��������С
						{
							m_allmindistant = m_tempallmindistant;
							phstarttemp = phnowtemp;
						}
					}

				}
				else//˵������ͷ�����
				{
					if (!(phnowtemp->m_AcceptGeomcloseHEAD))//˵�������ջ���û�б��滮��
					{
						//�ҵ���һ��û�л�û�б��ձ��ͷ��㣬������ԭ��ľ���Ϊ��׼
						pptemp = head->FirstGeomClose;
						while (pptemp->m_AcceptGeomcloseHEAD)//�ҵ���һ��û�б���λ�ķ�ջ��ڵ�
							pptemp = pptemp->nextGeomcloseNode;
						if (!(pptemp->prevGeomcloseNode))//���ptemp�Ƿ�ջ�ͷ�����û�б���λ����ô��ǰ��ڵ����ʼ��Ϊ����ԭ�㼴��0.0��0.0
						{
							m_prevGeomCloseEnd_x = 0.0;
							m_prevGeomCloseEnd_y = 0.0;
						}
						else//˵��ptemp ���Ƿ�ջ�ͷ��㣬��ô�͸���ǰ��ڵ�
						{
							m_prevGeomCloseEnd_x = pptemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_x0;
							m_prevGeomCloseEnd_y = pptemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_y0;
						}
						m_tempGeomCloseEnd_x = phnowtemp->m_geomclose_startpoint.colse_start_x0;
						m_tempGeomCloseEnd_y = phnowtemp->m_geomclose_startpoint.colse_start_y0;
						//��ԭ��ľ���
						m_tempmindistant_origin = fabs(sqrt(((m_tempGeomCloseEnd_x - m_origin_x)*(m_tempGeomCloseEnd_x - m_origin_x)) + ((m_tempGeomCloseEnd_y - m_origin_y)*(m_tempGeomCloseEnd_y - m_origin_y))));//�������������֮�����̾���
						//������ջ�֮��ľ���
						m_tempmindistant_ptp = fabs(sqrt(((m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)*(m_tempGeomCloseEnd_x - m_prevGeomCloseEnd_x)) + ((m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y)*(m_tempGeomCloseEnd_y - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
						//����̾���
						m_tempallmindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
						if (m_tempallmindistant < m_allmindistant)//�����������ڵ���С��ҪС����ô��ǰӦ��������С
						{
							m_allmindistant = m_tempallmindistant;
							phstarttemp = phnowtemp;
						}
					}
				}

				phnowtemp = phnowtemp->nextGeomcloseNode;
			}
			//������ѭ��һ��֮���ҵ���Ŀǰ��˵������̵�һ����ջ�ͷ���
			if (!(phstarttemp->prevGeomcloseNode))//�����ͷ��㣬����һ�鷢�ֻ���ͷ���Ϊ����ķ�ջ��ڽڵ�
			{
				head->FirstGeomClose = phstarttemp;//������Ϊͷ���
				phstarttemp->m_AcceptGeomcloseHEAD = true;//�Ӵ˸ýڵ�ͱ��̶���
			}
			else//˵������ͷ����ˣ���ô����ڵ�Ҫ�ҵ�Ŀǰ���һ��m_AcceptGeomcloseHEADû�б�����Ϊtrue�ķ�ջ��ڵ����
			{
				//��Ҫ�ҵ�������һ��û�б���λ�ķ�ջ��ڵ�
				phnowtemp = head->FirstGeomClose;
				while (phnowtemp->nextGeomcloseNode->m_AcceptGeomcloseHEAD)//��phnowtemp����һ����ջ�ͷ��㲻����true��ʱ����ôphnowtemp��Ӧ�������һ��m_AcceptGeomcloseHEAD��true�ģ�Ҳ�����Ѿ���������˳���
					phnowtemp = phnowtemp->nextGeomcloseNode;
				//���������Ѱ�ң�����phnowtemp�����һ������λ�Ľڵ㣬��phstarttemp����Ҫ�ҵ�phnowtemp����
				//������ÿ�ε���˫������Ľڵ�λ��ʱ�򣬶�Ҫ���Ǹýڵ��ǲ���ͷβ����һ��ڵ�ģ���������Բ��ÿ���ͷ�����
				if (!(phstarttemp->nextGeomcloseNode))//�����β�ڵ�
				{
					//�����ж�phstarttemp��ǰһ���ǲ��ǵ����ڶ������������ֱ�ӹ���ȥ����Ҫ�䣬������ѭ��
					if (phnowtemp->nextGeomcloseNode == phstarttemp)//˵�����һ���Ѿ���λ�ķ�ջ�ͷ���ĺ�һ���ڵ㱾�����phstarttemp
					{
						phstarttemp->m_AcceptGeomcloseHEAD = true;//û��λ�ø����ˣ�ֱ����λ
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
				else//һ��ڵ�
				{
					phstarttemp->prevGeomcloseNode->nextGeomcloseNode = phstarttemp->nextGeomcloseNode;//��ԭ��ǰ��ĺ��汾�������ģ��������ˣ���Ȼ�ͱ������������Ǹ�
					phstarttemp->nextGeomcloseNode->prevGeomcloseNode = phstarttemp->prevGeomcloseNode;//��ԭ�������ǰ�汾�������ģ��������ˣ���Ȼ�ͱ������ǰ����Ǹ�
					phnowtemp->nextGeomcloseNode->prevGeomcloseNode = phstarttemp;
					phstarttemp->nextGeomcloseNode = phnowtemp->nextGeomcloseNode;
					phnowtemp->nextGeomcloseNode = phstarttemp;
					phstarttemp->prevGeomcloseNode = phnowtemp;
					phstarttemp->m_AcceptGeomcloseHEAD = true;
				}
			}
		}
		else//��ptempΪNULL��ʱ��ɶҲ���ɣ��ͺ���for�ͺ���
		{
			;
		}
		
	}
}


//���Ĵ��룬�����Ҷ�����
//���Ĵ��룬�����Ҷ�����
//���Ĵ��룬�����Ҷ�����
//�����Ѿ�ȷ����˳���ջ�˳��ȥ����ÿһ����ջ���������ݣ�Ѱ���µ�ͷ���
void GeomForCut::ChangeEleNodeOfGeomClosed_order(NestResultDataNode*head)
{
	GeomCloseHEAD*pHtemp;
	GeomEleNode*phtemp, *paimnode = NULL, *ptemp;//��Ҫ������������
	double m_prevGeomCloseEnd_x, m_prevGeomCloseEnd_y;//��һ����ջ�����ֹ��
	double temp_end_x, temp_end_y;//��ǰ��ջ����ݽ������
	double m_mindistant, m_tempmindistant;//��������֮����̾���

	pHtemp = head->FirstGeomClose;//��һ��ͷ���
	while (pHtemp->nextGeomcloseNode)//���з�ջ�ͷ��㶼������
	{
		if (!(pHtemp->prevGeomcloseNode))//������ǵ�һ����ջ�
		{
			//˵��Htempû��ǰһ����ջ����ѻ���ԭ�㸳����
			m_prevGeomCloseEnd_x = 0.0;//�Ի���ԭ��Ϊ�ʼ����㣬�ԣ�0,0��Ϊ����ԭ��
			m_prevGeomCloseEnd_y = 0.0;
		}
		else//˵������ͷ��㣬��ô����ǰ��ջ�����ˣ���ô��ǰһ����ջ�ͷ��㱣�����ֹ�����ݴ�����
		{
			m_prevGeomCloseEnd_x = pHtemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_x0;
			m_prevGeomCloseEnd_y = pHtemp->prevGeomcloseNode->m_geomclose_startpoint.colse_start_y0;

		}

		//���ڵ�����ջ�����
		//Ѱ���������ݽ������������һ����ջ������е���ֹ��������Ǹ����
		phtemp = pHtemp->FirstGeomele;//����ͷ���
		ptemp = pHtemp->FirstGeomele;//���������Ľ��
		//����һ�����������һ����ջ���ֹ�����̾���
		//��ͷ���������һ����ջ���ֹ�����̾���Ϊ���Ȳο�
		temp_end_x = phtemp->m_GeomStandData.GeoEle_start_x0;//�����������
		temp_end_y = phtemp->m_GeomStandData.GeoEle_start_y0;
		m_mindistant = fabs(sqrt(((temp_end_x - m_prevGeomCloseEnd_x)*(temp_end_x - m_prevGeomCloseEnd_x)) + ((temp_end_y - m_prevGeomCloseEnd_y)*(temp_end_y - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
		while (ptemp)//�ѷ�ջ������е����ݽ�����һ��,�ҳ�����������ݽ��
		{
			if (!(ptemp->prevGeomeleNode))//���ptemp��ͷ�����ôĿ�������ͷ���
			{
				//����Բ��˵������ջ�ֻ��һ����㣬��Ϊͷ��㣬��ô����Ҫ����ǰ���ջ�����ֹ������ڵ�Բ�ĵ�ֱ�ߺ�Բ�󽻵㡣
				if (3==ptemp->m_GeomStandData.m_typegeomele)//Բ
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
					distance_1 = fabs(sqrt(((x1 - m_prevGeomCloseEnd_x)*(x1 - m_prevGeomCloseEnd_x)) + ((y1 - m_prevGeomCloseEnd_y)*(y1 - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
					distance_2 = fabs(sqrt(((x2 - m_prevGeomCloseEnd_x)*(x2 - m_prevGeomCloseEnd_x)) + ((y2 - m_prevGeomCloseEnd_y)*(y2 - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
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
						paimnode = ptemp;//˵��������������һ����ջ���ֹ��ľ������Щ

					}
				}
				else
				{
					paimnode = phtemp;//���ͷ������ֹ����������ջ������һ����ջ���ʼ�����̾����ʱ��ͷ���ΪĿ����

				}

			}
			else//�������ͷ�������ô��Ҫ�������ݱȽ���
			{
				//����һ�����������һ����ջ���ֹ�����̾���
				temp_end_x = ptemp->m_GeomStandData.GeoEle_start_x0;//�����������
				temp_end_y = ptemp->m_GeomStandData.GeoEle_start_y0;
				m_tempmindistant = fabs(sqrt(((temp_end_x - m_prevGeomCloseEnd_x)*(temp_end_x - m_prevGeomCloseEnd_x)) + ((temp_end_y - m_prevGeomCloseEnd_y)*(temp_end_y - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
				if (m_tempmindistant < m_mindistant)
				{
					m_mindistant = m_tempmindistant;//����СֵҲҪ����
					paimnode = ptemp;//˵��������������һ����ջ���ֹ��ľ������Щ

				}
			}
			ptemp = ptemp->nextGeomeleNode;
		}
		//���ˣ�һ����ջ���������������о�����̵��Ǹ�����Ѿ��ҵ���������Ҫ�Է�ջ��ڵĽ��������ݵ�����
		if (paimnode->prevGeomeleNode)//����NULL����ô����˵���Ǵ�Բ�Ǳ߻�õ�����
		{
			ChangeEleNodeForAloneClse(pHtemp, paimnode);
		}
		pHtemp = pHtemp->nextGeomcloseNode;//��һ����ջ�������֮�󣬽���ڶ�����ջ������ѭ�������������dxf����ķ�ջ��������ꣻ����Բ

	}
}

//����һ��������������һ���Ѿ�����λ�ķ�ջ��������ͼԪ������̾���
Mindistant_EleNode*GeomForCut::CreatMindistant_EleNode(GeomEleNode* pmindst, double m_mindstant)
{
	Mindistant_EleNode*newNode = (Mindistant_EleNode*)malloc(sizeof(Mindistant_EleNode));
	newNode->m_mindistant = m_mindstant;
	newNode->pminnode = pmindst;
	return newNode;
}
//���Ĵ��룡
//���Ĵ��룡
//���Ĵ��룡
//���ڽ��ɡ���̰���㷨��·���滮���Ż��㷨

void GeomForCut::BaseTS_GR_ForCutPathPlan(NestResultDataNode*head)
{
	Mindistant_EleNode*newNode = (Mindistant_EleNode*)malloc(sizeof(Mindistant_EleNode));
	newNode->m_mindistant = 99999.0;
	newNode->pminhead = NULL;
	newNode->pminnode = NULL;
	GeomCloseHEAD*pTempCHead, *ppTempCHead;//��������ȫ����ջ�
	GeomCloseHEAD*pLastAcceptCHead;//��Ѱ�����һ������λ�ķ�ջ�ͷ���
	GeomCloseHEAD*pTheLastAcceptCHead;//��Ѱ�����һ������λ�ķ�ջ�ͷ���

	GeomCloseHEAD*pMinDistantCloseHead, *pntemp;//��Ѱ�����һ������λ�ķ�ջ�ͷ���
	GeomCloseHEAD*pSencond2FirstCloseHead;//����һ�����ȥ�����ӵڶ�����ջ���ʼ

	GeomEleNode*pTempGelenode;
	double pre_x, pre_y, temp_x, temp_y;
	double m_temp_mindistant;//������ʱ����̾��룬��������յ���̾��룬��ô�͸��Ǹ���	newNode->m_mindistant;
	double m_tempmindistant_origin;//��ԭ�����
	double m_tempmindistant_ptp;//���Ѿ�����λ�ķ�ջ��ľ���
	double m_origin_x = 0.0;//����ԭ��
	double m_origin_y = 0.0;//����ԭ��
	int a = 0, b = 0, m_TotalCloseID, m_GeomCloseID;
	//�㷨��ʼ
	//pSencond2FirstCloseHead = head->FirstGeomClose;
	//pntemp = pSencond2FirstCloseHead->nextGeomcloseNode;//��һ����ջ���800*800�Ĵ�ש����Ӧ����
	//head->FirstGeomClose = pntemp;
	//���ڶ�����ջ������һ����ջ�
	head = ChangeSencondCH2FH(head);
	pLastAcceptCHead = head->FirstGeomClose;
	pntemp = head->FirstGeomClose;
	m_TotalCloseID = 0;
	while (pntemp)//�ҵ����һ����ջ��ڵ�
	{
		m_TotalCloseID++;
		pntemp = pntemp->nextGeomcloseNode;
	}

	//�ص���m_TotalCloseID������Ҫ��,��֤ÿһ����һ����ջ�����λ
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_TotalCloseID; m_GeomCloseID++)//��ȫ��ѭ����ջ��ĸ�����
	{


		pTempCHead = head->FirstGeomClose;
		while (pTempCHead)//������������ͼ����ķ�ջ�ȫ������һ��,ֱ�����һ������ѭ��
		{
			a++;
			/*pTempCHead = head->FirstGeomClose;*/
			while (!(pTempCHead->m_AcceptGeomcloseHEAD))//ֻ�е��÷�ջ���û�б��滮��ʱ�򣬽����������
			{
				//pTempCHead�ܽ���˵�����ǵ�һ��û�б���λ�ķ�ջ�����ô�����ô�����ʼ
				//���TempCHead�ǵ�һ����ջ��ڵ㣬��ô��ǰ�沢û���Ѿ�����λ�ķ�ջ��������Թ���ԭ����Ϊ��һ����ջ�����ֹ��

				if (NULL == pTempCHead->prevGeomcloseNode)//��ͷ���
				{
					pre_x = 0.0;
					pre_y = 0.0;
				}
				else//��ôҪ�ҵ����һ������λ�ķ�ջ�ͷ���
				{
					/*pLastAcceptCHead = head->FirstGeomClose;
					while (pLastAcceptCHead->nextGeomcloseNode->m_AcceptGeomcloseHEAD)
						pLastAcceptCHead = pLastAcceptCHead->nextGeomcloseNode;*/

					//pre_x = pLastAcceptCHead->m_geomclose_startpoint.colse_start_x0;
					//pre_y = pLastAcceptCHead->m_geomclose_startpoint.colse_start_y0;
					pre_x = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
					pre_y = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				}
				//�����Ѿ��ҵ���һ����յ���ֹ���ˡ�
				//����Ҫ�Ը���ֹ�����������еķ�ջ��ڵ㣬�ҵ��䵽��һ����ջ���ֹ����̾�����Ǹ��ڵ�ͷ�ջ�
				//Ҫ��ʣ�µķ�ջ����б���
				//temp_x = pTempCHead->m_geomclose_startpoint.colse_start_x0;
				//temp_y = pTempCHead->m_geomclose_startpoint.colse_start_y0;//����m_geomclose_startpoint�Ѿ�û�����ˡ�
				temp_x = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
				temp_y = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				//��ԭ��ľ���
				m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//�������������֮�����̾���
				//������ջ�֮��ľ���
				m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//�������������֮�����̾���
				//����̾���,��ʱ����̾���
				newNode->m_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
				newNode->pminhead = pTempCHead;
				newNode->pminnode = pTempCHead->FirstGeomele;
				pMinDistantCloseHead = pTempCHead;
				//��ʣ�µ�û�б���λ�����б���һ��
				//����̾���Ϊ׼������￪ʼ��������һ����ջ���ͷ�����ʲô
				b = 0;
				while (pMinDistantCloseHead)
				{
					b++;
					pTempGelenode = pMinDistantCloseHead->FirstGeomele;
					//ע��Ҫ����Բ�ķ�ջ�
					if (3 != pTempGelenode->m_GeomStandData.m_typegeomele)//�������Բ
					{
						while (pTempGelenode)//�������ջ���ѽڵ�ȫ������һ��
						{
							temp_x = pTempGelenode->m_GeomStandData.GeoEle_start_x0;
							temp_y = pTempGelenode->m_GeomStandData.GeoEle_start_y0;
							//��ԭ��ľ���
							m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//�������������֮�����̾���
							//������ջ�֮��ľ���
							m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//�������������֮�����̾���
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
					else//�����Բ�Ļ�
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
						distance_1 = fabs(sqrt(((x1 - pre_x)*(x1 - pre_x)) + ((y1 - pre_y)*(y1 - pre_y))));//�������������֮�����̾���
						distance_2 = fabs(sqrt(((x2 - pre_x)*(x2 - pre_x)) + ((y2 - pre_y)*(y2 - pre_y))));//�������������֮�����̾���
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
				//�����Ѿ��ҵ����������������λ�ķ�ջ���ͷ����ȫ������
				//����Ҫ�����ǻ��ڷ�ջ�ͷ����ͼԪ�ڵ����˫���������ݵ���
				//���ǰ�ͼԪ�ڵ����Ϊ��ջ���ͷ���
				GeomEleNode*phtemp, *ptemp;
				//if (!(pTempCHead->prevGeomcloseNode))//���pTempCHead�ǵ�һ�����ͷ���������
				//{
				//	pLastAcceptCHead = pTempCHead;
				//}
				//else
				//{
				//	pLastAcceptCHead = pTempCHead->prevGeomcloseNode;
				//}
				phtemp = newNode->pminhead->FirstGeomele;
				if (newNode->pminnode == phtemp)//˵������һȦ���ֻ����ʼ�Ľ��������
				{
					//��ʵ��Լ����˵����û��
					newNode->pminhead->FirstGeomele = phtemp;
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//��ͷ������㸳����ջ�ͷ����ﱣ�棬��Ϊ��һ����ջ��жϵ�����
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
				}
				else
				{
					ptemp = phtemp;
					while (ptemp->nextGeomeleNode)//�ҵ����һ�����
						ptemp = ptemp->nextGeomeleNode;
					ptemp->nextGeomeleNode = phtemp;//ԭ��ptemp��Ϊβ��㣬���ĺ���ָ����ָ��NULL��
					phtemp->prevGeomeleNode = ptemp;//ԭ��phtemp��Ϊͷ��㣬����ǰ��ָ����ָ��NULL��
					newNode->pminnode->prevGeomeleNode->nextGeomeleNode = NULL;//ԭ��paimnode��ǰ��ָ����ָ�����ģ����ǴӴ˾���Ϊβ�����
					newNode->pminnode->prevGeomeleNode = NULL;//�Ӵ���Ϊͷ���
					newNode->pminhead->FirstGeomele = newNode->pminnode;//��Ϊ����Ĺ��������Ѿ��������ջ������ͷ�����
					newNode->pminhead->m_geomclose_startpoint.colse_start_x0 = newNode->pminnode->m_GeomStandData.GeoEle_start_x0;//����ͷ������㸳����ջ�ͷ����ﱣ�棬��Ϊ��һ����ջ��жϵ�����
					newNode->pminhead->m_geomclose_startpoint.colse_start_y0 = newNode->pminnode->m_GeomStandData.GeoEle_start_y0;
				}
				//������ѭ��һ��֮���ҵ���Ŀǰ��˵������̵�һ����ջ�ͷ���
				//����Ҫ��������ջ�˫�������˳����е���
				//˵��newNode->pminnodeǡ����ͷ��㣬���������һ��ջ����Ǹ���ջ�
				if (!(newNode->pminhead->prevGeomcloseNode))//�����ͷ��㣬����һ�鷢�ֻ���ͷ���Ϊ����ķ�ջ��ڽڵ�
				{
					head->FirstGeomClose = newNode->pminhead;//������Ϊͷ���
					newNode->pminhead->m_AcceptGeomcloseHEAD = true;//�Ӵ˸ýڵ�ͱ��̶���
				}
				//newNode->pminhead��ǰ��պ��Ǳ���λ��
				//�������һ�����������ǡ��pLastAcceptCHead=newNode->pminhead;��˱��ܽ���������
				else if (newNode->pminhead->prevGeomcloseNode->m_AcceptGeomcloseHEAD)
				{
					//��ôʲô�����ö�
					newNode->pminhead->m_AcceptGeomcloseHEAD = true;//�Ӵ˸ýڵ�ͱ��̶���

				}
				else//˵��newNode->pminnode����ͷ����ˣ���ô����ڵ�Ҫ�ҵ�Ŀǰ���һ��m_AcceptGeomcloseHEADû�б�����Ϊtrue�ķ�ջ��ڵ����
				{

					//���������Ѱ�ң�����pLastAcceptCHead�����һ������λ�Ľڵ㣬��newNode->pminhead����Ҫ�ҵ�pLastAcceptCHead����
					//������ÿ�ε���˫������Ľڵ�λ��ʱ�򣬶�Ҫ���Ǹýڵ��ǲ���ͷβ����һ��ڵ�ģ���������Բ��ÿ���ͷ�����
					//��ôҪ���ҵ����һ��û����λ�ķ�ջ�������ͷ���Ҳ����������ǰ�����˱���λ�ķ�ջ�
					pLastAcceptCHead = head->FirstGeomClose;
					while (pLastAcceptCHead->m_AcceptGeomcloseHEAD)
						pLastAcceptCHead = pLastAcceptCHead->nextGeomcloseNode;
					//pLastAcceptCHead��û�б���λ��
					if (!(newNode->pminhead->nextGeomcloseNode))//�����β�ڵ�
					{
						//��Ȼ�Ѿ���β�ڵ�newNode->pminhead����ôֱ�Ӱ���ǰ��һ��ֵ�ú���ڵ�ָ��NULL��
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = NULL;
					}
					else//һ��ڵ�
					{
						//newNode->pminheadԭ��ǰ��ָ���ϵ�ĵ���
						newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead->nextGeomcloseNode;
						newNode->pminhead->nextGeomcloseNode->prevGeomcloseNode = newNode->pminhead->prevGeomcloseNode;
					}
					//���ϴ�������newNode->pminhead��ԭ��λ�õĹ�ϵ��������Ҫ�������ڱ���λʱ��Ĺ�ϵ��
					//���pLastAcceptCHead����ͷ�ڵ㣬˵����һ����ջ�����û�б���λ
					//Ҫ���ǵ�һ���ǵ�pLastAcceptCHead=newNode->pminhead��ʱ����ô��
					if (!(pLastAcceptCHead->prevGeomcloseNode))//��ô��������ǣ�����һ��ĩβ��������Ǹ���ջ�����ͷ��㣬��������Ҳû�з�ջ�����λ
					{
						//���ʱ��pLastAcceptCHead���Ƿ�ջ�˫�������ͷ���
						//Ӧ�ð�newNode->pminhead����headָ��ĵ�һ����ջ��ڵ�
						head->FirstGeomClose = newNode->pminhead;
						newNode->pminhead->prevGeomcloseNode = NULL;//�׽��
						newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
						pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
						newNode->pminhead->m_AcceptGeomcloseHEAD = true;//��λ
					}
					//��һ�������newNode->pminhead������pLastAcceptCHead���
					else//��ô��newNode->pminheadֱ�ӹҵ�pLastAcceptCHead����ȥ
					{
						//��ҪpLastAcceptCHead�������Ѿ�����λ�ģ����ǵ�һ��û����λ�ģ�pLastAcceptCHead->prevGeomcloseNode�������һ������λ��
						newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
						newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
						pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
						pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
						newNode->pminhead->m_AcceptGeomcloseHEAD = true;//��λ
					}
				}
				pTempCHead = newNode->pminhead;//�������ѭ��
			}
			b;
			pTempCHead = pTempCHead->nextGeomcloseNode;
		}
	}
}
//���ڶ�����ջ�ͷ����Ϊ��һ��ͷ���
NestResultDataNode* GeomForCut::ChangeSencondCH2FH(NestResultDataNode*head)
{
	GeomCloseHEAD*pFirstCloseHead;
	pFirstCloseHead = head->FirstGeomClose;
	pFirstCloseHead = pFirstCloseHead->nextGeomcloseNode;
	pFirstCloseHead->prevGeomcloseNode = NULL;
	head->FirstGeomClose = pFirstCloseHead;
	return head;
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//�����Ǽ����и�գ����ǰ����и�����У������Ƕ�׵����и��ڻ������и��⻷
//�Ȼ�ȡ����ֱ�ߵĲ���
void GeomForCut::GetLinePara(Line_Cross *l)
{
	l->a = l->p1.y - l->p2.y;
	l->b = l->p2.x - l->p1.x;
	l->c = l->p1.x*l->p2.y - l->p2.x*l->p1.y;
}
//������λС��
double GeomForCut::Retain4Decimals(double a)
{
	//4��0������4����Ч����
	//int c = (a * 1000000 );
	//double b = c / 1000000.0;
	//int d = (b* 100000 );
	//double e = d / 100000.0;
	int f = (a * 10000+0.5 );
	double h = f / 10000.0;
	return h;

}

//�����������ֱ�ߵİ˸��㣬�������ֱ���Ƿ����ཻ�㣬�оͷ��ؽ��㣬û�оͷ���-1
//����㷨��������ֱ�ߺ�ˮƽ��֮��Ľ���
Point GeomForCut::GetCrossPoint(Line_Cross *m_linePoint1, Line_Cross  *m_linePoint2)
{
	Point p;
	GetLinePara(m_linePoint1);
	GetLinePara(m_linePoint2);
	double D = m_linePoint1->a*m_linePoint2->b - m_linePoint2->a*m_linePoint1->b;
	if (0 != D)//��D!=0��ʱ������ֱ�߲�ƽ�У���ʱ����н���ġ�
	{
		p.x = (m_linePoint1->b*m_linePoint2->c - m_linePoint2->b*m_linePoint1->c) / D;
		p.y = (m_linePoint1->c*m_linePoint2->a - m_linePoint2->c*m_linePoint1->a) / D;
		//�Խ��㱣����λС��
		p.x = Retain4Decimals(p.x);
		p.y = Retain4Decimals(p.y);

	}
	else//��D=0��ʱ������ֱ��ƽ�У���ʱ����û�н���ġ�
	{
		//������ͼ���棬����ķ�Χ��0-800֮���������
		p.x = -1.0;
		p.y = -1.0;
	}
	return p;
}
//���Ĵ��룡
//���Ĵ��룡
//���Ĵ��룡
//������ͼ�ķ�ջ��Ƿ�Ƕ��ȫ���ҳ������������Ƿ�Ƕ�׵Ĺ�ϵ�ҵ���ͬ�ķ�ջ�ͷ�����
void GeomForCut::Find_AdjustNestCloseHead(NestResultDataNode*head)
{
	int a = 0;
	GeomCloseHEAD*pTempCHead, *pTempNextCHead;//��������ȫ����ջ�
	GeomCloseHEAD*pNextCHead;//��ΪpTempNextCHead�������pTempCHead���ӷ�ջ���Ҫ�뿪���ջ��Ľ��ӻ�
	m_G2CloseHeadNest.m_IfCloseNest= false;//�жϵڶ�����ջ��ǲ��ǵ�һ����ջ����ӷ�ջ�
	//�㷨��ʼ
	pTempCHead = head->FirstGeomClose;
	//��Ҫȫ������һ�飬��ֻ�����һ����ջ���ʱ��Ϳ�������ȥ�ˣ���ΪҪǶ������Ҫ������ջ�
	//��while (pTempCHead->nextGeomcloseNode)��Ȼ����ԭ������pTempCHead֮ǰ�ĺ�һ�����ڱ���������ӷ�ջ�����ô�ͻ���pTempCHead������һ��
	//��ô���ͳ������һ������ô���������pTempCHead = pTempCHead->nextGeomcloseNode;
	//��ʱpTempCHead�Ѿ���NULL������pTempCHead->nextGeomcloseNode�ǻᱨ���
	//����Ҫ��Ϊwhile (pTempCHead)
	while (pTempCHead)//�����ڶ������Խ���ѭ�������һ����Ҫ����ѭ����
	{
		pNextCHead = head->FirstGeomClose;
		//���ǰ���ÿһ�����Ǵ󻷵ļ�����ж����µķ�ջ������Ų�
		while (pNextCHead)//������һ��
		{
			//��pNextCHead��һ������pTempNextCHead,
			pTempNextCHead = pNextCHead;
			////////////�����ߵķ�ʽ�ж���������ջ���û��Ƕ��
			//////////m_G2CloseHeadNest = EstimateCloseHeadNest(pTempCHead, pTempNextCHead);
			//�ð���ķ�ʽ���ж�������ջ���û��Ƕ��
			m_G2CloseHeadNest = JudgeCloseHeadNest(pTempCHead, pTempNextCHead);
			//�����Ƕ�ף���ô��Ҫ�ѵڶ�����ջ��ҵ���һ�������
			if (m_G2CloseHeadNest.m_IfCloseNest)
			{
				//�ѵڶ�����ջ����ҵ���һ�������
				pTempNextCHead = m_G2CloseHeadNest.KidCloseHead;
				SetInSideClose(pTempCHead, pTempNextCHead);
				pNextCHead = m_G2CloseHeadNest.NextCloseHead;//����ĺ����Ѿ���������е� pNextCHead->nextGeomcloseNode���ƵĹ���
			}
			else//�������û��Ƕ�ף���ô���ڵ�pNextCHead��û������仯�������Լ����ҵ�ԭ������һ��
			{
				pNextCHead = pNextCHead->nextGeomcloseNode;

			}
		}	
		
		a++;
		pTempCHead = pTempCHead->nextGeomcloseNode;
	}
}
//���Ĵ��룡
//���Ĵ��룡
//���Ĵ��룡
//�ж�������ջ��Ƿ����Ƕ��
//�ݹ鷽��
Geom2CloseHeadNest  GeomForCut::EstimateCloseHeadNest(GeomCloseHEAD*pTempCHead, GeomCloseHEAD*pTempNextCHead)
{
	GeomEleNode*ptemp1Gelenode, *ptemp2Gelenode;
	double pre_x, pre_y, temp_x, temp_y;
	double x1_min, x1_max, y1_min, y1_max;//ֱ��1�ķ�Χ
	double x2_min, x2_max, y2_min, y2_max;//ֱ��2�ķ�Χ

	//�ӵ�һ����ջ��ĵ�һ��ͼԪ�ڵ㿪ʼ
	//Ҫ��һ����ջ��������Σ���Բ��������
	ptemp1Gelenode = pTempCHead->FirstGeomele;
	if (3 != ptemp1Gelenode->m_GeomStandData.m_typegeomele)//�������Բ�������
	{
		//�ж��ǲ���������
		if (NULL == ptemp1Gelenode->nextGeomeleNode->nextGeomeleNode)//������
		{
			//���������ζ��ԣ�Ҫ�Զ�����Ա��е�ȡֱ��
			;
		}
		else//һ������
		{
			//��һ��while��Ҫ��֤��һ����ջ������ж�����ܲ����ĶԽ������걸��
			while (ptemp1Gelenode->nextGeomeleNode->nextGeomeleNode)//��������ڵĽڵ����һ������һ������NULL��ʱ����ô����ѭ������Ϊ��Щ���Լ���ǰ���ظ���
			{

				m_line1Point.p1.x = ptemp1Gelenode->m_GeomStandData.GeoEle_start_x0;//��ǰͼԪ���ݵ���ʼ��
				m_line1Point.p1.y = ptemp1Gelenode->m_GeomStandData.GeoEle_start_y0;
				m_line1Point.p2.x = ptemp1Gelenode->nextGeomeleNode->nextGeomeleNode->m_GeomStandData.GeoEle_start_x0;//����һ��ͼԪ���ݵ���ʼ�㣨���Բ�Ҫ�ж���û������һ������ͼԪ��
				m_line1Point.p2.y = ptemp1Gelenode->nextGeomeleNode->nextGeomeleNode->m_GeomStandData.GeoEle_start_y0;
				//���ֱ��1��xy��Χֵ
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
				
				if (pTempNextCHead == pTempCHead)//��Ϊ��ͷ��ʼ���϶�����������ͬ��ʱ��
				{
					//��pTempCHead�� pTempNextCHead��ȵ�ʱ��Ҫע���ж��ǲ������һ����ջ�
					if (!(pTempNextCHead->nextGeomcloseNode))//���һ����ջ���ʱ���Լ������ܰ����Լ�
					{
						m_G2CloseHeadNest.m_IfCloseNest = false;
						m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
						return m_G2CloseHeadNest;
					}
					else//�������һ����ʱ��
					{
						pTempNextCHead = pTempNextCHead->nextGeomcloseNode;
					}
					
				}
					ptemp2Gelenode = pTempNextCHead->FirstGeomele;
					//Ҫ�ж������ջ��ǲ���Բ
					if (3 != ptemp2Gelenode->m_GeomStandData.m_typegeomele)//�������Բ
					{
						//�ڶ���while��Ҫ��֤�ڶ�����ջ��ı߶��������һ��
						while (ptemp2Gelenode)//��ôҪȫ������һ��
						{
							m_line2Point.p1.x = ptemp2Gelenode->m_GeomStandData.GeoEle_start_x0;
							m_line2Point.p1.y = ptemp2Gelenode->m_GeomStandData.GeoEle_start_y0;
							m_line2Point.p2.x = ptemp2Gelenode->m_GeomStandData.GeoEle_start_x1;
							m_line2Point.p2.y = ptemp2Gelenode->m_GeomStandData.GeoEle_start_y1;

							//���ֱ��2��xy��Χֵ
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
								//��ý���
							m_PointCross = GetCrossPoint(&m_line1Point, &m_line2Point);
							//�жϷ�����x��ֵ�ڲ���m_line1Point->p1.x��m_line1Point->p2.x����y��ֵm_line1Point->p1.y��m_line1Point->p2.y�ķ�Χ��
							//��Ϊ��ֱ�ߴ�ֱx���ʱ��ֱ�����κ�ֱ�ߵĽ����xֵ���������Χ�ڣ�����ֻ������ֵ���������Χ�ڣ�����Ƕ�װ����Ĺ�ϵ
							//Ҫ�Ƚ����ĸ���������λС��
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
								//����н��������£���ôӦ�ð������ջ�2�ҵ���ջ�1�У���Ϊ�����ӷ�ջ�
								m_G2CloseHeadNest.m_IfCloseNest = true;
								m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
								m_G2CloseHeadNest.NextCloseHead = pTempNextCHead->nextGeomcloseNode;
								return m_G2CloseHeadNest;
							}
							else//���������Χ�ڣ�˵�����ǽ���
							{
								;
							}
							ptemp2Gelenode = ptemp2Gelenode->nextGeomeleNode;
						}
						m_G2CloseHeadNest.m_IfCloseNest = false;
						m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
						return m_G2CloseHeadNest;
					}
					else//����������ӷ�ջ���Բ�������
					{
						//��Բ������£���ô��Ҫֱ����ֱ����Բ��û�н���ͺ��ˡ�
						;
					}
				ptemp1Gelenode = ptemp1Gelenode->nextGeomeleNode;
			}
		}

	}
	else//��ջ���Բ�������
	{
		;
	}
}
//���Ĵ��룡
//���Ĵ��룡
//���Ĵ��룡
//��pHNtemp�ҵ�pHtemp���ӷ�ջ���
void  GeomForCut::SetInSideClose(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp)
{
	
	GeomCloseHEAD*pKidHtemp=NULL;
	GeomCloseHEAD*pKidLastClose = NULL;//�ӷ�ջ����ڵķ�ջ�˫�������е����һ����ջ�����
	bool m_IfCloseNest = false;//�жϵڶ�����ջ��ǲ��ǵ�һ����ջ����ӷ�ջ�
	//��Ҫ�ж�pHtemp��û���ӷ�ջ�������еĻ��ֵ�����pHNtemp��ԭ�����ӷ�ջ���û����Ƕ�Ĺ�ϵ
	if (!(pHtemp->FirstInsideGCloseNode))//���ԭ����û���ӷ�ջ�
	{
		pHtemp->FirstInsideGCloseNode = pHNtemp;
		//ͬʱ��Ҫ��pHNtemp���˫�������ǰ�����ָ�����
		//����Ҫע��pHNtemp�����һ����ջ�����������������ǵ�һ����ջ�
		if (pHNtemp->nextGeomcloseNode)//���pHNtemp�������ķ�ջ�
		{
			pHNtemp->prevGeomcloseNode->nextGeomcloseNode = pHNtemp->nextGeomcloseNode;//pHNtemp��ǰ��ĺ���ԭ����pHNtemp�������ڸ������ĺ�����Ǹ���
			pHNtemp->nextGeomcloseNode->prevGeomcloseNode = pHNtemp->prevGeomcloseNode;//pHNtemp�ĺ����ǰ��ԭ����pHNtemp�������ڸ�������ǰ����Ǹ���
		}
		else//���pHNtemp�����ķ�ջ��ڵ�������
		{
			pHNtemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
		}
		//��������֮��pHNtemp�͸ð�ǰ���ΪNULL�ˡ�
		pHNtemp->prevGeomcloseNode =pHNtemp->nextGeomcloseNode = NULL;
		//ָ���丸�ڵ�
		pHNtemp->prevOutsideGCloseNode = pHtemp;
	}
	else//�������pHtemp�Ѿ������ӷ�ջ��ˣ���ôҪ�ж����ڵ��ӷ�ջ���ԭ�����ӷ�ջ���û��Ƕ�׹�ϵ
	{
		pKidHtemp = pHtemp->FirstInsideGCloseNode;
		//��Ҫȫ������һ�飬��ֻ�����һ����ջ���ʱ��Ϳ�������ȥ�ˣ���ΪҪǶ������Ҫ������ջ�
		while (pKidHtemp)//�����е��ӷ�ջ�����һ��
		{
			//���ǰ���ÿһ�����Ǵ󻷵ļ�����й滮
			//������������ջ�ͷ����ж��Ƿ���Ƕ�׹�ϵ
			//////////�ж���������ջ���û��Ƕ��
			////////m_G2CloseHeadNest = EstimateCloseHeadNest(pKidHtemp, pHNtemp);
			//�ð���ķ�ʽ���ж�������ջ���û��Ƕ��
			m_G2CloseHeadNest = JudgeCloseHeadNest(pKidHtemp, pHNtemp);

			//�����Ƕ�ף���ô��Ҫ�ѵڶ�����ջ��ҵ���һ�������
			if (m_G2CloseHeadNest.m_IfCloseNest)
			{
				//�ѵڶ�����ջ����ҵ���һ�������
				pHNtemp = m_G2CloseHeadNest.KidCloseHead;
				SetInSideClose(pKidHtemp, pHNtemp);
			}
			else//���û��Ƕ�ף�
			{
				//����Ҫ��pHNtempԭ���Ĺ�ϵ���н���
				//����Ҫע��pHNtemp�����һ����ջ�����������������ǵ�һ����ջ�
				if (pHNtemp->nextGeomcloseNode)//���pHNtemp�������ķ�ջ�
				{
					pHNtemp->prevGeomcloseNode->nextGeomcloseNode = pHNtemp->nextGeomcloseNode;//pHNtemp��ǰ��ĺ���ԭ����pHNtemp�������ڸ������ĺ�����Ǹ���
					pHNtemp->nextGeomcloseNode->prevGeomcloseNode = pHNtemp->prevGeomcloseNode;//pHNtemp�ĺ����ǰ��ԭ����pHNtemp�������ڸ�������ǰ����Ǹ���
				}
				else//���pHNtemp�����ķ�ջ��ڵ�������
				{
					pHNtemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
				}
				//���з�ջ��Ĺ�ϵ
				//��ô��Ҫ��pKidHtemp���ڵ��ӷ�ջ�˫�������ջ��ˣ����û������ֱ�Ӱ�pHNtemp�ҵ�pKidHtemp����ͺ���
				if (!(pKidHtemp->nextGeomcloseNode))//�������û��������ջ���
				{
					pKidHtemp->nextGeomcloseNode = pHNtemp;//pKidHtemp��Ϊ�½�����һ������ķ�ջ������Ѿ���ԭ���Ĺ�ϵ��������֮�󶪵��ˣ������û����ݵ��ˣ���ʱǰ��ڵ㶼��NULL
					//��������֮��pHNtemp�͸ð�����ӷ�ջ�����Ĺ�ϵ��������
					pHNtemp->prevGeomcloseNode = pKidHtemp;
					pHNtemp->nextGeomcloseNode = NULL;
				}
				else//pKidHtemp���ڵ��ӷ�ջ�˫�������ջ�������������ջ�
				{
					//��Ȼ���滹��������ջ�����ô�п����Ǹ�������ķ�ջ�Ƕ�ף������Ǹ�ͷ����ջ�
					//�ж�pHNtemp��pKidHtemp����ķ�ջ��Ƿ񹹳�Ƕ�׹�ϵ�����������ôҪ�ҵ����棬
					//��Ҫȫ������һ�飬��ֻ�����һ����ջ���ʱ��Ϳ�������ȥ�ˣ���ΪҪǶ������Ҫ������ջ�
					pKidHtemp = pKidHtemp->nextGeomcloseNode;
					while (pKidHtemp)//���ӷ�ջ��������µĲ��еķ�ջ�����һ��
					{
						//�ð���ķ�ʽ���ж�������ջ���û��Ƕ��
						m_G2CloseHeadNest = JudgeCloseHeadNest(pKidHtemp, pHNtemp);

						//�����Ƕ�ף���ô��Ҫ�ѵڶ�����ջ��ҵ���һ�������
						if (m_G2CloseHeadNest.m_IfCloseNest)
						{
							//�ѵڶ�����ջ����ҵ���һ�������
							pHNtemp = m_G2CloseHeadNest.KidCloseHead;
							SetInSideClose(pKidHtemp, pHNtemp);
							m_IfCloseNest = true;//˵���Ѿ�Ƕ�׽�ȥ��
							break;
						}
						//�����ҵ����һ����Ȼ���pHNtemp�ҵ������
						if (!(pKidHtemp->nextGeomcloseNode))//���pKidHtemp�����һ��
						{
							pKidLastClose = pKidHtemp;//�������һ����ջ����ᵽ������˵�������һ����ջ���pHNtempҲû��Ƕ�׵Ĺ�ϵ
							m_IfCloseNest = false;
						}
						pKidHtemp = pKidHtemp->nextGeomcloseNode;
					}			
					if (!(m_IfCloseNest))
					{
						pKidLastClose->nextGeomcloseNode = pHNtemp;
						//��������֮��pHNtemp�͸ð�����ӷ�ջ�����Ĺ�ϵ��������
						pHNtemp->prevGeomcloseNode = pKidLastClose;
						pHNtemp->nextGeomcloseNode = NULL;
					}
					
				}
				
			}
			//���whileѭ������Ҫ�ٴ������������ȥѭ������Ϊ����һ���Եݹ�ĺ�������ȥ�������elseʱ������while��ʵӦ����Ҫֹͣ��
			//pKidHtemp = pKidHtemp->nextGeomcloseNode;
			//���������Ĵ��룬����ʹ��pKidHtemp����Լ�ѭ��һ��
			break;
		}
	}
}
//���ջ��İ������xy�ļ���ֵ

Envelope_Rect  GeomForCut::GetLimtofGeomClosed(GeomCloseHEAD*pTempCHead)
{
	GeomEleNode*ptempGelenode;
	double temp_x_min, temp_x_max, temp_y_min, temp_y_max;
	double x_min, x_max, y_min, y_max;//Ѱ�ҷ�ջ�1�İ�����εķ�Χ
	//�ӵ�һ����ջ��ĵ�һ��ͼԪ�ڵ㿪ʼ
	//Ҫ��һ����ջ��������Σ���Բ��������
	ptempGelenode = pTempCHead->FirstGeomele;
	if (3 != ptempGelenode->m_GeomStandData.m_typegeomele)//�������Բ�������
	{
		//�ȸ���һ����ʼ����ֵ
		x_min = ptempGelenode->m_GeomStandData.GeoEle_start_x0;
		x_max = ptempGelenode->m_GeomStandData.GeoEle_start_x0;
		y_min = ptempGelenode->m_GeomStandData.GeoEle_start_y0;
		y_max = ptempGelenode->m_GeomStandData.GeoEle_start_y0;

		//Ҫ��֤��ջ������ж�������ݶ�������
		while (ptempGelenode)//��������ڵĽڵ����һ������һ������NULL��ʱ����ô����ѭ������Ϊ��Щ���Լ���ǰ���ظ���
		{
			//temp_x1_min��temp_x1_max��ʵ���ִ�С
			//Բ��Ҳ�Ѿ����������ֱ��ģʽ
			temp_x_min = ptempGelenode->m_GeomStandData.GeoEle_start_x0;
			temp_x_max = ptempGelenode->m_GeomStandData.GeoEle_start_x1;
			temp_y_min = ptempGelenode->m_GeomStandData.GeoEle_start_y0;
			temp_y_max = ptempGelenode->m_GeomStandData.GeoEle_start_y1;
			//x��limit
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
			//y��limit
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
	else//�����Բ������£���ô�Ͳ��ù�Բ�ĵ�ˮƽ�ߺʹ�ֱ����Բ�Ľ��������ֵ
	{
		;
	}
	x_min = Retain4Decimals(x_min);
	x_max = Retain4Decimals(x_max);
	y_min = Retain4Decimals(y_min);
	y_max = Retain4Decimals(y_max);
	m_GemoClosedLimt.x_min = x_min;
	m_GemoClosedLimt.x_max = x_max;
	m_GemoClosedLimt.y_min = y_min;
	m_GemoClosedLimt.y_max = y_max;
	return m_GemoClosedLimt;
}

//�����㷨
//�����㷨
//�����㷨
//�ð���ķ������ж�������ջ��Ƿ�Ƕ��
Geom2CloseHeadNest  GeomForCut::JudgeCloseHeadNest(GeomCloseHEAD*pTempCHead, GeomCloseHEAD*pTempNextCHead)

{
	Envelope_Rect m_GeomClosed1, m_GeomClosed2;
	double x1_min, x1_max, y1_min, y1_max;//Ѱ�ҷ�ջ�1�İ�����εķ�Χ
	double x2_min, x2_max, y2_min, y2_max;//Ѱ�ҷ�ջ�1�İ�����εķ�Χ
	if (pTempNextCHead == pTempCHead)//��Ϊ��ͷ��ʼ���϶�����������ͬ��ʱ��
	{
		//��pTempCHead�� pTempNextCHead��ȵ�ʱ��Ҫע���ж��ǲ������һ����ջ�
		if (!(pTempNextCHead->nextGeomcloseNode))//���һ����ջ���ʱ���Լ������ܰ����Լ�
		{
			m_G2CloseHeadNest.m_IfCloseNest = false;
			m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
			m_G2CloseHeadNest.NextCloseHead = NULL;
			return m_G2CloseHeadNest;
		}
		else//�������һ����ʱ��
		{
			pTempNextCHead = pTempNextCHead->nextGeomcloseNode;
		}

	}
	//��������ջ��İ�����εķ�Χ
	m_GeomClosed1 = GetLimtofGeomClosed(pTempCHead);
	m_GeomClosed2 = GetLimtofGeomClosed(pTempNextCHead);
	//����ֵ������λС��
	x1_min = m_GeomClosed1.x_min;
	x1_max = m_GeomClosed1.x_max;
	y1_min = m_GeomClosed1.y_min;
	y1_max = m_GeomClosed1.y_max;
	x2_min = m_GeomClosed2.x_min;
	x2_max = m_GeomClosed2.x_max;
	y2_min = m_GeomClosed2.y_min;
	y2_max = m_GeomClosed2.y_max;
	//�����Ѿ��Ѱ�����εķ�Χ�������
	//������ж����������Ƿ�Ƕ�׽����ж�
	//��Ҫ���жϺ����Ǹ���ջ��ǲ���ǰ���ջ����ӷ�ջ�
	//����Ĭ��ͬ����ջ������ֵ��min<max
	if (((x1_min <=x2_min ) && (x2_max <= x1_max)) && ((y1_min <= y2_min) && (y2_max<=y1_max)))
	{
		//����н��������£���ôӦ�ð������ջ�2�ҵ���ջ�1�У���Ϊ�����ӷ�ջ�
		m_G2CloseHeadNest.m_IfCloseNest = true;
		m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
		m_G2CloseHeadNest.NextCloseHead = pTempNextCHead->nextGeomcloseNode;
		
	}
	else//���������Χ�ڣ�˵�����ǽ���
	{
		m_G2CloseHeadNest.m_IfCloseNest = false;
		m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
		m_G2CloseHeadNest.NextCloseHead = pTempNextCHead->nextGeomcloseNode;
	}
	return m_G2CloseHeadNest;
}
