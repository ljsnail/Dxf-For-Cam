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
	//先预处理，将x0定义为比x1小的坐标
	if (x1<=x2)
	{
		Lp.x0 = x1;
		Lp.x1 = x2;
		Lp.y0 = y1;
		Lp.y1 = y2;
	}
	else
	{
		Lp.x0 = x2;
		Lp.x1 = x1;
		Lp.y0 = y2;
		Lp.y1 = y1;
	}
	Lp.k = k;//保存切割引刀线的斜率，以后用得着
	return Lp;
}
//给直线类型的加入切割引刀线
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
//给圆类型的切割引导线用的，少了中间的开口方向参数m_OpenDirect,多了圆心坐标Circle_Center
Line_para CutLeadLine::Get_CutLine_StartPoint(Line_para m_Line, Point Circle_Center, bool m, Point start)
{
	Line_para m_CutGuideLine;
	Point p1, p2;
	double d1,d2;//求两点之间的距离
	m_CutGuideLine.x1 = start.x;//start就是圆原来的切割控制点
	m_CutGuideLine.y1 = start.y;
	p1.x = m_Line.x0;
	p1.y = m_Line.y0;
	p2.x = m_Line.x1;
	p2.y = m_Line.y1;
	//求m_Line两个点到圆心的距离
	d1 = sqrt((p1.x - Circle_Center.x)*(p1.x - Circle_Center.x) + (p1.y - Circle_Center.y)*(p1.y - Circle_Center.y));
	d2 = sqrt((p2.x - Circle_Center.x)*(p2.x - Circle_Center.x) + (p2.y - Circle_Center.y)*(p2.y - Circle_Center.y));

	//对于圆来说区分了奇偶层之后，就只剩下选圆内的点还是圆外的点的问题了。
	//现在是去掉了板材外轮廓，所以是单层往外取，双层往里取
	//这里涉及到m_Line中的两个点，该如何选择？
	//应该是要把圆的圆心也加入进来，双层往里选则选离圆心近的，单层往外选则选离圆心远的
	if (m)//单层
	{
		//往外选,则应该选择远离圆心的
		if (d1<d2)
		{
			m_CutGuideLine.x0 = m_Line.x1;
			m_CutGuideLine.y0 = m_Line.y1;
			
		}
		else
		{
			m_CutGuideLine.x0 = m_Line.x0;
			m_CutGuideLine.y0 = m_Line.y0;
		}
	}
	else//双层
	{
		//往里选，则选靠近圆心的
		if (d1<d2)
		{
			m_CutGuideLine.x0 = m_Line.x0;
			m_CutGuideLine.y0 = m_Line.y0;
			
		}
		else
		{
			m_CutGuideLine.x0 = m_Line.x1;
			m_CutGuideLine.y0 = m_Line.y1;
		}
	}
	return m_CutGuideLine;
}

//直线类型的切割引导线的生产方式
//输入起止封闭环基本图元，和封闭环单双性质，求切割引刀线基本参数
Line_para CutLeadLine::Get_CutLeadLine(Line_para a, Line_para b, bool m, int m_TypeCGLine)
{
	Point start;
	double k = 0.0;
	Line_para m_Line;
	Line_para m_CutGuideL;
	int m_opendirect;//开口方向判断,1向上开，2向下开，3向左开，4向右开
	start.x = a.x0;//获取切割控制点
	start.y = a.y0;
	a.k = Get_k_twoPoint(a);
	b.k = Get_k_twoPoint(b);
	m_opendirect = Get_OpenDirect(a, b);//判断夹角开口方向
	k = Get_k_twoK(a.k, b.k, m_opendirect);//求切割引刀线斜率
	m_Line = Get_CutLine_Point(start, k);
	if (1==m_TypeCGLine)//按照封闭环奇偶层信息生成切割引导线
	{
		m_CutGuideL = Get_CutLine_StartPoint(m_Line, m_opendirect, m, start);
	}
	else//此时m_TypeCGLine=2;要按照调整切割引导线的方式生成
	{
		m_CutGuideL = Get_ChangeCutLine_StartPoint(m_Line, m_opendirect, start);
	}
	return m_CutGuideL;
}
//圆类型的切割引导线
//由于圆类型的切割引导线生产的方式是两点连线法，所以输入上一个封闭环的切割控制点，和当前封闭环的切割控制点，其中m_Singlelayer为封闭环的奇偶层信息，m_TypeCGLine为生成切割引导线的方式，1为按奇偶层生成，2为按调整方式生成
//其中参数Last_GClose为上一个封闭环的切割控制点，Current_GClose为当前圆的切割控制点，
Line_para  CutLeadLine::Get_CutLeadLine(Point Last_GClose, Point Current_GClose, Point Circle_Center, bool m_Singlelayer, int m_TypeCGLine)
{
	Point start;
	double k = 0.0;
	Line_para m_MLine;//切割引导线的母线
	Line_para m_Line;//切割引导线
	Line_para m_CutGuideLine;
	start.x = Current_GClose.x;
	start.y = Current_GClose.y;
	m_MLine.x0 = Current_GClose.x;
	m_MLine.y0 = Current_GClose.y;
	m_MLine.x1 = Last_GClose.x;
	m_MLine.y1 = Last_GClose.y;
	k = Get_k_twoPoint(m_MLine);
	m_Line = Get_CutLine_Point(start, k);
	if (1 == m_TypeCGLine)//按照封闭环奇偶层信息生成切割引导线
	{
		m_CutGuideLine = Get_CutLine_StartPoint(m_Line, Circle_Center, m_Singlelayer, start);
	}
	else//此时m_TypeCGLine=2;要按照调整切割引导线的方式生成
	{
		//m_CutGuideLine = Get_ChangeCutLine_StartPoint(m_Line, Circle_Center, m_Singlelayer, start);
	}
	return m_CutGuideLine;


	
}

//核心代码
//核心代码
//核心代码
//切割引导线干涉判断，输入当前需要判断切割引导线的封闭环，然后判断其与包括自身在内的兄弟封闭环是否干涉。
bool CutLeadLine::JudgeCGLineVsGeomclosedH(GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head)
{
	GeomCloseHEAD*pHtemp;//用来存储封闭环双向链表头结点
	GeomCloseHEAD*pHOVtemp;//用来存储奇偶层封闭环头结点
	GeomEleNode*pCGLine;//用来存储切割引导线基本图元
	//GeomEleNode*Add_more_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//切割引入线
	bool m_MaybeCGLInter = false;//预先判断下是不是可能干涉
	bool m_IfCGLIeter = false;//存储是否切割引导线干涉判断值,初始化为未干涉
	bool m_Singlelayer=pCHtemp->m_Singlelayer;//加入奇偶层性质可以减少判别的数量
	//获取需要判断的封闭环的圆弧切割引导线与直线型切割引导线
	//将圆弧与直线型切割引导线归为一类
	pCGLine = GetLimtOfAuxiliaryLine(pCHtemp);

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
		while (pHtemp->prevGeomcloseNode)//寻找首节点
		{
			pHtemp = pHtemp->prevGeomcloseNode;
		}
	}
	//以上已经找到了封闭环的兄弟封闭环的头结点pHtemp
	while (pHtemp && (!m_IfCGLIeter))//将整个封闭环双向链表遍历，就算是该封闭环双向链表只有一个也可以
	{
		//先预先判断下切割引导线的包络矩形与封闭环的包络矩形是否相交，相交才有可能会有干涉
		m_MaybeCGLInter = If_HaveCGLineInterPoint_UseRect(pCGLine, pHtemp);
		if (m_MaybeCGLInter)
		{
			//相交，则进一步判断是否干涉
			m_IfCGLIeter = JudgeCGLineVsOneClosedHead(pCGLine, pHtemp, m_ceramic_Head);
			if (m_IfCGLIeter)//如果有一个地方干涉了，那么就退出
			{
				return true;
				break;
			}
		}
		
		pHtemp = pHtemp->nextGeomcloseNode;
	}
		return false;	
}

//核心代码
//核心代码
//核心代码
//切割引导线干涉判断,输入切割引导线与任意一个封闭环，判断其是否干涉，这是最核心代码
bool CutLeadLine::JudgeCGLineVsOneClosedHead(GeomEleNode*pCGLine, GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head)
{
	GeomEleNode*pCGeomtemp;//存储封闭环的基本图元（现在只是考虑了直线类型的)
	bool m_IfCGLIeter = false;//存储是否切割引导线干涉判断值,初始化为未干涉
	//注意：但pCHtemp是板材外轮廓时候，要从第一个直线开始直到最后一个直线为止。
	if (pCHtemp == m_ceramic_Head)
	{
		pCGeomtemp = pCHtemp->FirstGeomele;//注意 pCHtemp为板材外轮廓时候没有切割引刀线
		//要将板材外轮廓的从第一个基本图元节点到倒数第一个基本图元节点进行判断是否有干涉，核心代码
		while ((pCGeomtemp) && (!m_IfCGLIeter))//到倒数第一个，因为板材外轮廓没有切割引导线
		{
			m_IfCGLIeter = If_HaveCGLineInterPoint(pCGLine, pCGeomtemp);
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
	else//其他切割封闭环则有切割引刀线
	{
		pCGeomtemp = pCHtemp->FirstGeomele->nextGeomeleNode;//注意 pCHtemp->FirstGeomele是切割引导线，而后面的才是封闭环本身的基本图元
		//要将这个封闭环的从第二个基本图元节点到倒数第二个基本图元节点进行判断是否有干涉，核心代码
		while ((pCGeomtemp->nextGeomeleNode) && (!m_IfCGLIeter))//到倒数第二个就好了，因为倒数第一个还是切割引导线
		{
			m_IfCGLIeter = If_HaveCGLineInterPoint(pCGLine, pCGeomtemp);
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
}
//干涉的原则是交点在切割引导线的两个点之间。
//输入切割引刀线与另一个基元，求其交点，然后判断交点是否在切割引刀线的两个点之间
//注意切割引导线有可能是圆弧、直线，而另一个基元可能也是圆弧，直线，或者圆。
//最底层代码，核心层；
bool CutLeadLine::If_HaveCGLineInterPoint(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	
	bool m_IfInter=false;//判断是否干涉,默认不干涉
	
	//但切割引导线是直线，且基元也是直线时候
	//求两条直线的交点
	//先求直线的标准参数abc
	//重点在于将所有情况的交点都考虑进去。
	//切割引导线是直线，而判断的基本图元也是直线
	if ((61 == pCGLine->m_GeomStandData.m_typegeomele) && (1 == pCGeomtemp->m_GeomStandData.m_typegeomele))
	{
		m_IfInter = CheckCGLineVsLine(pCGLine, pCGeomtemp);
	}
	//切割引导线是直线，而判断的基本图元是圆
	else if ((61 == pCGLine->m_GeomStandData.m_typegeomele) && (3 == pCGeomtemp->m_GeomStandData.m_typegeomele))
	{
		m_IfInter = CheckCGLineVsCircle(pCGLine, pCGeomtemp);
	}
	//切割引导线是圆弧，而判断的基本图元是圆
	else if ((62 == pCGLine->m_GeomStandData.m_typegeomele) && (3 == pCGeomtemp->m_GeomStandData.m_typegeomele))
	{
		m_IfInter = CheckCGArcVsCircle(pCGLine, pCGeomtemp);
	}
	//切割引导线是直线，而判断的基本图元是圆弧，或者切割引导线是圆弧，而判断的基本图元是直线
	else if (((61 == pCGLine->m_GeomStandData.m_typegeomele) && (2 == pCGeomtemp->m_GeomStandData.m_typegeomele)) || ((62 == pCGLine->m_GeomStandData.m_typegeomele) && (1 == pCGeomtemp->m_GeomStandData.m_typegeomele)))
	{
		m_IfInter = CheckCGLineVsCGArc(pCGLine, pCGeomtemp);
	}
	return m_IfInter;
}
//直线型切割引导线与直线型基本图元的干涉判断
bool CutLeadLine::CheckCGLineVsLine(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	Point m_CGLineStart, m_CGLineEnd;//存储切割引刀线的起止点
	Point m_LineStart, m_LineEnd;//存储要判断直线的起止点
	Point m_InterPoint;//交点
	Line_Inter m_CGLine, m_Line;//存储切割引刀线与疑似干涉直线的abc参数
	Line_Point m_CGlinePoint;//存储切割引刀线中xy的坐标顺序
	Line_Point m_linePoint;//存储干涉线中xy的坐标顺序
	double m_x, m_y;
	//存储切割引刀线的控制点
	m_CGLineStart.x = pCGLine->m_GeomStandData.GeoEle_start_x0;
	m_CGLineStart.y = pCGLine->m_GeomStandData.GeoEle_start_y0;
	m_CGLineEnd.x = pCGLine->m_GeomStandData.GeoEle_start_x1;
	m_CGLineEnd.y = pCGLine->m_GeomStandData.GeoEle_start_y1;
	//存储待判断线段的控制点
	m_LineStart.x = pCGeomtemp->m_GeomStandData.GeoEle_start_x0;
	m_LineStart.y = pCGeomtemp->m_GeomStandData.GeoEle_start_y0;
	m_LineEnd.x = pCGeomtemp->m_GeomStandData.GeoEle_start_x1;
	m_LineEnd.y = pCGeomtemp->m_GeomStandData.GeoEle_start_y1;
	m_CGLine = GetLine(m_CGLineStart, m_CGLineEnd);
	m_Line = GetLine(m_LineStart, m_LineEnd);
	//求两直线的交点
	m_InterPoint = GetCrossPoint(m_CGLine, m_Line);
	//判断交点是不是在切割引导线的范围内。
	//先将切割引导线的两个端点值按大小排序范围
	m_CGlinePoint = GetPointOrder(m_CGLineStart, m_CGLineEnd);
	//再将与之干涉的线的两个端点值按大小排序范围
	m_linePoint = GetPointOrder(m_LineStart, m_LineEnd);
	//先排除交点不在干涉线范围内的点
	//如果交点在干涉线内，说明是实际相交
	if ((m_linePoint.x_min <= m_InterPoint.x) && (m_InterPoint.x <= m_linePoint.x_max) && (m_linePoint.y_min <= m_InterPoint.y) && (m_InterPoint.y <= m_linePoint.y_max))
	{
		//再判断交点在不在切割引导线内，如果在说明是实际相交
		if ((m_CGlinePoint.x_min <= m_InterPoint.x) && (m_InterPoint.x <= m_CGlinePoint.x_max) && (m_CGlinePoint.y_min <= m_InterPoint.y) && (m_InterPoint.y <= m_CGlinePoint.y_max))
		{
			m_x = fabs(m_InterPoint.x - m_CGLineEnd.x);
			m_y = fabs(m_InterPoint.y - m_CGLineEnd.y);
			//判断交点与切割线终点是否重合
			if ((m_x< EPSILON) && (m_y< EPSILON))//说明如今这两点相同，那么不是干涉，而是同个封闭环中切割引导线与封闭环的连接点
			{
				return  false;
			}
			//否则就是实际的干涉
			else
			{
				return  true;
			}
		}
		//否则只是切割引刀线的延伸线与之相交
		else
		{
			return  false;
		}
	}
	//说明是假相交
	else
	{
		return  false;
	}
}
//直线型切割引导线与圆基本图元的干涉判断
bool CutLeadLine::CheckCGLineVsCircle(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	Point m_CGLineStart, m_CGLineEnd;//存储切割引刀线的起止点
	Point m_Circent;//圆心点
	Point m_InterP1, m_InterP2;//线段与圆的两个交点
	Line_Inter m_CGLine;//存储切割引刀线与疑似干涉直线的abc参数
	Line_Point m_CGlinePoint;//存储切割引刀线中xy的坐标顺序
	double m_x, m_y;
	double m_r, m_Circent_x, m_Circent_y;//保存圆的半径，圆心坐标
	double m_d;//圆心到切割引导线的距离
	//存储切割引刀线的控制点
	m_CGLineStart.x = pCGLine->m_GeomStandData.GeoEle_start_x0;
	m_CGLineStart.y = pCGLine->m_GeomStandData.GeoEle_start_y0;
	m_CGLineEnd.x = pCGLine->m_GeomStandData.GeoEle_start_x1;
	m_CGLineEnd.y = pCGLine->m_GeomStandData.GeoEle_start_y1;
	//存储待判断圆的控制点
	m_r = pCGeomtemp->m_GeomStandData.m_circle.m_Circle_r;
	m_Circent.x = pCGeomtemp->m_GeomStandData.m_circle.m_Circent_x;
	m_Circent.y = pCGeomtemp->m_GeomStandData.m_circle.m_Circent_y;
	//先判断圆心到切割引导线的距离，如果大于半径，那么就不可能是相交的。
	//m_d = CalcuDistance(m_Circent, m_CGLineStart, m_CGLineEnd);
	//如果距离不超过半径，那么就是相交的，起码有一个交点
	//	if (m_r <= m_d)
	//{
	//}
	//超过半径，那么就不可能是相交的
	//else
	//{
	//	return false;
	//}
	//求直线与圆的交点
	//LineInterCircle直接可以返回是否相交，以及交点。要根据交点来判断是否真的相交，还是有相交的趋势。
	if (!LineInterCircle(m_CGLineStart, m_CGLineEnd, m_Circent, m_r, m_InterP1, m_InterP2))
	{
		return false;//不相交
	}
	//相交，根据交点坐标值进一步判断是否真的相交。
	else
	{
		//相求直线型切割引导线的范围
		m_CGlinePoint = GetPointOrder(m_CGLineStart, m_CGLineEnd);
		//判断交点在不在引导线范围内
		if((((m_CGlinePoint.x_min <= m_InterP1.x) && (m_InterP1.x <= m_CGlinePoint.x_max)) && ((m_CGlinePoint.y_min <= m_InterP1.y) && (m_InterP1.y <= m_CGlinePoint.y_max)))//m_InterP1这个交点是有用的
			|| (((m_CGlinePoint.x_min <= m_InterP2.x) && (m_InterP2.x <= m_CGlinePoint.x_max)) && ((m_CGlinePoint.y_min <= m_InterP2.y) && (m_InterP2.y <= m_CGlinePoint.y_max))))//或者m_InterP2这个交点是有用的
		{
			return true;//实际相交
		}
		else//只是又相交的趋势，但并没有相交
		{
			return false;//假相交

		}
		
	}
}
//圆弧切割引导线与圆基本图元的干涉判断
bool CutLeadLine::CheckCGArcVsCircle(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	GeomEleNode*pCGLineForArc = (GeomEleNode*)malloc(sizeof(GeomEleNode));//用圆弧切割引导线的两个端点连线段来代替圆弧
	Point m_Circle, m_Arc;//存储圆的圆心坐标，与圆弧切割引导线的圆心坐标
	double m_d;//存储圆心距
	bool m_IfInter = true;//存储代直线pCGLineForArc是否与圆相交,默认是相交
	//只需要传入这几个控制点就好了，其他参数不需要
	pCGLineForArc->m_GeomStandData.GeoEle_start_x0 = pCGLine->m_GeomStandData.GeoEle_start_x0;
	pCGLineForArc->m_GeomStandData.GeoEle_start_y0 = pCGLine->m_GeomStandData.GeoEle_start_y0;
	pCGLineForArc->m_GeomStandData.GeoEle_start_x1 = pCGLine->m_GeomStandData.GeoEle_start_x1;
	pCGLineForArc->m_GeomStandData.GeoEle_start_y1 = pCGLine->m_GeomStandData.GeoEle_start_y1;
	//借助圆弧的两个端点的连线段与圆进行判断，如果该线段与圆有交点，实际的交点，那么就可以说是圆弧与圆干涉
	m_IfInter = CheckCGLineVsCircle(pCGLineForArc, pCGeomtemp);
	//如果没有直线段与圆没有干涉的情况下，进一步判断，圆弧是不是在圆内部，判断的标准是圆与圆弧的圆心小于半径
	if (m_IfInter)//如果相交，就肯定是相交的
	{
		return true;
	}
	else//如果不相交，还要判断是不是圆弧在圆内部，即是直线在圆内部的情况
	{
		//判断的方法是用圆心距
		m_Circle.x = pCGeomtemp->m_GeomStandData.m_circle.m_Circent_x;
		m_Circle.y = pCGeomtemp->m_GeomStandData.m_circle.m_Circent_y;
		m_Arc.x = pCGLine->m_GeomStandData.m_arc.Arccent_x;
		m_Arc.y = pCGLine->m_GeomStandData.m_arc.Arccent_y;
		//求圆心距
		m_d = fabs(sqrt(((m_Circle.x - m_Arc.x)*(m_Circle.x - m_Arc.x)) + ((m_Circle.y - m_Arc.y)*(m_Circle.y - m_Arc.y))));//这里求的是两点之间的最短距离
		//圆心距小于圆的半径，说明圆弧包含在内部
		if (m_d<pCGeomtemp->m_GeomStandData.m_circle.m_Circle_r)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
}
//圆弧与直线的干涉判断
//此函数可以判断直线切割引导线与圆弧基本图元的干涉判断
//也可以判断圆弧切割引导线与直线基本图元的干涉判断
bool CutLeadLine::CheckCGLineVsCGArc(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	//注意pCGLinetemp可能是直线（此时pCGeomtemp为圆弧），也可能是圆弧（此时pCGeomtemp为直线）,
	//方法是先按照直线与圆的干涉判断来求交点
	GeomEleNode*pCGLinetemp;//存储直线，不管这是切割引导线直线还是基本图元直线
	GeomEleNode*pCGArc;//存储圆弧，不管这是切割引导线圆弧还是基本图元圆弧
	GCIRCLE pCGCricle;//存储圆，将圆弧的数据传给圆，然后调用圆的求角度的命令，求交点角度
	Point m_Circent;//存储圆弧的圆心
	Point m_InterP1, m_InterP2;//线段与圆的两个交点
	Point m_CGLineStart, m_CGLineEnd;//存储直线的起止点
	Line_Point m_CGlinePoint;//存储切割引刀线中xy的坐标顺序
	double m_r;//保存圆的半径，圆心坐标
	double m_InterPAngle1, m_InterPAngle2;//交点与圆弧切割引导线的角度范围。
	double m_ArcAngleStart, m_ArcAngleEnd;//存储圆弧切割引导线的起止角
	double m_MidAngle;//存储圆弧切割引导线的中间角度，用来判断半圆切割引导线是哪一块
	bool m_IfClockRota=false;//判断圆弧切割引导线是否顺时针旋转，默认是逆时针
	if (61 == pCGLine->m_GeomStandData.m_typegeomele)//是直线
	{
		pCGLinetemp = pCGLine;
		pCGArc = pCGeomtemp;
	}
	else//此时pCGLinetemp为圆弧
	{
		pCGLinetemp = pCGeomtemp;
		pCGArc = pCGLine;
	}
	//以上已经将直线与圆弧划分出来了
	//用直线与圆的干涉判断算法判断是否有干涉。
	//存储切割引刀线的控制点
	m_CGLineStart.x = pCGLinetemp->m_GeomStandData.GeoEle_start_x0;
	m_CGLineStart.y = pCGLinetemp->m_GeomStandData.GeoEle_start_y0;
	m_CGLineEnd.x = pCGLinetemp->m_GeomStandData.GeoEle_start_x1;
	m_CGLineEnd.y = pCGLinetemp->m_GeomStandData.GeoEle_start_y1;
	//存储待判断圆的控制点
	//圆弧参数转给圆
	pCGCricle.m_Circle_r = m_r = pCGArc->m_GeomStandData.m_arc.m_Arc_r;
	pCGCricle.m_Circent_x = m_Circent.x = pCGArc->m_GeomStandData.m_arc.Arccent_x;
	pCGCricle.m_Circent_y = m_Circent.y = pCGArc->m_GeomStandData.m_arc.Arccent_y;
	//存储切割引导线的起止角
	m_ArcAngleStart = pCGArc->m_GeomStandData.m_arc.m_ArcAngle_start;
	m_ArcAngleEnd = pCGArc->m_GeomStandData.m_arc.m_ArcAngle_end;
	m_MidAngle = pCGArc->m_GeomStandData.k;
	//求直线与圆的交点
	//LineInterCircle直接可以返回是否相交，以及交点。要根据交点来判断是否真的相交，还是有相交的趋势。
	if (!LineInterCircle(m_CGLineStart, m_CGLineEnd, m_Circent, m_r, m_InterP1, m_InterP2))
	{
		return false;//不相交,那么肯定是直线与圆弧也不相交的
	}
	//相交，根据交点坐标值进一步判断是否真的相交。
	//如果是直线切割引导线那么就判断交点是不是在线段的范围内
	//如果是圆弧引导线，那么就要判断该交点所引起的起止角度在不在原来的起止范围内
	else
	{
		//要区分哪个才是切割引导线
		//如果直线是切割引导线
		if (61 == pCGLine->m_GeomStandData.m_typegeomele)
		{
			//相求直线型切割引导线的范围
			m_CGlinePoint = GetPointOrder(m_CGLineStart, m_CGLineEnd);
			//判断交点在不在引导线范围内
			if ((((m_CGlinePoint.x_min <= m_InterP1.x) && (m_InterP1.x <= m_CGlinePoint.x_max)) && ((m_CGlinePoint.y_min <= m_InterP1.y) && (m_InterP1.y <= m_CGlinePoint.y_max)))//m_InterP1这个交点是有用的
				|| (((m_CGlinePoint.x_min <= m_InterP2.x) && (m_InterP2.x <= m_CGlinePoint.x_max)) && ((m_CGlinePoint.y_min <= m_InterP2.y) && (m_InterP2.y <= m_CGlinePoint.y_max))))//或者m_InterP2这个交点是有用的
			{
				return true;//实际相交
			}
			else//只是又相交的趋势，但并没有相交
			{
				return false;//假相交

			}
		}
		//如果是圆弧切割引导线那么就要判断该交点在圆弧上的角度是不是在圆弧的角度范围内。
		else
		{
			//获取两个交点在圆中的角度
			//注意由于m_InterP1与m_InterP2的精度问题，所以，求的角度可能与原来的角度有一定的差距。
			m_InterPAngle1 = m_geomele.ForCircleStartAngle(m_InterP1.x, m_InterP1.y, pCGCricle);
			m_InterPAngle2 = m_geomele.ForCircleStartAngle(m_InterP2.x, m_InterP2.y, pCGCricle);
			//判断角度是不是在圆弧的角度范围内。
			//注意圆弧的切割起止角跨度是180°，但是有可能是正时针旋转180与逆时针旋转180；
			//如果起点角度大于180，且是顺时针旋转的话，终点角度就会是超过360，如起始角度为200°，那么终止就是380°（实际上是20°），则这里要判断200°-360°与0°-20°
			//如果起点角度小于180，且是逆时针旋转的话，终点角度就会是小于0°，如起始角度为160°，那么终止就是-20°（实际上是340°），则这里要判断0°-160°与340°-360°
			//所以这个算法的核心在于如何处理圆弧的起止角度问题，以及看是顺时针旋转还是逆时针旋转问题
			///////////////////////////////////////////////
			//分出起止角的大小，下面用得着
			//让起止角小于终止角
			if (m_ArcAngleStart<m_ArcAngleEnd)
			{
				;//不用变
			}
			else//否则要把起止角度调整
			{
				m_ArcAngleStart = m_ArcAngleEnd;
				m_ArcAngleEnd = pCGArc->m_GeomStandData.m_arc.m_ArcAngle_start;
			}
			//特别注意，可用下面代码判断的原因是圆弧为半圆，其跨度为180°，其他跨度小于180°的圆弧不适用
			//特别注意，可用下面代码判断的原因是圆弧为半圆，其跨度为180°，其他跨度小于180°的圆弧不适用
			//特别注意，可用下面代码判断的原因是圆弧为半圆，其跨度为180°，其他跨度小于180°的圆弧不适
			//上半（左半）部分圆弧判断
			if (fabs(m_MidAngle - (m_ArcAngleStart + m_ArcAngleEnd))<1)//1是因为精度不一定很高，原则上是要等于0的
			{
				//说明该段圆弧可以看做是由小角度经过顺时针旋转到大角度
				//判断交点是不是在这段圆弧中
				if (((m_ArcAngleStart <= m_InterPAngle1) && (m_InterPAngle1 <= m_ArcAngleEnd))//交点1在这段圆弧内
					|| ((m_ArcAngleStart <= m_InterPAngle2) && (m_InterPAngle2 <= m_ArcAngleEnd)))//或者交点2在这段圆弧内
				{
					return true;//说明是相交的
				}
				else
				{
					return false;//说明不相交
				}
			}
			//否则就是下半（右半）部分圆弧判断
			else
			{
				//说明该段圆弧可以看做是由大角度经过顺时针旋转到360，然后又从0°经过顺时针旋转到小角度。
				//判断交点是不是在这两段圆弧内部
				if((((m_ArcAngleEnd <= m_InterPAngle1) && (m_InterPAngle1 <= 360))//交点1在第一段圆弧内
					|| ((0<= m_InterPAngle1) && (m_InterPAngle1 <= m_ArcAngleStart)))//或者交点1在第二段圆弧内
					|| (((m_ArcAngleEnd <= m_InterPAngle2) && (m_InterPAngle2 <= 360))//交点2在第二段圆弧内
					|| ((0 <= m_InterPAngle2) && (m_InterPAngle2 <= m_ArcAngleStart))))//或者交点2在第一段圆弧内
				{
					return true;//说明是相交的
				}
				else
				{
					return false;//说明不相交
				}
			
			}

		}
	}


}
//求点到线段之间的距离
double CutLeadLine::CalcuDistance(Point m_point, Point m_LineP1, Point m_LineP2)
{
	double x, y, x1, y1, x2, y2;
	x = m_point.x;
	y = m_point.y;
	x1 = m_LineP1.x;
	y1 = m_LineP1.y;
	x2 = m_LineP2.x;
	y2 = m_LineP2.y;
	//算法主体
	double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);
	if (cross <= 0)
		return sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));

	double d2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	if (cross >= d2)
		return sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));

	double r = cross / d2;
	double px = x1 + (x2 - x1) * r;
	double py = y1 + (y2 - y1) * r;
	return sqrt((x - px) * (x - px) + (y - py) * (y - py));
}
//求线段与圆的交点
//与圆可能存在两个交点，如果存在两个交点在ptInter1和ptInter2都为有效值，如果有一个交点，则ptInter2的值为无效值，此处为65536.0
// 线段起点,线段终点,圆心坐标,全局的交点坐标
bool CutLeadLine::LineInterCircle(const Point ptStart, const Point ptEnd, const Point ptCenter, const double Radius, Point& ptInter1, Point& ptInter2)
{
	ptInter1.x = ptInter2.x = 65536.0f;
	ptInter2.y = ptInter2.y = 65536.0f;

	double fDis = sqrt((ptEnd.x - ptStart.x) * (ptEnd.x - ptStart.x) + (ptEnd.y - ptStart.y) * (ptEnd.y - ptStart.y));

	Point d;
	d.x = (ptEnd.x - ptStart.x) / fDis;
	d.y = (ptEnd.y - ptStart.y) / fDis;

	Point E;
	E.x = ptCenter.x - ptStart.x;
	E.y = ptCenter.y - ptStart.y;

	double a = E.x * d.x + E.y * d.y;
	double a2 = a * a;

	double e2 = E.x * E.x + E.y * E.y;

	double r2 = Radius * Radius;

	if ((r2 - e2 + a2) < 0)
	{
		return false;
	}
	else
	{
		double f = sqrt(r2 - e2 + a2);

		double t = a - f;

		if (((t - 0.0) > -EPSILON) && (t - fDis) < EPSILON)
		{
			ptInter1.x = ptStart.x + t * d.x;
			ptInter1.y = ptStart.y + t * d.y;
		}

		t = a + f;

		if (((t - 0.0) > -EPSILON) && (t - fDis) < EPSILON)
		{
			ptInter2.x = ptStart.x + t * d.x;
			ptInter2.y = ptStart.y + t * d.y;
		}

		return true;
	}
}


//输入直线的两个端点值，然后求标准直线的abc三个参数
Line_Inter CutLeadLine::GetLine(Point m_StratPoint, Point m_EndPoint)
{
	Line_Inter m_Line;
	m_Line.a = m_StratPoint.y - m_EndPoint.y;
	m_Line.b = m_EndPoint.x - m_StratPoint.x;
	m_Line.c = m_StratPoint.x*m_EndPoint.y - m_EndPoint.x*m_StratPoint.y;
	return m_Line;
}
//输入切割引刀线以及另一条疑似有干涉的线段的abc参数，求交点
Point CutLeadLine::GetCrossPoint(Line_Inter l1, Line_Inter l2)
{
	Point pTemp;
	double D;//如果D=0说明两条直线是平行的。目前没有做这一块工作
	D = l1.a*l2.b - l2.a*l1.b;
	Point p;
	pTemp.x = (l1.b*l2.c - l2.b*l1.c) / D;
	pTemp.y = (l1.c*l2.a - l2.c*l1.a) / D;
	return pTemp;//返回了交点
}
//求切割引导线两个端点四个点之间的范围
Line_Point  CutLeadLine::GetPointOrder(Point m_StratPoint, Point m_EndPoint)
{
	Line_Point m_point;
	//求x的范围
	if (m_StratPoint.x <= m_EndPoint.x)
	{
		m_point.x_min = m_StratPoint.x;
		m_point.x_max = m_EndPoint.x;
	}
	else
	{
		m_point.x_min = m_EndPoint.x;
		m_point.x_max = m_StratPoint.x;
	}
	//求y的范围
	if (m_StratPoint.y <= m_EndPoint.y)
	{
		m_point.y_min = m_StratPoint.y;
		m_point.y_max = m_EndPoint.y;
	}
	else
	{
		m_point.y_min = m_EndPoint.y;
		m_point.y_max = m_StratPoint.y;
	}
	return m_point;
}
//新提出一种根据矩形包络预判断是否有相交关系，如果有再进一步采用上述算法If_HaveCGLineInterPoint；
//用切割引导线的包络矩形与封闭环的包络矩形进行判断
bool CutLeadLine::If_HaveCGLineInterPoint_UseRect(GeomEleNode*pCGLine, GeomCloseHEAD*pCHtemp)
{
	//切割引导线有包络，封闭环也有包络
	//先判断两者的包络是否相交，封闭环的包络一般会比切割引导线的大。
	double x1_min, x1_max, y1_min, y1_max;//封闭环的包络矩形的范围
	double x2_min, x2_max, y2_min, y2_max;//切割引导线的包络矩形的范围
	double x_length, y_length;//切割引导线的长度范围；
	//封闭环的包络矩形
	x1_min = pCHtemp->m_GemoClosedLimtPoint.x_min.x;
	x1_max = pCHtemp->m_GemoClosedLimtPoint.x_max.x;
	y1_min = pCHtemp->m_GemoClosedLimtPoint.y_min.y;
	y1_max = pCHtemp->m_GemoClosedLimtPoint.y_max.y;
	//切割引导线的包络矩形
	x2_min = pCGLine->m_AuxiLineLimtPoint.x_min;
	x2_max = pCGLine->m_AuxiLineLimtPoint.x_max;
	y2_min = pCGLine->m_AuxiLineLimtPoint.y_min;
	y2_max = pCGLine->m_AuxiLineLimtPoint.y_max;
	x_length = x2_max - x2_min;
	y_length = y2_max - y2_min;
	//判断两个矩形的关系
	//这是计算包含以及相交的范围
	if ((((x1_min - x_length) <= x2_min) && (x2_max <= (x1_max + x_length))) && (((y1_min - y_length) <= y2_min) && (y2_max <= (y1_max + y_length))))
	{
		return true;
	}
	else
	{
		return false;
	}

}


//调整切割引导线
//输入切割切割引导线有干涉的封闭环，然后调整其切割引导线
void CutLeadLine::ChangeCGLine(GeomCloseHEAD*pCHtemp)
{
	GeomEleNode*cut_in_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//切割引入线
	GeomEleNode*cut_out_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//切割引出线
	//将原来的切割引导线与生产原来切割引导线的一条边重新生成切割引导线
	GeomCloseHEAD*ptemp;//备用封闭环节点
	GeomEleNode*Fnode, *Enode;
	GeomEleNode*node;
	Line_para m_startline, m_endline;//封闭环首图元节点和尾图元节点的基本数据
	Line_para m_cutline;//切割引刀线的核心参数
	int m_TypeCGLine = 2;//生成切割引导线的方式，2为调整生成
	double x0_min, y0_min, x0_max, y0_max;
	double x1_min, y1_min, x1_max, y1_max;
	bool m_Singlelayer;
	m_Singlelayer = pCHtemp->m_Singlelayer;
	//只测试直线类型的图元,并不考虑是否有交涉，不考虑圆
	Fnode = pCHtemp->FirstGeomele;//切割引导线
	Enode = Fnode->nextGeomeleNode;
	while (Enode->nextGeomeleNode->nextGeomeleNode)//生成原来切割引导线的其中一条母线，注意不是最后一个基本图元是倒数第二个，此时Enode是组成封闭环原来最后一个图元。
	{
		Enode = Enode->nextGeomeleNode;
	}
	//以下用的是角平分线法
	//首图元节点
	/*m_startline.x0 = Fnode->m_GeomStandData.GeoEle_start_x0;
	m_startline.y0 = Fnode->m_GeomStandData.GeoEle_start_y0;
	m_startline.x1 = Fnode->m_GeomStandData.GeoEle_start_x1;
	m_startline.y1 = Fnode->m_GeomStandData.GeoEle_start_y1;*/
	m_startline.x0 = Fnode->m_GeomStandData.GeoEle_start_x1;
	m_startline.y0 = Fnode->m_GeomStandData.GeoEle_start_y1;
	m_startline.x1 = Fnode->m_GeomStandData.GeoEle_start_x0;
	m_startline.y1 = Fnode->m_GeomStandData.GeoEle_start_y0;
	//尾图元节点，要注意首尾对调
	m_endline.x0 = Enode->m_GeomStandData.GeoEle_start_x1;
	m_endline.y0 = Enode->m_GeomStandData.GeoEle_start_y1;
	m_endline.x1 = Enode->m_GeomStandData.GeoEle_start_x0;
	m_endline.y1 = Enode->m_GeomStandData.GeoEle_start_y0;
	//调用切割引刀线函数
	m_cutline = Get_CutLeadLine(m_startline, m_endline, m_Singlelayer, m_TypeCGLine);
	//将切割引刀线也一并保存为同一的图元格式
	cut_in_Node->m_GeomStandData.GeoEle_start_x0 = m_cutline.x0;
	cut_in_Node->m_GeomStandData.GeoEle_start_y0 = m_cutline.y0;
	cut_in_Node->m_GeomStandData.GeoEle_start_x1 = m_cutline.x1;
	cut_in_Node->m_GeomStandData.GeoEle_start_y1 = m_cutline.y1;
	cut_in_Node->m_GeomStandData.k = m_cutline.k;
	cut_in_Node->m_GeomStandData.m_typegeomele = 6;//切割引刀线的标准
	//对于直线型的切割引刀线而言，cut_in与cut_out的数据是一样的，但起码位置要调换
	cut_out_Node->m_GeomStandData.GeoEle_start_x0 = m_cutline.x1;
	cut_out_Node->m_GeomStandData.GeoEle_start_y0 = m_cutline.y1;
	cut_out_Node->m_GeomStandData.GeoEle_start_x1 = m_cutline.x0;
	cut_out_Node->m_GeomStandData.GeoEle_start_y1 = m_cutline.y0;
	cut_out_Node->m_GeomStandData.k = m_cutline.k;
	cut_out_Node->m_GeomStandData.m_typegeomele = 6;//切割引刀线的标准
	//重点！！！
	//因为现在的封闭环是带有切入线与切出线的，所以要先将原来的切入线切出线去除掉
	//然后将现在的切入线切出线挂到封闭环的原来第二个和倒数第二个基本图元上
	pCHtemp->FirstGeomele = cut_in_Node;//切割引刀线为首图元
	cut_in_Node->nextGeomeleNode = Fnode->nextGeomeleNode;//指向原来的第二个图元
	cut_in_Node->prevGeomeleNode = NULL;//自然它就成了第一个了
	Fnode->nextGeomeleNode->prevGeomeleNode = cut_in_Node;//原来前面指向的是NULL
	//经过以上调整Fnode已经没有利用价值了,要把这块内存释放掉
	free(Fnode);
	//设置cut_out
	node = Enode->nextGeomeleNode;//原来切割引导线的引出线
	free(node);//释放掉原来切割引出线这一块内存
	Enode->nextGeomeleNode = cut_out_Node;//原来封闭环的最后一个图元指向的是node，现在指向cut_out
	cut_out_Node->prevGeomeleNode = Enode;
	cut_out_Node->nextGeomeleNode = NULL;
	//如此封闭环就加上了新的切割引刀线
}
//调整切割引刀线中取切割引刀线控制点的方法
//此时不管奇层层，都往内取
Line_para  CutLeadLine::Get_ChangeCutLine_StartPoint(Line_para m_Line, int m_OpenDirect, Point start)
{
	Line_para m_CutGuideL;
	m_CutGuideL.x1 = start.x;
	m_CutGuideL.y1 = start.y;
	m_CutGuideL.k = m_Line.k;
	//往里取
	//如果开口向上，则取y大的，开口向下，则取y小的。
	//如果开口向左，则取x小的，开口向右，则取x大的。
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

	return m_CutGuideL;
}
//切割引导线，直线与圆弧都可以一起放在这个基本图元上。
//用于求出用来判断的切割引导线的。
//这段代码用来数组，需要另外测试
GeomEleNode* CutLeadLine::GetLimtOfAuxiliaryLine(GeomCloseHEAD*pCHtemp)
{
	GeomEleNode*pFirst, *pEnd;//封闭环的第一与最后切割引导线，用在圆弧切割引导线时
	GeomEleNode*pCGLine = (GeomEleNode*)malloc(sizeof(GeomEleNode));//用来存储切割引导线
	double x_min = -9999.99, y_min = -9999.99, x_max = -9999.99, y_max = -9999.99, x_mid = -9999.99, y_mid = -9999.99;//保存切割引导线的外包络矩形。x_mid, y_mid是圆弧才有的。
	double x[3], y[3];

	GCIRCLE m_circle;//计算圆弧的终止角用
	//对于圆弧切割引导线而言，切割引导线就不仅仅是第一基本图元，还包括最后一个基本图元。
	if (61 == pCHtemp->FirstGeomele->m_GeomStandData.m_typegeomele)//如果是直线的情况下
	{
		pCGLine = pCHtemp->FirstGeomele;
		x[0] = pCGLine->m_GeomStandData.GeoEle_start_x0;
		x[1] = pCGLine->m_GeomStandData.GeoEle_start_x1;
		y[0] = pCGLine->m_GeomStandData.GeoEle_start_y0;
		y[1] = pCGLine->m_GeomStandData.GeoEle_start_y1;
		if (x[0] <= x[1])
		{
			pCGLine->m_AuxiLineLimtPoint.x_min = x[0];
			pCGLine->m_AuxiLineLimtPoint.x_max = x[1];
		}
		else
		{
			pCGLine->m_AuxiLineLimtPoint.x_min = x[1];
			pCGLine->m_AuxiLineLimtPoint.x_max = x[0];
		}
		if (y[0] <= y[1])
		{
			pCGLine->m_AuxiLineLimtPoint.y_min = y[0];
			pCGLine->m_AuxiLineLimtPoint.y_max = y[1];
		}
		else
		{
			pCGLine->m_AuxiLineLimtPoint.y_min = y[1];
			pCGLine->m_AuxiLineLimtPoint.y_max = y[0];
		}
	}
	else if (62 == pCHtemp->FirstGeomele->m_GeomStandData.m_typegeomele)
	{
		//圆弧切割引导线中的圆参数
		m_circle.m_Circent_x = pCHtemp->FirstGeomele->m_GeomStandData.m_arc.Arccent_x;
		m_circle.m_Circent_y = pCHtemp->FirstGeomele->m_GeomStandData.m_arc.Arccent_y;
		m_circle.m_Circle_r = m_CutLineLength;

		pFirst = pCHtemp->FirstGeomele;
		pEnd = pCHtemp->FirstGeomele;
		while (pEnd->nextGeomeleNode)
			pEnd = pEnd->nextGeomeleNode;
		pCGLine->m_GeomStandData = pFirst->m_GeomStandData;//先将圆弧引入线付给pCGLine
		//唯一不同的是，要将圆弧引出线pEnd的切割终点以及终止角给予pFirst
		pCGLine->m_GeomStandData.GeoEle_start_x1 = pEnd->m_GeomStandData.GeoEle_start_x1;
		pCGLine->m_GeomStandData.GeoEle_start_y1 = pEnd->m_GeomStandData.GeoEle_start_y1;
		//末端的角度应该是计算得出。
		pCGLine->m_GeomStandData.m_arc.m_ArcAngle_end = m_geomele.ForCircleStartAngle(pCGLine->m_GeomStandData.GeoEle_start_x1, pCGLine->m_GeomStandData.GeoEle_start_y1, m_circle);
		pCGLine->m_GeomStandData.k = pFirst->m_GeomStandData.m_arc.m_ArcAngle_end;//注意这个k保存的是圆弧切割引导线的中间值
		//控制矩形
		x[0] = pCGLine->m_GeomStandData.GeoEle_start_x0;//圆弧的起点
		x[1] = pCGLine->m_GeomStandData.GeoEle_start_x1;//圆弧的终点
		x[2] = pEnd->m_GeomStandData.GeoEle_start_x0;//圆弧的切点
		y[0] = pCGLine->m_GeomStandData.GeoEle_start_y0;
		y[1] = pCGLine->m_GeomStandData.GeoEle_start_y1;
		y[2] = pEnd->m_GeomStandData.GeoEle_start_y0;
		pCGLine->m_AuxiLineLimtPoint.x_min = x[0];//赋予初始值
		pCGLine->m_AuxiLineLimtPoint.x_max = x[0];//赋予初始值
		pCGLine->m_AuxiLineLimtPoint.y_min = y[0];//赋予初始值
		pCGLine->m_AuxiLineLimtPoint.y_max = y[0];//赋予初始值
		for (int i = 0; i < 2; i++)
		{
			//x
			if (x[i + 1]<=pCGLine->m_AuxiLineLimtPoint.x_min )
			{
				pCGLine->m_AuxiLineLimtPoint.x_min = x[i + 1];
			}
			if (pCGLine->m_AuxiLineLimtPoint.x_max <= x[i + 1])
			{
				pCGLine->m_AuxiLineLimtPoint.x_max = x[i + 1];
			}
			//y
			if (y[i + 1] <= pCGLine->m_AuxiLineLimtPoint.y_min)
			{
				pCGLine->m_AuxiLineLimtPoint.y_min = y[i + 1];
			}
			if (pCGLine->m_AuxiLineLimtPoint.y_max <= y[i + 1])
			{
				pCGLine->m_AuxiLineLimtPoint.y_max = y[i + 1];
			}

		}
	}
	return pCGLine;
}


