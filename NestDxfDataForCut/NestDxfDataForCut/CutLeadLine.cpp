#include "stdafx.h"
#include "CutLeadLine.h"


CutLeadLine::CutLeadLine()
{
}


CutLeadLine::~CutLeadLine()
{
}
double CutLeadLine::Get_k_twoPoint(Line_para a)
{
	double k;//斜率
	if (a.x0 != a.x1)//斜率存在
	{
		return k = (a.y0 - a.y1) / (a.x0 - a.x1);
	}
	else//斜率不存在，给定一个定值
	{
		return k = 999.12;
	}
}
//判断夹角的开口方向,1为向上开口，2为向下开口，3为向左开口，4为向右开口。
int  CutLeadLine::Get_OpenDirect(Line_para a, Line_para b)
{
	//其中ab的x0,y0都是相同的
	int m_opendirect = 0;
	//如果切割控制点的Y是最小值，那么就是向上开口
	if ((a.y0 <= a.y1) && (b.y0 <= b.y1))
	{
		return m_opendirect = 1;//向上方向开口
	}
	//如果切割控制点的Y是最大值，那么就是向下开口
	else if ((a.y0 >= a.y1) && (b.y0 >= b.y1))
	{
		return m_opendirect = 2;//向下方向开口
	}
	//上下开口方向比较容易区分，左右开口方向则需要分多种情况区分
	//根据斜率来加以判断区分
	//以下判断左右开口
	else if (a.k*b.k<0)//斜率一正一负
	{
		//x为最大值，则向左方向开口
		if ((a.x0 >= a.x1) && (b.x0 >= b.x1))
		{
			return m_opendirect = 3;
		}
		else//x为最小值，则向右方向开口
		{
			return m_opendirect = 4;
		}
	}
	//斜率同正的时候
	else if (a.k >= 0 && b.k >= 0)
	{
		//斜率小的x1小，斜率大的x1大，则是向左开
		if (a.k<b.k && a.x1<b.x1)
		{
			return m_opendirect = 3;
		}
		else//斜率小的x1大，斜率大的x1小，则是向右开
		{
			return m_opendirect = 4;
		}
	}
	//斜率同负的时候
	else if (a.k <= 0 && b.k <= 0)
	{
		//斜率小的x1大，斜率大的x1小，则是向左开
		if (a.k<b.k && a.x1>b.x1)
		{
			return m_opendirect = 3;
		}
		else//斜率小的x1小，斜率大的x1大，则是向右开
		{
			return m_opendirect = 4;
		}
	}
}
//输入两条直线的斜率，和夹角的开口方向
double CutLeadLine::Get_k_twoK(double k1, double k2, int m_opendirect)
{
	double k = 0.0;
	double a, b, c;
	double d;//保存切割引刀线偏转角的弧度
	if ((k1 != 999.12) && (k2 != 999.12))//两条直线的斜率均存在的情况下
	{
		a = atan(k1) * 180 / pii;//倾斜角
		b = atan(k2) * 180 / pii;//倾斜角
	}
	else if (k1 == 999.12)//k1是垂直线没有斜率
	{
		a = 90.0;//即是90度
		b = atan(k2) * 180 / pii;//倾斜角
	}
	else if (k2 == 999.12)//k2是垂直线没有斜率
	{
		a = atan(k1) * 180 / pii;//倾斜角
		b = 90.0;//即是90度
	}
	//对大于90度的倾斜角进行转换
	//两条直线可以构成两个夹角，要判断夹角开口方向
	//如果Y是最值那么就上下开口，如果不是那么就是左右开口
	if (m_opendirect == 3 || m_opendirect == 4)//左右开口
	{
		if (a*b<0)//斜率一正一负
		{
			c = (a + b) / 2;//直接相加/2
		}
		else if ((a >= 0) && (b >= 0))//斜率都是正的
		{
			if (a>b)//斜率大的取负的补角后相加
			{
				a = a - 180;
			}
			else
			{
				b = b - 180;
			}
			c = (a + b) / 2;//直接相加/2
		}
		else//两个都是负的情况下
		{
			if (a<b)//斜率小的取正补角
			{
				a = 180 + a;
			}
			else
			{
				b = 180 + b;
			}
			c = (a + b) / 2;//直接相加/2
		}
	}
	else//夹角上下开口的时候，此时切割控制点的y值就是最值
	{
		if (a*b<0)//斜率一正一负,将负的取正角
		{
			if (a<0)
			{
				a = 180 + a;
			}
			else
			{
				b = 180 + b;
			}
			c = (a + b) / 2;//直接相加/2
		}
		else if ((a >= 0) && (b >= 0))//斜率都是正的
		{
			c = (a + b) / 2;//直接相加/2
		}
		else//两个都是负的情况下
		{
			c = (a + b) / 2;//直接相加/2
		}
	}
	if (c != 90)
	{
		d = (c / 180)*pii;
		k = tan(d);
	}
	else
		k = 999.12;
	return k;
}
//前面已经计算出了切割引刀线的斜率
//输入，切割控制点，切割引刀线的斜率，封闭环的单双层来取那个切割引刀线的起点
//单层往外取值，双层内里取值
//把两个值都输出来
Line_para 	CutLeadLine::Get_CutLine_Point(Point start, double k)
{
	//直线方程：y = k(x-a) + b；
	//圆方程：(x - a) ^ 2 + (y - b) ^ 2 = r ^ 2;
	Line_para Lp;
	double x1, x2, y1, y2;//交点坐标
	double a, b, r;
	a = start.x;
	b = start.y;
	r = 25.0;//引刀线长度
	x1 = a + sqrt(r*r / (1 + k*k));
	y1 = b + k*sqrt(r*r / (1 + k*k));
	x2 = a - sqrt(r*r / (1 + k*k));
	y2 = b - k*sqrt(r*r / (1 + k*k));
	Lp.x0 = x1;
	Lp.x1 = x2;
	Lp.y0 = y1;
	Lp.y1 = y2;
	Lp.k = k;//保存切割引刀线的斜率，以后用得着
	return Lp;
}
//根据开口方向和封闭环单双层取其中一个值作为切割引刀线的起点
//输入两个可疑值，输入开口方向，输入单双层,输入切割控制点
//输出切割引刀线图元
Line_para CutLeadLine::Get_CutLine_StartPoint(Line_para m_Line, int m_OpenDirect, bool m, Point start)
{
	Line_para m_CutGuideL;
	m_CutGuideL.x1 = start.x;
	m_CutGuideL.y1 = start.y;
	m_CutGuideL.k = m_Line.k;
	//如果是板材轮廓进入规划，此时板材外轮廓为第一个奇层，那么单层就要往内取值，偶层往外取值
	//如果是板材轮廓没有进入规划，此时切割部件封闭环为第一个奇层，那么单层就要往外取值，偶层往内取值
	//如果开口向上，则取y小的，开口向下，则取y大的。
	//如果开口向左，则取x大的，开口向右，则取x小的。
	//!m为板材不进入规划，m则是板材进入规划
	if (m)//单层
	{
		switch (m_OpenDirect)
		{
		case 1://开口向上
			if (m_Line.y0<m_Line.y1)
			{
				m_CutGuideL.x0 = m_Line.x0;
				m_CutGuideL.y0 = m_Line.y0;
			}
			else
			{
				m_CutGuideL.x0 = m_Line.x1;
				m_CutGuideL.y0 = m_Line.y1;
			}
			break;
		case 2://开口向下
			if (m_Line.y0<m_Line.y1)
			{
				m_CutGuideL.x0 = m_Line.x1;
				m_CutGuideL.y0 = m_Line.y1;
			}
			else
			{
				m_CutGuideL.x0 = m_Line.x0;
				m_CutGuideL.y0 = m_Line.y0;
			}
			break;
		case 3://开口向左
			if (m_Line.x0<m_Line.x1)
			{
				m_CutGuideL.x0 = m_Line.x1;
				m_CutGuideL.y0 = m_Line.y1;
			}
			else
			{
				m_CutGuideL.x0 = m_Line.x0;
				m_CutGuideL.y0 = m_Line.y0;
			}
			break;
		case 4://开口向右
			if (m_Line.x0<m_Line.x1)
			{
				m_CutGuideL.x0 = m_Line.x0;
				m_CutGuideL.y0 = m_Line.y0;
			}
			else
			{
				m_CutGuideL.x0 = m_Line.x1;
				m_CutGuideL.y0 = m_Line.y1;
			}
			break;
		default:
			break;
		}
	}
	//双层，往里取
	//如果开口向上，则取y大的，开口向下，则取y小的。
	//如果开口向左，则取x小的，开口向右，则取x大的。
	else
	{
		switch (m_OpenDirect)
		{
		case 1://开口向上
			if (m_Line.y0<m_Line.y1)
			{
				m_CutGuideL.x0 = m_Line.x1;
				m_CutGuideL.y0 = m_Line.y1;
			}
			else
			{
				m_CutGuideL.x0 = m_Line.x0;
				m_CutGuideL.y0 = m_Line.y0;
			}
			break;
		case 2://开口向下
			if (m_Line.y0<m_Line.y1)
			{
				m_CutGuideL.x0 = m_Line.x0;
				m_CutGuideL.y0 = m_Line.y0;
			}
			else
			{
				m_CutGuideL.x0 = m_Line.x1;
				m_CutGuideL.y0 = m_Line.y1;
			}
			break;
		case 3://开口向左
			if (m_Line.x0<m_Line.x1)
			{
				m_CutGuideL.x0 = m_Line.x0;
				m_CutGuideL.y0 = m_Line.y0;
			}
			else
			{
				m_CutGuideL.x0 = m_Line.x1;
				m_CutGuideL.y0 = m_Line.y1;
			}
			break;
		case 4://开口向右
			if (m_Line.x0<m_Line.x1)
			{
				m_CutGuideL.x0 = m_Line.x1;
				m_CutGuideL.y0 = m_Line.y1;
			}
			else
			{
				m_CutGuideL.x0 = m_Line.x0;
				m_CutGuideL.y0 = m_Line.y0;
			}
			break;
		default:
			break;
		}
	}
	return m_CutGuideL;
}
//输入起止封闭环基本图元，和封闭环单双性质，求切割引刀线基本参数
Line_para CutLeadLine::Get_CutLeadLine(Line_para a, Line_para b, bool m)
{
	Point start;
	double k = 0.0;
	Line_para m_Line;
	Line_para m_CutGuideL;
	int m_opendirect;//开口方向判断,1向上开，2向下开，3向左开，4向右开
	start.x = a.x0;
	start.y = a.y0;
	a.k = Get_k_twoPoint(a);
	b.k = Get_k_twoPoint(b);
	m_opendirect = Get_OpenDirect(a, b);//判断夹角开口方向
	k = Get_k_twoK(a.k, b.k, m_opendirect);//求切割引刀线斜率
	m_Line = Get_CutLine_Point(start, k);
	m_CutGuideL = Get_CutLine_StartPoint(m_Line, m_opendirect, m, start);
	return m_CutGuideL;
}



