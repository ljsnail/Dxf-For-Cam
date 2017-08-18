#include "stdafx.h"
#include "GEOMELE.h"
#include<math.h>//�������Ǻ���ʱ��


GEOMELE::GEOMELE() 
{
	 /////*m_line = { 0.0, 0.0, 0.0, 0.0 };
	 ////m_arc = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	 ////m_circle = { 0.0, 0.0, 0.0 };
	 ////m_geomstandData = { 0.0, 0.0, 0.0, 0.0, m_line, m_arc, m_circle, 1, 0, 0, NULL };*/

}


GEOMELE::~GEOMELE()
{
}
GeomStandData GEOMELE::ReadLineData(GLINE line)//ReadLineData��GeomStandData ���͵ĺ���������Ҫ��ǰ���GeomStandData
{
	//����LINE���ĸ����������GEOMELE����ʼ��ֹ����
	//ͬʱ��ʼ��
	m_geomstandData.GeoEle_start_x0 = line.x0;
	m_geomstandData.m_typegeomele = 1;
	m_geomstandData.GeoEle_start_y0 = line.y0;
	m_geomstandData.GeoEle_start_x1 = line.x1;
	m_geomstandData.GeoEle_start_y1 = line.y1;
	m_geomstandData.m_line = line;//��dxf�����lineֵ����ͳһ��ͼԪ����
	m_geomstandData.m_arc = { 0.0, 0.0, 0.0, 0.0, 0.0 };//����֮ǰ��ȡ��ARC�����֮ǰ�������������ֵ
	m_geomstandData.m_circle = { 0.0, 0.0, 0.0 };//����֮ǰ��ȡ��ARC�����֮ǰ�������������ֵ
	m_geomstandData.m_GeomEleID++;
	m_geomstandData.m_GeomCloseID = 1;//LINE ARC ������ʱ�򶼹�Ϊ��һ����ջ�
	m_geomstandData.m_IsGeomeleAccept = false;
	m_geomstandData.m_IsTranData = false;
	m_geomstandData.k = 9876.5;//Ϊ�����и�����������б��
	return m_geomstandData;
}
//����ARC�����������ת�����GEOMELE����ʼ��ֹ����
GeomStandData GEOMELE::ReadArcData(GARC m_arc)
{	
	    double angle_start,angle_end;//ת���Ƕ�ʹ��
		angle_start = PI*m_arc.m_ArcAngle_start / 180.0;
		angle_end = PI*m_arc.m_ArcAngle_end / 180.0;
		m_geomstandData.GeoEle_start_x0 = m_arc.Arccent_x + m_arc.m_Arc_r*cos(angle_start);//ת��ΪԲ������ʼ�˵�
		m_geomstandData.GeoEle_start_y0 = m_arc.Arccent_y - m_arc.m_Arc_r*sin(angle_start);
		m_geomstandData.GeoEle_start_x1 = m_arc.Arccent_x + m_arc.m_Arc_r*cos(angle_end);//ת��ΪԲ������ʼ�˵�
		m_geomstandData.GeoEle_start_y1 = m_arc.Arccent_y - m_arc.m_Arc_r*sin(angle_end);
		m_geomstandData.m_line = { 0.0, 0.0, 0.0, 0.0 };
		m_geomstandData.m_circle = { 0.0, 0.0, 0.0 };//����֮ǰ��ȡ��ARC�����֮ǰ�������������ֵ
		m_geomstandData.m_typegeomele = 2;//2ΪԲ��
		m_geomstandData.m_arc = m_arc;
		m_geomstandData.m_GeomEleID++;
		m_geomstandData.m_GeomCloseID = 1;//LINE ARC ������ʱ�򶼹�Ϊ��һ����ջ�
		m_geomstandData.m_IsGeomeleAccept = false;
		m_geomstandData.m_IsTranData = false;
		return m_geomstandData;
}
//����Բ���ԣ���Ϊ�Ѿ���һ�������ķ��ͼԪ�ˣ����Բ���Ҫ����ֹ�㣬��Ҫ��ʶ��ʼ
GeomStandData GEOMELE::ReadCircleData(GCIRCLE m_circle)
{	//����ԭ���Բ��Ϊ�����ֱ����Բ���̵Ľ��㣬ȡ���ԭ����Ǹ�����
	double a, b, r,inter_x,inter_y;
	a = m_circle.m_Circent_x;
	b = m_circle.m_Circent_y;
	r = m_circle.m_Circle_r;
	inter_x = a - ((a*r) / (sqrt(a*a + b*b)));//ת����x����
	inter_y = (b / a)*inter_x;//ת����y����
	m_geomstandData.GeoEle_start_x0 = inter_x;//Բ�Ĵ�׵�x����
	m_geomstandData.GeoEle_start_y0 = inter_y;//Բ�Ĵ�׵�x����
	m_geomstandData.GeoEle_start_x1 = inter_x;//Բ��������ʹ���俪ʼ��������ͬһ��
	m_geomstandData.GeoEle_start_y1 = inter_y;//Բ��������ʹ���俪ʼ��������ͬһ��
	m_geomstandData.m_circle = m_circle;//����ԭ�е����ݡ�
	m_geomstandData.m_circle.m_Circle_Start_Angle = ForCircleStartAngle(inter_x, inter_y, m_circle);
	m_geomstandData.m_typegeomele = 3;//3ΪԲ����������switch
	m_geomstandData.m_GeomEleID++;
	m_geomstandData.m_GeomCloseID++;//ÿ��Բ�����Զ���Ϊһ����ջ�
	m_geomstandData.m_line = { 0.0, 0.0, 0.0, 0.0 };
	m_geomstandData.m_arc = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	m_geomstandData.m_IsGeomeleAccept = false;
	m_geomstandData.m_IsTranData = false;
	return m_geomstandData;
}
double GEOMELE::ForCircleStartAngle(double inter_x, double inter_y, GCIRCLE m_circle)
{
	//Ҫ�����ĸ�����ȥ��������
	double a, b, r;
	double m_CircleStartAngle=-9999, m_abs_y;
	a = m_circle.m_Circent_x;
	b = m_circle.m_Circent_y;
	r = m_circle.m_Circle_r;
	//����ǵ�һ����
	if ((a <= inter_x) && (inter_x <= (a + r)) && (b <= inter_y)&&(inter_y <= (b + r)))
	{
		m_abs_y = inter_y - b;
		m_CircleStartAngle = asin((m_abs_y/r)) * 180 / PI;

	}
	//����ǵڶ�����
	else if (((a - r) <= inter_x) && (inter_x<a) && (b <= inter_y)&&( inter_y <= (b + r)))
	{
		m_abs_y = inter_y - b;
		m_CircleStartAngle = asin((m_abs_y / r)) * 180 / PI;
		m_CircleStartAngle = 180 - m_CircleStartAngle;

	}
	//����ǵ�������
	else if (((a - r) <= inter_x) && (inter_x<a) && ((b - r) <= inter_y)&&(inter_y <b))
	{
		m_abs_y = b-inter_y ;
		m_CircleStartAngle = asin((m_abs_y / r)) * 180 / PI;
		m_CircleStartAngle = 180 + m_CircleStartAngle;

	}
	//����ǵ�������
	else if ((a <= inter_x) && (inter_x <= (a + r)) && ((b - r) <= inter_y)&&(inter_y <b))
	{
		m_abs_y = b-inter_y;
		m_CircleStartAngle = asin((m_abs_y / r)) * 180 / PI;
		m_CircleStartAngle = 360 - m_CircleStartAngle;

	}
	return m_CircleStartAngle;
}
//���ˣ������������DXFͼ��Ԫ�ؾͶ����ˣ���һ�ֲ���׼����ȫ��
