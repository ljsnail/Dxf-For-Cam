#pragma once
#define PI 3.1415926
//********************************************************************//
//第一层结构，从排样结果的dxf中读取数据的数据结构，经过排样后，
//只有LINE,ARC,CIRCLE这三种数据结构了，多段线转为了LINE
//********************************************************************//
typedef struct
{
	double x0, y0, x1, y1;//直线的起止点
	
}LINE;//读取排样dxf结果时候的直线数据存储结构
typedef struct
{
	double Arccent_x, Arccent_y;//保留ARC的圆心
	double m_Arc_r;//ARC的半径
	double m_ArcAngle_start, m_ArcAngle_end;//ARC的起止角度
}ARC;
typedef struct
{
	double m_Circent_x, m_Circent_y;//CIRCLE的圆心
	double m_Circle_r;//CIRCLE的半径
}CIRCLE;
typedef struct
{
	double GeoEle_start_x0, GeoEle_start_y0, GeoEle_start_x1, GeoEle_start_y1;//作为每一个图元的标准直线段，用作后续寻求同一个封闭图元而用的标准
	LINE m_line;//包含三种类型
	ARC m_arc;
	CIRCLE m_circle;
	unsigned int m_typegeomele;//1为LINE,2为ARC,3为CIRCLE,默认是1
	int m_GeomEleID;//对每一个图元编号，将来为寻找封闭图元遍历用的，默认为-1
	int m_GeomCloseID;//对每一个封闭环所属的图元进行编号，给将来封闭环之间判断使用，默认为-1
	bool m_IsGeomeleAccept;//判断该图元是不是已经被收录，默认为NULL
}GeomStandData;//创建一个统一的数据结构，用来存放所有的类型，带着走
//********************************************************************//
//第二层结构，要对上面的三种数据结构建立统一标准的ID,用来后续遍历所有图元用
//这应该是一个统一的数据结构类型，包含了LINE,ARC,CIRCLE上面的三种数据类型
//********************************************************************//
class GEOMELE//只是用来对图元参数进行处理
{
public:
	GEOMELE();
	~GEOMELE();
public://将LINE ARC CIRLCE等元素读取进来，将来切割时候要用到这些原始数据
	LINE m_line ;
	ARC m_arc ;
	CIRCLE m_circle;
	GeomStandData m_geomstandData ;
	
public://这个类要实现的功能
	
	GeomStandData ReadLineData(LINE m_line);//输入dxf里面LINE的四个参数，输出
	GeomStandData ReadARCData(ARC m_arc);//读取ARC的参数，共有五个参数，但是要转为图元的统一起点和终点，用来将来寻找封闭环用
	GeomStandData ReadCIRCLE(CIRCLE m_circle);//对于圆而言，那么他就是独立的封闭环，给它附一个特别的值如99999，下个类判断到这个值的时候，直接让他是一个封闭环就好
	
};

