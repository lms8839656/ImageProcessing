
// Project1View.h: CProject1View 클래스의 인터페이스
//

#pragma once


class CProject1View : public CView
{
protected: // serialization에서만 만들어집니다.
	CProject1View() noexcept;
	DECLARE_DYNCREATE(CProject1View)

// 특성입니다.
public:
	CProject1Doc* GetDocument() const;

// 작업입니다.
public:
	int match_x;
	int match_y;
	int match_flag;
	int hflag;
	int check = 0;
	int doline = 0;
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CProject1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRun();
	afx_msg void OnProject1Run2();
	afx_msg void OnTest();
	afx_msg void OnRun3();
};

#ifndef _DEBUG  // Project1View.cpp의 디버그 버전
inline CProject1Doc* CProject1View::GetDocument() const
   { return reinterpret_cast<CProject1Doc*>(m_pDocument); }
#endif

