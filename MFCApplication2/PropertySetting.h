#pragma once
#include <afxpropertysheet.h>
#include "PropertyPage_Color.h"
#include "PropertyPage_Border.h"
#include "PropertyPage_Font.h"

class PropertySetting : public CMFCPropertySheet
{
public:
	PropertyPage_Color page1;
	PropertyPage_Border page2;
	PropertyPage_Font page3;
	PropertySetting();
	BOOL OnInitDialog();
};

