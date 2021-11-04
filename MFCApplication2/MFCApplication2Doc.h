
// MFCApplication2Doc.h : interface of the CMFCApplication2Doc class
//


#pragma once

#include <vector>
#include "CGraphicObj.h"

class CMFCApplication2Doc : public CDocument
{
protected: // create from serialization only
	CMFCApplication2Doc() noexcept;
	DECLARE_DYNCREATE(CMFCApplication2Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMFCApplication2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	std::vector<CGraphicObj*> GetGraphic() { return c_graphic; };
	void AddGraphic(CGraphicObj* g) { c_graphic.push_back(g); };
	void DeleteGraphic(int index) {
		delete c_graphic[index];
		c_graphic.erase(c_graphic.begin() + index);
	};

	double zoom = 1.0;

protected:
	std::vector<CGraphicObj*> c_graphic;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
