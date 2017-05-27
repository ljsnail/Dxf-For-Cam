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
	double k;//б��
	if (a.x0 != a.x1)//б�ʴ���
	{
		return k = (a.y0 - a.y1) / (a.x0 - a.x1);
	}
	else//б�ʲ����ڣ�����һ����ֵ
	{
		return k = 999.12;
	}
}
//�жϼнǵĿ��ڷ���,1Ϊ���Ͽ��ڣ�2Ϊ���¿��ڣ�3Ϊ���󿪿ڣ�4Ϊ���ҿ��ڡ�
int  CutLeadLine::Get_OpenDirect(Line_para a, Line_para b)
{
	//����ab��x0,y0������ͬ��
	int m_opendirect = 0;
	//����и���Ƶ��Y����Сֵ����ô�������Ͽ���
	if ((a.y0 <= a.y1) && (b.y0 <= b.y1))
	{
		return m_opendirect = 1;//���Ϸ��򿪿�
	}
	//����и���Ƶ��Y�����ֵ����ô�������¿���
	else if ((a.y0 >= a.y1) && (b.y0 >= b.y1))
	{
		return m_opendirect = 2;//���·��򿪿�
	}
	//���¿��ڷ���Ƚ��������֣����ҿ��ڷ�������Ҫ�ֶ����������
	//����б���������ж�����
	//�����ж����ҿ���
	else if (a.k*b.k<0)//б��һ��һ��
	{
		//xΪ���ֵ���������򿪿�
		if ((a.x0 >= a.x1) && (b.x0 >= b.x1))
		{
			return m_opendirect = 3;
		}
		else//xΪ��Сֵ�������ҷ��򿪿�
		{
			return m_opendirect = 4;
		}
	}
	//б��ͬ����ʱ��
	else if (a.k >= 0 && b.k >= 0)
	{
		//б��С��x1С��б�ʴ��x1����������
		if (a.k<b.k && a.x1<b.x1)
		{
			return m_opendirect = 3;
		}
		else//б��С��x1��б�ʴ��x1С���������ҿ�
		{
			return m_opendirect = 4;
		}
	}
	//б��ͬ����ʱ��
	else if (a.k <= 0 && b.k <= 0)
	{
		//б��С��x1��б�ʴ��x1С����������
		if (a.k<b.k && a.x1>b.x1)
		{
			return m_opendirect = 3;
		}
		else//б��С��x1С��б�ʴ��x1���������ҿ�
		{
			return m_opendirect = 4;
		}
	}
}
//��������ֱ�ߵ�б�ʣ��ͼнǵĿ��ڷ���
double CutLeadLine::Get_k_twoK(double k1, double k2, int m_opendirect)
{
	double k = 0.0;
	double a, b, c;
	double d;//�����и�������ƫת�ǵĻ���
	if ((k1 != 999.12) && (k2 != 999.12))//����ֱ�ߵ�б�ʾ����ڵ������
	{
		a = atan(k1) * 180 / pii;//��б��
		b = atan(k2) * 180 / pii;//��б��
	}
	else if (k1 == 999.12)//k1�Ǵ�ֱ��û��б��
	{
		a = 90.0;//����90��
		b = atan(k2) * 180 / pii;//��б��
	}
	else if (k2 == 999.12)//k2�Ǵ�ֱ��û��б��
	{
		a = atan(k1) * 180 / pii;//��б��
		b = 90.0;//����90��
	}
	//�Դ���90�ȵ���б�ǽ���ת��
	//����ֱ�߿��Թ��������нǣ�Ҫ�жϼнǿ��ڷ���
	//���Y����ֵ��ô�����¿��ڣ����������ô�������ҿ���
	if (m_opendirect == 3 || m_opendirect == 4)//���ҿ���
	{
		if (a*b<0)//б��һ��һ��
		{
			c = (a + b) / 2;//ֱ�����/2
		}
		else if ((a >= 0) && (b >= 0))//б�ʶ�������
		{
			if (a>b)//б�ʴ��ȡ���Ĳ��Ǻ����
			{
				a = a - 180;
			}
			else
			{
				b = b - 180;
			}
			c = (a + b) / 2;//ֱ�����/2
		}
		else//�������Ǹ��������
		{
			if (a<b)//б��С��ȡ������
			{
				a = 180 + a;
			}
			else
			{
				b = 180 + b;
			}
			c = (a + b) / 2;//ֱ�����/2
		}
	}
	else//�н����¿��ڵ�ʱ�򣬴�ʱ�и���Ƶ��yֵ������ֵ
	{
		if (a*b<0)//б��һ��һ��,������ȡ����
		{
			if (a<0)
			{
				a = 180 + a;
			}
			else
			{
				b = 180 + b;
			}
			c = (a + b) / 2;//ֱ�����/2
		}
		else if ((a >= 0) && (b >= 0))//б�ʶ�������
		{
			c = (a + b) / 2;//ֱ�����/2
		}
		else//�������Ǹ��������
		{
			c = (a + b) / 2;//ֱ�����/2
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
//ǰ���Ѿ���������и������ߵ�б��
//���룬�и���Ƶ㣬�и������ߵ�б�ʣ���ջ��ĵ�˫����ȡ�Ǹ��и������ߵ����
//��������ȡֵ��˫������ȡֵ
//������ֵ�������
Line_para 	CutLeadLine::Get_CutLine_Point(Point start, double k)
{
	//ֱ�߷��̣�y = k(x-a) + b��
	//Բ���̣�(x - a) ^ 2 + (y - b) ^ 2 = r ^ 2;
	Line_para Lp;
	double x1, x2, y1, y2;//��������
	double a, b, r;
	a = start.x;
	b = start.y;
	r = m_CutLineLength;//�����߳���
	x1 = a + sqrt(r*r / (1 + k*k));
	y1 = b + k*sqrt(r*r / (1 + k*k));
	x2 = a - sqrt(r*r / (1 + k*k));
	y2 = b - k*sqrt(r*r / (1 + k*k));
	Lp.x0 = x1;
	Lp.x1 = x2;
	Lp.y0 = y1;
	Lp.y1 = y2;
	Lp.k = k;//�����и������ߵ�б�ʣ��Ժ��õ���
	return Lp;
}
//���ݿ��ڷ���ͷ�ջ���˫��ȡ����һ��ֵ��Ϊ�и������ߵ����
//������������ֵ�����뿪�ڷ������뵥˫��,�����и���Ƶ�
//����и�������ͼԪ
Line_para CutLeadLine::Get_CutLine_StartPoint(Line_para m_Line, int m_OpenDirect, bool m, Point start)
{
	Line_para m_CutGuideL;
	m_CutGuideL.x1 = start.x;
	m_CutGuideL.y1 = start.y;
	m_CutGuideL.k = m_Line.k;
	//����ǰ����������滮����ʱ���������Ϊ��һ����㣬��ô�����Ҫ����ȡֵ��ż������ȡֵ
	//����ǰ������û�н���滮����ʱ�и����ջ�Ϊ��һ����㣬��ô�����Ҫ����ȡֵ��ż������ȡֵ
	//����������ϣ���ȡyС�ģ��������£���ȡy��ġ�
	//�������������ȡx��ģ��������ң���ȡxС�ġ�
	//!mΪ��Ĳ�����滮��m���ǰ�Ľ���滮
	if (m)//����
	{
		switch (m_OpenDirect)
		{
		case 1://��������
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
		case 2://��������
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
		case 3://��������
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
		case 4://��������
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
	//˫�㣬����ȡ
	//����������ϣ���ȡy��ģ��������£���ȡyС�ġ�
	//�������������ȡxС�ģ��������ң���ȡx��ġ�
	else
	{
		switch (m_OpenDirect)
		{
		case 1://��������
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
		case 2://��������
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
		case 3://��������
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
		case 4://��������
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
//������ֹ��ջ�����ͼԪ���ͷ�ջ���˫���ʣ����и������߻�������
Line_para CutLeadLine::Get_CutLeadLine(Line_para a, Line_para b, bool m)
{
	Point start;
	double k = 0.0;
	Line_para m_Line;
	Line_para m_CutGuideL;
	int m_opendirect;//���ڷ����ж�,1���Ͽ���2���¿���3���󿪣�4���ҿ�
	start.x = a.x0;
	start.y = a.y0;
	a.k = Get_k_twoPoint(a);
	b.k = Get_k_twoPoint(b);
	m_opendirect = Get_OpenDirect(a, b);//�жϼнǿ��ڷ���
	k = Get_k_twoK(a.k, b.k, m_opendirect);//���и�������б��
	m_Line = Get_CutLine_Point(start, k);
	m_CutGuideL = Get_CutLine_StartPoint(m_Line, m_opendirect, m, start);
	return m_CutGuideL;
}
//���Ĵ���
//���Ĵ���
//���Ĵ���
//�и������߸����жϣ����뵱ǰ��Ҫ�ж��и������ߵķ�ջ���Ȼ���ж���������������ڵ��ֵܷ�ջ��Ƿ���档
bool CutLeadLine::JudgeCGLineVSGeomclosedH(GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head)
{
	GeomCloseHEAD*pHtemp;//�����洢��ջ�˫������ͷ���
	GeomCloseHEAD*pHOVtemp;//�����洢��ż���ջ�ͷ���
	GeomEleNode*pCGLine;//�����洢�и������߻���ͼԪ
	bool m_IfCGLIeter = false;//�洢�Ƿ��и������߸����ж�ֵ,��ʼ��Ϊδ����
	bool m_Singlelayer=pCHtemp->m_Singlelayer;//������ż�����ʿ��Լ����б������
	pCGLine = pCHtemp->FirstGeomele;//�и�������
	if (m_Singlelayer)//��ջ������ʱ�����жϷ�ջ����ڵ�һȦ
	{
		pHOVtemp = pCHtemp;//����ջ���Ҫ�жϵ���һȦ��ջ��Ǳ������ڵ�
	}
	else//��ջ���ż��ʱ��
	{
		pHOVtemp = pCHtemp->FirstInsideGCloseNode;//ż���ջ�ʱ����Ҫ�жϵ���һȦ��ջ������ӷ�ջ����ڵ�
	}
	//����ȷ������Ҫѭ���ķ�ջ������Ǳ����㻹���Ӳ�
	//����Ҫȷ��ѭ���Ŀ�ʼ��Ȼ��������ջ�˫������ȫ��ѭ��һ��
	//���������㷨�������Ժ���Կ�����һ����ֵ��Χ�ڵ��ֵܷ�ջ��п��Ǹ��档��
	//���������㷨�������Ժ���Կ�����һ����ֵ��Χ�ڵ��ֵܷ�ջ��п��Ǹ��档��
	//���������㷨�������Ժ���Կ�����һ����ֵ��Χ�ڵ��ֵܷ�ջ��п��Ǹ��档��
	if (!(pHOVtemp->prevGeomcloseNode))//����÷�ջ����׽ڵ�
	{
		pHtemp = pHOVtemp;//ѭ������������ʼ
	}
	//��������׽�㣬��ôҲ������ص��׽��ȥ����
	else
	{
		pHtemp = pHOVtemp;
		while (pHtemp->prevGeomcloseNode)//Ѱ���׽ڵ�
		{
			pHtemp = pHtemp->prevGeomcloseNode;
		}
	}
	//�����Ѿ��ҵ��˷�ջ����ֵܷ�ջ���ͷ���pHtemp
	while (pHtemp && (!m_IfCGLIeter))//��������ջ�˫�����������������Ǹ÷�ջ�˫������ֻ��һ��Ҳ����
	{
		m_IfCGLIeter = JudgeCGLineVSOneClosedHead(pCGLine, pHtemp, m_ceramic_Head);
		if (m_IfCGLIeter)//�����һ���ط������ˣ���ô���˳�
		{
			return true;
			break;
		}
		pHtemp = pHtemp->nextGeomcloseNode;
	}
		return false;	
}

//���Ĵ���
//���Ĵ���
//���Ĵ���
//�и������߸����ж�,�����и�������������һ����ջ����ж����Ƿ���棬��������Ĵ���
bool CutLeadLine::JudgeCGLineVSOneClosedHead(GeomEleNode*pCGLinetemp, GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head)
{
	GeomEleNode*pCGeomtemp;//�洢��ջ��Ļ���ͼԪ������ֻ�ǿ�����ֱ�����͵�)
	bool m_IfCGLIeter = false;//�洢�Ƿ��и������߸����ж�ֵ,��ʼ��Ϊδ����
	//ע�⣺��pCHtemp�ǰ��������ʱ��Ҫ�ӵ�һ��ֱ�߿�ʼֱ�����һ��ֱ��Ϊֹ��
	if (pCHtemp == m_ceramic_Head)
	{
		pCGeomtemp = pCHtemp->FirstGeomele;//ע�� pCHtempΪ���������ʱ��û���и�������
		//Ҫ������������Ĵӵ�һ������ͼԪ�ڵ㵽������һ������ͼԪ�ڵ�����ж��Ƿ��и��棬���Ĵ���
		while ((pCGeomtemp) && (!m_IfCGLIeter))//��������һ������Ϊ���������û���и�������
		{
			m_IfCGLIeter = If_HaveCGLineInterPoint(pCGLinetemp, pCGeomtemp);
			//����и�����ô�Ϳ��Է��������ջ��ˡ��������ֻ�����������ж϶���
			if (m_IfCGLIeter)
			{
				return true;
				break;
			}
			pCGeomtemp = pCGeomtemp->nextGeomeleNode;//ע�� pCHtemp->FirstGeomele���и������ߣ�������Ĳ��Ƿ�ջ������Ļ���ͼԪ
		}
		return false;
	}
	else//�����и��ջ������и�������
	{
		pCGeomtemp = pCHtemp->FirstGeomele->nextGeomeleNode;//ע�� pCHtemp->FirstGeomele���и������ߣ�������Ĳ��Ƿ�ջ������Ļ���ͼԪ
		//Ҫ�������ջ��Ĵӵڶ�������ͼԪ�ڵ㵽�����ڶ�������ͼԪ�ڵ�����ж��Ƿ��и��棬���Ĵ���
		while ((pCGeomtemp->nextGeomeleNode) && (!m_IfCGLIeter))//�������ڶ����ͺ��ˣ���Ϊ������һ�������и�������
		{
			m_IfCGLIeter = If_HaveCGLineInterPoint(pCGLinetemp, pCGeomtemp);
			//����и�����ô�Ϳ��Է��������ջ��ˡ��������ֻ�����������ж϶���
			if (m_IfCGLIeter)
			{
				return true;
				break;
			}
			pCGeomtemp = pCGeomtemp->nextGeomeleNode;//ע�� pCHtemp->FirstGeomele���и������ߣ�������Ĳ��Ƿ�ջ������Ļ���ͼԪ
		}
		return false;
	}
}
//�����ԭ���ǽ������и������ߵ�������֮�䡣
//�����и�����������һ��ֱ�ߣ����佻�㣬Ȼ���жϽ����Ƿ����и������ߵ�������֮��
bool CutLeadLine::If_HaveCGLineInterPoint(GeomEleNode*pCGLinetemp, GeomEleNode*pCGeomtemp)
{
	Point m_CGLineStart, m_CGLineEnd;//�洢�и������ߵ���ֹ��
	Point m_LineStart, m_LineEnd;//�洢Ҫ�ж�ֱ�ߵ���ֹ��
	Point m_InterPoint;//����
	Line_Inter m_CGLine, m_Line;//�洢�и������������Ƹ���ֱ�ߵ�abc����
	Line_Point m_CGlinePoint;//�洢�и���������xy������˳��
	double m_x, m_y;
	//�洢�и������ߵĿ��Ƶ�
	m_CGLineStart.x = pCGLinetemp->m_GeomStandData.GeoEle_start_x0;
	m_CGLineStart.y = pCGLinetemp->m_GeomStandData.GeoEle_start_y0;
	m_CGLineEnd.x = pCGLinetemp->m_GeomStandData.GeoEle_start_x1;
	m_CGLineEnd.y = pCGLinetemp->m_GeomStandData.GeoEle_start_y1;
	//�洢���ж��߶εĿ��Ƶ�
	m_LineStart.x = pCGeomtemp->m_GeomStandData.GeoEle_start_x0;
	m_LineStart.y = pCGeomtemp->m_GeomStandData.GeoEle_start_y0;
	m_LineEnd.x = pCGeomtemp->m_GeomStandData.GeoEle_start_x1;
	m_LineEnd.y = pCGeomtemp->m_GeomStandData.GeoEle_start_y1;
	//������ֱ�ߵĽ���
	//����ֱ�ߵı�׼����abc
	m_CGLine = GetLine(m_CGLineStart, m_CGLineEnd);
	m_Line = GetLine(m_LineStart, m_LineEnd);
	//�󽻵�
	m_InterPoint = GetCrossPoint(m_CGLine, m_Line);
	//�жϽ����ǲ������и������ߵķ�Χ�ڡ�
	//�Ƚ��и������ߵ������˵�ֵ����С����Χ
	m_CGlinePoint = GetPointOrder(m_CGLineStart, m_CGLineEnd);
	if ((m_CGlinePoint.x_min <= m_InterPoint.x) && (m_InterPoint.x <= m_CGlinePoint.x_max) && (m_CGlinePoint.y_min <= m_InterPoint.y) && (m_InterPoint.y <= m_CGlinePoint.y_max))
	{
		//�жϽ������и����յ��Ƿ��غ�
		if ((fabs(m_InterPoint.x - m_CGLineEnd.x) < EPSILON) && (fabs(m_InterPoint.y - m_CGLineEnd.y) < EPSILON))//˵�������������ͬ����ô���Ǹ��棬����ͬ����ջ����и����������ջ������ӵ�
		{
			return  false;
		}
		else
		{
			return  true;
		}
	}
	else
	{
		return  false;
	}
}
//����ֱ�ߵ������˵�ֵ��Ȼ�����׼ֱ�ߵ�abc��������
Line_Inter CutLeadLine::GetLine(Point m_StratPoint, Point m_EndPoint)
{
	Line_Inter m_Line;
	m_Line.a = m_StratPoint.y - m_EndPoint.y;
	m_Line.b = m_EndPoint.x - m_StratPoint.x;
	m_Line.c = m_StratPoint.x*m_EndPoint.y - m_EndPoint.x*m_StratPoint.y;
	return m_Line;
}
//�����и��������Լ���һ�������и�����߶ε�abc�������󽻵�
Point CutLeadLine::GetCrossPoint(Line_Inter l1, Line_Inter l2)
{
	Point pTemp;
	double D;//���D=0˵������ֱ����ƽ�еġ�Ŀǰû������һ�鹤��
	D = l1.a*l2.b - l2.a*l1.b;
	Point p;
	pTemp.x = (l1.b*l2.c - l2.b*l1.c) / D;
	pTemp.y = (l1.c*l2.a - l2.c*l1.a) / D;
	return pTemp;//�����˽���
}
//���и������������˵��ĸ���֮��ķ�Χ
Line_Point  CutLeadLine::GetPointOrder(Point m_StratPoint, Point m_EndPoint)
{
	Line_Point m_point;
	//��x�ķ�Χ
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
	//��y�ķ�Χ
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

//�����и��и��������и���ķ�ջ���Ȼ��������и�������
void CutLeadLine::ChangeCGLine(GeomCloseHEAD*pCHtemp)
{
	GeomEleNode*cut_in_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//�и�������
	GeomEleNode*cut_out_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//�и�������
	//��ԭ�����и�������������ԭ���и������ߵ�һ�������������и�������
	GeomCloseHEAD*ptemp;//���÷�ջ��ڵ�
	GeomEleNode*Fnode, *Enode;
	GeomEleNode*node;
	Line_para m_startline, m_endline;//��ջ���ͼԪ�ڵ��βͼԪ�ڵ�Ļ�������
	Line_para m_cutline;//�и������ߵĺ��Ĳ���
	double x0_min, y0_min, x0_max, y0_max;
	double x1_min, y1_min, x1_max, y1_max;
	bool m_Singlelayer;
	m_Singlelayer = pCHtemp->m_Singlelayer;
	//ֻ����ֱ�����͵�ͼԪ,���������Ƿ��н��棬������Բ
	Fnode = pCHtemp->FirstGeomele;//�и�������
	Enode = Fnode->nextGeomeleNode;
	while (Enode->nextGeomeleNode->nextGeomeleNode)//����ԭ���и������ߵ�����һ��ĸ�ߣ�ע�ⲻ�����һ������ͼԪ�ǵ����ڶ�������ʱEnode����ɷ�ջ�ԭ�����һ��ͼԪ��
	{
		Enode = Enode->nextGeomeleNode;
	}
	//�����õ��ǽ�ƽ���߷�
	//��ͼԪ�ڵ�
	/*m_startline.x0 = Fnode->m_GeomStandData.GeoEle_start_x0;
	m_startline.y0 = Fnode->m_GeomStandData.GeoEle_start_y0;
	m_startline.x1 = Fnode->m_GeomStandData.GeoEle_start_x1;
	m_startline.y1 = Fnode->m_GeomStandData.GeoEle_start_y1;*/
	m_startline.x0 = Fnode->m_GeomStandData.GeoEle_start_x1;
	m_startline.y0 = Fnode->m_GeomStandData.GeoEle_start_y1;
	m_startline.x1 = Fnode->m_GeomStandData.GeoEle_start_x0;
	m_startline.y1 = Fnode->m_GeomStandData.GeoEle_start_y0;
	//βͼԪ�ڵ㣬Ҫע����β�Ե�
	m_endline.x0 = Enode->m_GeomStandData.GeoEle_start_x1;
	m_endline.y0 = Enode->m_GeomStandData.GeoEle_start_y1;
	m_endline.x1 = Enode->m_GeomStandData.GeoEle_start_x0;
	m_endline.y1 = Enode->m_GeomStandData.GeoEle_start_y0;
	//�����и������ߺ���
	m_cutline =Get_CutLeadLine(m_startline, m_endline, m_Singlelayer);
	//���и�������Ҳһ������Ϊͬһ��ͼԪ��ʽ
	cut_in_Node->m_GeomStandData.GeoEle_start_x0 = m_cutline.x0;
	cut_in_Node->m_GeomStandData.GeoEle_start_y0 = m_cutline.y0;
	cut_in_Node->m_GeomStandData.GeoEle_start_x1 = m_cutline.x1;
	cut_in_Node->m_GeomStandData.GeoEle_start_y1 = m_cutline.y1;
	cut_in_Node->m_GeomStandData.k = m_cutline.k;
	cut_in_Node->m_GeomStandData.m_typegeomele = 6;//�и������ߵı�׼
	//����ֱ���͵��и������߶��ԣ�cut_in��cut_out��������һ���ģ�������λ��Ҫ����
	cut_out_Node->m_GeomStandData.GeoEle_start_x0 = m_cutline.x1;
	cut_out_Node->m_GeomStandData.GeoEle_start_y0 = m_cutline.y1;
	cut_out_Node->m_GeomStandData.GeoEle_start_x1 = m_cutline.x0;
	cut_out_Node->m_GeomStandData.GeoEle_start_y1 = m_cutline.y0;
	cut_out_Node->m_GeomStandData.k = m_cutline.k;
	cut_out_Node->m_GeomStandData.m_typegeomele = 6;//�и������ߵı�׼
	//�ص㣡����
	//��Ϊ���ڵķ�ջ��Ǵ������������г��ߵģ�����Ҫ�Ƚ�ԭ�����������г���ȥ����
	//Ȼ�����ڵ��������г��߹ҵ���ջ���ԭ���ڶ����͵����ڶ�������ͼԪ��
	pCHtemp->FirstGeomele = cut_in_Node;//�и�������Ϊ��ͼԪ
	cut_in_Node->nextGeomeleNode = Fnode->nextGeomeleNode;//ָ��ԭ���ĵڶ���ͼԪ
	cut_in_Node->prevGeomeleNode = NULL;//��Ȼ���ͳ��˵�һ����
	Fnode->nextGeomeleNode->prevGeomeleNode = cut_in_Node;//ԭ��ǰ��ָ�����NULL
	//�������ϵ���Fnode�Ѿ�û�����ü�ֵ��,Ҫ������ڴ��ͷŵ�
	free(Fnode);
	//����cut_out
	node = Enode->nextGeomeleNode;//ԭ���и������ߵ�������
	free(node);//�ͷŵ�ԭ���и���������һ���ڴ�
	Enode->nextGeomeleNode = cut_out_Node;//ԭ����ջ������һ��ͼԪָ�����node������ָ��cut_out
	cut_out_Node->prevGeomeleNode = Enode;
	cut_out_Node->nextGeomeleNode = NULL;
	//��˷�ջ��ͼ������µ��и�������
}



