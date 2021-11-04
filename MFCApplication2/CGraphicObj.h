#pragma once

#include <atltypes.h>
#include <afxwin.h>

// 圖形物件，儲存圖形資料，以及Draw、IsInArea、UpdateSize等method。
// 先定義，然後再implement成各種圖形物件，使用"new"語法創造子類別時可存放在此父類別(必須以指標型態存取)。
class CGraphicObj
{
public:
	byte type;				// 物件種類，1是text，2是triangle，3是ellipse
	COLORREF color;			// 物件顏色
	COLORREF bk_color;		// 背景顏色
	bool IsShowBK;			// 是否顯示背景
	CPoint TLpoint;			// 矩形左上角座標
	CPoint BRpoint;			// 矩形右下角座標
	int linetype;			// 物件邊線種類
	COLORREF line_color;	// 物件邊線顏色
	CString font;			// 字體種類
	bool bold;				// 是否粗體
	bool italic;			// 是否斜體
	bool underline;			// 是否底線
	double zoom;			// 縮放倍率
	CPoint zoom_TLpoint;	// 縮放後的TLpoint
	CPoint zoom_BRpoint;	// 縮放後的BRpoint

	virtual void Draw(CDC* pDC) {};							// 繪圖method，需override
	virtual bool IsInArea(CPoint p) { return false; };		// 判斷是否在物件內，需override
	void UpdateSize()										// 更新zoom_TLpoint和zoom_BRpoint，通常使用在Draw之前
	{
		zoom_TLpoint.x = (int)((double)TLpoint.x * zoom);
		zoom_TLpoint.y = (int)((double)TLpoint.y * zoom);
		zoom_BRpoint.x = (int)((double)BRpoint.x * zoom);
		zoom_BRpoint.y = (int)((double)BRpoint.y * zoom);
	};
};

// Implement text object
class CTextObj : public CGraphicObj
{
public:
	CTextObj(CPoint TL, CPoint BR, COLORREF c, COLORREF bk_c, bool isShow, CString s, bool b, bool i, bool u, double z)
	{
		TLpoint = TL;
		BRpoint = BR;
		color = c;
		type = 1;
		bk_color = bk_c;
		IsShowBK = isShow;
		font = s;
		bold = b;
		italic = i;
		underline = u;
		zoom = z;
	}
	void Draw(CDC* pDC)
	{	
		BRpoint.x = TLpoint.x + (BRpoint.y - TLpoint.y) * 5;	// 修改矩形寬度以符合字體大型(字體大小以高度決定)

		UpdateSize();	// 先更新zoom_TLpoint和zoom_BRpoint

		// 創造字體物件
		CFont font;
		font.CreatePointFont((zoom_BRpoint.y - zoom_TLpoint.y) * 5, this->font);

		// 取得字體訊息
		LOGFONT log;
		font.GetLogFont(&log);

		// 修改字體訊息(bold、italic、underline)
		if(bold)		log.lfWeight = FW_BOLD;
		else			log.lfWeight = FW_NORMAL;

		if(italic)		log.lfItalic = TRUE;
		else			log.lfItalic = FALSE;

		if(underline)	log.lfUnderline = TRUE;
		else			log.lfUnderline = FALSE;

		font.DeleteObject();				// 刪除物件
		font.CreateFontIndirectW(&log);		// 用字體訊息創造一個新的字體物件

		pDC->SelectObject(&font);			// 選取繪圖的工具

		// 是否繪製背景顏色
		if (IsShowBK) pDC->SetBkColor(bk_color);
		else pDC->SetBkMode(TRANSPARENT);

		pDC->SetTextColor(color);	// 設定字體顏色
		pDC->TextOutW(zoom_TLpoint.x + (zoom_BRpoint.y - zoom_TLpoint.y) / 9, zoom_TLpoint.y - (zoom_BRpoint.y - zoom_TLpoint.y) / 9, TEXT("Hello World"));	// 繪製字體
	}
	bool IsInArea(CPoint p)
	{
		return zoom_TLpoint.x + 5 < p.x&& zoom_BRpoint.x - 5 > p.x && zoom_TLpoint.y + 5 < p.y&& zoom_BRpoint.y - 5 > p.y;	// 範圍就是矩形大小(我把它縮小了一點範圍)
	}
};

// Implement triangle object
class CTriangleObj : public CGraphicObj
{
public:
	CTriangleObj(CPoint TL, CPoint BR, COLORREF c, int l, COLORREF bk_c, bool isShow, COLORREF l_c, double z)
	{
		TLpoint = TL;
		BRpoint = BR;
		color = c;
		type = 2;
		linetype = l;
		bk_color = bk_c;
		IsShowBK = isShow;
		line_color = l_c;
		zoom = z;
	}
	void Draw(CDC* pDC)
	{
		UpdateSize();	// 先更新zoom_TLpoint和zoom_BRpoint

		POINT ThreeP[] = { {(zoom_TLpoint.x + zoom_BRpoint.x) / 2, zoom_TLpoint.y}, {zoom_BRpoint.x, zoom_BRpoint.y}, {zoom_TLpoint.x, zoom_BRpoint.y} };	// 設定繪製三角形的三個點

		// 創造筆物件
		CPen pen;
		pen.CreatePen(linetype, 1, line_color);

		// 創造上色物件
		CBrush brush;
		brush.CreateSolidBrush(color);

		// 如果要背景顏色要先畫(避免將圖形擋住)
		if (IsShowBK)
		{
			CRect rect(zoom_TLpoint, zoom_BRpoint);
			CBrush brush_bk;
			brush_bk.CreateSolidBrush(bk_color);
			pDC->FillRect(&rect, &brush_bk);
		}

		// 畫邊線圖形(要比圖形更早，不然會把圖形擋住)
		pDC->SelectObject(&pen);
		pDC->Polygon(ThreeP, 3);

		// 畫實心圖形
		pDC->SelectObject(&brush);
		pDC->Polygon(ThreeP, 3);
	}
	bool IsInArea(CPoint p)
	{
		// 判斷是否在圖形內，要先創造圖形物件，再用內建method判斷
		POINT ThreeP[] = { {(zoom_TLpoint.x + zoom_BRpoint.x) / 2, zoom_TLpoint.y}, {zoom_BRpoint.x, zoom_BRpoint.y}, {zoom_TLpoint.x, zoom_BRpoint.y} };
		CRgn triangle;
		triangle.CreatePolygonRgn(ThreeP, 3, ALTERNATE);

		return triangle.PtInRegion(p);
	}
};

// Implement ellipse object
class CEllipseObj : public CGraphicObj
{
public:
	CEllipseObj(CPoint TL, CPoint BR, COLORREF c, int l, COLORREF bk_c, bool isShow, COLORREF l_c, double z)
	{
		TLpoint = TL;
		BRpoint = BR;
		color = c;
		type = 3;
		linetype = l;
		bk_color = bk_c;
		IsShowBK = isShow;
		line_color = l_c;
		zoom = z;
	}
	void Draw(CDC* pDC)	// 實踐的方式與Triangle.Draw差不多
	{
		UpdateSize();

		CRect rect;
		rect.SetRect(zoom_TLpoint, zoom_BRpoint);

		CPen pen;
		pen.CreatePen(linetype, 1, line_color);

		CBrush brush;
		brush.CreateSolidBrush(color);

		if (IsShowBK)
		{
			CBrush brush_bk;
			brush_bk.CreateSolidBrush(bk_color);
			pDC->FillRect(&rect, &brush_bk);
		}

		pDC->SelectObject(&pen);
		pDC->Ellipse(&rect);

		pDC->SelectObject(&brush);
		pDC->Ellipse(&rect);
	}
	bool IsInArea(CPoint p)
	{
		// 判斷是否在圖形內，要先創造圖形物件，再用內建method判斷
		CRect rect;
		rect.SetRect(zoom_TLpoint, zoom_BRpoint);

		CRgn circle;
		circle.CreateEllipticRgnIndirect(&rect);

		return circle.PtInRegion(p);
	}
};