

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
#include"afxdlgs.h"//for open file
//输出相关

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
	m_IfDataDisposed = false;
	m_NewDxf = true;//新图纸

	//Onstart();//放这里等效于点击了start的button
	//opengl

	
}

void CNestDxfDataForCutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//opengl
	DDX_Control(pDX, IDC_OPENGL, m_Draw);

}

BEGIN_MESSAGE_MAP(CNestDxfDataForCutDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CNestDxfDataForCutDlg::OnOpenFile)
	ON_BN_CLICKED(IDC_start, &CNestDxfDataForCutDlg::Onstart)
	ON_BN_CLICKED(IDC_savefile, &CNestDxfDataForCutDlg::OnSavefile)
	ON_BN_CLICKED(IDC_savefile2, &CNestDxfDataForCutDlg::OnSimulation)
	ON_WM_TIMER()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_SAVEarc, &CNestDxfDataForCutDlg::OnSavearc)
	ON_BN_CLICKED(IDC_SAVTran, &CNestDxfDataForCutDlg::Savtran)
	ON_BN_CLICKED(IDC_SAVECircle, &CNestDxfDataForCutDlg::Savecircle)
	ON_BN_CLICKED(IDC_SAVTran2, &CNestDxfDataForCutDlg::SaveNestCloseHead)
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
	//关于OpenGL
	//CRect rect;
	//// Get size and position of the picture control
	////GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);
	//// Convert screen coordinates to client coordinates
	//ScreenToClient(rect);
	///////////////////////OPENGL INIT/////////////////////////
	//CWnd *wnd = GetDlgItem(IDC_OPENGL);

	//hrenderDC=::GetDC(wnd->m_hWnd);
	////ScreenToClient(rect);
	//if (SetWindowPixelFormat(hrenderDC) == FALSE)
	//	return 0;

	//if (CreateViewGLContext(hrenderDC) == FALSE)
	//	return 0;

	//glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_FILL);
	/////////////////////////////////////////////
	//glEnable(GL_TEXTURE_2D);
	//glShadeModel(GL_SMOOTH);
	//glViewport(0, 0, 259, 231);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45, 1, 0.1, 100.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glShadeModel(GL_SMOOTH); // Enable Smooth Shading
	//glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
	//glClearDepth(1.0f); // Depth Buffer Setup
	////glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	//glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
	///////////////////////////////////////////////////////////////////////////
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	////SetTimer(1, 10, 0);
	////////////////////////////////////////////////////////////////

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
	////这里可能也会出问题
	//ValidateRect(NULL);//这里就让窗口一开始就不能显示任何东西
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
    m_NewDxf= true;//新图纸
	NestResult_ID++;
	m_pNestrsltdtND = m_GeomForCut.CreatNestResultNode(NestResult_ID);//在初始化时候记录这是打开的第几张图纸
	m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestrsltdtND);//把新生成的结点挂在生产批次指向的图纸双向链表中
	//生成一个只存放圆的排样结果图结点
	m_pNestNode_forCircle = m_GeomForCut.CreatNestResultNode(NestResult_ID);//输入上一张图纸的ID，因为这里只是先划一块区域来挂圆，ID不需要改变
	//把只存放圆的排样结果图结点挂到生产批次结点上
	m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestNode_forCircle);//新的图纸双向链表结点挂在同一生产批次F头结点上

	//创建记录图元结点的F头结点
	GEOMCLOSE_ID++;
	m_pGeomclsHead = m_GeomClose.CreatGeomCloseHEAD(GEOMCLOSE_ID);
	//把封闭环结点挂到排样结果图结点上
	m_pNestrsltdtND = m_GeomForCut.AddGeomCloseHeadNode(m_pNestrsltdtND, m_pGeomclsHead);//输入指向封闭环结点的排样dxf结果图结点，和封闭环结点，把封闭环结点挂到排样dxf结果图上，这里已经包含了三层结构

	//
	OPENFILENAME ofn = { 0 };

	ofn.Flags = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
	CFileDialog m_dlg(TRUE, NULL, NULL, ofn.Flags, TEXT("DXF(*.*)|*.*|"), NULL);
	///////////////////////////////////////////
	/*CFileDialog m_dlg(true);
	m_dlg.m_ofn.nMaxFile = 511;
	m_dlg.m_ofn.lpstrFilter = "DXF Files(*.dxf)\0*.dxf\0All Files(*.*)\0*.*\0\0";
	m_dlg.m_ofn.lpstrTitle = "Open DXF Files";
	CString m_filename;
*/

	if (m_dlg.DoModal() == IDOK)
	{
		path = m_dlg.GetPathName();
		switchkeyword(path);
	}
	//以上已经把排样结果DXF里面的数据全部读取了，接下来要对数据进行：封闭环分开挂到不同的封闭环头结点上
	AdjustGeomCloseNode(m_pNestrsltdtND);

}
//按照打开的文件名路径去搜索LINE ARC CIRCLE
void CNestDxfDataForCutDlg::switchkeyword(CString path)
{
	//bool a=false;
	CStdioFile m_dxfofnestresult(path, CFile::modeRead);//输入文件路径，只读
	//a=m_dxfofnestresult.ReadString(m_readgeomele);//每次读一行，读取一次之后，指向下一行，放回bool
	while (m_dxfofnestresult.ReadString(m_readgeomele))//如果不是空文件
	//while (a)
	{
		//如果是LINE ARC CIRCLE这三个关键字之间的一个
		//a = m_dxfofnestresult.ReadString(m_readgeomele);
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
				//////////////////////////////////////////////////////////////////////////////////
				////生成一个排样结果图结点
				//m_pNestNode_forCircle = m_GeomForCut.CreatNestResultNode(NestResult_ID);//输入上一张图纸的ID，因为这里只是先划一块区域来挂圆，ID不需要改变
				////把图结点挂到生产批次结点上
				//m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestNode_forCircle);//新的图纸双向链表结点挂在同一生产批次F头结点上
				////生成一个封闭环结点
				////////////////////////////////////////////////////////////////////////////////////
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

bool CNestDxfDataForCutDlg::AdjustGeomCloseNode(NestResultDataNode*head)
{
	//以上已经把排样结果DXF里面的数据全部读取了，接下来要对数据进行：封闭环分开挂到不同的封闭环头结点上
	int m_GeomCloseID;//处理了的封闭环的个数
	GeomEleNode*temp;
	GeomCloseHEAD*Htemp, *m_FindTheEndClosed, *m_CircleFirstClosed=NULL;
	int m_MaxNumOfGeomClose, m_MAXNumOfGeomClose;

	
	
	temp = head->FirstGeomClose->FirstGeomele;//最原始链表的第一个数据结点
	while (temp->nextGeomeleNode)//找到最原始链表的最后一个数据结点
		temp = temp->nextGeomeleNode;
	m_MaxNumOfGeomClose = temp->m_NumGeomCloseID;//保存在最原始数据结点双向链表上最后一个结点的封闭环数是除了圆之外最大的封闭环数了
	GeomCloseHEAD*m_NoIntactGeomCloseHead = NULL;
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_MaxNumOfGeomClose; m_GeomCloseID++)//保证全部结点已经被处理一遍，且给以跳出的条件
	{
			m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
			m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);
	}
	//对已经分开不同封闭环的数据结点分析其封闭环完整性
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_MaxNumOfGeomClose; m_GeomCloseID++)//保证全部结点已经被处理一遍，且给以跳出的条件
	{
		m_NoIntactGeomCloseHead = m_GeomForCut.JudgeGeomCloseIntact(m_pNestrsltdtND);//判断封闭环是否完整
		m_pNestrsltdtND = m_GeomForCut.Find_Change_GeomCloseHEAD(m_pNestrsltdtND, m_NoIntactGeomCloseHead);//输入不完整的封闭环结点，寻找配对的封闭环结点，并内部做调整
	}
	//再将封闭环清理一遍，将不同的分出去
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_MaxNumOfGeomClose; m_GeomCloseID++)//保证全部结点已经被处理一遍，且给以跳出的条件
	{
		m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
		m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);
	}
	//对已经分开不同封闭环的数据结点分析其封闭环完整性
	//以上应该是已经保证了封闭环挂在不同的封闭环F头结点指向的双向链表上，并且每一个封闭环结点内的数据结点都是完整的
	//以下还有封闭环之间的排序问题，封闭环之间的过渡线问题，圆往回挂的问题。
	//封闭环内的数据结点处理问题
	//要把圆加入到排样图里面
	if ((m_pNestrsltdtND->nextNestResultDataNode->FirstGeomClose))//如果有圆的排样图
	{
		m_CircleFirstClosed = m_pNestrsltdtND->nextNestResultDataNode->FirstGeomClose;//全部是圆的那个封闭环双向链表的第一个封闭环头结点
		m_FindTheEndClosed = m_pNestrsltdtND->FirstGeomClose;

		while (m_FindTheEndClosed->nextGeomcloseNode)//找到最后一个直线圆弧组成的封闭环
		{
			m_FindTheEndClosed = m_FindTheEndClosed->nextGeomcloseNode;
		}
		m_FindTheEndClosed->nextGeomcloseNode = m_CircleFirstClosed;
		m_CircleFirstClosed->prevGeomcloseNode = m_FindTheEndClosed;
		while (m_CircleFirstClosed->nextGeomcloseNode)//找到圆封闭环的最后一个封闭环
		{
			m_CircleFirstClosed = m_CircleFirstClosed->nextGeomcloseNode;

		}
		m_MAXNumOfGeomClose = m_MaxNumOfGeomClose + m_CircleFirstClosed->GEOMCLOSE_ID;
		m_FindTheEndClosed->GEOMCLOSE_ID = m_MAXNumOfGeomClose;//这样就可以把所有的封闭环数传到最后一个圆封闭环数据里
		//对包含有圆的再处理一遍
		for (m_GeomCloseID = 1; m_GeomCloseID <= m_MAXNumOfGeomClose; m_GeomCloseID++)//保证全部结点已经被处理一遍，且给以跳出的条件
		{
			m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
			m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);
		}
		m_pNestrsltdtND->nextNestResultDataNode->FirstGeomClose = NULL;//既然圆已经从这个链表头里面退出来了，那么就该让其下面变为表。
	}
	
	//以上以及将所有的封闭环处理好了。
	//m_GeomForCut.ChangeEleNodeOfGeomClosed_origin(m_pNestrsltdtND);//可惜这代码没有起到任何效果，原因之一可能是封闭环本身就没有分清楚，其二是处理的算法有问题。
	//m_GeomForCut.ChangClosedNodeOfNRDXF(m_pNestrsltdtND);
	//m_GeomForCut.ChangeEleNodeOfGeomClosed_order(m_pNestrsltdtND);
	//////另一种处理方式
	//先贪婪算法将所有的封闭环按给定初始顺序
	m_GeomForCut.BaseTS_GR_ForCutPathPlan(head);
	//划分出不同的封闭环层次
	//m_GeomForCut.Find_AdjustNestCloseHead(head);
	////用蚁群算法对第一层封闭环进行路径规划与优化
	//m_GeomForCut.Base_ACO_ForFirstCHead(head);
	////根据蚁群算法调整后的第一层封闭环，调整每个封闭环群里面的子封闭环顺序，并调整相应的父封闭环打孔点
	//m_GeomForCut.BaseTS_GR_ForKidCHead(head);
	//

	m_IfDataDisposed = true;
	return m_IfDataDisposed;
}
void CNestDxfDataForCutDlg::OnSavefile()
{
	
	// TODO:  在此添加控件通知处理程序代码
	GeomCloseHEAD*Htemp;
	GeomEleNode*tempnode;
	int typegeomele;
	double x0, x1, y0, y1, Arccent_x, Arccent_y, r, temp;

	USES_CONVERSION;
	DWORD dFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	CFileDialog saveDlg(FALSE, NULL, NULL, dFlags, _T("Txt Files (*.txt)|*.txt||"), NULL);
		if (m_IfDataDisposed)//数据处理完了，保存才有意义
		{
			if (saveDlg.DoModal() == IDOK)
			{
				// the txt file path.
				CString strFilePath = saveDlg.GetPathName();
				strFilePath += _T(".txt");
				string strDxfPath = strFilePath;
				ofstream outfile(strDxfPath.c_str());
				Htemp = m_pNestrsltdtND->FirstGeomClose;//第一个封闭环F结点
				while (Htemp)//全部遍历
				{
					tempnode = Htemp->FirstGeomele;//封闭环里的第一个数据结点
					while (tempnode)//全部遍历完
					{
						x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
						x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
						y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
						y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
						outfile << x0 << "    " << y0 << endl;
						outfile << x1 << "    " << y1 << endl;
						tempnode = tempnode->nextGeomeleNode;
					}
					Htemp = Htemp->nextGeomcloseNode;
				}
			}
	}
	else
	{
		AfxMessageBox((_T("请等待数据处理完毕！"), MB_YESNO) == IDYES);
	}

}

void CNestDxfDataForCutDlg::OnSimulation()
{
	// TODO:  在此添加控件通知处理程序代码
	// Setup the OpenGL Window's timer to render
	SetTimer(1, 100, 0);

}
////////////////////////////////////////////////////////////////////////////////
//opengl
BOOL CNestDxfDataForCutDlg::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 0;
	pixelDesc.cRedShift = 0;
	pixelDesc.cGreenBits = 0;
	pixelDesc.cGreenShift = 0;
	pixelDesc.cBlueBits = 0;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 0;
	pixelDesc.cAccumRedBits = 0;
	pixelDesc.cAccumGreenBits = 0;
	pixelDesc.cAccumBlueBits = 0;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 0;
	pixelDesc.cStencilBits = 1;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	PixelFormat = ChoosePixelFormat(hDC, &pixelDesc);

	if (PixelFormat == 0) // Choose default
	{
		PixelFormat = 1;
		if (DescribePixelFormat(hDC, PixelFormat,
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
		{
			return FALSE;
		}
	}
	if (SetPixelFormat(hDC, PixelFormat, &pixelDesc) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CNestDxfDataForCutDlg::CreateViewGLContext(HDC hDC)
{
	hrenderRC = wglCreateContext(hDC);
	if (hrenderRC == NULL)
		return FALSE;
	if (wglMakeCurrent(hDC, hrenderRC) == FALSE)
		return FALSE;
	return TRUE;
}

void CNestDxfDataForCutDlg::OnTimer(UINT nIDEvent) //实时绘制场景
{
	
	// TODO: Add your message handler code here and/or call default
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -6.0f); // Move Left 1.5 Units And Into The Screen 6.0
	//glRotated(m_yRotate, 0.0, 1.0, 0.0);
	//数据
	GeomCloseHEAD*Htemp;
	GeomEleNode*tempnode;
	GLINE m_line;
	Htemp = m_pNestrsltdtND->FirstGeomClose;//第一个封闭环F结点
	while (Htemp)//全部遍历
	{
		tempnode = Htemp->FirstGeomele;//封闭环里的第一个数据结点
		while (tempnode)//全部遍历完
		{
			//////////opengl///////////////////////////
			m_line = ReadDataForOGL(tempnode);
			glBegin(GL_LINE_STRIP);//折线
			glVertex2f(m_line.x0, m_line.y0);
			glVertex2f(m_line.x1, m_line.y1);
			glEnd();
			SwapBuffers(hrenderDC);
			//SetTimer(1, 500, 0);
			tempnode = tempnode->nextGeomeleNode;
		}
		Htemp = Htemp->nextGeomcloseNode;
	}
	CDialog::OnTimer(nIDEvent);
}
	GLINE CNestDxfDataForCutDlg::ReadDataForOGL(GeomEleNode*node)
{
	GLINE m_line;
	m_line.x0 = node->m_GeomStandData.GeoEle_start_x0;
	m_line.y0 = node->m_GeomStandData.GeoEle_start_y0;
	m_line.x1 = node->m_GeomStandData.GeoEle_start_x1;
	m_line.y1 = node->m_GeomStandData.GeoEle_start_y1;
	return m_line;
	
}


	void CNestDxfDataForCutDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// 此功能要求 Windows Vista 或更高版本。
		// _WIN32_WINNT 符号必须 >= 0x0600。
		// TODO:  在此添加消息处理程序代码和/或调用默认值
		CRect crectt;
		m_Draw.GetWindowRect(&crectt);
		// check whether the current cursor is on the material view window.
		if (crectt.PtInRect(pt))
			//m_Draw.ZoomViewPort(zDelta, pt);
			m_Draw.IsZoomed();
		CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
	}


	void CNestDxfDataForCutDlg::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO:  在此添加消息处理程序代码和/或调用默认值
		//CRect crecttt;
		//m_Draw.GetWindowRect(&crecttt);
		//ScreenToClient(&crecttt);
		//if (crecttt.PtInRect(point))
		//{
		//	m_Draw.SetFocus();

		//	if (nFlags & MK_MBUTTON)
		//	{
		//		CPoint ptOffSet = point - m_Draw.GetPanReferPt();
		//		m_Draw.PanViewPort(ptOffSet.x, ptOffSet.y);
		//		m_Draw.SetPanReferPt(point);
		//	}

		//	// display the current coordinate.
		//	GlViewPortPtr pViewPort = m_matPreviewWnd.GetViewPort();
		//	Point2D currentPt = pViewPort->GetCursorPos();
		//	m_strCoordinate.Format(_T("x= %.4f,  y= %.4f"), currentPt.X(), currentPt.Y());
		//	/*UpdateData(FALSE);*/
		//	SetDlgItemText(IDC_EDIT_Coordinate, m_strCoordinate);
		//}
		CDialogEx::OnMouseMove(nFlags, point);
	}


	void CNestDxfDataForCutDlg::OnSavearc()
	{
		// TODO:  在此添加控件通知处理程序代码
		GeomCloseHEAD*Htemp;
		GeomEleNode*tempnode;
		int typegeomele;
		double x0, x1, y0, y1, Arccent_x, Arccent_y, r, temp, Angle_start, Angle_end;
		if (m_IfDataDisposed)//数据处理完了，保存才有意义
		{
			ofstream outfile("I:\\MATLAB\\DXF\\最低点21.txt");
			Htemp = m_pNestrsltdtND->FirstGeomClose;//第一个封闭环F结点
			while (Htemp)//全部遍历
			{
				tempnode = Htemp->FirstGeomele;//封闭环里的第一个数据结点
				while (tempnode)//全部遍历完
				{
					
					switch (tempnode->m_GeomStandData.m_typegeomele)
					{
					case 1:

						 typegeomele = 1;//实切直线
						 x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
						 x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
						 y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
						 y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
						 //outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
						//outfile << typegeomele << endl;
						outfile << x0 << "    " << y0 << endl;
						outfile << x1 << "    " << y1 << endl;
						break;
					case 2:
						 //现在会产生X0>X1的现象导致不能画出图形
					x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
					x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
					Angle_start = tempnode->m_GeomStandData.m_arc.m_ArcAngle_start;
					Angle_end = tempnode->m_GeomStandData.m_arc.m_ArcAngle_end;
					if (x0>x1)
					{
						/*temp = Angle_start;
						Angle_start = Angle_end;
						Angle_end = temp;*/
						typegeomele = 4;//代表反向画圆弧
					}
					else
					{
						typegeomele = 2;
					}
					Arccent_x = tempnode->m_GeomStandData.m_arc.Arccent_x;
					Arccent_y = tempnode->m_GeomStandData.m_arc.Arccent_y;
					r = tempnode->m_GeomStandData.m_arc.m_Arc_r;
					outfile << typegeomele << endl;
					outfile << Angle_start << "    " << Angle_end << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;
					break;
					case 3:
					break;
					default:
					break;
					}
					//////////////opengl///////////////////////////


					tempnode = tempnode->nextGeomeleNode;
				}
				Htemp = Htemp->nextGeomcloseNode;
			}

		}
		else
		{
			AfxMessageBox((_T("请等待数据处理完毕！"), MB_YESNO) == IDYES);
		}

	}


	void CNestDxfDataForCutDlg::Savtran()
	{
		// TODO:  在此添加控件通知处理程序代码
		GeomCloseHEAD*Htemp;
		GeomEleNode*tempnode;
		int typegeomele;
		double x0, x1, y0, y1, Arccent_x, Arccent_y, r, temp, Angle_start, Angle_end, Angle_cut_start;
		double x0_tran=0,y0_tran=0, x1_tran, y1_tran;
		double Angle_add;//增量角度
		if (m_IfDataDisposed)//数据处理完了，保存才有意义
		{
			//将第二个封闭环代替第一个封闭环
			/*m_pNestrsltdtND = m_GeomForCut.ChangeSencondCH2FH(m_pNestrsltdtND);*/
			ofstream outfile("I:\\MATLAB\\DXF\\过渡线01.txt");
			Htemp = m_pNestrsltdtND->FirstGeomClose;//第一个封闭环F结点
			while (Htemp)//全部遍历
			{
				tempnode = Htemp->FirstGeomele;//封闭环里的第一个数据结点
				x1_tran = tempnode->m_GeomStandData.GeoEle_start_x0;
				y1_tran = tempnode->m_GeomStandData.GeoEle_start_y0;
				typegeomele = 4;//空跑直线
				outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1_tran << "    " << y1_tran << endl;

				while (tempnode)//全部遍历完
				{
						switch (tempnode->m_GeomStandData.m_typegeomele)
					{
					case 1://直线
						
						typegeomele = 1;//实切直线
						x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
						x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
						y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
						y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
						outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
						break;
					case 2://圆弧，要按照太极控制卡的API输出相应的数据
						typegeomele = 2;//实切圆弧
						Angle_start = tempnode->m_GeomStandData.m_arc.m_ArcAngle_start;
						Angle_end = tempnode->m_GeomStandData.m_arc.m_ArcAngle_end;
						r = tempnode->m_GeomStandData.m_arc.m_Arc_r;
						Arccent_x = tempnode->m_GeomStandData.m_arc.Arccent_x;
						Arccent_y = tempnode->m_GeomStandData.m_arc.Arccent_y;
						if (tempnode->m_GeomStandData.m_IsTranData)//如果将圆弧的起止角度
						{
							Angle_cut_start = Angle_end;
							if (Angle_start < Angle_end)//起始角小于终止角
							{
								Angle_add = Angle_end - Angle_start;
								Angle_add = -Angle_add;//调换了位置则是负的增量角度
							}
							else//起始角大于终止角
							{
								Angle_add = 360 - Angle_start + Angle_end;
								Angle_add = -Angle_add;//调换了位置则是负的增量角度
							}
						}
						else//如果没有调换圆弧的起止角度
						{
							Angle_cut_start = Angle_start;
							if (Angle_start < Angle_end)//起始角小于终止角
				
							{
								Angle_add = Angle_end - Angle_start;								
							}
							else//起始角大于终止角
							{
								Angle_add = 360 - Angle_start + Angle_end;					
							}
						}		
						outfile << typegeomele << "    " << Angle_cut_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;
						break;
					case 3:
						typegeomele = 2;//实切圆弧
						Angle_start = tempnode->m_GeomStandData.m_circle.m_Circle_Start_Angle;
						Angle_add = 360;
						r = tempnode->m_GeomStandData.m_circle.m_Circle_r;
						Arccent_x = tempnode->m_GeomStandData.m_circle.m_Circent_x;
						Arccent_y = tempnode->m_GeomStandData.m_circle.m_Circent_y;
						outfile << typegeomele << "    " << Angle_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;

						break;
					default:
						break;
					}
					if (NULL == tempnode->nextGeomeleNode)//如果是最后一个,那么要把他的值保留下来
					{
						x0_tran = tempnode->m_GeomStandData.GeoEle_start_x1;
						y0_tran = tempnode->m_GeomStandData.GeoEle_start_y1;
					}


					tempnode = tempnode->nextGeomeleNode;
				
				}
				if (NULL == Htemp->nextGeomcloseNode)//如果是最后一个,那么要把他的值保留下来
				{
					x0 = x0_tran;
					y0 = y0_tran;
				}
				Htemp = Htemp->nextGeomcloseNode;
			}
			x1 = 0.0;
			y1 = 0.0;

			typegeomele = 4;//空跑直线
			outfile << typegeomele << "    " << x0 << "    " << y0<< "    " << x1<< "    " << y1<< endl;


		}
		else
		{
			AfxMessageBox((_T("请等待数据处理完毕！"), MB_YESNO) == IDYES);
		}

	}


	void CNestDxfDataForCutDlg::Savecircle()
	{
		// TODO:  在此添加控件通知处理程序代码
		GeomCloseHEAD*Htemp;
		GeomEleNode*tempnode;
		NestResultDataNode*DxfTempHead;
		int typegeomele;
		int m_DxfResult_ID=0;
		double x0, x1, y0, y1, Arccent_x, Arccent_y, r, temp, Angle_start, Angle_end, Angle_cut_start;
		double x0_tran , y0_tran , x1_tran, y1_tran;
		double Angle_add;//增量角度
		bool m_ifOneDxfResultCuted=false;
		USES_CONVERSION;
		DWORD dFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
		CFileDialog saveDlg(FALSE, NULL, NULL, dFlags, _T("Txt Files (*.txt)|*.txt||"), NULL);
		if (m_IfDataDisposed)//数据处理完了，保存才有意义
		{
			if (saveDlg.DoModal() == IDOK)
			{
				// the txt file path.
				CString strFilePath = saveDlg.GetPathName();
				strFilePath += _T(".txt");
				string strDxfPath = strFilePath;
				ofstream outfile(strDxfPath.c_str());
				DxfTempHead = m_pBatchHead->FirstNestRsltNode;
				while (DxfTempHead)
				{
					//每次处理新的图纸时候，置位0；
					x0_tran = 0;
					y0_tran = 0;
					Htemp = DxfTempHead->FirstGeomClose;//第一个封闭环F结点
					if (Htemp)//不是由圆占据之后迁移留下的空表
					{
						while (Htemp)//全部遍历
						{
							tempnode = Htemp->FirstGeomele;//封闭环里的第一个数据结点
							x1_tran = tempnode->m_GeomStandData.GeoEle_start_x0;
							y1_tran = tempnode->m_GeomStandData.GeoEle_start_y0;
							typegeomele = 4;//空跑直线
							outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1_tran << "    " << y1_tran << endl;

							while (tempnode)//全部遍历完
							{
								switch (tempnode->m_GeomStandData.m_typegeomele)
								{
								case 1://直线

									typegeomele = 1;//实切直线
									x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
									x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
									y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
									y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
									outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
									break;
								case 2://圆弧，要按照太极控制卡的API输出相应的数据
									typegeomele = 2;//实切圆弧
									Angle_start = tempnode->m_GeomStandData.m_arc.m_ArcAngle_start;
									Angle_end = tempnode->m_GeomStandData.m_arc.m_ArcAngle_end;
									r = tempnode->m_GeomStandData.m_arc.m_Arc_r;
									Arccent_x = tempnode->m_GeomStandData.m_arc.Arccent_x;
									Arccent_y = tempnode->m_GeomStandData.m_arc.Arccent_y;
									if (tempnode->m_GeomStandData.m_IsTranData)//如果将圆弧的起止角度
									{
										Angle_cut_start = Angle_end;
										if (Angle_start < Angle_end)//起始角小于终止角
										{
											Angle_add = Angle_end - Angle_start;
											Angle_add = -Angle_add;//调换了位置则是负的增量角度
										}
										else//起始角大于终止角
										{
											Angle_add = 360 - Angle_start + Angle_end;
											Angle_add = -Angle_add;//调换了位置则是负的增量角度
										}
									}
									else//如果没有调换圆弧的起止角度
									{
										Angle_cut_start = Angle_start;
										if (Angle_start < Angle_end)//起始角小于终止角
										{
											Angle_add = Angle_end - Angle_start;
										}
										else//起始角大于终止角
										{
											Angle_add = 360 - Angle_start + Angle_end;
										}
									}
									outfile << typegeomele << "    " << Angle_cut_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;
									break;
								case 3:
									typegeomele = 2;//实切圆弧
									Angle_start = tempnode->m_GeomStandData.m_circle.m_Circle_Start_Angle;
									Angle_add = 360;
									r = tempnode->m_GeomStandData.m_circle.m_Circle_r;
									Arccent_x = tempnode->m_GeomStandData.m_circle.m_Circent_x;
									Arccent_y = tempnode->m_GeomStandData.m_circle.m_Circent_y;
									outfile << typegeomele << "    " << Angle_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;

									break;
								default:
									break;
								}
								if (NULL == tempnode->nextGeomeleNode)//如果是最后一个,那么要把他的值保留下来
								{
									x0_tran = tempnode->m_GeomStandData.GeoEle_start_x1;
									y0_tran = tempnode->m_GeomStandData.GeoEle_start_y1;
								}


								tempnode = tempnode->nextGeomeleNode;

								/*}
								Htemp = Htemp->nextGeomcloseNode;
								}*/
								if (NULL == Htemp->nextGeomcloseNode)//如果是最后一个,那么要把他的值保留下来
								{
									x0 = x0_tran;
									y0 = y0_tran;
								}
							}
						Htemp = Htemp->nextGeomcloseNode;
					}
					x1 = 0.0;
					y1 = 0.0;
					typegeomele = 4;//空跑直线
					outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;


						m_ifOneDxfResultCuted = true;//一张图纸已经处理完毕
						typegeomele = 99;//如果控制机床这里该是暂停切割和机床回到工具坐标原点，和传送带运输机床。
						outfile << typegeomele << endl;

					}
					DxfTempHead = DxfTempHead->nextNestResultDataNode;
				}
			}
		}
		else
		{
			AfxMessageBox((_T("请等待数据处理完毕！"), MB_YESNO) == IDYES);
		}

		 
	}
//核心代码
//核心代码
//核心代码	
//读取有子封闭环的数据
void CNestDxfDataForCutDlg::SaveNestCloseHead()
{
		// TODO:  在此添加控件通知处理程序代码
		GeomCloseHEAD*Hkidtemp;
		GeomEleNode*tempnode;
		bool m_ifHvkidClose;
		
		//ofstream outfile("I:\\MATLAB\\DXF\\过渡线01.txt");
		if (m_IfDataDisposed)//数据处理完了，保存才有意义
			{

			pTheFirstLevelCloseHead = m_pNestrsltdtND->FirstGeomClose;//第一个封闭环F结点
			while (pTheFirstLevelCloseHead)//全部遍历
				{
					//要判断是否有子封闭环， 
				m_ifHvkidClose = IfIncludeKidClose(pTheFirstLevelCloseHead);
					if (m_ifHvkidClose)
					{
						//既然有子封闭环，那么就在子封闭环里把整一个封闭嵌套的所有数据读完
						//包括了父封闭环的数据
						ReadKidCloseHeadData(pTheFirstLevelCloseHead);
					}
					else//如果没有子封闭环的话
					{
						ReadCloseHeadData(pTheFirstLevelCloseHead);
					}
					pTheFirstLevelCloseHead = pTheFirstLevelCloseHead->nextGeomcloseNode;
				}
			}
		else
		{
			AfxMessageBox((_T("请等待数据处理完毕！"), MB_YESNO) == IDYES);
		}

}
	//判断此是否包含有子封闭环
	bool  CNestDxfDataForCutDlg::IfIncludeKidClose(GeomCloseHEAD*pTemp)
	{
		if (pTemp->FirstInsideGCloseNode)
			return true;
		else
			return false;
	}
	//读取封闭环内部子封闭环的数据
	//这个代码是把这个大封闭环的数据从里到外全部读取出来
	void CNestDxfDataForCutDlg::ReadKidCloseHeadData(GeomCloseHEAD*pTemp)
	{
		GeomCloseHEAD*Htemp, *Hkidtemp, *HFattemp, *pfadtemph;
		GeomCloseHEAD*NextCHtemp;//下一个封闭环头结点
		bool m_ifHvkidClose;
		bool m_ifotherHvkidClose;


		//既然pTemp是有子封闭环的，那么就得先读子封闭环的数据
		Hkidtemp = pTemp->FirstInsideGCloseNode;


		//这个while是要找到最深的一层子封闭环
		while (Hkidtemp->FirstInsideGCloseNode)//找到最深一层；
			Hkidtemp = Hkidtemp->FirstInsideGCloseNode;
		Htemp = Hkidtemp;
		while (Htemp)//把这一层给遍历一遍
		{
			if (Htemp->FirstInsideGCloseNode)//如果兄弟节点有子节点,那么在这个代码就会把子封闭环和父封闭环一起读完
			{

				ReadKidCloseHeadData(Htemp);
			}
			else
			{
				ReadCloseHeadData(Htemp);
			}

			Htemp = Htemp->nextGeomcloseNode;
		}


		//上一个循环是已经把最深一层子封闭处理完，那么现在要进行上一层子封闭环
		HFattemp = Hkidtemp->prevOutsideGCloseNode;
		int a = 0;
		while (HFattemp != pTemp)//当还不是最先的父节点的时候
		{
			ReadCloseHeadData(HFattemp);

			NextCHtemp = HFattemp->nextGeomcloseNode;//第一个兄弟封闭环结点
			while (NextCHtemp)//全部遍历
			{
				//要判断是否有子封闭环， 
				m_ifotherHvkidClose = IfIncludeKidClose(NextCHtemp);
				if (m_ifotherHvkidClose)
				{
					//既然有子封闭环，那么就在子封闭环里把整一个封闭嵌套的所有数据读完
					//包括了父封闭环的数据
					ReadKidCloseHeadData(NextCHtemp);
				}
				else//如果没有子封闭环的话
				{
					ReadCloseHeadData(NextCHtemp);
				}
				NextCHtemp = NextCHtemp->nextGeomcloseNode;
			}
			HFattemp = HFattemp->prevOutsideGCloseNode;
		}
		//以上已经把除了第一层的父节点之外的所以子节点的数据都处理完了，余下要处理的是第一层的父节点
		ReadCloseHeadData(pTemp);
	}

//读单一封闭环的数据
	void CNestDxfDataForCutDlg::ReadCloseHeadData(GeomCloseHEAD*Htemp)
	{
		GeomEleNode*tempnode;
		int typegeomele;
			ofstream outfile("I:\\MATLAB\\DXF\\过渡线38.txt", ios_base::out | ios_base::app);//打开并追加
			tempnode = Htemp->FirstGeomele;//封闭环里的第一个数据结点
			x1_tran = tempnode->m_GeomStandData.GeoEle_start_x0;
			y1_tran = tempnode->m_GeomStandData.GeoEle_start_y0;
			typegeomele = 4;//空跑直线
			if (m_NewDxf == true)//表明这是新图片
			{
				x0_tran = 0.0;
				y0_tran = 0.0;
			}
			outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1_tran << "    " << y1_tran << endl;
			m_NewDxf = false;//新图纸;//进入一次之后便不再是新图片
			while (tempnode)//全部遍历完
			{
				switch (tempnode->m_GeomStandData.m_typegeomele)
				{
				case 1://直线

					typegeomele = 1;//实切直线
					x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
					x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
					y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
					y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
					outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
					break;
				case 2://圆弧，要按照太极控制卡的API输出相应的数据
					typegeomele = 2;//实切圆弧
					Angle_start = tempnode->m_GeomStandData.m_arc.m_ArcAngle_start;
					Angle_end = tempnode->m_GeomStandData.m_arc.m_ArcAngle_end;
					r = tempnode->m_GeomStandData.m_arc.m_Arc_r;
					Arccent_x = tempnode->m_GeomStandData.m_arc.Arccent_x;
					Arccent_y = tempnode->m_GeomStandData.m_arc.Arccent_y;
					if (tempnode->m_GeomStandData.m_IsTranData)//如果将圆弧的起止角度
					{
						Angle_cut_start = Angle_end;
						if (Angle_start < Angle_end)//起始角小于终止角
						{
							Angle_add = Angle_end - Angle_start;
							Angle_add = -Angle_add;//调换了位置则是负的增量角度
						}
						else//起始角大于终止角
						{
							Angle_add = 360 - Angle_start + Angle_end;
							Angle_add = -Angle_add;//调换了位置则是负的增量角度
						}
					}
					else//如果没有调换圆弧的起止角度
					{
						Angle_cut_start = Angle_start;
						if (Angle_start < Angle_end)//起始角小于终止角
						{
							Angle_add = Angle_end - Angle_start;
						}
						else//起始角大于终止角
						{
							Angle_add = 360 - Angle_start + Angle_end;
						}
					}
					outfile << typegeomele << "    " << Angle_cut_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;
					break;
				case 3:
					typegeomele = 2;//实切圆弧
					Angle_start = tempnode->m_GeomStandData.m_circle.m_Circle_Start_Angle;
					Angle_add = 360;
					r = tempnode->m_GeomStandData.m_circle.m_Circle_r;
					Arccent_x = tempnode->m_GeomStandData.m_circle.m_Circent_x;
					Arccent_y = tempnode->m_GeomStandData.m_circle.m_Circent_y;
					outfile << typegeomele << "    " << Angle_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;

					break;
				default:
					break;
				}
				if (NULL == tempnode->nextGeomeleNode)//如果是最后一个,那么要把他的值保留下来
				{
					x0_tran = tempnode->m_GeomStandData.GeoEle_start_x1;
					y0_tran = tempnode->m_GeomStandData.GeoEle_start_y1;
				}


				tempnode = tempnode->nextGeomeleNode;

			}
			//pTheFirstLevelCloseHead是全局变量，下面代码是表示:
			//如果第一层的封闭环遍历完了，那么就该让水刀回到机床原点。
			if (NULL == pTheFirstLevelCloseHead->nextGeomcloseNode)
			{
				x1 = 0.0;
				y1 = 0.0;
				typegeomele = 4;//空跑直线
				outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1 << "    " << y1 << endl;
			}
			
	}



