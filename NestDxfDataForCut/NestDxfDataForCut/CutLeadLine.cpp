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
	r = m_CutLineLength;//引刀线长度
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
//核心代码
//核心代码
//核心代码
//切割引导线干涉判断，输入当前需要判断切割引导线的封闭环，然后判断其与包括自身在内的兄弟封闭环是否干涉。
bool CutLeadLine::JudgeCGLineVSGeomclosedH(GeomCloseHEAD*pCHtemp)
{
	GeomCloseHEAD*pHtemp;//用来存储封闭环双向链表头结点
	GeomCloseHEAD*pHOVtemp;//用来存储奇偶层封闭环头结点
	GeomEleNode*pCGLine;//用来存储切割引导线基本图元
	bool m_IfCGLIeter = false;//存储是否切割引导线干涉判断值,初始化为未干涉
	bool m_Singlelayer=pCHtemp->m_Singlelayer;//加入奇偶层性质可以减少判别的数量
	pCGLine = pCHtemp->FirstGeomele;//切割引导线
	if (m_Singlelayer)//封闭环是奇层时候，则判断封闭环所在的一圈
	{
		pHOVtemp = pCHtemp;//奇层封闭环则要判断的这一圈封闭环是本身所在的
	}
	else//封闭环是偶层时候
	{
		pHOVtemp = pCHtemp->FirstInsideGCloseNode;//偶层封闭环时，则要判断的这一圈封闭环则是子封闭环所在的
	}
	//以上确定了所要循环的封闭环到底是本身层还是子层
	//下面要确定循环的开始，然后将整个封闭环双向链表全部循环一遍
	//（但这样算法会慢，以后可以考虑在一个阈值范围内的兄弟封闭环中考虑干涉。）
	//（但这样算法会慢，以后可以考虑在一个阈值范围内的兄弟封闭环中考虑干涉。）
	//（但这样算法会慢，以后可以考虑在一个阈值范围内的兄弟封闭环中考虑干涉。）
	if (!(pHOVtemp->prevGeomcloseNode))//如果该封闭环是首节点
	{
		pHtemp = pHOVtemp;//循环从它本身开始
	}
	//如果不是首结点，那么也该让其回到首结点去遍历
	else
	{
		pHtemp = pHOVtemp;
		while (pHtemp)//寻找首节点
		{
			pHtemp = pHtemp->prevGeomcloseNode;
		}
	}
	//以上已经找到了封闭环的兄弟封闭环的头结点pHtemp
	while (pHtemp && (!m_IfCGLIeter))//将整个封闭环双向链表遍历，就算是该封闭环双向链表只有一个也可以
	{
		m_IfCGLIeter = JudgeCGLineVSOneClosedHead(pCGLine, pHtemp);
		if (m_IfCGLIeter)//如果有一个地方干涉了，那么就退出
		{
			return true;
			break;
		}
		pHtemp = pHtemp->nextGeomcloseNode;
	}
		return false;	
}

//核心代码
//核心代码
//核心代码
//切割引导线干涉判断,输入切割引导线与任意一个封闭环，判断其是否干涉，这是最核心代码
bool CutLeadLine::JudgeCGLineVSOneClosedHead(GeomEleNode*pCGLinetemp, GeomCloseHEAD*pCHtemp)
{
	GeomEleNode*pCGeomtemp;//存储封闭环的基本图元（现在只是考虑了直线类型的)
	pCGeomtemp = pCHtemp->FirstGeomele->nextGeomeleNode;//注意 pCHtemp->FirstGeomele是切割引导线，而后面的才是封闭环本身的基本图元
	bool m_IfCGLIeter = false;//存储是否切割引导线干涉判断值,初始化为未干涉
	//要将这个封闭环的从第二个基本图元节点到倒数第二个基本图元节点进行判断是否有干涉，核心代码
	while ((pCGeomtemp->nextGeomeleNode) && (!m_IfCGLIeter))//到倒数第二个就好了，因为倒数第一个还是切割引导线
	{
		m_IfCGLIeter = If_HaveCGLineInterPoint(pCGLinetemp, pCGeomtemp);
		//如果有干涉那么就可以返回这个封闭环了。这个代码只是在做干涉判断而已
		if (m_IfCGLIeter)
		{
			return true;
			break;
		}
		pCGeomtemp = pCGeomtemp->nextGeomeleNode;//注意 pCHtemp->FirstGeomele是切割引导线，而后面的才是封闭环本身的基本图元
	}
	return false;
}
//干涉的原则是交点在切割引导线的两个点之间。
bool CutLeadLine::If_HaveCGLineInterPoint(GeomEleNode*pCGLinetemp, GeomEleNode*pCGeomtemp)
{
	return true;
}
//输入切割切割引导线有干涉的封闭环，然后调整其切割引导线
void CutLeadLine::ChangeCGLine(GeomCloseHEAD*pCHtemp)
{

}




