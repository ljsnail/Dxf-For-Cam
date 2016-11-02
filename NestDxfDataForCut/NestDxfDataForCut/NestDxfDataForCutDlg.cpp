

// NestDxfDataForCutDlg.cpp : ʵ���ļ�
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
//������

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CNestDxfDataForCutDlg �Ի���



CNestDxfDataForCutDlg::CNestDxfDataForCutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNestDxfDataForCutDlg::IDD, pParent)//���캯��
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	BatchHEAD_ID = 0;//��ʼ��ʱ�򣬻�û������
	NestResult_ID = 0;//��ʼ������û��ͼֽ����
	GEOMCLOSE_ID = 0;//��ʼ������û�з�ջ�
	m_pBatchHead = NULL;//δָ���κεط�
	m_pNestrsltdtND = NULL;
	m_pGeomclsHead=NULL;
	m_IfDataDisposed = false;
	m_NewDxf = true;//��ͼֽ

	//Onstart();//�������Ч�ڵ����start��button
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


// CNestDxfDataForCutDlg ��Ϣ�������

BOOL CNestDxfDataForCutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//����OpenGL
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNestDxfDataForCutDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	////�������Ҳ�������
	//ValidateRect(NULL);//������ô���һ��ʼ�Ͳ�����ʾ�κζ���
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNestDxfDataForCutDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��������,��ʼ��һϵ�е�������
void CNestDxfDataForCutDlg::Onstart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BatchHEAD_ID++;//������������ô��Ʒ���ξ���һ��
	m_pBatchHead = m_GeomForCut.CreatBatchHEAD(BatchHEAD_ID);//�������ν��г�ʼ������

}

void CNestDxfDataForCutDlg::OnOpenFile()//��һ�ξ���һ��ͼֽ
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ͼֽ����ͼֽ���
    m_NewDxf= true;//��ͼֽ
	NestResult_ID++;
	m_pNestrsltdtND = m_GeomForCut.CreatNestResultNode(NestResult_ID);//�ڳ�ʼ��ʱ���¼���Ǵ򿪵ĵڼ���ͼֽ
	m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestrsltdtND);//�������ɵĽ�������������ָ���ͼֽ˫��������
	//����һ��ֻ���Բ���������ͼ���
	m_pNestNode_forCircle = m_GeomForCut.CreatNestResultNode(NestResult_ID);//������һ��ͼֽ��ID����Ϊ����ֻ���Ȼ�һ����������Բ��ID����Ҫ�ı�
	//��ֻ���Բ���������ͼ���ҵ��������ν����
	m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestNode_forCircle);//�µ�ͼֽ˫�����������ͬһ��������Fͷ�����

	//������¼ͼԪ����Fͷ���
	GEOMCLOSE_ID++;
	m_pGeomclsHead = m_GeomClose.CreatGeomCloseHEAD(GEOMCLOSE_ID);
	//�ѷ�ջ����ҵ��������ͼ�����
	m_pNestrsltdtND = m_GeomForCut.AddGeomCloseHeadNode(m_pNestrsltdtND, m_pGeomclsHead);//����ָ���ջ���������dxf���ͼ��㣬�ͷ�ջ���㣬�ѷ�ջ����ҵ�����dxf���ͼ�ϣ������Ѿ�����������ṹ

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
	//�����Ѿ����������DXF���������ȫ����ȡ�ˣ�������Ҫ�����ݽ��У���ջ��ֿ��ҵ���ͬ�ķ�ջ�ͷ�����
	AdjustGeomCloseNode(m_pNestrsltdtND);

}
//���մ򿪵��ļ���·��ȥ����LINE ARC CIRCLE
void CNestDxfDataForCutDlg::switchkeyword(CString path)
{
	//bool a=false;
	CStdioFile m_dxfofnestresult(path, CFile::modeRead);//�����ļ�·����ֻ��
	//a=m_dxfofnestresult.ReadString(m_readgeomele);//ÿ�ζ�һ�У���ȡһ��֮��ָ����һ�У��Ż�bool
	while (m_dxfofnestresult.ReadString(m_readgeomele))//������ǿ��ļ�
	//while (a)
	{
		//�����LINE ARC CIRCLE�������ؼ���֮���һ��
		//a = m_dxfofnestresult.ReadString(m_readgeomele);
		while ((strcmp(m_readgeomele, "LINE") == 0) || (strcmp(m_readgeomele, "ARC") == 0) || (strcmp(m_readgeomele, "CIRCLE") == 0))
		{
			if (strcmp(m_readgeomele, "LINE") == 0) m_typegeomele = 0;
			if (strcmp(m_readgeomele, "ARC") == 0) m_typegeomele = 1;
			if (strcmp(m_readgeomele, "CIRCLE") == 0) m_typegeomele = 2;
			switch (m_typegeomele)
			{
			case LINE://�鵽�Ĺؼ���ΪLINE��ʱ������ֻ�Ƕ�����LINE����Ҫ������
					m_dxfofnestresult.ReadString(m_readgeomele);//������ﲻ����һ�У���ô����LINE����ô�ͽ���ȥ�����while����һֱ����LINE,��������һ��while�γ���ѭ����
					//ÿ����һ��LINE֮�������������һ��LINE��ARC,CIRCLE�����������������LINEѭ��
					//����������ֻ�в���LINE ,ARC,CIRCLE��ENDSEC���ܽ��룬����������LINE ,ARC,CIRCLE��ENDSEC���ܳ�������LINE ,ARC,CIRCLE��ENDSEC���ų�����
					while ((strcmp(m_readgeomele, "LINE") != 0) && (strcmp(m_readgeomele, "ARC") != 0) && (strcmp(m_readgeomele, "CIRCLE") != 0)  && (strcmp(m_readgeomele, "ENDSEC") != 0))//���ǼӶ���һ��(strcmp(m_readgeomele, "ENDSEC") != 0)���while��������ȥ��
					{
						m_dxfofnestresult.ReadString(m_readgeomele);//��DXF������ַ�����Ϣ����m_readgeomele
						symbol = atoi(m_readgeomele);//��m_readgeomele������ַ���תΪ������ֵsymbol
						m_line = AcceptDxfLineData(symbol, m_readgeomele, m_dxfofnestresult);//����symbol�����ԴͼԪ����ֹ���꣬
					}//�뿪�����ʱ���Ѿ�ѭ����һ��LINE��
					//���ȴ�ǵ����������ʱ��m_lineΪ0�ˣ�֮ǰ��ֵȫ��û�б�������������һ�δα����ǣ�Ҫ����취���м�Ĳ�������������
					m_geomstanddata = m_geomele.ReadLineData(m_line);//����ԴͼԪ����ֹ���꣬���һ����׼��ͼԪ�����ݸ�ʽ
					//������¼����ͼԪ��˫������,�ѻ�õ����ݱ�������
					m_pGeomEleND = m_GeomClose.CreatGeomEleNode(m_geomstanddata);//���������õ����ݣ�����һ����㱣������
					m_pGeomclsHead = m_GeomClose.InsertGeomEleNode(m_pGeomclsHead, m_pGeomEleND, m_geomstanddata);//�Ѵ����Ļ���ͼԪ���ҵ���m_pGeomclsHeadָ���˫�������С�
					break;//����cade LINE���¼�
			case ARC:
				m_dxfofnestresult.ReadString(m_readgeomele);
				while ((strcmp(m_readgeomele, "LINE") != 0) && (strcmp(m_readgeomele, "ARC") != 0) && (strcmp(m_readgeomele, "CIRCLE") != 0) && (strcmp(m_readgeomele, "ENDSEC") != 0))//���ǼӶ���һ��(strcmp(m_readgeomele, "ENDSEC") != 0)���while��������ȥ��
				{
					m_dxfofnestresult.ReadString(m_readgeomele);//��DXF������ַ�����Ϣ����m_readgeomele
					symbol = atoi(m_readgeomele);//��m_readgeomele������ַ���תΪ������ֵsymbol
					m_arc= AcceptDxfArcData(symbol, m_readgeomele, m_dxfofnestresult);//����symbol�����ԴͼԪ����ֹ���꣬
				}//�뿪�����ʱ���Ѿ�ѭ����һ��LINE��
				//���ȴ�ǵ����������ʱ��m_lineΪ0�ˣ�֮ǰ��ֵȫ��û�б�������������һ�δα����ǣ�Ҫ����취���м�Ĳ�������������
				m_geomstanddata = m_geomele.ReadArcData(m_arc);//����ԴͼԪ����ֹ���꣬���һ����׼��ͼԪ�����ݸ�ʽ
				//������¼����ͼԪ��˫������,�ѻ�õ����ݱ�������
				m_pGeomEleND = m_GeomClose.CreatGeomEleNode(m_geomstanddata);//���������õ����ݣ�����һ����㱣������
				m_pGeomclsHead = m_GeomClose.InsertGeomEleNode(m_pGeomclsHead, m_pGeomEleND, m_geomstanddata);//�Ѵ����Ļ���ͼԪ���ҵ���m_pGeomclsHeadָ���˫�������С�
				break;
			case CIRCLE:
				m_dxfofnestresult.ReadString(m_readgeomele);
				while ((strcmp(m_readgeomele, "LINE") != 0) && (strcmp(m_readgeomele, "ARC") != 0) && (strcmp(m_readgeomele, "CIRCLE") != 0) && (strcmp(m_readgeomele, "ENDSEC") != 0))
				{
					m_dxfofnestresult.ReadString(m_readgeomele);//�Ӳ������д���ȡ���ڶ�ȡ�����ݽṹģʽ��Ҫ���ܲ��ܼ�
					symbol = atoi(m_readgeomele);
					m_circle = AcceptDxfCircleData(symbol,m_readgeomele, m_dxfofnestresult);
				}
				m_geomstanddata = m_geomele.ReadCircleData(m_circle);
				//������¼����ͼԪ��˫������,�ѻ�õ����ݱ�������
				//Բ������������Ϊ���������һ����ջ�,�������ҵ���һ���������ͼ�У���ȡ������֮���ٹһ���
				GEOMCLOSE_ID++;//�Լ��Ϳ�������һ����ջ�
				//////////////////////////////////////////////////////////////////////////////////
				////����һ���������ͼ���
				//m_pNestNode_forCircle = m_GeomForCut.CreatNestResultNode(NestResult_ID);//������һ��ͼֽ��ID����Ϊ����ֻ���Ȼ�һ����������Բ��ID����Ҫ�ı�
				////��ͼ���ҵ��������ν����
				//m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestNode_forCircle);//�µ�ͼֽ˫�����������ͬһ��������Fͷ�����
				////����һ����ջ����
				////////////////////////////////////////////////////////////////////////////////////
				m_pGeomclsHead_forCircle = m_GeomClose.CreatGeomCloseHEAD(GEOMCLOSE_ID);
				//�ѷ�ջ����ҵ��������ͼ�����
				m_pNestNode_forCircle = m_GeomForCut.AddGeomCloseHeadNode(m_pNestNode_forCircle, m_pGeomclsHead_forCircle);//����ָ���ջ���������dxf���ͼ��㣬�ͷ�ջ���㣬�ѷ�ջ����ҵ�����dxf���ͼ�ϣ������Ѿ�����������ṹ
				//����һ�����ݽ��
				m_pGeomEleND = m_GeomClose.CreatGeomEleNode(m_geomstanddata);//���������õ����ݣ�����һ����㱣������
				//�����ݽ��ҵ���ջ������
				m_pGeomclsHead_forCircle = m_GeomClose.InsertGeomEleNode(m_pGeomclsHead_forCircle, m_pGeomEleND, m_geomstanddata);//�Ѵ����Ļ���ͼԪ���ҵ���m_pGeomclsHeadָ���˫�������С�
				break;
			default:break;//����switch (m_typegeomele)�¼�
			}
			//����Ҳ��Ӧ��break������������Ҳ�������ˡ�while�ǻ���ѭ���ģ�ÿһ�ζ���ȥ�ж��£������ֵ��
			//break;//������һ���ؼ���֮�����һ���ˣ��������ڶ���while���ж������ؼ��ֵ��Ǹ�while�ӵ�һ��while����ȥ��ʼ�ڶ��ֵ�ѭ����Ҳ������m_dxfofnestresult.ReadString(m_readgeomele);�����ڲ�ѭ������������Զ������ѭ���ˡ�
		}
		//break;//��Ӧ������������ֻ��һ�ξͽ�����
	}//û��������һ��ѭ������䣬����һֱ����ѭ��ָ���ļ�ĩβ��

	

}
//����dxf����ķ��ţ����dxf����ֱ�ߵ���ֹ��
GLINE CNestDxfDataForCutDlg::AcceptDxfLineData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult)//ǰ�������CNestDxfDataForCutDlg��ʹ������һЩm_line���Ա�ʹ��
{
	//GLINE m_line = {0.0,0.0,0.0,0.0};//ÿһ�ν�������ʼ��һ�Σ���ʼ����Ӧ��������
	switch (symbol)//ֻ����һ��
	{
	case 10://��ʼ��x
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.x0 = atof(m_readgeomele);
		break;
	case 20://��ʼ��y
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.y0 = atof(m_readgeomele);
		break;
	case 11://��ֹ��x
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.x1 = atof(m_readgeomele);
		break;
	case 21://��ֹ��y
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_line.y1 = atof(m_readgeomele);
		break;
	default://����������������
		break;
	}
	return m_line;
}
GARC CNestDxfDataForCutDlg::AcceptDxfArcData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult)
{
	
	switch (symbol)
	{
	case 10://Բ��x
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_arc.Arccent_x = atof(m_readgeomele);
		break;
	case 20://Բ��y
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_arc.Arccent_y = atof(m_readgeomele);
		break;
	case 40://�뾶r
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_arc.m_Arc_r = atof(m_readgeomele);
		break;
	case 50://��ʼ��
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_arc.m_ArcAngle_start = atof(m_readgeomele);
		break;
	case 51://��ֹ��
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
	case 10://Բ��x
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_circle.m_Circent_x = atof(m_readgeomele);
		break;
	case 20://Բ��y
		m_dxfofnestresult.ReadString(m_readgeomele);
		m_circle.m_Circent_y = atof(m_readgeomele);
		break;
	case 40://�뾶r
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
	//�����Ѿ����������DXF���������ȫ����ȡ�ˣ�������Ҫ�����ݽ��У���ջ��ֿ��ҵ���ͬ�ķ�ջ�ͷ�����
	int m_GeomCloseID;//�����˵ķ�ջ��ĸ���
	GeomEleNode*temp;
	GeomCloseHEAD*Htemp, *m_FindTheEndClosed, *m_CircleFirstClosed=NULL;
	int m_MaxNumOfGeomClose, m_MAXNumOfGeomClose;

	
	
	temp = head->FirstGeomClose->FirstGeomele;//��ԭʼ����ĵ�һ�����ݽ��
	while (temp->nextGeomeleNode)//�ҵ���ԭʼ��������һ�����ݽ��
		temp = temp->nextGeomeleNode;
	m_MaxNumOfGeomClose = temp->m_NumGeomCloseID;//��������ԭʼ���ݽ��˫�����������һ�����ķ�ջ����ǳ���Բ֮�����ķ�ջ�����
	GeomCloseHEAD*m_NoIntactGeomCloseHead = NULL;
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_MaxNumOfGeomClose; m_GeomCloseID++)//��֤ȫ������Ѿ�������һ�飬�Ҹ�������������
	{
			m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
			m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);
	}
	//���Ѿ��ֿ���ͬ��ջ������ݽ��������ջ�������
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_MaxNumOfGeomClose; m_GeomCloseID++)//��֤ȫ������Ѿ�������һ�飬�Ҹ�������������
	{
		m_NoIntactGeomCloseHead = m_GeomForCut.JudgeGeomCloseIntact(m_pNestrsltdtND);//�жϷ�ջ��Ƿ�����
		m_pNestrsltdtND = m_GeomForCut.Find_Change_GeomCloseHEAD(m_pNestrsltdtND, m_NoIntactGeomCloseHead);//���벻�����ķ�ջ���㣬Ѱ����Եķ�ջ���㣬���ڲ�������
	}
	//�ٽ���ջ�����һ�飬����ͬ�ķֳ�ȥ
	for (m_GeomCloseID = 1; m_GeomCloseID <= m_MaxNumOfGeomClose; m_GeomCloseID++)//��֤ȫ������Ѿ�������һ�飬�Ҹ�������������
	{
		m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
		m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);
	}
	//���Ѿ��ֿ���ͬ��ջ������ݽ��������ջ�������
	//����Ӧ�����Ѿ���֤�˷�ջ����ڲ�ͬ�ķ�ջ�Fͷ���ָ���˫�������ϣ�����ÿһ����ջ�����ڵ����ݽ�㶼��������
	//���»��з�ջ�֮����������⣬��ջ�֮��Ĺ��������⣬Բ���عҵ����⡣
	//��ջ��ڵ����ݽ�㴦������
	//Ҫ��Բ���뵽����ͼ����
	if ((m_pNestrsltdtND->nextNestResultDataNode->FirstGeomClose))//�����Բ������ͼ
	{
		m_CircleFirstClosed = m_pNestrsltdtND->nextNestResultDataNode->FirstGeomClose;//ȫ����Բ���Ǹ���ջ�˫������ĵ�һ����ջ�ͷ���
		m_FindTheEndClosed = m_pNestrsltdtND->FirstGeomClose;

		while (m_FindTheEndClosed->nextGeomcloseNode)//�ҵ����һ��ֱ��Բ����ɵķ�ջ�
		{
			m_FindTheEndClosed = m_FindTheEndClosed->nextGeomcloseNode;
		}
		m_FindTheEndClosed->nextGeomcloseNode = m_CircleFirstClosed;
		m_CircleFirstClosed->prevGeomcloseNode = m_FindTheEndClosed;
		while (m_CircleFirstClosed->nextGeomcloseNode)//�ҵ�Բ��ջ������һ����ջ�
		{
			m_CircleFirstClosed = m_CircleFirstClosed->nextGeomcloseNode;

		}
		m_MAXNumOfGeomClose = m_MaxNumOfGeomClose + m_CircleFirstClosed->GEOMCLOSE_ID;
		m_FindTheEndClosed->GEOMCLOSE_ID = m_MAXNumOfGeomClose;//�����Ϳ��԰����еķ�ջ����������һ��Բ��ջ�������
		//�԰�����Բ���ٴ���һ��
		for (m_GeomCloseID = 1; m_GeomCloseID <= m_MAXNumOfGeomClose; m_GeomCloseID++)//��֤ȫ������Ѿ�������һ�飬�Ҹ�������������
		{
			m_pDiffGeomclsDataNode = m_GeomForCut.FindDiffGeomCloseNode(m_pNestrsltdtND);
			m_GeomForCut.InsertGeomCloseHEAD(m_pNestrsltdtND, m_pDiffGeomclsDataNode);
		}
		m_pNestrsltdtND->nextNestResultDataNode->FirstGeomClose = NULL;//��ȻԲ�Ѿ����������ͷ�����˳����ˣ���ô�͸����������Ϊ��
	}
	
	//�����Լ������еķ�ջ�������ˡ�
	//m_GeomForCut.ChangeEleNodeOfGeomClosed_origin(m_pNestrsltdtND);//��ϧ�����û�����κ�Ч����ԭ��֮һ�����Ƿ�ջ������û�з����������Ǵ�����㷨�����⡣
	//m_GeomForCut.ChangClosedNodeOfNRDXF(m_pNestrsltdtND);
	//m_GeomForCut.ChangeEleNodeOfGeomClosed_order(m_pNestrsltdtND);
	//////��һ�ִ���ʽ
	//��̰���㷨�����еķ�ջ���������ʼ˳��
	m_GeomForCut.BaseTS_GR_ForCutPathPlan(head);
	//���ֳ���ͬ�ķ�ջ����
	//m_GeomForCut.Find_AdjustNestCloseHead(head);
	////����Ⱥ�㷨�Ե�һ���ջ�����·���滮���Ż�
	//m_GeomForCut.Base_ACO_ForFirstCHead(head);
	////������Ⱥ�㷨������ĵ�һ���ջ�������ÿ����ջ�Ⱥ������ӷ�ջ�˳�򣬲�������Ӧ�ĸ���ջ���׵�
	//m_GeomForCut.BaseTS_GR_ForKidCHead(head);
	//

	m_IfDataDisposed = true;
	return m_IfDataDisposed;
}
void CNestDxfDataForCutDlg::OnSavefile()
{
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GeomCloseHEAD*Htemp;
	GeomEleNode*tempnode;
	int typegeomele;
	double x0, x1, y0, y1, Arccent_x, Arccent_y, r, temp;

	USES_CONVERSION;
	DWORD dFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	CFileDialog saveDlg(FALSE, NULL, NULL, dFlags, _T("Txt Files (*.txt)|*.txt||"), NULL);
		if (m_IfDataDisposed)//���ݴ������ˣ������������
		{
			if (saveDlg.DoModal() == IDOK)
			{
				// the txt file path.
				CString strFilePath = saveDlg.GetPathName();
				strFilePath += _T(".txt");
				string strDxfPath = strFilePath;
				ofstream outfile(strDxfPath.c_str());
				Htemp = m_pNestrsltdtND->FirstGeomClose;//��һ����ջ�F���
				while (Htemp)//ȫ������
				{
					tempnode = Htemp->FirstGeomele;//��ջ���ĵ�һ�����ݽ��
					while (tempnode)//ȫ��������
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
		AfxMessageBox((_T("��ȴ����ݴ�����ϣ�"), MB_YESNO) == IDYES);
	}

}

void CNestDxfDataForCutDlg::OnSimulation()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

void CNestDxfDataForCutDlg::OnTimer(UINT nIDEvent) //ʵʱ���Ƴ���
{
	
	// TODO: Add your message handler code here and/or call default
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -6.0f); // Move Left 1.5 Units And Into The Screen 6.0
	//glRotated(m_yRotate, 0.0, 1.0, 0.0);
	//����
	GeomCloseHEAD*Htemp;
	GeomEleNode*tempnode;
	GLINE m_line;
	Htemp = m_pNestrsltdtND->FirstGeomClose;//��һ����ջ�F���
	while (Htemp)//ȫ������
	{
		tempnode = Htemp->FirstGeomele;//��ջ���ĵ�һ�����ݽ��
		while (tempnode)//ȫ��������
		{
			//////////opengl///////////////////////////
			m_line = ReadDataForOGL(tempnode);
			glBegin(GL_LINE_STRIP);//����
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
		// �˹���Ҫ�� Windows Vista ����߰汾��
		// _WIN32_WINNT ���ű��� >= 0x0600��
		// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
		// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		GeomCloseHEAD*Htemp;
		GeomEleNode*tempnode;
		int typegeomele;
		double x0, x1, y0, y1, Arccent_x, Arccent_y, r, temp, Angle_start, Angle_end;
		if (m_IfDataDisposed)//���ݴ������ˣ������������
		{
			ofstream outfile("I:\\MATLAB\\DXF\\��͵�21.txt");
			Htemp = m_pNestrsltdtND->FirstGeomClose;//��һ����ջ�F���
			while (Htemp)//ȫ������
			{
				tempnode = Htemp->FirstGeomele;//��ջ���ĵ�һ�����ݽ��
				while (tempnode)//ȫ��������
				{
					
					switch (tempnode->m_GeomStandData.m_typegeomele)
					{
					case 1:

						 typegeomele = 1;//ʵ��ֱ��
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
						 //���ڻ����X0>X1�������²��ܻ���ͼ��
					x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
					x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
					Angle_start = tempnode->m_GeomStandData.m_arc.m_ArcAngle_start;
					Angle_end = tempnode->m_GeomStandData.m_arc.m_ArcAngle_end;
					if (x0>x1)
					{
						/*temp = Angle_start;
						Angle_start = Angle_end;
						Angle_end = temp;*/
						typegeomele = 4;//������Բ��
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
			AfxMessageBox((_T("��ȴ����ݴ�����ϣ�"), MB_YESNO) == IDYES);
		}

	}


	void CNestDxfDataForCutDlg::Savtran()
	{
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		GeomCloseHEAD*Htemp;
		GeomEleNode*tempnode;
		int typegeomele;
		double x0, x1, y0, y1, Arccent_x, Arccent_y, r, temp, Angle_start, Angle_end, Angle_cut_start;
		double x0_tran=0,y0_tran=0, x1_tran, y1_tran;
		double Angle_add;//�����Ƕ�
		if (m_IfDataDisposed)//���ݴ������ˣ������������
		{
			//���ڶ�����ջ������һ����ջ�
			/*m_pNestrsltdtND = m_GeomForCut.ChangeSencondCH2FH(m_pNestrsltdtND);*/
			ofstream outfile("I:\\MATLAB\\DXF\\������01.txt");
			Htemp = m_pNestrsltdtND->FirstGeomClose;//��һ����ջ�F���
			while (Htemp)//ȫ������
			{
				tempnode = Htemp->FirstGeomele;//��ջ���ĵ�һ�����ݽ��
				x1_tran = tempnode->m_GeomStandData.GeoEle_start_x0;
				y1_tran = tempnode->m_GeomStandData.GeoEle_start_y0;
				typegeomele = 4;//����ֱ��
				outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1_tran << "    " << y1_tran << endl;

				while (tempnode)//ȫ��������
				{
						switch (tempnode->m_GeomStandData.m_typegeomele)
					{
					case 1://ֱ��
						
						typegeomele = 1;//ʵ��ֱ��
						x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
						x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
						y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
						y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
						outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
						break;
					case 2://Բ����Ҫ����̫�����ƿ���API�����Ӧ������
						typegeomele = 2;//ʵ��Բ��
						Angle_start = tempnode->m_GeomStandData.m_arc.m_ArcAngle_start;
						Angle_end = tempnode->m_GeomStandData.m_arc.m_ArcAngle_end;
						r = tempnode->m_GeomStandData.m_arc.m_Arc_r;
						Arccent_x = tempnode->m_GeomStandData.m_arc.Arccent_x;
						Arccent_y = tempnode->m_GeomStandData.m_arc.Arccent_y;
						if (tempnode->m_GeomStandData.m_IsTranData)//�����Բ������ֹ�Ƕ�
						{
							Angle_cut_start = Angle_end;
							if (Angle_start < Angle_end)//��ʼ��С����ֹ��
							{
								Angle_add = Angle_end - Angle_start;
								Angle_add = -Angle_add;//������λ�����Ǹ��������Ƕ�
							}
							else//��ʼ�Ǵ�����ֹ��
							{
								Angle_add = 360 - Angle_start + Angle_end;
								Angle_add = -Angle_add;//������λ�����Ǹ��������Ƕ�
							}
						}
						else//���û�е���Բ������ֹ�Ƕ�
						{
							Angle_cut_start = Angle_start;
							if (Angle_start < Angle_end)//��ʼ��С����ֹ��
				
							{
								Angle_add = Angle_end - Angle_start;								
							}
							else//��ʼ�Ǵ�����ֹ��
							{
								Angle_add = 360 - Angle_start + Angle_end;					
							}
						}		
						outfile << typegeomele << "    " << Angle_cut_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;
						break;
					case 3:
						typegeomele = 2;//ʵ��Բ��
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
					if (NULL == tempnode->nextGeomeleNode)//��������һ��,��ôҪ������ֵ��������
					{
						x0_tran = tempnode->m_GeomStandData.GeoEle_start_x1;
						y0_tran = tempnode->m_GeomStandData.GeoEle_start_y1;
					}


					tempnode = tempnode->nextGeomeleNode;
				
				}
				if (NULL == Htemp->nextGeomcloseNode)//��������һ��,��ôҪ������ֵ��������
				{
					x0 = x0_tran;
					y0 = y0_tran;
				}
				Htemp = Htemp->nextGeomcloseNode;
			}
			x1 = 0.0;
			y1 = 0.0;

			typegeomele = 4;//����ֱ��
			outfile << typegeomele << "    " << x0 << "    " << y0<< "    " << x1<< "    " << y1<< endl;


		}
		else
		{
			AfxMessageBox((_T("��ȴ����ݴ�����ϣ�"), MB_YESNO) == IDYES);
		}

	}


	void CNestDxfDataForCutDlg::Savecircle()
	{
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		GeomCloseHEAD*Htemp;
		GeomEleNode*tempnode;
		NestResultDataNode*DxfTempHead;
		int typegeomele;
		int m_DxfResult_ID=0;
		double x0, x1, y0, y1, Arccent_x, Arccent_y, r, temp, Angle_start, Angle_end, Angle_cut_start;
		double x0_tran , y0_tran , x1_tran, y1_tran;
		double Angle_add;//�����Ƕ�
		bool m_ifOneDxfResultCuted=false;
		USES_CONVERSION;
		DWORD dFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
		CFileDialog saveDlg(FALSE, NULL, NULL, dFlags, _T("Txt Files (*.txt)|*.txt||"), NULL);
		if (m_IfDataDisposed)//���ݴ������ˣ������������
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
					//ÿ�δ����µ�ͼֽʱ����λ0��
					x0_tran = 0;
					y0_tran = 0;
					Htemp = DxfTempHead->FirstGeomClose;//��һ����ջ�F���
					if (Htemp)//������Բռ��֮��Ǩ�����µĿձ�
					{
						while (Htemp)//ȫ������
						{
							tempnode = Htemp->FirstGeomele;//��ջ���ĵ�һ�����ݽ��
							x1_tran = tempnode->m_GeomStandData.GeoEle_start_x0;
							y1_tran = tempnode->m_GeomStandData.GeoEle_start_y0;
							typegeomele = 4;//����ֱ��
							outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1_tran << "    " << y1_tran << endl;

							while (tempnode)//ȫ��������
							{
								switch (tempnode->m_GeomStandData.m_typegeomele)
								{
								case 1://ֱ��

									typegeomele = 1;//ʵ��ֱ��
									x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
									x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
									y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
									y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
									outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
									break;
								case 2://Բ����Ҫ����̫�����ƿ���API�����Ӧ������
									typegeomele = 2;//ʵ��Բ��
									Angle_start = tempnode->m_GeomStandData.m_arc.m_ArcAngle_start;
									Angle_end = tempnode->m_GeomStandData.m_arc.m_ArcAngle_end;
									r = tempnode->m_GeomStandData.m_arc.m_Arc_r;
									Arccent_x = tempnode->m_GeomStandData.m_arc.Arccent_x;
									Arccent_y = tempnode->m_GeomStandData.m_arc.Arccent_y;
									if (tempnode->m_GeomStandData.m_IsTranData)//�����Բ������ֹ�Ƕ�
									{
										Angle_cut_start = Angle_end;
										if (Angle_start < Angle_end)//��ʼ��С����ֹ��
										{
											Angle_add = Angle_end - Angle_start;
											Angle_add = -Angle_add;//������λ�����Ǹ��������Ƕ�
										}
										else//��ʼ�Ǵ�����ֹ��
										{
											Angle_add = 360 - Angle_start + Angle_end;
											Angle_add = -Angle_add;//������λ�����Ǹ��������Ƕ�
										}
									}
									else//���û�е���Բ������ֹ�Ƕ�
									{
										Angle_cut_start = Angle_start;
										if (Angle_start < Angle_end)//��ʼ��С����ֹ��
										{
											Angle_add = Angle_end - Angle_start;
										}
										else//��ʼ�Ǵ�����ֹ��
										{
											Angle_add = 360 - Angle_start + Angle_end;
										}
									}
									outfile << typegeomele << "    " << Angle_cut_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;
									break;
								case 3:
									typegeomele = 2;//ʵ��Բ��
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
								if (NULL == tempnode->nextGeomeleNode)//��������һ��,��ôҪ������ֵ��������
								{
									x0_tran = tempnode->m_GeomStandData.GeoEle_start_x1;
									y0_tran = tempnode->m_GeomStandData.GeoEle_start_y1;
								}


								tempnode = tempnode->nextGeomeleNode;

								/*}
								Htemp = Htemp->nextGeomcloseNode;
								}*/
								if (NULL == Htemp->nextGeomcloseNode)//��������һ��,��ôҪ������ֵ��������
								{
									x0 = x0_tran;
									y0 = y0_tran;
								}
							}
						Htemp = Htemp->nextGeomcloseNode;
					}
					x1 = 0.0;
					y1 = 0.0;
					typegeomele = 4;//����ֱ��
					outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;


						m_ifOneDxfResultCuted = true;//һ��ͼֽ�Ѿ��������
						typegeomele = 99;//������ƻ������������ͣ�и�ͻ����ص���������ԭ�㣬�ʹ��ʹ����������
						outfile << typegeomele << endl;

					}
					DxfTempHead = DxfTempHead->nextNestResultDataNode;
				}
			}
		}
		else
		{
			AfxMessageBox((_T("��ȴ����ݴ�����ϣ�"), MB_YESNO) == IDYES);
		}

		 
	}
//���Ĵ���
//���Ĵ���
//���Ĵ���	
//��ȡ���ӷ�ջ�������
void CNestDxfDataForCutDlg::SaveNestCloseHead()
{
		// TODO:  �ڴ���ӿؼ�֪ͨ����������
		GeomCloseHEAD*Hkidtemp;
		GeomEleNode*tempnode;
		bool m_ifHvkidClose;
		
		//ofstream outfile("I:\\MATLAB\\DXF\\������01.txt");
		if (m_IfDataDisposed)//���ݴ������ˣ������������
			{

			pTheFirstLevelCloseHead = m_pNestrsltdtND->FirstGeomClose;//��һ����ջ�F���
			while (pTheFirstLevelCloseHead)//ȫ������
				{
					//Ҫ�ж��Ƿ����ӷ�ջ��� 
				m_ifHvkidClose = IfIncludeKidClose(pTheFirstLevelCloseHead);
					if (m_ifHvkidClose)
					{
						//��Ȼ���ӷ�ջ�����ô�����ӷ�ջ������һ�����Ƕ�׵��������ݶ���
						//�����˸���ջ�������
						ReadKidCloseHeadData(pTheFirstLevelCloseHead);
					}
					else//���û���ӷ�ջ��Ļ�
					{
						ReadCloseHeadData(pTheFirstLevelCloseHead);
					}
					pTheFirstLevelCloseHead = pTheFirstLevelCloseHead->nextGeomcloseNode;
				}
			}
		else
		{
			AfxMessageBox((_T("��ȴ����ݴ�����ϣ�"), MB_YESNO) == IDYES);
		}

}
	//�жϴ��Ƿ�������ӷ�ջ�
	bool  CNestDxfDataForCutDlg::IfIncludeKidClose(GeomCloseHEAD*pTemp)
	{
		if (pTemp->FirstInsideGCloseNode)
			return true;
		else
			return false;
	}
	//��ȡ��ջ��ڲ��ӷ�ջ�������
	//��������ǰ�������ջ������ݴ��ﵽ��ȫ����ȡ����
	void CNestDxfDataForCutDlg::ReadKidCloseHeadData(GeomCloseHEAD*pTemp)
	{
		GeomCloseHEAD*Htemp, *Hkidtemp, *HFattemp, *pfadtemph;
		GeomCloseHEAD*NextCHtemp;//��һ����ջ�ͷ���
		bool m_ifHvkidClose;
		bool m_ifotherHvkidClose;


		//��ȻpTemp�����ӷ�ջ��ģ���ô�͵��ȶ��ӷ�ջ�������
		Hkidtemp = pTemp->FirstInsideGCloseNode;


		//���while��Ҫ�ҵ������һ���ӷ�ջ�
		while (Hkidtemp->FirstInsideGCloseNode)//�ҵ�����һ�㣻
			Hkidtemp = Hkidtemp->FirstInsideGCloseNode;
		Htemp = Hkidtemp;
		while (Htemp)//����һ�������һ��
		{
			if (Htemp->FirstInsideGCloseNode)//����ֵܽڵ����ӽڵ�,��ô���������ͻ���ӷ�ջ��͸���ջ�һ�����
			{

				ReadKidCloseHeadData(Htemp);
			}
			else
			{
				ReadCloseHeadData(Htemp);
			}

			Htemp = Htemp->nextGeomcloseNode;
		}


		//��һ��ѭ�����Ѿ�������һ���ӷ�մ����꣬��ô����Ҫ������һ���ӷ�ջ�
		HFattemp = Hkidtemp->prevOutsideGCloseNode;
		int a = 0;
		while (HFattemp != pTemp)//�����������ȵĸ��ڵ��ʱ��
		{
			ReadCloseHeadData(HFattemp);

			NextCHtemp = HFattemp->nextGeomcloseNode;//��һ���ֵܷ�ջ����
			while (NextCHtemp)//ȫ������
			{
				//Ҫ�ж��Ƿ����ӷ�ջ��� 
				m_ifotherHvkidClose = IfIncludeKidClose(NextCHtemp);
				if (m_ifotherHvkidClose)
				{
					//��Ȼ���ӷ�ջ�����ô�����ӷ�ջ������һ�����Ƕ�׵��������ݶ���
					//�����˸���ջ�������
					ReadKidCloseHeadData(NextCHtemp);
				}
				else//���û���ӷ�ջ��Ļ�
				{
					ReadCloseHeadData(NextCHtemp);
				}
				NextCHtemp = NextCHtemp->nextGeomcloseNode;
			}
			HFattemp = HFattemp->prevOutsideGCloseNode;
		}
		//�����Ѿ��ѳ��˵�һ��ĸ��ڵ�֮��������ӽڵ�����ݶ��������ˣ�����Ҫ������ǵ�һ��ĸ��ڵ�
		ReadCloseHeadData(pTemp);
	}

//����һ��ջ�������
	void CNestDxfDataForCutDlg::ReadCloseHeadData(GeomCloseHEAD*Htemp)
	{
		GeomEleNode*tempnode;
		int typegeomele;
			ofstream outfile("I:\\MATLAB\\DXF\\������38.txt", ios_base::out | ios_base::app);//�򿪲�׷��
			tempnode = Htemp->FirstGeomele;//��ջ���ĵ�һ�����ݽ��
			x1_tran = tempnode->m_GeomStandData.GeoEle_start_x0;
			y1_tran = tempnode->m_GeomStandData.GeoEle_start_y0;
			typegeomele = 4;//����ֱ��
			if (m_NewDxf == true)//����������ͼƬ
			{
				x0_tran = 0.0;
				y0_tran = 0.0;
			}
			outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1_tran << "    " << y1_tran << endl;
			m_NewDxf = false;//��ͼֽ;//����һ��֮��㲻������ͼƬ
			while (tempnode)//ȫ��������
			{
				switch (tempnode->m_GeomStandData.m_typegeomele)
				{
				case 1://ֱ��

					typegeomele = 1;//ʵ��ֱ��
					x0 = tempnode->m_GeomStandData.GeoEle_start_x0;
					x1 = tempnode->m_GeomStandData.GeoEle_start_x1;
					y0 = tempnode->m_GeomStandData.GeoEle_start_y0;
					y1 = tempnode->m_GeomStandData.GeoEle_start_y1;
					outfile << typegeomele << "    " << x0 << "    " << y0 << "    " << x1 << "    " << y1 << endl;
					break;
				case 2://Բ����Ҫ����̫�����ƿ���API�����Ӧ������
					typegeomele = 2;//ʵ��Բ��
					Angle_start = tempnode->m_GeomStandData.m_arc.m_ArcAngle_start;
					Angle_end = tempnode->m_GeomStandData.m_arc.m_ArcAngle_end;
					r = tempnode->m_GeomStandData.m_arc.m_Arc_r;
					Arccent_x = tempnode->m_GeomStandData.m_arc.Arccent_x;
					Arccent_y = tempnode->m_GeomStandData.m_arc.Arccent_y;
					if (tempnode->m_GeomStandData.m_IsTranData)//�����Բ������ֹ�Ƕ�
					{
						Angle_cut_start = Angle_end;
						if (Angle_start < Angle_end)//��ʼ��С����ֹ��
						{
							Angle_add = Angle_end - Angle_start;
							Angle_add = -Angle_add;//������λ�����Ǹ��������Ƕ�
						}
						else//��ʼ�Ǵ�����ֹ��
						{
							Angle_add = 360 - Angle_start + Angle_end;
							Angle_add = -Angle_add;//������λ�����Ǹ��������Ƕ�
						}
					}
					else//���û�е���Բ������ֹ�Ƕ�
					{
						Angle_cut_start = Angle_start;
						if (Angle_start < Angle_end)//��ʼ��С����ֹ��
						{
							Angle_add = Angle_end - Angle_start;
						}
						else//��ʼ�Ǵ�����ֹ��
						{
							Angle_add = 360 - Angle_start + Angle_end;
						}
					}
					outfile << typegeomele << "    " << Angle_cut_start << "    " << Angle_add << "    " << r << "    " << Arccent_x << "    " << Arccent_y << endl;
					break;
				case 3:
					typegeomele = 2;//ʵ��Բ��
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
				if (NULL == tempnode->nextGeomeleNode)//��������һ��,��ôҪ������ֵ��������
				{
					x0_tran = tempnode->m_GeomStandData.GeoEle_start_x1;
					y0_tran = tempnode->m_GeomStandData.GeoEle_start_y1;
				}


				tempnode = tempnode->nextGeomeleNode;

			}
			//pTheFirstLevelCloseHead��ȫ�ֱ�������������Ǳ�ʾ:
			//�����һ��ķ�ջ��������ˣ���ô�͸���ˮ���ص�����ԭ�㡣
			if (NULL == pTheFirstLevelCloseHead->nextGeomcloseNode)
			{
				x1 = 0.0;
				y1 = 0.0;
				typegeomele = 4;//����ֱ��
				outfile << typegeomele << "    " << x0_tran << "    " << y0_tran << "    " << x1 << "    " << y1 << endl;
			}
			
	}



