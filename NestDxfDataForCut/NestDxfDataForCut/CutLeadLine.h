#pragma once
#include<math.h>
#include"GeomClose.h"
//#include<gl.h>
#define pii 3.1416;
//#define m_CutLineLength  25;
#define EPSILON 0.001

struct Line_para
{
	double x0, y0;
	double x1, y1;
	double k;
};
struct Line_Inter//切割引导线干涉的数据结构
{//直线方程的标准形式：ax+by=c
	double a;
	double b;
	double c;
};
struct Line_Point//求切割引导线的两个端点四个坐标的范围
{
	double x_min, x_max, y_min,y_max;
};
//struct Point
//{
//	double x;
//	double y;
//};
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
	//输出在切割引刀线上到切割控制点距离等于引刀线长度的两个点
	Line_para Get_CutLine_Point(Point start, double k);					
	//输出初步生成的切割引刀线图元
	//给直线类型的切割引刀线用的
	Line_para Get_CutLine_StartPoint(Line_para m_Line, int m_OpenDirect, bool m, Point start);
	//给圆类型的切割引导线用的，少了中间的开口方向参数m_OpenDirect
	Line_para Get_CutLine_StartPoint(Line_para m_Line, Point Circle_Center, bool m, Point start);
	//输入起止封闭环基本图元，和封闭环单双性质，求切割引刀线基本参数,以及生成切割引导线的方式
	//直线类型的切割引导线
	//给外部调用的API，其中a为封闭环的第一条基本直线段图元，b为最后一条基本直线段图元。其中m为封闭环的奇偶层信息，m_TypeCGLine为生成切割引导线的方式，1为按奇偶层生成，2为按调整方式生成
	Line_para Get_CutLeadLine(Line_para a, Line_para b, bool m,int m_TypeCGLine);
	//圆类型的切割引导线
	//由于圆类型的切割引导线生产的方式是两点连线法，所以输入上一个封闭环的切割控制点，和当前封闭环的切割控制点，其中m_Singlelayer为封闭环的奇偶层信息，m_TypeCGLine为生成切割引导线的方式，1为按奇偶层生成，2为按调整方式生成
	//其中参数Last_GClose为上一个封闭环的切割控制点，Current_GClose为当前圆的切割控制点，
	Line_para Get_CutLeadLine(Point Last_GClose, Point Current_GClose, Point Circle_Center, bool m_Singlelayer, int m_TypeCGLine);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//切割引导线干涉判断，输入当前需要判断切割引导线的封闭环，然后判断其与包括自身在内的兄弟封闭环是否干涉。
	bool JudgeCGLineVsGeomclosedH(GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head);
	//切割引导线干涉判断,输入切割引导线与任意一个封闭环，判断其是否干涉，如果干涉，返回true,否则返回false,这是最核心代码
	bool JudgeCGLineVsOneClosedHead(GeomEleNode*pCGLine, GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head);
	//干涉的原则是交点在切割引导线的两个点之间。
	bool If_HaveCGLineInterPoint(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp);
	//直线型切割引导线与直线型基本图元的干涉判断
	bool CheckCGLineVsLine(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp);
	//直线型切割引导线与圆基本图元的干涉判断
	bool CheckCGLineVsCircle(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp);
	//圆弧切割引导线与圆基本图元的干涉判断
	bool CheckCGArcVsCircle(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp);
	//圆弧与直线的干涉判断
	//此函数可以判断直线切割引导线与圆弧基本图元的干涉判断
	//也可以判断圆弧切割引导线与直线基本图元的干涉判断
	bool CheckCGLineVsCGArc(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp);

	//求点到线段之间的距离
	double CalcuDistance(Point m_point, Point m_LineP1, Point m_LineP2);
	//求线段与圆的交点
	//与圆可能存在两个交点，如果存在两个交点在ptInter1和ptInter2都为有效值，如果有一个交点，则ptInter2的值为无效值，此处为65536.0
	// 线段起点,线段终点,圆心坐标,全局的交点坐标
	//const的作用为算法内不可改变对应的变量数值
	bool LineInterCircle(const Point ptStart, const Point ptEnd,const Point ptCenter,const double Radius,Point& ptInter1,Point& ptInter2);
	


	
	//新提出一种根据矩形包络预判断是否有相交关系，如果有再进一步采用上述算法If_HaveCGLineInterPoint；
	//用切割引导线的包络矩形与封闭环的包络矩形进行判断
	bool If_HaveCGLineInterPoint_UseRect(GeomEleNode*pCGLinetemp, GeomCloseHEAD*pCHtemp);
	//目前只能处理有交点的直线，对于平行的线段并不能进一步区分
	//输入直线的两个端点值，然后求标准直线的abc三个参数
	Line_Inter GetLine(Point m_StratPoint, Point m_EndPoint);
	//输入切割引刀线以及另一条疑似有干涉的线段的abc参数，求交点
	Point GetCrossPoint(Line_Inter l1, Line_Inter l2);
	//求切割引导线两个端点四个点之间的范围
	Line_Point GetPointOrder(Point m_StratPoint, Point m_EndPoint);

	//输入切割切割引导线有干涉的封闭环，然后调整其切割引导线
	void ChangeCGLine(GeomCloseHEAD*pCHtemp);
	//调整切割引刀线中取切割引刀线控制点的方法
	//此时不管奇层层，都往内取
	Line_para Get_ChangeCutLine_StartPoint(Line_para m_Line, int m_OpenDirect, Point start);
	//用于求出用来判断的切割引导线的。
	GeomEleNode*GetLimtOfAuxiliaryLine(GeomCloseHEAD*pCHtemp);
	GEOMELE m_geomele;//作为GEOMELE类调用的变量

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//新的获取切割引导线的方式///////////////////
	///////////////////////////////////////////////////////
	
	
};

