#pragma once
#define PI 3.1415926
//********************************************************************//
//��һ��ṹ�������������dxf�ж�ȡ���ݵ����ݽṹ������������
//ֻ��LINE,ARC,CIRCLE���������ݽṹ�ˣ������תΪ��LINE
//********************************************************************//
typedef struct 
{
	double x0, y0, x1, y1;//ֱ�ߵ���ֹ��
	
}GLINE;//��ȡ����dxf���ʱ���ֱ�����ݴ洢�ṹ
typedef struct
{
	double Arccent_x, Arccent_y;//����ARC��Բ��
	double m_Arc_r;//ARC�İ뾶
	double m_ArcAngle_start, m_ArcAngle_end;//ARC����ֹ�Ƕ�
	
}GARC;
typedef struct
{
	double m_Circle_Start_Angle;//ת�����и��ʱ���и�Բʱ�����ʼ�ǡ�
	double m_Circent_x, m_Circent_y;//CIRCLE��Բ��
	double m_Circle_r;//CIRCLE�İ뾶
}GCIRCLE;
typedef struct
{
	//��Ϊÿһ��ͼԪ�ı�׼ֱ�߶Σ���������Ѱ��ͬһ�����ͼԪ���õı�׼
	//�и������ߵ�����Ҳ����������
	double GeoEle_start_x0, GeoEle_start_y0, GeoEle_start_x1, GeoEle_start_y1;
	GLINE m_line;//������������
	GARC m_arc;
	GCIRCLE m_circle;
	//�����и������߻���һ��б��Ҫ����
	double k;//Ĭ����9876.5��
	//////////////
	unsigned int m_typegeomele;//1ΪLINE,2ΪARC,3ΪCIRCLE,6Ϊ�и������ߣ�Ĭ����1
	int m_GeomEleID;//��ÿһ��ͼԪ��ţ�����ΪѰ�ҷ��ͼԪ�����õģ�Ĭ��Ϊ0,��ô��һ������1
	int m_GeomCloseID;//��ÿһ����ջ�������ͼԪ���б�ţ���������ջ�֮���ж�ʹ�ã�Ĭ��Ϊ0,��ô��һ������1
	bool m_IsGeomeleAccept;//�жϸ�ͼԪ�ǲ����Ѿ�����¼��Ĭ��Ϊfales
	bool m_IsTranData;//�ж��Ƿ������Ѿ������ı䣬�ڲ����ʱ���Ҫ�ж������Ƿ���Ҫ��������Ĭ����fales
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
	GLINE m_line ;
	GARC m_arc ;
	GCIRCLE m_circle;
	GeomStandData m_geomstandData ;
	
public://�����Ҫʵ�ֵĹ���
	
    GeomStandData ReadLineData(GLINE m_line);//����dxf����LINE���ĸ����������
	GeomStandData ReadArcData(GARC m_arc);//��ȡARC�Ĳ����������������������ҪתΪͼԪ��ͳһ�����յ㣬��������Ѱ�ҷ�ջ���
	GeomStandData ReadCircleData(GCIRCLE m_circle);//����Բ���ԣ���ô�����Ƕ����ķ�ջ���������һ���ر��ֵ��99999���¸����жϵ����ֵ��ʱ��ֱ��������һ����ջ��ͺ�
	//����Բ��˵��Ҫ����������Ĵ�׵����꣬����и�ʱ����Ҫ����ʼ�ǲ���
	double ForCircleStartAngle(double inter_x, double inter_y,GCIRCLE m_circle);
};

