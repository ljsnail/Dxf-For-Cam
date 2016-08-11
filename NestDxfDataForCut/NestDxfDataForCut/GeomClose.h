#pragma once
#include "GEOMELE.h"
//********************************************************************//
//������ṹ��Ҫ�Լ���ͼԪ���б���������x0, y0, x1, y1�Ĺ�ϵ���ҵ���ջ���ÿ����ջ���IID������
//��ջ������ͼԪӦ������������У���ѡ������һ����ͼԪ��Ϊ��ջ������
//������֮���Ѿ�û��LINE ARC CIRCLE������ͼԪ�ˣ��е���һ������ջ���
//********************************************************************//
typedef struct//��ջ�����㣬��Ϊ��һ����γ�ȫ������·��ʱ����жϵ㡣
{
	double colse_start_x0, colse_start_y0;//����Ѱ�ҵ�ʱ��Ӧ��Ҫ�е�ԭ���ǰ�����������µ�ԭ��ȥ�ҡ�����ԭ��

}GEOMCCLOSTAPOINT;

typedef struct SORTGEOM//����ÿһ����ջ��Ľṹ����Ҫ���÷�ջ������ÿһ��ͼԪ����
{
	GEOMELE geomele;//����ÿ������ͼԪ�����ݣ��洢
	struct SORTGEOM*prorNode;
	struct SORTGEOM*nextNode;
}SORTGEOM;
typedef struct HEAD
{
	GEOMCCLOSTAPOINT geomclose_startpoint;
	int GEOMCLOSE_ID;//��Ϊÿһ����ջ��ı�ʶ�����и�ʱ����������ı�ʶ��Ӧ����Ϊ���ñ�־
	struct SORTGEOM*first;//ָ��˫�������ͷ���
	bool flag;//����Ժ�Ҫȫ������·���滮��ʱ����Ϊÿһ����ջ�ʱ���Ѿ����滮�ı�ʶ
}HEAD;
class GeomClose
{
public:
	GeomClose();
	~GeomClose();
public:
	GEOMCCLOSTAPOINT geomclose_startpoint;
	int GEOMCLOSE_ID;//��Ϊÿһ����ջ��ı�ʶ������һ��α���ʱ��ʹ��,���ø�ͷ��㣬���ϵ��ã�ͨ��Э�顣
	GEOMELE geomele;//��ΪARC ��CIRCLE��ʹ��
public://������ջ�������ÿһ����ջ����������
	//����ÿһ��ͼԪ��ID����һ�����ݣ��ȶ�ÿһ�����ֵ�ͼԪ�ֱ����ò�ͬ��IID��
	//�������ĳ��ͼԪ����������IID�й�ϵ����ô�Ͱ�������IID�ϲ�����ЩͼԪ����ͬIID��ʱ�����ͬһ��˫��������
	//���GEOMCLOSE_ID��ͬʱ����Ҳ��GEOMCLOSE_ID���õ�GEOMELEԤ���ı�־λ��
	int FindGeomClose(int ID);//����LINE ARC CIRCLE ��ID�����벻ͬ�ķ�ջ�ID
	int MergerGeomClose(int ID1, int ID2);//������ͬһ��ͼԪ�������˵���ص�ID,�ں���ЩID������һ�������ϲ���������һ��ID��β���ָ����һ��ID��ͷ��㣩
	HEAD*SetGeomeleOrder(HEAD*head);//����ÿһ����ջ���ͷ��㣬ʹ֮�ڲ���geomele�ų���
	void SetGeomCloseID(int GEOMCLOSE_ID);//����HEADͷ��㴦��ID.
	GEOMCCLOSTAPOINT SetGeomCloseStartpoint(int IID);//����ÿһ����ջ���IID�����ÿ����ջ�����ʼ��,���������Ҫ���뵽ͷ�������ȥ��
	GEOMCCLOSTAPOINT TranForCircle(GEOMELE geomele);//�жϵ���CIRCLE��ʱ������ ��ջ��Ĳ��������ת�����circle���������µ��Ǹ������Ϊ��㡣
};

