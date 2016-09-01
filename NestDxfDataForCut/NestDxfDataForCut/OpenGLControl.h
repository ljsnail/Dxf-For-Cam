#pragma once
#include "afxwin.h"
#include <gl/gl.h>
#include <gl/glu.h>

class COpenGLControl :
	public CWnd
{
public:
	COpenGLControl();
	virtual ~COpenGLControl();
public:
	UINT_PTR m_unpTimer;
private:
	/*******************/
	/* PRIVATE MEMBERS */
	/*******************/
	// Window information
	CWnd    *hWnd;
	HDC     hdc;
	HGLRC   hrc;
	int     m_nPixelFormat;
	CRect   m_rect;
	CRect   m_oldWindow;
	CRect   m_originalRect;
	double  m_fLastX, m_fLastY, m_fZoom, m_fPosX, m_fPosY, m_fRotX, m_fRotY;
public:
	bool m_bIsMaximized;
public:
	void oglCreate(CRect rect, CWnd *parent);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void oglInitialize(void);
	afx_msg void OnDraw(CDC *pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void oglDrawScene(void);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool m_ifsimulation;
};

