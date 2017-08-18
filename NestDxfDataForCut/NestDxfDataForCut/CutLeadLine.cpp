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
	//��Ԥ������x0����Ϊ��x1С������
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
	Lp.k = k;//�����и������ߵ�б�ʣ��Ժ��õ���
	return Lp;
}
//��ֱ�����͵ļ����и�������
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
//��Բ���͵��и��������õģ������м�Ŀ��ڷ������m_OpenDirect,����Բ������Circle_Center
Line_para CutLeadLine::Get_CutLine_StartPoint(Line_para m_Line, Point Circle_Center, bool m, Point start)
{
	Line_para m_CutGuideLine;
	Point p1, p2;
	double d1,d2;//������֮��ľ���
	m_CutGuideLine.x1 = start.x;//start����Բԭ�����и���Ƶ�
	m_CutGuideLine.y1 = start.y;
	p1.x = m_Line.x0;
	p1.y = m_Line.y0;
	p2.x = m_Line.x1;
	p2.y = m_Line.y1;
	//��m_Line�����㵽Բ�ĵľ���
	d1 = sqrt((p1.x - Circle_Center.x)*(p1.x - Circle_Center.x) + (p1.y - Circle_Center.y)*(p1.y - Circle_Center.y));
	d2 = sqrt((p2.x - Circle_Center.x)*(p2.x - Circle_Center.x) + (p2.y - Circle_Center.y)*(p2.y - Circle_Center.y));

	//����Բ��˵��������ż��֮�󣬾�ֻʣ��ѡԲ�ڵĵ㻹��Բ��ĵ�������ˡ�
	//������ȥ���˰���������������ǵ�������ȡ��˫������ȡ
	//�����漰��m_Line�е������㣬�����ѡ��
	//Ӧ����Ҫ��Բ��Բ��Ҳ���������˫������ѡ��ѡ��Բ�Ľ��ģ���������ѡ��ѡ��Բ��Զ��
	if (m)//����
	{
		//����ѡ,��Ӧ��ѡ��Զ��Բ�ĵ�
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
	else//˫��
	{
		//����ѡ����ѡ����Բ�ĵ�
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

//ֱ�����͵��и������ߵ�������ʽ
//������ֹ��ջ�����ͼԪ���ͷ�ջ���˫���ʣ����и������߻�������
Line_para CutLeadLine::Get_CutLeadLine(Line_para a, Line_para b, bool m, int m_TypeCGLine)
{
	Point start;
	double k = 0.0;
	Line_para m_Line;
	Line_para m_CutGuideL;
	int m_opendirect;//���ڷ����ж�,1���Ͽ���2���¿���3���󿪣�4���ҿ�
	start.x = a.x0;//��ȡ�и���Ƶ�
	start.y = a.y0;
	a.k = Get_k_twoPoint(a);
	b.k = Get_k_twoPoint(b);
	m_opendirect = Get_OpenDirect(a, b);//�жϼнǿ��ڷ���
	k = Get_k_twoK(a.k, b.k, m_opendirect);//���и�������б��
	m_Line = Get_CutLine_Point(start, k);
	if (1==m_TypeCGLine)//���շ�ջ���ż����Ϣ�����и�������
	{
		m_CutGuideL = Get_CutLine_StartPoint(m_Line, m_opendirect, m, start);
	}
	else//��ʱm_TypeCGLine=2;Ҫ���յ����и������ߵķ�ʽ����
	{
		m_CutGuideL = Get_ChangeCutLine_StartPoint(m_Line, m_opendirect, start);
	}
	return m_CutGuideL;
}
//Բ���͵��и�������
//����Բ���͵��и������������ķ�ʽ���������߷�������������һ����ջ����и���Ƶ㣬�͵�ǰ��ջ����и���Ƶ㣬����m_SinglelayerΪ��ջ�����ż����Ϣ��m_TypeCGLineΪ�����и������ߵķ�ʽ��1Ϊ����ż�����ɣ�2Ϊ��������ʽ����
//���в���Last_GCloseΪ��һ����ջ����и���Ƶ㣬Current_GCloseΪ��ǰԲ���и���Ƶ㣬
Line_para  CutLeadLine::Get_CutLeadLine(Point Last_GClose, Point Current_GClose, Point Circle_Center, bool m_Singlelayer, int m_TypeCGLine)
{
	Point start;
	double k = 0.0;
	Line_para m_MLine;//�и������ߵ�ĸ��
	Line_para m_Line;//�и�������
	Line_para m_CutGuideLine;
	start.x = Current_GClose.x;
	start.y = Current_GClose.y;
	m_MLine.x0 = Current_GClose.x;
	m_MLine.y0 = Current_GClose.y;
	m_MLine.x1 = Last_GClose.x;
	m_MLine.y1 = Last_GClose.y;
	k = Get_k_twoPoint(m_MLine);
	m_Line = Get_CutLine_Point(start, k);
	if (1 == m_TypeCGLine)//���շ�ջ���ż����Ϣ�����и�������
	{
		m_CutGuideLine = Get_CutLine_StartPoint(m_Line, Circle_Center, m_Singlelayer, start);
	}
	else//��ʱm_TypeCGLine=2;Ҫ���յ����и������ߵķ�ʽ����
	{
		//m_CutGuideLine = Get_ChangeCutLine_StartPoint(m_Line, Circle_Center, m_Singlelayer, start);
	}
	return m_CutGuideLine;


	
}

//���Ĵ���
//���Ĵ���
//���Ĵ���
//�и������߸����жϣ����뵱ǰ��Ҫ�ж��и������ߵķ�ջ���Ȼ���ж���������������ڵ��ֵܷ�ջ��Ƿ���档
bool CutLeadLine::JudgeCGLineVsGeomclosedH(GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head)
{
	GeomCloseHEAD*pHtemp;//�����洢��ջ�˫������ͷ���
	GeomCloseHEAD*pHOVtemp;//�����洢��ż���ջ�ͷ���
	GeomEleNode*pCGLine;//�����洢�и������߻���ͼԪ
	//GeomEleNode*Add_more_Node = (GeomEleNode*)malloc(sizeof(GeomEleNode));//�и�������
	bool m_MaybeCGLInter = false;//Ԥ���ж����ǲ��ǿ��ܸ���
	bool m_IfCGLIeter = false;//�洢�Ƿ��и������߸����ж�ֵ,��ʼ��Ϊδ����
	bool m_Singlelayer=pCHtemp->m_Singlelayer;//������ż�����ʿ��Լ����б������
	//��ȡ��Ҫ�жϵķ�ջ���Բ���и���������ֱ�����и�������
	//��Բ����ֱ�����и������߹�Ϊһ��
	pCGLine = GetLimtOfAuxiliaryLine(pCHtemp);

	if (m_Singlelayer)//��ջ������ʱ�����жϷ�ջ����ڵ�һȦ
	{
		pHOVtemp = pCHtemp;//����ջ���Ҫ�жϵ���һȦ��ջ��Ǳ������ڵ�
	}
	else//��ջ���ż��ʱ��
	{
		pHOVtemp = pCHtemp->FirstInsideGCloseNode;//ż���ջ�ʱ����Ҫ�жϵ���һȦ��ջ������ӷ�ջ����ڵ�
	}
	//����ȷ������Ҫѭ���ķ�ջ������Ǳ���㻹���Ӳ�
	//����Ҫȷ��ѭ���Ŀ�ʼ��Ȼ��������ջ�˫������ȫ��ѭ��һ��
	//���������㷨�������Ժ���Կ�����һ����ֵ��Χ�ڵ��ֵܷ�ջ��п��Ǹ��档��
	//���������㷨�������Ժ���Կ�����һ����ֵ��Χ�ڵ��ֵܷ�ջ��п��Ǹ��档��
	//���������㷨�������Ժ���Կ�����һ����ֵ��Χ�ڵ��ֵܷ�ջ��п��Ǹ��档��
	if (!(pHOVtemp->prevGeomcloseNode))//����÷�ջ����׽ڵ�
	{
		pHtemp = pHOVtemp;//ѭ����������ʼ
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
	while (pHtemp && (!m_IfCGLIeter))//��������ջ�˫����������������Ǹ÷�ջ�˫������ֻ��һ��Ҳ����
	{
		//��Ԥ���ж����и������ߵİ���������ջ��İ�������Ƿ��ཻ���ཻ���п��ܻ��и���
		m_MaybeCGLInter = If_HaveCGLineInterPoint_UseRect(pCGLine, pHtemp);
		if (m_MaybeCGLInter)
		{
			//�ཻ�����һ���ж��Ƿ����
			m_IfCGLIeter = JudgeCGLineVsOneClosedHead(pCGLine, pHtemp, m_ceramic_Head);
			if (m_IfCGLIeter)//�����һ���ط������ˣ���ô���˳�
			{
				return true;
				break;
			}
		}
		
		pHtemp = pHtemp->nextGeomcloseNode;
	}
		return false;	
}

//���Ĵ���
//���Ĵ���
//���Ĵ���
//�и������߸����ж�,�����и�������������һ����ջ����ж����Ƿ���棬��������Ĵ���
bool CutLeadLine::JudgeCGLineVsOneClosedHead(GeomEleNode*pCGLine, GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head)
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
			m_IfCGLIeter = If_HaveCGLineInterPoint(pCGLine, pCGeomtemp);
			//����и�����ô�Ϳ��Է��������ջ��ˡ��������ֻ�����������ж϶���
			if (m_IfCGLIeter)
			{
				return true;
				break;
			}
			pCGeomtemp = pCGeomtemp->nextGeomeleNode;//ע�� pCHtemp->FirstGeomele���и������ߣ�������Ĳ��Ƿ�ջ�����Ļ���ͼԪ
		}
		return false;
	}
	else//�����и��ջ������и�������
	{
		pCGeomtemp = pCHtemp->FirstGeomele->nextGeomeleNode;//ע�� pCHtemp->FirstGeomele���и������ߣ�������Ĳ��Ƿ�ջ�����Ļ���ͼԪ
		//Ҫ�������ջ��Ĵӵڶ�������ͼԪ�ڵ㵽�����ڶ�������ͼԪ�ڵ�����ж��Ƿ��и��棬���Ĵ���
		while ((pCGeomtemp->nextGeomeleNode) && (!m_IfCGLIeter))//�������ڶ����ͺ��ˣ���Ϊ������һ�������и�������
		{
			m_IfCGLIeter = If_HaveCGLineInterPoint(pCGLine, pCGeomtemp);
			//����и�����ô�Ϳ��Է��������ջ��ˡ��������ֻ�����������ж϶���
			if (m_IfCGLIeter)
			{
				return true;
				break;
			}
			pCGeomtemp = pCGeomtemp->nextGeomeleNode;//ע�� pCHtemp->FirstGeomele���и������ߣ�������Ĳ��Ƿ�ջ�����Ļ���ͼԪ
		}
		return false;
	}
}
//�����ԭ���ǽ������и������ߵ�������֮�䡣
//�����и�����������һ����Ԫ�����佻�㣬Ȼ���жϽ����Ƿ����и������ߵ�������֮��
//ע���и��������п�����Բ����ֱ�ߣ�����һ����Ԫ����Ҳ��Բ����ֱ�ߣ�����Բ��
//��ײ���룬���Ĳ㣻
bool CutLeadLine::If_HaveCGLineInterPoint(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	
	bool m_IfInter=false;//�ж��Ƿ����,Ĭ�ϲ�����
	
	//���и���������ֱ�ߣ��һ�ԪҲ��ֱ��ʱ��
	//������ֱ�ߵĽ���
	//����ֱ�ߵı�׼����abc
	//�ص����ڽ���������Ľ��㶼���ǽ�ȥ��
	//�и���������ֱ�ߣ����жϵĻ���ͼԪҲ��ֱ��
	if ((61 == pCGLine->m_GeomStandData.m_typegeomele) && (1 == pCGeomtemp->m_GeomStandData.m_typegeomele))
	{
		m_IfInter = CheckCGLineVsLine(pCGLine, pCGeomtemp);
	}
	//�и���������ֱ�ߣ����жϵĻ���ͼԪ��Բ
	else if ((61 == pCGLine->m_GeomStandData.m_typegeomele) && (3 == pCGeomtemp->m_GeomStandData.m_typegeomele))
	{
		m_IfInter = CheckCGLineVsCircle(pCGLine, pCGeomtemp);
	}
	//�и���������Բ�������жϵĻ���ͼԪ��Բ
	else if ((62 == pCGLine->m_GeomStandData.m_typegeomele) && (3 == pCGeomtemp->m_GeomStandData.m_typegeomele))
	{
		m_IfInter = CheckCGArcVsCircle(pCGLine, pCGeomtemp);
	}
	//�и���������ֱ�ߣ����жϵĻ���ͼԪ��Բ���������и���������Բ�������жϵĻ���ͼԪ��ֱ��
	else if (((61 == pCGLine->m_GeomStandData.m_typegeomele) && (2 == pCGeomtemp->m_GeomStandData.m_typegeomele)) || ((62 == pCGLine->m_GeomStandData.m_typegeomele) && (1 == pCGeomtemp->m_GeomStandData.m_typegeomele)))
	{
		m_IfInter = CheckCGLineVsCGArc(pCGLine, pCGeomtemp);
	}
	return m_IfInter;
}
//ֱ�����и���������ֱ���ͻ���ͼԪ�ĸ����ж�
bool CutLeadLine::CheckCGLineVsLine(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	Point m_CGLineStart, m_CGLineEnd;//�洢�и������ߵ���ֹ��
	Point m_LineStart, m_LineEnd;//�洢Ҫ�ж�ֱ�ߵ���ֹ��
	Point m_InterPoint;//����
	Line_Inter m_CGLine, m_Line;//�洢�и������������Ƹ���ֱ�ߵ�abc����
	Line_Point m_CGlinePoint;//�洢�и���������xy������˳��
	Line_Point m_linePoint;//�洢��������xy������˳��
	double m_x, m_y;
	//�洢�и������ߵĿ��Ƶ�
	m_CGLineStart.x = pCGLine->m_GeomStandData.GeoEle_start_x0;
	m_CGLineStart.y = pCGLine->m_GeomStandData.GeoEle_start_y0;
	m_CGLineEnd.x = pCGLine->m_GeomStandData.GeoEle_start_x1;
	m_CGLineEnd.y = pCGLine->m_GeomStandData.GeoEle_start_y1;
	//�洢���ж��߶εĿ��Ƶ�
	m_LineStart.x = pCGeomtemp->m_GeomStandData.GeoEle_start_x0;
	m_LineStart.y = pCGeomtemp->m_GeomStandData.GeoEle_start_y0;
	m_LineEnd.x = pCGeomtemp->m_GeomStandData.GeoEle_start_x1;
	m_LineEnd.y = pCGeomtemp->m_GeomStandData.GeoEle_start_y1;
	m_CGLine = GetLine(m_CGLineStart, m_CGLineEnd);
	m_Line = GetLine(m_LineStart, m_LineEnd);
	//����ֱ�ߵĽ���
	m_InterPoint = GetCrossPoint(m_CGLine, m_Line);
	//�жϽ����ǲ������и������ߵķ�Χ�ڡ�
	//�Ƚ��и������ߵ������˵�ֵ����С����Χ
	m_CGlinePoint = GetPointOrder(m_CGLineStart, m_CGLineEnd);
	//�ٽ���֮������ߵ������˵�ֵ����С����Χ
	m_linePoint = GetPointOrder(m_LineStart, m_LineEnd);
	//���ų����㲻�ڸ����߷�Χ�ڵĵ�
	//��������ڸ������ڣ�˵����ʵ���ཻ
	if ((m_linePoint.x_min <= m_InterPoint.x) && (m_InterPoint.x <= m_linePoint.x_max) && (m_linePoint.y_min <= m_InterPoint.y) && (m_InterPoint.y <= m_linePoint.y_max))
	{
		//���жϽ����ڲ����и��������ڣ������˵����ʵ���ཻ
		if ((m_CGlinePoint.x_min <= m_InterPoint.x) && (m_InterPoint.x <= m_CGlinePoint.x_max) && (m_CGlinePoint.y_min <= m_InterPoint.y) && (m_InterPoint.y <= m_CGlinePoint.y_max))
		{
			m_x = fabs(m_InterPoint.x - m_CGLineEnd.x);
			m_y = fabs(m_InterPoint.y - m_CGLineEnd.y);
			//�жϽ������и����յ��Ƿ��غ�
			if ((m_x< EPSILON) && (m_y< EPSILON))//˵�������������ͬ����ô���Ǹ��棬����ͬ����ջ����и����������ջ������ӵ�
			{
				return  false;
			}
			//�������ʵ�ʵĸ���
			else
			{
				return  true;
			}
		}
		//����ֻ���и������ߵ���������֮�ཻ
		else
		{
			return  false;
		}
	}
	//˵���Ǽ��ཻ
	else
	{
		return  false;
	}
}
//ֱ�����и���������Բ����ͼԪ�ĸ����ж�
bool CutLeadLine::CheckCGLineVsCircle(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	Point m_CGLineStart, m_CGLineEnd;//�洢�и������ߵ���ֹ��
	Point m_Circent;//Բ�ĵ�
	Point m_InterP1, m_InterP2;//�߶���Բ����������
	Line_Inter m_CGLine;//�洢�и������������Ƹ���ֱ�ߵ�abc����
	Line_Point m_CGlinePoint;//�洢�и���������xy������˳��
	double m_x, m_y;
	double m_r, m_Circent_x, m_Circent_y;//����Բ�İ뾶��Բ������
	double m_d;//Բ�ĵ��и������ߵľ���
	//�洢�и������ߵĿ��Ƶ�
	m_CGLineStart.x = pCGLine->m_GeomStandData.GeoEle_start_x0;
	m_CGLineStart.y = pCGLine->m_GeomStandData.GeoEle_start_y0;
	m_CGLineEnd.x = pCGLine->m_GeomStandData.GeoEle_start_x1;
	m_CGLineEnd.y = pCGLine->m_GeomStandData.GeoEle_start_y1;
	//�洢���ж�Բ�Ŀ��Ƶ�
	m_r = pCGeomtemp->m_GeomStandData.m_circle.m_Circle_r;
	m_Circent.x = pCGeomtemp->m_GeomStandData.m_circle.m_Circent_x;
	m_Circent.y = pCGeomtemp->m_GeomStandData.m_circle.m_Circent_y;
	//���ж�Բ�ĵ��и������ߵľ��룬������ڰ뾶����ô�Ͳ��������ཻ�ġ�
	//m_d = CalcuDistance(m_Circent, m_CGLineStart, m_CGLineEnd);
	//������벻�����뾶����ô�����ཻ�ģ�������һ������
	//	if (m_r <= m_d)
	//{
	//}
	//�����뾶����ô�Ͳ��������ཻ��
	//else
	//{
	//	return false;
	//}
	//��ֱ����Բ�Ľ���
	//LineInterCircleֱ�ӿ��Է����Ƿ��ཻ���Լ����㡣Ҫ���ݽ������ж��Ƿ�����ཻ���������ཻ�����ơ�
	if (!LineInterCircle(m_CGLineStart, m_CGLineEnd, m_Circent, m_r, m_InterP1, m_InterP2))
	{
		return false;//���ཻ
	}
	//�ཻ�����ݽ�������ֵ��һ���ж��Ƿ�����ཻ��
	else
	{
		//����ֱ�����и������ߵķ�Χ
		m_CGlinePoint = GetPointOrder(m_CGLineStart, m_CGLineEnd);
		//�жϽ����ڲ��������߷�Χ��
		if((((m_CGlinePoint.x_min <= m_InterP1.x) && (m_InterP1.x <= m_CGlinePoint.x_max)) && ((m_CGlinePoint.y_min <= m_InterP1.y) && (m_InterP1.y <= m_CGlinePoint.y_max)))//m_InterP1������������õ�
			|| (((m_CGlinePoint.x_min <= m_InterP2.x) && (m_InterP2.x <= m_CGlinePoint.x_max)) && ((m_CGlinePoint.y_min <= m_InterP2.y) && (m_InterP2.y <= m_CGlinePoint.y_max))))//����m_InterP2������������õ�
		{
			return true;//ʵ���ཻ
		}
		else//ֻ�����ཻ�����ƣ�����û���ཻ
		{
			return false;//���ཻ

		}
		
	}
}
//Բ���и���������Բ����ͼԪ�ĸ����ж�
bool CutLeadLine::CheckCGArcVsCircle(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	GeomEleNode*pCGLineForArc = (GeomEleNode*)malloc(sizeof(GeomEleNode));//��Բ���и������ߵ������˵����߶�������Բ��
	Point m_Circle, m_Arc;//�洢Բ��Բ�����꣬��Բ���и������ߵ�Բ������
	double m_d;//�洢Բ�ľ�
	bool m_IfInter = true;//�洢��ֱ��pCGLineForArc�Ƿ���Բ�ཻ,Ĭ�����ཻ
	//ֻ��Ҫ�����⼸�����Ƶ�ͺ��ˣ�������������Ҫ
	pCGLineForArc->m_GeomStandData.GeoEle_start_x0 = pCGLine->m_GeomStandData.GeoEle_start_x0;
	pCGLineForArc->m_GeomStandData.GeoEle_start_y0 = pCGLine->m_GeomStandData.GeoEle_start_y0;
	pCGLineForArc->m_GeomStandData.GeoEle_start_x1 = pCGLine->m_GeomStandData.GeoEle_start_x1;
	pCGLineForArc->m_GeomStandData.GeoEle_start_y1 = pCGLine->m_GeomStandData.GeoEle_start_y1;
	//����Բ���������˵�����߶���Բ�����жϣ�������߶���Բ�н��㣬ʵ�ʵĽ��㣬��ô�Ϳ���˵��Բ����Բ����
	m_IfInter = CheckCGLineVsCircle(pCGLineForArc, pCGeomtemp);
	//���û��ֱ�߶���Բû�и��������£���һ���жϣ�Բ���ǲ�����Բ�ڲ����жϵı�׼��Բ��Բ����Բ��С�ڰ뾶
	if (m_IfInter)//����ཻ���Ϳ϶����ཻ��
	{
		return true;
	}
	else//������ཻ����Ҫ�ж��ǲ���Բ����Բ�ڲ�������ֱ����Բ�ڲ������
	{
		//�жϵķ�������Բ�ľ�
		m_Circle.x = pCGeomtemp->m_GeomStandData.m_circle.m_Circent_x;
		m_Circle.y = pCGeomtemp->m_GeomStandData.m_circle.m_Circent_y;
		m_Arc.x = pCGLine->m_GeomStandData.m_arc.Arccent_x;
		m_Arc.y = pCGLine->m_GeomStandData.m_arc.Arccent_y;
		//��Բ�ľ�
		m_d = fabs(sqrt(((m_Circle.x - m_Arc.x)*(m_Circle.x - m_Arc.x)) + ((m_Circle.y - m_Arc.y)*(m_Circle.y - m_Arc.y))));//�������������֮�����̾���
		//Բ�ľ�С��Բ�İ뾶��˵��Բ���������ڲ�
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
//Բ����ֱ�ߵĸ����ж�
//�˺��������ж�ֱ���и���������Բ������ͼԪ�ĸ����ж�
//Ҳ�����ж�Բ���и���������ֱ�߻���ͼԪ�ĸ����ж�
bool CutLeadLine::CheckCGLineVsCGArc(GeomEleNode*pCGLine, GeomEleNode*pCGeomtemp)
{
	//ע��pCGLinetemp������ֱ�ߣ���ʱpCGeomtempΪԲ������Ҳ������Բ������ʱpCGeomtempΪֱ�ߣ�,
	//�������Ȱ���ֱ����Բ�ĸ����ж����󽻵�
	GeomEleNode*pCGLinetemp;//�洢ֱ�ߣ����������и�������ֱ�߻��ǻ���ͼԪֱ��
	GeomEleNode*pCGArc;//�洢Բ�������������и�������Բ�����ǻ���ͼԪԲ��
	GCIRCLE pCGCricle;//�洢Բ����Բ�������ݴ���Բ��Ȼ�����Բ����Ƕȵ�����󽻵�Ƕ�
	Point m_Circent;//�洢Բ����Բ��
	Point m_InterP1, m_InterP2;//�߶���Բ����������
	Point m_CGLineStart, m_CGLineEnd;//�洢ֱ�ߵ���ֹ��
	Line_Point m_CGlinePoint;//�洢�и���������xy������˳��
	double m_r;//����Բ�İ뾶��Բ������
	double m_InterPAngle1, m_InterPAngle2;//������Բ���и������ߵĽǶȷ�Χ��
	double m_ArcAngleStart, m_ArcAngleEnd;//�洢Բ���и������ߵ���ֹ��
	double m_MidAngle;//�洢Բ���и������ߵ��м�Ƕȣ������жϰ�Բ�и�����������һ��
	bool m_IfClockRota=false;//�ж�Բ���и��������Ƿ�˳ʱ����ת��Ĭ������ʱ��
	if (61 == pCGLine->m_GeomStandData.m_typegeomele)//��ֱ��
	{
		pCGLinetemp = pCGLine;
		pCGArc = pCGeomtemp;
	}
	else//��ʱpCGLinetempΪԲ��
	{
		pCGLinetemp = pCGeomtemp;
		pCGArc = pCGLine;
	}
	//�����Ѿ���ֱ����Բ�����ֳ�����
	//��ֱ����Բ�ĸ����ж��㷨�ж��Ƿ��и��档
	//�洢�и������ߵĿ��Ƶ�
	m_CGLineStart.x = pCGLinetemp->m_GeomStandData.GeoEle_start_x0;
	m_CGLineStart.y = pCGLinetemp->m_GeomStandData.GeoEle_start_y0;
	m_CGLineEnd.x = pCGLinetemp->m_GeomStandData.GeoEle_start_x1;
	m_CGLineEnd.y = pCGLinetemp->m_GeomStandData.GeoEle_start_y1;
	//�洢���ж�Բ�Ŀ��Ƶ�
	//Բ������ת��Բ
	pCGCricle.m_Circle_r = m_r = pCGArc->m_GeomStandData.m_arc.m_Arc_r;
	pCGCricle.m_Circent_x = m_Circent.x = pCGArc->m_GeomStandData.m_arc.Arccent_x;
	pCGCricle.m_Circent_y = m_Circent.y = pCGArc->m_GeomStandData.m_arc.Arccent_y;
	//�洢�и������ߵ���ֹ��
	m_ArcAngleStart = pCGArc->m_GeomStandData.m_arc.m_ArcAngle_start;
	m_ArcAngleEnd = pCGArc->m_GeomStandData.m_arc.m_ArcAngle_end;
	m_MidAngle = pCGArc->m_GeomStandData.k;
	//��ֱ����Բ�Ľ���
	//LineInterCircleֱ�ӿ��Է����Ƿ��ཻ���Լ����㡣Ҫ���ݽ������ж��Ƿ�����ཻ���������ཻ�����ơ�
	if (!LineInterCircle(m_CGLineStart, m_CGLineEnd, m_Circent, m_r, m_InterP1, m_InterP2))
	{
		return false;//���ཻ,��ô�϶���ֱ����Բ��Ҳ���ཻ��
	}
	//�ཻ�����ݽ�������ֵ��һ���ж��Ƿ�����ཻ��
	//�����ֱ���и���������ô���жϽ����ǲ������߶εķ�Χ��
	//�����Բ�������ߣ���ô��Ҫ�жϸý������������ֹ�Ƕ��ڲ���ԭ������ֹ��Χ��
	else
	{
		//Ҫ�����ĸ������и�������
		//���ֱ�����и�������
		if (61 == pCGLine->m_GeomStandData.m_typegeomele)
		{
			//����ֱ�����и������ߵķ�Χ
			m_CGlinePoint = GetPointOrder(m_CGLineStart, m_CGLineEnd);
			//�жϽ����ڲ��������߷�Χ��
			if ((((m_CGlinePoint.x_min <= m_InterP1.x) && (m_InterP1.x <= m_CGlinePoint.x_max)) && ((m_CGlinePoint.y_min <= m_InterP1.y) && (m_InterP1.y <= m_CGlinePoint.y_max)))//m_InterP1������������õ�
				|| (((m_CGlinePoint.x_min <= m_InterP2.x) && (m_InterP2.x <= m_CGlinePoint.x_max)) && ((m_CGlinePoint.y_min <= m_InterP2.y) && (m_InterP2.y <= m_CGlinePoint.y_max))))//����m_InterP2������������õ�
			{
				return true;//ʵ���ཻ
			}
			else//ֻ�����ཻ�����ƣ�����û���ཻ
			{
				return false;//���ཻ

			}
		}
		//�����Բ���и���������ô��Ҫ�жϸý�����Բ���ϵĽǶ��ǲ�����Բ���ĽǶȷ�Χ�ڡ�
		else
		{
			//��ȡ����������Բ�еĽǶ�
			//ע������m_InterP1��m_InterP2�ľ������⣬���ԣ���ĽǶȿ�����ԭ���ĽǶ���һ���Ĳ�ࡣ
			m_InterPAngle1 = m_geomele.ForCircleStartAngle(m_InterP1.x, m_InterP1.y, pCGCricle);
			m_InterPAngle2 = m_geomele.ForCircleStartAngle(m_InterP2.x, m_InterP2.y, pCGCricle);
			//�жϽǶ��ǲ�����Բ���ĽǶȷ�Χ�ڡ�
			//ע��Բ�����и���ֹ�ǿ����180�㣬�����п�������ʱ����ת180����ʱ����ת180��
			//������Ƕȴ���180������˳ʱ����ת�Ļ����յ�ǶȾͻ��ǳ���360������ʼ�Ƕ�Ϊ200�㣬��ô��ֹ����380�㣨ʵ������20�㣩��������Ҫ�ж�200��-360����0��-20��
			//������Ƕ�С��180��������ʱ����ת�Ļ����յ�ǶȾͻ���С��0�㣬����ʼ�Ƕ�Ϊ160�㣬��ô��ֹ����-20�㣨ʵ������340�㣩��������Ҫ�ж�0��-160����340��-360��
			//��������㷨�ĺ���������δ���Բ������ֹ�Ƕ����⣬�Լ�����˳ʱ����ת������ʱ����ת����
			///////////////////////////////////////////////
			//�ֳ���ֹ�ǵĴ�С�������õ���
			//����ֹ��С����ֹ��
			if (m_ArcAngleStart<m_ArcAngleEnd)
			{
				;//���ñ�
			}
			else//����Ҫ����ֹ�Ƕȵ���
			{
				m_ArcAngleStart = m_ArcAngleEnd;
				m_ArcAngleEnd = pCGArc->m_GeomStandData.m_arc.m_ArcAngle_start;
			}
			//�ر�ע�⣬������������жϵ�ԭ����Բ��Ϊ��Բ������Ϊ180�㣬�������С��180���Բ��������
			//�ر�ע�⣬������������жϵ�ԭ����Բ��Ϊ��Բ������Ϊ180�㣬�������С��180���Բ��������
			//�ر�ע�⣬������������жϵ�ԭ����Բ��Ϊ��Բ������Ϊ180�㣬�������С��180���Բ������
			//�ϰ루��룩����Բ���ж�
			if (fabs(m_MidAngle - (m_ArcAngleStart + m_ArcAngleEnd))<1)//1����Ϊ���Ȳ�һ���ܸߣ�ԭ������Ҫ����0��
			{
				//˵���ö�Բ�����Կ�������С�ǶȾ���˳ʱ����ת����Ƕ�
				//�жϽ����ǲ��������Բ����
				if (((m_ArcAngleStart <= m_InterPAngle1) && (m_InterPAngle1 <= m_ArcAngleEnd))//����1�����Բ����
					|| ((m_ArcAngleStart <= m_InterPAngle2) && (m_InterPAngle2 <= m_ArcAngleEnd)))//���߽���2�����Բ����
				{
					return true;//˵�����ཻ��
				}
				else
				{
					return false;//˵�����ཻ
				}
			}
			//��������°루�Ұ룩����Բ���ж�
			else
			{
				//˵���ö�Բ�����Կ������ɴ�ǶȾ���˳ʱ����ת��360��Ȼ���ִ�0�㾭��˳ʱ����ת��С�Ƕȡ�
				//�жϽ����ǲ�����������Բ���ڲ�
				if((((m_ArcAngleEnd <= m_InterPAngle1) && (m_InterPAngle1 <= 360))//����1�ڵ�һ��Բ����
					|| ((0<= m_InterPAngle1) && (m_InterPAngle1 <= m_ArcAngleStart)))//���߽���1�ڵڶ���Բ����
					|| (((m_ArcAngleEnd <= m_InterPAngle2) && (m_InterPAngle2 <= 360))//����2�ڵڶ���Բ����
					|| ((0 <= m_InterPAngle2) && (m_InterPAngle2 <= m_ArcAngleStart))))//���߽���2�ڵ�һ��Բ����
				{
					return true;//˵�����ཻ��
				}
				else
				{
					return false;//˵�����ཻ
				}
			
			}

		}
	}


}
//��㵽�߶�֮��ľ���
double CutLeadLine::CalcuDistance(Point m_point, Point m_LineP1, Point m_LineP2)
{
	double x, y, x1, y1, x2, y2;
	x = m_point.x;
	y = m_point.y;
	x1 = m_LineP1.x;
	y1 = m_LineP1.y;
	x2 = m_LineP2.x;
	y2 = m_LineP2.y;
	//�㷨����
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
//���߶���Բ�Ľ���
//��Բ���ܴ����������㣬�����������������ptInter1��ptInter2��Ϊ��Чֵ�������һ�����㣬��ptInter2��ֵΪ��Чֵ���˴�Ϊ65536.0
// �߶����,�߶��յ�,Բ������,ȫ�ֵĽ�������
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
//�����һ�ָ��ݾ��ΰ���Ԥ�ж��Ƿ����ཻ��ϵ��������ٽ�һ�����������㷨If_HaveCGLineInterPoint��
//���и������ߵİ���������ջ��İ�����ν����ж�
bool CutLeadLine::If_HaveCGLineInterPoint_UseRect(GeomEleNode*pCGLine, GeomCloseHEAD*pCHtemp)
{
	//�и��������а��磬��ջ�Ҳ�а���
	//���ж����ߵİ����Ƿ��ཻ����ջ��İ���һ�����и������ߵĴ�
	double x1_min, x1_max, y1_min, y1_max;//��ջ��İ�����εķ�Χ
	double x2_min, x2_max, y2_min, y2_max;//�и������ߵİ�����εķ�Χ
	double x_length, y_length;//�и������ߵĳ��ȷ�Χ��
	//��ջ��İ������
	x1_min = pCHtemp->m_GemoClosedLimtPoint.x_min.x;
	x1_max = pCHtemp->m_GemoClosedLimtPoint.x_max.x;
	y1_min = pCHtemp->m_GemoClosedLimtPoint.y_min.y;
	y1_max = pCHtemp->m_GemoClosedLimtPoint.y_max.y;
	//�и������ߵİ������
	x2_min = pCGLine->m_AuxiLineLimtPoint.x_min;
	x2_max = pCGLine->m_AuxiLineLimtPoint.x_max;
	y2_min = pCGLine->m_AuxiLineLimtPoint.y_min;
	y2_max = pCGLine->m_AuxiLineLimtPoint.y_max;
	x_length = x2_max - x2_min;
	y_length = y2_max - y2_min;
	//�ж��������εĹ�ϵ
	//���Ǽ�������Լ��ཻ�ķ�Χ
	if ((((x1_min - x_length) <= x2_min) && (x2_max <= (x1_max + x_length))) && (((y1_min - y_length) <= y2_min) && (y2_max <= (y1_max + y_length))))
	{
		return true;
	}
	else
	{
		return false;
	}

}


//�����и�������
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
	int m_TypeCGLine = 2;//�����и������ߵķ�ʽ��2Ϊ��������
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
	m_cutline = Get_CutLeadLine(m_startline, m_endline, m_Singlelayer, m_TypeCGLine);
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
//�����и���������ȡ�и������߿��Ƶ�ķ���
//��ʱ�������㣬������ȡ
Line_para  CutLeadLine::Get_ChangeCutLine_StartPoint(Line_para m_Line, int m_OpenDirect, Point start)
{
	Line_para m_CutGuideL;
	m_CutGuideL.x1 = start.x;
	m_CutGuideL.y1 = start.y;
	m_CutGuideL.k = m_Line.k;
	//����ȡ
	//����������ϣ���ȡy��ģ��������£���ȡyС�ġ�
	//�������������ȡxС�ģ��������ң���ȡx��ġ�
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

	return m_CutGuideL;
}
//�и������ߣ�ֱ����Բ��������һ������������ͼԪ�ϡ�
//������������жϵ��и������ߵġ�
//��δ����������飬��Ҫ�������
GeomEleNode* CutLeadLine::GetLimtOfAuxiliaryLine(GeomCloseHEAD*pCHtemp)
{
	GeomEleNode*pFirst, *pEnd;//��ջ��ĵ�һ������и������ߣ�����Բ���и�������ʱ
	GeomEleNode*pCGLine = (GeomEleNode*)malloc(sizeof(GeomEleNode));//�����洢�и�������
	double x_min = -9999.99, y_min = -9999.99, x_max = -9999.99, y_max = -9999.99, x_mid = -9999.99, y_mid = -9999.99;//�����и������ߵ��������Ρ�x_mid, y_mid��Բ�����еġ�
	double x[3], y[3];

	GCIRCLE m_circle;//����Բ������ֹ����
	//����Բ���и������߶��ԣ��и������߾Ͳ������ǵ�һ����ͼԪ�����������һ������ͼԪ��
	if (61 == pCHtemp->FirstGeomele->m_GeomStandData.m_typegeomele)//�����ֱ�ߵ������
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
		//Բ���и��������е�Բ����
		m_circle.m_Circent_x = pCHtemp->FirstGeomele->m_GeomStandData.m_arc.Arccent_x;
		m_circle.m_Circent_y = pCHtemp->FirstGeomele->m_GeomStandData.m_arc.Arccent_y;
		m_circle.m_Circle_r = m_CutLineLength;

		pFirst = pCHtemp->FirstGeomele;
		pEnd = pCHtemp->FirstGeomele;
		while (pEnd->nextGeomeleNode)
			pEnd = pEnd->nextGeomeleNode;
		pCGLine->m_GeomStandData = pFirst->m_GeomStandData;//�Ƚ�Բ�������߸���pCGLine
		//Ψһ��ͬ���ǣ�Ҫ��Բ��������pEnd���и��յ��Լ���ֹ�Ǹ���pFirst
		pCGLine->m_GeomStandData.GeoEle_start_x1 = pEnd->m_GeomStandData.GeoEle_start_x1;
		pCGLine->m_GeomStandData.GeoEle_start_y1 = pEnd->m_GeomStandData.GeoEle_start_y1;
		//ĩ�˵ĽǶ�Ӧ���Ǽ���ó���
		pCGLine->m_GeomStandData.m_arc.m_ArcAngle_end = m_geomele.ForCircleStartAngle(pCGLine->m_GeomStandData.GeoEle_start_x1, pCGLine->m_GeomStandData.GeoEle_start_y1, m_circle);
		pCGLine->m_GeomStandData.k = pFirst->m_GeomStandData.m_arc.m_ArcAngle_end;//ע�����k�������Բ���и������ߵ��м�ֵ
		//���ƾ���
		x[0] = pCGLine->m_GeomStandData.GeoEle_start_x0;//Բ�������
		x[1] = pCGLine->m_GeomStandData.GeoEle_start_x1;//Բ�����յ�
		x[2] = pEnd->m_GeomStandData.GeoEle_start_x0;//Բ�����е�
		y[0] = pCGLine->m_GeomStandData.GeoEle_start_y0;
		y[1] = pCGLine->m_GeomStandData.GeoEle_start_y1;
		y[2] = pEnd->m_GeomStandData.GeoEle_start_y0;
		pCGLine->m_AuxiLineLimtPoint.x_min = x[0];//�����ʼֵ
		pCGLine->m_AuxiLineLimtPoint.x_max = x[0];//�����ʼֵ
		pCGLine->m_AuxiLineLimtPoint.y_min = y[0];//�����ʼֵ
		pCGLine->m_AuxiLineLimtPoint.y_max = y[0];//�����ʼֵ
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


