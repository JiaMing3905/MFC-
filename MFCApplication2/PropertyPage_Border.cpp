// PropertyPage_Border.cpp : implementation file
//

#include "pch.h"
#include "MFCApplication2.h"
#include "PropertyPage_Border.h"
#include "afxdialogex.h"


// PropertyPage_Border dialog

IMPLEMENT_DYNAMIC(PropertyPage_Border, CMFCPropertyPage)

PropertyPage_Border::PropertyPage_Border(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_DIALOG2)
{
}

PropertyPage_Border::~PropertyPage_Border()
{
}

void PropertyPage_Border::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m2_lineType);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m2_lineColor);
}


BEGIN_MESSAGE_MAP(PropertyPage_Border, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &PropertyPage_Border::OnSelchangeCombo1)
END_MESSAGE_MAP()


// PropertyPage_Border message handlers


void PropertyPage_Border::OnSelchangeCombo1()
{
	int index = m2_lineType.GetCurSel();
	if (index != LB_ERR)
	{
		switch (index)
		{
		case 0:
			linetype = PS_SOLID;
			break;
		case 1:
			linetype = PS_DASH;
			break;
		case 2:
			linetype = PS_DOT;
			break;
		default:
			break;
		}
	}
}
