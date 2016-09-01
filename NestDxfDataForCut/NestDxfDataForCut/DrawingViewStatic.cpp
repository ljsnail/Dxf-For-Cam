// DrawingView.cpp : implementation file
//
#include "stdafx.h"
#include "DrawingViewStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawingView

CDrawingViewStatic::CDrawingViewStatic()
{
	//m_pDrawing = NULL;
	m_Pan = false;
}

CDrawingViewStatic::~CDrawingViewStatic()
{
}


BEGIN_MESSAGE_MAP(CDrawingViewStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingViewStatic message handlers
void CDrawingViewStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	HDC hdc = dc.GetSafeHdc();



	//if(m_pDrawing!=NULL)
	//{
	//	if(m_pDrawing->isOpen())
	//	{
	//		CDC tempdc;											//用于缓冲作图的内存DC
	//		CBitmap tempbitmap;									//内存中承载临时图象的位图
	//		RECT rect;
	//		GetClientRect(&rect);
	//		rect.left+=1;
	//		rect.top+=1;
	//		rect.bottom-=2;
	//		rect.right-=2;
	//		tempdc.CreateCompatibleDC(&dc);						//依附窗口DC创建兼容内存DC
	//		tempbitmap.CreateCompatibleBitmap(&dc, rect.right-rect.left, rect.bottom-rect.top);//创建兼容位图
	//		tempdc.SelectObject(&tempbitmap);					//将位图选择进内存DC
	//		tempdc.FillSolidRect(&rect, dc.GetBkColor());		//按原来背景填充客户区，不然会是黑色
	//		m_pDrawing->Paint(tempdc.m_hDC);
	//		dc.BitBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, &tempdc, rect.left, rect.top, SRCCOPY);
	//		tempdc.DeleteDC();									//删除DC		
	//		tempbitmap.DeleteObject();							//删除位图		
	//	}
	//	else
	//	{
	//		CRect r;
	//		GetClientRect(r);
	//		dc.SelectObject(GetStockObject(NULL_BRUSH));
	//		dc.Rectangle(r);
	//	}
	//}
}

//jpg
void CDrawingViewStatic::SavePictur(char* filename)
{
	CPaintDC dc(this); // device context for painting
/*
	if(m_pDrawing!=NULL)
	{
		if(m_pDrawing->isOpen())
		{
			CDC tempdc;											//用于缓冲作图的内存DC
			CBitmap tempbitmap;								//内存中承载临时图象的位图
			RECT rect;
			GetClientRect(&rect);
			rect.left+=1;
			rect.top+=1;
			//   			rect.bottom-=2;
			//  			rect.right-=1;
			int width = rect.right-rect.left;
			int height = rect.bottom-rect.top;

			tempdc.CreateCompatibleDC(&dc);						//依附窗口DC创建兼容内存DC
			tempbitmap.CreateCompatibleBitmap(&dc, width, height);//创建兼容位图
			tempdc.SelectObject(&tempbitmap);					//将位图选择进内存DC
			tempdc.FillSolidRect(&rect, dc.GetBkColor());		//按原来背景填充客户区，不然会是黑色
			m_pDrawing->Paint(tempdc.m_hDC);
			dc.BitBlt(rect.left, rect.top, width, height, &tempdc, rect.left, rect.top, SRCCOPY);

			//////////////////////////////////////////////////////////////////////////
			BITMAP bmp;	
			tempbitmap.GetBitmap(&bmp);							//
			FILE *fp			= fopen(filename, "w+b");	
			BITMAPINFOHEADER bih= {0};								//
			bih.biBitCount		= bmp.bmBitsPixel;					
			bih.biSize			= sizeof(BITMAPINFOHEADER);			
			bih.biCompression	= BI_RGB;
			bih.biPlanes		= 1;			
			bih.biHeight		= bmp.bmHeight;						//
			bih.biWidth			= bmp.bmWidth;						//
			bih.biSizeImage		= bmp.bmWidthBytes*bmp.bmHeight;	//

			//////////////////////////////////////////////////////////////////////////
			BITMAPFILEHEADER bfh= {0};								//
			bfh.bfOffBits		= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);	//
			bfh.bfSize			= bfh.bfOffBits + bmp.bmWidthBytes*bmp.bmHeight;		//
			bfh.bfType			= (WORD)0x4d42;

			fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);			// 
			fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);			//

			byte * p = new byte[bmp.bmWidthBytes*bmp.bmHeight];	//	
			GetDIBits(tempdc.m_hDC, (HBITMAP) tempbitmap.m_hObject, 0, rect.bottom-rect.top, p,
				(LPBITMAPINFO) &bih, DIB_RGB_COLORS);		//

			fwrite(p, 1, bmp.bmWidthBytes*bmp.bmHeight, fp);		//	
			delete [] p; p=NULL;	
			fclose(fp);			
			tempdc.DeleteDC();									//删除DC		
			tempbitmap.DeleteObject();							//删除位图									//
		}
	}*/
}


void CDrawingViewStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	PrevMousePnt = point;
	m_Pan = true;
	CStatic::OnLButtonDown(nFlags, point);
}

void CDrawingViewStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_Pan = false;
	CStatic::OnLButtonUp(nFlags, point);
}

void CDrawingViewStatic::OnMButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	PrevMousePnt = point;
	m_Pan = true;
	CStatic::OnMButtonDown(nFlags, point);
}

void CDrawingViewStatic::OnMButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_Pan = false;
	CStatic::OnMButtonUp(nFlags, point);
}

void CDrawingViewStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_Pan && GetDC()->PtVisible(point))
	{
// 		if(m_pDrawing->isOpen())
// 		{
// 			double OnePixel;
// 			VIEW drwview;
// 			m_pDrawing->GetViewProperties(&drwview);
// 			OnePixel = 1/(drwview.PPU*drwview.ZoomLevel);
// 
// 			drwview.ViewLeft = drwview.ViewLeft - OnePixel*(point.x-PrevMousePnt.x);
// 			drwview.ViewBottom = drwview.ViewBottom + OnePixel*(point.y-PrevMousePnt.y);
// 			m_pDrawing->SetViewProperties(&drwview);
// 			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
// 
// 			PrevMousePnt = point;
// 		}
	}
	CStatic::OnMouseMove(nFlags, point);
}


BOOL CDrawingViewStatic::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	//return CStatic::OnEraseBkgnd(pDC);
	return true;
}


BOOL CDrawingViewStatic::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}
