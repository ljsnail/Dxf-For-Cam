#pragma once
#include"GeomClose.h"
#include"CutLeadLine.h"
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
////��������ֱ�߽�������ݽṹ,����ֱ��Ҫôû�н��㣬Ҫôֻ��һ������
//typedef struct
//{
//	double x;
//	double y;
//}Point;
//�������һ��ֱ�ߵ�����������ݽṹ
typedef struct
{
	Point p1, p2;
	double a, b, c;
}Line_Cross;
typedef struct
{
	bool m_IfCloseNest;
	GeomCloseHEAD*KidCloseHead;
	GeomCloseHEAD*NextCloseHead;
}Geom2CloseHeadNest;
////����Ϊ�и���������ص����ݽṹ
////�и������ߵ����ݽṹ
//struct Line_point
//{
//	double x0, x1;//�и������ߵ���ʼ��
//	double y0, y1;//�и������ߵ���ֹ�㣬Ҳ����ԭ����ջ����׽���һ����
//};
//�и�������ͼԪ���ݽṹ
//�и�������Ϊֱ��
typedef struct
{
	Line_para m_cutline;
	GeomEleNode*prevGelenode;//ָ��ǰһ��ͼԪ�ڵ㣬��������и������ߣ���������ֻ����
	GeomEleNode*nextGelenode;//ָ���һ����ջ�GeomCloseHEAD���,��Ϊ�����������ߣ���������ֻ��ָ���ջ��ĵ�һ��ͷ���
	bool if_CutGuideLine;//��Ϊ�и������ߵ��жϣ�Ĭ��Ϊture
}CutGuideLine;
////����һ�����ݽṹ������ȥ����Ŀ�������ͼֽͷ�ļ��Ͱ�Ŀ��ջ�ͷ�ļ�
////�ڽ����и�·���滮�����������ǰ��Ԥ����
//typedef struct
//{
//	GeomCloseHEAD*SheetMate;
//	NestResultDataNode*head;
//}Pretreat;
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
	//Ϊ�˱ܿ�С�Ĳ����и������ʹ��ײ��ˮ��ͷ��Ҫ�������С�Ĵ�ש���и��ΪԶ��ԭ��һ�Σ��պ��������෴��
	void ChangeEleNode_Avoid_Impact(NestResultDataNode*head);

	//�����µ��㷨��������������������һ����������
	//���ڽ���������̰���㷨
	//һ������ȫ�����ŵĽ���_̰���㷨,�����ջ�ͷ��㣬��ʱһ�����ε�����ͼԪ���Ѿ����������ˡ�
	void BaseTS_GR_ForCutPathPlan(NestResultDataNode*head);

	//Ѱ���������֮�����������Ǹ����ɽڵ�
	Mindistant_EleNode *FindMinDstGeomeleNode(NestResultDataNode*head, GeomCloseHEAD*pGMCLSHead);//����û����λ�ĵ�һ����ջ�ͷ��㣬�������һ���Ѿ���λ�ķ�ջ�ͷ���֮�����̾���Ĺ��ɽڵ�
	//����һ����ջ���ͷ����Ŀ��ͼԪ�ڵ㣬������ջ��ڲ��ڵ��˳��
	void ChangeEleNodeForAloneClse(GeomCloseHEAD*pHtemp, GeomEleNode *paimnode);
	//Ѱ��һ������dxf�У���һ��û�б���λ�ķ�ջ�ͷ���
	GeomCloseHEAD*FindNotAcceptClosedNode(NestResultDataNode*head);
	//����һ��������������һ���Ѿ�����λ�ķ�ջ��������ͼԪ������̾���
	Mindistant_EleNode*CreatMindistant_EleNode(GeomEleNode* pmindst,double m_mindstant);
	//��һ����ջ��ǲ�Ҫ�������ݶ�ȡ���и��,���ڶ�����ջ���Ϊ��һ����ջ���
	NestResultDataNode*ChangeSencondCH2FH(NestResultDataNode*head);

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
	public:
	//��ӹ���
	//Ѱ��Ƕ�׵ķ�ջ�����һ�����ݵļ��룬ʹ������·���滮�ô�����
	//����һ���������ֱ�����ݵ���ڴ�
	//���������������ͼͷ��㣬�ҵ�����Ƕ�׵ķ�ջ���Ȼ����ڻ��ҵ���ջ����ӻ���
	void Find_AdjustNestCloseHead(NestResultDataNode*head);
	//�жϵڶ�����ջ��ڲ��ڵ�һ����ջ�����
	Geom2CloseHeadNest EstimateCloseHeadNest(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp);
	//�󵥸���ջ�������ε�xy��ֵ
	void GetLimtofGeomClosed(GeomCloseHEAD*pHtemp);
	//�����ջ�������
	Envelope_Rect m_GemoClosedLimt;
	//�ð���ķ�ʽ�жϺ���ķ�ջ���ǰ��ķ�ջ��Ƿ���Ƕ�׹�ϵ
	Geom2CloseHeadNest JudgeCloseHeadNest(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp);
	//������λС��
	double Retain4Decimals(double a);
	Point m_PointCross;
	Geom2CloseHeadNest m_G2CloseHeadNest;
	Line_Cross m_line1Point, m_line2Point;
	//����ֱ�ߵ��������꣬��ֱ�߻������̵Ĳ���
	void GetLinePara(Line_Cross *line);
	//������ֱ�ߵĽ���
	Point  GetCrossPoint(Line_Cross *m_line1Point, Line_Cross *m_line2Point);
	//�������Ѿ�֪�����ӷ�ջ��ҵ���һ����ջ���,
	//����������������Ϊ����ջ�ͷ����Ѿ���Ϊ�ӷ�ջ�������������ջ�ʱ����Ӧ�ð��и�ͼͷ���ҲҪ���任����һ����ջ��Ϲ�
	void SetInSideClose(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp,NestResultDataNode*head);
	//�����ӷ�ջ������ڵ㣬�ı丸��ջ���ͼԪͷ��㣨��׵㣩
	void ChangeOutsideGeomEleNode(GeomCloseHEAD*pKidCloseHead, GeomCloseHEAD*pFadClosedHead);
	
	//�����һ�㸸��ջ���ͼԪͷ��㣬�ı���һ����һ�㸸��ջ����ӷ�ջ���˳��,�������һ���ӷ�ջ����
	GeomCloseHEAD*ChangeINsideGeomEleNode(GeomCloseHEAD*pFadClosedHead, GeomCloseHEAD*pNKidCloseHead);

	//����Ⱥ�㷨��������һ��ķ�ջ�˳����һ��
	void Base_ACO_ForFirstCHead(NestResultDataNode*head);
	//������Ⱥ�㷨�ó��ĵ�һ���ջ���˳��
	//������ͬ��һ���ջ����ӷ�ջ���˳��
	//���������ӷ�ջ���Ӧ�ĸ���ջ��Ĵ�׵�
	void BaseTS_GR_ForKidCHead(NestResultDataNode*head);
	//�жϸ÷�ջ��Ƿ����ӷ�ջ�������У���ȡ���ķ�ջ�
	GeomCloseHEAD*GetTheLastKidCH(GeomCloseHEAD*pCHead);
	//����һ��ķ�ջ�ͷ��㣬Ȼ���˫���������ջ�����̰���㷨���򣬲�������ķ�ջ��ڵ�
	GeomCloseHEAD*ChangeGeomCHead(GeomCloseHEAD*pNKidCloseHead);
	//Ѱ����һ����һ���ջ�
	GeomCloseHEAD*Find_Next_TheFirstLevel(GeomCloseHEAD*pFirstClosedHead);
	//////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	//����������и������ߵ���ش��루�и��ש�Ͳ����ã����и����֮������ã�
	GeomCloseHEAD*m_ceramic_Head;//�����ש������ͷ�ļ�
	//����ͷ�ļ�������и������ߣ��������Ѿ�ȷ�����и���Ƶ�֮��Ҳ�����������̰���㷨������Ƕ��ʶ���㷨�������и���Ƶ�֮��
	void Add_CutGuideLine(NestResultDataNode*head);
	CutGuideLine*CreatCutGuideLine(GeomCloseHEAD*Phead); //�����и������ߣ��и��ջ���ͷ���
	CutGuideLine*CreatCutGuideLine_Circle(GeomEleNode*Pnode,bool m_bilayer); //��Բ���и�������
	CutGuideLine*CreatCutGuideLine_Polygon(GeomEleNode*Pnode, bool m_bilayer); //�����Σ���Բ�����и�������
	double CalculateSlope(GeomEleNode*pGnode);//����һ������ͼԪ���ݽṹ���������ͼԪ��βͼԪֱ�ߵ�б��
	//�������и������ߵķ�ʽ���ڸ�����
	//���һ�ֻ��ڷ�ջ�֮����߼���θ���Ĳ���ȥ���и�����������
	//���Ǹ��ݷ�ջ�ʶ��֮����߼���ϵ�����и�˳�򣬽����и������ߵ����
	void CreatCutGuideLINE(GeomCloseHEAD*Phead);//����ÿһ����ջ���Ȼ���������и� �����ߵ���ӡ�
	bool IfIncludeKidClose(GeomCloseHEAD*Phead);//����һ����ջ����ж��Ƿ����ӷ�ջ�
	void Add_KidCloseCutLine(GeomCloseHEAD*Phead);//����һ�������ӷ�ջ��ķ�ջ�����������и�������
	//����CutLeadLine�����
	CutLeadLine m_CutLeadLine;
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////�и������߸����жϣ������/////////////////////////
	///////////////////////////���Ĵ��룬���Ĵ��룬���Ĵ���,��һƪ�Ĺ���////////
	//�����и�ͼͷ��㣬�ѷ�ջ����и������߶�����һ�顣
	//NestResultDataNode*CheckCutGuideLINE()

};

