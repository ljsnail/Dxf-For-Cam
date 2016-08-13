#include "stdafx.h"
#include "GeomClose.h"


GeomClose::GeomClose()
{
	
}


GeomClose::~GeomClose()
{
}
//��ʼ��һ���ڴ����������ͼԪ��־�����ݸ�ʽGeomStandData
//��ײ��һ������
GeomEleNode* GeomClose::CreatGeomEleNode(GeomStandData m_geomstandData)
{
	GeomEleNode*newNode = (GeomEleNode*)malloc(sizeof(GeomEleNode));
	newNode->m_GeomStandData = m_geomstandData;//�����ݱ�������
	newNode->prevGeomeleNode = NULL;
	newNode->nextGeomeleNode = NULL;
	newNode->m_AccptGeomEleNode = false;
	newNode->m_NumGeomEleID=m_geomstandData.m_GeomEleID;//������������ͼ�ĵڼ�������ͼԪ
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
	newNode->m_AcceptGeomcloseHEAD = false;//���ʱ��û����¼
	return newNode;
}
GeomCloseHEAD*GeomClose::AddGeomEleNode(GeomCloseHEAD*head, GeomEleNode *node)
{
	GeomEleNode*temp;//���ɽ��
	//����������ʱ��ֿձ���Ѿ������ݵı�
	//����ǿձ���ô��ֱ�Ӱ����ݹ���ȥ
	if (!(head->FirstGeomele))
	{
		head->FirstGeomele = node;
		head->m_geomclose_startpoint.colse_start_x0 = node->m_GeomStandData.GeoEle_start_x0;//��Ȼ�ǿձ���ô�������ͷ��㣬���ԾͰ�ͷ������ʼ����first����
		head->m_geomclose_startpoint.colse_start_y0 = node->m_GeomStandData.GeoEle_start_y0;
		head->m_NumGeomele++;//��ʾ����һ��ͼԪ
		return head;
	}
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
		return head;
	}
}