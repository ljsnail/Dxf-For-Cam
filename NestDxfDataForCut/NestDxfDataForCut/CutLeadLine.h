#pragma once
#include<math.h>
#include"GeomClose.h"
//#include<gl.h>
#define pii 3.1416;
#define m_CutLineLength  25;
#define EPSILON 0.001
struct Line_para
{
	double x0, y0;
	double x1, y1;
	double k;
};
struct Line_Inter//�и������߸�������ݽṹ
{//ֱ�߷��̵ı�׼��ʽ��ax+by=c
	double a;
	double b;
	double c;
};
struct Line_Point//���и������ߵ������˵��ĸ�����ķ�Χ
{
	double x_min, x_max, y_min,y_max;
};
struct Point
{
	double x;
	double y;
};
class CutLeadLine
{
public:
	CutLeadLine();
	~CutLeadLine();
	//��ֱ�ߵ�б��
	double Get_k_twoPoint(Line_para a);
	//�жϼнǵĿ��ڷ���,1Ϊ���Ͽ��ڣ�2Ϊ���¿��ڣ�3Ϊ���󿪿ڣ�4Ϊ���ҿ��ڡ�
	int Get_OpenDirect(Line_para a, Line_para b);
	//��������ֱ�ߵ�б�ʣ��ͼнǵĿ��ڷ���
	double Get_k_twoK(double k1, double k2, int m_opendirect);
	//������и��������ϵ��и���Ƶ������������߳��ȵ�������
	Line_para Get_CutLine_Point(Point start, double k);					
	//����������ɵ��и�������ͼԪ
	Line_para Get_CutLine_StartPoint(Line_para m_Line, int m_OpenDirect, bool m, Point start);
	//������ֹ��ջ�����ͼԪ���ͷ�ջ���˫���ʣ����и������߻�������,�Լ������и������ߵķ�ʽ
	//���ⲿ���õ�API������mΪ��ջ�����ż����Ϣ��m_TypeCGLineΪ�����и������ߵķ�ʽ��1Ϊ����ż�����ɣ�2Ϊ��������ʽ����
	Line_para Get_CutLeadLine(Line_para a, Line_para b, bool m,int m_TypeCGLine);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//�и������߸����жϣ����뵱ǰ��Ҫ�ж��и������ߵķ�ջ���Ȼ���ж���������������ڵ��ֵܷ�ջ��Ƿ���档
	bool JudgeCGLineVSGeomclosedH(GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head);
	//�и������߸����ж�,�����и�������������һ����ջ����ж����Ƿ���棬������棬����true,���򷵻�false,��������Ĵ���
	bool JudgeCGLineVSOneClosedHead(GeomEleNode*pCGLinetemp, GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head);
	//�����ԭ���ǽ������и������ߵ�������֮�䡣
	bool If_HaveCGLineInterPoint(GeomEleNode*pCGLinetemp, GeomEleNode*pCGeomtemp);
	//Ŀǰֻ�ܴ����н����ֱ�ߣ�����ƽ�е��߶β����ܽ�һ������
	//����ֱ�ߵ������˵�ֵ��Ȼ�����׼ֱ�ߵ�abc��������
	Line_Inter GetLine(Point m_StratPoint, Point m_EndPoint);
	//�����и��������Լ���һ�������и�����߶ε�abc�������󽻵�
	Point GetCrossPoint(Line_Inter l1, Line_Inter l2);
	//���и������������˵��ĸ���֮��ķ�Χ
	Line_Point GetPointOrder(Point m_StratPoint, Point m_EndPoint);

	//�����и��и��������и���ķ�ջ���Ȼ��������и�������
	void ChangeCGLine(GeomCloseHEAD*pCHtemp);
	//�����и���������ȡ�и������߿��Ƶ�ķ���
	//��ʱ�������㣬������ȡ
	Line_para Get_ChangeCutLine_StartPoint(Line_para m_Line, int m_OpenDirect, Point start);


};

