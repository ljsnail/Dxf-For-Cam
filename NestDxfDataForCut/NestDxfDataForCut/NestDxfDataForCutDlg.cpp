
// NestDxfDataForCutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NestDxfDataForCut.h"
#include "NestDxfDataForCutDlg.h"
#include "GeomForCut.h"
#include "afxdialogex.h" 
#include "io.h" 
#include "fcntl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	: CDialogEx(CNestDxfDataForCutDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
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




void CNestDxfDataForCutDlg::OnOpenFile()
{
	// TODO:  在此添加控件通知处理程序代码
	OPENFILENAME ofn = { 0 };

	ofn.Flags = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
	CFileDialog hFileDlg(TRUE, NULL, NULL, ofn.Flags, TEXT("DXF(*.*)|*.*|"), NULL);
	if (hFileDlg.DoModal() == IDOK)
	{
		path = hFileDlg.GetPathName();
		switchkeyword(path);
	}
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
					m_dxfofnestresult.ReadString(m_readgeomele);//如果这里不读下一行，那么还是LINE，那么就进不去下面的while
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
					break;//跳出cade LINE的事件
			default:break;//跳出switch (m_typegeomele)事件
			}
			//下面也不应该break跳出，否则，再也进不来了。while是会自循环的，每一次都会去判断下（）里的值。
			//break;//当进来一个关键字之后读完一遍了，则跳出第二个while即判断三个关键字的那个while从第一个while那里去开始第二轮的循环。也可以用m_dxfofnestresult.ReadString(m_readgeomele);让它内部循环但这样就永远跳不出循环了。
		}
		//break;//不应该跳出，否则只是一次就结束了
	}//没有跳出第一个循环的语句，所以一直在内循环指导文件末尾。

	

}
//输入dxf里面的符号，输出dxf里面直线的起止点
GLINE CNestDxfDataForCutDlg::AcceptDxfLineData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult)
{
	//GLINE m_line = {0.0,0.0,0.0,0.0};//每一次进来都初始化一次，初始化不应该在这里
	switch (symbol)//只进来一次
	{
	case 10:
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.x0 = atof(m_readgeomele);
		break;
	case 20:
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.y0 = atof(m_readgeomele);
		break;
	case 11:
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.x1 = atof(m_readgeomele);
		break;
	case 21:
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.y1 = atof(m_readgeomele);
		break;
	default://如果出现了其他情况
		break;
	}
	return m_line;

}