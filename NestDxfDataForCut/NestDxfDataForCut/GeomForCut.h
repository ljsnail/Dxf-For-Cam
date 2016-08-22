#pragma once
#include"GeomClose.h"
//这个类主要是实现把之前的封闭环之间串起来切割使用。
//实现，将封闭环之间排序，对圆这种特殊图形要另外处理，对于圆弧要转换成原来的数据形式
//包含封闭图元之间的过渡直线
//弄个单向链表来存储着一张排样dxf的数据

typedef struct NestResultDataNode//存储所有的图元的单向链表。
{
	
	struct GeomCloseHEAD*FirstGeomClose;//指向封闭环GeomCloseHEAD的头结点
	struct NestResultDataNode*prevNestResultDataNode;//指向前一个排样图数据结果NestResultDataNode的地址，在不同的排样结果图之间调用
	struct NestResultDataNode*nextNestResultDataNode;//指向前一个排样图数据结果NestResultDataNode的地址，在不同的排样结果图之间调用
	unsigned int NestResult_ID;//系统存储的第几张排样结果图，初始化为0。
	unsigned int m_NumGeomClose;//该双向链表存储的封闭环的个数
	bool AcceptNestResultflag;//是否已经被调用，初始化为false,目的不同排样结果图案中调用。
}NestResultDataNode;
//存储生产批次，一次开机就划出一块内存保存指向了这批生产加工的n张排样结果图，和每张排样结果图里面的n个封闭图形，和每个封闭图形里面的n个基本单元LINE,ARC,CIRCLE
typedef struct BatchHEAD
{
	unsigned int BatchHEAD_ID;
	unsigned int m_NumNestrsltDtNode;//这个批次有多少张排样结果图
	struct NestResultDataNode*FirstNestRsltNode;//指向第一张加工的排样图头结点
}BatchHEAD;
class GeomForCut
{
public:
	GeomForCut();
	~GeomForCut();
public:
	GeomClose m_geomclose;
	NestResultDataNode m_nestrsltNode;
public:
	
	BatchHEAD*CreatBatchHEAD(int BatchHEAD_ID);//初始化一块内存为头结点存放，无需参数传入
	NestResultDataNode*CreatNestResultNode(int NestResult_ID);//初始化一块内存来存放链表结点，无需参数传入
	//先无规律的把结点挂上
	NestResultDataNode*AddGeomCloseHeadNode(NestResultDataNode*head,GeomCloseHEAD*node);//输入每一个闭环的F头结点，把他们全部挂在一条链上。
	BatchHEAD*AddNestRsltDtNode(BatchHEAD*head, NestResultDataNode*node);//把每一张排样图的地址挂到以BatchHEAD为头结点的双向链表中。
		//***************************************************************************************//
		/**///把无规律的GeomEleNode找出其规律，转化成有规律的几个封闭环。
		/**///*************************************************************************//
		/**///步骤是先分封闭环，再在每一个封闭环里面对数据进行排序。
		/**///*************************************************************************//
		/**///void SepDiffGeomCls(GeomCloseHEAD*head);//输入指向收录整张排样图全部基本图元的双向链表的F头结点
		//***************************************************************************************//

	//查找数据结点双向链表里面不是同一个封闭环的结点
	GeomEleNode*FindDiffGeomCloseNode(NestResultDataNode*head);//输入排样结果图的头结点
	//将一个双向链表上的不同封闭环挂到不同的链表头指向的双向链表中去。
	void InsertGeomCloseHEAD(NestResultDataNode*head, GeomEleNode*node);//输入新的不同封闭环ID的结点，产生新F头结点，把同时F头结点指向node。
	//对已经挂到不同封闭环双向链表结点的数据结点查看其是否是一个完整的封闭环
	GeomCloseHEAD*JudgeGeomCloseIntact(NestResultDataNode*head);//输入排样结果图dxf头结点，一个个去排查封闭环结点下的数据是否完整
	//将不完整的头结点输入进来，寻求配对的其他部分;直到所有的封闭环都被置位为完整。
	NestResultDataNode*Find_Change_GeomCloseHEAD(NestResultDataNode*head, GeomCloseHEAD*node);
	//将与之配对的头结点输进来，调整
	NestResultDataNode*ChangeGeomCloseHEAD(NestResultDataNode*head, GeomCloseHEAD*node);
	//以上已经把原始数据双向链表上的不同封闭环挂到不同的封闭环头结点上了，但是还涉及单一封闭环内部的数据如何处理，和各个封闭环之间的顺序确定
	//对于单一封闭环内部而言，查找最左最下的那个起点所在的结点为头结点，然后改变封闭环内部顺序
	void ChangeEleNodeOfGeomClosed(NestResultDataNode*head);//输入排样结果图头结点，把其内部所有的封闭环里面的基本数据结点按照最左最下的结点为头结点的原则重新排序，并把头结点的起始点放到F头结点中
	
};

