
// NestDxfDataForCutDlg.h : ͷ�ļ�
//

#pragma once

#include "GeomForCut.h"
#include "GEOMELE.h"
#include "DrawingViewStatic.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "GLAUX.H"
#include "afxwin.h"

// CNestDxfDataForCutDlg �Ի���
class CNestDxfDataForCutDlg : public CDialogEx
{
// ����
public:
	CNestDxfDataForCutDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NESTDXFDATAFORCUT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenFile();
public:

	void switchkeyword(CString path);//����·�������ؼ���LINE ARC CIRCLE��ȥƥ��ͼԪ,
public:
	enum{LINE,ARC,CIRCLE};//ʹ�������ͷֿ�
	int symbol;//������Ѱ��ѭ����ʱ�����������жϴ洢
	CString path;//����򿪵��ļ���
	GeomStandData m_geomstanddata;//ʵ����һ���洢���ݽṹ
	GLINE m_line;//ȫ�ֵ�m_line����
	GARC m_arc;//ȫ�ֵ�m_arc����
	GCIRCLE m_circle;//ȫ�ֵ�m_circle����
	int m_typegeomele = -1;//��������ͼԪ������
	CString m_readgeomele;//�洢��ȡ����dxf������
public://�������
	GEOMELE m_geomele;//��ΪҪ���õ�ͼԪ�Ĳ�����ʵ���������
	GeomClose m_GeomClose;//��ΪҪ���õ�ͼԪ�Ĳ�����ʵ���������
	GeomForCut m_GeomForCut;
	int BatchHEAD_ID;//��������
	int NestResult_ID;//�������ͼֽ
	int GEOMCLOSE_ID;//��ջ��ĸ���
	BatchHEAD*m_pBatchHead;//���仮Ϊȫ�ֱ�����ͳһ��������
	NestResultDataNode*m_pNestrsltdtND;//ָ���ջ�˫�������ͷ���
	GeomCloseHEAD*m_pGeomclsHead;//ָ�����ͼԪ˫�������ͷ���
	GeomEleNode*m_pGeomEleND;//����ͼԪ˫�������ͷ���
	//����Բ����Ӧ���������һ��·
	NestResultDataNode*m_pNestNode_forCircle;
	GeomCloseHEAD*m_pGeomclsHead_forCircle;//ָ�����ͼԪ˫�������ͷ���
public:
	//��������������symbol���Ѿ�תΪint�͵�dxf�ַ������ݣ�m_readgeomele����ȡ��dxf���ݴ洢�أ�m_dxfofnestresult�����ĸ��ļ������ȡ
	GLINE AcceptDxfLineData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//����DXF�ļ��еı�ʶ���룬���GLINE������
	GARC AcceptDxfArcData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//����DXF�ļ��еı�ʶ���룬���GARC������
	GCIRCLE AcceptDxfCircleData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//����DXF�ļ��еı�ʶ���룬���GCIRCLE������
	afx_msg void Onstart();
	afx_msg void OnSavefile();
public:
	//���Ҳ�ͬ��ջ������ݽ�㣬������ջ�˫������Ĺҽ�
	bool AdjustGeomCloseNode(NestResultDataNode*head);//�����ݴ�����ϣ����ܱ�������
	GeomEleNode*m_pDiffGeomclsDataNode;//��ͬ��ջ������ݽ��
	GeomEleNode*m_pTempGeomDataNode;
	bool m_IfDataDisposed;//�����Ƿ��Ѿ��������
	
	afx_msg void OnSimulation();
	//opengl

	BOOL SetWindowPixelFormat(HDC hDC); //�趨���ظ�ʽ
	BOOL CreateViewGLContext(HDC hDC); //View GL Context
	//void RenderScene(); //���Ƴ���
	afx_msg void OnTimer(UINT nIDEvent);
public:
	CStatic m_Draw;
	HDC		hrenderDC; //DC
	HGLRC	hrenderRC; //RC
	float m_yRotate; //ת��
	int PixelFormat;
	GLINE ReadDataForOGL(GeomEleNode*node);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSavearc();
	afx_msg void Savtran();
	afx_msg void Savecircle();
};