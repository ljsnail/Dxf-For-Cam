#pragma once
#include"GeomClose.h"
//�������Ҫ��ʵ�ְ�֮ǰ�ķ�ջ�֮�䴮�����и�ʹ�á�
//ʵ�֣�����ջ�֮�����򣬶�Բ��������ͼ��Ҫ���⴦������Բ��Ҫת����ԭ����������ʽ
//�������ͼԪ֮��Ĺ���ֱ��
//Ū�������������洢��һ������dxf������

typedef struct NestResultDataNode//�洢���е�ͼԪ�ĵ�������
{
	
	struct GeomCloseHEAD*FirstGeomClose;//ָ���ջ�GeomCloseHEAD��ͷ���
	struct NestResultDataNode*prevNestResultDataNode;//ָ��ǰһ������ͼ���ݽ��NestResultDataNode�ĵ�ַ���ڲ�ͬ���������ͼ֮�����
	struct NestResultDataNode*nextNestResultDataNode;//ָ��ǰһ������ͼ���ݽ��NestResultDataNode�ĵ�ַ���ڲ�ͬ���������ͼ֮�����
	unsigned int NestResult_ID;//ϵͳ�洢�ĵڼ����������ͼ����ʼ��Ϊ0��
	unsigned int m_NumGeomClose;//��˫������洢�ķ�ջ��ĸ���
	bool AcceptNestResultflag;//�Ƿ��Ѿ������ã���ʼ��Ϊfalse,Ŀ�Ĳ�ͬ�������ͼ���е��á�
}NestResultDataNode;
//�洢�������Σ�һ�ο����ͻ���һ���ڴ汣��ָ�������������ӹ���n���������ͼ����ÿ���������ͼ�����n�����ͼ�Σ���ÿ�����ͼ�������n��������ԪLINE,ARC,CIRCLE
typedef struct BatchHEAD
{
	unsigned int BatchHEAD_ID;
	unsigned int m_NumNestrsltDtNode;//��������ж������������ͼ
	struct NestResultDataNode*FirstNestRsltNode;//ָ���һ�żӹ�������ͼͷ���
}BatchHEAD;
//���ڽ���������̰���㷨�ϵ����ݽṹ
//����������̾���ͷ�ջ���ͼԪ�ڵ�����ݽṹ
typedef struct 
{
	GeomCloseHEAD*pminhead;
	GeomEleNode *pminnode;
	double m_mindistant;
}Mindistant_EleNode;
class GeomForCut
{
public:
	GeomForCut();
	~GeomForCut();
public:
	GeomClose m_geomclose;
	NestResultDataNode m_nestrsltNode;
public:
	GEOMELE m_geomele;
	BatchHEAD*CreatBatchHEAD(int BatchHEAD_ID);//��ʼ��һ���ڴ�Ϊͷ����ţ������������
	NestResultDataNode*CreatNestResultNode(int NestResult_ID);//��ʼ��һ���ڴ�����������㣬�����������
	//���޹��ɵİѽ�����
	NestResultDataNode*AddGeomCloseHeadNode(NestResultDataNode*head,GeomCloseHEAD*node);//����ÿһ���ջ���Fͷ��㣬������ȫ������һ�����ϡ�
	BatchHEAD*AddNestRsltDtNode(BatchHEAD*head, NestResultDataNode*node);//��ÿһ������ͼ�ĵ�ַ�ҵ���BatchHEADΪͷ����˫�������С�
		//***************************************************************************************//
		/**///���޹��ɵ�GeomEleNode�ҳ�����ɣ�ת�����й��ɵļ�����ջ���
		/**///*************************************************************************//
		/**///�������ȷַ�ջ�������ÿһ����ջ���������ݽ�������
		/**///*************************************************************************//
		/**///void SepDiffGeomCls(GeomCloseHEAD*head);//����ָ����¼��������ͼȫ������ͼԪ��˫�������Fͷ���
		//***************************************************************************************//

	//�������ݽ��˫���������治��ͬһ����ջ��Ľ��
	GeomEleNode*FindDiffGeomCloseNode(NestResultDataNode*head);//�����������ͼ��ͷ���
	//��һ��˫�������ϵĲ�ͬ��ջ��ҵ���ͬ������ͷָ���˫��������ȥ��
	void InsertGeomCloseHEAD(NestResultDataNode*head, GeomEleNode*node);//�����µĲ�ͬ��ջ�ID�Ľ�㣬������Fͷ��㣬��ͬʱFͷ���ָ��node��
	//���Ѿ��ҵ���ͬ��ջ�˫������������ݽ��鿴���Ƿ���һ�������ķ�ջ�
	GeomCloseHEAD*JudgeGeomCloseIntact(NestResultDataNode*head);//�����������ͼdxfͷ��㣬һ����ȥ�Ų��ջ�����µ������Ƿ�����
	//����������ͷ������������Ѱ����Ե���������;ֱ�����еķ�ջ�������λΪ������
	NestResultDataNode*Find_Change_GeomCloseHEAD(NestResultDataNode*head, GeomCloseHEAD*node);
	//����֮��Ե�ͷ��������������
	NestResultDataNode*ChangeGeomCloseHEAD(NestResultDataNode*head, GeomCloseHEAD*node);
	//�����Ѿ���ԭʼ����˫�������ϵĲ�ͬ��ջ��ҵ���ͬ�ķ�ջ�ͷ������ˣ����ǻ��漰��һ��ջ��ڲ���������δ����͸�����ջ�֮���˳��ȷ��
	//���ڵ�һ��ջ��ڲ����ԣ������������µ��Ǹ�������ڵĽ��Ϊͷ��㣬Ȼ��ı��ջ��ڲ�˳��
	void ChangeEleNodeOfGeomClosed_origin(NestResultDataNode*head);//��������ͼͷ��㣬����ÿ����ջ��ڲ����ݵ�00����̾���Ѱ��ͷ��㣬�������ͷ������ָ���ջ�ͷ����F�ڵ��
	//��������֮��һ��ջ��ڲ������ݵ����Ѿ���ϣ�����ÿһ����ջ����ͷ��㶼����Զ��������Ǹ��㡣
	//ȷ����ջ�˳��֮����ȷ��һ���ջ��ڲ���ͷ��㣬ǰ���Ȱ��յ�����ԭ����ȷ����ջ�����ʼ���ݵ�
	void ChangClosedNodeOfNRDXF(NestResultDataNode*head);
	//��������֮����������ڵķ�ջ��Ѿ�������˳����ôΪ��ʹ���г̸��̣����Ӧ�ø��ݵ���һ����ջ���̾���������ÿ����ջ��ڲ���ͷ���
	void ChangeEleNodeOfGeomClosed_order(NestResultDataNode*head);//�����������ͼͷ��㣬�����Ѿ��źõķ�ջ�
	//�����µ��㷨��������������������һ����������
	//���ڽ���������̰���㷨
	//�����������DXF��ͷ��㣬Ȼ����ÿһ������Ѱ�����ŵķ�ʽ��һ���Խ����ջ�˳��ͷ�ջ����ɵ�����
	void BaseTS_GR_forChangeClosedNodeOfNRDXF(NestResultDataNode*head);
	void BaseTS_GR_forChangeClosedNodeOfNRDXF1(NestResultDataNode*head);

	//Ѱ���������֮�����������Ǹ����ɽڵ�
	Mindistant_EleNode *FindMinDstGeomeleNode(NestResultDataNode*head, GeomCloseHEAD*pGMCLSHead);//����û����λ�ĵ�һ����ջ�ͷ��㣬�������һ���Ѿ���λ�ķ�ջ�ͷ���֮�����̾���Ĺ��ɽڵ�
	//����һ����ջ���ͷ����Ŀ��ͼԪ�ڵ㣬������ջ��ڲ��ڵ��˳��
	void ChangeEleNodeForAloneClse(GeomCloseHEAD*pHtemp, GeomEleNode *paimnode);
	//Ѱ��һ������dxf�У���һ��û�б���λ�ķ�ջ�ͷ���
	GeomCloseHEAD*FindNotAcceptClosedNode(NestResultDataNode*head);
	//����һ��������������һ���Ѿ�����λ�ķ�ջ��������ͼԪ������̾���
	Mindistant_EleNode*CreatMindistant_EleNode(GeomEleNode* pmindst,double m_mindstant);

public:
	int i,j,a;
////////////////////////////////////for taiji controller////////////////
public:
	//�ػ���ԭ��
	bool reback_origin_point(bool gobackorigin);
	//�궨��������ԭ��,���˹����ƻ����㶯
	bool demarcate(double x, double y, double z, double A, double B, double C);//�ж��Ƿ񵽴﹤������ϵԭ�㣬����xyz�����˹��ֶ����ڻ�����xyz�����˶�
	void RunForXaixs(double x);//�˹�����x�᷽����˶���
	void RunForYaixs(double y);//�˹�����Y�᷽����˶���
	void RunForZaixs(double z);//�˹�����Z�᷽����˶���
	void RunForAaix(double x); //�˹�����A�᷽����˶���
	void RunForBaix(double x); //�˹�����B�᷽����˶���
	void RunForCaix(double x); //�˹�����C�᷽����˶���
	double GetDemarcatePoint(double x, double y, double z, double A, double B, double C);//���湤������ϵԭ������ڻ���ԭ�������ֵ
	
	//�˶������ٶȿɵ���
	double GetSpeed(double v);//����Ǳ���API,��̫�����ƿ��ṩ
	double SetSpeed(double v);//�ѱ��صĻ�ȡ���ٶ�ֵ�������ƿ�
	double ReceiveEncodeSpeed();//��ȡ�����˶���ʵ���ٶ�ֵ
	void ChangeSpeed(double v);//����Ǳ���API,��������Ҫ�ı����ٶ�ֵ
	//�˶�����
	bool RunLine(double x0, double y0, double x1, double y1, double v);//����DXF�����ֱ�ߵ���ʼ��ֹ�㣬�������ƿ�����ָ����ƻ�����ֱ��,����xy�������ԭʼ���꣬û�о����궨��ת����vΪ�ٶ�
	bool RunArc(double Angle_start, double Angle_end, double r, double Arccent_x, double Arccent_y, double v);//����DXF�����ARC����ʼ�Ƕȣ���ֹ�Ƕȣ��뾶��Բ�����꣬�˶��ٶȣ���Ϊԭʼ���������������ƿ�����ָ����ƻ�����ARC��
	bool RunCircle( double Arccent_x, double Arccent_y,double R, double v);//����DXF�����CIRCLE��Բ�����꣬�뾶���˶��ٶȣ���Ϊԭʼ���������������ƿ�����ָ����ƻ�����circle��
	void Suspend();//�˶���ͣ
	void ReStart();//�˶�����
	//��Χ��·
	bool OpenWaterJet();//����ѹˮ��
	bool OpenSandValve();//��ɳ��
	bool OpenWaterPump();//��ˮ��



};

