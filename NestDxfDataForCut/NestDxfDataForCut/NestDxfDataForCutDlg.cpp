
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
#include <fstream>

#include <iostream>

#include <conio.h>
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
	m_MaxNumOfGeomClose = 0;
	m_IfDataDisposed = false;
	//Onstart();//�������Ч�ڵ����start��button
	
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
	NestResult_ID++;
	m_pNestrsltdtND = m_GeomForCut.CreatNestResultNode(NestResult_ID);//�ڳ�ʼ��ʱ���¼���Ǵ򿪵ĵڼ���ͼֽ
	m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestrsltdtND);//�������ɵĽ�������������ָ���ͼֽ˫��������
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
				//����һ���������ͼ���
				m_pNestNode_forCircle = m_GeomForCut.CreatNestResultNode(NestResult_ID);//������һ��ͼֽ��ID����Ϊ����ֻ���Ȼ�һ����������Բ��ID����Ҫ�ı�
				//��ͼ���ҵ��������ν����
				m_pBatchHead = m_GeomForCut.AddNestRsltDtNode(m_pBatchHead, m_pNestNode_forCircle);//�µ�ͼֽ˫�����������ͬһ��������Fͷ�����
				//����һ����ջ����
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
	//����Ӧ�����Ѿ���֤�˷�ջ����ڲ�ͬ�ķ�ջ�Fͷ���ָ���˫�������ϣ�����ÿһ����ջ�����ڵ����ݽ�㶼��������
	//���»��з�ջ�֮����������⣬��ջ�֮��Ĺ��������⣬Բ���عҵ����⡣
	//��ջ��ڵ����ݽ�㴦������
	m_GeomForCut.ChangeEleNodeOfGeomClosed(m_pNestrsltdtND);
	m_IfDataDisposed = true;
	return m_IfDataDisposed;
}
void CNestDxfDataForCutDlg::OnSavefile()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_IfDataDisposed)//���ݴ������ˣ������������
	{
		GeomCloseHEAD*Htemp;
		GeomEleNode*tempnode;
		ofstream outfile("C:\\Users\\BIRL\\Desktop\\��͵�.txt");
		Htemp = m_pNestrsltdtND->FirstGeomClose;//��һ����ջ�F���
		while (Htemp)//ȫ������
		{
			tempnode = Htemp->FirstGeomele;//��ջ���ĵ�һ�����ݽ��
			while (tempnode)//ȫ��������
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
			Htemp = Htemp->nextGeomcloseNode;
		}
		
	}
	else
	{
		AfxMessageBox((_T("��ȴ����ݴ�����ϣ�"), MB_YESNO) == IDYES);
	}

}