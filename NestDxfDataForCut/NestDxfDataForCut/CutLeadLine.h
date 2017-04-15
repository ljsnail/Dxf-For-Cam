#pragma once
#include<math.h>
//#include<gl.h>
#define pii 3.1416;
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
	//求直线的斜率
	double Get_k_twoPoint(Line_para a);
	//判断夹角的开口方向,1为向上开口，2为向下开口，3为向左开口，4为向右开口。
	int Get_OpenDirect(Line_para a, Line_para b);
	//输入两条直线的斜率，和夹角的开口方向
	double Get_k_twoK(double k1, double k2, int m_opendirect);
	//输出在切割引刀线上到切割控制点距离等于5mm的两个点
	Line_para Get_CutLine_Point(Point start, double k);
	//输出初步生成的切割引刀线图元
	Line_para Get_CutLine_StartPoint(Line_para m_Line, int m_OpenDirect, bool m, Point start);
	//输入起止封闭环基本图元，和封闭环单双性质，求切割引刀线基本参数
	//给外部调用的API
	Line_para Get_CutLeadLine(Line_para a, Line_para b, bool m);

};

