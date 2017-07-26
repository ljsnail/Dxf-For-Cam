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
bool CutLeadLine::JudgeCGLineVSGeomclosedH(GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head)
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
		while (pHtemp->prevGeomcloseNode)//寻找首节点
		{
			pHtemp = pHtemp->prevGeomcloseNode;
		}
	}
	//以上已经找到了封闭环的兄弟封闭环的头结点pHtemp
	while (pHtemp && (!m_IfCGLIeter))//将整个封闭环双向链表遍历，就算是该封闭环双向链表只有一个也可以
	{
		m_IfCGLIeter = JudgeCGLineVSOneClosedHead(pCGLine, pHtemp, m_ceramic_Head);
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
bool CutLeadLine::JudgeCGLineVSOneClosedHead(GeomEleNode*pCGLinetemp, GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head)
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
	else//其他切割封闭环则有切割引刀线
	{
		pCGeomtemp = pCHtemp->FirstGeomele->nextGeomeleNode;//注意 pCHtemp->FirstGeomele是切割引导线，而后面的才是封闭环本身的基本图元
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
}
//干涉的原则是交点在切割引导线的两个点之间。
//输入切割引刀线与另一个直线，求其交点，然后判断交点是否在切割引刀线的两个点之间
bool CutLeadLine::If_HaveCGLineInterPoint(GeomEleNode*pCGLinetemp, GeomEleNode*pCGeomtemp)
{
	Point m_CGLineStart, m_CGLineEnd;//存储切割引刀线的起止点
	Point m_LineStart, m_LineEnd;//存储要判断直线的起止点
	Point m_InterPoint;//交点
	Line_Inter m_CGLine, m_Line;//存储切割引刀线与疑似干涉直线的abc参数
	Line_Point m_CGlinePoint;//存储切割引刀线中xy的坐标顺序
	Line_Point m_linePoint;//存储干涉线中xy的坐标顺序

	double m_x, m_y;
	//存储切割引刀线的控制点
	m_CGLineStart.x = pCGLinetemp->m_GeomStandData.GeoEle_start_x0;
	m_CGLineStart.y = pCGLinetemp->m_GeomStandData.GeoEle_start_y0;
	m_CGLineEnd.x = pCGLinetemp->m_GeomStandData.GeoEle_start_x1;
	m_CGLineEnd.y = pCGLinetemp->m_GeomStandData.GeoEle_start_y1;
	//存储待判断线段的控制点
	m_LineStart.x = pCGeomtemp->m_GeomStandData.GeoEle_start_x0;
	m_LineStart.y = pCGeomtemp->m_GeomStandData.GeoEle_start_y0;
	m_LineEnd.x = pCGeomtemp->m_GeomStandData.GeoEle_start_x1;
	m_LineEnd.y = pCGeomtemp->m_GeomStandData.GeoEle_start_y1;
	//求两条直线的交点
	//先求直线的标准参数abc
	m_CGLine = GetLine(m_CGLineStart, m_CGLineEnd);
	m_Line = GetLine(m_LineStart, m_LineEnd);
	//求交点
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


