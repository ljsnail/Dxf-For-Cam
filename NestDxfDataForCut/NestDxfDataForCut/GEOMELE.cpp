#include "stdafx.h"
#include "GEOMELE.h"
#include<math.h>//运用三角函数时候


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
GeomStandData GEOMELE::ReadLineData(GLINE line)//ReadLineData是GeomStandData 类型的函数，所以要在前面加GeomStandData
{
	//输入LINE的四个参数，输出GEOMELE的起始终止参数
	//同时初始化
	m_geomstandData.m_typegeomele = 1;
	m_geomstandData.GeoEle_start_x0 = line.x0;
	m_geomstandData.GeoEle_start_y0 = line.y0;
	m_geomstandData.GeoEle_start_x1 = line.x1;
	m_geomstandData.GeoEle_start_y1 = line.y1;
	m_geomstandData.m_line = line;//把dxf里面的line值传递统一的图元里面
	m_geomstandData.m_arc = { 0.0, 0.0, 0.0, 0.0, 0.0 };//假如之前读取过ARC则清空之前保留在这里的数值
	m_geomstandData.m_circle = { 0.0, 0.0, 0.0 };//假如之前读取过ARC则清空之前保留在这里的数值
	m_geomstandData.m_GeomEleID++;
	m_geomstandData.m_GeomCloseID = 1;//LINE ARC 进来的时候都挂为第一个封闭环
	m_geomstandData.m_IsGeomeleAccept = false;
	m_geomstandData.m_IsTranData = false;
	return m_geomstandData;
}
//输入ARC的五个参数，转换输出GEOMELE的起始终止参数
GeomStandData GEOMELE::ReadArcData(GARC m_arc)
{	
	    double angle_start,angle_end;//转换角度使用
		angle_start = PI*m_arc.m_ArcAngle_start / 180.0;
		angle_end = PI*m_arc.m_ArcAngle_end / 180.0;
		m_geomstandData.GeoEle_start_x0 = m_arc.Arccent_x + m_arc.m_Arc_r*cos(angle_start);//转换为圆弧的起始端点
		m_geomstandData.GeoEle_start_y0 = m_arc.Arccent_y + m_arc.m_Arc_r*sin(angle_start);
		m_geomstandData.GeoEle_start_x1 = m_arc.Arccent_x + m_arc.m_Arc_r*cos(angle_end);//转换为圆弧的起始端点
		m_geomstandData.GeoEle_start_y1 = m_arc.Arccent_y + m_arc.m_Arc_r*sin(angle_end);
		m_geomstandData.m_line = { 0.0, 0.0, 0.0, 0.0 };
		m_geomstandData.m_circle = { 0.0, 0.0, 0.0 };//假如之前读取过ARC则清空之前保留在这里的数值
		m_geomstandData.m_typegeomele = 2;//2为圆弧
		m_geomstandData.m_arc = m_arc;
		m_geomstandData.m_GeomEleID++;
		m_geomstandData.m_GeomCloseID = 1;//LINE ARC 进来的时候都挂为第一个封闭环
		m_geomstandData.m_IsGeomeleAccept = false;
		m_geomstandData.m_IsTranData = false;
		return m_geomstandData;
}
//对于圆而言，因为已经是一个独立的封闭图元了，所以不需要给起止点，但要标识起始
GeomStandData GEOMELE::ReadCircleData(GCIRCLE m_circle)
{	//求以原点和圆心为两点的直线与圆方程的交点，取最靠近原点的那个交点
	double a, b, r;
	a = m_circle.m_Circent_x;
	b = m_circle.m_Circent_y;
	r = m_circle.m_Circle_r;
	m_geomstandData.GeoEle_start_x0 = a - ((a*r) / (sqrt(a*a + b*b)));//转换得x交点
	m_geomstandData.GeoEle_start_y0 = (b / a)*m_geomstandData.GeoEle_start_x0;//转换得y交点
	m_geomstandData.GeoEle_start_x1 = 99999.0;//特殊值用来与LINE ARC区分开
	m_geomstandData.GeoEle_start_y1 = 99999.0;//特殊值用来与LINE ARC区分开
	m_geomstandData.m_circle = m_circle;//保存原有的数据。
	m_geomstandData.m_typegeomele = 3;//3为圆，将来用作switch
	m_geomstandData.m_GeomEleID++;
	m_geomstandData.m_GeomCloseID++;//每个圆都可以独立为一个封闭环
	m_geomstandData.m_line = { 0.0, 0.0, 0.0, 0.0 };
	m_geomstandData.m_arc = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	m_geomstandData.m_IsGeomeleAccept = false;
	m_geomstandData.m_IsTranData = false;
	return m_geomstandData;
}
//至此，整个排样结果DXF图的元素就读完了，第一手材料准备完全。
