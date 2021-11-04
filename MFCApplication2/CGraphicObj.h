#pragma once

#include <atltypes.h>
#include <afxwin.h>

// �ϧΪ���A�x�s�ϧθ�ơA�H��Draw�BIsInArea�BUpdateSize��method�C
// ���w�q�A�M��Aimplement���U�عϧΪ���A�ϥ�"new"�y�k�гy�l���O�ɥi�s��b�������O(�����H���Ы��A�s��)�C
class CGraphicObj
{
public:
	byte type;				// ��������A1�Otext�A2�Otriangle�A3�Oellipse
	COLORREF color;			// �����C��
	COLORREF bk_color;		// �I���C��
	bool IsShowBK;			// �O�_��ܭI��
	CPoint TLpoint;			// �x�Υ��W���y��
	CPoint BRpoint;			// �x�Υk�U���y��
	int linetype;			// ������u����
	COLORREF line_color;	// ������u�C��
	CString font;			// �r�����
	bool bold;				// �O�_����
	bool italic;			// �O�_����
	bool underline;			// �O�_���u
	double zoom;			// �Y�񭿲v
	CPoint zoom_TLpoint;	// �Y��᪺TLpoint
	CPoint zoom_BRpoint;	// �Y��᪺BRpoint

	virtual void Draw(CDC* pDC) {};							// ø��method�A��override
	virtual bool IsInArea(CPoint p) { return false; };		// �P�_�O�_�b���󤺡A��override
	void UpdateSize()										// ��szoom_TLpoint�Mzoom_BRpoint�A�q�`�ϥΦbDraw���e
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
		BRpoint.x = TLpoint.x + (BRpoint.y - TLpoint.y) * 5;	// �ק�x�μe�ץH�ŦX�r��j��(�r��j�p�H���רM�w)

		UpdateSize();	// ����szoom_TLpoint�Mzoom_BRpoint

		// �гy�r�骫��
		CFont font;
		font.CreatePointFont((zoom_BRpoint.y - zoom_TLpoint.y) * 5, this->font);

		// ���o�r��T��
		LOGFONT log;
		font.GetLogFont(&log);

		// �ק�r��T��(bold�Bitalic�Bunderline)
		if(bold)		log.lfWeight = FW_BOLD;
		else			log.lfWeight = FW_NORMAL;

		if(italic)		log.lfItalic = TRUE;
		else			log.lfItalic = FALSE;

		if(underline)	log.lfUnderline = TRUE;
		else			log.lfUnderline = FALSE;

		font.DeleteObject();				// �R������
		font.CreateFontIndirectW(&log);		// �Φr��T���гy�@�ӷs���r�骫��

		pDC->SelectObject(&font);			// ���ø�Ϫ��u��

		// �O�_ø�s�I���C��
		if (IsShowBK) pDC->SetBkColor(bk_color);
		else pDC->SetBkMode(TRANSPARENT);

		pDC->SetTextColor(color);	// �]�w�r���C��
		pDC->TextOutW(zoom_TLpoint.x + (zoom_BRpoint.y - zoom_TLpoint.y) / 9, zoom_TLpoint.y - (zoom_BRpoint.y - zoom_TLpoint.y) / 9, TEXT("Hello World"));	// ø�s�r��
	}
	bool IsInArea(CPoint p)
	{
		return zoom_TLpoint.x + 5 < p.x&& zoom_BRpoint.x - 5 > p.x && zoom_TLpoint.y + 5 < p.y&& zoom_BRpoint.y - 5 > p.y;	// �d��N�O�x�Τj�p(�ڧ⥦�Y�p�F�@�I�d��)
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
		UpdateSize();	// ����szoom_TLpoint�Mzoom_BRpoint

		POINT ThreeP[] = { {(zoom_TLpoint.x + zoom_BRpoint.x) / 2, zoom_TLpoint.y}, {zoom_BRpoint.x, zoom_BRpoint.y}, {zoom_TLpoint.x, zoom_BRpoint.y} };	// �]�wø�s�T���Ϊ��T���I

		// �гy������
		CPen pen;
		pen.CreatePen(linetype, 1, line_color);

		// �гy�W�⪫��
		CBrush brush;
		brush.CreateSolidBrush(color);

		// �p�G�n�I���C��n���e(�קK�N�ϧξצ�)
		if (IsShowBK)
		{
			CRect rect(zoom_TLpoint, zoom_BRpoint);
			CBrush brush_bk;
			brush_bk.CreateSolidBrush(bk_color);
			pDC->FillRect(&rect, &brush_bk);
		}

		// �e��u�ϧ�(�n��ϧΧ󦭡A���M�|��ϧξצ�)
		pDC->SelectObject(&pen);
		pDC->Polygon(ThreeP, 3);

		// �e��߹ϧ�
		pDC->SelectObject(&brush);
		pDC->Polygon(ThreeP, 3);
	}
	bool IsInArea(CPoint p)
	{
		// �P�_�O�_�b�ϧΤ��A�n���гy�ϧΪ���A�A�Τ���method�P�_
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
	void Draw(CDC* pDC)	// ���覡�PTriangle.Draw�t���h
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
		// �P�_�O�_�b�ϧΤ��A�n���гy�ϧΪ���A�A�Τ���method�P�_
		CRect rect;
		rect.SetRect(zoom_TLpoint, zoom_BRpoint);

		CRgn circle;
		circle.CreateEllipticRgnIndirect(&rect);

		return circle.PtInRegion(p);
	}
};