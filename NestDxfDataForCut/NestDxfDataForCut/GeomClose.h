#pragma once
#include "GEOMELE.h"
//********************************************************************//
//第三层结构，要对几何图元进行遍历，按照x0, y0, x1, y1的关系来找到封闭环，每个封闭环用IID来区分
//封闭环里面的图元应该是有序的排列，并选择其中一个套图元作为封闭环的起点
//在这里之后已经没有LINE ARC CIRCLE这样的图元了，有的是一个个封闭环。
//********************************************************************//
typedef struct//封闭环的起点，作为下一层次形成全局搜索路径时候的判断点。
{
	double colse_start_x0, colse_start_y0;//这里寻找的时候应该要有的原则是按照最左和最下的原则去找。靠近原点

}GEOMCCLOSTAPOINT;
//将两个封闭图元的起点存到这里，使之形成一条过渡直线
typedef struct//封闭图元之间的过渡线
{
	double prev_x, prev_y, next_x, next_y;//上一封闭图元的xy和下一封闭图元的起点xy
}TranLine;
//********************************************************************//
//对于每一个读取进来的数据都应该划分一块内存来存放它，这个结点作为双向链表的结点
//先把所有的结点挂在同一个链表上，并把头结点的地址传给一个HEAD结点，之后再对这些结点来划分不同的封闭环
//********************************************************************//
typedef struct GeomEleNode//所有的数据进来时候存放的结点
{
	GeomStandData m_GeomStandData;//结点里面应该保存了每一个读取进来的数据
	struct GeomEleNode* prevGeomeleNode;//指向前一个GeomeleNode结点
	struct GeomEleNode* nextGeomeleNode;//指向后一个GeomeleNode结点
	unsigned int m_NumGeomEleID;//记录第几个图元
	unsigned int m_NumGeomCloseID;//记录第几个封闭环的。
	bool m_AccptGeomEleNode;//判断该结点是否已经被收录的,初始化为false
}GeomEleNode;
//********************************************************************//
//将每个封闭环的双向链表的头结点保存起来。刚开始的时候保存整个DXF的图元的结点所形成的无序双向链表的头结点
//这些头结点之间在更上一层还会形成以同一个排样dxf图形所有封闭环组成的双向链表，
//所以这个结点应该保存该封闭环的一个起点（终点），用做更上一层的结点之间顺序和
//********************************************************************//
typedef struct GeomCloseHEAD
{
	TranLine m_tranline;//存储过渡直线
	GEOMCCLOSTAPOINT m_geomclose_startpoint;//存储所指向双向链表的起止重合点
	unsigned int m_NumGeomele;//该链表存有多少个基本图元
	unsigned int GEOMCLOSE_ID;//作为每一个封闭环的标识，在切割时候与类里面的标识对应，作为调用标志
	struct GeomEleNode*FirstGeomele;//指向双向链表GeomEleNode的头结点
	struct GeomCloseHEAD*prevGeomcloseNode;//指向前一个封闭环GeomCloseHEAD结点
	struct GeomCloseHEAD*nextGeomcloseNode;//指向后一个封闭环GeomCloseHEAD结点
	bool m_IfGeomCloseIntact;//判断一个封闭环内的数据是否完整，标志是头结点的起点与尾结点的终点相同
	bool m_AcceptGeomcloseHEAD;//如果以后要全局最优路径规划的时候，最为每一个封闭环时候已经被规划的标识
}GeomCloseHEAD;
class GeomClose
{
public:
	GeomClose();
	~GeomClose();
public:
	GEOMCCLOSTAPOINT geomclose_startpoint;
	int GEOMCLOSE_ID;//作为每一个封闭环的标识，在下一层次遍历时候使用,设置给头结点，联合调用，通信协议。
	GEOMELE geomele;//作为ARC 和CIRCLE的使用
	//GeomStandData m_geomstandData;
public://搜索封闭环，设置每一个封闭环的起点坐标
	//输入每一个图元的ID遍历一遍数据，先对每一个出现的图元分别设置不同的IID，
	//如果出现某个图元与其中两个IID有关系，那么就把这两个IID合并，这些图元有相同IID的时候放入同一个双向链表中
	//输出GEOMCLOSE_ID，同时里面也把GEOMCLOSE_ID设置到GEOMELE预留的标志位了
	int FindGeomClose(int ID);//输入LINE ARC CIRCLE 的ID，输入不同的封闭环ID
	int MergerGeomClose(int ID1, int ID2);//输入与同一个图元的两个端点相关的ID,融合这些ID，减少一个，并合并。（其中一个ID的尾结点指向另一个ID的头结点）
	GeomCloseHEAD*SetGeomeleOrder(GeomCloseHEAD*head);//输入每一个封闭环的头结点，使之内部的geomele排成序
	void SetGeomCloseID(int GEOMCLOSE_ID);//设置HEAD头结点处的ID.
	GEOMCCLOSTAPOINT SetGeomCloseStartpoint(int IID);//输入每一个封闭环的IID，输出每个封闭环的起始点,最终这个就要输入到头结点里面去。
	GEOMCCLOSTAPOINT TranForCircle(GEOMELE geomele);//判断到是CIRCLE的时候，输入 封闭环的参数，输出转换后的circle中最左最下的那个坐标点为起点。
public:
	//存储数据的双向链表初始化
	//CreatGeomEleNode这个函数可能有问题的,GeomStandData m_geomstandData这个参数并没有在这里定义
	GeomEleNode*CreatGeomEleNode(GeomStandData m_geomstandData);//初始化这个结点的时候，是已经有了数据的，这里就是要把数据传进来放在一个固定的内存里面
	GeomCloseHEAD*CreatGeomCloseHEAD(int GEOMCLOSE_ID);//划一块内存出来作为头指向结点存放基础图元双向链表的头结点
	GeomCloseHEAD*InsertGeomEleNode(GeomCloseHEAD*head, GeomEleNode*node, GeomStandData m_geomstandData);//第一个参数是说明挂在哪里，第二个参数是说明，挂什么。
public:
	//需找数据结点之间的关系
	
	GeomEleNode*FindRelatGmElNd_xyStart(GeomCloseHEAD*head, GeomStandData m_geomstandData);//判断新进来的数据与原数据是否有起起相同
	GeomEleNode*FindRelatGmElNd_xyEnd(GeomCloseHEAD*head, GeomStandData m_geomstandData);//判断新进来的数据与原数据是否有止止相同
	GeomEleNode*FindRelatGmElNd_xySEnd(GeomCloseHEAD*head, GeomStandData m_geomstandData);//判断新进来的数据的起点与原数据的终点是否相同
	GeomEleNode*FindRelatGmElNd_xyEStart(GeomCloseHEAD*head, GeomStandData m_geomstandData);//判断新进来的数据的终点与原数据的起点是否相同
	GeomStandData DataSwap(GeomStandData m_geomstandData);//将起点和终点的数据调换
	
};

