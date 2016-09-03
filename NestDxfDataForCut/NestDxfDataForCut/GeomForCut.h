#pragma once
#include"GeomClose.h"
//�������Ҫ��ʵ�ְ�֮ǰ�ķ�ջ�֮�䴮�����и�ʹ�á�
//ʵ�֣�����ջ�֮�����򣬶�Բ��������ͼ��Ҫ���⴦������Բ��Ҫת����ԭ����������ʽ
//�������ͼԪ֮��Ĺ���ֱ��
//Ū�������������洢��һ������dxf������

typedef struct NestResultDataNode//�洢���е�ͼԪ�ĵ�������
{
	
	struct GeomCloseHEAD*FirstGeomClose;//ָ���ջ�GeomCloseHEAD��ͷ���
	struct NestResultDataNode*prevNestResultDataNode;//ָ��ǰһ������ͼ���ݽ��NestResultDataNode�ĵ�ַ���ڲ�ͬ���������ͼ֮�����
	struct NestResultDataNode*nextNestResultDataNode;//ָ��ǰһ������ͼ���ݽ��NestResultDataNode�ĵ�ַ���ڲ�ͬ���������ͼ֮�����
	unsigned int NestResult_ID;//ϵͳ�洢�ĵڼ����������ͼ����ʼ��Ϊ0��
	unsigned int m_NumGeomClose;//��˫������洢�ķ�ջ��ĸ���
	bool AcceptNestResultflag;//�Ƿ��Ѿ������ã���ʼ��Ϊfalse,Ŀ�Ĳ�ͬ�������ͼ���е��á�
}NestResultDataNode;
//�洢�������Σ�һ�ο����ͻ���һ���ڴ汣��ָ�������������ӹ���n���������ͼ����ÿ���������ͼ�����n�����ͼ�Σ���ÿ�����ͼ�������n��������ԪLINE,ARC,CIRCLE
typedef struct BatchHEAD
{
	unsigned int BatchHEAD_ID;
	unsigned int m_NumNestrsltDtNode;//��������ж������������ͼ
	struct NestResultDataNode*FirstNestRsltNode;//ָ���һ�żӹ�������ͼͷ���
}BatchHEAD;
class GeomForCut
{
public:
	GeomForCut();
	~GeomForCut();
public:
	GeomClose m_geomclose;
	NestResultDataNode m_nestrsltNode;
public:
	
	BatchHEAD*CreatBatchHEAD(int BatchHEAD_ID);//��ʼ��һ���ڴ�Ϊͷ����ţ������������
	NestResultDataNode*CreatNestResultNode(int NestResult_ID);//��ʼ��һ���ڴ�����������㣬�����������
	//���޹��ɵİѽ�����
	NestResultDataNode*AddGeomCloseHeadNode(NestResultDataNode*head,GeomCloseHEAD*node);//����ÿһ���ջ���Fͷ��㣬������ȫ������һ�����ϡ�
	BatchHEAD*AddNestRsltDtNode(BatchHEAD*head, NestResultDataNode*node);//��ÿһ������ͼ�ĵ�ַ�ҵ���BatchHEADΪͷ����˫�������С�
		//***************************************************************************************//
		/**///���޹��ɵ�GeomEleNode�ҳ�����ɣ�ת�����й��ɵļ�����ջ���
		/**///*************************************************************************//
		/**///�������ȷַ�ջ�������ÿһ����ջ���������ݽ�������
		/**///*************************************************************************//
		/**///void SepDiffGeomCls(GeomCloseHEAD*head);//����ָ����¼��������ͼȫ������ͼԪ��˫�������Fͷ���
		//***************************************************************************************//

	//�������ݽ��˫���������治��ͬһ����ջ��Ľ��
	GeomEleNode*FindDiffGeomCloseNode(NestResultDataNode*head);//�����������ͼ��ͷ���
	//��һ��˫�������ϵĲ�ͬ��ջ��ҵ���ͬ������ͷָ���˫��������ȥ��
	void InsertGeomCloseHEAD(NestResultDataNode*head, GeomEleNode*node);//�����µĲ�ͬ��ջ�ID�Ľ�㣬������Fͷ��㣬��ͬʱFͷ���ָ��node��
	//���Ѿ��ҵ���ͬ��ջ�˫������������ݽ��鿴���Ƿ���һ�������ķ�ջ�
	GeomCloseHEAD*JudgeGeomCloseIntact(NestResultDataNode*head);//�����������ͼdxfͷ��㣬һ����ȥ�Ų��ջ�����µ������Ƿ�����
	//����������ͷ������������Ѱ����Ե���������;ֱ�����еķ�ջ�������λΪ������
	NestResultDataNode*Find_Change_GeomCloseHEAD(NestResultDataNode*head, GeomCloseHEAD*node);
	//����֮��Ե�ͷ��������������
	NestResultDataNode*ChangeGeomCloseHEAD(NestResultDataNode*head, GeomCloseHEAD*node);
	//�����Ѿ���ԭʼ����˫�������ϵĲ�ͬ��ջ��ҵ���ͬ�ķ�ջ�ͷ������ˣ����ǻ��漰��һ��ջ��ڲ���������δ����͸�����ջ�֮���˳��ȷ��
	//���ڵ�һ��ջ��ڲ����ԣ������������µ��Ǹ�������ڵĽ��Ϊͷ��㣬Ȼ��ı��ջ��ڲ�˳��
	void ChangeEleNodeOfGeomClosed_origin(NestResultDataNode*head);//��������ͼͷ��㣬����ÿ����ջ��ڲ����ݵ�00����̾���Ѱ��ͷ��㣬�������ͷ������ָ���ջ�ͷ����F�ڵ��
	//��������֮��һ��ջ��ڲ������ݵ����Ѿ���ϣ�����ÿһ����ջ����ͷ��㶼����Զ��������Ǹ��㡣
	//ȷ����ջ�˳��֮����ȷ��һ���ջ��ڲ���ͷ��㣬ǰ���Ȱ��յ�����ԭ����ȷ����ջ�����ʼ���ݵ�
	void ChangClosedNodeOfNRDXF(NestResultDataNode*head);
	//��������֮����������ڵķ�ջ��Ѿ�������˳����ôΪ��ʹ���г̸��̣����Ӧ�ø��ݵ���һ����ջ���̾���������ÿ����ջ��ڲ���ͷ���
	void ChangeEleNodeOfGeomClosed_order(NestResultDataNode*head);//�����������ͼͷ��㣬�����Ѿ��źõķ�ջ�

};

