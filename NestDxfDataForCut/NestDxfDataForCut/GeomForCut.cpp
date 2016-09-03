#include "stdafx.h"
#include "GeomForCut.h"
#define EPSILON 0.018
#define WEIGHT_ORIGIN 1//��ջ�������ԭ������Ȩ��
#define WEIGHT_PTP 0//��ջ�����һ����ջ�֮������Ȩ��
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
					head->FirstGeomClose = Htemp->nextGeomcloseNode;//�Ӵ˵ڶ���������ͷ���
					head->m_NumGeomClose--;//��ջ���Ҫ����һ��
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
		if (paimnode == phtemp)//˵������һȦ���ֻ����ʼ�Ľ��������
		{
			//��ʵ��Լ����˵����û��
			pHtemp->FirstGeomele = phtemp;
			pHtemp->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//��ͷ������㸳����ջ�ͷ����ﱣ�棬��Ϊ��һ����ջ��жϵ�����
			pHtemp->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
		}
		else//˵��������ͷ���Ϊ������Ľ��
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
	int m_TotalCloseID;//�����ܹ��ķ�ջ��㣬�����������һ�����⣬Բ�ķ��ͷ��㲢û�в������˫�������У���ô����ͻ�����٣��������ǿ϶�Ҫ�Ȱ�Բ��Ҳ���������ٵ���һ����
	pHtemp = head->FirstGeomClose;//��һ����ջ�ͷ���
	pntemp = head->FirstGeomClose;
	while (pntemp->nextGeomcloseNode)//�ҵ����һ����ջ��ڵ�
		pntemp = pntemp->nextGeomcloseNode;
	m_TotalCloseID = pntemp->GEOMCLOSE_ID;
	for (m_GeomCloseID = 1; m_GeomCloseID <m_TotalCloseID; m_GeomCloseID++)//��ȫ��ѭ����ջ��ĸ�����
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
			//������ѭ��һ��֮���ҵ���Ŀǰ��˵��С��һ����ջ�ͷ���
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
				//���������Ѱ�ң�����phnowtemp�����һ����true�Ľڵ㣬��phstarttemp����Ҫ�ҵ�phnowtemp����
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
	while (pHtemp)//���з�ջ�ͷ��㶼������
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
		if (paimnode == phtemp)//˵������һȦ���ֻ����ʼ�Ľ��������
		{
			//��ʵ��Լ����˵����û��
			pHtemp->FirstGeomele = phtemp;
			pHtemp->m_geomclose_startpoint.colse_start_x0 = phtemp->m_GeomStandData.GeoEle_start_x0;//��ͷ������㸳����ջ�ͷ����ﱣ�棬��Ϊ��һ����ջ��жϵ�����
			pHtemp->m_geomclose_startpoint.colse_start_y0 = phtemp->m_GeomStandData.GeoEle_start_y0;
		}
		else//˵��������ͷ���Ϊ������Ľ��
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
		pHtemp = pHtemp->nextGeomcloseNode;//��һ����ջ�������֮�󣬽���ڶ�����ջ������ѭ�������������dxf����ķ�ջ��������ꣻ����Բ
	}
}