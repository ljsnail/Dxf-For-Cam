#pragma once
#define PI 3.1415926
//********************************************************************//
//��һ��ṹ�������������dxf�ж�ȡ���ݵ����ݽṹ������������
//ֻ��LINE,ARC,CIRCLE���������ݽṹ�ˣ������תΪ��LINE
//********************************************************************//
typedef struct
{
	double x0, y0, x1, y1;//ֱ�ߵ���ֹ��
	
}LINE;//��ȡ����dxf���ʱ���ֱ�����ݴ洢�ṹ
typedef struct
{
	double Arccent_x, Arccent_y;//����ARC��Բ��
	double m_Arc_r;//ARC�İ뾶
	double m_ArcAngle_start, m_ArcAngle_end;//ARC����ֹ�Ƕ�
}ARC;
typedef struct
{
	double m_Circent_x, m_Circent_y;//CIRCLE��Բ��
	double m_Circle_r;//CIRCLE�İ뾶
}CIRCLE;
typedef struct
{
	double GeoEle_start_x0, GeoEle_start_y0, GeoEle_start_x1, GeoEle_start_y1;//��Ϊÿһ��ͼԪ�ı�׼ֱ�߶Σ���������Ѱ��ͬһ�����ͼԪ���õı�׼
	LINE m_line;//������������
	ARC m_arc;
	CIRCLE m_circle;
	unsigned int m_typegeomele;//1ΪLINE,2ΪARC,3ΪCIRCLE,Ĭ����1
	int m_GeomEleID;//��ÿһ��ͼԪ��ţ�����ΪѰ�ҷ��ͼԪ�����õģ�Ĭ��Ϊ-1
	int m_GeomCloseID;//��ÿһ����ջ�������ͼԪ���б�ţ���������ջ�֮���ж�ʹ�ã�Ĭ��Ϊ-1
	bool m_IsGeomeleAccept;//�жϸ�ͼԪ�ǲ����Ѿ�����¼��Ĭ��ΪNULL
}GeomStandData;//����һ��ͳһ�����ݽṹ������������е����ͣ�������
//********************************************************************//
//�ڶ���ṹ��Ҫ��������������ݽṹ����ͳһ��׼��ID,����������������ͼԪ��
//��Ӧ����һ��ͳһ�����ݽṹ���ͣ�������LINE,ARC,CIRCLE�����������������
//********************************************************************//
class GEOMELE//ֻ��������ͼԪ�������д���
{
public:
	GEOMELE();
	~GEOMELE();
public://��LINE ARC CIRLCE��Ԫ�ض�ȡ�����������и�ʱ��Ҫ�õ���Щԭʼ����
	LINE m_line ;
	ARC m_arc ;
	CIRCLE m_circle;
	GeomStandData m_geomstandData ;
	
public://�����Ҫʵ�ֵĹ���
	
	GeomStandData ReadLineData(LINE m_line);//����dxf����LINE���ĸ����������
	GeomStandData ReadARCData(ARC m_arc);//��ȡARC�Ĳ����������������������ҪתΪͼԪ��ͳһ�����յ㣬��������Ѱ�ҷ�ջ���
	GeomStandData ReadCIRCLE(CIRCLE m_circle);//����Բ���ԣ���ô�����Ƕ����ķ�ջ���������һ���ر��ֵ��99999���¸����жϵ����ֵ��ʱ��ֱ��������һ����ջ��ͺ�
	
};

