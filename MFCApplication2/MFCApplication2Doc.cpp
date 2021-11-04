
// MFCApplication2Doc.cpp : implementation of the CMFCApplication2Doc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplication2Doc

IMPLEMENT_DYNCREATE(CMFCApplication2Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFCApplication2Doc, CDocument)
END_MESSAGE_MAP()


// CMFCApplication2Doc construction/destruction

CMFCApplication2Doc::CMFCApplication2Doc() noexcept
{
	// TODO: add one-time construction code here
	
}

CMFCApplication2Doc::~CMFCApplication2Doc()
{
}

BOOL CMFCApplication2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	for (int i = 0; i < c_graphic.size(); i++)
	{
		delete c_graphic[i];
	}
	c_graphic.clear();

	return TRUE;
}




// CMFCApplication2Doc serialization

void CMFCApplication2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		CFile* file = ar.GetFile();

		ar << c_graphic.size();
		for (auto g : c_graphic)
		{
			ar << g->type << g->color << g->bk_color << g->IsShowBK << g->TLpoint << g->BRpoint << g->linetype << g->line_color << g->font << g->bold << g->italic << g->underline;
		}
	}
	else
	{
		for (int i = 0; i < c_graphic.size(); i++)
		{
			delete c_graphic[i];
		}
		c_graphic.clear();

		int size;
		ar >> size;

		byte type;
		COLORREF color;
		COLORREF bk_color;
		bool IsShowBK;
		CPoint TLpoint;
		CPoint BRpoint;
		int linetype;
		COLORREF line_color;
		CString font;
		bool bold;
		bool italic;
		bool underline;

		for (int i = 0; i < size; i++)
		{
			ar >> type >> color >> bk_color >> IsShowBK >> TLpoint >> BRpoint >> linetype >> line_color >> font >> bold >> italic >> underline;
			
			switch (type)
			{
				case 1:
					AddGraphic(new CTextObj(TLpoint, BRpoint, color, bk_color, IsShowBK, font, bold, italic, underline, zoom));
					break;
				case 2:
					AddGraphic(new CTriangleObj(TLpoint, BRpoint, color, linetype, bk_color, IsShowBK, line_color, zoom));
					break;
				case 3:
					AddGraphic(new CEllipseObj(TLpoint, BRpoint, color, linetype, bk_color, IsShowBK, line_color, zoom));
					break;
				default:
					break;
			}
		}
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMFCApplication2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMFCApplication2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCApplication2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCApplication2Doc diagnostics

#ifdef _DEBUG
void CMFCApplication2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCApplication2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCApplication2Doc commands
