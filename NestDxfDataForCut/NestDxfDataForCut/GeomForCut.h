#pragma once
#include"GeomClose.h"
#include"CutLeadLine.h"

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
//用在禁忌搜索和贪婪算法上的数据结构
//用来保存最短距离和封闭环内图元节点的数据结构
typedef struct 
{
	GeomCloseHEAD*pminhead;
	GeomEleNode *pminnode;
	double m_mindistant;
}Mindistant_EleNode;
////保存两条直线交点的数据结构,两条直线要么没有交点，要么只有一个交点
//typedef struct
//{
//	double x;
//	double y;
//}Point;
//保存决定一条直线的两个点的数据结构
typedef struct
{
	Point p1, p2;
	double a, b, c;
}Line_Cross;
typedef struct
{
	bool m_IfCloseNest;
	GeomCloseHEAD*FadCloseHead;
	GeomCloseHEAD*KidCloseHead;
	GeomCloseHEAD*NextCloseHead;
	int type ;//这是判断两者谁是谁嵌套封闭环的标识，如果是后者是前者的封闭环那么type为1，否则为2，默认为1.
}Geom2CloseHeadNest;
////以下为切割引导性相关的数据结构
////切割引刀线的数据结构
//struct Line_point
//{
//	double x0, x1;//切割引刀线的起始点
//	double y0, y1;//切割引刀线的终止点，也就是原来封闭环的首结点的一个点
//};
//切割引刀线图元数据结构
//切割引刀线为直线
typedef struct
{
	Line_para m_cutline;
	GeomEleNode*prevGelenode;//指向前一个图元节点，但这里的切割引刀线，所以这里只能是
	GeomEleNode*nextGelenode;//指向后一个封闭环GeomCloseHEAD结点,因为这里是引刀线，所以这里只能指向封闭环的第一个头结点
	bool if_CutGuideLine;//作为切割引刀线的判断，默认为ture
}CutGuideLine;
//创建一个返回两个点的数据结构，即是求圆弧圆心的内外区域坐标
typedef struct
{
	Point P_arc_out;//外部区域的圆心
	Point P_arc_in;//内部区域的圆心
	bool If_select;//记录是否已经选择到了，默认是false，如果选择到了则是true.
	int m_belong;//记录圆弧的切点处于封闭环的哪一块区域，以x_min点为A,y_min点为B，x_max点为C,y_max点为D;其中AB之间为1，BC为2，CD为3，DA为4.
}Arc_Point;
//切割引导线的数据结构
//包含了圆弧与直线等类型的切割引导线
typedef struct
{
	GLINE m_line_in,m_line_out;
	GARC m_arc_in,m_arc_out;
	Point P_AuxiStart,P_AuxiEnd;//切割引导线的起点与终点，是留给指引过渡线用的，也为将来判断切割引导线干涉提供了判断的依据
	bool m_IfLine;//切割引导线类型，true默认是直线型，false为圆弧。
}AuxiliarPath;
////创建一个数据结构，保存去掉板材框后的排样图纸头文件和板材框封闭环头文件
////在进行切割路径规划和引刀线添加前的预处理
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
	void ChangeEleNodeOfGeomClosed_origin(NestResultDataNode*head);//输入排样图头结点，按照每个封闭环内部数据到00的最短距离寻找头结点，并把这个头结点给以指向封闭环头结点的F节点里。
	//经过上面之后单一封闭环内部的数据调整已经完毕，现在每一个封闭环里的头结点都是离远点最近的那个点。
	//确定封闭环顺序之后再确定一遍封闭环内部的头结点，前面先按照到机床原点来确定封闭环的起始数据点
	void ChangClosedNodeOfNRDXF(NestResultDataNode*head);
	//经过上面之后，排样结果内的封闭环已经调整好顺序，那么为了使空行程更短，这次应该根据到上一个封闭环最短距离来设置每个封闭环内部的头结点
	void ChangeEleNodeOfGeomClosed_order(NestResultDataNode*head);//输入排样结果图头结点，按照已经排好的封闭环
	//为了避开小的部件切割后跳起使得撞刀水刀头，要把面积过小的瓷砖的切割点为远离原点一段，刚好与上面相反。
	void ChangeEleNode_Avoid_Impact(NestResultDataNode*head);

	//开辟新的算法，将以上三个函数合在一个函数里面
	//基于禁忌搜索的贪婪算法
	//一种面向全局最优的禁忌_贪婪算法,输入封闭环头结点，此时一个批次的所有图元都已经在这里面了。
	void BaseTS_GR_ForCutPathPlan(NestResultDataNode*head);

	//寻找两个封闭之间距离最近的那个过渡节点
	Mindistant_EleNode *FindMinDstGeomeleNode(NestResultDataNode*head, GeomCloseHEAD*pGMCLSHead);//输入没有置位的第一个封闭环头结点，输出与上一个已经置位的封闭环头结点之间的最短距离的过渡节点
	//输入一个封闭环的头结点和目标图元节点，调整封闭环内部节点的顺序
	void ChangeEleNodeForAloneClse(GeomCloseHEAD*pHtemp, GeomEleNode *paimnode);
	//寻找一个排样dxf中，第一个没有被置位的封闭环头结点
	GeomCloseHEAD*FindNotAcceptClosedNode(NestResultDataNode*head);
	//创造一个用来存放离最后一个已经被置位的封闭环最近的内图元结点和最短距离
	Mindistant_EleNode*CreatMindistant_EleNode(GeomEleNode* pmindst,double m_mindstant);
	//第一个封闭环是不要进行数据读取和切割的,将第二个封闭环作为第一个封闭环。
	NestResultDataNode*ChangeSencondCH2FH(NestResultDataNode*head);

public:

	int i,j,a;
	public:
	//添加工艺第一篇EI的工作
	//寻找嵌套的封闭环，这一个内容的加入，使得整个路径规划得从来。
	//创建一块用来存放直线数据点的内存
	//输入整个排样结果图头结点，找到其中嵌套的封闭环，然后把内环挂到封闭环的子环下
	void Find_AdjustNestCloseHead(NestResultDataNode*head);
	//判断第二个封闭环在不在第一个封闭环里面
	Geom2CloseHeadNest EstimateCloseHeadNest(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp);
	//求所有封闭环的包络矩形的控制角点
	void GetLimtofGeomClosed(NestResultDataNode*head);
	//求单个封闭环包络矩形的xy最值
	void GetLimtofGeomClosed(GeomCloseHEAD*pHtemp);
	//求所有封闭环的区域属性，即在板材的哪一块区域里。 
	void GetRigionOfGeomClosed(NestResultDataNode*head);
	//包络封闭环的数据
	Envelope_Rect m_GemoClosedLimt;
	//用包络的方式判断后面的封闭环和前面的封闭环是否有嵌套关系
	Geom2CloseHeadNest JudgeCloseHeadNest(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp);
	//用包络的方法判断后面的封闭环（已经确定是最外层父封闭环的子封闭环的情况下）与现有父封闭环子封闭环的嵌套关系。
	//这里与上面不同的地方在于，上面是只判断后者是否为前者的子封闭环的情况，而这里是判断两者之间的嵌套关系。
	Geom2CloseHeadNest Judge2KidCloseHNest(GeomCloseHEAD*pHKtemp, GeomCloseHEAD*pHNtemp);
	//输入子封闭环，然后改变该子封闭环下的所有封闭环的奇偶性，用在封闭环1是封闭环2的子封闭环这种情况下。
	void Change_CloseHSinglelayer(GeomCloseHEAD*pHKtemp);
	//转换单一封闭环的奇偶层性质，不管其他。
	void Change_singlelayer(GeomCloseHEAD*phktemp);
	//保留四位小数
	double Retain4Decimals(double a);
	Point m_PointCross;
	Geom2CloseHeadNest m_G2CloseHeadNest;
	Line_Cross m_line1Point, m_line2Point;
	//输入直线的两个坐标，求直线基本方程的参数
	void GetLinePara(Line_Cross *line);
	//求两个直线的交点
	Point  GetCrossPoint(Line_Cross *m_line1Point, Line_Cross *m_line2Point);
	//把现在已经知道的子封闭环挂到另一个封闭环上,
	//第三个参数则是因为当封闭环头结点已经作为子封闭环挂在了其他封闭环时候，则应该把切割图头结点也要将其换另外一个封闭环上挂
	//输入三个参数，第一个参数为原来假定是父封闭环的封闭环，第二个参数为将两个封闭环判别后进行区分开的父封闭环与子封闭环，第三参数为切割图头结点。
	void SetInSideClose(GeomCloseHEAD*pHtemp, Geom2CloseHeadNest m_G2CloseHeadNest, NestResultDataNode*head);
	//清除识别到有嵌套关系的封闭环的原来封闭环双向链表中的关系，让它干净地进入新的封闭环双向链表中。
	//返回清除了原先关系的子封闭环头结点指针
	void ClearCloseHeadOldRela(GeomCloseHEAD*pHtemp, GeomCloseHEAD*pHNtemp, NestResultDataNode*head);
	//输入子封闭环的最后节点，改变父封闭环的图元头结点（打孔点）
	void ChangeOutsideGeomEleNode(GeomCloseHEAD*pKidCloseHead, GeomCloseHEAD*pFadClosedHead);
	
	//输入第一层父封闭环的图元头结点，改变下一个第一层父封闭环的子封闭环的顺序,并把最后一个子封闭环输出
	GeomCloseHEAD*ChangeINsideGeomEleNode(GeomCloseHEAD*pFadClosedHead, GeomCloseHEAD*pNKidCloseHead);

	//用蚁群算法把整个第一层的封闭环顺序处理一遍
	void Base_ACO_ForFirstCHead(NestResultDataNode*head);
	//根据蚁群算法得出的第一层封闭环的顺序，
	//调整不同第一层封闭环里子封闭环的顺序，
	//并调整与子封闭环对应的父封闭环的打孔点
	void BaseTS_GR_ForKidCHead(NestResultDataNode*head);
	//判断该封闭环是否有子封闭环，如果有，获取最后的封闭环
	GeomCloseHEAD*GetTheLastKidCH(GeomCloseHEAD*pCHead);
	//输入一层的封闭环头结点，然后对双向链表里封闭环进行贪婪算法排序，并输出最后的封闭环节点
	GeomCloseHEAD*ChangeGeomCHead(GeomCloseHEAD*pNKidCloseHead);
	//寻找下一个第一层封闭环
	GeomCloseHEAD*Find_Next_TheFirstLevel(GeomCloseHEAD*pFirstClosedHead);
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//以下是求解切割引刀线的相关代码（切割瓷砖和玻璃用，而切割钢铁之类的则不用）
	///////////////////////////切割引刀线之板材语义信息，引刀线生成，干涉判断，与调整/////////////////////////
	GeomCloseHEAD*m_ceramic_Head;//保存瓷砖部件的头文件
	//输入平面切割图头文件，将现在封闭环数据结构中的每一个封闭环的奇偶属性确定下来，为下面添加封闭环的切割引刀线作为判断的准则。
	void JudgeClosedHead_Odd_even(NestResultDataNode*head);
	//输入每一个封闭环，写入其奇偶性的标准
	void WriteClosedHead_Odd_even(GeomCloseHEAD*pClosedH);
	//输入每一个父封闭环，改写其与子封闭环的奇偶性
	void KidClosedHead_Odd_even(GeomCloseHEAD*pFtemp);
	//输入头文件，添加切割引刀线，这是在已经确定了切割控制点之后，也就是在完成了贪婪算法，多重嵌套识别算法，调节切割控制点之后
	void Add_CutGuideLine(NestResultDataNode*head, bool if_LineAuxiliary);
	CutGuideLine*CreatCutGuideLine_Circle(GeomEleNode*Pnode,bool m_bilayer); //求圆的切割引刀线
	double CalculateSlope(GeomEleNode*pGnode);//输入一个基本图元数据结构，求解其首图元和尾图元直线的斜率
	//以上求切割引刀线的方式过于复杂了
	//提出一种基于封闭环之间的逻辑层次感念的材料去留切割引刀线问题
	//即是根据封闭环识别之后的逻辑关系，和切割顺序，进行切割引刀线的添加
	void CreatCutGuideLINE(GeomCloseHEAD*Phead);//输入每一个封闭环，然后对其进行切割 引刀线的添加。
	bool IfIncludeKidClose(GeomCloseHEAD*Phead);//输入一个封闭环，判断是否有子封闭环
	void Add_KidCloseCutLine(GeomCloseHEAD*Phead, bool if_LineAuxiliary);//输入一个含有子封闭环的封闭环，给它添加切割引刀线
	//还是要分直线，圆，圆弧的切割引导线的生成方式的不同。
	//由直线构成的封闭环，其生成切割引导线的方法
	Line_para CreatCutGuideLINE_Polygon(GeomCloseHEAD*Phead, int m_TypeCGLine);
	//由圆型构成的封闭环，其生成切割引导线的方法
	Line_para CreatCutGuideLine_Circle(GeomCloseHEAD*Phead, int m_TypeCGLine);

	//用上CutLeadLine这个类
	CutLeadLine m_CutLeadLine;
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////切割引刀线干涉判断，与调整/////////////////////////
	///////////////////////////核心代码，核心代码，核心代码,下一篇的工作////////
	//输入切割图头结点，把封闭环的切割引导线都清理一遍。
	void CheckCutGuideLINE(NestResultDataNode*head);
	//输入每一个父封闭环，判断其与子封闭环的切割引导线是否存在干涉现象。
	void CheckFKCutGuideLINE(GeomCloseHEAD*pFtemp);

	//输入封闭环，保存板材外轮廓的封闭环头文件（父层封闭环一定要与板材封闭环轮廓判断）与平面切割图头结点（不一定能用得上），
	//判断其切割引导线与其他封闭环是否有干涉，如果有则返回封闭头结点，没有则返回NULL
	//通用的判断算法
	bool CheckCGLineInter(GeomCloseHEAD*pCHtemp, GeomCloseHEAD*m_ceramic_Head);
	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////上面的切割引导线生成方式是角平分线以及两点连线法//////////////////////////////////////
	///////////下面的切割引导线生成方式是圆弧法与切线法//////////////////////////////////////
	//输入一个封闭环头结点，之后对此封闭环头结点进行设置切割引导线
	void CreatCutAuxiliaryPath(GeomCloseHEAD*pCHtemp);
	//多边形与圆弧之类结合的圆弧切割引导线生成方式。将切割引导线直接在此生成，不再提供外部接口。
	void CreatCutAuxiliaryPath_Polygon(GeomCloseHEAD*pCHtemp, int m_TypeCGLine);
	//圆类型的切割引导线的生成方式。将切割引导线直接在此生成，不再提供外部接口。
	void CreatCutAuxiliaryPath_Circle(GeomCloseHEAD*pCHtemp, int m_TypeCGLine);

	//输入封闭环，圆弧与封闭环的切点，切割引导线母线，求圆弧圆心坐标。
	Arc_Point GetArccentPoint(GeomCloseHEAD*pCHtemp, Line_para m_line, Point P_mid);
	//这个函数将求得圆弧切割引导线的参数，并将圆弧切割引导线添加到封闭环上。
	//输入圆弧的起止点，圆弧与封闭环的切点，圆弧的圆心，封闭环。
	//多边形的封闭环，在这个函数里设置切割引导线
	void SetCutAuxiliaryPath(Line_para m_arc_auxi_SEP, Point P_mid, Point P_arccent, GeomCloseHEAD*pCHtemp);
	//这个函数将求得圆弧切割引导线的参数，并将圆弧切割引导线添加到封闭环上。
	//输入圆弧的起止点，圆弧的切点所在的区域，圆弧的圆心，圆封闭环。
	//注意这与上面的函数少了圆弧与封闭环的切点参数，但是多了切点所在的区域。
	//圆形的封闭环，在这个函数里设置切割引导线
	void SetCutAuxiliaryPath(Line_para m_arc_auxi_SEP, int m_belong, Point P_arccent, GeomCloseHEAD*pCHtemp);
	//将切割引导线作为封闭环的固有节点添加进去。
	//m_auxiliaryPath是存储的切割引导线的数据信息，P_mid是圆弧切割引导线的与封闭环的切点,pCHtemp为封闭环头结点
	//在这个函数里将切割引导线挂靠到封闭环上
	//注意这是挂靠多边形类的封闭环的。
	void AddAuxiNodeToCH(AuxiliarPath m_auxiliaryPath, Point P_mid, GeomCloseHEAD*pCHtemp);
	//将 三个切割封闭环的基元挂到封闭环上，并调整封闭环中的基元位置。
	void AddThreeNodeToCH(GeomEleNode*Add_more_Node, GeomEleNode*cut_in_Node, GeomEleNode*cut_out_Node, GeomCloseHEAD*pCHtemp);
	//因为第一个基本图元的垂线不合格，需要第二个基本图元变成第一个基本图元
	GeomCloseHEAD*ChangeSecendEleToFirst(GeomCloseHEAD*pCHtemp);

};

