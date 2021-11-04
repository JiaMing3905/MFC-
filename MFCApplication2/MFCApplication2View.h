
// MFCApplication2View.h : interface of the CMFCApplication2View class
//

#pragma once


class CMFCApplication2View : public CScrollView
{
protected: // create from serialization only
	CMFCApplication2View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication2View)

// Attributes
public:
	CMFCApplication2Doc* GetDocument() const;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMFCApplication2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGraphicText();
	afx_msg void OnPolygonEllipse();
	afx_msg void OnPolygonTriangle();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnColorBlack();
	afx_msg void OnColorRed();
	afx_msg void OnColorBlue();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnTextSolid();
	afx_msg void OnTextDash();
	afx_msg void OnTextDot();
	afx_msg void OnContextProperty();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonPan();
	afx_msg void OnButtonDelet();
	afx_msg void OnButtonWord();
	afx_msg void OnButtonTriangle();
	afx_msg void OnButtonEllipse();
	afx_msg void OnUpdateButtonSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonPan(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonDelet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonWord(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonTriangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonEllipse(CCmdUI* pCmdUI);
	afx_msg void OnButtonZoomin();
	afx_msg void OnButtonZoomout();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnInitialUpdate();
	CPoint WaitLeftMouseDown();
	void ModifyObjColor(COLORREF color);
	void SelectGraphic(int index);
	void ModifyLineType(int type);
	void GetTruePoint(CPoint* p);
	void GetLogicalPoint(CPoint* p);
	void GetPhysicalPoint(CPoint* p);

public:
	COLORREF c_color = RGB(0, 0, 0);	// force顏色的預設值
	COLORREF bk_color = RGB(0, 0, 0);	// background顏色的預設值
	bool IsShowBK = false;				// 是否顯示背景顏色
	bool IsModify = false;	// true修改顏色
	int c_index;			// 修改顏色時索引物件
	byte setCursor;			// 鼠標的樣式
	CRectTracker tracker;	// 追蹤框
	bool IsSelecting = false;	// 選取物件或功能時，要將值設為true，避免tracker的rubber band出現
	int linetype = PS_SOLID;	// 線條樣式
	COLORREF ownDrawColor = RGB(0, 0, 0);	// 屬於menu的設定，color
	int ownDrawLineType = PS_SOLID;			// 屬於menu的設定，線的格式
	COLORREF line_color = RGB(0, 0, 0);		// line color顏色的預設值
	CString font = TEXT("Comic Sans MS");	// 字體
	bool bold = false;			// 粗體字
	bool italic = false;		// 斜體字
	bool underline = false;		// 底線
	int toolBar_checkButton = ID_BUTTON_SELECT;
	int scrollWidth = 1000;
	int scrollHeight = 600;
};

#ifndef _DEBUG  // debug version in MFCApplication2View.cpp
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

