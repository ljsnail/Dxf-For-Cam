#pragma once
#include<math.h>
#include"GeomClose.h"
//#include<gl.h>
#define pii 3.1416;
#define m_CutLineLength  25;

struct Line_para
{
	double x0, y0;
	double x1, y1;
	double k;
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
	//������и��������ϵ��и���Ƶ�������5mm��������
	Line_para Get_CutLine_Point(Point start, double k);
	//����������ɵ��и�������ͼԪ
	Line_para Get_CutLine_StartPoint(Line_para m_Line, int m_OpenDirect, bool m, Point start);
	//������ֹ��ջ�����ͼԪ���ͷ�ջ���˫���ʣ����и������߻�������
	//���ⲿ���õ�API
	Line_para Get_CutLeadLine(Line_para a, Line_para b, bool m);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//�и������߸����жϣ����뵱ǰ��Ҫ�ж��и������ߵķ�ջ���Ȼ���ж���������������ڵ��ֵܷ�ջ��Ƿ���档
	bool JudgeCGLineVSGeomclosedH(GeomCloseHEAD*pCHtemp);
	//�и������߸����ж�,�����и�������������һ����ջ����ж����Ƿ���棬������棬����true,���򷵻�false,��������Ĵ���
	bool JudgeCGLineVSOneClosedHead(GeomEleNode*pCGLinetemp, GeomCloseHEAD*pCHtemp);
	//�����ԭ���ǽ������и������ߵ�������֮�䡣
	bool If_HaveCGLineInterPoint(GeomEleNode*pCGLinetemp, GeomEleNode*pCGeomtemp);
	//�����и��и��������и���ķ�ջ���Ȼ��������и�������
	void ChangeCGLine(GeomCloseHEAD*pCHtemp);

};

