// PropertyPage_Color.cpp : implementation file
//

#include "pch.h"
#include "MFCApplication2.h"
#include "PropertyPage_Color.h"
#include "afxdialogex.h"


// PropertyPage_Color dialog

IMPLEMENT_DYNAMIC(PropertyPage_Color, CMFCPropertyPage)

PropertyPage_Color::PropertyPage_Color(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_DIALOG1)
	, m1_showBK(FALSE)
{

}

PropertyPage_Color::~PropertyPage_Color()
{
}

void PropertyPage_Color::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_M1_SHOWBK, m1_showBK);
	DDX_Control(pDX, IDC_M1_FORCECOLOR, m1_forceColor);
	DDX_Control(pDX, IDC_M1_BKCOLOR, m1_bkColor);
}


BEGIN_MESSAGE_MAP(PropertyPage_Color, CMFCPropertyPage)
END_MESSAGE_MAP()


// PropertyPage_Color message handlers
