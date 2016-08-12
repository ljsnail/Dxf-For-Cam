
// NestDxfDataForCutDlg.h : ͷ�ļ�
//

#pragma once

#include "GeomForCut.h"
#include "GEOMELE.h"

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
	void switchkeyword(CString path);//����·�������ؼ���LINE ARC CIRCLE��ȥƥ��ͼԪ
public:
	enum{LINE,ARC,CIRCLE};//ʹ�������ͷֿ�
	int symbol;//������Ѱ��ѭ����ʱ�����������жϴ洢
	CString path;//����򿪵��ļ���
	GEOMELE m_geomele;//��ΪҪ���õ�ͼԪ�Ĳ���
	GeomStandData m_geomstanddata;//ʵ����һ���洢���ݽṹ
	GLINE m_line;//ȫ�ֵ�m_line����
	GARC m_arc;//ȫ�ֵ�m_arc����
	GCIRCLE m_circle;//ȫ�ֵ�m_circle����
	int m_typegeomele = -1;//��������ͼԪ������
	CString m_readgeomele;//�洢��ȡ����dxf������
	

public:
	//��������������symbol���Ѿ�תΪint�͵�dxf�ַ������ݣ�m_readgeomele����ȡ��dxf���ݴ洢�أ�m_dxfofnestresult�����ĸ��ļ������ȡ
	GLINE AcceptDxfLineData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//����DXF�ļ��еı�ʶ���룬���GLINE������
	GARC AcceptDxfArcData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//����DXF�ļ��еı�ʶ���룬���GARC������
	GCIRCLE AcceptDxfCircleData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//����DXF�ļ��еı�ʶ���룬���GCIRCLE������
};
