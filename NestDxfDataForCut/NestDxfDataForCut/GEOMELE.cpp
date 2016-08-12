#include "stdafx.h"
#include "GEOMELE.h"
#include<math.h>//�������Ǻ���ʱ��


GEOMELE::GEOMELE() 
{
	 m_line = { 0.0, 0.0, 0.0, 0.0 };
	 m_arc = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	 m_circle = { 0.0, 0.0, 0.0 };
	 m_geomstandData = { 0.0, 0.0, 0.0, 0.0, m_line, m_arc, m_circle, 1, 0, 0, NULL };

}


GEOMELE::~GEOMELE()
{
}
GeomStandData GEOMELE::ReadLineData(GLINE line)//ReadLineData��GeomStandData ���͵ĺ���������Ҫ��ǰ���GeomStandData
{
	//����LINE���ĸ����������GEOMELE����ʼ��ֹ����
	m_geomstandData.m_typegeomele = 1;
	m_geomstandData.GeoEle_start_x0 = line.x0;
	m_geomstandData.GeoEle_start_y0 = line.y0;
	m_geomstandData.GeoEle_start_x1 = line.x1;
	m_geomstandData.GeoEle_start_y1 = line.y1;
	m_geomstandData.m_line = line;//��dxf�����lineֵ����ͳһ��ͼԪ����
	m_geomstandData.m_arc = { 0.0, 0.0, 0.0, 0.0, 0.0 };//����֮ǰ��ȡ��ARC�����֮ǰ�������������ֵ
	m_geomstandData.m_circle = { 0.0, 0.0, 0.0 };//����֮ǰ��ȡ��ARC�����֮ǰ�������������ֵ
	m_geomstandData.m_GeomEleID++;
	return m_geomstandData;
}
//����ARC�����������ת�����GEOMELE����ʼ��ֹ����
GeomStandData GEOMELE::ReadArcData(GARC m_arc)
{	
	    double angle_start,angle_end;//ת���Ƕ�ʹ��
		angle_start = PI*m_arc.m_ArcAngle_start / 180.0;
		angle_end = PI*m_arc.m_ArcAngle_end / 180.0;
		m_geomstandData.GeoEle_start_x0 = m_arc.Arccent_x + m_arc.m_Arc_r*cos(angle_start);//ת��ΪԲ������ʼ�˵�
		m_geomstandData.GeoEle_start_y0 = m_arc.Arccent_y + m_arc.m_Arc_r*sin(angle_start);
		m_geomstandData.GeoEle_start_x1 = m_arc.Arccent_x + m_arc.m_Arc_r*cos(angle_end);//ת��ΪԲ������ʼ�˵�
		m_geomstandData.GeoEle_start_y1 = m_arc.Arccent_y + m_arc.m_Arc_r*sin(angle_end);
		m_geomstandData.m_line = { 0.0, 0.0, 0.0, 0.0 };
		m_geomstandData.m_circle = { 0.0, 0.0, 0.0 };//����֮ǰ��ȡ��ARC�����֮ǰ�������������ֵ
		m_geomstandData.m_typegeomele = 2;//2ΪԲ��
		m_geomstandData.m_arc = m_arc;
		m_geomstandData.m_GeomEleID++;
		return m_geomstandData;
}
//����Բ���ԣ���Ϊ�Ѿ���һ�������ķ��ͼԪ�ˣ����Բ���Ҫ����ֹ�㣬��Ҫ��ʶ��ʼ
GeomStandData GEOMELE::ReadCircleData(GCIRCLE m_circle)
{
	
	m_geomstandData.m_circle = m_circle;//����ԭ�е����ݡ�
	m_geomstandData.m_typegeomele = 3;//3ΪԲ����������switch
	m_geomstandData.m_GeomEleID++;
	m_geomstandData.m_line = { 0.0, 0.0, 0.0, 0.0 };
	m_geomstandData.m_arc = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	return m_geomstandData;
}
//���ˣ������������DXFͼ��Ԫ�ؾͶ����ˣ���һ�ֲ���׼����ȫ��
