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
	GeomClose geomclose;
	NestResultDataNode m_nestrsltNode;
public:
	
	BatchHEAD*CreatBatchHEAD(int BatchHEAD_ID);//��ʼ��һ���ڴ�Ϊͷ����ţ������������
	NestResultDataNode*CreatNestResultNode(int NestResult_ID);//��ʼ��һ���ڴ�����������㣬�����������
	//���޹��ɵİѽ�����
	NestResultDataNode*AddGeomCloseHeadNode(NestResultDataNode*head,GeomCloseHEAD*node);//����ÿһ���ջ���Fͷ��㣬������ȫ������һ�����ϡ�
	BatchHEAD*AddNestRsltDtNode(BatchHEAD*head, NestResultDataNode*node);//��ÿһ������ͼ�ĵ�ַ�ҵ���BatchHEADΪͷ����˫�������С�
};

