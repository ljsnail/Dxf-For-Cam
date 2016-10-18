
// NestDxfDataForCutDlg.h : 头文件
//

#pragma once

#include "GeomForCut.h"
#include "GEOMELE.h"
#include "DrawingViewStatic.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "GLAUX.H"
#include "afxwin.h"

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
public:

	void switchkeyword(CString path);//按照路径搜索关键字LINE ARC CIRCLE，去匹配图元,
public:
	enum{LINE,ARC,CIRCLE};//使三种类型分开
	int symbol;//当进入寻找循环的时候，用它来做判断存储
	CString path;//保存打开的文件名
	GeomStandData m_geomstanddata;//实例化一个存储数据结构
	GLINE m_line;//全局的m_line变量
	GARC m_arc;//全局的m_arc变量
	GCIRCLE m_circle;//全局的m_circle变量
	int m_typegeomele = -1;//区分三种图元的类型
	CString m_readgeomele;//存储读取到的dxf的数据
public://链表相关
	GEOMELE m_geomele;//因为要运用到图元的操作，实例化这个类
	GeomClose m_GeomClose;//因为要运用到图元的操作，实例化这个类
	GeomForCut m_GeomForCut;
	int BatchHEAD_ID;//生产批次
	int NestResult_ID;//排样结果图纸
	int GEOMCLOSE_ID;//封闭环的个数
	BatchHEAD*m_pBatchHead;//将其划为全局变量，统一保存起来
	NestResultDataNode*m_pNestrsltdtND;//指向封闭环双向链表的头结点
	GeomCloseHEAD*m_pGeomclsHead;//指向基本图元双向链表的头结点
	GeomEleNode*m_pGeomEleND;//基本图元双向链表的头结点
	//对于圆而言应该是另外的一条路
	NestResultDataNode*m_pNestNode_forCircle;
	GeomCloseHEAD*m_pGeomclsHead_forCircle;//指向基本图元双向链表的头结点
public:
	//三个参数的意义symbol：已经转为int型的dxf字符串数据，m_readgeomele：读取的dxf数据存储池，m_dxfofnestresult：从哪个文件里面读取
	GLINE AcceptDxfLineData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//输入DXF文件中的标识输入，输出GLINE的数据
	GARC AcceptDxfArcData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//输入DXF文件中的标识输入，输出GARC的数据
	GCIRCLE AcceptDxfCircleData(int symbol, CString m_readgeomele, CStdioFile &m_dxfofnestresult);//输入DXF文件中的标识输入，输出GCIRCLE的数据
	afx_msg void Onstart();
	afx_msg void OnSavefile();
public:
	//查找不同封闭环的数据结点，调整封闭环双向链表的挂接
	bool AdjustGeomCloseNode(NestResultDataNode*head);//当数据处理完毕，才能保存和输出
	GeomEleNode*m_pDiffGeomclsDataNode;//不同封闭环的数据结点
	GeomEleNode*m_pTempGeomDataNode;
	bool m_IfDataDisposed;//数据是否已经处理完毕
	
	afx_msg void OnSimulation();
	//opengl

	BOOL SetWindowPixelFormat(HDC hDC); //设定象素格式
	BOOL CreateViewGLContext(HDC hDC); //View GL Context
	//void RenderScene(); //绘制场景
	afx_msg void OnTimer(UINT nIDEvent);
public:
	CStatic m_Draw;
	HDC		hrenderDC; //DC
	HGLRC	hrenderRC; //RC
	float m_yRotate; //转速
	int PixelFormat;
	GLINE ReadDataForOGL(GeomEleNode*node);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSavearc();
	afx_msg void Savtran();
	afx_msg void Savecircle();
};