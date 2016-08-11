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

typedef struct SORTGEOM//保存每一个封闭环的结构，主要是让封闭环里面的每一个图元有序
{
	GEOMELE geomele;//输入每个基本图元的数据，存储
	struct SORTGEOM*prorNode;
	struct SORTGEOM*nextNode;
}SORTGEOM;
typedef struct HEAD
{
	GEOMCCLOSTAPOINT geomclose_startpoint;
	int GEOMCLOSE_ID;//作为每一个封闭环的标识，在切割时候与类里面的标识对应，作为调用标志
	struct SORTGEOM*first;//指向双向链表的头结点
	bool flag;//如果以后要全局最优路径规划的时候，最为每一个封闭环时候已经被规划的标识
}HEAD;
class GeomClose
{
public:
	GeomClose();
	~GeomClose();
public:
	GEOMCCLOSTAPOINT geomclose_startpoint;
	int GEOMCLOSE_ID;//作为每一个封闭环的标识，在下一层次遍历时候使用,设置给头结点，联合调用，通信协议。
	GEOMELE geomele;//作为ARC 和CIRCLE的使用
public://搜索封闭环，设置每一个封闭环的起点坐标
	//输入每一个图元的ID遍历一遍数据，先对每一个出现的图元分别设置不同的IID，
	//如果出现某个图元与其中两个IID有关系，那么就把这两个IID合并，这些图元有相同IID的时候放入同一个双向链表中
	//输出GEOMCLOSE_ID，同时里面也把GEOMCLOSE_ID设置到GEOMELE预留的标志位了
	int FindGeomClose(int ID);//输入LINE ARC CIRCLE 的ID，输入不同的封闭环ID
	int MergerGeomClose(int ID1, int ID2);//输入与同一个图元的两个端点相关的ID,融合这些ID，减少一个，并合并。（其中一个ID的尾结点指向另一个ID的头结点）
	HEAD*SetGeomeleOrder(HEAD*head);//输入每一个封闭环的头结点，使之内部的geomele排成序
	void SetGeomCloseID(int GEOMCLOSE_ID);//设置HEAD头结点处的ID.
	GEOMCCLOSTAPOINT SetGeomCloseStartpoint(int IID);//输入每一个封闭环的IID，输出每个封闭环的起始点,最终这个就要输入到头结点里面去。
	GEOMCCLOSTAPOINT TranForCircle(GEOMELE geomele);//判断到是CIRCLE的时候，输入 封闭环的参数，输出转换后的circle中最左最下的那个坐标点为起点。
};

