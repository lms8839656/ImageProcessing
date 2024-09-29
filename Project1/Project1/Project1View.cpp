
// Project1View.cpp: CProject1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Project1.h"
#endif

#include <string.h>
#include "Project1Doc.h"
#include "Project1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProject1View

IMPLEMENT_DYNCREATE(CProject1View, CView)

BEGIN_MESSAGE_MAP(CProject1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CProject1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_RUN, &CProject1View::OnRun)
	ON_COMMAND(ID_PROJECT1_RUN2, &CProject1View::OnProject1Run2)
	ON_COMMAND(ID_TEST, &CProject1View::OnTest)
	ON_COMMAND(ID_RUN3, &CProject1View::OnRun3)
END_MESSAGE_MAP()

// CProject1View 생성/소멸

CProject1View::CProject1View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CProject1View::~CProject1View()
{

}

BOOL CProject1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CProject1View 그리기

void CProject1View::OnDraw(CDC* pDC)
{
	CProject1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	/*
	//속도 개선을 위해 비트맵 이용해서 비트맵에 그린 후에 화면에 전송하기
	//pDC :: 원도우 dc 포인터 (화면상의 dc ) – 화면에 보이는 도화지 그림
	CDC MemDC; //메모리 DC 생성 -> 화면에 그리기 전에 메모리 상에 도화지를 만든다.
	if (MemDC.GetSafeHdc() == NULL)//만약에 MemDC 가 없다면 생성해라~!(메모리에 도화지가 없으면 만들어라)
	{
		MemDC.CreateCompatibleDC(pDC);//화면에 pDC와 호환되는 메모리DC 생성 (화면에 맞는 메로리 도화지 생성)
	}
	CBitmap bmp;//메모리 DC를 이용해서 비트맵에 그릴 비트맵 생성
	CBitmap* pOldBmp = NULL; //백업용 비트맵 생성
	//지금 사용중인 윈도우 크기만큼(높이 * 넓이)의 비트맵 생성(횐 도화지 생성)
	Bmp.CreateCompatibleBitmap(pDC, pDoc->n_OpenImg_Height, pDoc->n_OpenImg_Width);
	pOldBmp = (CBitmap*)MemDC.SelectObject(&bmp);
	//메모리 DC에서 bmp선택 (메모리에 흰도화지 연결)(지금메모리 도화지를 크기기 1*1인 크기의 도화지 이므로 이렇게 연결을 해주어야 메모리 도화지가 화면도화지 만큼 커진다. 그리고 연결을 할경우 메모리도화지에 그린것들이 비트맵에도 그려진다.)
	for(int y = 0; y < pDoc->n_OpenImg_Height; y++)
	{
		for(int x = 0; x < pDoc->n_OpenImg_Width; x++)
		{
			MemDC.SetPixel(x, y
				RGB(*(pDoc->m_pDib->m_OpenImg),
					*(pDoc->m_pDib->m_OpenImg),
					*(pDoc->m_pDib->m_OpenImg)));//MemDC에 픽셀들에 RGB값을 찍는다.
			pDoc->m_pDib->m_OpenImg++;
			//SetPixel(memDC, x, y, RGB);
		}
	}
	//화면에 그린다: BufferADC 안에 내용들을 화면에 표현
	pDC->BitBlt(0, 0, pDoc->n_OpenImg_Height, pDoc->n_OpenImg_Width, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBmp);
	ReleaseDC(&MemDC);
	*/

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	int x, y, hy, bx, gx, rx;
	unsigned char b, g, r;
	if (pDoc->m_InImg == NULL) return;
	//for (y = pDoc->height/2; y < pDoc->height; y++) {
	//	hy = pDoc->height - 1 - y;
	//	for (x = 0; x < pDoc->width; x++) {
	//		bx = 3 * x;
	//		gx = bx + 1;
	//		rx = bx + 2;
	//		b = pDoc->m_InImg[hy][bx];
	//		g = pDoc->m_InImg[hy][gx];
	//		r = pDoc->m_InImg[hy][rx];
	//		pDC->SetPixel(x + 10, y + 10 - pDoc->height / 2, RGB(r, g, b));
	//	}
	//}
	//for (y = 0; y < pDoc->height / 2; y++) {
	//	hy = pDoc->height - 1 - y;
	//	for (x = 0; x < pDoc->width; x++) {
	//		bx = 3 * x;
	//		gx = bx + 1;
	//		rx = bx + 2;
	//		b = pDoc->m_InImg[hy][bx];
	//		g = pDoc->m_InImg[hy][gx];
	//		r = pDoc->m_InImg[hy][rx];
	//		pDC->SetPixel(x + 10, y + 10, RGB(r, g, b));
	//	}
	//}
	
	if (check == 0) {
		for (y = 0; y < pDoc->height; y++) {
			hy = pDoc->height - 1 - y;
			for (x = 0; x < pDoc->width; x++) {
				bx = 3 * x;
				gx = bx + 1;
				rx = bx + 2;
				b = pDoc->m_InImg[hy][bx];
				g = pDoc->m_InImg[hy][gx];
				r = pDoc->m_InImg[hy][rx];
				pDC->SetPixel(x, y, RGB(r, g, b));
			}
		}
		if (doline == 1) {
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));    // 빨간색 펜 생성
			CPen* oldPen = pDC->SelectObject(&pen);
			pDC->MoveTo(pDoc->center_x, pDoc->center_y);
			pDC->LineTo(pDoc->maxdis_x, pDoc->maxdis_y);
			
			CString str;
			pDC->TextOutW(800, 100, _T("중심:"));
			str.Format(_T("%d, %d"), pDoc->center_x, pDoc->center_y);
			pDC->TextOutW(800, 150, str);

			pDC->TextOutW(800, 200, _T("실행시간:"));
			str.Format(_T("%lf"), pDoc->runtime);
			pDC->TextOutW(800, 250, str);

			++doline;

		}
		check = 0;
		//if(pDoc->check_center == 1) pDC->Ellipse(pDoc->center_x, pDoc->center_x, pDoc->center_y + 1, pDoc->center_y + 1);
	}


	/*
	int x, y;
	for(y = 0;y < 40;y++){
	for(x = 0;x < 800;x++){
	if(pDoc->m_OpenImg[y][x] == 0)
	pDC->SetPixel(x, y + 50, RGB(255, 0, 0));
	else pDC->SetPixel(x, y + 50, RGB(255, 255, 255));
	}
	}
	pDC->MoveTo(0, 50);
	pDC->LineTo(800, 50);
	pDC->LineTo(800, 90);
	pDC->LineTo(0, 90);
	pDC->LineTo(0, 50);
	
	for(y = 0;y < 40;y++){
	pDC->MoveTo(800, y + 50);
	pDC->LineTo(800 + pDoc->m_histo_y[y] / 3, y + 50);
	}
	
	pDC->MoveTo(0, pDoc->sy + 50);
	pDC->LineTo(800, pDoc->sy + 50);
	pDC->MoveTo(0, pDoc->ey + 50);
	pDC->LineTo(800, pDoc->ey + 50);

	for(x = 0;x < 800;x++){
	pDC->MoveTo(x, 90);
	pDC->LineTo(x, 90 + pDoc->m_histo_x[x]);
	}

	for(y = 0;y < pDoc->letter_no;y++){
	if(pDoc->dx[y] == 0) continue;
	pDC->MoveTo(pDoc->sx[y], pDoc->sy + 50);
	pDC->LineTo(pDoc->sx[y], pDoc->ey + 50);
	pDC->MoveTo(pDoc->ex[y], pDoc->sy + 50);
	pDC->LineTo(pDoc->ex[y], pDoc->ey + 50);
	}

	for(y = 0;y < pDoc->pat_h;y++){
	for(x = 0;x < pDoc->pat_w;x++){
	if(pDoc->pattern[y][x] != 0)
	pDC->SetPixel(x + 300, y + 120 , RGB(0, 0, 0));
	else
	pDC->SetPixel(x + 300, y + 120 , RGB(0, 255, 255));
	}
	}

	int n;
	for(n = 0;n < pDoc->letter_no;n++){
	if(pDoc->same_no[n] == 0) continue;
	for(y = pDoc->sy;y <= pDoc->ey;y++){
	for(x=pDoc->sx[n];x <= pDoc->ex[n]; x++){
	if(pDoc->m_OpenImg[y][x] == 0) pDC->SetPixel(x, y + 50, RGB(0, 255, 0));
	else pDC->SetPixel(x, y + 50, RGB(0, 0, 0));
	}
	}
	}
	*/
	///////////
	//복수라인
	////
	//for(y = 0;y < pDoc->height;y++){
	//	for(x = 0;x < pDoc->width;x++){
	//		if(pDoc->m_InImg[y][x] == 0)
	//			pDC->SetPixel(x, y, RGB(255, 0, 0));
	//	}
	//}


	
	// 수평방향 라인 히스토그램 표시
	if(hflag == 1){
		pDC->MoveTo(pDoc->width, 0);
		pDC->LineTo(pDoc->width, pDoc->height);
		pDC->LineTo(0, pDoc->height);
		for (y = 0; y < pDoc->height; y++) {
			pDC->MoveTo(pDoc->width, y);
			pDC->LineTo(pDoc->width + pDoc->m_histo_y[y] / 10, y);
		}

		int m;
		for (m = 0; m < pDoc->line; m++) {
			pDC->MoveTo(0, pDoc->sy[m]);
			pDC->LineTo(pDoc->width, pDoc->sy[m]); //시작라인
			pDC->MoveTo(0, pDoc->ey[m]);
			pDC->LineTo(pDoc->width, pDoc->ey[m]); //끝라인
		}

		//세로라인
		int n;
		for (m = 0; m < pDoc->line; m++) {
			for (n = 0; n < pDoc->letter_no[m]; n++) {
				CPen pen;
				pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));    // 빨간색 펜 생성
				CPen* oldPen = pDC->SelectObject(&pen);
				if (pDoc->letter_dx[m][n] == 0) continue;
				pDC->MoveTo(pDoc->letter_sx[m][n], pDoc->sy[m]);
				pDC->LineTo(pDoc->letter_sx[m][n], pDoc->ey[m]);
				pDC->MoveTo(pDoc->letter_ex[m][n], pDoc->sy[m]);
				pDC->LineTo(pDoc->letter_ex[m][n], pDoc->ey[m]);
			}
		}

		//패턴
		for (y = 0; y < pDoc->pat_h; y++) {
			for (x = 0; x < pDoc->pat_w; x++) {
				if (pDoc->pattern[y][x] != 0) continue;
				bx = 3 * x;
				gx = bx + 1;
				rx = bx + 2;
				pDC->SetPixel(x + 300, y + pDoc->height, RGB(0, 0, 255));
			}
		}


		for (y = 0; y < pDoc->height; y++) {
			hy = pDoc->height - 1 - y;
			for (x = 0; x < pDoc->width; x++) {
				bx = 3 * x;
				gx = bx + 1;
				rx = bx + 2;
				b = pDoc->m_OrgImg[hy][bx];
				g = pDoc->m_OrgImg[hy][gx];
				r = pDoc->m_OrgImg[hy][rx];
				pDC->SetPixel(x, y, RGB(r, g, b));
			}
		}

		//매칭
		int pix[3];
		for (m = 0; m < pDoc->line; m++) {
			for (n = 0; n < pDoc->letter_no[m]; n++) {
				if (pDoc->same_no[m][n] != 0) continue;
				for (y = pDoc->sy[m]; y <= pDoc->ey[m]; y++) {
					for (x = pDoc->letter_sx[m][n]; x <= pDoc->letter_ex[m][n]; x++) {
						bx = 3 * x;
						gx = bx + 1;
						rx = bx + 2;
						pix[0] = pDoc->m_InImg[pDoc->height - 1 - y][bx];
						pix[1] = pDoc->m_InImg[pDoc->height - 1 - y][gx];
						pix[2] = pDoc->m_InImg[pDoc->height - 1 - y][rx];
						pDoc->pixsum2 = pix[0] + pix[1] + pix[2];
						pix[0] = pDoc->m_OrgImg[pDoc->height - 1 - y][bx];
						pix[1] = pDoc->m_OrgImg[pDoc->height - 1 - y][gx];
						pix[2] = pDoc->m_OrgImg[pDoc->height - 1 - y][rx];
						if (pDoc->pixsum2 < 10) pDC->SetPixel(x, y, RGB(abs(pix[2] - 255), abs(pix[1] - 255), abs(pix[0] - 255)));
						else pDC->SetPixel(x, y, RGB(abs(pix[2] - 255), abs(pix[1] - 255), abs(pix[0] - 255)));
					}
				}
			}
		}
	}

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CProject1View 인쇄


void CProject1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CProject1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CProject1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CProject1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CProject1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CProject1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CProject1View 진단

#ifdef _DEBUG
void CProject1View::AssertValid() const
{
	CView::AssertValid();
}

void CProject1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProject1Doc* CProject1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProject1Doc)));
	return (CProject1Doc*)m_pDocument;
}
#endif //_DEBUG


// CProject1View 메시지 처리기


void CProject1View::OnRun()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CProject1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->Run();
	Invalidate(FALSE);
}


void CProject1View::OnProject1Run2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CProject1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->Run2();
	hflag = 1;
	Invalidate(FALSE);
}


void CProject1View::OnTest()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CProject1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->Test();
	Invalidate(FALSE);
}


void CProject1View::OnRun3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CProject1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->Run3();
	doline = 1;
	Invalidate(FALSE);
}
