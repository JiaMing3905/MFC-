
// MFCApplication2View.cpp : implementation of the CMFCApplication2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "MFCApplication2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CGraphicObj.h"
#include "PropertySetting.h"

// CMFCApplication2View

IMPLEMENT_DYNCREATE(CMFCApplication2View, CScrollView)

BEGIN_MESSAGE_MAP(CMFCApplication2View, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_GRAPHIC_TEXT, &CMFCApplication2View::OnGraphicText)
	ON_COMMAND(ID_POLYGON_ELLIPSE, &CMFCApplication2View::OnPolygonEllipse)
	ON_COMMAND(ID_POLYGON_TRIANGLE, &CMFCApplication2View::OnPolygonTriangle)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_COLOR_BLACK, &CMFCApplication2View::OnColorBlack)
	ON_COMMAND(ID_COLOR_RED, &CMFCApplication2View::OnColorRed)
	ON_COMMAND(ID_COLOR_BLUE, &CMFCApplication2View::OnColorBlue)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_COMMAND(ID_TEXT_SOLID, &CMFCApplication2View::OnTextSolid)
	ON_COMMAND(ID_TEXT_DASH, &CMFCApplication2View::OnTextDash)
	ON_COMMAND(ID_TEXT_DOT, &CMFCApplication2View::OnTextDot)
	ON_COMMAND(ID_CONTEXT_PROPERTY, &CMFCApplication2View::OnContextProperty)
	ON_COMMAND(ID_BUTTON_SELECT, &CMFCApplication2View::OnButtonSelect)
	ON_COMMAND(ID_BUTTON_PAN, &CMFCApplication2View::OnButtonPan)
	ON_COMMAND(ID_BUTTON_DELET, &CMFCApplication2View::OnButtonDelet)
	ON_COMMAND(ID_BUTTON_WORD, &CMFCApplication2View::OnButtonWord)
	ON_COMMAND(ID_BUTTON_TRIANGLE, &CMFCApplication2View::OnButtonTriangle)
	ON_COMMAND(ID_BUTTON_ELLIPSE, &CMFCApplication2View::OnButtonEllipse)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SELECT, &CMFCApplication2View::OnUpdateButtonSelect)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PAN, &CMFCApplication2View::OnUpdateButtonPan)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DELET, &CMFCApplication2View::OnUpdateButtonDelet)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_WORD, &CMFCApplication2View::OnUpdateButtonWord)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TRIANGLE, &CMFCApplication2View::OnUpdateButtonTriangle)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ELLIPSE, &CMFCApplication2View::OnUpdateButtonEllipse)
	ON_COMMAND(ID_BUTTON_ZOOMIN, &CMFCApplication2View::OnButtonZoomin)
	ON_COMMAND(ID_BUTTON_ZOOMOUT, &CMFCApplication2View::OnButtonZoomout)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMFCApplication2View construction/destruction

CMFCApplication2View::CMFCApplication2View() noexcept
{
	// TODO: add construction code here

	tracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside;
	tracker.m_nHandleSize = 10;
}

CMFCApplication2View::~CMFCApplication2View()
{
}

BOOL CMFCApplication2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CMFCApplication2View drawing

void CMFCApplication2View::OnDraw(CDC* pDC)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 取得視窗的寬與高，然後乘上縮放比例，但如果低於預設值則取用預設值(避免無法覆蓋先前的繪圖)
	int nWidth = (int)((double)GetSystemMetrics(SM_CXFULLSCREEN) * pDoc->zoom);
	int nHeight = (int)((double)GetSystemMetrics(SM_CYFULLSCREEN) * pDoc->zoom);
	nWidth = nWidth > 1000 ? nWidth : 1000;
	nHeight = nHeight > 600 ? nHeight : 600;

	// 在記憶體建立顯示物件來當緩衝，然後複寫EraseBKgrd關閉刷新背景，避免閃爍
	CDC memDC;															// 顯示物件
	CBitmap memBitMap;													// 位圖物件
	memDC.CreateCompatibleDC(pDC);										// 建立與螢幕顯示相容的顯示物件
	memBitMap.CreateCompatibleBitmap(pDC, nWidth, nHeight);				// 建立與螢幕顯示相容的位圖物件
	memDC.SelectObject(&memBitMap);										// 選入點陣圖才能繪圖
	memDC.FillSolidRect(0, 0, nWidth, nHeight, RGB(255, 255, 255));		// 背景漆成白色

	// 將物件全部畫出來
	std::vector<CGraphicObj*> graphics = pDoc->GetGraphic();
	if (graphics.size() > 0)
	{
		for (auto graphic : graphics)
		{
			graphic->Draw(&memDC);
		}
	}

	tracker.Draw(&memDC);	// 把tracker矩形畫出來

	// 將Scroll View以外的區域圖成灰色
	CBrush br(GetSysColor(COLOR_GRAYTEXT));
	FillOutsideRect(&memDC, &br);

	pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);	// 將畫面複製到螢幕顯示物件上

	// 清理物件
	memBitMap.DeleteObject();
	memDC.DeleteDC();
}

// CMFCApplication2View printing

BOOL CMFCApplication2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCApplication2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCApplication2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMFCApplication2View diagnostics

#ifdef _DEBUG
void CMFCApplication2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFCApplication2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFCApplication2Doc* CMFCApplication2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication2Doc)));
	return (CMFCApplication2Doc*)m_pDocument;
}
#endif //_DEBUG


void CMFCApplication2View::OnGraphicText()
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	setCursor = 1;
	CPoint p = WaitLeftMouseDown();
	GetLogicalPoint(&p);
	GetTruePoint(&p);
	CPoint p_tl = p;
	CPoint p_br = { p.x + 100, p.y + 100 };
	pDoc->AddGraphic(new CTextObj(p_tl, p_br, c_color, bk_color, IsShowBK, font, bold, italic, underline, pDoc->zoom));
	setCursor = 0;

	Invalidate();
	UpdateWindow();
}


void CMFCApplication2View::OnPolygonEllipse()
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	setCursor = 2;
	CPoint p = WaitLeftMouseDown();
	GetLogicalPoint(&p);
	GetTruePoint(&p);
	CPoint p_tl = p;
	CPoint p_br = { p.x + 100, p.y + 100 };
	pDoc->AddGraphic(new CEllipseObj(p_tl, p_br, c_color, linetype, bk_color, IsShowBK, line_color, pDoc->zoom));
	setCursor = 0;

	Invalidate();
	UpdateWindow();
}


void CMFCApplication2View::OnPolygonTriangle()
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	setCursor = 2;
	CPoint p = WaitLeftMouseDown();
	GetLogicalPoint(&p);
	GetTruePoint(&p);
	CPoint p_tl = p;
	CPoint p_br = { p.x + 100, p.y + 100 };
	pDoc->AddGraphic(new CTriangleObj(p_tl, p_br, c_color, linetype, bk_color, IsShowBK, line_color, pDoc->zoom));
	setCursor = 0;

	Invalidate();
	UpdateWindow();
}

//滑鼠左鍵點擊事件，返回point座標。
CPoint CMFCApplication2View::WaitLeftMouseDown()
{	
	IsSelecting = true;

	CPoint p = { 0, 0 };

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0) > 0 && IsSelecting)
	{
		if (!IsSelecting) return { 0, 0 };

		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_LBUTTONDOWN)
		{
			p.x = GET_X_LPARAM(msg.lParam);
			p.y = GET_Y_LPARAM(msg.lParam);
			break;
		}
	}

	IsSelecting = false;

	return p;
}

void CMFCApplication2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint p = point;
	GetLogicalPoint(&p);

	IsModify = false;
	ownDrawColor = c_color;
	ownDrawLineType = linetype;

	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 確認是否有點到物件。
	std::vector<CGraphicObj*> graphics = pDoc->GetGraphic();
	for (int i = graphics.size() - 1; i >= 0; i--)
	{
		if (graphics[i]->IsInArea(p))
		{
			ownDrawColor = graphics[i]->color;
			ownDrawLineType = graphics[i]->linetype;

			IsModify = true;
			c_index = i;

			break;
		}
	}

	ClientToScreen(&point);		// 設定成目前滑鼠的位置

	// 載入需要的menu
	CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	CMenu *subMenu = menu.GetSubMenu(0);
	subMenu->ModifyMenuW(ID_COLOR_BLACK, MF_BYCOMMAND | MF_OWNERDRAW, ID_COLOR_BLACK);
	subMenu->ModifyMenuW(ID_COLOR_RED, MF_BYCOMMAND | MF_OWNERDRAW, ID_COLOR_RED);
	subMenu->ModifyMenuW(ID_COLOR_BLUE, MF_BYCOMMAND | MF_OWNERDRAW, ID_COLOR_BLUE);
	subMenu->ModifyMenuW(ID_TEXT_SOLID, MF_BYCOMMAND | MF_OWNERDRAW, ID_TEXT_SOLID);
	subMenu->ModifyMenuW(ID_TEXT_DASH, MF_BYCOMMAND | MF_OWNERDRAW, ID_TEXT_DASH);
	subMenu->ModifyMenuW(ID_TEXT_DOT, MF_BYCOMMAND | MF_OWNERDRAW, ID_TEXT_DOT);

	CMenu* pPopup = menu.GetSubMenu(0);		// 做成pop up的模式
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);	// 顯示menu

	CScrollView::OnRButtonDown(nFlags, point);

}

// 預設值改成黑色，或是物件
void CMFCApplication2View::OnColorBlack()
{
	if (IsModify)
	{
		ModifyObjColor(RGB(0, 0, 0));
		IsModify = false;
		return;
	}
	c_color = RGB(0, 0, 0);
}

void CMFCApplication2View::OnColorRed()
{
	if (IsModify)
	{
		ModifyObjColor(RGB(255, 0, 0));
		IsModify = false;
		return;
	}
	c_color = RGB(255, 0, 0);
}


void CMFCApplication2View::OnColorBlue()
{
	if (IsModify)
	{
		ModifyObjColor(RGB(0, 0, 255));
		IsModify = false;
		return;
	}
	c_color = RGB(0, 0, 255);
}

void CMFCApplication2View::ModifyObjColor(COLORREF color)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	std::vector<CGraphicObj*> graphics = pDoc->GetGraphic();
	graphics[c_index]->color = color;	// 改變物件顏色

	Invalidate();
	UpdateWindow();
}


// 設定鼠標樣式
void CMFCApplication2View::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint p = point;
	GetLogicalPoint(&p);

	HCURSOR hCur = LoadCursor(NULL, IDC_ARROW);	// 載入鼠標

	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 如果在物件上就設定樣式3，但是處於建立物件的狀態下不會設定
	std::vector<CGraphicObj*> graphics = pDoc->GetGraphic();
	for (int i = graphics.size() - 1; i >= 0; i--)
	{
		if (graphics[i]->IsInArea(p) && setCursor == 0)
		{
			setCursor = 3;
			break;
		}
	}

	// 1為建立text時，2是建立shape時，3是在物件上
	switch (setCursor)
	{
		case 1:
			hCur = LoadCursor(NULL, IDC_IBEAM);
			break;
		case 2:
			hCur = LoadCursor(NULL, IDC_CROSS);
			break;
		case 3:
			hCur = LoadCursor(NULL, IDC_HAND);
			setCursor = 0;
			break;
		default:
			hCur = LoadCursor(NULL, IDC_ARROW);
			break;
	}
	
	::SetCursor(hCur);	// 設定鼠標function

	CScrollView::OnMouseMove(nFlags, point);
}


void CMFCApplication2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint p = point;
	GetLogicalPoint(&p);

	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	std::vector<CGraphicObj*> graphics = pDoc->GetGraphic();

	if (toolBar_checkButton == ID_BUTTON_SELECT)
	{
		// 若是點到物件則選取物件
		bool IsEmpty = true;
		for (int i = graphics.size() - 1; i >= 0; i--)
		{
			if (graphics[i]->IsInArea(p))
			{
				tracker.m_rect.SetRect(0, 0, 0, 0);

				MSG msg;

				CPoint newP;
				CPoint oldP = point;
				GetTruePoint(&oldP);
				while (GetMessage(&msg, NULL, 0, 0) > 0)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					if (msg.message == WM_MOUSEMOVE)
					{
						newP.x = GET_X_LPARAM(msg.lParam);
						newP.y = GET_Y_LPARAM(msg.lParam);
						GetTruePoint(&newP);

						graphics[i]->TLpoint.x -= oldP.x - newP.x;
						graphics[i]->BRpoint.x -= oldP.x - newP.x;
						graphics[i]->TLpoint.y -= oldP.y - newP.y;
						graphics[i]->BRpoint.y -= oldP.y - newP.y;

						oldP = newP;
					}
					else if (msg.message == WM_LBUTTONUP)
					{
						break;
					}

					Invalidate();
					UpdateWindow();
				}

				IsEmpty = false;
				IsSelecting = true;
				SelectGraphic(i);
				IsSelecting = false;
				break;
			}
		}

		// 若是空白則用rebber band
		if (IsEmpty && !IsSelecting)
		{
			CRectTracker t;
			t.TrackRubberBand(this, point, TRUE);
			CRect rect;
			t.GetTrueRect(&rect);
			CPoint tl = rect.TopLeft();
			CPoint br = rect.BottomRight();

			GetLogicalPoint(&tl);
			GetLogicalPoint(&br);

			for (int i = graphics.size() - 1; i >= 0; i--)
			{
				CPoint topL = graphics[i]->zoom_TLpoint;
				CPoint bottomR = graphics[i]->zoom_BRpoint;

				if (tl.x < topL.x && tl.y < topL.y && br.x > bottomR.x && br.y > bottomR.y)
				{
					IsSelecting = true;
					SelectGraphic(i);
					IsSelecting = false;
					break;
				}
			}
		}
	}
	else if (toolBar_checkButton == ID_BUTTON_PAN)
	{
		MSG msg;

		CPoint newP;
		CPoint oldP = point;
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_MOUSEMOVE)
			{
				BOOL x_bar;
				BOOL y_bar;
				CheckScrollBars(x_bar, y_bar);
				if (x_bar && y_bar)
				{
					CPoint scrollP = GetScrollPosition();

					newP.x = GET_X_LPARAM(msg.lParam);
					newP.y = GET_Y_LPARAM(msg.lParam);

					scrollP.x += oldP.x - newP.x;
					scrollP.y += oldP.y - newP.y;

					ScrollToPosition(scrollP);

					oldP = newP;
				}
			}
			else if (msg.message == WM_LBUTTONUP)
			{
				break;
			}

			Invalidate();
			UpdateWindow();
		}
	}
	else if (toolBar_checkButton == ID_BUTTON_DELET)
	{
		for (int i = graphics.size() - 1; i >= 0; i--)
		{
			if (graphics[i]->IsInArea(p))
			{
				pDoc->DeleteGraphic(i);
				break;
			}
		}
	}
	else if (toolBar_checkButton == ID_BUTTON_WORD)
	{
		GetTruePoint(&p);
		CPoint p_tl = p;
		CPoint p_br = { p.x + 100, p.y + 100 };
		pDoc->AddGraphic(new CTextObj(p_tl, p_br, c_color, bk_color, IsShowBK, font, bold, italic, underline, pDoc->zoom));
	}
	else if (toolBar_checkButton == ID_BUTTON_TRIANGLE)
	{
		GetTruePoint(&p);
		CPoint p_tl = p;
		CPoint p_br = { p.x + 100, p.y + 100 };
		pDoc->AddGraphic(new CTriangleObj(p_tl, p_br, c_color, linetype, bk_color, IsShowBK, line_color, pDoc->zoom));
	}
	else if (toolBar_checkButton == ID_BUTTON_ELLIPSE)
	{
		GetTruePoint(&p);
		CPoint p_tl = p;
		CPoint p_br = { p.x + 100, p.y + 100 };
		pDoc->AddGraphic(new CEllipseObj(p_tl, p_br, c_color, linetype, bk_color, IsShowBK, line_color, pDoc->zoom));
	}

	Invalidate();
	UpdateWindow();

	CScrollView::OnLButtonDown(nFlags, point);
}

void CMFCApplication2View::SelectGraphic(int index)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CGraphicObj* selectObj = pDoc->GetGraphic()[index];	// 取得選取的物件

	CPoint tl = selectObj->zoom_TLpoint;
	CPoint br = selectObj->zoom_BRpoint;
	tracker.m_rect.SetRect(tl, br);

	Invalidate();
	UpdateWindow();

	MSG msg;

	// 訊息事件，在空白處按下左鍵跳出，可上下左右移動，delete刪除
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_LBUTTONDOWN)
		{
			// 取得當前鼠標位置
			CPoint p;
			p.x = GET_X_LPARAM(msg.lParam);
			p.y = GET_Y_LPARAM(msg.lParam);
			GetLogicalPoint(&p);

			GetPhysicalPoint(&tl);
			GetPhysicalPoint(&br);

			// Hit Test，如果按到Tracker四角會產生Rubber Band可拖動調整形狀、大小
			if (tracker.HitTest(p) >= 0)
			{
				switch (tracker.HitTest(p))
				{
					case 0:
						tracker.TrackRubberBand(this, br, TRUE);
						break;
					case 1:
					{
						tracker.TrackRubberBand(this, CPoint(tl.x, br.y), TRUE);
						break;
					}
					case 2:
						tracker.TrackRubberBand(this, tl, TRUE);
						break;
					case 3:
					{
						tracker.TrackRubberBand(this, CPoint(br.x, tl.y), TRUE);
						break;
					}
					default:
						break;
				}
				CRect rect;
				tracker.GetTrueRect(&rect);

				CPoint topleft = rect.TopLeft();
				CPoint bottomright = rect.BottomRight();
				GetLogicalPoint(&topleft);
				GetLogicalPoint(&bottomright);
				GetTruePoint(&topleft);
				GetTruePoint(&bottomright);
				selectObj->TLpoint = topleft;
				selectObj->BRpoint = bottomright;

				Invalidate();
				UpdateWindow();

				tl = selectObj->zoom_TLpoint;
				br = selectObj->zoom_BRpoint;
				tracker.m_rect.SetRect(tl, br);

				Invalidate();
				UpdateWindow();
			}
			else
			{
				// 若點擊Tracker以外的地方則會將Tracker初始化為0(就是消失)
				tracker.m_rect.SetRect(0, 0, 0, 0);
				Invalidate();
				UpdateWindow();
				break;
			}
		}
		else if (msg.message == WM_KEYDOWN)
		{
			// 上下左右移動，以及刪除
			switch (msg.wParam)
			{
				case VK_UP:
					selectObj->TLpoint.y--;
					selectObj->BRpoint.y--;
					break;
				case VK_DOWN:
					selectObj->TLpoint.y++;
					selectObj->BRpoint.y++;
					break;
				case VK_LEFT:
					selectObj->TLpoint.x--;
					selectObj->BRpoint.x--;
					break;
				case VK_RIGHT:
					selectObj->TLpoint.x++;
					selectObj->BRpoint.x++;
					break;
				case VK_DELETE:
					pDoc->DeleteGraphic(index);	// 刪除物件
					break;
				default:
					break;
			}

			// 必須更新tracker框
			selectObj->UpdateSize();
			tl = selectObj->zoom_TLpoint;
			br = selectObj->zoom_BRpoint;
			tracker.m_rect.SetRect(tl, br);

			Invalidate();
			UpdateWindow();

			// 如果delete則必須跳出迴圈，結束事件
			if (msg.wParam == VK_DELETE)
			{
				break;
			}
		}
	}
}

// own draw必須複寫
void CMFCApplication2View::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemWidth = 30;
	lpMeasureItemStruct->itemHeight = ::GetSystemMetrics(SM_CYMENU);

	CScrollView::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

// own draw必須複寫，並且實作介面(自己畫)
void CMFCApplication2View::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	// 若是被選擇會改變背景顏色
	if ((lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		pDC->SetBkColor(GetSysColor(COLOR_MENUHILIGHT));
	}
	else
	{
		pDC->SetBkColor(GetSysColor(COLOR_MENU));
	}

	if (lpDrawItemStruct->itemID >= ID_COLOR_BLACK && lpDrawItemStruct->itemID <= ID_COLOR_BLUE)
	{
		CBrush brush;
		switch (lpDrawItemStruct->itemID)
		{
			case ID_COLOR_BLACK:
				brush.CreateSolidBrush(RGB(0, 0, 0));
				if (ownDrawColor == RGB(0, 0, 0))
				{
					pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("✔　　　　　　　"));
				}
				else
				{
					pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("　　　　　　　　"));
				}
				break;
			case ID_COLOR_RED:
				brush.CreateSolidBrush(RGB(255, 0, 0));
				if (ownDrawColor == RGB(255, 0, 0))
				{
					pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("✔　　　　　　　"));
				}
				else
				{
					pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("　　　　　　　　"));
				}
				break;
			case ID_COLOR_BLUE:
				brush.CreateSolidBrush(RGB(0, 0, 255));
				if (ownDrawColor == RGB(0, 0, 255))
				{
					pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("✔　　　　　　　"));
				}
				else
				{
					pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("　　　　　　　　"));
				}
				break;
			default:
				break;
		}
		pDC->SelectObject(&brush);

		pDC->Rectangle(lpDrawItemStruct->rcItem.left + 25, lpDrawItemStruct->rcItem.top + 5, lpDrawItemStruct->rcItem.right - 20, lpDrawItemStruct->rcItem.bottom - 5);
	}
	else if(lpDrawItemStruct->itemID >= ID_TEXT_SOLID && lpDrawItemStruct->itemID <= ID_TEXT_DOT)
	{
		CPen pen;
		switch (lpDrawItemStruct->itemID)
		{
		case ID_TEXT_SOLID:
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			if (ownDrawLineType == PS_SOLID)
			{
				pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("✔　　　　　　　"));
			}
			else
			{
				pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("　　　　　　　　"));
			}
			break;
		case ID_TEXT_DASH:
			pen.CreatePen(PS_DASH, 1, RGB(1, 0, 0));
			if (ownDrawLineType == PS_DASH)
			{
				pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("✔　　　　　　　"));
			}
			else
			{
				pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("　　　　　　　　"));
			}
			break;
		case ID_TEXT_DOT:
			pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
			if (ownDrawLineType == PS_DOT)
			{
				pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("✔　　　　　　　"));
			}
			else
			{
				pDC->TextOutW(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, TEXT("　　　　　　　　"));
			}
			break;
		default:
			break;
		}

		pDC->SelectObject(&pen);
		pDC->MoveTo({ lpDrawItemStruct->rcItem.left + 25, (lpDrawItemStruct->rcItem.top + lpDrawItemStruct->rcItem.bottom) / 2 });
		pDC->LineTo({ lpDrawItemStruct->rcItem.right - 20, (lpDrawItemStruct->rcItem.top + lpDrawItemStruct->rcItem.bottom) / 2 });
	}

	CScrollView::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CMFCApplication2View::OnTextSolid()
{
	if (IsModify)
	{
		ModifyLineType(PS_SOLID);
		IsModify = false;
		return;
	}
	linetype = PS_SOLID;
}


void CMFCApplication2View::OnTextDash()
{
	if (IsModify)
	{
		ModifyLineType(PS_DASH);
		IsModify = false;
		return;
	}
	linetype = PS_DASH;
}


void CMFCApplication2View::OnTextDot()
{
	if (IsModify)
	{
		ModifyLineType(PS_DOT);
		IsModify = false;
		return;
	}
	linetype = PS_DOT;
}

void CMFCApplication2View::ModifyLineType(int type)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	std::vector<CGraphicObj*> graphics = pDoc->GetGraphic();
	graphics[c_index]->linetype = type;		// 改變line type

	Invalidate();
	UpdateWindow();
}

void CMFCApplication2View::OnContextProperty()
{
	PropertySetting sheet;

	if (IsModify)
	{
		CMFCApplication2Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		std::vector<CGraphicObj*> graphics = pDoc->GetGraphic();

		sheet.page1.m1_forceColor.SetColor(graphics[c_index]->color);
		sheet.page1.m1_bkColor.SetColor(graphics[c_index]->bk_color);
		sheet.page1.m1_showBK = graphics[c_index]->IsShowBK;

		sheet.page2.linetype = graphics[c_index]->linetype;
		sheet.page2.m2_lineColor.SetColor(graphics[c_index]->line_color);

		sheet.page3.font = graphics[c_index]->font;
		sheet.page3.m3_bold = graphics[c_index]->bold;
		sheet.page3.m3_italic = graphics[c_index]->italic;
		sheet.page3.m3_underLine = graphics[c_index]->underline;

		if (sheet.DoModal() == IDOK)
		{
			graphics[c_index]->color = sheet.page1.m1_forceColor.GetColor();
			graphics[c_index]->bk_color = sheet.page1.m1_bkColor.GetColor();
			graphics[c_index]->IsShowBK = sheet.page1.m1_showBK;

			graphics[c_index]->linetype = sheet.page2.linetype;
			graphics[c_index]->line_color = sheet.page2.m2_lineColor.GetColor();

			graphics[c_index]->font = sheet.page3.font;
			graphics[c_index]->bold = sheet.page3.m3_bold;
			graphics[c_index]->italic = sheet.page3.m3_italic;
			graphics[c_index]->underline = sheet.page3.m3_underLine;
		}
	}
	else
	{
		sheet.page1.m1_forceColor.SetColor(c_color);
		sheet.page1.m1_bkColor.SetColor(bk_color);
		sheet.page1.m1_showBK = IsShowBK;

		sheet.page2.linetype = linetype;
		sheet.page2.m2_lineColor.SetColor(line_color);

		sheet.page3.font = font;
		sheet.page3.m3_bold = bold;
		sheet.page3.m3_italic = italic;
		sheet.page3.m3_underLine = underline;

		if (sheet.DoModal() == IDOK)
		{
			c_color = sheet.page1.m1_forceColor.GetColor();
			bk_color = sheet.page1.m1_bkColor.GetColor();
			IsShowBK = sheet.page1.m1_showBK;

			linetype = sheet.page2.linetype;
			line_color = sheet.page2.m2_lineColor.GetColor();

			font = sheet.page3.font;
			bold = sheet.page3.m3_bold;
			italic = sheet.page3.m3_italic;
			underline = sheet.page3.m3_underLine;
		}
	}

	Invalidate();
	UpdateWindow();
}


void CMFCApplication2View::OnButtonSelect()
{
	toolBar_checkButton = ID_BUTTON_SELECT;
	setCursor = 0;
}


void CMFCApplication2View::OnButtonPan()
{
	toolBar_checkButton = ID_BUTTON_PAN;
	setCursor = 0;
}


void CMFCApplication2View::OnButtonDelet()
{
	toolBar_checkButton = ID_BUTTON_DELET;
	setCursor = 0;
}


void CMFCApplication2View::OnButtonWord()
{
	toolBar_checkButton = ID_BUTTON_WORD;
	setCursor = 1;
}


void CMFCApplication2View::OnButtonTriangle()
{
	toolBar_checkButton = ID_BUTTON_TRIANGLE;
	setCursor = 2;
}


void CMFCApplication2View::OnButtonEllipse()
{
	toolBar_checkButton = ID_BUTTON_ELLIPSE;
	setCursor = 2;
}


void CMFCApplication2View::OnUpdateButtonSelect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(toolBar_checkButton != ID_BUTTON_SELECT);
}


void CMFCApplication2View::OnUpdateButtonPan(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(toolBar_checkButton != ID_BUTTON_PAN);
}


void CMFCApplication2View::OnUpdateButtonDelet(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(toolBar_checkButton != ID_BUTTON_DELET);
}


void CMFCApplication2View::OnUpdateButtonWord(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(toolBar_checkButton != ID_BUTTON_WORD);
}


void CMFCApplication2View::OnUpdateButtonTriangle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(toolBar_checkButton != ID_BUTTON_TRIANGLE);
}


void CMFCApplication2View::OnUpdateButtonEllipse(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(toolBar_checkButton != ID_BUTTON_ELLIPSE);
}

void CMFCApplication2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CRect rect;
	SetScrollSizes(MM_TEXT, CSize(scrollWidth, scrollHeight));
}



void CMFCApplication2View::OnButtonZoomin()
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->zoom *= 2.0;
	scrollWidth *= 2;
	scrollHeight *= 2;
	SetScrollSizes(MM_TEXT, CSize(scrollWidth, scrollHeight));

	std::vector<CGraphicObj*> graphics = pDoc->GetGraphic();
	for (int i = 0; i < graphics.size(); i++)
	{
		graphics[i]->zoom = pDoc->zoom;
	}

	Invalidate();
	UpdateWindow();
}


void CMFCApplication2View::OnButtonZoomout()
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->zoom /= 2.0;
	scrollWidth /= 2;
	scrollHeight /= 2;
	SetScrollSizes(MM_TEXT, CSize(scrollWidth, scrollHeight));

	std::vector<CGraphicObj*> graphics = pDoc->GetGraphic();
	for (int i = 0; i < graphics.size(); i++)
	{
		graphics[i]->zoom = pDoc->zoom;
	}

	Invalidate();
	UpdateWindow();
}

void CMFCApplication2View::GetTruePoint(CPoint* p)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	p->x = (int)((double)p->x / pDoc->zoom);
	p->y = (int)((double)p->y / pDoc->zoom);
}

void CMFCApplication2View::GetLogicalPoint(CPoint* p)
{
	CPoint origin = GetScrollPosition();

	p->x += origin.x;
	p->y += origin.y;
}

void CMFCApplication2View::GetPhysicalPoint(CPoint* p)
{
	CPoint origin = GetScrollPosition();

	p->x -= origin.x;
	p->y -= origin.y;
}

// override，直接返回true，關閉每次繪圖都要刷新背景，才能解決閃爍問題
BOOL CMFCApplication2View::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC);
	return TRUE;
}
