#if !defined(AFX_DRAWINGVIEW_H__7F83BA52_1753_4DD9_A95C_FB756C26F8C2__INCLUDED_)
#define AFX_DRAWINGVIEW_H__7F83BA52_1753_4DD9_A95C_FB756C26F8C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingView.h : header file
//

//#include "CadLib.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawingViewStatic window

class CDrawingViewStatic : public CStatic
{

public:
	CDrawingViewStatic();
	virtual ~CDrawingViewStatic();
	void SavePictur(char* filename);

public:

	//CDrawing* m_pDrawing;
	// Generated message map functions

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	CPoint PrevMousePnt;
	bool m_Pan;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGVIEW_H__7F83BA52_1753_4DD9_A95C_FB756C26F8C2__INCLUDED_)
