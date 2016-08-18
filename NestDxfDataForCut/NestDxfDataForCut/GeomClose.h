#pragma once
#include "GEOMELE.h"
//********************************************************************//
//������ṹ��Ҫ�Լ���ͼԪ���б���������x0, y0, x1, y1�Ĺ�ϵ���ҵ���ջ���ÿ����ջ���IID������
//��ջ������ͼԪӦ������������У���ѡ������һ����ͼԪ��Ϊ��ջ������
//������֮���Ѿ�û��LINE ARC CIRCLE������ͼԪ�ˣ��е���һ������ջ���
//********************************************************************//
typedef struct//��ջ�����㣬��Ϊ��һ����γ�ȫ������·��ʱ����жϵ㡣
{
	double colse_start_x0, colse_start_y0;//����Ѱ�ҵ�ʱ��Ӧ��Ҫ�е�ԭ���ǰ�����������µ�ԭ��ȥ�ҡ�����ԭ��

}GEOMCCLOSTAPOINT;
//���������ͼԪ�����浽���ʹ֮�γ�һ������ֱ��
typedef struct//���ͼԪ֮��Ĺ�����
{
	double prev_x, prev_y, next_x, next_y;//��һ���ͼԪ��xy����һ���ͼԪ�����xy
}TranLine;
//********************************************************************//
//����ÿһ����ȡ���������ݶ�Ӧ�û���һ���ڴ������������������Ϊ˫������Ľ��
//�Ȱ����еĽ�����ͬһ�������ϣ�����ͷ���ĵ�ַ����һ��HEAD��㣬֮���ٶ���Щ��������ֲ�ͬ�ķ�ջ�
//********************************************************************//
typedef struct GeomEleNode//���е����ݽ���ʱ���ŵĽ��
{
	GeomStandData m_GeomStandData;//�������Ӧ�ñ�����ÿһ����ȡ����������
	struct GeomEleNode* prevGeomeleNode;//ָ��ǰһ��GeomeleNode���
	struct GeomEleNode* nextGeomeleNode;//ָ���һ��GeomeleNode���
	unsigned int m_NumGeomEleID;//��¼�ڼ���ͼԪ
	unsigned int m_NumGeomCloseID;//��¼�ڼ�����ջ��ġ�
	bool m_AccptGeomEleNode;//�жϸý���Ƿ��Ѿ�����¼��,��ʼ��Ϊfalse
}GeomEleNode;
//********************************************************************//
//��ÿ����ջ���˫�������ͷ��㱣���������տ�ʼ��ʱ�򱣴�����DXF��ͼԪ�Ľ�����γɵ�����˫�������ͷ���
//��Щͷ���֮���ڸ���һ�㻹���γ���ͬһ������dxfͼ�����з�ջ���ɵ�˫������
//����������Ӧ�ñ���÷�ջ���һ����㣨�յ㣩����������һ��Ľ��֮��˳���
//********************************************************************//
typedef struct GeomCloseHEAD
{
	TranLine m_tranline;//�洢����ֱ��
	GEOMCCLOSTAPOINT m_geomclose_startpoint;//�洢��ָ��˫���������ֹ�غϵ�
	unsigned int m_NumGeomele;//��������ж��ٸ�����ͼԪ
	unsigned int GEOMCLOSE_ID;//��Ϊÿһ����ջ��ı�ʶ�����и�ʱ����������ı�ʶ��Ӧ����Ϊ���ñ�־
	struct GeomEleNode*FirstGeomele;//ָ��˫������GeomEleNode��ͷ���
	struct GeomCloseHEAD*prevGeomcloseNode;//ָ��ǰһ����ջ�GeomCloseHEAD���
	struct GeomCloseHEAD*nextGeomcloseNode;//ָ���һ����ջ�GeomCloseHEAD���
	bool m_IfGeomCloseIntact;//�ж�һ����ջ��ڵ������Ƿ���������־��ͷ���������β�����յ���ͬ
	bool m_AcceptGeomcloseHEAD;//����Ժ�Ҫȫ������·���滮��ʱ����Ϊÿһ����ջ�ʱ���Ѿ����滮�ı�ʶ
}GeomCloseHEAD;
class GeomClose
{
public:
	GeomClose();
	~GeomClose();
public:
	GEOMCCLOSTAPOINT geomclose_startpoint;
	int GEOMCLOSE_ID;//��Ϊÿһ����ջ��ı�ʶ������һ��α���ʱ��ʹ��,���ø�ͷ��㣬���ϵ��ã�ͨ��Э�顣
	GEOMELE geomele;//��ΪARC ��CIRCLE��ʹ��
	//GeomStandData m_geomstandData;
public://������ջ�������ÿһ����ջ����������
	//����ÿһ��ͼԪ��ID����һ�����ݣ��ȶ�ÿһ�����ֵ�ͼԪ�ֱ����ò�ͬ��IID��
	//�������ĳ��ͼԪ����������IID�й�ϵ����ô�Ͱ�������IID�ϲ�����ЩͼԪ����ͬIID��ʱ�����ͬһ��˫��������
	//���GEOMCLOSE_ID��ͬʱ����Ҳ��GEOMCLOSE_ID���õ�GEOMELEԤ���ı�־λ��
	int FindGeomClose(int ID);//����LINE ARC CIRCLE ��ID�����벻ͬ�ķ�ջ�ID
	int MergerGeomClose(int ID1, int ID2);//������ͬһ��ͼԪ�������˵���ص�ID,�ں���ЩID������һ�������ϲ���������һ��ID��β���ָ����һ��ID��ͷ��㣩
	GeomCloseHEAD*SetGeomeleOrder(GeomCloseHEAD*head);//����ÿһ����ջ���ͷ��㣬ʹ֮�ڲ���geomele�ų���
	void SetGeomCloseID(int GEOMCLOSE_ID);//����HEADͷ��㴦��ID.
	GEOMCCLOSTAPOINT SetGeomCloseStartpoint(int IID);//����ÿһ����ջ���IID�����ÿ����ջ�����ʼ��,���������Ҫ���뵽ͷ�������ȥ��
	GEOMCCLOSTAPOINT TranForCircle(GEOMELE geomele);//�жϵ���CIRCLE��ʱ������ ��ջ��Ĳ��������ת�����circle���������µ��Ǹ������Ϊ��㡣
public:
	//�洢���ݵ�˫�������ʼ��
	//CreatGeomEleNode������������������,GeomStandData m_geomstandData���������û�������ﶨ��
	GeomEleNode*CreatGeomEleNode(GeomStandData m_geomstandData);//��ʼ���������ʱ�����Ѿ��������ݵģ��������Ҫ�����ݴ���������һ���̶����ڴ�����
	GeomCloseHEAD*CreatGeomCloseHEAD(int GEOMCLOSE_ID);//��һ���ڴ������Ϊͷָ�����Ż���ͼԪ˫�������ͷ���
	GeomCloseHEAD*InsertGeomEleNode(GeomCloseHEAD*head, GeomEleNode*node, GeomStandData m_geomstandData);//��һ��������˵����������ڶ���������˵������ʲô��
public:
	//�������ݽ��֮��Ĺ�ϵ
	
	GeomEleNode*FindRelatGmElNd_xyStart(GeomCloseHEAD*head, GeomStandData m_geomstandData);//�ж��½�����������ԭ�����Ƿ���������ͬ
	GeomEleNode*FindRelatGmElNd_xyEnd(GeomCloseHEAD*head, GeomStandData m_geomstandData);//�ж��½�����������ԭ�����Ƿ���ֹֹ��ͬ
	GeomEleNode*FindRelatGmElNd_xySEnd(GeomCloseHEAD*head, GeomStandData m_geomstandData);//�ж��½��������ݵ������ԭ���ݵ��յ��Ƿ���ͬ
	GeomEleNode*FindRelatGmElNd_xyEStart(GeomCloseHEAD*head, GeomStandData m_geomstandData);//�ж��½��������ݵ��յ���ԭ���ݵ�����Ƿ���ͬ
	GeomStandData DataSwap(GeomStandData m_geomstandData);//�������յ�����ݵ���
	
};

