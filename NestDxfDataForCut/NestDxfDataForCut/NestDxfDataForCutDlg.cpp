
// NestDxfDataForCutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NestDxfDataForCut.h"
#include "NestDxfDataForCutDlg.h"
#include "GeomForCut.h"
#include "afxdialogex.h" 
#include "io.h" 
#include "fcntl.h"
#include"stdio.h"
//输出相关
#include <fstream>

#include <iostream>

#include <conio.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNestDxfDataForCutDlg 对话框



CNestDxfDataForCutDlg::CNestDxfDataForCutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNestDxfDataForCutDlg::IDD, pParent)//构造函数
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	BatchHEAD_ID = 0;//初始化时候，还没有批次
	NestResult_ID = 0;//初始化，还没有图纸导入
	GEOMCLOSE_ID = 0;//初始化，还没有封闭环
	m_pBatchHead = NULL;//未指向任何地方
	m_pNestrsltdtND = NULL;
	m_pGeomclsHead=NULL;
	//Onstart();//放这里等效于点击了start的button
	
}

void CNestDxfDataForCutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNestDxfDataForCutDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CNestDxfDataForCutDlg::OnOpenFile)
	ON_BN_CLICKED(IDC_start, &CNestDxfDataForCutDlg::Onstart)
	ON_BN_CLICKED(IDC_savefile, &CNestDxfDataForCutDlg::OnSavefile)
END_MESSAGE_MAP()


// CNestDxfDataForCutDlg 消息处理程序

BOOL CNestDxfDataForCutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNestDxfDataForCutDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNestDxfDataForCutDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNestDxfDataForCutDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//启动生产,初始化一系列的链表结点
void CNestDxfDataForCutDlg::Onstart()
{
	// TODO:  在此添加控件通知处理程序代码
	BatchHEAD_ID++;//启动生产，那么产品批次就是一次
	m_pBatchHead = m_GeomForCut.CreatBatchHEAD(BatchHEAD_ID);//输入批次进行初始化调用

}

void CNestDxfDataForCutDlg::OnOpenFile()//打开一次就是一张图纸
{
	// TODO:  在此添加控件通知处理程序代码
	//打开图纸导入图纸结点
	NestResult_ID++;
	m_pNestrsltdtND = m_GeomForCut.CreatNestResultNode(NestResult_ID);//在初始化时候记录这是打开的第几张图纸
	m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestrsltdtND);//把新生成的结点挂在生产批次指向的图纸双向链表中
	//创建记录图元结点的F头结点
	GEOMCLOSE_ID++;
	m_pGeomclsHead = m_GeomClose.CreatGeomCloseHEAD(GEOMCLOSE_ID);

	//
	OPENFILENAME ofn = { 0 };

	ofn.Flags = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
	CFileDialog hFileDlg(TRUE, NULL, NULL, ofn.Flags, TEXT("DXF(*.*)|*.*|"), NULL);
	if (hFileDlg.DoModal() == IDOK)
	{
		path = hFileDlg.GetPathName();
		switchkeyword(path);
	}
	//以上已经把排样结果DXF里面的数据全部读取了，接下来要对数据进行：封闭环分开挂到不同的封闭环头结点上
	AdjustGeomCloseNode(m_pNestrsltdtND);

}
//按照打开的文件名路径去搜索LINE ARC CIRCLE
void CNestDxfDataForCutDlg::switchkeyword(CString path)
{
	
	CStdioFile m_dxfofnestresult(path, CFile::modeRead);//输入文件路径，只读
	m_dxfofnestresult.ReadString(m_readgeomele);//每次读一行，读取一次之后，指向下一行，放回bool
	while (m_dxfofnestresult.ReadString(m_readgeomele))//如果不是空文件
	{
		//如果是LINE ARC CIRCLE这三个关键字之间的一个
		
		while ((strcmp(m_readgeomele, "LINE") == 0) || (strcmp(m_readgeomele, "ARC") == 0) || (strcmp(m_readgeomele, "CIRCLE") == 0))
		{
			if (strcmp(m_readgeomele, "LINE") == 0) m_typegeomele = 0;
			if (strcmp(m_readgeomele, "ARC") == 0) m_typegeomele = 1;
			if (strcmp(m_readgeomele, "CIRCLE") == 0) m_typegeomele = 2;
			switch (m_typegeomele)
			{
			case LINE://查到的关键字为LINE的时候，这事只是读到了LINE，但要往下走
					m_dxfofnestresult.ReadString(m_readgeomele);//如果这里不读下一行，那么还是LINE，那么就进不去下面的while，且一直都是LINE,还会在上一个while形成死循环。
					//每进入一次LINE之后除非遇到了下一个LINE，ARC,CIRCLE否则不能让它跳出这个LINE循环
					//从外面往里只有不是LINE ,ARC,CIRCLE，ENDSEC才能进入，从里则遇到LINE ,ARC,CIRCLE，ENDSEC才能出来（把LINE ,ARC,CIRCLE，ENDSEC）排除出来
					while ((strcmp(m_readgeomele, "LINE") != 0) && (strcmp(m_readgeomele, "ARC") != 0) && (strcmp(m_readgeomele, "CIRCLE") != 0)  && (strcmp(m_readgeomele, "ENDSEC") != 0))//真是加多了一个(strcmp(m_readgeomele, "ENDSEC") != 0)这个while才能跳出去。
					{
						m_dxfofnestresult.ReadString(m_readgeomele);//将DXF里面的字符串信息读入m_readgeomele
						symbol = atoi(m_readgeomele);//将m_readgeomele里面的字符串转为整形数值symbol
						m_line = AcceptDxfLineData(symbol, m_readgeomele, m_dxfofnestresult);//输入symbol，输出源图元的起止坐标，
					}//离开这里的时候已经循环完一个LINE了
					//结果却是当到了这里的时候m_line为0了，之前的值全部没有保存起来，而且一次次被覆盖，要想个办法把中间的参数保存起来。
					m_geomstanddata = m_geomele.ReadLineData(m_line);//输入源图元的起止坐标，输出一个标准的图元的数据格式
					//创立记录基本图元的双向链表,把获得的数据保存起来
					m_pGeomEleND = m_GeomClose.CreatGeomEleNode(m_geomstanddata);//输入上面获得的数据，创立一个结点保存数据
					m_pGeomclsHead = m_GeomClose.InsertGeomEleNode(m_pGeomclsHead, m_pGeomEleND, m_geomstanddata);//把创立的基本图元结点挂到由m_pGeomclsHead指向的双向链表中。
					break;//跳出cade LINE的事件
			case ARC:
				m_dxfofnestresult.ReadString(m_readgeomele);
				while ((strcmp(m_readgeomele, "LINE") != 0) && (strcmp(m_readgeomele, "ARC") != 0) && (strcmp(m_readgeomele, "CIRCLE") != 0) && (strcmp(m_readgeomele, "ENDSEC") != 0))//真是加多了一个(strcmp(m_readgeomele, "ENDSEC") != 0)这个while才能跳出去。
				{
					m_dxfofnestresult.ReadString(m_readgeomele);//将DXF里面的字符串信息读入m_readgeomele
					symbol = atoi(m_readgeomele);//将m_readgeomele里面的字符串转为整形数值symbol
					m_arc= AcceptDxfArcData(symbol, m_readgeomele, m_dxfofnestresult);//输入symbol，输出源图元的起止坐标，
				}//离开这里的时候已经循环完一个LINE了
				//结果却是当到了这里的时候m_line为0了，之前的值全部没有保存起来，而且一次次被覆盖，要想个办法把中间的参数保存起来。
				m_geomstanddata = m_geomele.ReadArcData(m_arc);//输入源图元的起止坐标，输出一个标准的图元的数据格式
				//创立记录基本图元的双向链表,把获得的数据保存起来
				m_pGeomEleND = m_GeomClose.CreatGeomEleNode(m_geomstanddata);//输入上面获得的数据，创立一个结点保存数据
				m_pGeomclsHead = m_GeomClose.InsertGeomEleNode(m_pGeomclsHead, m_pGeomEleND, m_geomstanddata);//把创立的基本图元结点挂到由m_pGeomclsHead指向的双向链表中。
				break;
			case CIRCLE:
				m_dxfofnestresult.ReadString(m_readgeomele);
				while ((strcmp(m_readgeomele, "LINE") != 0) && (strcmp(m_readgeomele, "ARC") != 0) && (strcmp(m_readgeomele, "CIRCLE") != 0) && (strcmp(m_readgeomele, "ENDSEC") != 0))
				{
					m_dxfofnestresult.ReadString(m_readgeomele);//加不加这行代码取决于读取的数据结构模式，要看能不能加
					symbol = atoi(m_readgeomele);
					m_circle = AcceptDxfCircleData(symbol,m_readgeomele, m_dxfofnestresult);
				}
				m_geomstanddata = m_geomele.ReadCircleData(m_circle);
				//创立记录基本图元的双向链表,把获得的数据保存起来
				//圆的特殊性是因为它本身就是一个封闭环,先让它挂到另一张排样结果图中，读取完数据之后再挂回来
				GEOMCLOSE_ID++;//自己就可以增加一个封闭环
				//生成一个排样结果图结点
				m_pNestNode_forCircle = m_GeomForCut.CreatNestResultNode(NestResult_ID);//输入上一张图纸的ID，因为这里只是先划一块区域来挂圆，ID不需要改变
				//把图结点挂到生产批次结点上
				m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestNode_forCircle);//新的图纸双向链表结点挂在同一生产批次F头结点上
				//生成一个封闭环结点
				m_pGeomclsHead_forCircle = m_GeomClose.CreatGeomCloseHEAD(GEOMCLOSE_ID);
				//把封闭环结点挂到排样结果图结点上
				m_pNestNode_forCircle = m_GeomForCut.AddGeomCloseHeadNode(m_pNestNode_forCircle, m_pGeomclsHead_forCircle);//输入指向封闭环结点的排样dxf结果图结点，和封闭环结点，把封闭环结点挂到排样dxf结果图上，这里已经包含了三层结构
				//生成一个数据结点
				m_pGeomEleND = m_GeomClose.CreatGeomEleNode(m_geomstanddata);//输入上面获得的数据，创立一个结点保存数据
				//把数据结点挂到封闭环结点上
				m_pGeomclsHead_forCircle = m_GeomClose.InsertGeomEleNode(m_pGeomclsHead_forCircle, m_pGeomEleND, m_geomstanddata);//把创立的基本图元结点挂到由m_pGeomclsHead指向的双向链表中。
				break;
			default:break;//跳出switch (m_typegeomele)事件
			}
			//下面也不应该break跳出，否则，再也进不来了。while是会自循环的，每一次都会去判断下（）里的值。
			//break;//当进来一个关键字之后读完一遍了，则跳出第二个while即判断三个关键字的那个while从第一个while那里去开始第二轮的循环。也可以用m_dxfofnestresult.ReadString(m_readgeomele);让它内部循环但这样就永远跳不出循环了。
		}
		//break;//不应该跳出，否则只是一次就结束了
	}//没有跳出第一个循环的语句，所以一直在内循环指导文件末尾。

	

}
//输入dxf里面的符号，输出dxf里面直线的起止点
GLINE CNestDxfDataForCutDlg::AcceptDxfLineData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult)//前面加上了CNestDxfDataForCutDlg是使得下面一些m_line可以被使用
{
	//GLINE m_line = {0.0,0.0,0.0,0.0};//每一次进来都初始化一次，初始化不应该在这里
	switch (symbol)//只进来一次
	{
	case 10://起始点x
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.x0 = atof(m_readgeomele);
		break;
	case 20://起始点y
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.y0 = atof(m_readgeomele);
		break;
	case 11://终止点x
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.x1 = atof(m_readgeomele);
		break;
	case 21://终止点y
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.y1 = atof(m_readgeomele);
		break;
	default://如果出现了其他情况
		break;
	}
	return m_line;
}
GARC CNestDxfDataForCutDlg::AcceptDxfArcData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult)
{
	switch (symbol)
	{
	case 10://圆心x
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_arc.Arccent_x = atof(m_readgeomele);
		break;
	case 20://圆心y
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_arc.Arccent_y = atof(m_readgeomele);
		break;
	case 40://半径r
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_arc.m_Arc_r = atof(m_readgeomele);
		break;
	case 50://起始角
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_arc.m_ArcAngle_start = atof(m_readgeomele);
		break;
	case 51://终止角
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_arc.m_ArcAngle_end = atof(m_readgeomele);
		break;
	default:
		break;
	}
	return m_arc;
}
GCIRCLE CNestDxfDataForCutDlg::AcceptDxfCircleData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult)
{
	switch (symbol)
	{
	case 10://圆心x
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_circle.m_Circent_x = atof(m_readgeomele);
		break;
	case 20://圆心y
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_circle.m_Circent_y = atof(m_readgeomele);
		break;
	case 40://半径r
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_circle.m_Circle_r = atof(m_readgeomele);
		break;
	default:
		break;
	}
	return m_circle;
}
void CNestDxfDataForCutDlg::OnSavefile()
{
	// TODO:  在此添加控件通知处理程序代码
	
	GeomEleNode*tempnode;
	ofstream outfile("C:\\Users\\BIRL\\Desktop\\59282.txt");
	tempnode = m_pGeomclsHead->FirstGeomele;
	/*outfile << "起点x0,起点y0,终点x1,终点y1" << endl;*/
	while (tempnode)//遍历完所有结点
	{
		
		double x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
		double x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
		double y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
		double y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
		//outfile << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
		outfile << x0 << "    " << y0 << endl;
		outfile << x1 << "    " << y1 << endl; 
		tempnode = tempnode->nextGeomeleNode;
	}
}
void CNestDxfDataForCutDlg::AdjustGeomCloseNode(NestResultDataNode*head)
{
	//以上已经把排样结果DXF里面的数据全部读取了，接下来要对数据进行：封闭环分开挂到不同的封闭环头结点上
	int m_GeomCloseID;//处理了的封闭环的个数
	GeomCloseHEAD*m_Hclstemp;
	for (m_GeomCloseID = 1; m_GeomCloseID <= GEOMCLOSE_ID; m_GeomCloseID++)//保证全部结点已经被处理一遍，且给以跳出的条件
	{
			m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
			m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);
	}
	
}