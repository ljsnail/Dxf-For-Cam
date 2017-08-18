#include "stdafx.h"
#include "GeomForCut.h"
#define WEIGHT_ORIGIN 0.1//��ջ�������ԭ������Ȩ��
#define WEIGHT_PTP 0.9//��ջ�����һ����ջ�֮������Ȩ��

GeomForCut::GeomForCut()
{
	//BatchHEAD m_batchHead = { 0, NULL };//�ṹ���ʼ��
	i = 0;
	j = 0;
	a = 0;
	m_nestrsltNode = { NULL, NULL, NULL, false };//������Ϊÿ�δ洢ֵ��ȫ�ֱ�������ÿ��֮�󶼻ᱻ��д
	m_ceramic_Head = NULL;
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
		//��ʵ��Լ����ʲô��û��
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
//���շ�ջ������ֹ�㵽����ԭ��ľ����ɽ���Զ
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
	/*while (pHtemp->nextGeomcloseNode)
	{
		pHtemp = pHtemp->nextGeomcloseNode;
	}
	*///�ص���m_TotalCloseID������Ҫ��
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
				//ע�����֣������û���κ�һ���ڵ㱻��λ����һ������ĵ�Ҳ����ͷ�������
				if ((!(phnowtemp->prevGeomcloseNode)) && (!(phnowtemp->m_AcceptGeomcloseHEAD)))//˵�����������û��һ����ջ�����λ��
				{
					if (!(phstarttemp->nextGeomcloseNode))//�����β�ڵ�
					{
						//�����ж�phstarttemp��ǰһ���ǲ��ǵ����ڶ������������ֱ�ӹ���ȥ����Ҫ�䣬������ѭ��				
							phstarttemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
							phstarttemp->nextGeomcloseNode = phnowtemp;
							phnowtemp->prevGeomcloseNode = phstarttemp;
							phstarttemp->prevGeomcloseNode = NULL;
							phstarttemp->m_AcceptGeomcloseHEAD = true;
							head->FirstGeomClose = phstarttemp;
					}
					else//һ��ڵ�
					{
						phstarttemp->prevGeomcloseNode->nextGeomcloseNode = phstarttemp->nextGeomcloseNode;//��ԭ��ǰ��ĺ��汾�������ģ��������ˣ���Ȼ�ͱ������������Ǹ�
						phstarttemp->nextGeomcloseNode->prevGeomcloseNode = phstarttemp->prevGeomcloseNode;//��ԭ�������ǰ�汾�������ģ��������ˣ���Ȼ�ͱ������ǰ����Ǹ�
						phnowtemp->prevGeomcloseNode = phstarttemp;
						phstarttemp->nextGeomcloseNode = phnowtemp;
						phstarttemp->prevGeomcloseNode = NULL;
						phstarttemp->m_AcceptGeomcloseHEAD = true;
						head->FirstGeomClose = phstarttemp;
					}

				}
				//���������Ѱ�ң�����phnowtemp�����һ������λ�Ľڵ㣬��phstarttemp����Ҫ�ҵ�phnowtemp����
				//������ÿ�ε���˫������Ľڵ�λ��ʱ�򣬶�Ҫ���Ǹýڵ��ǲ���ͷβ����һ��ڵ�ģ���������Բ��ÿ���ͷ�����
				else
				{
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
	int a = 0;
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
		a++;
		//if (paimnode->prevGeomeleNode)//����NULL����ô����˵���Ǵ�Բ�Ǳ߻�õ�����
		if (pHtemp->FirstGeomele->m_GeomStandData.m_typegeomele!=3)//����Բ
		{
			ChangeEleNodeForAloneClse(pHtemp, paimnode);
		}
		pHtemp = pHtemp->nextGeomcloseNode;//��һ����ջ�������֮�󣬽���ڶ�����ջ������ѭ�������������dxf����ķ�ջ��������ꣻ����Բ

	}
}
//���Ĵ��룬�����Ҷ�����
//���Ĵ��룬�����Ҷ�����
//���Ĵ��룬�����Ҷ�����
//�����Ѿ�ȷ����˳���ջ�˳��ȥ����ÿһ����ջ������и���Ƶ㣬ʹ�ñ���ײ��
void GeomForCut::ChangeEleNode_Avoid_Impact(NestResultDataNode*head)
{
	int a = 0;
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
				if (3 == ptemp->m_GeomStandData.m_typegeomele)//Բ
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
					distance_1 = fabs(sqrt(((x1 - m_prevGeomCloseEnd_x)*(x1 - m_prevGeomCloseEnd_x)) + ((y1 - m_prevGeomCloseEnd_y)*(y1 - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
					distance_2 = fabs(sqrt(((x2 - m_prevGeomCloseEnd_x)*(x2 - m_prevGeomCloseEnd_x)) + ((y2 - m_prevGeomCloseEnd_y)*(y2 - m_prevGeomCloseEnd_y))));//�������������֮�����̾���
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
				if (m_mindistant<m_tempmindistant)
				{
					m_mindistant = m_tempmindistant;//����СֵҲҪ����
					paimnode = ptemp;//˵��������������һ����ջ���ֹ��ľ������Щ

				}
			}
			ptemp = ptemp->nextGeomeleNode;
		}
		//���ˣ�һ����ջ���������������о�����̵��Ǹ�����Ѿ��ҵ���������Ҫ�Է�ջ��ڵĽ��������ݵ�����
		a++;
		//if (paimnode->prevGeomeleNode)//����NULL����ô����˵���Ǵ�Բ�Ǳ߻�õ�����
		if (pHtemp->FirstGeomele->m_GeomStandData.m_typegeomele != 3)//����Բ
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
	//head = ChangeSencondCH2FH(head);
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
				//newNode->m_mindistant = 1 / newNode->m_mindistant;
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
	m_ceramic_Head = pFirstCloseHead;//�����ש�Ĳ����������������и� ��������ײ���ʱ��ʹ��
	//������������ƽ���и�ͼ�ķ�ջ�ͷ���ʱ��Ҳ���ǰ��������ʱ�򣩣���û��nextGeomcloseNode�ġ�
	if (!(pFirstCloseHead->nextGeomcloseNode))//�������
	{
		pFirstCloseHead = pFirstCloseHead->FirstInsideGCloseNode;
	}
	else
	{
		pFirstCloseHead = pFirstCloseHead->nextGeomcloseNode;

	}
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
//�����ӷ�ջ�����������ջ��Ĵ�׵�(���ĸ���ջ��ڲ���ͼԪ�ڵ��˳��)
void GeomForCut::ChangeOutsideGeomEleNode(GeomCloseHEAD*pKidCloseHead, GeomCloseHEAD*pFadClosedHead)
{
	GeomEleNode*pkidGelenode, *pfadGelenode,*pmindistantGelenode;
	double x0, y0, x1, y1;
	double m_distant, m_tempdistant;
	//�ҵ��ӷ�յ����һ��ͼԪ�ڵ�
	pkidGelenode = pKidCloseHead->FirstGeomele;
	while (pkidGelenode->nextGeomeleNode)
		pkidGelenode = pkidGelenode->nextGeomeleNode;
	x0 = pkidGelenode->m_GeomStandData.GeoEle_start_x1;
	y0 = pkidGelenode->m_GeomStandData.GeoEle_start_y1;
	pmindistantGelenode = pFadClosedHead->FirstGeomele;
	x1 = pmindistantGelenode->m_GeomStandData.GeoEle_start_x0;
	y1 = pmindistantGelenode->m_GeomStandData.GeoEle_start_y0;
	m_distant = fabs(sqrt(((x0 - x1)*(x0 - x1)) + ((y0 - y1)*(y0 - y1))));//�������������֮�����̾���
	pfadGelenode = pmindistantGelenode;
	while (pmindistantGelenode)//�����ڵ��ͼԪȫ������һ��
	{
		x1 = pmindistantGelenode->m_GeomStandData.GeoEle_start_x0;
		y1 = pmindistantGelenode->m_GeomStandData.GeoEle_start_y0;
		m_tempdistant = fabs(sqrt(((x0 - x1)*(x0 - x1)) + ((y0 - y1)*(y0 - y1))));//�������������֮�����̾���
		if (m_tempdistant<m_distant)
		{
			pfadGelenode = pmindistantGelenode;
			//�������д��룬�ͻ����ֻҪ���Ͼͽ�������ȴ�����ǲ��������С
			m_distant = m_tempdistant;
		}
		pmindistantGelenode = pmindistantGelenode->nextGeomeleNode;
	}
	ChangeEleNodeForAloneClse(pFadClosedHead, pfadGelenode);

}
//�����һ�㸸��ջ���ͼԪͷ��㣬�ı���һ����һ�㸸��ջ����ӷ�ջ���˳��,�������һ���ӷ�ջ����
GeomCloseHEAD* GeomForCut::ChangeINsideGeomEleNode(GeomCloseHEAD*pFadClosedHead, GeomCloseHEAD*pNKidCloseHead)
{

	Mindistant_EleNode*newNode = (Mindistant_EleNode*)malloc(sizeof(Mindistant_EleNode));
	newNode->m_mindistant = 99999.0;
	newNode->pminhead = NULL;
	newNode->pminnode = NULL;
	GeomCloseHEAD*pTempCHead, *ppTempCHead;//��������ȫ����ջ�
	GeomCloseHEAD*pLastAcceptCHead;//��Ѱ�����һ������λ�ķ�ջ�ͷ���
	GeomCloseHEAD*pTheLastKidCHead=NULL;//��Ѱ�����һ������λ�ķ�ջ�ͷ���

	GeomCloseHEAD*pMinDistantCloseHead, *pntemp;//��Ѱ�����һ������λ�ķ�ջ�ͷ���

	GeomEleNode*pTempGelenode;
	double pre_x, pre_y, temp_x, temp_y;
	double m_temp_mindistant;//������ʱ����̾��룬��������յ���̾��룬��ô�͸��Ǹ���	newNode->m_mindistant;
	double m_tempmindistant_origin;//��ԭ�����
	double m_tempmindistant_ptp;//���Ѿ�����λ�ķ�ջ��ľ���
	double m_origin_x = 0.0;//����ԭ��
	double m_origin_y = 0.0;//����ԭ��
	int a = 0, b = 0, m_TotalCloseID, m_GeomCloseID;
	//�㷨��ʼ
	//Ҫע���һ���ջ�ֻ��һ��û��Ƕ�����
	if (pNKidCloseHead->prevOutsideGCloseNode)//����ֻ��һ��
	{
		pTempCHead = pNKidCloseHead;
		while (pTempCHead)//������ӷ�ջ�ȫ������һ��,ֱ�����һ������ѭ��
		{
			a++;
			while (!(pTempCHead->m_KidAcceptGeomCloseHead))//ֻ�е��÷�ջ���û�б��滮��ʱ�򣬽����������
			{
				//pTempCHead�ܽ���˵�����ǵ�һ��û�б���λ�ķ�ջ�����ô�����ô�����ʼ
				//���TempCHead�ǵ�һ����ջ��ڵ㣬��ô��ǰ�沢û���Ѿ�����λ�ķ�ջ��������Թ���ԭ����Ϊ��һ����ջ�����ֹ��

				if (NULL == pTempCHead->prevGeomcloseNode)//��ͷ���
				{
					if (!(pFadClosedHead->prevGeomcloseNode))//���pNKidCloseHead�ǵ�һ���ջ��ĵ�һ���ӷ�ջ�����ôǰ��û�з�ջ�
					{
						pre_x = 0.0;
						pre_y = 0.0;
					}
					else//����͸��ҵ���һ����յ���ʼ��
					{
						pre_x = pFadClosedHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
						pre_y = pFadClosedHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
					}

				}
				else//��ôҪ�ҵ����һ������λ�ķ�ջ�ͷ���
				{
					pre_x = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
					pre_y = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				}
				//�����Ѿ��ҵ���һ����յ���ֹ���ˡ�
				//����Ҫ�Ը���ֹ�����������еķ�ջ��ڵ㣬�ҵ��䵽��һ����ջ���ֹ����̾�����Ǹ��ڵ�ͷ�ջ�
				//Ҫ��ʣ�µķ�ջ����б���
				temp_x = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
				temp_y = pTempCHead->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				//��ԭ��ľ���
				m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//�������������֮�����̾���
				//������ջ�֮��ľ���
				m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//�������������֮�����̾���
				//����̾���,��ʱ����̾���
				newNode->m_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
				//newNode->m_mindistant = 1 / newNode->m_mindistant;
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
				//�����Ѿ��ҵ����������������λ�ķ�ջ���ͷ����ȫ������
				//����Ҫ�����ǻ��ڷ�ջ�ͷ����ͼԪ�ڵ����˫���������ݵ���
				//���ǰ�ͼԪ�ڵ����Ϊ��ջ���ͷ���
				GeomEleNode*phtemp, *ptemp;
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
					pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode = newNode->pminhead;//��������Ϊ�ӷ�ջ���ͷ�ڵ�
					newNode->pminhead->prevOutsideGCloseNode = pNKidCloseHead->prevOutsideGCloseNode;
					newNode->pminhead->m_KidAcceptGeomCloseHead = true;//�Ӵ˸ýڵ�ͱ��̶���
				}
				//newNode->pminhead��ǰ��պ��Ǳ���λ��
				//�������һ�����������ǡ��pLastAcceptCHead=newNode->pminhead;��˱��ܽ���������
				else if (newNode->pminhead->prevGeomcloseNode->m_KidAcceptGeomCloseHead)
				{
					//��ôʲô�����ö�
					newNode->pminhead->m_KidAcceptGeomCloseHead = true;//�Ӵ˸ýڵ�ͱ��̶���

				}
				else//˵��newNode->pminnode����ͷ����ˣ���ô����ڵ�Ҫ�ҵ�Ŀǰ���һ��m_AcceptGeomcloseHEADû�б�����Ϊtrue�ķ�ջ��ڵ����
				{

					//���������Ѱ�ң�����pLastAcceptCHead�����һ������λ�Ľڵ㣬��newNode->pminhead����Ҫ�ҵ�pLastAcceptCHead����
					//������ÿ�ε���˫������Ľڵ�λ��ʱ�򣬶�Ҫ���Ǹýڵ��ǲ���ͷβ����һ��ڵ�ģ���������Բ��ÿ���ͷ�����
					//��ôҪ���ҵ����һ��û����λ�ķ�ջ�������ͷ���Ҳ����������ǰ�����˱���λ�ķ�ջ�
					pLastAcceptCHead = pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode;
					while (pLastAcceptCHead->m_KidAcceptGeomCloseHead)
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
						pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode = newNode->pminhead;
						newNode->pminhead->prevOutsideGCloseNode = pNKidCloseHead->prevOutsideGCloseNode;
						newNode->pminhead->prevGeomcloseNode = NULL;//�׽��
						newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
						pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
						newNode->pminhead->m_KidAcceptGeomCloseHead = true;//��λ
					}
					//��һ�������newNode->pminhead������pLastAcceptCHead���
					else//��ô��newNode->pminheadֱ�ӹҵ�pLastAcceptCHead����ȥ
					{
						//��ҪpLastAcceptCHead�������Ѿ�����λ�ģ����ǵ�һ��û����λ�ģ�pLastAcceptCHead->prevGeomcloseNode�������һ������λ��
						newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
						newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
						pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
						pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
						newNode->pminhead->m_KidAcceptGeomCloseHead = true;//��λ
					}
				}
				pTempCHead = newNode->pminhead;//�������ѭ��
			}
			b;
			if (!(pTempCHead->nextGeomcloseNode))//˵���������һ���ӷ�ջ�
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
//���Ĵ��룡
//���Ĵ��룡
//���Ĵ��룡
//������ͼ�ķ�ջ��Ƿ�Ƕ��ȫ���ҳ������������Ƿ�Ƕ�׵Ĺ�ϵ�ҵ���ͬ�ķ�ջ�ͷ�����
void GeomForCut::Find_AdjustNestCloseHead(NestResultDataNode*head)
{
	int a = 0;//��bugʹ��
	GeomCloseHEAD*pTempCHead, *pTempNextCHead;//��������ȫ����ջ�
	GeomCloseHEAD*pNextCHead;//��ΪpTempNextCHead�������pTempCHead���ӷ�ջ���Ҫ�뿪���ջ��Ľ��ӻ�
	m_G2CloseHeadNest.m_IfCloseNest= true;//�жϵڶ�����ջ��ǲ��ǵ�һ����ջ����ӷ�ջ�
	//�㷨��ʼ
	pTempCHead = head->FirstGeomClose;
	//��Ҫȫ������һ�飬��ֻ�����һ����ջ���ʱ��Ϳ�������ȥ�ˣ���ΪҪǶ������Ҫ������ջ�
	//��while (pTempCHead->nextGeomcloseNode)��Ȼ����ԭ������pTempCHead֮ǰ�ĺ�һ�����ڱ���������ӷ�ջ�����ô�ͻ���pTempCHead������һ��
	//��ô���ͳ������һ������ô���������pTempCHead = pTempCHead->nextGeomcloseNode;
	//��ʱpTempCHead�Ѿ���NULL������pTempCHead->nextGeomcloseNode�ǻᱨ���
	//����Ҫ��Ϊwhile (pTempCHead)
	//pTempCHead->m_Singlelayer = true;//�����800*800�İ�ģ��ǵ�һ��Ҳ���ǵ���
	while (pTempCHead)//�����ڶ������Խ���ѭ�������һ����Ҫ����ѭ����
	{
 		pNextCHead = head->FirstGeomClose;
		//���ǰ���ÿһ�����Ǵ󻷵ļ�����ж����µķ�ջ������Ų�
		while (pNextCHead)//������һ��
		{
			//��pNextCHead��һ������pTempNextCHead,
			a++;
			pTempNextCHead = pNextCHead;
			////////////�����ߵķ�ʽ�ж���������ջ���û��Ƕ��
			////////m_G2CloseHeadNest = EstimateCloseHeadNest(pTempCHead, pTempNextCHead);
			//�ð���ķ�ʽ���ж�������ջ���û��Ƕ��
			m_G2CloseHeadNest = JudgeCloseHeadNest(pTempCHead, pTempNextCHead);
			//�����Ƕ�ף���ô��Ҫ�ѵڶ�����ջ��ҵ���һ�������
			if (m_G2CloseHeadNest.m_IfCloseNest)
			{
				//�ȱ�����һ����ջ���λ��
				pNextCHead = m_G2CloseHeadNest.NextCloseHead;//����ĺ����Ѿ���������е� pNextCHead->nextGeomcloseNode���ƵĹ���
				//pTempNextCHead = m_G2CloseHeadNest.KidCloseHead;
				//�ѵڶ�����ջ����ҵ���һ�������
				SetInSideClose(pTempCHead, m_G2CloseHeadNest, head);

				//pNextCHead = m_G2CloseHeadNest.NextCloseHead;//����ĺ����Ѿ���������е� pNextCHead->nextGeomcloseNode���ƵĹ���
			}
			else//�������û��Ƕ�ף���ô���ڵ�pNextCHead��û������仯�������Լ����ҵ�ԭ������һ��
			{
				pNextCHead = pNextCHead->nextGeomcloseNode;

			}
		}	
		
		
		pTempCHead = pTempCHead->nextGeomcloseNode;
	}
}//���ϴ���ʵ���ˣ���˫���жϺ��������εĿ��Ƶ��¼
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
////���Ĵ��룡
////���Ĵ��룡
////���Ĵ��룡
////��pHNtemp�ҵ�pHtemp���ӷ�ջ���
////����������㷨������������ͬ�Ĺ��̡�
//void  GeomForCut::SetInSideClose(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp, NestResultDataNode*head)
//{
//	
//	GeomCloseHEAD*pKidHtemp=NULL;
//	GeomCloseHEAD*pKidLastClose = NULL;//�ӷ�ջ����ڵķ�ջ�˫�������е����һ����ջ�����
//	bool m_IfCloseNest = false;//�жϵڶ�����ջ��ǲ��ǵ�һ����ջ����ӷ�ջ�
//	//��Ҫ�ж�pHtemp��û���ӷ�ջ�������еĻ��ֵ�����pHNtemp��ԭ�����ӷ�ջ���û����Ƕ�Ĺ�ϵ
//	if (!(pHtemp->FirstInsideGCloseNode))//���ԭ����û���ӷ�ջ�
//	{
//		pHtemp->FirstInsideGCloseNode = pHNtemp;
//		//ͬʱ��Ҫ��pHNtemp���˫�������ǰ�����ָ�����
//		//����Ҫע��pHNtemp�����һ����ջ�����������������ǵ�һ����ջ���Ҳ�п����ǵ�һ����ջ��ģ�������
//		//Ҫ�������׷�ջ���β��ջ�ͬʱ����һ����ջ������
//		if (pHNtemp->nextGeomcloseNode)//���pHNtemp�������ķ�ջ�
//		{
//			//���pHNtemp���׷�ջ�����ô����һ����ջ���ǰ�ڵ���λΪnull
//			if (!(pHNtemp->prevGeomcloseNode))
//			{
//				//���и�ͼ��ջ�ͷ��㻻�ɵڶ�����ջ�
//				head->FirstGeomClose = pHNtemp->nextGeomcloseNode;
//				//�ٵ�����Ϊ�ӷ�ջ���pHNtemp��ԭ����ջ����еĹ�ϵ��
//				pHNtemp->nextGeomcloseNode->prevGeomcloseNode = NULL;
//
//			}
//			else//���pHNtemp�����׷�ջ���Ҳ����β��ջ��Ļ�����ô����һ����ջ���ǰ�ڵ���λΪpHNtemp��ǰ�ڵ㡣
//			{
//				pHNtemp->prevGeomcloseNode->nextGeomcloseNode = pHNtemp->nextGeomcloseNode;//pHNtemp��ǰ��ĺ���ԭ����pHNtemp�������ڸ������ĺ�����Ǹ���
//				pHNtemp->nextGeomcloseNode->prevGeomcloseNode = pHNtemp->prevGeomcloseNode;//pHNtemp�ĺ����ǰ��ԭ����pHNtemp�������ڸ�������ǰ����Ǹ���
//
//			}
//		}
//		//���pHNtemp�����ķ�ջ��ڵ�������
//		//Ҫע�����������ֻ��һ����ջ������
//		else
//		{
//			if (!(pHNtemp->prevGeomcloseNode))//���pHNtempͬʱҲ���׷�ջ���ʱ��
//			{
//				pHNtemp->nextGeomcloseNode = NULL;//ֱ����λΪNULL
//			}
//			else
//			{
//				pHNtemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
//
//			}
//		}
//		//��������֮��pHNtemp�͸ð�ǰ���ΪNULL�ˡ�
//		pHNtemp->prevGeomcloseNode =pHNtemp->nextGeomcloseNode = NULL;
//		//ָ���丸�ڵ�
//		pHNtemp->prevOutsideGCloseNode = pHtemp;
//	}
//	else//�������pHtemp�Ѿ������ӷ�ջ��ˣ���ôҪ�ж����ڵ��ӷ�ջ���ԭ�����ӷ�ջ���û��Ƕ�׹�ϵ
//	{
//		pKidHtemp = pHtemp->FirstInsideGCloseNode;
//		//��Ҫȫ������һ�飬��ֻ�����һ����ջ���ʱ��Ϳ�������ȥ�ˣ���ΪҪǶ������Ҫ������ջ�
//		while (pKidHtemp)//�����е��ӷ�ջ�����һ��
//		{
//			//���ǰ���ÿһ�����Ǵ󻷵ļ�����й滮
//			//������������ջ�ͷ����ж��Ƿ���Ƕ�׹�ϵ
//			//////////�ж���������ջ���û��Ƕ��
//			////////m_G2CloseHeadNest = EstimateCloseHeadNest(pKidHtemp, pHNtemp);
//			//�ð���ķ�ʽ���ж�������ջ���û��Ƕ��
//			m_G2CloseHeadNest = Judge2KidCloseHNest(pKidHtemp, pHNtemp);
//
//			//�����Ƕ�ף���ô��Ҫ�ѵڶ�����ջ��ҵ���һ�������
//			if (m_G2CloseHeadNest.m_IfCloseNest)
//			{
//				//�ѵڶ�����ջ����ҵ���һ�������
//				//pHNtemp = m_G2CloseHeadNest.KidCloseHead;
//				SetInSideClose(pHtemp,m_G2CloseHeadNest, head);
//			}
//			else//���û��Ƕ�ף�
//			{
//				//����Ҫ��pHNtempԭ���Ĺ�ϵ���н���
//				//ע�⾭�����ε���֮���κ�һ����ջ������ܳ�Ϊ��һ����ջ�������Ҫ�������ǲ��ǵ�һ����ջ�
//				if (!(pHNtemp->prevGeomcloseNode))//���׽��������
//				{
//					//���и�ͼ��ջ�ͷ��㻻�ɵڶ�����ջ�
//					head->FirstGeomClose = pHNtemp->nextGeomcloseNode;
//					//�ٵ�����Ϊ�ӷ�ջ���pHNtemp��ԭ����ջ����еĹ�ϵ��
//					pHNtemp->nextGeomcloseNode->prevGeomcloseNode = NULL;
//				}
//				//����Ҫע��pHNtemp�����һ����ջ�����������������ǵ�һ����ջ�
//				else if (pHNtemp->nextGeomcloseNode)//���pHNtemp�������ķ�ջ�
//				{
//					pHNtemp->prevGeomcloseNode->nextGeomcloseNode = pHNtemp->nextGeomcloseNode;//pHNtemp��ǰ��ĺ���ԭ����pHNtemp�������ڸ������ĺ�����Ǹ���
//					pHNtemp->nextGeomcloseNode->prevGeomcloseNode = pHNtemp->prevGeomcloseNode;//pHNtemp�ĺ����ǰ��ԭ����pHNtemp�������ڸ�������ǰ����Ǹ���
//				}
//				else if (!(pHNtemp->nextGeomcloseNode))//���pHNtemp�����ķ�ջ��ڵ�������
//				{
//					pHNtemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
//				}
//				//���з�ջ��Ĺ�ϵ
//				//��ô��Ҫ��pKidHtemp���ڵ��ӷ�ջ�˫�������ջ��ˣ����û������ֱ�Ӱ�pHNtemp�ҵ�pKidHtemp����ͺ���
//				if (!(pKidHtemp->nextGeomcloseNode))//�������û��������ջ���
//				{
//					pKidHtemp->nextGeomcloseNode = pHNtemp;//pKidHtemp��Ϊ�½�����һ������ķ�ջ������Ѿ���ԭ���Ĺ�ϵ��������֮�󶪵��ˣ������û����ݵ��ˣ���ʱǰ��ڵ㶼��NULL
//					//��������֮��pHNtemp�͸ð�����ӷ�ջ�����Ĺ�ϵ��������
//					pHNtemp->prevGeomcloseNode = pKidHtemp;
//					pHNtemp->nextGeomcloseNode = NULL;
//				}
//				else//pKidHtemp���ڵ��ӷ�ջ�˫�������ջ�������������ջ�
//				{
//					//��Ȼ���滹��������ջ�����ô�п����Ǹ�������ķ�ջ�Ƕ�ף������Ǹ�ͷ����ջ�
//					//�ж�pHNtemp��pKidHtemp����ķ�ջ��Ƿ񹹳�Ƕ�׹�ϵ�����������ôҪ�ҵ����棬
//					//��Ҫȫ������һ�飬��ֻ�����һ����ջ���ʱ��Ϳ�������ȥ�ˣ���ΪҪǶ������Ҫ������ջ�
//					pKidHtemp = pKidHtemp->nextGeomcloseNode;
//					while (pKidHtemp)//���ӷ�ջ��������µĲ��еķ�ջ�����һ��
//					{
//						//�ð���ķ�ʽ���ж�������ջ���û��Ƕ��
//						//�����и����⣬������ڶ��ذ��������£�û�а취���pHNtemp��
//						//pKidHtemp��һ���ջ�ʱ����жϣ���������㷨�Ὣ�����ջ�ת��Ϊ�Ӳ��ջ��ķ�ջ���
//						m_G2CloseHeadNest = Judge2KidCloseHNest(pKidHtemp, pHNtemp);
//
//						//�����Ƕ�ף���ô��Ҫ�ѵڶ�����ջ��ҵ���һ�������
//						if (m_G2CloseHeadNest.m_IfCloseNest)
//						{
//							//�ѵڶ�����ջ����ҵ���һ�������
//							//pHNtemp = m_G2CloseHeadNest.KidCloseHead;
//							SetInSideClose(pHtemp, m_G2CloseHeadNest, head);
//							m_IfCloseNest = true;//˵���Ѿ�Ƕ�׽�ȥ��
//							break;
//						}
//						//�����ҵ����һ����Ȼ���pHNtemp�ҵ������
//						if (!(pKidHtemp->nextGeomcloseNode))//���pKidHtemp�����һ��
//						{
//							pKidLastClose = pKidHtemp;//�������һ����ջ����ᵽ������˵�������һ����ջ���pHNtempҲû��Ƕ�׵Ĺ�ϵ
//							m_IfCloseNest = false;
//						}
//						pKidHtemp = pKidHtemp->nextGeomcloseNode;
//					}			
//					if (!(m_IfCloseNest))
//					{
//						pKidLastClose->nextGeomcloseNode = pHNtemp;
//						//��������֮��pHNtemp�͸ð�����ӷ�ջ�����Ĺ�ϵ��������
//						pHNtemp->prevGeomcloseNode = pKidLastClose;
//						pHNtemp->nextGeomcloseNode = NULL;
//					}
//					
//				}
//				
//			}
//			//���whileѭ������Ҫ�ٴ������������ȥѭ������Ϊ����һ���Եݹ�ĺ�������ȥ�������elseʱ������while��ʵӦ����Ҫֹͣ��
//			//pKidHtemp = pKidHtemp->nextGeomcloseNode;
//			//���������Ĵ��룬����ʹ��pKidHtemp����Լ�ѭ��һ��
//			break;
//		}
//	}
//}

//���ʶ���������ӷ�ջ���Ƕ�׹�ϵ�ķ�ջ���ԭ����ջ�˫�������еĹ�ϵ�������ɾ��ؽ����µķ�ջ�˫�������С�
//һ��ֻ�������¼��ֹ�ϵ����һ���и�ͼ��ջ�ͷ��㣬�����һ��ڵ㣬������β�ڵ㡣�������֡�
//��Ҫע������������п������ӷ�ջ�����ô��Ҫ�������ӹ�ϵ���ֵܷ�ջ���ϵ������������ص㡣
//������������һ��Ϊʶ���ĸ���ջ����ڶ���Ϊʶ�����ӷ�ջ���������Ϊƽ���и�ͼͷ��㣬ֻ��ʶ�����ӷ�ջ�����
void GeomForCut::ClearCloseHeadOldRela(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp, NestResultDataNode*head)
{
	int a = 0;
	//�ж�pHNtemp�ǲ����Ѿ����������,��������ָ����NULL
	if ((pHNtemp->prevGeomcloseNode == NULL) && (pHNtemp->nextGeomcloseNode == NULL) && (pHNtemp->prevOutsideGCloseNode == NULL))//ע��pHNtemp->FirstInsideGCloseNode�ǲ���Ҫ�ж��Ƿ�ΪNULL�ģ���Ϊ�������漰���ӷ�ջ���ָ��
	{
		a++;
	}
	else//���û�б����������ô��Ҫ����
	{
		//�����ֵܷ�ջ�˫������
		//��pHNtemp�Ƿ�ջ��׽��ʱ��
		if (!(pHNtemp->prevGeomcloseNode))//�ܽ���˵�����׽�㣬Ҳ�п����Ǹ���ջ��ĵ�һ�ӷ�ջ�
		{
			//Ҫ�ж��������и�ƽ��ͼ���׽�㻹���ӷ�ջ�˫�������е��׽��
			//����������и�ƽ��ͼ���׽��
			if (pHNtemp == head->FirstGeomClose)
			{
				//���и�ͼ��ջ�ͷ��㻻�ɵڶ�����ջ�
				head->FirstGeomClose = pHNtemp->nextGeomcloseNode;
				//�ٵ�����Ϊ�ӷ�ջ���pHNtemp��ԭ����ջ����еĹ�ϵ��
				pHNtemp->nextGeomcloseNode->prevGeomcloseNode = NULL;
			}
			else//�������ӷ�ջ�˫�������е��׽��
			{
				//��Ҫ�ж��ǲ���ֻ��һ���ӷ�ջ������˫��������
				//ֻ��һ������ô����ͬʱҲ��β��ջ�
				if (!(pHNtemp->nextGeomcloseNode))
				{
					pHNtemp->nextGeomcloseNode = pHNtemp->prevGeomcloseNode = NULL;
				}
				else//���������ֵܷ�ջ��ڵ�
				{
					pHNtemp->nextGeomcloseNode->prevGeomcloseNode = NULL;
				}
			}
		}
			//��pHNtemp��β��ջ�ʱ��ǰ���Ѿ�������ͬʱ����β��ջ�������������ֻ�ô���򵥵�β�ڵ㣩
		else if (!(pHNtemp->nextGeomcloseNode))
		{
			pHNtemp->prevGeomcloseNode->nextGeomcloseNode = NULL;
		}
		else//�����׽�㣬����β�ڵ㣬��һ��ڵ�
		{
			pHNtemp->prevGeomcloseNode->nextGeomcloseNode = pHNtemp->nextGeomcloseNode;//pHNtemp��ǰ��ĺ���ԭ����pHNtemp�������ڸ������ĺ�����Ǹ���
			pHNtemp->nextGeomcloseNode->prevGeomcloseNode = pHNtemp->prevGeomcloseNode;//pHNtemp�ĺ����ǰ��ԭ����pHNtemp�������ڸ�������ǰ����Ǹ���
		}
		//�����ӷ�ջ�˫������
		//��pHNtemp�����ӷ�ջ�
		if (pHNtemp->prevOutsideGCloseNode)//�ܽ���˵�������ӷ�ջ�����prevOutsideGCloseNodeָ��ΪNULL
		{
			//������ջ�ָ������ӷ�ջ���ΪpHtemp
			pHNtemp->prevOutsideGCloseNode->FirstInsideGCloseNode = pHtemp;
			pHtemp->prevOutsideGCloseNode = pHNtemp->prevOutsideGCloseNode;
		}
		else//�������ӷ�ջ�ʱ,��ô���ӷ�ջ�˫������ָ��Ͳ���Ҫ�ı��ˡ�
		{
			;
		}
		//��������֮��pHNtemp�͸ð�ǰ���ΪNULL�ˡ�
		pHNtemp->prevGeomcloseNode = pHNtemp->nextGeomcloseNode = pHNtemp->prevOutsideGCloseNode = NULL;
		
	}
}

//���Ĵ��룬��Ƕ�׷�ջ��ֱ�ҵ����ʵ�λ����ȥ��
//��ջ�����ż���Ѿ����ж�ʱ��������ˡ�
//����ĸ���˼���ǣ�����ջ�ԭ����˫�������еĹ�ϵ����ɾ���Ȼ������µĹ�ϵ��˫�������д�µĹ�ϵ��
void  GeomForCut::SetInSideClose(GeomCloseHEAD*pFHtemp, Geom2CloseHeadNest m_G2CloseHeadNest, NestResultDataNode*head)
{
	Geom2CloseHeadNest m_G2CloseHeadNest_Local;//ֻ�ڱ������е��õı���
	GeomCloseHEAD*pKidHtemp = NULL;
	GeomCloseHEAD*pKidLastClose = NULL;//�ӷ�ջ����ڵķ�ջ�˫�������е����һ����ջ�����
	GeomCloseHEAD*pKidNHtemp = NULL;//����case2�ϣ����������ӷ�ջ����ֵܷ�ջ�
	GeomCloseHEAD*pHtemp;//�����жϺ����ɵĸ���ջ�
	GeomCloseHEAD*pHNtemp;//�����жϺ����ɵ��ӷ�ջ�
	bool m_IfCloseNest = false;//�жϵڶ�����ջ��ǲ��ǵ�һ����ջ����ӷ�ջ�
	//��Ϊ���ڽ�������ջ�֮��Ĺ�ϵ�������3�����������Ҫ�õ�switch case���ж���䡣
	switch (m_G2CloseHeadNest.type)
	{
	case 1://��������������ǰ��ķ�ջ����Ǻ����ջ��ĸ���ջ�
		  //��Ϊԭ��Ƕ�׷�ջ�ʶ��֮��ҿ������������㷨������ֻ������һ�֡�
		 //��Ҫ�ж�pHtemp��û���ӷ�ջ�������еĻ��ֵ�����pHNtemp��ԭ�����ӷ�ջ���û����Ƕ�Ĺ�ϵ
		pHtemp = pFHtemp;
		pHNtemp = m_G2CloseHeadNest.KidCloseHead;//������ö��ѡ�
		//ͬʱ��Ҫ��pHNtemp���˫�������ǰ�����ָ�����
		ClearCloseHeadOldRela(pHtemp,pHNtemp, head);
		//�����ǽ�pHNtempԭ����ջ�˫�������еĹ�ϵ������������ܸɾ������µķ�ջ��
		if (!(pHtemp->FirstInsideGCloseNode))//���ԭ����û���ӷ�ջ�
		{
			pHtemp->FirstInsideGCloseNode = pHNtemp;
			//ָ���丸�ڵ�
			pHNtemp->prevOutsideGCloseNode = pHtemp;
		}
		else//�������pHtemp�Ѿ������ӷ�ջ��ˣ���ôҪ�ж����ڵ��ӷ�ջ���ԭ�����ӷ�ջ���û��Ƕ�׹�ϵ����ʱ���õ���Judge2KidCloseHNest�����㷨��Ҫ����ǰ��˭�Ǹ��ӵĹ�ϵ��������ֻ���Ǻ����ǲ���ǰ�ߵ��ӷ�ջ������⡣
		{
			pKidHtemp = pHtemp->FirstInsideGCloseNode;
			//��Ҫȫ������һ�飬��ֻ�����һ����ջ���ʱ��Ϳ�������ȥ�ˣ���ΪҪǶ������Ҫ������ջ�
			//while (pKidHtemp)//�����е��ӷ�ջ�����һ��
			//{
				//�ð���ķ�ʽ���ж�������ջ���û��Ƕ�ף�����Ҫ��ϸ�жϡ�
				m_G2CloseHeadNest_Local = Judge2KidCloseHNest(pKidHtemp, pHNtemp);
				
				//�����Ƕ�ף���ô��Ҫ�ѵڶ�����ջ��ҵ���һ�������
				if (m_G2CloseHeadNest_Local.m_IfCloseNest)
				{
					//�ѵڶ�����ջ����ҵ���һ�������
					//pHNtemp = m_G2CloseHeadNest.KidCloseHead;
					SetInSideClose(pKidHtemp, m_G2CloseHeadNest_Local, head);//��������һֱ���ӷ�ջ��ҵ���ײ�
					break;
				}
				else//���û��Ƕ�ף�
				{
					//�Ǿ��ǲ��з�ջ��Ĺ�ϵ
					//��ô��Ҫ��pKidHtemp���ڵ��ӷ�ջ�˫�������ջ��ˣ����û������ֱ�Ӱ�pHNtemp�ҵ�pKidHtemp����ͺ���
					if (!(pKidHtemp->nextGeomcloseNode))//�������û��������ջ���
					{
						pKidHtemp->nextGeomcloseNode = pHNtemp;//pKidHtemp��Ϊ�½�����һ������ķ�ջ������Ѿ���ԭ���Ĺ�ϵ��������֮�󶪵��ˣ������û����ݵ��ˣ���ʱǰ��ڵ㶼��NULL
						//��������֮��pHNtemp�͸ð�����ӷ�ջ�����Ĺ�ϵ��������
						pHNtemp->prevGeomcloseNode = pKidHtemp;
						pHNtemp->nextGeomcloseNode = NULL;
						break;
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
							//�����и����⣬������ڶ��ذ��������£�û�а취���pHNtemp��
							//pKidHtemp��һ���ջ�ʱ����жϣ���������㷨�Ὣ�����ջ�ת��Ϊ�Ӳ��ջ��ķ�ջ���
							m_G2CloseHeadNest_Local = Judge2KidCloseHNest(pKidHtemp, pHNtemp);

							//�����Ƕ�ף���ô��Ҫ�ѵڶ�����ջ��ҵ���һ�������
							if (m_G2CloseHeadNest_Local.m_IfCloseNest)
							{
								//�ѵڶ�����ջ����ҵ���һ�������
								//pHNtemp = m_G2CloseHeadNest.KidCloseHead;
								SetInSideClose(pKidHtemp, m_G2CloseHeadNest_Local, head);
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
						if (!(m_IfCloseNest))//Ҫ������־λҪ�ж�ʲôʱ������whileѭ��
						{
							pKidLastClose->nextGeomcloseNode = pHNtemp;
							//��������֮��pHNtemp�͸ð�����ӷ�ջ�����Ĺ�ϵ��������
							pHNtemp->prevGeomcloseNode = pKidLastClose;
							pHNtemp->nextGeomcloseNode = NULL;
						}

					}

				}
			//}
		}
		break;
		//ע�⣬�ܽ��뵽case 2˵���ǽ�����ջ���ԭ���ĺ��ջ����Ѿ������һ���ˣ�������Ҫ�������ԭ����ǰ��ջ����ӷ�ջ���
	case 2://������ķ�ջ�����ǰ��ķ�ջ��ĸ���ջ�����ʱҪ���洦���������
		   //��ʱҪ������������һ�ǣ�ǰ���ջ��Ǹ���ջ�ָ��ĵ�һ���ӷ�ջ���������ǣ�ǰ���ջ��Ǹ���ջ����ӷ�ջ�˫�������е�����һ����
		pHtemp = m_G2CloseHeadNest.FadCloseHead;//����ԭ����ջ��еĺ����Ǹ���ջ�
		pHNtemp = m_G2CloseHeadNest.KidCloseHead;//����ԭ����ջ��е�ǰ���Ǹ���ջ�
		pKidNHtemp = pHNtemp;
		//ͬʱ��Ҫ��pHNtemp���˫�������ǰ�����ָ����ģ�pHtempΪ����ķ�ջ�
		ClearCloseHeadOldRela(pHtemp, pHNtemp, head);
		//�����ǽ�pHNtempԭ����ջ�˫�������еĹ�ϵ������������ܸɾ������µķ�ջ��
		//����ֻ�ÿ��ǽ����ӷ�ջ��Ĺҿ����¹Ҿͺ��ˡ�
		if (!(pHtemp->FirstInsideGCloseNode))//pHtempԭ��û���ӷ�ջ�ʱ��
		{
			pHtemp->FirstInsideGCloseNode = pHNtemp;
			pHNtemp->prevOutsideGCloseNode = pHtemp;
		}
		else//���pHtemp�Ѿ����ӷ�ջ��ˣ���ô��Ӧ�����ҵ�����Ǹ��ӷ�ջ�����ȥ
		{
			pKidLastClose = pHtemp->FirstInsideGCloseNode;
			while (pKidLastClose)//�ҵ����һ����ջ�
			{
				pKidLastClose = pKidLastClose->nextGeomcloseNode;
			}
			pKidLastClose->nextGeomcloseNode = pHNtemp;
			pHNtemp->prevGeomcloseNode = pKidLastClose;
		}
		//����ֻ�ǽ���˺����ջ���ǰ���ջ��ĸ���ջ�������������ǰ���ӷ�ջ����ֵܷ�ջ���Ҳ�к����ջ����ӷ�ջ�����ô���ü������ҡ�
		if (pKidNHtemp->nextGeomcloseNode)//ԭ�����ӷ�ջ������ֵܷ�ջ�
		{
			pKidNHtemp = pKidNHtemp->nextGeomcloseNode;
			while (pKidNHtemp)//Ҫ�����µ��ֵܷ�ջ���һ��
			{
				//��Ϊ�����Ѿ�������ȷ������ķ�ջ���ǰ���ջ��ĸ���ջ�����ô����Ҫ�һ���û���ӷ�ջ������Ծ��жϺ����ǲ���ǰ�ߵ��ӷ�ջ��ͺá�
				m_G2CloseHeadNest_Local = JudgeCloseHeadNest(pHtemp, pKidNHtemp);
				//�����Ƕ�ף���ô��Ҫ�ҵ����ʵط�
				if (m_G2CloseHeadNest_Local.m_IfCloseNest)
				{
					//�ѵڶ�����ջ����ҵ���һ�������
					//pHNtemp = m_G2CloseHeadNest.KidCloseHead;
					SetInSideClose(pHtemp, m_G2CloseHeadNest_Local, head);
				}
				else//���û��Ƕ���ˣ���ô�ͼ���ѭ����
					pKidNHtemp = pKidNHtemp->nextGeomcloseNode;
			}
				pKidNHtemp = pKidNHtemp->nextGeomcloseNode;
			}
		else//���ǰ����ӷ�ջ�û�����ֵܷ�ջ�����ô����жϽ����ˡ�
		{
			;
		}
		break;
	case 3:
			break;
	default:
		break;
	}
}
//�����з�ջ��İ�����εĿ��ƽǵ�
void GeomForCut::GetLimtofGeomClosed(NestResultDataNode*head)
{
	GeomCloseHEAD*pTempCHead, *pFirstCH;
	pFirstCH = head->FirstGeomClose;
	//ȫ������
	while (pFirstCH)
	{
		GetLimtofGeomClosed(pFirstCH);
		pFirstCH = pFirstCH->nextGeomcloseNode;
	}
}
//���ջ��İ������xy�ļ���ֵ
void  GeomForCut::GetLimtofGeomClosed(GeomCloseHEAD*pTempCHead)
{
	GeomEleNode*ptempGelenode;
	double temp_x_min=0.0, temp_x_max=0.0, temp_y_min=0.0, temp_y_max=0.0;
	double x_min=0.0, x_max=0.0, y_min=0.0, y_max=0.0;//Ѱ�ҷ�ջ��İ�����εķ�Χ
	double x_min_y = 0.0, x_max_y = 0.0, y_min_x = 0.0, y_max_x = 0.0;//Ѱ�ҷ�ջ��İ�����εķ�Χ����ڵĿ��ƽǵ���һ������
	double c_x, c_y, c_r;//����Բ���͵�Բ����뾶
	//�ӵ�һ����ջ��ĵ�һ��ͼԪ�ڵ㿪ʼ
	//Ҫ��һ����ջ��������Σ���Բ��������
	ptempGelenode = pTempCHead->FirstGeomele;
	//��������ֱ����Բ������������£���ô�Ϳ��ܻ���ִ�������û�����������
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
					x_min_y = temp_y_min;//����Ǹ�ӹ
				}
				else
				{
					x_min = temp_x_max;
					x_min_y = temp_y_max;//����Ǹ�ӹ
				}

			}
			if ((x_max <= temp_x_min) || (x_max <= temp_x_max))
			{
				if (x_max <= temp_x_max)
				{
					x_max = temp_x_max;
					x_max_y = temp_y_max;//����Ǹ�ӹ
				}
				else
				{
					x_max = temp_x_min;
					x_max_y = temp_y_min;//����Ǹ�ӹ
				}
			}
			//y��limit
			if ((temp_y_min <= y_min) || (temp_y_max <= y_min))
			{
				if (temp_y_min <= temp_y_max)
				{
					y_min = temp_y_min;
					y_min_x = temp_x_min;
				}
				else
				{
					y_min = temp_y_max;
					y_min_x = temp_x_max;
				}

			}
			if ((y_max <= temp_y_min) || (y_max <= temp_y_max))
			{
				if (y_max <= temp_y_max)
				{
					y_max = temp_y_max;
					y_max_x = temp_x_max;
				}
				else
				{
					y_max = temp_y_min;
					y_max_x = temp_x_min;
				}
			}
			ptempGelenode = ptempGelenode->nextGeomeleNode;
		}
	}
	else//�����Բ������£���ô�Ͳ��ù�Բ�ĵ�ˮƽ�ߺʹ�ֱ����Բ�Ľ��������ֵ
	{
		c_x = ptempGelenode->m_GeomStandData.m_circle.m_Circent_x;
		c_y = ptempGelenode->m_GeomStandData.m_circle.m_Circent_y;
		c_r = ptempGelenode->m_GeomStandData.m_circle.m_Circle_r;
		x_min = c_x - c_r;
		x_min_y = c_y;
		x_max = c_x + c_r;
		x_max_y = c_y;
		y_min = c_y - c_r;
		y_min_x = c_x;
		y_max = c_y + c_r;
		y_max_x = c_x;
	}
	//����������ֵ������λС��
	x_min = Retain4Decimals(x_min);
	x_max = Retain4Decimals(x_max);
	y_min = Retain4Decimals(y_min);
	y_max = Retain4Decimals(y_max);
	x_min_y = Retain4Decimals(x_min_y);
	x_max_y = Retain4Decimals(x_max_y);
	y_min_x = Retain4Decimals(y_min_x);
	y_max_x = Retain4Decimals(y_max_x);
	
	//�����ǽ����η�ջ����ĸ����ƽǵ������ֵ��������
	//Բ�εĻ�û�д���
	pTempCHead->m_GemoClosedLimtPoint.x_min.x = x_min;
	pTempCHead->m_GemoClosedLimtPoint.x_min.y = x_min_y;
	pTempCHead->m_GemoClosedLimtPoint.x_max.x = x_max;
	pTempCHead->m_GemoClosedLimtPoint.x_max.y = x_max_y;
	pTempCHead->m_GemoClosedLimtPoint.y_min.y = y_min;
	pTempCHead->m_GemoClosedLimtPoint.y_min.x = y_min_x;
	pTempCHead->m_GemoClosedLimtPoint.y_max.y = y_max;
	pTempCHead->m_GemoClosedLimtPoint.y_max.x = y_max_x;
}
//�����з�ջ����������ԣ����ڰ�ĵ���һ������� 
//�жϷ�ջ����ڵ������԰�ĵ������д��߽���Ļ���Ϊ4������������Ϊ����1������Ϊ����2������Ϊ����4������Ϊ����7�������3,6,11,8,14
void GeomForCut::GetRigionOfGeomClosed(NestResultDataNode*head)
{
	double G_x_min , G_x_mid , G_x_max , G_y_min , G_y_mid , G_y_max;//�����ĵĻ��ֳߴ磬����800��˵��
	double x_min, x_max, y_min, y_max;
	GeomCloseHEAD*PGHtemp;//��ջ�ͷ����ָ��ָ��
	GetLimtofGeomClosed(m_ceramic_Head);//���ĵķ�ջ��������
	G_x_min = m_ceramic_Head->m_GemoClosedLimtPoint.x_min.x;
	G_x_max = m_ceramic_Head->m_GemoClosedLimtPoint.x_max.x;
	G_x_mid = (G_x_min + G_x_max) / 2;
	G_y_min = m_ceramic_Head->m_GemoClosedLimtPoint.y_min.y;
	G_y_max = m_ceramic_Head->m_GemoClosedLimtPoint.y_max.y;
	G_y_mid = (G_y_min + G_y_max) / 2;
	PGHtemp = head->FirstGeomClose;
	while (PGHtemp)//ȫ������һ��
	{
		
		x_min = PGHtemp->m_GemoClosedLimtPoint.x_min.x;
		x_max = PGHtemp->m_GemoClosedLimtPoint.x_max.x;
		y_min = PGHtemp->m_GemoClosedLimtPoint.y_min.y;
		y_max = PGHtemp->m_GemoClosedLimtPoint.y_max.y;
		//����ջ��������������֣�����������������
		//ͬʱ�ҵ����������Щ��ջ�
		//��x������
		if ((G_x_min <= x_min) && (x_max <= G_x_mid))//x���ǰ�벿��
		{
			if ((G_y_min <= y_min) && (y_max <= G_y_mid))//y���°벿��
			{
				PGHtemp->m_Region = 7;
			}
			else if ((G_y_mid <= y_min) && (y_max <= G_y_max))//y���ϰ벿��
			{
				PGHtemp->m_Region = 1;
			}
			else if ((y_min <= G_y_mid) && (G_y_mid <= y_max))//y���м䲿��
			{
				PGHtemp->m_Region = 8;//8���ڿ���1��7��������
			}
			
		}
		else if ((G_x_mid <= x_min) && (x_max <= G_x_max))//x��ĺ�벿��
		{
			if ((G_y_min <= y_min) && (y_max <= G_y_mid))//y���°벿��
			{
				PGHtemp->m_Region = 4;
			}
			else if ((G_y_mid <= y_min) && (y_max <= G_y_max))//y���ϰ벿��
			{
				PGHtemp->m_Region = 2;
			}
			else if ((y_min <= G_y_mid) && (G_y_mid <= y_max))//y���м䲿��
			{
				PGHtemp->m_Region = 6;//6���ڿ���2��4��������
			}
		}
		else if ((x_min <= G_x_mid) && (G_x_mid <= x_max))//x���м䲿��
		{
			if ((G_y_min <= y_min) && (y_max <= G_y_mid))//y���°벿��
			{
				PGHtemp->m_Region = 11;//����4��7��������
			}
			else if ((G_y_mid <= y_min) && (y_max <= G_y_max))//y���ϰ벿��
			{
				PGHtemp->m_Region = 3;//����1��2��������
			}
			else if ((y_min <= G_y_mid) && (G_y_mid <= y_max))//y���м䲿��
			{
				PGHtemp->m_Region = 14;//����1��2��4��7��Щ����
			}
		}
		PGHtemp = PGHtemp->nextGeomcloseNode;
	}
	
}
//�����㷨
//�����㷨
//�����㷨
//�ð���ķ������ж�������ջ��Ƿ�Ƕ��
Geom2CloseHeadNest  GeomForCut::JudgeCloseHeadNest(GeomCloseHEAD*pTempCHead, GeomCloseHEAD*pTempNextCHead)

{
	Envelope_RectPoint m_GeomClosed1, m_GeomClosed2;
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
	////��������ջ��İ�����εķ�Χ
	//GetLimtofGeomClosed(pTempCHead);
	//GetLimtofGeomClosed(pTempNextCHead);
	m_GeomClosed1 = pTempCHead->m_GemoClosedLimtPoint;
	m_GeomClosed2 = pTempNextCHead->m_GemoClosedLimtPoint;
	//����ջ��ľ��ΰ������괫������ע�����ﲢû���õ����ƽǵ�����
	x1_min = m_GeomClosed1.x_min.x;
	x1_max = m_GeomClosed1.x_max.x;
	y1_min = m_GeomClosed1.y_min.y;
	y1_max = m_GeomClosed1.y_max.y;
	x2_min = m_GeomClosed2.x_min.x;
	x2_max = m_GeomClosed2.x_max.x;
	y2_min = m_GeomClosed2.y_min.y;
	y2_max = m_GeomClosed2.y_max.y;
	//�����Ѿ��Ѱ�����εķ�Χ�������
	//������ж����������Ƿ�Ƕ�׽����ж�
	//��Ҫ���жϺ����Ǹ���ջ��ǲ���ǰ���ջ����ӷ�ջ�
	//����Ĭ��ͬ����ջ������ֵ��min<max
	if (((x1_min <=x2_min ) && (x2_max <= x1_max)) && ((y1_min <= y2_min) && (y2_max<=y1_max)))
	{
		//����н��������£���ôӦ�ð������ջ�2�ҵ���ջ�1�У���Ϊ�����ӷ�ջ�
		m_G2CloseHeadNest.type = 1;//2��1���ӷ�ջ���
		m_G2CloseHeadNest.m_IfCloseNest = true;
		m_G2CloseHeadNest.FadCloseHead = pTempCHead;
		m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
		////���ݸ���ջ��ǵ�˫�����ж��ӷ�ջ��ĵ�˫��
		//if (pTempCHead->m_Singlelayer)//�ܽ���˵���ǵ��㣬��ΪĬ���ǵ���
		//{
		//	pTempNextCHead->m_Singlelayer = false;//��Ϊ���Ѿ����˼ҵ��ӷ�ջ����������ǵ��㣬���͵���˫��
		//}
		//else//pTempCHead->m_Singlelayer�������ջ���˫�㣬��ô�ӷ�ջ��͵��ǵ��㣬Ҳ���ǲ��ñ�
		//{
		//	pTempNextCHead->m_Singlelayer = true;//����˫���ʱ������Ӧ���ǵ�����
		//}
		m_G2CloseHeadNest.NextCloseHead = pTempNextCHead->nextGeomcloseNode;
		
	}
	else//���������Χ�ڣ�˵�����ǽ���
	{
		m_G2CloseHeadNest.type = 3;//2��1�Ĳ��߱�Ƕ�׹�ϵ��
		m_G2CloseHeadNest.m_IfCloseNest = false;
		m_G2CloseHeadNest.KidCloseHead = pTempNextCHead;
		m_G2CloseHeadNest.NextCloseHead = pTempNextCHead->nextGeomcloseNode;
	}
	return m_G2CloseHeadNest;
}
//�ð���ķ����жϺ���ķ�ջ�pHNtemp���Ѿ�ȷ��pHNtemp������㸸��ջ����ӷ�ջ�������£������и���ջ��ӷ�ջ�pHKtemp��Ƕ�׹�ϵ��
//���������治ͬ�ĵط����ڣ�������ֻ�жϺ����Ƿ�Ϊǰ�ߵ��ӷ�ջ�����������������ж�����֮���Ƕ�׹�ϵ��
Geom2CloseHeadNest GeomForCut::Judge2KidCloseHNest(GeomCloseHEAD*pHKtemp, GeomCloseHEAD*pHNtemp)
{
	Envelope_RectPoint m_GeomClosed1, m_GeomClosed2;
	double x1_min, x1_max, y1_min, y1_max;//Ѱ�ҷ�ջ�1�İ�����εķ�Χ
	double x2_min, x2_max, y2_min, y2_max;//Ѱ�ҷ�ջ�1�İ�����εķ�Χ
	m_GeomClosed1 = pHKtemp->m_GemoClosedLimtPoint;
	m_GeomClosed2 = pHNtemp->m_GemoClosedLimtPoint;
	//����ջ��ľ��ΰ������괫������ע�����ﲢû���õ����ƽǵ�����
	x1_min = m_GeomClosed1.x_min.x;
	x1_max = m_GeomClosed1.x_max.x;
	y1_min = m_GeomClosed1.y_min.y;
	y1_max = m_GeomClosed1.y_max.y;
	x2_min = m_GeomClosed2.x_min.x;
	x2_max = m_GeomClosed2.x_max.x;
	y2_min = m_GeomClosed2.y_min.y;
	y2_max = m_GeomClosed2.y_max.y;
	//�����Ѿ��Ѱ�����εķ�Χ�������
	//������ж����������Ƿ�Ƕ�׽����ж�
	//����Ҫ�ж�������ջ�֮���Ƕ�׹�ϵ��һ��������ֹ�ϵ������Բ�ͬ��type������
	//type=1������߷�ջ���ǰ�߷�ջ����ӷ�ջ���type=2����ǰ�߷�ջ��Ǻ��߷�ջ����ӷ�ջ���type=3����ǰ�����߷�ջ�û��Ƕ�׹�ϵ���˴�֮����ƽ�еĹ�ϵ��
	//����Ĭ��ͬ����ջ������ֵ��min<max
	//���������ǰ�ߵ��ӷ�ջ��������
	if (((x1_min <= x2_min) && (x2_max <= x1_max)) && ((y1_min <= y2_min) && (y2_max <= y1_max)))
	{
		//����н��������£���ôӦ�ð������ջ�2�ҵ���ջ�1�У���Ϊ�����ӷ�ջ�
		m_G2CloseHeadNest.m_IfCloseNest = true;
		m_G2CloseHeadNest.type = 1;//��ջ�2�Ƿ�ջ�1���ӷ�ջ�
		m_G2CloseHeadNest.FadCloseHead = pHKtemp;
		m_G2CloseHeadNest.KidCloseHead = pHNtemp;
		////���ݸ���ջ��ǵ�˫�����ж��ӷ�ջ��ĵ�˫��
		//if (pHKtemp->m_Singlelayer)//�ܽ���˵���ǵ��㣬��ΪĬ���ǵ���
		//{
		//	pHNtemp->m_Singlelayer = false;//��Ϊ���Ѿ����˼ҵ��ӷ�ջ����������ǵ��㣬���͵���˫��
		//}
		//else//pTempCHead->m_Singlelayer�������ջ���˫�㣬��ô�ӷ�ջ��͵��ǵ��㣬Ҳ���ǲ��ñ�
		//{
		//	pHNtemp->m_Singlelayer = true;//����˫���ʱ������Ӧ���ǵ�����
		//}
		m_G2CloseHeadNest.NextCloseHead = pHNtemp->nextGeomcloseNode;

	}
	//���������ǰ�ߵĸ���ջ��������
	//�����ص�Ҫ��ע������
	else if (((x2_min <= x1_min) && (x1_max <= x2_max)) && ((y2_min <= y1_min) && (y1_max <= y2_max)))
	{
		//����н��������£���ôӦ�ð������ջ�1�ҵ������ķ�ջ�2�У���Ϊ�����ӷ�ջ�
		//�����м�������Ҫ���ģ���һ�����������ջ�1����һ�㸸��ֱ��ָ�����ӷ�ջ�����ô��Ҫ������̳й�ϵҲת����ջ�2,�������ֱ�Ӽ̳�����Ҫ�������
		//�ڶ�������������Щ�ӷ�ջ�����ż�����⣬��ջ�1����ż������Ҫ�����ջ�2����ջ�1�Լ��Լ��ӷ�ջ�����ż����Ϣ��Ҫ���¸ı�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
		m_G2CloseHeadNest.m_IfCloseNest = true;
		m_G2CloseHeadNest.type = 2;//��ջ�2�Ƿ�ջ�1���ӷ�ջ�
		m_G2CloseHeadNest.FadCloseHead = pHNtemp;//��ݵ���
		m_G2CloseHeadNest.KidCloseHead = pHKtemp;
		////���ݸ���ջ��ǵ�˫�����ж��ӷ�ջ��ĵ�˫��
		//if (pHKtemp->m_Singlelayer)//�ܽ���˵���ǵ��㣬��ΪĬ���ǵ���
		//{
		//	pHNtemp->m_Singlelayer = true;//��Ϊ���Ѿ����˼ҵ��ӷ�ջ����������ǵ��㣬���͵���˫��
		//}
		//else//pTempCHead->m_Singlelayer�������ջ���˫�㣬��ô�ӷ�ջ��͵��ǵ��㣬Ҳ���ǲ��ñ�
		//{
		//	pHNtemp->m_Singlelayer = true;//����˫���ʱ������Ӧ���ǵ�����
		//}
		//Change_CloseHSinglelayer(pHKtemp);//ԭ�����Ӳ��ջ�ҲҪ������ż��ĸı�
		m_G2CloseHeadNest.NextCloseHead = pHNtemp->nextGeomcloseNode;
	}
	else		
	{
		m_G2CloseHeadNest.type = 3;//��ջ�2���ջ�1��û�й�ϵ
		m_G2CloseHeadNest.m_IfCloseNest = false;//ֻҪ��false�����涼���Բ��ÿ��ˡ�
		m_G2CloseHeadNest.KidCloseHead = pHNtemp;
		m_G2CloseHeadNest.NextCloseHead = pHNtemp->nextGeomcloseNode;
	}
	return m_G2CloseHeadNest;
}
////�����ӷ�ջ���Ȼ��ı���ӷ�ջ��µ����з�ջ�����ż�ԣ����ڷ�ջ�1�Ƿ�ջ�2���ӷ�ջ���������¡�
////������ԭ���ǽ����ڵ�ֵ�ı�ͺá�
//void GeomForCut::Change_CloseHSinglelayer(GeomCloseHEAD*pHKtemp)
//{
//	GeomCloseHEAD*phktemp;//��1��ջ���2��ջ����ӷ�ջ�ʱ��������1���ӷ�ջ�������ż�����
//	GeomCloseHEAD*phkbtemp;//��1��ջ���2��ջ����ӷ�ջ�ʱ��������1���ӷ�ջ����ֵܷ�ջ�������ż�����
//	//��������Ҫ���Լ�����ż�Ըı����
//	Change_singlelayer(pHKtemp);
//	//Ȼ�󿴿���û���ӷ�ջ����ӷ�ջ���û���ֵܷ�ջ��ˣ������Ҳ�а���Щ�ı䡣
//	//���pHKtemp���Ӳ㣬��ôҪ���Ӳ��Ҳ���иı�
//	//�����������˴Ӹ��ӷ�ջ���һ��ı䵽�ӷ�ջ����ֵܷ�ջ���һ��ĸı�
//	if (pHKtemp->FirstInsideGCloseNode)
//	{
//		phktemp = pHKtemp->FirstInsideGCloseNode;
//		while (phktemp)//�������Ӳ���иı�,ֱ�����һ���Ӳ�
//			{
//				if (phktemp->nextGeomcloseNode)//�ӷ�ջ������ֵܷ�ջ�
//					{
//						phkbtemp = phktemp->nextGeomcloseNode;
//						while (phkbtemp)//���ӷ�ջ����ֵܷ�ջ�Ҳ����һ�飬�ı�����ż��
//						{
//							Change_singlelayer(phkbtemp);
//							phkbtemp = phktemp->nextGeomcloseNode;
//						}
//					}
//				else//����ӷ�ջ�û���ֵܷ�ջ�����ô�Ͳ��ù����
//				{
//					;
//				}
//				Change_singlelayer(phktemp);
//				phktemp = phktemp->FirstInsideGCloseNode;
//			}
//	}
//	else//���û���ӷ�ջ�����ô�Ͳ��ù����
//	{
//		;
//	}
//}
////ת����һ��ջ�����ż�����ʣ�����������
////��Ϊ�������ż����֮ǰ�Ѿ�ȷ���õģ���ô���ڲ��ù�������ʲô��ֻ��֪��������֮ǰ��һ���ͺ���
//void GeomForCut::Change_singlelayer(GeomCloseHEAD*phktemp)
//{
//	if (phktemp->m_Singlelayer)//�ܽ������������
//	{
//		phktemp->m_Singlelayer = false;
//	}
//	else//����˵��������ż�㣬��ô�͸��Ǳ�Ϊ�����
//	{
//		phktemp->m_Singlelayer = true;
//	}
//}
//�����㷨
//�����㷨
//�����㷨
//��Ⱥ�㷨������һ���ջ�
void GeomForCut::Base_ACO_ForFirstCHead(NestResultDataNode*head)
{
	;
}
//�����㷨
//�����㷨
//�����㷨
//���յĴ������ӷ�ջ���˳��������Ѹ���ջ��Ĵ�׵�Ҳ����
//void GeomForCut::BaseTS_GR_ForKidCHead(NestResultDataNode*head)
//{
//	GeomCloseHEAD*pKidCloseHead, *pFadClosedHead, *pFirstClosedHead, *tempCloseHead, *pTheLastKidCHead;
//	double x_C0, y_C0;//��һ����һ���ջ���β�ڵ㣬����Ѱ����һ����һ���ջ��ӷ�ջ��Ĵ�׵㣬����һ����һ���ջ��Ĵ�׵��ѡ��
//	
//	pFirstClosedHead = head->FirstGeomClose;
//	//�ѵ�һ���ջ�ѭ����Ϳ�����
//	tempCloseHead = pFirstClosedHead;
//	while (tempCloseHead)
//	{
//		pKidCloseHead = tempCloseHead->FirstInsideGCloseNode;
//		while (pKidCloseHead->FirstInsideGCloseNode)//Ѱ�ҵ����һ���ӷ�ջ�
//			pKidCloseHead = pKidCloseHead->FirstInsideGCloseNode;
//			pFadClosedHead = tempCloseHead;
//		//�����һ���ջ��͵�һ����һ����ջ����ӷ�ջ���������һ���ӷ�ջ�
//		pTheLastKidCHead = ChangeINsideGeomEleNode(pFadClosedHead, pKidCloseHead);
//		if (tempCloseHead)//ֻҪ����NULL����Ӧ�ø������һ���ӷ�ջ���������һ�㸸��ջ���ͷ���
//		{
//			ChangeOutsideGeomEleNode(pTheLastKidCHead, tempCloseHead);
//		}
//		tempCloseHead = tempCloseHead->nextGeomcloseNode;
//	}
//}


//��һ��������ԭ������ķ�ջ�Ⱥ��
void GeomForCut::BaseTS_GR_ForKidCHead(NestResultDataNode*head)
{
	GeomCloseHEAD*pKidCloseHead, *pFadClosedHead, *pFirstClosedHead, *tempCloseHead, *pTheLastKidCHead;
	GeomCloseHEAD*tempfadCloseHead=NULL;
	GeomCloseHEAD*pFindTheFCH;
	double x_C0, y_C0;//��һ����һ���ջ���β�ڵ㣬����Ѱ����һ����һ���ջ��ӷ�ջ��Ĵ�׵㣬����һ����һ���ջ��Ĵ�׵��ѡ��

	pFirstClosedHead = head->FirstGeomClose;
	//�ѵ�һ���ջ�ѭ����Ϳ�����
	while (pFirstClosedHead)
	{
		
		pKidCloseHead =  pFirstClosedHead->FirstInsideGCloseNode;
		//����һ���ջ�ֻ��һ���ʱ��pKidCloseHeadΪNULL
		if (!(pKidCloseHead))
		{
			//��û���ӷ�ջ�ʱ�򣬾�������
			pKidCloseHead = pFirstClosedHead;
		}
		else
		{
			while (pKidCloseHead->FirstInsideGCloseNode)//Ѱ�ҵ����һ���ӷ�ջ�
				pKidCloseHead = pKidCloseHead->FirstInsideGCloseNode;
		}
		
		pFadClosedHead = pFirstClosedHead;
		//�����һ���ջ��͵�һ����һ����ջ����ӷ�ջ���������һ���ӷ�ջ�
		pTheLastKidCHead = ChangeINsideGeomEleNode(pFirstClosedHead, pKidCloseHead);
		//����һ��ķ�ջ��ڵ����
		pFindTheFCH = pTheLastKidCHead;
		while (pFindTheFCH->prevGeomcloseNode)//�ҵ�����һ���ջ���ͷ���
			pFindTheFCH = pFindTheFCH->prevGeomcloseNode;
		//�����ڶ���
			tempfadCloseHead = pFindTheFCH->prevOutsideGCloseNode;
			//�ӵ����ڶ��㿪ʼ���ڶ�������и�·���滮�ʹ�׵�滮
		while (tempfadCloseHead)//ȫ������һ��
		{
			
			//�����ӷ�ջ������һ���ڵ㣬������һ��ĸ���ջ�
			ChangeOutsideGeomEleNode(pTheLastKidCHead, tempfadCloseHead);
			//�����һ���Ѿ��ǵ�һ��ķ�ջ�����ô����Ҫ�滮��֮ǰ�Ѿ���ACO�滮���ˣ�ֱ������
			if (tempfadCloseHead == pFirstClosedHead)
			{
				break;
			}
			//������ĸ���ջ����ֵܷ�ջ������Ҳ��ǵ�һ��ķ�ջ���
			//��ô�͸ð����������ջ�Ϊ��׼���������ڵ���һ��
			else
			{
				//tempfadCloseHead��ÿһ���ջ���ͷ���
				//������һ����и�˳�򣬲�������һ����ջ�
				pTheLastKidCHead = ChangeGeomCHead(tempfadCloseHead);
			}
			tempfadCloseHead = tempfadCloseHead->prevOutsideGCloseNode;
			
		}
		//����ֻ�ǰѵ�һ��ĵ�һ���������ˡ�
		//����Ӧ���Ǹ������ڵ��и���Ƶ㣬Ѱ����һ����һ���ջ���
		//�����һ�㣬Ȼ�������һ���˳��
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
	GeomCloseHEAD*pTempCHead, *ppTempCHead;//��������ȫ����ջ�
	GeomCloseHEAD*pLastAcceptCHead;//��Ѱ�����һ������λ�ķ�ջ�ͷ���
	GeomCloseHEAD*pTheLastKidCHead = NULL;//��Ѱ�����һ������λ�ķ�ջ�ͷ���

	GeomCloseHEAD*pMinDistantCloseHead, *pntemp;//��Ѱ�����һ������λ�ķ�ջ�ͷ���

	GeomEleNode*pTempGelenode;
	double pre_x, pre_y, temp_x, temp_y;
	double m_temp_mindistant;//������ʱ����̾��룬��������յ���̾��룬��ô�͸��Ǹ���	newNode->m_mindistant;
	double m_tempmindistant_origin;//��ԭ�����
	double m_tempmindistant_ptp;//���Ѿ�����λ�ķ�ջ��ľ���
	double m_origin_x = 0.0;//����ԭ��
	double m_origin_y = 0.0;//����ԭ��
	int a = 0, b = 0, m_TotalCloseID, m_GeomCloseID;
	//�㷨��ʼ
	if (pNKidCloseHead->nextGeomcloseNode)//������ֵܷ�ջ��Ļ�
	{
		pTempCHead = pNKidCloseHead;
		pTempCHead->m_KidAcceptGeomCloseHead = true;
		while (pTempCHead)//������ӷ�ջ�ȫ������һ��,ֱ�����һ������ѭ��
		{
			a++;
			while (!(pTempCHead->m_KidAcceptGeomCloseHead))//ֻ�е��÷�ջ���û�б��滮��ʱ�򣬽����������
			{
				//pTempCHead�ܽ���˵�����������ջ�˫������ĵ�һ��û�б���λ�ķ�ջ���
				//ǰ���Ѿ���һ����ջ�����λ��

					pre_x = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
					pre_y = pTempCHead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				
				//�����Ѿ��ҵ���һ����յ���ֹ���ˡ�
				//����Ҫ�Ը���ֹ�����������еķ�ջ��ڵ㣬�ҵ��䵽��һ����ջ���ֹ����̾�����Ǹ��ڵ�ͷ�ջ�
				//Ҫ��ʣ�µķ�ջ����б���
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
				b++;
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
				//���ǰ��һ���ڵ�������һ������λ�Ľڵ㣬��ôֱ�ӹ���ȥ
				if (newNode->pminhead->prevGeomcloseNode->m_KidAcceptGeomCloseHead)
				{
					//��ôʲô�����ö�
					newNode->pminhead->m_KidAcceptGeomCloseHead = true;//�Ӵ˸ýڵ�ͱ��̶���

				}
				else//����ҪѰ�ҵ������λ�Ľڵ㣬�ҵ������
				{
					pLastAcceptCHead = pNKidCloseHead;
					while (pLastAcceptCHead->m_KidAcceptGeomCloseHead)
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
					//��ҪpLastAcceptCHead�������Ѿ�����λ�ģ����ǵ�һ��û����λ�ģ�pLastAcceptCHead->prevGeomcloseNode�������һ������λ��
					newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
					newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
					pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
					pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
					newNode->pminhead->m_KidAcceptGeomCloseHead = true;//��λ
				}
				//��ΪpNKidCloseHeadһ�������ڲ��ͷ��㣬���Ѿ����̶��ˣ�����������벻��Ҫ
				
				
				////˵��newNode->pminnodeǡ����ͷ��㣬���������һ��ջ����Ǹ���ջ�
				//if (!(newNode->pminhead->prevGeomcloseNode))//�����ͷ��㣬����һ�鷢�ֻ���ͷ���Ϊ����ķ�ջ��ڽڵ�
				//{
				//	pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode = newNode->pminhead;//��������Ϊ�ӷ�ջ���ͷ�ڵ�
				//	newNode->pminhead->m_KidAcceptGeomCloseHead = true;//�Ӵ˸ýڵ�ͱ��̶���
				//}
				//newNode->pminhead��ǰ��պ��Ǳ���λ��
				//�������һ�����������ǡ��pLastAcceptCHead=newNode->pminhead;��˱��ܽ���������
				// if (newNode->pminhead->prevGeomcloseNode->m_KidAcceptGeomCloseHead)
				//{
				//	//��ôʲô�����ö�
				//	newNode->pminhead->m_KidAcceptGeomCloseHead = true;//�Ӵ˸ýڵ�ͱ��̶���

				//}
				//else//˵��newNode->pminnode����ͷ����ˣ���ô����ڵ�Ҫ�ҵ�Ŀǰ���һ��m_AcceptGeomcloseHEADû�б�����Ϊtrue�ķ�ջ��ڵ����
				//{

				//	//���������Ѱ�ң�����pLastAcceptCHead�����һ������λ�Ľڵ㣬��newNode->pminhead����Ҫ�ҵ�pLastAcceptCHead����
				//	//������ÿ�ε���˫������Ľڵ�λ��ʱ�򣬶�Ҫ���Ǹýڵ��ǲ���ͷβ����һ��ڵ�ģ���������Բ��ÿ���ͷ�����
				//	//��ôҪ���ҵ����һ��û����λ�ķ�ջ�������ͷ���Ҳ����������ǰ�����˱���λ�ķ�ջ�
				//	
				//	pLastAcceptCHead = pNKidCloseHead;
				//	while (pLastAcceptCHead->m_KidAcceptGeomCloseHead)
				//		pLastAcceptCHead = pLastAcceptCHead->nextGeomcloseNode;
				//	//pLastAcceptCHead��û�б���λ��
				//	if (!(newNode->pminhead->nextGeomcloseNode))//�����β�ڵ�
				//	{
				//		//��Ȼ�Ѿ���β�ڵ�newNode->pminhead����ôֱ�Ӱ���ǰ��һ��ֵ�ú���ڵ�ָ��NULL��
				//		newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = NULL;
				//	}
				//	else//һ��ڵ�
				//	{
				//		//newNode->pminheadԭ��ǰ��ָ���ϵ�ĵ���
				//		newNode->pminhead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead->nextGeomcloseNode;
				//		newNode->pminhead->nextGeomcloseNode->prevGeomcloseNode = newNode->pminhead->prevGeomcloseNode;
				//	}
				//	//���ϴ�������newNode->pminhead��ԭ��λ�õĹ�ϵ��������Ҫ�������ڱ���λʱ��Ĺ�ϵ��
				//	//���pLastAcceptCHead����ͷ�ڵ㣬˵����һ����ջ�����û�б���λ
				//	//Ҫ���ǵ�һ���ǵ�pLastAcceptCHead=newNode->pminhead��ʱ����ô��
				//	if (!(pLastAcceptCHead->prevGeomcloseNode))//��ô��������ǣ�����һ��ĩβ��������Ǹ���ջ�����ͷ��㣬��������Ҳû�з�ջ�����λ
				//	{
				//		//���ʱ��pLastAcceptCHead���Ƿ�ջ�˫�������ͷ���
				//		//Ӧ�ð�newNode->pminhead����headָ��ĵ�һ����ջ��ڵ�
				//		pNKidCloseHead->prevOutsideGCloseNode->FirstInsideGCloseNode = newNode->pminhead;
				//		newNode->pminhead->prevGeomcloseNode = NULL;//�׽��
				//		newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
				//		pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
				//		newNode->pminhead->m_KidAcceptGeomCloseHead = true;//��λ
				//	}
				//	//��һ�������newNode->pminhead������pLastAcceptCHead���
				//	else//��ô��newNode->pminheadֱ�ӹҵ�pLastAcceptCHead����ȥ
				//	{
				//		//��ҪpLastAcceptCHead�������Ѿ�����λ�ģ����ǵ�һ��û����λ�ģ�pLastAcceptCHead->prevGeomcloseNode�������һ������λ��
				//		newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
				//		newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
				//		pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
				//		pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
				//		newNode->pminhead->m_KidAcceptGeomCloseHead = true;//��λ
				//	}
				//}
				pTempCHead = newNode->pminhead;//�������ѭ��
			}
			b;
			if (!(pTempCHead->nextGeomcloseNode))//˵���������һ���ӷ�ջ�
			{
				pTheLastKidCHead = pTempCHead;
			}
			pTempCHead = pTempCHead->nextGeomcloseNode;
		}
	}
	else//���û��������ջ�����ô���ķ�ջ��ڵ㻹���䱾��
	{
		pTheLastKidCHead = pNKidCloseHead;
	}
	

	return pTheLastKidCHead;
}
//Ѱ����һ����һ���ջ�
GeomCloseHEAD*GeomForCut::Find_Next_TheFirstLevel(GeomCloseHEAD*pFirstClosedHead)
{
	Mindistant_EleNode*newNode = (Mindistant_EleNode*)malloc(sizeof(Mindistant_EleNode));
	newNode->m_mindistant = 99999.0;
	newNode->pminhead = NULL;
	newNode->pminnode = NULL;
	GeomCloseHEAD*pFirstl, *pHtemp, *pMinDistantCloseHead;
	GeomCloseHEAD*pLastAcceptCHead;//��Ѱ�����һ������λ�ķ�ջ�ͷ���
	GeomCloseHEAD*pTheLastKidCHead = NULL;//��Ѱ�����һ������λ�ķ�ջ�ͷ���

	double pre_x, pre_y, temp_x, temp_y;
	double m_temp_mindistant;//������ʱ����̾��룬��������յ���̾��룬��ô�͸��Ǹ���	newNode->m_mindistant;
	double m_tempmindistant_origin;//��ԭ�����
	double m_tempmindistant_ptp;//���Ѿ�����λ�ķ�ջ��ľ���
	double m_origin_x = 0.0;//����ԭ��
	double m_origin_y = 0.0;//����ԭ��
	GeomEleNode*pTempGelenode;
	if (pFirstClosedHead->nextGeomcloseNode)//������ֵܷ�ջ��Ļ�
	{
		pHtemp = pFirstClosedHead->nextGeomcloseNode;//����һ����ʼ
		pFirstl = pFirstClosedHead->nextGeomcloseNode;
		while (pFirstl)//������һ��,�����еĵ�һ���ջ��뿪���ɱ�
		{
			pFirstl->m_AcceptGeomcloseHEAD = false;
			pFirstl = pFirstl->nextGeomcloseNode;
		}
		//�㷨��ʼ
		pre_x = pFirstClosedHead->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
		pre_y = pFirstClosedHead->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
		while (pHtemp)//������ӷ�ջ�ȫ������һ��,ֱ�����һ������ѭ��
		{
			a++;
			while (!(pHtemp->m_AcceptGeomcloseHEAD))//ֻ�е��÷�ջ���û�б��滮��ʱ�򣬽����������
			{
				//pTempCHead�ܽ���˵�����������ջ�˫������ĵ�һ��û�б���λ�ķ�ջ���
				//ǰ���Ѿ���һ����ջ�����λ��	
				//����Ҫ�Ը���ֹ�����������еķ�ջ��ڵ㣬�ҵ��䵽��һ����ջ���ֹ����̾�����Ǹ��ڵ�ͷ�ջ�
				//Ҫ��ʣ�µķ�ջ����б���
				temp_x = pHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
				temp_y = pHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_y0;

				//��ԭ��ľ���
				m_tempmindistant_origin = fabs(sqrt(((temp_x - m_origin_x)*(temp_x - m_origin_x)) + ((temp_y - m_origin_y)*(temp_y - m_origin_y))));//�������������֮�����̾���
				//������ջ�֮��ľ���
				m_tempmindistant_ptp = fabs(sqrt(((temp_x - pre_x)*(temp_x - pre_x)) + ((temp_y - pre_y)*(temp_y - pre_y))));//�������������֮�����̾���
				//����̾���,��ʱ����̾���
				newNode->m_mindistant = m_tempmindistant_origin*WEIGHT_ORIGIN + m_tempmindistant_ptp*WEIGHT_PTP;
				//newNode->m_mindistant = 1 / newNode->m_mindistant;
				newNode->pminhead = pHtemp;
				newNode->pminnode = pHtemp->FirstGeomele;
				pMinDistantCloseHead = pHtemp;
				//��ʣ�µ�û�б���λ�����б���һ��
				//����̾���Ϊ׼������￪ʼ��������һ����ջ���ͷ�����ʲô
				while (pMinDistantCloseHead)
				{
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
				//�����Ѿ��ҵ����������������λ�ķ�ջ���ͷ����ȫ������
				//����Ҫ�����ǻ��ڷ�ջ�ͷ����ͼԪ�ڵ����˫���������ݵ���
				//���ǰ�ͼԪ�ڵ����Ϊ��ջ���ͷ���
				GeomEleNode*phtemp, *ptemp;
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
				//���ǰ��һ���ڵ�������һ������λ�Ľڵ㣬��ôֱ�ӹ���ȥ
				if (newNode->pminhead->prevGeomcloseNode->m_AcceptGeomcloseHEAD)
				{
					//��ôʲô�����ö�
					newNode->pminhead->m_AcceptGeomcloseHEAD = true;//�Ӵ˸ýڵ�ͱ��̶���

				}
				else//����ҪѰ�ҵ������λ�Ľڵ㣬�ҵ������
				{
					pLastAcceptCHead = pFirstClosedHead;
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
					//��ҪpLastAcceptCHead�������Ѿ�����λ�ģ����ǵ�һ��û����λ�ģ�pLastAcceptCHead->prevGeomcloseNode�������һ������λ��
					newNode->pminhead->nextGeomcloseNode = pLastAcceptCHead;
					newNode->pminhead->prevGeomcloseNode = pLastAcceptCHead->prevGeomcloseNode;
					pLastAcceptCHead->prevGeomcloseNode->nextGeomcloseNode = newNode->pminhead;
					pLastAcceptCHead->prevGeomcloseNode = newNode->pminhead;
					newNode->pminhead->m_AcceptGeomcloseHEAD = true;//��λ
				}
				pHtemp = newNode->pminhead;//�������ѭ��
			}

			
				pTheLastKidCHead = pFirstClosedHead;
			
			break;//����ѭ����ֻҪ�ҵ���һ���ͺ���
			//pHtemp = pHtemp->nextGeomcloseNode;
		}
	}


	else//���û��������ջ�����ô���ķ�ջ��ڵ㻹���䱾��
	{
		pTheLastKidCHead = pFirstClosedHead;
	}


	return pTheLastKidCHead;
	
}
////////////////////////////////////����и�������//////////////////////////////////////////
//����ƽ���и�ͼͷ�ļ��������ڷ�ջ����ݽṹ�е�ÿһ����ջ�����ż����ȷ��������Ϊ������ӷ�ջ����и���������Ϊ�жϵ�׼��
void GeomForCut::JudgeClosedHead_Odd_even(NestResultDataNode*head)
{
	//����ѭ�����Ե�һ���ջ����ֵܷ�ջ�֮��ı�����ѭ������ÿһ����һ���ջ����ӷ�ջ�Ϊ��ѭ����
	GeomCloseHEAD*pFtemp;//����ָ����һ���ջ�ͷ���
	bool m_IfHaveKidCH;//��¼ĳ����ջ��Ƿ����ӷ�ջ���
	pFtemp = head->FirstGeomClose;
	//��һ��ѭ������ѭ��
	while (pFtemp)//��һ���ջ����ѭ�������������һ��
	{
		//�жϷ�ջ��Ƿ����ӷ�ջ�
		m_IfHaveKidCH = IfIncludeKidClose(pFtemp);
		if (!(m_IfHaveKidCH))//���û���ӷ�ջ�
		{
			WriteClosedHead_Odd_even(pFtemp);
		}
		else//������ӷ�ջ��������
		{
			KidClosedHead_Odd_even(pFtemp);
		}
		pFtemp = pFtemp->nextGeomcloseNode;
	}
}
//����ÿһ������ջ�����д�����ӷ�ջ�����ż�ԡ�
void GeomForCut::KidClosedHead_Odd_even(GeomCloseHEAD*pFtemp)
{
	GeomCloseHEAD*pKtemp;//����ָ���ӷ�ջ�ͷ���
	bool m_IfHaveKidCH;//��¼ĳ����ջ��Ƿ����ӷ�ջ���
	//�ı丸��ջ�����ż����
	WriteClosedHead_Odd_even(pFtemp);
	pKtemp = pFtemp->FirstInsideGCloseNode;
	//�ڶ���ѭ��������ڲ��ӷ�ջ�����ż�Խ��е���,Ҳ�Ƕ�ͬһ����е���
	while (pKtemp)
	{
		//�ж��ӷ�ջ����ֵܷ�ջ��Ƿ����ӷ�ջ�
		m_IfHaveKidCH = IfIncludeKidClose(pKtemp);
		if (!(m_IfHaveKidCH))//���û���ӷ�ջ�
		{
			WriteClosedHead_Odd_even(pKtemp);
		}
		else//������ӷ�ջ��������
		{
			KidClosedHead_Odd_even(pKtemp);
		}
		pKtemp = pKtemp->nextGeomcloseNode;//����Եݹ鷽�̣������ײ�ķ�ջ��������ˣ����Զ�������һ�����ѭ����
	}
}



//����ÿһ����ջ���д������ż�Եı�׼
//ÿһ����ջ�����ż�Զ����ɴ˺���д��
void  GeomForCut::WriteClosedHead_Odd_even(GeomCloseHEAD*pClosedH)
{
	GeomCloseHEAD*pFClosedH;//��Ÿ���ջ�
	//�жϵ�׼����ͬ�ֵܷ�ջ���ķ�ջ�����ż��һ�£������ӷ�ջ���ķ�ջ�����ż�����
	//��Ҫ������Ƿ�ջ�˫��������׷�ջ�����ô����ż�Ը��ݸ���ջ���ȷ�����෴��
	//������ǣ���ô����ż�Ը���ͬ���ֵܷ�ջ�����ż����������ͬ��

	//��ջ��ֵ�˫�������ͷ���
	if (!(pClosedH->prevGeomcloseNode))
	{
		//��Ҫ����������и�ͼ�ĵ�һ�㸸��ջ�ʱ����ôһ�������
		if (!(pClosedH->prevOutsideGCloseNode))//ƽ���и�ͼ���׷�ջ�
		{
			pClosedH->m_Singlelayer = true;
		}
		else//���������и���ջ���
		{
			pFClosedH = pClosedH->prevOutsideGCloseNode;
			if (pFClosedH->m_Singlelayer)//����
			{
				pClosedH->m_Singlelayer = false;
			}
			else//��ż
			{
				pClosedH->m_Singlelayer = true;
			}
		}

	}
	else//��ջ��ֵ�˫�������е������ڵ㣬ע�������з�ջ�ǰ��ڵ�,��ʱͬһ���ջ�����ż����ͬ
	{
		pClosedH->m_Singlelayer = pClosedH->prevGeomcloseNode->m_Singlelayer;
	}
}

//***************************************û���õ�**************************************************//
//�����ͼԪ��βͼԪ��ֱ�ߵ�б�ʣ��ô�б����ȷ���и������ߵ�б��
double GeomForCut::CalculateSlope(GeomEleNode*pGnode)
{
	double slope;//б��
	double x0, x1, y0, y1;//����ͼԪ���ĸ����Ƶ�
	x0 = pGnode->m_GeomStandData.GeoEle_start_x0;
	y0 = pGnode->m_GeomStandData.GeoEle_start_y0;
	x1 = pGnode->m_GeomStandData.GeoEle_start_x1;
	y1 = pGnode->m_GeomStandData.GeoEle_start_y1;
	//�ж��Ƿ�Ϊ��ֱֱ��
	if (x0 == x1)
	{
		slope = -987654;//����һ���̶���ֵ����Ϊ��ֱֱ�ߵı�ʶ
	}
	//�ж��Ƿ�Ϊˮƽֱ��
	else if (y1 == y0)
	{
		slope = 0.0;
	}
	//һ��ֱ��
	else
	{
		slope = (y1 - y0) / (x1 - x0);
	}
	return slope;
}


//****************************************************************************************//
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//�µ�����и������ߵķ���//
//����и������ߣ����������Ѿ��ѷ�ջ������߼�˳��ҿ����ˡ�
void GeomForCut::Add_CutGuideLine(NestResultDataNode*head, bool if_LineAuxiliary)
{
	//�����Ҫ�����еķ�ջ������߼�˳���һ�飬���ǿӵ���
	GeomCloseHEAD*ptemp;//��ջ��׽�㡣
	bool m_HaveKidClosed;//�ж��Ƿ����ӷ�ջ������ӷ�ջ�����Ҫ����������ӷ�ջ���ʼ�ҿ�

	ptemp = head->FirstGeomClose;
	while (ptemp)//����ջ�ȫ�������������ǶԵ�һ���ջ����б���
	{
		//�ж��Ƿ����ӷ�ջ�
		m_HaveKidClosed = IfIncludeKidClose(ptemp);
		if (m_HaveKidClosed)//����У���ô����ר��Ϊ���ӷ�ջ����и���������Ӻ���
		{
			Add_KidCloseCutLine(ptemp, if_LineAuxiliary);
		}
		else
		{
			if (if_LineAuxiliary)//ֱ�����и�������
			{
				CreatCutGuideLINE(ptemp);//���������
			}
			else//Բ�����и�������
			{
				CreatCutAuxiliaryPath(ptemp);//���Բ�����и�������
			}
		}
		//���û���ӷ�ջ�����ô�Ϳ��԰���ÿ����ջ��������������������������
		
		ptemp = ptemp->nextGeomcloseNode;
	}

}
//�жϷ�ջ��Ƿ����ӷ�ջ�
bool GeomForCut::IfIncludeKidClose(GeomCloseHEAD*Phead)
{
	if (Phead->FirstInsideGCloseNode)//���ӷ�ջ�
	{
		return true;
	}
	else
	{
		return false;
	}
}
//���Ĵ��룬�������ӷ�ջ��ķ�ջ�����
//�����ӷ�ջ�����ô�Ϳ��ܴ���һ���ķ�ջ���
//��Ϊ�ӷ�ջ��������ֵܷ�ջ����ֵܷ�ջ��������ӷ�ջ���
//����������Ҫ���ӷ�ջ������ݴ����꣬Ȼ��Ҫ�ѷ�ջ���������ݴ�����
//�Եݹ鷽��
void GeomForCut::Add_KidCloseCutLine(GeomCloseHEAD*Phead, bool if_LineAuxiliary)//����һ�������ӷ�ջ��ķ�ջ�����������и�������
{
	GeomCloseHEAD*pktemp;//�ӷ�ջ���㡣
	GeomCloseHEAD*pkNtemp;//���ֵܷ�ջ���㡣
	GeomCloseHEAD*pFtemp;//��һ�㸸��ջ���㡣
	GeomCloseHEAD*pFNtemp;//��һ�㸸�ֵܷ�ջ���㡣
	bool m_HaveKidClosed;//�ж��Ƿ����ӷ�ջ������ӷ�ջ�����Ҫ����������ӷ�ջ���ʼ�ҿ�
	pktemp = Phead->FirstInsideGCloseNode;//�ӷ�ջ�
	m_HaveKidClosed = IfIncludeKidClose(pktemp);
	if (m_HaveKidClosed)//�����,���һֱ�жϣ����룬ֱ�����һ��û���ӷ�ջ��ķ�ջ�
	{
		Add_KidCloseCutLine(pktemp,if_LineAuxiliary);
	}
	else//û���ӷ�ջ�����ô�Ϳ��Խ����ӷ�ջ�����и������ߡ�
	{
		if (if_LineAuxiliary)//ֱ�����и�������
		{
			CreatCutGuideLINE(pktemp);//���������
		}
		else//Բ�����и�������
		{
			CreatCutAuxiliaryPath(pktemp);//���Բ�����и�������
		}
	}
	//����ֻ�ǰѺ����ӷ�ջ��ĵ�һ������ķ�ջ�����ײ��ջ���������
	//����Ҫ������ײ���ֵܷ�ջ�
	//����һ������ջ�
	//ʣ���ջ�����ʼ
	//������ֵܷ�ջ�,��ô��Ҫ�Ѹò���ֵܻ�������
	if (pktemp->nextGeomcloseNode)
	{
		pkNtemp = pktemp->nextGeomcloseNode;
		//�����ò���ֵܷ�ջ�
		while (pkNtemp)
		{
			//����ֵܷ�ջ����ӷ�ջ��������
			if (pkNtemp->FirstInsideGCloseNode)
			{
				Add_KidCloseCutLine(pkNtemp, if_LineAuxiliary);
			}
			else//���û�е������,ֱ�Ӵ���
			{
				//�ж��и������ߵ����ɷ�ʽ
				if (if_LineAuxiliary)//ֱ�����и�������
				{
					CreatCutGuideLINE(pkNtemp);//���������
				}
				else//Բ�����и�������
				{
					CreatCutAuxiliaryPath(pkNtemp);//���Բ�����и�������
				}
			}
			pkNtemp = pkNtemp->nextGeomcloseNode;
		}
	}
	//���û���ֵܷ�ջ��������ֵܷ�ջ��Ѿ���������
	//һ�����и���ջ���
	pFtemp = pktemp->prevOutsideGCloseNode;
	//��Ҫ����
	while (pFtemp != Phead)//�����һ�㸸��ջ������ǵ�һ�㸸��ջ���ʱ����ô��Ҫ�������ϴ���
	{
		//�ж��и������ߵ����ɷ�ʽ
		if (if_LineAuxiliary)//ֱ�����и�������
		{
			CreatCutGuideLINE(pFtemp);//���������
		}
		else//Բ�����и�������
		{
			CreatCutAuxiliaryPath(pFtemp);//���Բ�����и�������
		}
		//�ж��Ƿ����ֵܷ�ջ�
		if (pFtemp->nextGeomcloseNode)
		{
			pFNtemp = pFtemp->nextGeomcloseNode;
			//����һ���ֵܷ�ջ�������
			while (pFNtemp)
			{
				//�ֵܷ�ջ��Ƿ����ӷ�ջ�
				if (pFNtemp->FirstInsideGCloseNode)
				{
					Add_KidCloseCutLine(pFNtemp,if_LineAuxiliary);
				}
				else
				{
					//�ж��и������ߵ����ɷ�ʽ
					if (if_LineAuxiliary)//ֱ�����и�������
					{
						CreatCutGuideLINE(pFNtemp);//���������
					}
					else//Բ�����и�������
					{
						CreatCutAuxiliaryPath(pFNtemp);//���Բ�����и�������
					}
				}
				pFNtemp = pFNtemp->nextGeomcloseNode;
			}

		}

		pFtemp = pFtemp->prevOutsideGCloseNode;

	}
	//�����Ѿ����ɵ�һ����Ǹ���ջ�������ӷ�ջ�����һ���ӷ�ջ�֮��ķ�ջ�ȫ��������
	//�ж��и������ߵ����ɷ�ʽ
		if (if_LineAuxiliary)//ֱ�����и�������
		{
			CreatCutGuideLINE(Phead);//���������
		}
		else//Բ�����и�������
		{
			CreatCutAuxiliaryPath(Phead);//���Բ�����и�������
		}
}
//����ĵĴ��룬������뽫��Ҫ��ÿһ����ջ��������и�������
//Ҫ���жϸ��и������߲������������н��㣬Ҳ���ǲ�����ڰ��и������߿���Ҫ�����Ĳ�����
//��Ҫ���и������߼��뵽�ӷ�ջ��У�����Ϊͷ���
//��������ʵĻ�����Ҫ�Է�ջ����и���Ƶ��������ѡ��
 void GeomForCut::CreatCutGuideLINE(GeomCloseHEAD*Phead)//����ÿһ����ջ���Ȼ���������и� �����ߵ���ӡ�
{
	 GeomEleNode*cut_in_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//�и�������
	 GeomEleNode*cut_out_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//�и�������
	GeomCloseHEAD*ptemp;//���÷�ջ��ڵ�
	GeomEleNode*Fnode = NULL;
	GeomEleNode*Enode = NULL;
	//Line_para m_startline, m_endline;//��ջ���ͼԪ�ڵ��βͼԪ�ڵ�Ļ�������
	Line_para m_cutline;//�и������ߵĺ��Ĳ���
	double x0_min, y0_min, x0_max, y0_max;
	double x1_min, y1_min, x1_max, y1_max;
	int m_TypeGemoEle;//��Ϊ�и������ߵ��������ջ�����ɻ���ͼԪ�йأ�����ֱ������Ϊ1��Բ������Ϊ2��Բ������Ϊ3.
	int m_TypeCGLine = 1;//Ϊ�����и������ߵ����㷨�������и������߷�ʽ�������֣�����������m_TypeCGLineΪ1���ӵ����Ǳ߽����m_TypeCGLineΪ2.
	//ֻ����ֱ�����͵�ͼԪ,û�п���Բ��Բ����Ҳû�п����Ƿ��и��������н��档
	//��ȡ����ͼԪ���ͣ�Ϊ�����и������������ṩ������
	m_TypeGemoEle = Phead->FirstGeomele->m_GeomStandData.m_typegeomele;//����ͼԪ������һ�ֻ������ԡ�
	//����ͼԪ���͵��ò�ͬ���и�������������ʽ
	//����ֱ���͵��Ѿ������
	//Բ�͵Ŀ������������߷�������
	//Բ���͵Ŀ�����������Բ��Ȼ�����ֱ�ߵ�������������ʽ��
	switch (m_TypeGemoEle)
	{
	case 1:
		//ֱ���͵Ļ���ͼԪ�и������ߴ��졣
		Fnode = Phead->FirstGeomele;
		Enode = Fnode->nextGeomeleNode;
		while (Enode->nextGeomeleNode)//�ҵ����һ���ڵ�
			Enode = Enode->nextGeomeleNode;
		m_cutline = CreatCutGuideLINE_Polygon(Phead, m_TypeCGLine);
		break;
	case 2:
		
		break;
	case 3:
		//Բ���͵Ļ���ͼԪ�и������ߴ��졣
		Fnode = Phead->FirstGeomele;
		Enode = Phead->FirstGeomele;
		m_cutline = CreatCutGuideLine_Circle(Phead, m_TypeCGLine);
		break;

	default:
		break;
	}
	
	
	//�������������и������ߣ������ǽ��и������߼��뵽���еķ�ջ��С�
	//���и�������Ҳһ������Ϊͬһ��ͼԪ��ʽ
	cut_in_Node->m_GeomStandData.GeoEle_start_x0 = m_cutline.x0;
	cut_in_Node->m_GeomStandData.GeoEle_start_y0 = m_cutline.y0;
	cut_in_Node->m_GeomStandData.GeoEle_start_x1 = m_cutline.x1;
	cut_in_Node->m_GeomStandData.GeoEle_start_y1 = m_cutline.y1;
	cut_in_Node->m_GeomStandData.k = m_cutline.k;
	cut_in_Node->m_GeomStandData.m_typegeomele = 61;//ֱ�����и������ߵı�׼
	//����ֱ���͵��и������߶��ԣ�cut_in��cut_out��������һ���ģ�������λ��Ҫ����
	cut_out_Node->m_GeomStandData.GeoEle_start_x0 = m_cutline.x1;
	cut_out_Node->m_GeomStandData.GeoEle_start_y0 = m_cutline.y1;
	cut_out_Node->m_GeomStandData.GeoEle_start_x1 = m_cutline.x0;
	cut_out_Node->m_GeomStandData.GeoEle_start_y1 = m_cutline.y0;
	cut_out_Node->m_GeomStandData.k = m_cutline.k;
	cut_out_Node->m_GeomStandData.m_typegeomele = 61;//ֱ�����и������ߵı�׼
	//�ص㣡����
	//�Ӵ˷�ջ���ͷ��㲻���ǵ�һ����ջ�ͼԪ�������и�������
	//�ڶ�������ԭ�����е�ͼԪ��
	Phead->FirstGeomele = cut_in_Node;//�и�������Ϊ��ͼԪ
	cut_in_Node->nextGeomeleNode = Fnode;//ָ��ԭ������ͼԪ
	cut_in_Node->prevGeomeleNode = NULL;//��Ȼ���ͳ��˵�һ����
	Fnode->prevGeomeleNode = cut_in_Node;//ԭ��ǰ��ָ�����NULL
	//����cut_out
	Enode->nextGeomeleNode = cut_out_Node;//ԭ����ջ������һ��ͼԪָ�����NULL������ָ��cut_out
	cut_out_Node->prevGeomeleNode = Enode;
	cut_out_Node->nextGeomeleNode = NULL;
	//��˷�ջ��ͼ������и�������
}
 //���Ĵ���//
 //��ֱ�߹��ɵķ�ջ����������и������ߵķ���
 Line_para GeomForCut::CreatCutGuideLINE_Polygon(GeomCloseHEAD*Phead, int m_TypeCGLine)
 {
	 GeomCloseHEAD*ptemp;//���÷�ջ��ڵ�
	 GeomEleNode*Fnode = NULL;
	 GeomEleNode*Enode = NULL;
	 Line_para m_startline, m_endline;//��ջ���ͼԪ�ڵ��βͼԪ�ڵ�Ļ�������
	 Line_para m_cutline;//�и������ߵĺ��Ĳ���
	 bool m_Singlelayer;
	 m_Singlelayer = Phead->m_Singlelayer;
	 Fnode = Phead->FirstGeomele;
	 Enode = Fnode->nextGeomeleNode;
	 while (Enode->nextGeomeleNode)//�ҵ����һ���ڵ�
		 Enode = Enode->nextGeomeleNode;
	 //�����õ��ǽ�ƽ���߷�
	 //��ͼԪ�ڵ�
	 m_startline.x0 = Fnode->m_GeomStandData.GeoEle_start_x0;
	 m_startline.y0 = Fnode->m_GeomStandData.GeoEle_start_y0;
	 m_startline.x1 = Fnode->m_GeomStandData.GeoEle_start_x1;
	 m_startline.y1 = Fnode->m_GeomStandData.GeoEle_start_y1;
	 //βͼԪ�ڵ㣬Ҫע����β�Ե�
	 m_endline.x0 = Enode->m_GeomStandData.GeoEle_start_x1;
	 m_endline.y0 = Enode->m_GeomStandData.GeoEle_start_y1;
	 m_endline.x1 = Enode->m_GeomStandData.GeoEle_start_x0;
	 m_endline.y1 = Enode->m_GeomStandData.GeoEle_start_y0;
	 //�����и������ߺ���
	 m_cutline = m_CutLeadLine.Get_CutLeadLine(m_startline, m_endline, m_Singlelayer, m_TypeCGLine);
	 return m_cutline;
 }
 //���Ĵ���
 //��Բ�͹��ɵķ�ջ����������и������ߵķ���
 Line_para GeomForCut::CreatCutGuideLine_Circle(GeomCloseHEAD*Phead, int m_TypeCGLine)
 {
	 GeomCloseHEAD*ptemp;//���÷�ջ��ڵ�
	 Line_para m_cutline;//�и������ߵĺ��Ĳ���
	 Point Last_GClose;//��һ����ջ����и���Ƶ�
	 Point Current_GClose;//��ǰ��ջ����и���Ƶ�
	 Point Circle_Center;//Բ������
	 bool m_Singlelayer;
	 m_Singlelayer = Phead->m_Singlelayer;
	 //ԲͼԪֻ��һ����ջ�������ֻ��һ������ͼԪ��ͷ�����β�ڵ㶼��ͬһ��
	
	 //��ȡԲ������
	 Circle_Center.x = Phead->FirstGeomele->m_GeomStandData.m_circle.m_Circent_x;
	 Circle_Center.y = Phead->FirstGeomele->m_GeomStandData.m_circle.m_Circent_y;
	 //�����õ����������߷��������������߷���Ӧ�ò���ȡ�ġ�
	//Ҫ�ж�Phead�ǲ��ǵ�һ��ͼԪ��
	 if (!(Phead->prevGeomcloseNode))
	 {
		// �����ǰ��Բ�ǵ�һ��ͼԪ��ô��һ������ԭ��
		 Last_GClose.x = 0;
		 Last_GClose.y = 0;
	 }
	 else
	 {
		 // �����ǰ��Բ���ǵ�һ��ͼԪ����ô������һ����ջ���
		 Last_GClose.x = Phead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
		 Last_GClose.y = Phead->prevGeomcloseNode->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
	 }
	 //�����ҵ����������߷��е�һ���㡣
	 //Ҫ�ѵ�ǰ��ջ����и���Ƶ��ȡ
	 Current_GClose.x = Phead->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
	 Current_GClose.y = Phead->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
	 //���½����и�������ĸ�ߵ�����
	 //�����и������ߺ���
	 m_cutline=m_CutLeadLine.Get_CutLeadLine(Last_GClose, Current_GClose,Circle_Center, m_Singlelayer, m_TypeCGLine);
	 return m_cutline;
 }

 ////////////////////////////////////�и������߸����ж�//////////////////////////////////////////
 //���Ĵ��룬���Ĵ��룬���Ĵ���
 //�и������߸����жϴ����㷨
 //�����Ѿ����ú��и������ߵ�ƽ���и�ͼָ�룬Ȼ���ÿһ����ջ����и������߽��и����жϣ�������
 //����Ѿ����и������߸��������ƽ���и�ͼָ�롣
 //ע���������Ѿ�����ջ�ȫ����װǶ�׵�˳��ҿ���ϣ����ԣ�Ҫ��װ��ȡ��ջ��ķ�ʽ���з�ջ����и������߸�����
 void GeomForCut::CheckCutGuideLINE(NestResultDataNode*head)
 {
	 //����ѭ�����Ե�һ���ջ����ֵܷ�ջ�֮��ı�����ѭ������ÿһ����һ���ջ����ӷ�ջ�Ϊ��ѭ����
	 GeomCloseHEAD*pFtemp;//����ָ����һ���ջ�ͷ���
	 bool m_IfHaveKidCH;//��¼ĳ����ջ��Ƿ����ӷ�ջ���
	 GeomCloseHEAD*pKtemp;//�洢�ڲ��Ӳ��ջ�ͷ���
	 bool m_IfCGLIeter = false;//�洢�Ƿ��и������߸����ж�ֵ,��ʼ��Ϊδ����
	 pFtemp = head->FirstGeomClose;
	 int a=0;
	 //��һ��ѭ������ѭ��
	 while (pFtemp)//��һ���ջ����ѭ�������������һ��
	 {
		 //�жϷ�ջ��Ƿ����ӷ�ջ�
		 m_IfHaveKidCH = IfIncludeKidClose(pFtemp);
		 if (!(m_IfHaveKidCH))//���û���ӷ�ջ�
		 {
			 //�ж�Ҫ��ѭ������⣬ż�������жϵ�ԭ��
				 m_IfCGLIeter = CheckCGLineInter(pFtemp, m_ceramic_Head);
				 //������˵���и��棬����Ҫ�����������������ѭ��
				 while (m_IfCGLIeter)
				 {
					 a++;
					 //�ж�Ҫ��ѭ������⣬ż�������жϵ�ԭ��
					/* m_CutLeadLine.ChangeCGLine(pFtemp);
					 m_IfCGLIeter = CheckCGLineInter(pFtemp, m_ceramic_Head);*/
					 m_IfCGLIeter = false;
				 }
			
		 }
		 else//������ӷ�ջ��������
		 {
			 CheckFKCutGuideLINE(pFtemp);
		 }
		 pFtemp = pFtemp->nextGeomcloseNode;
	 }
 }
 //����ÿһ������ջ���������������з�ջ��и��������Ƿ���ʡ�
 void GeomForCut::CheckFKCutGuideLINE(GeomCloseHEAD*pFtemp)
 {
	 GeomCloseHEAD*pKtemp;//����ָ���ӷ�ջ�ͷ���
	 bool m_IfHaveKidCH;//��¼ĳ����ջ��Ƿ����ӷ�ջ���
	 bool m_IfCGLIeter = false;//�洢�Ƿ��и������߸����ж�ֵ,��ʼ��Ϊδ����
	 //�ж�Ҫ��ѭ������⣬ż�������жϵ�ԭ��
	 m_IfCGLIeter = CheckCGLineInter(pFtemp, m_ceramic_Head);
	 //�����˾�Ҫ���ò��ܳ�ȥ
	 while (m_IfCGLIeter)
	 {
		 //�ж�Ҫ��ѭ������⣬ż�������жϵ�ԭ��
		 m_CutLeadLine.ChangeCGLine(pFtemp);
		 m_IfCGLIeter = CheckCGLineInter(pFtemp, m_ceramic_Head);
	 }

	 pKtemp = pFtemp->FirstInsideGCloseNode;
	 //�ڶ���ѭ��������ڲ��ӷ�ջ�����ż�Խ��е���,Ҳ�Ƕ�ͬһ����е���
	 while (pKtemp)
	 {
		 //�ж��ӷ�ջ����ֵܷ�ջ��Ƿ����ӷ�ջ�
		 m_IfHaveKidCH = IfIncludeKidClose(pKtemp);
		 if (!(m_IfHaveKidCH))//���û���ӷ�ջ�
		 {
			 //�����˾�Ҫ���Լ������ò��ܳ�ȥ
			 m_IfCGLIeter = CheckCGLineInter(pFtemp, m_ceramic_Head);
			 while (m_IfCGLIeter)
			 {
				 //�ж�Ҫ��ѭ������⣬ż�������жϵ�ԭ��
					 m_CutLeadLine.ChangeCGLine(pFtemp);
					 m_IfCGLIeter = CheckCGLineInter(pFtemp, m_ceramic_Head);
			 }
			
		 }
		 else//������ӷ�ջ��������
		 {
			 CheckFKCutGuideLINE(pKtemp);
		 }
		 pKtemp = pKtemp->nextGeomcloseNode;//����Եݹ鷽�̣������ײ�ķ�ջ��������ˣ����Զ�������һ�����ѭ����
	 }
 }



  //�����ջ����ж����и���������������ջ��Ƿ��и��棬������򷵻ط��ͷ��㣬û���򷵻�NULL
 //�жϵ�ԭ��1���κ�һ����ջ���Ҫ��һ�����˫�������жϣ���������������ڵģ�ż��������һ���ӷ�ջ����ڵģ�JudgeCGLineVSGeomclosedH
 //�жϵ�ԭ��2���κ�һ����ջ���Ҫ��һ����ջ������жϣ����������һ��ĸ���ջ����ķ�ջ���ż�������������ջ��жϣ�
 //�������ͬ���ջ�֮�䲻���ڸ��棬��ô��Ҫ���ݷ�ջ�����ż���ж�
  //�ж�ԭ������������жϣ���ô��Ҫ�ж��Ƿ��븸��ջ�Ҳ�и��棬�����ǵ�һ�㸸��ջ�˫���������Ҫ�ж��Ƿ������������и���
  //�ж�ԭ����ż�������жϣ���ô��Ҫ�ж��Ƿ����ڲ��ӷ�ջ�˫������Ҳ�и��������뱾������
  bool GeomForCut::CheckCGLineInter(GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head)
  {
	  bool m_IfCGLIeter = false;//�洢�Ƿ��и������߸����ж�ֵ,��ʼ��Ϊδ����
	  GeomCloseHEAD*ptemp;//���ɽڵ�
	  GeomCloseHEAD*pHtemp;//�洢�����ջ��������������ջ��������ջ�
	  GeomEleNode*pCGLinetemp;//�洢��ǰ��ջ����и�������
	  bool m_Singlelayer ;
	  //���жϷ�ջ��뱾�����ڷ�ջ�˫����������ջ������ӷ�ջ�˫�������Ƿ��и��棨ż���ջ�����
	 //��һ����ջ�˫����������ж�
	  m_IfCGLIeter = m_CutLeadLine.JudgeCGLineVsGeomclosedH(pCHtemp, m_ceramic_Head);
	  if (m_IfCGLIeter)//����������ջ�˫�������и��棨����ջ�),�����Ӳ��ջ�˫�������и��棨ż���ջ���
	  {
		  return true;//ֱ���˳�������Ҫ������һ�����ж��ˡ�
	  }
	  //˵������û�и��棬��ô��Ҫ�����븸���ջ�������ջ����������������ջ�������ջ����������ջ������ж��ˣ�ż���ջ���
	  else//����ֻ�뵥����һ����ջ������ж�
	  {
		  m_Singlelayer = pCHtemp->m_Singlelayer;
		  if (m_Singlelayer)//�ܽ�����˵��������ջ�����ô��Ҫ���⿼��
		  {
			//��Ҫ�ж����Ƿ�Ϊ�ײ㸸��ջ�(Ҫ���ֵܷ�ջ�˫��������׽ڵ����жϣ�
			  ptemp = pCHtemp;
			  if (ptemp->prevGeomcloseNode)//��������׷�ջ����
			  {
				  while (ptemp->prevGeomcloseNode)//�ҵ����׽ڵ��ջ�
				  {
					  ptemp = ptemp->prevGeomcloseNode;
				  }
			  }
			  //�����Ѿ���ptemp����Ϊ��ջ�˫��������׽ڵ�
			  //���ͬʱû�и���ջ���ֻ��ƽ���и�ͼ���ӷ�ջ�
			  if (!(ptemp->prevOutsideGCloseNode))
			  {
				  pHtemp = m_ceramic_Head;//���������
			  }
			  else
			  {
				  pHtemp = ptemp->prevOutsideGCloseNode;//����Ϊ����ջ��ĸ����ջ�
			  }
			 
		  }
		  else//ż���ջ�,��ô���Ƕ��Լ���������ж�
		  {
			  pHtemp = pCHtemp;
		  }
		  pCGLinetemp = m_CutLeadLine.GetLimtOfAuxiliaryLine(pCHtemp);
		  //���뵱ǰ��ջ����и������ߣ�����Ҫ�����жϵķ�ջ����ж����и��������Ƿ��и��档
		  m_IfCGLIeter = m_CutLeadLine.JudgeCGLineVsOneClosedHead(pCGLinetemp, pHtemp, m_ceramic_Head);
		  if (m_IfCGLIeter)//�������ķ�ջ�����һ�㸸���ջ��и��棨����ջ�),�����ջ��и��棨ż���ջ���
		  {
			  return true;//ֱ���˳�������Ҫ������һ�����ж��ˡ�
		  }
	  }
	  //������ܵ����ˣ�˵�������ĸ����ж϶����ɹ���Ҳ����û�и��档
	  return false;
  }
  /////////////////////////////////////////////////////////////////////////////////////////////
  ///////////������и����������ɷ�ʽ�ǽ�ƽ�����Լ��������߷�//////////////////////////////////////
  ///////////������и����������ɷ�ʽ��Բ���������߷�//////////////////////////////////////
  //����һ����ջ�ͷ��㣬֮��Դ˷�ջ�ͷ�����������и�������
  void  GeomForCut::CreatCutAuxiliaryPath(GeomCloseHEAD*pCHtemp)
  {
	  //��������и������ߵķ�������ʹ��ԭ����ֱ�߻���Բ������ϣ�֮��ֱ�����и������ߣ���˻�������ͼԪ��
	  //����Ҫ�����������ڴ档
	 
	  Line_para m_cutline;//�и������ߵĺ��Ĳ���
	 
	  int m_TypeGemoEle;//��Ϊ�и������ߵ��������ջ�����ɻ���ͼԪ�йأ�����ֱ������Ϊ1��Բ������Ϊ2��Բ������Ϊ3.
	  int m_TypeCGLine = 1;//Ϊ�����и������ߵ����㷨�������и������߷�ʽ�������֣�����������m_TypeCGLineΪ1���ӵ����Ǳ߽����m_TypeCGLineΪ2.
	  //ֻ����ֱ�����͵�ͼԪ,û�п���Բ��Բ����Ҳû�п����Ƿ��и��������н��档
	  //��ȡ����ͼԪ���ͣ�Ϊ�����и������������ṩ������
	  m_TypeGemoEle = pCHtemp->FirstGeomele->m_GeomStandData.m_typegeomele;//����ͼԪ������һ�ֻ������ԡ�
	  //����ͼԪ���͵��ò�ͬ���и�������������ʽ
	  //����ֱ���͵��Ѿ������
	  //Բ�͵Ŀ������������߷�������
	  //Բ���͵Ŀ�����������Բ��Ȼ�����ֱ�ߵ�������������ʽ��
	  switch (m_TypeGemoEle)
	  {
	  case 1:
		  //ֱ���͵Ļ���ͼԪ�и������ߴ��졣
		  CreatCutAuxiliaryPath_Polygon(pCHtemp, m_TypeCGLine);
		  break;
	  case 2:
		 
		  break;
	  case 3:
		  //Բ���͵Ļ���ͼԪ�и������ߴ��졣
		  CreatCutAuxiliaryPath_Circle(pCHtemp, m_TypeCGLine);
		  break;

	  default:
		  break;
	  }
  }
  //�������Բ��֮���ϵ�Բ���и����������ɷ�ʽ�����и�������ֱ���ڴ����ɣ���������ⲿ�ӿڡ�
  void GeomForCut::CreatCutAuxiliaryPath_Polygon(GeomCloseHEAD*pCHtemp, int m_TypeCGLine)
  {
	  a++;
	  Point P_0, P_1;//����ֱ���ͷ�ջ��Ķ˵�����
	  Point P_mid;//����ֱ���ͷ�ջ����е�
	  Point	P_Arccent_out, P_Arccent_in;//out��ָ�������ⲿ��Բ��Բ�ģ�in����ָ�������ڲ���Բ��Բ�ġ�
	  Point P_Arccent_use = { -9999, -9999 };//ʹ���˵�Բ���е�
	  GeomEleNode*Fnode = NULL;
	  GeomEleNode*Enode = NULL;
	  Line_para m_startline, m_endline;//��ջ���ͼԪ�ڵ��βͼԪ�ڵ�Ļ�������
	  Line_para m_line;//�и������ߵ�Բ������ĸ��
	  Line_para m_arc_auxi_SEP = { -9999, -9999, -9999, -999, -9999 };//����Բ���и������ߵ���ֹ�˵㡣
	  double m_startline_k;//��ͼԪ��б��
	  double m_auxiliary_k;//Բ��Բ������ֱ�ߵ�б�ʣ�����һ��б�����Ϊ-1����ֱ�ߴ�ֱ
	  double m_x_min, m_x_max, m_y_min, m_y_max;//ͼ�εİ�����εĿ��ƽǵ�
	  Arc_Point P_Arccent;
	  bool m_Singlelayer;
	  int m_TypeCGLine_local;//��API����ֵ���������������������жϣ���ֹ���������ѭ����ʱ�򣬸ı���ֵ��
	  //�㷨����
	  m_Singlelayer = pCHtemp->m_Singlelayer;//��ջ�����ż�㣻
	  Fnode = pCHtemp->FirstGeomele;
	  P_0.x = Fnode->m_GeomStandData.GeoEle_start_x0;
	  P_0.y = Fnode->m_GeomStandData.GeoEle_start_y0;
	  P_1.x = Fnode->m_GeomStandData.GeoEle_start_x1;
	  P_1.y = Fnode->m_GeomStandData.GeoEle_start_y1;
	  //�����е�����
	  P_mid.x = (P_0.x + P_1.x) / 2;
	  P_mid.y = (P_0.y + P_1.y) / 2;
	  //�Ը��е�����ΪԲ����ֱ�ߵ��е㣬�����Ըõ������ֱ�߶ν��л��֣��ֳ�����
	  Enode = Fnode->nextGeomeleNode;
	  while (Enode->nextGeomeleNode)//�ҵ����һ���ڵ�
		  Enode = Enode->nextGeomeleNode;
	  //�Է�ջ���ͷ��㣬ֱ�ߵ��е㣬�Լ��и����������Խ��봴���и������ߡ�
	  //ֱ������ͼԪ�ڵ�������˵�
	  m_startline.x0 = Fnode->m_GeomStandData.GeoEle_start_x0;
	  m_startline.y0 = Fnode->m_GeomStandData.GeoEle_start_y0;
	  m_startline.x1 = Fnode->m_GeomStandData.GeoEle_start_x1;
	  m_startline.y1 = Fnode->m_GeomStandData.GeoEle_start_y1;
	  //����ֱ������ͼԪ�ڵ��б��
	  m_startline_k = m_CutLeadLine.Get_k_twoPoint(m_startline);
	  //����Բ��Բ������ֱ�ߵ�б��,������֮��б�����Ϊ-1
	  //ע��������ܻ����Ҫ�ж�m_startline_k�Ƿ�Ϊ0
	  m_auxiliary_k = -1 / m_startline_k;
	  //��������б�ʣ�Ҳ���˵㣬�������õ�бʽ����y=kx+b
	  //��Բ���и�������Բ������ֱ�ߵ�ĸ��
	  m_line = m_CutLeadLine.Get_CutLine_Point(P_mid, m_auxiliary_k);
	//��ֵ���ݹ�����
	  m_TypeCGLine_local = m_TypeCGLine;
	  //�����и�Բ��������
	  if (1 == m_TypeCGLine_local)//���������и�������
	  {
		  P_Arccent = GetArccentPoint(pCHtemp, m_line, P_mid);
		  //��Ҫ�ж��Ƿ��Ѿ�ѡ���˺��ʵ�Բ��Բ�ģ���Ϊ�п����и�ĸ�߲��ʺϣ�����û��ѡ��
		  if (P_Arccent.If_select)
		  {
			  //�Ƚ�Բ�����������ĵ�����
			  P_Arccent_out = P_Arccent.P_arc_out;
			  P_Arccent_in = P_Arccent.P_arc_in;
			 
			  //������ż����ѡ��
			  if (m_Singlelayer)
			  {
				  //����ջ�����ô��ѡ�������Բ��
				  //����Բ����Բ�ģ��Լ���ջ�ֱ�߶ε�б�ʡ�
				  //���Բ���������˵㣬����ǰ��һ���˵��xֵ�Ƚ�С��
				  m_arc_auxi_SEP = m_CutLeadLine.Get_CutLine_Point(P_Arccent_out, m_startline_k);
				  P_Arccent_use = P_Arccent_out;
			  }
			  else
			  {
				  //ż���ջ���ѡ���ڲ��Բ��
				  //����Բ����Բ�ģ��Լ���ջ�ֱ�߶ε�б�ʡ�
				  //���Բ���������˵㣬����ǰ��һ���˵��xֵ�Ƚ�С��
				  m_arc_auxi_SEP = m_CutLeadLine.Get_CutLine_Point(P_Arccent_in, m_startline_k);
				  P_Arccent_use = P_Arccent_in;
			  }
		  }
		  //���û��ѡ����Ҫ����ѡ�񣬽�Ҫ���ڶ�������ͼԪ��ɵ�һ��ele.
		  else
		  {
			  pCHtemp = ChangeSecendEleToFirst(pCHtemp);
			  //����������Ĺ���������һ��
			  CreatCutAuxiliaryPath_Polygon(pCHtemp, m_TypeCGLine);
		  }
		  //�����Ѿ������Բ�������ߵ��������Ƶ㣬����û��ת����Բ����Ҳû�н���ջ��ĵĽṹ�������Լ���Բ�������ȥ
		 //m_arc_auxi_SEPԲ���������г��˵� ����ֱ�ߵ��е㣬Բ��Բ�ģ���ջ�
		  //���и���������ӵ���ջ���ȥ�����ҵ�����ջ��Ļ�Ԫ��
		  SetCutAuxiliaryPath(m_arc_auxi_SEP, P_mid, P_Arccent_use, pCHtemp);
	 
	  }
	  else//��ʱm_TypeCGLine=2;Ҫ���յ����и������ߵķ�ʽ����
	  {
		  ;
	  }
	 
  }
  //Բ���͵��и������ߵ����ɷ�ʽ�����и�������ֱ���ڴ����ɣ������ṩ�ⲿ�ӿڡ�
  void  GeomForCut::CreatCutAuxiliaryPath_Circle(GeomCloseHEAD*pCHtemp, int m_TypeCGLine)
  {
	  Point P_CHCircle;//����Բ��ջ���Բ������
	  Point P_mid;//����ֱ���ͷ�ջ����е�,ͬʱҲ��Բ��ջ�ԭ�����и���Ƶ㣻
	  Point	P_Arccent_out, P_Arccent_in;//out��ָ�������ⲿ��Բ��Բ�ģ�in����ָ�������ڲ���Բ��Բ�ġ�
	  Point P_Arccent_use = { -9999, -9999 };//ʹ���˵�Բ���е�
	  GeomEleNode*Fnode = NULL;
	  Line_para m_startCircle;//��ջ���ͼԪ�ڵ��βͼԪ�ڵ�Ļ�������
	  Line_para m_line;//�и������ߵ�Բ������ĸ��
	  Line_para m_arc_auxi_SEP = { -9999, -9999, -9999, -9999, -9999 };//����Բ���и������ߵ���ֹ�˵㡣
	  int m_belong;//�洢Բ����Բ���е����ڵ�����1ΪAB,2ΪBC,3ΪCD,4ΪDA��
	  double m_auxiliary_k;//Բ��Բ������ֱ�ߵ�б�ʣ�����Բ�����и���Ƶ��ֱ�ߵ�б�ʡ�
	  double m_Tangentline_k;//Բ���ߵ�б�ʣ�Ҫ�������б��������Բ�������ߵ���ֹ�˵㡣����һ��б�����Ϊ-1����ֱ�ߴ�ֱ
	  double m_x_min, m_x_max, m_y_min, m_y_max;//ͼ�εİ�����εĿ��ƽǵ㣬��ԲҲ��Ҫ��ô����
	  Arc_Point P_Arccent;
	  bool m_Singlelayer;
	  int m_TypeCGLine_local;//��API����ֵ���������������������жϣ���ֹ���������ѭ����ʱ�򣬸ı���ֵ��
	  //�㷨����
	  m_Singlelayer = pCHtemp->m_Singlelayer;//��ջ�����ż�㣻
	  Fnode = pCHtemp->FirstGeomele;
	  //�����е����꣬��ʵ����Բ�����е��и���Ƶ����ꡣ�������и�������ʱ�򣬽����ǽ������任��
	  P_mid.x = Fnode->m_GeomStandData.GeoEle_start_x0;
	  P_mid.y =  Fnode->m_GeomStandData.GeoEle_start_y0;
	  //�Ը��е�����ΪԲ����ֱ�ߵ��е㣬�����Ըõ������Բ���и������߽��л��֣��ֳ�����
	  //��Բ��ջ����и���Ƶ㣬�����е㣬�Լ�Բ�Ĵ���Բ���и������ߡ�
	  //ֱ������ͼԪ�ڵ�������˵�
	  //��Բ����Ϊx0,y0,���е���Ϊx1,y1��ĸ�ߵ�б�ʡ�
	  m_startCircle.x0 = Fnode->m_GeomStandData.m_circle.m_Circent_x;
	  m_startCircle.y0 = Fnode->m_GeomStandData.m_circle.m_Circent_y;
	  m_startCircle.x1 = Fnode->m_GeomStandData.GeoEle_start_x0;
	  m_startCircle.y1 = Fnode->m_GeomStandData.GeoEle_start_y0;
	  //����ֱ������ͼԪ�ڵ��б��
	  m_auxiliary_k = m_CutLeadLine.Get_k_twoPoint(m_startCircle);
	  //����Բ��Բ������ֱ�ߵ�б��,������֮��б�����Ϊ-1
	  //ע��������ܻ����Ҫ�ж�m_auxiliary_k�Ƿ�Ϊ0
	  //������б����Ϊ�˺�����Բ��ֱ�߽����õġ�
	  m_Tangentline_k = -1 / m_auxiliary_k;
	  //��������б�ʣ�Ҳ���˵㣬�������õ�бʽ����y=kx+b
	  //��Բ���и�������Բ������ֱ�ߵ�ĸ�ߣ���Ϊ�˺�����Բ����׼��
	  m_line = m_CutLeadLine.Get_CutLine_Point(P_mid, m_auxiliary_k);
	  //��ֵ���ݹ�����
	  m_TypeCGLine_local = m_TypeCGLine;
	  //�����и�Բ��������
	  if (1 == m_TypeCGLine_local)//���������и�������
	  {
		  P_Arccent = GetArccentPoint(pCHtemp, m_line, P_mid);
		
		  //��Ҫ�ж��Ƿ��Ѿ�ѡ���˺��ʵ�Բ��Բ�ģ���Ϊ�п����и�ĸ�߲��ʺϣ�����û��ѡ��
		  if (P_Arccent.If_select)
		  {
			  //�Ƚ�Բ�����������ĵ�����
			  m_belong = P_Arccent.m_belong;
			  P_Arccent_out = P_Arccent.P_arc_out;
			  P_Arccent_in = P_Arccent.P_arc_in;

			  //������ż����ѡ��
			  if (m_Singlelayer)
			  {
				  //����ջ�����ô��ѡ�������Բ��
				  //����Բ����Բ�ģ��Լ����ߵ�б�ʡ�
				  //���Բ���������˵㣬����ǰ��һ���˵��xֵ�Ƚ�С��
				  m_arc_auxi_SEP = m_CutLeadLine.Get_CutLine_Point(P_Arccent_out, m_Tangentline_k);
				  P_Arccent_use = P_Arccent_out;
			  }
			  else
			  {
				  //ż���ջ���ѡ���ڲ��Բ��
				  //����Բ����Բ�ģ��Լ���ջ�ֱ�߶ε�б�ʡ�
				  //���Բ���������˵㣬����ǰ��һ���˵��xֵ�Ƚ�С��
				  m_arc_auxi_SEP = m_CutLeadLine.Get_CutLine_Point(P_Arccent_in, m_Tangentline_k);
				  P_Arccent_use = P_Arccent_in;
			  }
		  }
		  //���û��ѡ����Ҫ����ѡ�񣬽�Ҫ���ڶ�������ͼԪ��ɵ�һ��ele.
		  else
		  {
			  pCHtemp = ChangeSecendEleToFirst(pCHtemp);
			  //����������Ĺ���������һ��
			  CreatCutAuxiliaryPath_Polygon(pCHtemp, m_TypeCGLine);
		  }
		  //�����Ѿ������Բ�������ߵ��������Ƶ㣬����û��ת����Բ����Ҳû�н���ջ��ĵĽṹ�������Լ���Բ�������ȥ
		  //m_arc_auxi_SEPԲ���������г��˵� ����ֱ�ߵ��е㣬Բ��Բ�ģ���ջ�
		  //���и���������ӵ���ջ���ȥ�����ҵ�����ջ��Ļ�Ԫ��
		  SetCutAuxiliaryPath(m_arc_auxi_SEP, m_belong,  P_Arccent_use, pCHtemp);

	  }
	  else//��ʱm_TypeCGLine=2;Ҫ���յ����и������ߵķ�ʽ����
	  {
		  ;
	  }

  }

  //��Ϊ��һ������ͼԪ�Ĵ��߲��ϸ���Ҫ�ڶ�������ͼԪ��ɵ�һ������ͼԪ
  GeomCloseHEAD* GeomForCut::ChangeSecendEleToFirst(GeomCloseHEAD*pCHtemp)
  {
	  GeomEleNode*FirstPE,*SecendPE,*EndPE;
	  FirstPE = pCHtemp->FirstGeomele;
	  EndPE = pCHtemp->FirstGeomele;
	  EndPE = SecendPE = FirstPE->nextGeomeleNode;
	  while (EndPE->nextGeomeleNode)
		  EndPE = EndPE->nextGeomeleNode;//�ҵ����һ������ͼԪ
	  //������ջ�����Ļ���ͼԪλ�ã�
	  //�ڶ����ŵ���ǰ
	  SecendPE->prevGeomeleNode = NULL;
	  pCHtemp->FirstGeomele = SecendPE;
	  //��һ���ŵ����
	  FirstPE->prevGeomeleNode = EndPE;
	  FirstPE->nextGeomeleNode = NULL;
	  EndPE->nextGeomeleNode = FirstPE;
	  return pCHtemp;

  }
  //���Ĵ���
  //���Ĵ���  
   //���Ĵ���
  //�����ջ������и�������ĸ�ߣ���Բ��Բ�����ꡣ
  Arc_Point GeomForCut::GetArccentPoint(GeomCloseHEAD*pCHtemp, Line_para m_line, Point P_mid)
  {
	  Point P_x_min, P_x_max, P_y_min, P_y_max;//��ջ��İ�����ο��ƽǵ����꣬�������ɾ��ΰ��������Լ����ڵĸ������깹��
	  Arc_Point P_arccent = { -9999, -9999, -9999, -9999, false ,-9999};
	  int m_belong = -9999;//�ж��е�֮ǰ�Ƿ��Ѿ��ҵ���ص��������û�о����ֵ������о���1��
	  //��÷�ջ��ľ��ΰ�����ƽǵ�
	  P_x_min.x = pCHtemp->m_GemoClosedLimtPoint.x_min.x;
	  P_x_min.y = pCHtemp->m_GemoClosedLimtPoint.x_min.y;
	  P_x_max.x = pCHtemp->m_GemoClosedLimtPoint.x_max.x;
	  P_x_max.y = pCHtemp->m_GemoClosedLimtPoint.x_max.y;
	  P_y_min.x = pCHtemp->m_GemoClosedLimtPoint.y_min.x;
	  P_y_min.y = pCHtemp->m_GemoClosedLimtPoint.y_min.y;
	  P_y_max.x = pCHtemp->m_GemoClosedLimtPoint.y_max.x;
	  P_y_max.y = pCHtemp->m_GemoClosedLimtPoint.y_max.y;
	  //���е����ڵ��������жϸô�Ӧ����ôѡ��Բ����Բ������
	  //ԭ��������е�λ�ھ��ΰ����x_min,��x_max���ߵķ�Χ֮�ϣ�y_min,��y_max���ߵķ�Χ֮��Ҳ���ǵڶ����ޣ�����ôy����xС���Ǹ������ⲿ����
	  //���ĳ���㲻��ͬʱ�߱�y��xС������yСx����ô����㲻��ȷ���ǲ��������棬Ҫ������������һ��ֱ�߱����ȷ���ˡ�
	  //���շ�ջ���ż����Ϣ�����и�������
	  //Ҫ���κ�һ����ջ������ĸ����ƽǵ㽫�仮��Ϊ�Ĳ���
	  //����ΪA,��ΪB����ΪC����ΪD;(����������Σ������һ���ߣ���һ���ǣ�����Ӧ��Ҳ��ͬ����ȡ���������д�ġ���
	  //�е�����AB֮��
	  if ((P_x_min.x < P_mid.x) && (P_x_min.y < P_mid.y) && (P_mid.x < P_y_max.x) && (P_mid.y < P_y_max.y))
	  {
		  //�ⲿ��Ӧ����y��ͬʱxС,�����ĵ�Ϊ�ⲿ����ĵ�
		  //m_line������������Ѿ���������ģ�x0�Ǳ�����Ϊ��x1С������
		  //m_line.x0 <= m_line.x1��Ĭ�ϵģ��ٸ��ж�����Ҳ�ܽ�ʡ����
		  //ֻ����������y�Ǵ�Ĳ���ʱ��
		  if (m_line.y0 >= m_line.y1)
		  {
			  //�ܽ�����˵���Ƿ��Ϲ���
			  if (m_belong == -9999)//˵�����е���δ���������κ�һ������
			  {
				  m_belong =  1;//��AB֮��͸�ֵΪ1
				  P_arccent.P_arc_out.x = m_line.x0;
				  P_arccent.P_arc_out.y = m_line.y0;
				  P_arccent.P_arc_in.x = m_line.x1;
				  P_arccent.P_arc_in.y = m_line.y1;
				  P_arccent.m_belong = m_belong;
				  P_arccent.If_select = true;
			  }
			  else//��Ϊ�����ʼ���жϣ�Ҫô��������Ҫô��������-9999
			  {
				  P_arccent.If_select = false;
			  }
			 
		  }
		  //�������������С�ģ���ô˵������и������ߵ�ĸ�������ﲻ����
		  else
		  {
			  P_arccent.If_select = false;
		  }
	  }
		  //�е�����BC֮��
	 // else if ((P_y_max.x < P_mid.x) && (P_mid.y <P_y_max.y) && (P_mid.x < P_x_max.x) && (P_x_max.y< P_mid.y))
	  //�˴�����if else ���ָ�ʽ����Ϊ���ⲻ��ֻ������һ�����ܣ����ǣ����ܻ����ͬʱ�����������������
	  //���Զ�Ҫ�жϡ�
	   if ((P_y_max.x < P_mid.x) && (P_mid.y <P_y_max.y) && (P_mid.x < P_x_max.x) && (P_x_max.y< P_mid.y))
	  {
		  //��һ��������xyͬʱ��������Ϊ�ⲿ����
		  //m_line������������Ѿ���������ģ�x0�Ǳ�����Ϊ��x1С������
		  //m_line.x0 <= m_line.x1��Ĭ�ϵģ��ٸ��ж�����Ҳ�ܽ�ʡ����
		  if (m_line.y0<=m_line.y1)
		  {
			  //�ܽ�����˵���Ƿ��Ϲ���
			  if (m_belong == -9999)//˵�����е���δ���������κ�һ������
			  {
				  m_belong = 2;//��BC֮��͸�ֵΪ2
				  P_arccent.P_arc_out.x = m_line.x1;
				  P_arccent.P_arc_out.y = m_line.y1;
				  P_arccent.P_arc_in.x = m_line.x0;
				  P_arccent.P_arc_in.y = m_line.y0;
				  P_arccent.m_belong = m_belong;
				  P_arccent.If_select = true;
			  }
			  else//˵�����Ѿ����ı���ֵ����ô����ͬʱ������������
			  {
				  P_arccent.If_select = false;
			  }
			  
		  }
		  else
		  {
			  P_arccent.If_select = false;
		  }
	  }
	  //����Ǵ���CD��
	 // else if ((P_y_min.x < P_mid.x) && (P_y_min.y <P_mid.y) && (P_mid.x < P_x_max.x) && (P_mid.y<P_x_max.y))

	   if ((P_y_min.x < P_mid.x) && (P_y_min.y <P_mid.y) && (P_mid.x < P_x_max.x) && (P_mid.y<P_x_max.y))
	  {
		  //��һ������yСx��Ϊ�ⲿ
		  //m_line������������Ѿ���������ģ�x0�Ǳ�����Ϊ��x1С������
		  //m_line.x0 <= m_line.x1��Ĭ�ϵģ��ٸ��ж�����Ҳ�ܽ�ʡ����
		  if (m_line.y1<=m_line.y0)
		  {
			  //�ܽ�����˵���Ƿ��Ϲ���
			  if (m_belong == -9999)//˵�����е���δ���������κ�һ������
			  {
				  m_belong = 3;//��BC֮��͸�ֵΪ3
				  P_arccent.P_arc_out.x = m_line.x1;
				  P_arccent.P_arc_out.y = m_line.y1;
				  P_arccent.P_arc_in.x = m_line.x0;
				  P_arccent.P_arc_in.y = m_line.y0;
				  P_arccent.m_belong = m_belong;
				  P_arccent.If_select = true;
			  }
			  else//˵�����Ѿ����ı���ֵ����ô����ͬʱ������������
			  {
				  P_arccent.If_select = false;
			  }
			 
		  }
		  else
		  {
			  P_arccent.If_select = false;
		  }
	  }
	  //����Ǵ���DA��
	//  else if ((P_mid.x <P_y_min.x) && (P_y_min.y <P_mid.y) && (P_x_min.x< P_mid.x) && (P_mid.y<P_x_min.y))

	   if ((P_mid.x <P_y_min.x) && (P_y_min.y <P_mid.y) && (P_x_min.x< P_mid.x) && (P_mid.y<P_x_min.y))
	  {
		  //��һ������xyͬʱΪСΪ�ⲿ
		  //m_line������������Ѿ���������ģ�x0�Ǳ�����Ϊ��x1С������
		  //m_line.x0 <= m_line.x1��Ĭ�ϵģ��ٸ��ж�����Ҳ�ܽ�ʡ����
		  if (m_line.y0 <= m_line.y1)
		  {
			  //�ܽ�����˵���Ƿ��Ϲ���
			  if (m_belong == -9999)//˵�����е���δ���������κ�һ������
			  {
				  m_belong = 4;//��DA֮��͸�ֵΪ4
				  P_arccent.P_arc_out.x = m_line.x0;
				  P_arccent.P_arc_out.y = m_line.y0;
				  P_arccent.P_arc_in.x = m_line.x1;
				  P_arccent.P_arc_in.y = m_line.y1;
				  P_arccent.m_belong = m_belong;
				  P_arccent.If_select = true;
			  }
			  else//˵�����Ѿ����ı���ֵ����ô����ͬʱ������������
			  {
				  P_arccent.If_select = false;
			  }
			  
		  }
		  else
		  {
			  P_arccent.If_select = false;
		  }
	  }
	  //���Ͻ����������Բ�������������ֳ�����
	  return P_arccent;

  }

  //������������Բ���и������ߵĲ���������Բ���и���������ӵ���ջ��ϡ�
  //����Բ������ֹ�㣬Բ�����ջ����е㣬Բ����Բ�ģ���ջ���
  //����εķ�ջ�������������������и�������
  void GeomForCut::SetCutAuxiliaryPath(Line_para m_arc_auxi_SEP, Point P_mid, Point P_arccent, GeomCloseHEAD*pCHtemp)
  {
	   AuxiliarPath m_auxiliaryPath;//�������渨���ߵ�
	   GARC m_arc_auxipath_in;//Բ���и�������
	   GARC  m_arc_auxipath_out;//Բ���и�������
	  GCIRCLE m_circle;
	  Point m_GeleStartP, m_GeleEndP;//��¼��ջ�Բ�����������ڵ�ֱ����������յ����Ϣ
	  Point m_Pstrat, m_Pend;//�洢Բ���и������ߵ���ֹ�˵�
	  double m_ArcAuxiStartAngle, m_ArcAuxiEndAngle;//Բ���и������ߵ���ֹ�Ƕ�
	  double m_P_midStartAngle, m_P_midEndAngle;//Բ����ֱ���е㴦����ֹ�Ƕȡ�һ����˵�������Ƕ���һ���ġ�
	 //Բ���и������ߵĻ�������
	  m_arc_auxipath_in.Arccent_x = m_arc_auxipath_out.Arccent_x = P_arccent.x;
	  m_arc_auxipath_in.Arccent_y = m_arc_auxipath_out.Arccent_y = P_arccent.y;
	  m_arc_auxipath_in.m_Arc_r = m_arc_auxipath_out.m_Arc_r = m_CutLineLength;
	  //���ǻ���һ���и���ʼ����һ���и���ֹ�ǣ����µĴ�������ڼ�����������
	  //////////////////////////////////////////////////////////////////////////
	  //Բ���и��������е�Բ����
	  m_circle.m_Circent_x = P_arccent.x;
	  m_circle.m_Circent_y = P_arccent.y;
	  m_circle.m_Circle_r = m_CutLineLength;
	  //�ٶ�Բ���и�����������x��С��Բ���˵㿪ʼ����
	  //m_arc_auxi_SEP����Ĳ������Ѿ���������ģ�x0��x1��С��
	  //��ȡm_arc_auxi_SEP��ǰ��һ����Ϊ��㡣
	  m_Pstrat.x = m_arc_auxi_SEP.x0;
	  m_Pstrat.y = m_arc_auxi_SEP.y0;
	  m_Pend.x = m_arc_auxi_SEP.x1;
	  m_Pend.y = m_arc_auxi_SEP.y1;
	  //��Բ���и������߶�Ӧ���и���ʼ��
	  m_arc_auxipath_in.m_ArcAngle_start = m_geomele.ForCircleStartAngle(m_Pstrat.x, m_Pstrat.y, m_circle);
	  //��Բ����ֱ�ߵ��е���ΪԲ���и������ߵ��յ�
	  //��Բ���и������߶�Ӧ���и���ֹ��
	  m_arc_auxipath_in.m_ArcAngle_end = m_geomele.ForCircleStartAngle(P_mid.x, P_mid.y, m_circle);
	  //��Բ���и������߶�Ӧ����ʼ��
	  //m_arc_auxipath_in.m_ArcAngle_endͬʱҲ��Բ���и������ߵ���ʼ��
	  m_arc_auxipath_out.m_ArcAngle_start = m_arc_auxipath_in.m_ArcAngle_end;
	  //��Բ���и������߶�Ӧ����ֹ��
	  //��Բ�������˵��е���һ���˵�����
	  m_arc_auxipath_out.m_ArcAngle_end = m_geomele.ForCircleStartAngle(m_Pend.x, m_Pend.y, m_circle);
	  /////////////////////////////////////////////////////////////////////////////////////
	  //������ʵ��Բ���и��������Ƿ������Ԥ���е��и�˳��һ��
	  //��ȡԲ���и���������֮���е�ֱ�ߣ�����ջ����׻���ͼԪ�ڵ�
	  m_GeleStartP.x = pCHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
	  m_GeleStartP.y = pCHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
	  m_GeleEndP.x = pCHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_x1;
	  m_GeleEndP.y = pCHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_y1;
	  //���ݷ�ջ�ԭ�����и�˳��ȷ��Բ�������������г���
	  //ԭ�����Բ���Ķ˵�����������ֱ����ƽ�й�ϵ�����������������һ��
	  //����ջ�����ͼԪ�ڵ������x���յ��xС������Ԥ���е�Բ���и�˳��һ�£�������ǣ���պ��෴��
	  //���Ƕ�Ӧ���и���ǲ���ģ�ֻ��˵��ֹλ�ñ仯
	  //�պ���Ԥ���е��뷨�෴
	  if (m_GeleStartP.x >= m_GeleEndP.x)
	  {
		  //��Բ���и������ߵ���ʼ��������Ԥ���е�Բ���и������ߵ���ֹ��
		  m_arc_auxipath_in.m_ArcAngle_start = m_arc_auxipath_out.m_ArcAngle_end;
		  //��Բ���и������ߵ���ֹ���ǲ���ģ���Ϊ��ֹ�˵㲻��
		  //��Բ���и������ߵ���ʼ��Ҳ�ǲ���ģ���Ϊ��ʼ�˵㲻��
		  //Բ���и������ߵ���ֹ��������ԭ���и������ߵ���ʼ�ǣ���ԭ���ļ��㷽ʽ���㡣
		  m_arc_auxipath_out.m_ArcAngle_end = m_geomele.ForCircleStartAngle(m_Pstrat.x, m_Pstrat.y, m_circle);
	 //��ôԲ���������ҲҪ��˱�Ϊԭ��Բ�����г��㡢��������г��㶼Ҫ�仯
		  m_Pstrat.x = m_arc_auxi_SEP.x1;
		  m_Pstrat.y = m_arc_auxi_SEP.y1;
		  m_Pend.x = m_arc_auxi_SEP.x0;
		  m_Pend.y = m_arc_auxi_SEP.y0;
	  }
	  else//��������������֣������˵����ʵ�����ԭ���������һ���ġ����ù�
	  {
		  ;
	  }
	  //��Բ���и���������ӵ�һ��ͳһ�ĸ�ʽ���棬֮�󴫵���ӵĺ����
	  m_auxiliaryPath.m_IfLine = false;//false�Ǵ���Բ��
	  m_auxiliaryPath.m_arc_in = m_arc_auxipath_in;
	  m_auxiliaryPath.m_arc_out = m_arc_auxipath_out;
	  m_auxiliaryPath.P_AuxiStart = m_Pstrat;
	  m_auxiliaryPath.P_AuxiEnd = m_Pend;

	  //�����ǽ�Բ���и������ߵ��������������ߵĲ������Ѿ�������ˣ����ǲ�û�а�Բ���и������߱�Ϊ��ջ��Ĺ���һ��
	  //���Ҫ�õ�˫��������ҿ���
	  AddAuxiNodeToCH(m_auxiliaryPath, P_mid, pCHtemp);

  }
  //������������Բ���и������ߵĲ���������Բ���и���������ӵ���ջ��ϡ�
  //����Բ������ֹ�㣬Բ����Բ�ģ�Բ��ջ���
  //ע����������ĺ�������Բ�����ջ����е������
  //Բ�εķ�ջ�������������������и�������
  //Բ��ջ����ص����ڣ��Ҹ������䰴����ʱ��ķ�������и
  void GeomForCut::SetCutAuxiliaryPath(Line_para m_arc_auxi_SEP, int m_belong,Point P_arccent, GeomCloseHEAD*pCHtemp)
  {
	  AuxiliarPath m_auxiliaryPath;//�������渨���ߵ�
	  GARC m_arc_auxipath_in;//Բ���и�������
	  GARC  m_arc_auxipath_out;//Բ���и�������
	  GCIRCLE m_circle;
	  Point m_Pstrat, m_Pend;//�洢Բ���и������ߵ���ֹ�˵�
	  Point P_mid;//�洢Բ��ջ����и���Ƶ㣬Ҳ�����е㡣
	  double m_ArcAuxiStartAngle, m_ArcAuxiEndAngle;//Բ���и������ߵ���ֹ�Ƕ�
	  double m_P_midStartAngle, m_P_midEndAngle;//Բ����ֱ���е㴦����ֹ�Ƕȡ�һ����˵�������Ƕ���һ���ġ�
	  //Բ���и������ߵĻ�������
	  m_arc_auxipath_in.Arccent_x = m_arc_auxipath_out.Arccent_x = P_arccent.x;
	  m_arc_auxipath_in.Arccent_y = m_arc_auxipath_out.Arccent_y = P_arccent.y;
	  m_arc_auxipath_in.m_Arc_r = m_arc_auxipath_out.m_Arc_r = m_CutLineLength;
	  //���ǻ���һ���и���ʼ����һ���и���ֹ�ǣ����µĴ�������ڼ�����������
	  //////////////////////////////////////////////////////////////////////////
	  //Բ���и��������е�Բ����
	  m_circle.m_Circent_x = P_arccent.x;
	  m_circle.m_Circent_y = P_arccent.y;
	  m_circle.m_Circle_r = m_CutLineLength;
	  //Բ���и������ߵ��е�
	  P_mid.x = pCHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_x0;
	  P_mid.y = pCHtemp->FirstGeomele->m_GeomStandData.GeoEle_start_y0;
	  //�ٶ�Բ���и�����������x��С��Բ���˵㿪ʼ����
	  //m_arc_auxi_SEP����Ĳ������Ѿ���������ģ�x0��x1��С��
	  //��ȡm_arc_auxi_SEP��ǰ��һ����Ϊ��㡣
	  m_Pstrat.x = m_arc_auxi_SEP.x0;
	  m_Pstrat.y = m_arc_auxi_SEP.y0;
	  m_Pend.x = m_arc_auxi_SEP.x1;
	  m_Pend.y = m_arc_auxi_SEP.y1;
	  //��Բ���и������߶�Ӧ���и���ʼ��
	  m_arc_auxipath_in.m_ArcAngle_start = m_geomele.ForCircleStartAngle(m_Pstrat.x, m_Pstrat.y, m_circle);
	  //��Բ����ֱ�ߵ��е���ΪԲ���и������ߵ��յ�
	  //��Բ���и������߶�Ӧ���и���ֹ��
	  m_arc_auxipath_in.m_ArcAngle_end = m_geomele.ForCircleStartAngle(P_mid.x, P_mid.y, m_circle);
	  //��Բ���и������߶�Ӧ����ʼ��
	  //m_arc_auxipath_in.m_ArcAngle_endͬʱҲ��Բ���и������ߵ���ʼ��
	  m_arc_auxipath_out.m_ArcAngle_start = m_arc_auxipath_in.m_ArcAngle_end;
	  //��Բ���и������߶�Ӧ����ֹ��
	  //��Բ�������˵��е���һ���˵�����
	  m_arc_auxipath_out.m_ArcAngle_end = m_geomele.ForCircleStartAngle(m_Pend.x, m_Pend.y, m_circle);
	  /////////////////////////////////////////////////////////////////////////////////////
	  //����Բ���е����ڵ�������ѡ��Բ������ֹ�˵�
	  //��ΪԲ��ķ�ջ������㷨���趨��ÿ���и����ʱ���и��������Ϊ360��
	  //��Բ�����е���AB�μ��ǵڶ����ޣ���BC�μ��ǵ�һ����ʱ��Բ���и������ߵ�xΪ�ϴ�ֵ�㣻
	  //��Բ�����е���CD�μ��ǵ������ޣ���DA�μ��ǵ�������ʱ��Բ���и������ߵ�xΪ��Сֵ��,����������ļ���һ�¡�

	  if (1 == m_belong || 2 == m_belong)
	  {
		  //��Բ���и������ߵ���ʼ��������Ԥ���е�Բ���и������ߵ���ֹ��
		  m_arc_auxipath_in.m_ArcAngle_start = m_arc_auxipath_out.m_ArcAngle_end;
		  //��Բ���и������ߵ���ֹ���ǲ���ģ���Ϊ��ֹ�˵㲻��
		  //��Բ���и������ߵ���ʼ��Ҳ�ǲ���ģ���Ϊ��ʼ�˵㲻��
		  //Բ���и������ߵ���ֹ��������ԭ���и������ߵ���ʼ�ǣ���ԭ���ļ��㷽ʽ���㡣
		  m_arc_auxipath_out.m_ArcAngle_end = m_geomele.ForCircleStartAngle(m_Pstrat.x, m_Pstrat.y, m_circle);
		  //��ôԲ���������ҲҪ��˱�Ϊԭ��Բ�����г��㡢��������г��㶼Ҫ�仯
		  m_Pstrat.x = m_arc_auxi_SEP.x1;
		  m_Pstrat.y = m_arc_auxi_SEP.y1;
		  m_Pend.x = m_arc_auxi_SEP.x0;
		  m_Pend.y = m_arc_auxi_SEP.y0;
	  }
	  else//��������������֣������˵����ʵ�����ԭ���������һ���ġ����ù�
	  {
		  ;
	  }
	  //��Բ���и���������ӵ�һ��ͳһ�ĸ�ʽ���棬֮�󴫵���ӵĺ����
	  m_auxiliaryPath.m_IfLine = false;//false�Ǵ���Բ��
	  m_auxiliaryPath.m_arc_in = m_arc_auxipath_in;
	  m_auxiliaryPath.m_arc_out = m_arc_auxipath_out;
	  m_auxiliaryPath.P_AuxiStart = m_Pstrat;
	  m_auxiliaryPath.P_AuxiEnd = m_Pend;

	  //�����ǽ�Բ���и������ߵ��������������ߵĲ������Ѿ�������ˣ����ǲ�û�а�Բ���и������߱�Ϊ��ջ��Ĺ���һ��
	  //���Ҫ�õ�˫��������ҿ���
	  AddAuxiNodeToCH(m_auxiliaryPath, P_mid, pCHtemp);

  }
  //���и���������Ϊ��ջ��Ĺ��нڵ���ӽ�ȥ��
  //m_auxiliaryPath�Ǵ洢���и������ߵ�������Ϣ��P_mid��Բ���и������ߵ����ջ����е�,pCHtempΪ��ջ�ͷ���
  //����������ｫ�и������߹ҿ�����ջ���
  void  GeomForCut::AddAuxiNodeToCH(AuxiliarPath m_auxiliaryPath, Point P_mid, GeomCloseHEAD*pCHtemp)
  {
	  //Բ�����͵��и������߶��������ߣ�����������Բ�������ߣ�Բ���г��ߣ����������ǽ�ԭ����ֱ�ߴ�ϳ�Ϊ�����ߣ�������ͨ����������һ���Ķ˵㣬��ʵ�֡�ֻ��Ҫ�������һ���ͺá�
	  GeomEleNode*Add_more_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//�и�������
	  GeomEleNode*cut_in_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//�и�������
	  GeomEleNode*cut_out_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//�и�������
	 //��pFirstGele�������Ƕ� pCHtemp->FirstGeomele������
	  GeomEleNode *pFirstGele = pCHtemp->FirstGeomele;//�����洢��ջ�ԭ���ĵ�һ������ͼԪ�ģ������߶�ԭ�����е��Ϊ�е㴦��ע�⣬���еķ�ջ��ĵ�һ������ͼԪ���Ǹı���ˡ�
	//����һ���ĸ�ֵ��һ����. GeomEleNode *&pFirstGele = pCHtemp->FirstGeomele;//�����洢��ջ�ԭ���ĵ�һ������ͼԪ�ģ������߶�ԭ�����е��Ϊ�е㴦��ע�⣬���еķ�ջ��ĵ�һ������ͼԪ���Ǹı���ˡ�

	  //���ж������ջ����и���������ֱ�߻���Բ��
	  //�ܽ���˵����Բ��
	  if (!m_auxiliaryPath.m_IfLine)
	  {
		  //������
		  cut_in_Node->m_GeomStandData.m_arc = m_auxiliaryPath.m_arc_in;
		  cut_in_Node->m_GeomStandData.m_typegeomele = 62;//62ΪԲ�������и�������
		  cut_in_Node->m_GeomStandData.GeoEle_start_x0 = m_auxiliaryPath.P_AuxiStart.x;
		  cut_in_Node->m_GeomStandData.GeoEle_start_y0 = m_auxiliaryPath.P_AuxiStart.y;
		  cut_in_Node->m_GeomStandData.GeoEle_start_x1 = P_mid.x;
		  cut_in_Node->m_GeomStandData.GeoEle_start_y1 = P_mid.y;
		  cut_in_Node->m_GeomStandData.m_IsGeomeleAccept = false;
		  cut_in_Node->m_GeomStandData.m_IsTranData = false;

		  //�г���
		  cut_out_Node->m_GeomStandData.m_arc = m_auxiliaryPath.m_arc_out;
		  cut_out_Node->m_GeomStandData.m_typegeomele = 62;//2ΪԲ��
		  cut_out_Node->m_GeomStandData.GeoEle_start_x0 = P_mid.x;
		  cut_out_Node->m_GeomStandData.GeoEle_start_y0 = P_mid.y;
		  cut_out_Node->m_GeomStandData.GeoEle_start_x1 = m_auxiliaryPath.P_AuxiEnd.x;
		  cut_out_Node->m_GeomStandData.GeoEle_start_y1 = m_auxiliaryPath.P_AuxiEnd.y;
		  cut_out_Node->m_GeomStandData.m_IsGeomeleAccept = false;
		  cut_out_Node->m_GeomStandData.m_IsTranData = false;
	  }
	  else//������ֱ��
	  {
		  ;
	  }
	  //�����ǽ��и������߱���˷�ջ��Ļ���ͼԪ
	  //������Ҫ������ͼԪ���ջ��ںϵ�һ��
	  //�ȵ�����ջ�����Ļ���ͼԪ
	  //����Բ��������
	  if (3!=pCHtemp->FirstGeomele->m_GeomStandData.m_typegeomele)//�������Բ�εĻ�
	  {
		  //����ֻ������Ƕ��������
		  if (1 == pFirstGele->m_GeomStandData.m_typegeomele)//1��ֱ�߶�
		  {
			  //��pFirstGele�е�ǰ���ֱ�߶����ݸ�Add_more_Node�������ν���Ϊ��ջ��ĵ����ڶ��Ρ�
			  //�����Ƿ�ջ�ԭ�����и���Ƶ�
			  Add_more_Node->m_GeomStandData.m_line.x0 = Add_more_Node->m_GeomStandData.GeoEle_start_x0 = pFirstGele->m_GeomStandData.GeoEle_start_x0;
			  Add_more_Node->m_GeomStandData.m_line.y0 = Add_more_Node->m_GeomStandData.GeoEle_start_y0 = pFirstGele->m_GeomStandData.GeoEle_start_y0;
			  //�յ����е㣬Ҳ�����е�
			  Add_more_Node->m_GeomStandData.m_line.x1 = Add_more_Node->m_GeomStandData.GeoEle_start_x1 = P_mid.x;
			  Add_more_Node->m_GeomStandData.m_line.y1 = Add_more_Node->m_GeomStandData.GeoEle_start_y1 = P_mid.y;
			  //ͼԪ������
			  Add_more_Node->m_GeomStandData.m_typegeomele = 66;//66Ϊԭ������ͼԪ����һ���ֱ�����и�������
			  //����ջ����׻�Ԫ������ΪP_mid,Ҳ���Ǽ���һ�롣//���Ҫ����������ͨ��������ߴ���Add_more_Node������á�
			  pFirstGele->m_GeomStandData.m_line.x0 = pFirstGele->m_GeomStandData.GeoEle_start_x0 = P_mid.x;
			  pFirstGele->m_GeomStandData.m_line.y0 = pFirstGele->m_GeomStandData.GeoEle_start_y0 = P_mid.y;
			  //����������Ҫ������������Ļ�����ӵ���ջ��ϣ���������ջ��Ļ�Ԫλ���ˡ�
			  AddThreeNodeToCH(Add_more_Node, cut_in_Node, cut_out_Node, pCHtemp);
		  }
		  else//�����Բ���Σ���ô�ǲ��ǿ��Խ����Ϊֱ�߶�
		  {
			  ;
		  }
	  }
	  else//�����Բ�εĻ���
	  {
		  //����������Ҫ��Բ����������Բ���г��߱�Ϊ����ͼԪ��Ȼ������ӵ���ջ��ϣ���������ջ��Ļ�Ԫλ���ˡ�
		  //ע����Ȼ��������������εĺ���һ�������ǣ�Add_more_Node��û�õģ������������Ϊ��ջ����ı䡣
		  AddThreeNodeToCH(Add_more_Node, cut_in_Node, cut_out_Node, pCHtemp);
	  }
	 


	 
  }
  //�� �����и��ջ��Ļ�Ԫ�ҵ���ջ��ϣ���������ջ��еĻ�Ԫλ�á�
  //����������ǰ�����һ�����ں������������ԭ����β��Ԫ��Ϊ����������
  //���κ�һ����ջ���һ�������һ�������и������ߣ���
  void  GeomForCut::AddThreeNodeToCH(GeomEleNode*Add_more_Node, GeomEleNode*cut_in_Node, GeomEleNode*cut_out_Node, GeomCloseHEAD*pCHtemp)
  {
	  GeomEleNode*pFtemp,*pEtemp;//����ԭ����ջ����׽ڵ���β�ڵ�
	  //��ջ����׻���ͼԪ���Ѿ��������˵ģ������ԭ��ֱ�ߵ��е�
	  pFtemp = pCHtemp->FirstGeomele;
	  pEtemp = pCHtemp->FirstGeomele;
	  //Ѱ�ҵ����һ������ͼԪ�������Բ���ַ�ջ�����ô��ֻ��һ������ͼԪ��
	  while (pEtemp->nextGeomeleNode)
		  pEtemp = pEtemp->nextGeomeleNode;
	  //�����ڵ�,cut_in_Node��Ϊ���׻���ͼԪ
	  pCHtemp->FirstGeomele = cut_in_Node;
	  cut_in_Node->prevGeomeleNode = NULL;
	  cut_in_Node->nextGeomeleNode = pFtemp;
	  //�� pCHtemp�Ĺ��ɵ�m_geomclose_startpoint��ΪԲ������ʼ��
	  pCHtemp->m_geomclose_startpoint.colse_start_x0 = cut_in_Node->m_GeomStandData.GeoEle_start_x0;
	  pCHtemp->m_geomclose_startpoint.colse_start_y0 = cut_in_Node->m_GeomStandData.GeoEle_start_y0;

	  //ԭ���ķ�ջ��׻��ڳ�Ϊ�˵ڶ���
	  pFtemp->prevGeomeleNode = cut_in_Node;
	  //����Ҫ���ݷ�ջ�������Ҫ�������
	  //����Բ��ջ��������η�ջ���
	  //ע������Ҫ�õڶ�����ջ������ж��ˡ�
	  if (3!=pCHtemp->FirstGeomele->nextGeomeleNode->m_GeomStandData.m_typegeomele)//�������Բ��
	  {
		  //pEtemp���˵�����������Ԫ
		  pEtemp->nextGeomeleNode = Add_more_Node;
		  Add_more_Node->prevGeomeleNode = pEtemp;
		  //Add_more_Node��Ϊ�����ڶ���,cut_out_Node��Ϊ���һ��
		  Add_more_Node->nextGeomeleNode = cut_out_Node;
		  cut_out_Node->prevGeomeleNode = Add_more_Node;
		  cut_out_Node->nextGeomeleNode = NULL;
		  //��˱������
	  }
	  else//�����Բ��
	  {
		  pEtemp->nextGeomeleNode = cut_out_Node;
		  cut_out_Node->prevGeomeleNode = pEtemp;
		  cut_out_Node->nextGeomeleNode = NULL;
		  //��˱������
	  }
	
  }
