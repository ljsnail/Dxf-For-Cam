
// NestDxfDataForCutDlg.h : 头文件
//

#pragma once

#include "GeomForCut.h"
#include "GEOMELE.h"

// CNestDxfDataForCutDlg 对话框
class CNestDxfDataForCutDlg : public CDialogEx
{
// 构造
public:
	CNestDxfDataForCutDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NESTDXFDATAFORCUT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenFile();
	void switchkeyword(CString path);//按照路径搜索关键字LINE ARC CIRCLE，去匹配图元
public:
	enum{LINE,ARC,CIRCLE};//使三种类型分开
	int symbol;//当进入寻找循环的时候，用它来做判断存储
	CString path;//保存打开的文件名
	GEOMELE m_geomele;//因为要运用到图元的操作
	GeomStandData m_geomstanddata;//实例化一个存储数据结构
	GLINE m_line ;
	int m_typegeomele = -1;//区分三种图元的类型
	CString m_readgeomele;//存储读取到的dxf的数据
	

public:
	//三个参数的意义symbol：已经转为int型的dxf字符串数据，m_readgeomele：读取的dxf数据存储池，m_dxfofnestresult：从哪个文件里面读取
	GLINE AcceptDxfLineData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//输入DXF文件中的标识输入，输出GLINE的数据
	GeomStandData ReadLineData(GLINE m_line);//输入读入的DXF数据，输出统一的图元数据
};
