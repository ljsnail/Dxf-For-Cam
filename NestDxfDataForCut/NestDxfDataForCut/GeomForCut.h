#pragma once
#include"GeomClose.h"
//�������Ҫ��ʵ�ְ�֮ǰ�ķ�ջ�֮�䴮�����и�ʹ�á�
//ʵ�֣�����ջ�֮�����򣬶�Բ��������ͼ��Ҫ���⴦������Բ��Ҫת����ԭ����������ʽ
//�������ͼԪ֮��Ĺ���ֱ��
//Ū�������������洢��һ������dxf������
typedef struct//���ͼԪ֮��Ĺ�����
{
	double pror_x, poro_y, next_x, next_y;//��һ���ͼԪ��xy����һ���ͼԪ�����xy
}TranLine;
typedef struct NestResultData//�洢���е�ͼԪ�ĵ�������
{
	TranLine tranline;
	struct NestResultData*nextNode;//ָ����һ��ջ��ĵ�ַ
	bool flag;//�����Ժ���Ϊȫ�ֹ滮ʱ��ʹ�á�
}NestResultData;
class GeomForCut
{
public:
	GeomForCut();
	~GeomForCut();
public:
	GeomClose geomclose;
public:
	NestResultData*SetCutOrder;

};

