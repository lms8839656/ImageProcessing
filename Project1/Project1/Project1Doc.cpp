
// Project1Doc.cpp: CProject1Doc 클래스의 구현
//


#include "opencv2/opencv.hpp"
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Project1.h"
#endif

#include "Project1Doc.h"

#include <time.h>
#include <propkey.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;



// CProject1Doc

IMPLEMENT_DYNCREATE(CProject1Doc, CDocument)

BEGIN_MESSAGE_MAP(CProject1Doc, CDocument)
END_MESSAGE_MAP()


// CProject1Doc 생성/소멸

CProject1Doc::CProject1Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_InImg = NULL;
	m_TmpImg = NULL;
	m_OrgImg = NULL;
}

CProject1Doc::~CProject1Doc()
{
	if (m_InImg) {
		delete[] m_InImg[0];
		delete[] m_InImg;

	}
	if (m_TmpImg) {
		delete[] m_TmpImg[0];
		delete[] m_TmpImg;
	}
	if (m_OrgImg) {
		delete[] m_OrgImg[0];
		delete[] m_OrgImg;
	}
}

BOOL CProject1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CProject1Doc serialization

void CProject1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CProject1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CProject1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CProject1Doc::SetSearchContent(const CString& value)
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

// CProject1Doc 진단

#ifdef _DEBUG
void CProject1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProject1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CProject1Doc 명령


BOOL CProject1Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CFile file;
	file.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);
	file.Read(&dibHf, sizeof(BITMAPFILEHEADER)); // 파일 헤드를 읽음
	if (dibHf.bfType != 0x4D42) {//이 파일이 BMP파일인지 검사. 0x4d42==‘MB'
		AfxMessageBox(_T("Not BMP file!!"));
		//return FALSE;
	}
	file.Read(&dibHi, sizeof(BITMAPINFOHEADER)); // Header 읽기
	if (dibHi.biBitCount != 24) {
		AfxMessageBox(_T("True Color Image Possible!!"));
		//return FALSE;
	}
	int width_4;
	height = dibHi.biHeight;
	width = dibHi.biWidth;
	width_4 = ((dibHi.biBitCount * width) / 8 + 3) & ~3;
	// 메모리 할당
	int ImgSize;
	ImgSize = file.GetLength() - sizeof(BITMAPFILEHEADER)
		- sizeof(BITMAPINFOHEADER);
	m_InImg = new unsigned char* [height];
	m_InImg[0] = new unsigned char[height * width_4];
	for (int i = 1; i < height; i++) m_InImg[i] = m_InImg[i - 1] + width_4;
	file.Read(m_InImg[0], ImgSize);
	file.Close();
	return TRUE;
}


void CProject1Doc::Run()
{
	// TODO: 여기에 구현 코드 추가.
	//변수선언
	int x, y, hy, bx, gx, rx, i, pixnum;
	unsigned char b, g, r;

	int aheight = height / 64;
	int awidth = width / 64;
	pixnum = aheight * awidth;

	int px, py;
	int size = 64;

	//구조체 2차원 배열 동적 할당
	arr = new MY * [aheight];
	for (i = 0; i < aheight; i++) {
		arr[i] = new MY[awidth];
	}

	//구조체 멤버 배열 동적할당
	for (y = 0; y < aheight; y++) {
		for (x = 0; x < awidth; x++) {
			//row, y
			arr[y][x].pixel = new unsigned char* [size];
			for (i = 0; i < size; i++) {
				//column, x
				arr[y][x].pixel[i] = new unsigned char [size*3];
			}
		}
	}

	//구조체 arr에 입력 이미지 입력
	for (i = 0; i < pixnum; i++) {
		for (y = 0; y < 64; y++) {
			hy = ((i / awidth + 1) * 64 - 1 - y);
			for (x = 0; x < 64; x++) {
				bx = 3 * x + (i % awidth) * 64 * 3;
				gx = bx + 1;
				rx = bx + 2;
				arr[aheight - 1 - (i / awidth) % aheight][(i % awidth)].pixel[y][3*x] = m_InImg[hy][bx];
				arr[aheight - 1 - (i / awidth) % aheight][(i % awidth)].pixel[y][3*x + 1] = m_InImg[hy][gx];
				arr[aheight - 1 - (i / awidth) % aheight][(i % awidth)].pixel[y][3*x + 2] = m_InImg[hy][rx];
			}
		}
	}

	///////////정렬//////////
	int sum = 0, sumb = 0, sumr = 0, sumg = 0;

	//최소값을 통한 정렬을 위한 구조체 선언
	typedef struct _checksum {
		int min;
		int x;
		int y;
	} CHECKSUM;

	CHECKSUM minsum{255*3*64, 0, 0 }; //초기값 bmp이미지에서 가능한 최대값입력

	//정방향~절반까지
	for (py = 1; py < (aheight-1) / 2 + 1; py++) {
		for (px = 1; px < awidth - 1; px++) {
			for (i = 0; i < pixnum; i++) {
				sum = 0;
				sumb = 0;
				sumg = 0;
				sumr = 0;
				if (0 < X && X < (awidth - 1) && 0 < Y && Y < (aheight - 1) && py <= Y) {
					for (y = 0; y < 64; y++) {
						sumb = abs(arr[py][px - 1].pixel[y][3 * 63] - arr[Y][X].pixel[y][0]) + sumb;
						sumg = abs(arr[py][px - 1].pixel[y][3 * 63 + 1] - arr[Y][X].pixel[y][0 + 1]) + sumg;
						sumr = abs(arr[py][px - 1].pixel[y][3 * 63 + 2] - arr[Y][X].pixel[y][0 + 2]) + sumr;
					}
					for (x = 0; x < 64; x++) {
						sumb = abs(arr[py - 1][px].pixel[63][3 * x] - arr[Y][X].pixel[0][3 * x]) + sumb;
						sumg = abs(arr[py - 1][px].pixel[63][3 * x + 1] - arr[Y][X].pixel[0][3 * x + 1]) + sumg;
						sumr = abs(arr[py - 1][px].pixel[63][3 * x + 2] - arr[Y][X].pixel[0][3 * x + 2]) + sumr;
					}
					sum = sumb + sumg + sumr;
					if (sum < minsum.min) {
						minsum.min = sum;
						minsum.x = X;
						minsum.y = Y;
					}
				}
			}
			Swap(&arr[py][px], &arr[minsum.y][minsum.x]);
			minsum.min = 255 * 3 * 64;
			minsum.x = 0;
			minsum.y = 0;
		}
	}

	//역순~절반까지
	for (py = aheight - 2; py > (aheight) / 2 - 1; py--) {
		for (px = awidth - 2; px > 0; px--) {
			for (i = pixnum - 1; i >= 0; i--) {
				sum = 0;
				sumb = 0;
				sumg = 0;
				sumr = 0;
				if (0 < X && X < (awidth - 1) && 0 < Y && Y < (aheight - 1) && py >= Y) {
					for (y = 0; y < 64; y++) {
						sumb = abs(arr[py][px + 1].pixel[y][0] - arr[Y][X].pixel[y][3 * 63]) + sumb;
						sumg = abs(arr[py][px + 1].pixel[y][0 + 1] - arr[Y][X].pixel[y][3 * 63 + 1]) + sumg;
						sumr = abs(arr[py][px + 1].pixel[y][0 + 2] - arr[Y][X].pixel[y][3 * 63 + 2]) + sumr;
					}
					for (x = 0; x < 64; x++) {
						sumb = abs(arr[py + 1][px].pixel[0][3 * x] - arr[Y][X].pixel[63][3 * x]) + sumb;
						sumg = abs(arr[py + 1][px].pixel[0][3 * x + 1] - arr[Y][X].pixel[63][3 * x + 1]) + sumg;
						sumr = abs(arr[py + 1][px].pixel[0][3 * x + 2] - arr[Y][X].pixel[63][3 * x + 2]) + sumr;
					}
					sum = sumb + sumg + sumr;
					if (sum < minsum.min) {
						minsum.min = sum;
						minsum.x = X;
						minsum.y = Y;
					}
				}
			}
			Swap(&arr[py][px], &arr[minsum.y][minsum.x]);
			minsum.min = 255 * 3 * 64;
			minsum.x = 0;
			minsum.y = 0;
		}
	}

	//상하좌우 최소값을 통해 전체 검산
	for (py = 1; py < (aheight - 1); py++) {
		for (px = 1; px < awidth - 1; px++) {
			for (i = 0; i < pixnum; i++) {
				sum = 0;
				sumb = 0;
				sumg = 0;
				sumr = 0;
				if (0 < X && X < (awidth - 1) && 0 < Y && Y < (aheight - 1) && py <= Y) {
					for (y = 0; y < 64; y++) {
						sumb = abs(arr[py][px - 1].pixel[y][3 * 63] - arr[Y][X].pixel[y][0]) + sumb;
						sumg = abs(arr[py][px - 1].pixel[y][3 * 63 + 1] - arr[Y][X].pixel[y][0 + 1]) + sumg;
						sumr = abs(arr[py][px - 1].pixel[y][3 * 63 + 2] - arr[Y][X].pixel[y][0 + 2]) + sumr;
					}
					for (x = 0; x < 64; x++) {
						sumb = abs(arr[py - 1][px].pixel[63][3 * x] - arr[Y][X].pixel[0][3 * x]) + sumb;
						sumg = abs(arr[py - 1][px].pixel[63][3 * x + 1] - arr[Y][X].pixel[0][3 * x + 1]) + sumg;
						sumr = abs(arr[py - 1][px].pixel[63][3 * x + 2] - arr[Y][X].pixel[0][3 * x + 2]) + sumr;
					}
					for (y = 0; y < 64; y++) {
						sumb = abs(arr[py][px + 1].pixel[y][0] - arr[Y][X].pixel[y][3 * 63]) + sumb;
						sumg = abs(arr[py][px + 1].pixel[y][0 + 1] - arr[Y][X].pixel[y][3 * 63 + 1]) + sumg;
						sumr = abs(arr[py][px + 1].pixel[y][0 + 2] - arr[Y][X].pixel[y][3 * 63 + 2]) + sumr;
					}
					for (x = 0; x < 64; x++) {
						sumb = abs(arr[py + 1][px].pixel[0][3 * x] - arr[Y][X].pixel[63][3 * x]) + sumb;
						sumg = abs(arr[py + 1][px].pixel[0][3 * x + 1] - arr[Y][X].pixel[63][3 * x + 1]) + sumg;
						sumr = abs(arr[py + 1][px].pixel[0][3 * x + 2] - arr[Y][X].pixel[63][3 * x + 2]) + sumr;
					}
					sum = sumb + sumg + sumr;
					if (sum < minsum.min) {
						minsum.min = sum;
						minsum.x = X;
						minsum.y = Y;
					}
				}
			}
			Swap(&arr[py][px], &arr[minsum.y][minsum.x]);
			minsum.min = 255 * 3 * 64;
			minsum.x = 0;
			minsum.y = 0;
		}
	}

	//완성된 배열을 m_InImg에 대입
	for (i = 0; i < pixnum; i++) {
		for (y = 0; y < 64; y++) {
			hy = ((i / awidth + 1) * 64 - 1 - y);
			for (x = 0; x < 64; x++) {
				bx = 3 * x + (i % awidth) * 64 * 3;
				gx = bx + 1;
				rx = bx + 2;
				m_InImg[hy][bx] = arr[aheight - 1 - (i / awidth) % aheight][(i % awidth)].pixel[y][3 * x];
				m_InImg[hy][gx] = arr[aheight - 1 - (i / awidth) % aheight][(i % awidth)].pixel[y][3 * x + 1];
				m_InImg[hy][rx] = arr[aheight - 1 - (i / awidth) % aheight][(i % awidth)].pixel[y][3 * x + 2];
			}
		}
	}

	/*
//오른쪽 측면기준
	//가로줄 세로줄 64개단위 체크용
	//for (i = 0; i < pixnum; i++) {
	//	for (y = 0; y < 64; y++) {
	//		hy = ((i / 13 + 1) * 64 - 1 - y);
	//		for (x = 0; x < 64; x++) {
	//			bx = 3 * x + (i % 13) * 64 * 3;
	//			gx = bx + 1;
	//			rx = bx + 2;
	//
	//			//가로줄이 홀수개 짝수개에 따라 달라짐
	//			if ((i / 13) % 2 == 0) {
	//				m_InImg[hy][bx] = (i % 2) ? 0 : 255;
	//				m_InImg[hy][gx] = (i % 2) ? 0 : 255;
	//				m_InImg[hy][rx] = (i % 2) ? 0 : 255;
	//			}
	//			else if((i / 13) % 2 == 1){
	//				m_InImg[hy][bx] = (i % 2) ? 0 : 255;
	//				m_InImg[hy][gx] = (i % 2) ? 0 : 255;
	//				m_InImg[hy][rx] = (i % 2) ? 0 : 255;
	//			}
	//		}
	//	}
	//}

for (py = 0; py < 9; py++) {
	for (px = 0; px < 12; px++) {
		for (i = 0; i < pixnum; i++) {
			sum = 0;
			sumb = 0;
			sumg = 0;
			sumr = 0;
			tsum = 0;
			tsumb = 0;
			tsumg = 0;
			tsumr = 0;
			if (0 < X && X < 12 && 0 < Y && Y < 9) {
				for (y = 0; y < 64; y++) {
					sumb = abs(arr[py][px].pixel[y][3 * 63] - arr[Y][X].pixel[y][0]) + sumb;
					sumg = abs(arr[py][px].pixel[y][3 * 63 + 1] - arr[Y][X].pixel[y][0 + 1]) + sumg;
					sumr = abs(arr[py][px].pixel[y][3 * 63 + 2] - arr[Y][X].pixel[y][0 + 2]) + sumr;
					tsumb = abs(arr[py][px].pixel[y][3 * 63] - arr[py][px].pixel[y][3 * 62]) + tsumb;
					tsumg = abs(arr[py][px].pixel[y][3 * 63 + 1] - arr[py][px].pixel[y][3 * 62 + 1]) + tsumg;
					tsumr = abs(arr[py][px].pixel[y][3 * 63 + 2] - arr[py][px].pixel[y][3 * 62 + 2]) + tsumr;
				}
				sum = sumb + sumg + sumr;
				tsum = tsumb + tsumg + tsumr;
				if (sum - tsum < minsum.min) {
					minsum.min = abs(sum - tsum);
					minsum.x = X;
					minsum.y = Y;
				}
			}
		}
		Swap(&arr[py][px + 1], &arr[minsum.y][minsum.x]);
		minsum.min = 255 * 3 * 64;
		minsum.x = 0;
		minsum.y = 0;
	}
}


//아래면 기준
for (px = 0; px < 12; px++) {
	for (py = 0; py < 9; py++) {
		for (i = 0; i < pixnum; i++) {
			sum = 0;
			sumb = 0;
			sumg = 0;
			sumr = 0;
			tsum = 0;
			tsumb = 0;
			tsumg = 0;
			tsumr = 0;
			if (0 < X && X < 12 && 0 < Y && Y < 9) {
				for (x = 0; x < 64; x++) {
					sumb = abs(arr[py][px].pixel[63][3 * x] - arr[Y][X].pixel[0][3 * x]) + sumb;
					sumg = abs(arr[py][px].pixel[63][3 * x + 1] - arr[Y][X].pixel[0][3 * x + 1]) + sumg;
					sumr = abs(arr[py][px].pixel[63][3 * x + 2] - arr[Y][X].pixel[0][3 * x + 2]) + sumr;
					tsumb = abs(arr[py][px].pixel[63][3 * x] - arr[py][px].pixel[62][3 * x]) + tsumb;
					tsumg = abs(arr[py][px].pixel[63][3 * x + 1] - arr[py][px].pixel[62][3 * x + 1]) + tsumg;
					tsumr = abs(arr[py][px].pixel[63][3 * x + 2] - arr[py][px].pixel[62][3 * x + 2]) + tsumr;
				}
				sum = sumb + sumg + sumr;
				tsum = tsumb + tsumg + tsumr;
				if (sum - tsum < minsum.min) {
					minsum.min = abs(sum - tsum);
					minsum.x = X;
					minsum.y = Y;
				}
			}
		}
		Swap(&arr[py + 1][px], &arr[minsum.y][minsum.x]);
		minsum.min = 255 * 3 * 64;
		minsum.x = 0;
		minsum.y = 0;
	}
}



//	for (py = 8; py > 0; py--) {
//	for (px = 11; px > 0; px--) {
//		for (i = 0; i < pixnum; i++) {
//			sum = 0;
//			sumb = 0;
//			sumg = 0;
//			sumr = 0;
//			tsum = 0;
//			tsumb = 0;
//			tsumg = 0;
//			tsumr = 0;
//			//
//			if (0 < X && X < 12 && 0 < Y && Y < 9 && py > Y && px > X) {
//				for (y = 0; y < 64; y++) {
//					sumb = abs(arr[py][px + 1].pixel[y][0] - arr[Y][X].pixel[y][3 * 63]) + sumb;
//					sumg = abs(arr[py][px + 1].pixel[y][0 + 1] - arr[Y][X].pixel[y][3 * 63 + 1]) + sumg;
//					sumr = abs(arr[py][px + 1].pixel[y][0 + 2] - arr[Y][X].pixel[y][3 * 63 + 2]) + sumr;
//				}
//				for (x = 0; x < 64; x++) {
//					sumb = abs(arr[py + 1][px].pixel[0][3 * x] - arr[Y][X].pixel[63][3 * x]) + sumb;
//					sumg = abs(arr[py + 1][px].pixel[0][3 * x + 1] - arr[Y][X].pixel[63][3 * x + 1]) + sumg;
//					sumr = abs(arr[py + 1][px].pixel[0][3 * x + 2] - arr[Y][X].pixel[63][3 * x + 2]) + sumr;
//				}
//				sum = sumb + sumg + sumr;
//				if (sum < minsum.min) {
//					minsum.min = sum;
//					minsum.x = X;
//					minsum.y = Y;
//				}
//			}
//		}
//		Swap(&arr[py][px], &arr[minsum.y][minsum.x]);
//		minsum.min = 255 * 3 * 64;
//		minsum.x = 0;
//		minsum.y = 0;
//	}
//}


	//////////////////////////////
	//int k = 10*13;
	//while(k){
	//for (py = 1; py < 9; py++) {
	//	for (px = 1; px < 12; px++) {
	//		for (i = 0; i < pixnum; i++) {
	//			sum = 0;
	//			sumb = 0;
	//			sumg = 0;
	//			sumr = 0;
	//			tsum = 0;
	//			tsumb = 0;
	//			tsumg = 0;
	//			tsumr = 0;
	//			//
	//			if (0 < X && X < 12 && 0 < Y && Y < 9 && (X * Y < i)) {
	//				for (y = 0; y < 64; y++) {
	//					sumb = abs(arr[py][px - 1].pixel[y][3 * 63] - arr[Y][X].pixel[y][0]) + sumb;
	//					sumg = abs(arr[py][px - 1].pixel[y][3 * 63 + 1] - arr[Y][X].pixel[y][0 + 1]) + sumg;
	//					sumr = abs(arr[py][px - 1].pixel[y][3 * 63 + 2] - arr[Y][X].pixel[y][0 + 2]) + sumr;
	//				}
	//				for (x = 0; x < 64; x++) {
	//					sumb = abs(arr[py - 1][px].pixel[63][3 * x] - arr[Y][X].pixel[0][3 * x]) + sumb;
	//					sumg = abs(arr[py - 1][px].pixel[63][3 * x + 1] - arr[Y][X].pixel[0][3 * x + 1]) + sumg;
	//					sumr = abs(arr[py - 1][px].pixel[63][3 * x + 2] - arr[Y][X].pixel[0][3 * x + 2]) + sumr;
	//				}
	//				sum = sumb + sumg + sumr;
	//				if (sum < minsum.min) {
	//					minsum.min = sum;
	//					minsum.x = X;
	//					minsum.y = Y;
	//				}
	//			}
	//		}
	//		Swap(&arr[py][px], &arr[minsum.y][minsum.x]);
	//		minsum.min = 255 * 3 * 64;
	//		minsum.x = 0;
	//		minsum.y = 0;
	//	}
	//}
	//k = k - 1;
	//}

	//for (py = 8; py > 0; py--) {
	//	for (px = 11; px > 0; px--) {
	//		for (i = 0; i < pixnum; i++) {
	//			sum = 0;
	//			sumb = 0;
	//			sumg = 0;
	//			sumr = 0;
	//			tsum = 0;
	//			tsumb = 0;
	//			tsumg = 0;
	//			tsumr = 0;
	//			//
	//			if (0 < X && X < 12 && 0 < Y && Y < 9 && (X * Y > i)) {
	//				for (y = 0; y < 64; y++) {
	//					sumb = abs(arr[py][px + 1].pixel[y][0] - arr[Y][X].pixel[y][3 * 63]) + sumb;
	//					sumg = abs(arr[py][px + 1].pixel[y][0 + 1] - arr[Y][X].pixel[y][3 * 63 + 1]) + sumg;
	//					sumr = abs(arr[py][px + 1].pixel[y][0 + 2] - arr[Y][X].pixel[y][3 * 63 + 2]) + sumr;
	//				}
	//				for (x = 0; x < 64; x++) {
	//					sumb = abs(arr[py + 1][px].pixel[0][3 * x] - arr[Y][X].pixel[63][3 * x]) + sumb;
	//					sumg = abs(arr[py + 1][px].pixel[0][3 * x + 1] - arr[Y][X].pixel[63][3 * x + 1]) + sumg;
	//					sumr = abs(arr[py + 1][px].pixel[0][3 * x + 2] - arr[Y][X].pixel[63][3 * x + 2]) + sumr;
	//				}
	//				sum = sumb + sumg + sumr;
	//				if (sum < minsum.min) {
	//					minsum.min = sum;
	//					minsum.x = X;
	//					minsum.y = Y;
	//				}
	//			}
	//		}
	//		Swap(&arr[py][px], &arr[minsum.y][minsum.x]);
	//		minsum.min = 255 * 3 * 64;
	//		minsum.x = 0;
	//		minsum.y = 0;
	//	}
	//}
///////////////////////////////
	////윗면 기준
	//for (px = 0; px < 12; px++) {
	//	for (py = 0; py < 9; py++) {
	//		for (i = 0; i < pixnum; i++) {
	//			sum = 0;
	//			sumb = 0;
	//			sumg = 0;
	//			sumr = 0;
	//			tsum = 0;
	//			tsumb = 0;
	//			tsumg = 0;
	//			tsumr = 0;
	//			if (0 < X && X < 12 && 0 < Y && Y < 9) {
	//				for (x = 0; x < 64; x++) {
	//					sumb = abs(arr[10 - 1 - py][px].pixel[0][3 * x] - arr[10 - 1 - Y][X].pixel[63][3 * x]) + sumb;
	//					sumg = abs(arr[10 - 1 - py][px].pixel[0][3 * x + 1] - arr[10 - 1 - Y][X].pixel[63][3 * x + 1]) + sumg;
	//					sumr = abs(arr[10 - 1 - py][px].pixel[0][3 * x + 2] - arr[10 - 1 - Y][X].pixel[63][3 * x + 2]) + sumr;
	//					tsumb = abs(arr[10 - 1 - py][px].pixel[0][3 * x] - arr[10 - 1 - py][px].pixel[1][3 * x]) + tsumb;
	//					tsumg = abs(arr[10 - 1 - py][px].pixel[0][3 * x + 1] - arr[10 - 1 - py][px].pixel[1][3 * x + 1]) + tsumg;
	//					tsumr = abs(arr[10 - 1 - py][px].pixel[0][3 * x + 2] - arr[10 - 1 - py][px].pixel[1][3 * x + 2]) + tsumr;
	//				}
	//				sum = sumb + sumg + sumr;
	//				tsum = tsumb + tsumg + tsumr;
	//				if (sum - tsum < minsum.min) {
	//					minsum.min = abs(sum - tsum);
	//					minsum.x = X;
	//					minsum.y = Y;
	//				}
	//			}
	//		}
	//		Swap(&arr[10 - 1 - py - 1][px], &arr[minsum.y][minsum.x]);
	//		minsum.min = 255 * 3 * 64;
	//		minsum.x = 0;
	//		minsum.y = 0;
	//	}
	//}


	////왼쪽측면
	//for (py = 1; py < 10; py++) {
	//	for (px = 1; px < 13; px++) {
	//		for (i = 0; i < pixnum; i++) {
	//			sum = 0;
	//			sumb = 0;
	//			sumg = 0;
	//			sumr = 0;
	//			tsum = 0;
	//			tsumb = 0;
	//			tsumg = 0;
	//			tsumr = 0;
	//			if (0 < X && X < 12 && 0 < Y && Y < 9) {
	//				for (y = 0; y < 64; y++) {
	//					sumb = abs(arr[py][13 - 1 - px].pixel[y][0] - arr[Y][13 - 1 - X].pixel[y][3 * 63]) + sumb;
	//					sumg = abs(arr[py][13 - 1 - px].pixel[y][0 + 1] - arr[Y][13 - 1 - X].pixel[y][3 * 63 + 1]) + sumg;
	//					sumr = abs(arr[py][13 - 1 - px].pixel[y][0 + 2] - arr[Y][13 - 1 - X].pixel[y][3 * 63 + 2]) + sumr;
	//					tsumb = abs(arr[py][13 - 1 - px].pixel[y][0] - arr[py][13 - 1 - px].pixel[y][3 * 1]) + tsumb;
	//					tsumg = abs(arr[py][13 - 1 - px].pixel[y][0 + 1] - arr[py][13 - 1 - px].pixel[y][3 * 1 + 1]) + tsumg;
	//					tsumr = abs(arr[py][13 - 1 - px].pixel[y][0 + 2] - arr[py][13 - 1 - px].pixel[y][3 * 1 + 2]) + tsumr;
	//				}
	//				sum = sumb + sumg + sumr;
	//				tsum = tsumb + tsumg + tsumr;
	//				if (sum - tsum < minsum.min) {
	//					minsum.min = abs(sum - tsum);
	//					minsum.x = X;
	//					minsum.y = Y;
	//				}
	//			}
	//		}
	//		Swap(&arr[py][13 - 1 - px - 1], &arr[minsum.y][minsum.x]);
	//		minsum.min = 255 * 3 * 64;
	//		minsum.x = 0;
	//		minsum.y = 0;
	//	}
	//}







	////////////////////////////
	//Swap(&arr[1][1], &arr[5][8]);
	//Swap(&arr[0][0], &arr[0][1]);

	////배열 위치에 따른 0,0에 입력 테스트
	//for (i = 0; i < 1; i++) {
	//	for (y = 0; y < 64; y++) {
	//		hy = height - 1 - y;
	//		hhy = 64 - 1 - y;
	//		for (x = 0; x < 64; x++) {
	//			bx = 3 * x + i * 64 * 3;
	//			gx = bx + 1;
	//			rx = bx + 2;
	//			m_InImg[hy][bx] = arr[0][0].pixel[y][3 * x];
	//			m_InImg[hy][gx] = arr[0][0].pixel[y][3 * x + 1];
	//			m_InImg[hy][rx] = arr[0][0].pixel[y][3 * x + 2];
	//			//m_InImg[hy][bx] = 128;
	//			//m_InImg[hy][gx] = 128;
	//			//m_InImg[hy][rx] = 128;
	//		}
	//	}
	//}

	//변경된 arr을 이미지에 입력 ok
	//스왑함수 ok
	//유사한 경계값 확인 알고리즘

*/

}



void CProject1Doc::Swap(MY* a, MY* b)
{
	// TODO: 여기에 구현 코드 추가.
	MY temp;
	temp = *a;
	*a = *b;
	*b = temp;
}




void CProject1Doc::DispImage(unsigned char** resultimg, MY** resultarr)
{
	// TODO: 여기에 구현 코드 추가.
	int i, y, hy, hhy, x, bx, gx, rx;
	for (i = 0; i < 1; i++) {
		for (y = 0; y < 64; y++) {
			hy = height - 1 - y;
			hhy = 64 - 1 - y;
			for (x = 0; x < 64; x++) {
				bx = 3 * x + i * 64 * 3;
				gx = bx + 1;
				rx = bx + 2;
				resultimg[hy][bx] = 128;
				resultimg[hy][gx] = 128;
				resultimg[hy][rx] = 128;
			}
		}
	}
}


void CProject1Doc::Run2()
{
	// TODO: 여기에 구현 코드 추가.
	m_histo_x = new int [width];
	m_histo_y = new int [height];

	int i;
	//원본이미지 저장용 2차원 배열
	m_OrgImg = new unsigned char * [height];
	for (i = 0; i < height; i++) {
		m_OrgImg[i] = new unsigned char[width*3];
	}

	//원본이미지 저장
	int pixO[3], pixC[3];
	int x, y, iy, bx, gx, rx;
	int sumO;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			bx = 3 * x;
			gx = bx + 1;
			rx = bx + 2;
			m_OrgImg[height - 1 - y][bx] = m_InImg[height - 1 - y][bx];
			m_OrgImg[height - 1 - y][gx] = m_InImg[height - 1 - y][gx];
			m_OrgImg[height - 1 - y][rx] = m_InImg[height - 1 - y][rx];
		}
	}

	//초기값
	pixO[0] = m_InImg[height - 1][0] * 0.114;
	pixO[1] = m_InImg[height - 1][1] * 0.587;
	pixO[2] = m_InImg[height - 1][2] * 0.299;
	pixC[0] = m_InImg[height - 1][0];
	pixC[1] = m_InImg[height - 1][1];
	pixC[2] = m_InImg[height - 1][2];
	int difr, difg, difb;

	//영상 그레이->이진화
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			bx = 3 * x;
			gx = bx + 1;
			rx = bx + 2;
			pix[0] = m_InImg[height - 1 - y][bx] * 0.114;
			pix[1] = m_InImg[height - 1 - y][gx] * 0.587;
			pix[2] = m_InImg[height - 1 - y][rx] * 0.299;
			pixsum2 = pix[0] + pix[1] + pix[2];
			if (pixsum2 > 156) pixsum2 = 255;
			else pixsum2 = 1;
			m_InImg[height - 1 - y][bx] = pixsum2;
			m_InImg[height - 1 - y][gx] = pixsum2;
			m_InImg[height - 1 - y][rx] = pixsum2;
		}
	}
	//만약 배경이 검은색이면 반전
	if (m_InImg[0][0] < 10) {
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				bx = 3 * x;
				gx = bx + 1;
				rx = bx + 2;
				pix[0] = m_InImg[height - 1 - y][bx] * 0.114;
				pix[1] = m_InImg[height - 1 - y][gx] * 0.587;
				pix[2] = m_InImg[height - 1 - y][rx] * 0.299;
				pixsum2 = pix[0] + pix[1] + pix[2];
				if (pixsum2 > 156) pixsum2 = 1;
				else pixsum2 = 255;
				m_InImg[height - 1 - y][bx] = pixsum2;
				m_InImg[height - 1 - y][gx] = pixsum2;
				m_InImg[height - 1 - y][rx] = pixsum2;
			}
		}
	}

	//복수라인 히스토그램
	for (y = 2; y < height - 2; y++) {
		m_histo_y[y] = 0;
		for (x = 0; x < width; x++) {
			bx = 3 * x;
			gx = bx + 1;
			rx = bx + 2;
			pix[0] = m_InImg[height - 1 - y][bx];
			pix[1] = m_InImg[height - 1 - y][gx];
			pix[2] = m_InImg[height - 1 - y][rx];
			pixsum2 = pix[0] + pix[1] + pix[2];
			if (pixsum2 < 10) {
				m_histo_y[y]++;
				m_histo_y[y - 1]++;
				m_histo_y[y + 1]++;
				m_histo_y[y - 2]++;
				m_histo_y[y + 2]++;
			}
		}
		m_histo_y[0] = 0;
		m_histo_y[1] = 0;
		m_histo_y[2] = 0;
	}
	
	//라인검출
	int chk;
	line = 0;
	chk = 0;
	for (y = 1; y < height; y++) {
		if (chk == 0 && (m_histo_y[y] >= 10)) {
			sy[line] = y;
			chk = 1;
		}
		else if (chk == 1 && (m_histo_y[y] < 10)) {
			ey[line] = y - 1;
			chk = 0;
			line++;
		}
	}
	
	//문자검출
	int n, ln, sum = 0, tn = 0;
	for (n = 0; n < line; n++) {
		for (x = 0; x < width; x++) {
			m_histo_x[x] = 0;
			for (y = sy[n]; y < ey[n]; y++) {
				bx = 3 * x;
				gx = bx + 1;
				rx = bx + 2;
				pix[0] = m_InImg[height - 1 - y][bx];
				pix[1] = m_InImg[height - 1 - y][gx];
				pix[2] = m_InImg[height - 1 - y][rx];
				pixsum2 = pix[0] + pix[1] + pix[2];
				if (pixsum2 < 10) m_histo_x[x]++;
			}
		}
		ln = 0;
		chk = 0;
		for (x = 0; x < width; x++) {
			if (chk == 0 && m_histo_x[x] >= 1) {
				letter_sx[n][ln] = x - 1;
				chk = 1;
			}
			else if (chk == 1 && m_histo_x[x] < 1) {
				letter_ex[n][ln] = x + 1;
				letter_dx[n][ln] = letter_ex[n][ln] - letter_sx[n][ln];
				chk = 0;
				sum = sum + letter_dx[n][ln];
				ln++;
				tn++;
			}
		}
		letter_no[n] = ln;
	}
	if (tn == 0) tn = 1;
	avwidth = sum / tn;
	ln = 0;
	
	//문자보정
	int m, sum1;
	int dw1, dw2;
	sum = avwidth;
	for (i = 0; i < 2; i++) {
		ln = 0;
		sum1 = 0;
		for (n = 0; n < line; n++) {
			for (m = 0; m < letter_no[n]; m++) {
				if (letter_dx[n][m] < sum) continue;
				sum1 = sum1 + letter_dx[n][m];
				ln++;
			}
		}
		sum = sum1 / ln;
	}
	avwidth = sum;
	for (n = 0; n < line; n++) {
		m = 0;
		while (m < letter_no[n]) {
			if (letter_dx[n][m] == 0 || letter_dx[n][m] >= avwidth) {
				m++;
				continue;
			}
			if (m == (letter_no[n] - 1)) {
				dw1 = abs(avwidth - letter_dx[n][m]);
				if (dw1 > (avwidth / 2)) letter_dx[n][m] = 0;
				m++;
				continue;
			}
			dw1 = abs(avwidth - letter_dx[n][m]);
			dw2 = abs(avwidth - (letter_dx[n][m] + letter_dx[n][m + 1]));
			if (dw1 <= dw2) {
				m++;
				continue;
			}
			letter_dx[n][m + 1] = 0;
			letter_ex[n][m] = letter_ex[n][m + 1];
			letter_dx[n][m] = letter_ex[n][m] - letter_sx[n][m];
			m++;
		}
	}
	ln = 0;
	int hh, ww, one, two;
	float ff;
	int  yy, xx;

	//첫글자를 기준으로
	for (m = 0; m < letter_no[1]; m++) {
		if (letter_dx[0][m] != 0) {
			ln++;
			if (ln == 1) break;
		}
	}
	pat_h = ey[0] - sy[0];
	pat_w = letter_dx[0][m];
	yy = sy[0];
	for (y = 0; y < ey[0] - sy[0] + 1; y++) {
		xx = letter_sx[0][m];
		for (x = 0; x < letter_dx[0][m]; x++) {
			bx = 3 *(x + xx);
			gx = bx + 1;
			rx = bx + 2;
			pix[0] = m_InImg[height - 1 - y - yy][bx];
			pix[1] = m_InImg[height - 1 - y - yy][gx];
			pix[2] = m_InImg[height - 1 - y - yy][rx];
			pixsum2 = pix[0] + pix[1] + pix[2];
			if (pixsum2 < 10) {
				pattern[y][x] = 0;
			}
			else {
				pattern[y][x] = 1;
			}
		}
	}
	for (m = 0; m < line; m++) {
		hh = ey[m] - sy[m];
		for (n = 0; n < letter_no[m]; n++) {
			same_no[m][n] = 0;
			if (letter_dx[m][n] == 0) continue;
			ww = letter_dx[m][n];
			yy = sy[m];
			for (y = 0; y < hh; y++) {
				xx = letter_sx[m][n];
				for (x = 0; x < ww; x++) {
					bx = 3 * (x + xx);
					gx = bx + 1;
					rx = bx + 2;
					pix[0] = m_InImg[height - 1 - y - yy][bx];
					pix[1] = m_InImg[height - 1 - y - yy][gx];
					pix[2] = m_InImg[height - 1 - y - yy][rx];
					pixsum2 = pix[0] + pix[1] + pix[2];
					if (pixsum2 < 10) img[y][x] = 0;
					else img[y][x] = 1;
				}
			}
			sum = 0;
			one = 0;
			two = 0;
			for (y = 0; y < hh && y < pat_h; y++) {
				for (x = 0; x < ww && x < pat_w; x++) {
					sum = sum + img[y][x] * pattern[y][x];
					one = one + img[y][x] * img[y][x];
					two = two + pattern[y][x] * pattern[y][x];
				}
			}
			//ff = one - two;
			ff = (float)sqrt((float)one) * (float)sqrt((float)two);
			ff = (float)sum / ff;
			//if (ff > 0.875) same_no[m][n] = 1;
			if (ff > 0.885) same_no[m][n] = 1;
		}
	}

	//두번째글짜
	ln = 0;
	for (m = 1; m < letter_no[1]; m++) {
		if (letter_dx[0][m] != 0) {
			ln++;
			if (ln == 1) break;
		}
	}
	pat_h = ey[0] - sy[0];
	pat_w = letter_dx[0][m];
	yy = sy[0];
	for (y = 0; y < ey[0] - sy[0] + 1; y++) {
		xx = letter_sx[0][m];
		for (x = 0; x < letter_dx[0][m]; x++) {
			bx = 3 * (x + xx);
			gx = bx + 1;
			rx = bx + 2;
			pix[0] = m_InImg[height - 1 - y - yy][bx];
			pix[1] = m_InImg[height - 1 - y - yy][gx];
			pix[2] = m_InImg[height - 1 - y - yy][rx];
			pixsum2 = pix[0] + pix[1] + pix[2];
			if (pixsum2 < 10) {
				pattern[y][x] = 0;
			}
			else {
				pattern[y][x] = 1;
			}
		}
	}
	printf("");
	for (m = 0; m < line; m++) {
		hh = ey[m] - sy[m];
		for (n = 0; n < letter_no[m]; n++) {
			//same_no[m][n] = 0;
			if (letter_dx[m][n] == 0) continue;
			ww = letter_dx[m][n];
			yy = sy[m];
			for (y = 0; y < hh; y++) {
				xx = letter_sx[m][n];
				for (x = 0; x < ww; x++) {
					bx = 3 * (x + xx);
					gx = bx + 1;
					rx = bx + 2;
					pix[0] = m_InImg[height - 1 - y - yy][bx];
					pix[1] = m_InImg[height - 1 - y - yy][gx];
					pix[2] = m_InImg[height - 1 - y - yy][rx];
					pixsum2 = pix[0] + pix[1] + pix[2];
					if (pixsum2 < 10) img[y][x] = 0;
					else img[y][x] = 1;
				}
			}
			sum = 0;
			one = 0;
			two = 0;
			for (y = 0; y < hh && y < pat_h; y++) {
				for (x = 0; x < ww && x < pat_w; x++) {
					sum = sum + img[y][x] * pattern[y][x];
					one = one + img[y][x] * img[y][x];
					two = two + pattern[y][x] * pattern[y][x];
				}
			}
			//ff = one - two;
			ff = (float)sqrt((float)one) * (float)sqrt((float)two);
			ff = (float)sum / ff;
			//if (ff > 0.875) same_no[m][n] = 1;
			if (ff > 0.92) same_no[m][n] = same_no[m][n] + 1;
		}
	}

	//두번째글짜
	ln = 0;
	for (m = 1; m < letter_no[1]; m++) {
		if (letter_dx[0][m] != 0) {
			ln++;
			if (ln == 1) break;
		}
	}
	pat_h = ey[0] - sy[0];
	pat_w = letter_dx[0][m];
	yy = sy[0];
	for (y = 0; y < ey[0] - sy[0] + 1; y++) {
		xx = letter_sx[0][m];
		for (x = 0; x < letter_dx[0][m]; x++) {
			bx = 3 * (x + xx);
			gx = bx + 1;
			rx = bx + 2;
			pix[0] = m_InImg[height - 1 - y - yy][bx];
			pix[1] = m_InImg[height - 1 - y - yy][gx];
			pix[2] = m_InImg[height - 1 - y - yy][rx];
			pixsum2 = pix[0] + pix[1] + pix[2];
			if (pixsum2 < 10) {
				pattern[y][x] = 0;
			}
			else {
				pattern[y][x] = 1;
			}
		}
	}
	printf("");
	for (m = 0; m < line; m++) {
		hh = ey[m] - sy[m];
		for (n = 0; n < letter_no[m]; n++) {
			//same_no[m][n] = 0;
			if (letter_dx[m][n] == 0) continue;
			ww = letter_dx[m][n];
			yy = sy[m];
			for (y = 0; y < hh; y++) {
				xx = letter_sx[m][n];
				for (x = 0; x < ww; x++) {
					bx = 3 * (x + xx);
					gx = bx + 1;
					rx = bx + 2;
					pix[0] = m_InImg[height - 1 - y - yy][bx];
					pix[1] = m_InImg[height - 1 - y - yy][gx];
					pix[2] = m_InImg[height - 1 - y - yy][rx];
					pixsum2 = pix[0] + pix[1] + pix[2];
					if (pixsum2 < 10) img[y][x] = 0;
					else img[y][x] = 1;
				}
			}
			sum = 0;
			one = 0;
			two = 0;
			for (y = 0; y < hh && y < pat_h; y++) {
				for (x = 0; x < ww && x < pat_w; x++) {
					sum = sum + img[y][x] * pattern[y][x];
					one = one + img[y][x] * img[y][x];
					two = two + pattern[y][x] * pattern[y][x];
				}
			}
			//ff = one - two;
			ff = (float)sqrt((float)one) * (float)sqrt((float)two);
			ff = (float)sum / ff;
			//if (ff > 0.875) same_no[m][n] = 1;
			if (ff > 0.92) same_no[m][n] = same_no[m][n] + 1;
		}
	}

	////세번째글짜
	ln = 0;
	for (m = 2; m < letter_no[2]; m++) {
		if (letter_dx[0][m] != 0) {
			ln++;
			if (ln == 1) break;
		}
	}
	pat_h = ey[0] - sy[0];
	pat_w = letter_dx[0][m];
	yy = sy[0];
	for (y = 0; y < ey[0] - sy[0] + 1; y++) {
		xx = letter_sx[0][m];
		for (x = 0; x < letter_dx[0][m]; x++) {
			bx = 3 * (x + xx);
			gx = bx + 1;
			rx = bx + 2;
			pix[0] = m_InImg[height - 1 - y - yy][bx];
			pix[1] = m_InImg[height - 1 - y - yy][gx];
			pix[2] = m_InImg[height - 1 - y - yy][rx];
			pixsum2 = pix[0] + pix[1] + pix[2];
			if (pixsum2 < 10) {
				pattern[y][x] = 0;
			}
			else {
				pattern[y][x] = 1;
			}
		}
	}
	printf("");
	for (m = 0; m < line; m++) {
		hh = ey[m] - sy[m];
		for (n = 0; n < letter_no[m]; n++) {
			//same_no[m][n] = 0;
			if (letter_dx[m][n] == 0) continue;
			ww = letter_dx[m][n];
			yy = sy[m];
			for (y = 0; y < hh; y++) {
				xx = letter_sx[m][n];
				for (x = 0; x < ww; x++) {
					bx = 3 * (x + xx);
					gx = bx + 1;
					rx = bx + 2;
					pix[0] = m_InImg[height - 1 - y - yy][bx];
					pix[1] = m_InImg[height - 1 - y - yy][gx];
					pix[2] = m_InImg[height - 1 - y - yy][rx];
					pixsum2 = pix[0] + pix[1] + pix[2];
					if (pixsum2 < 10) img[y][x] = 0;
					else img[y][x] = 1;
				}
			}
			sum = 0;
			one = 0;
			two = 0;
			for (y = 0; y < hh && y < pat_h; y++) {
				for (x = 0; x < ww && x < pat_w; x++) {
					sum = sum + img[y][x] * pattern[y][x];
					one = one + img[y][x] * img[y][x];
					two = two + pattern[y][x] * pattern[y][x];
				}
			}
			//ff = one - two;
			ff = (float)sqrt((float)one) * (float)sqrt((float)two);
			ff = (float)sum / ff;
			//if (ff > 0.875) same_no[m][n] = 1;
			if (ff > 0.92) same_no[m][n] = same_no[m][n] + 1;
		}
	}
}


void CProject1Doc::Test()
{
	// TODO: 여기에 구현 코드 추가.
	int i;
	m_OrgImg = new unsigned char* [height];
	for (i = 0; i < height; i++) {
		m_OrgImg[i] = new unsigned char[width * 3];
	}


	int pixO[3], pixC[3];
	int x, y, iy, bx, gx, rx;
	int sumO;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			bx = 3 * x;
			gx = bx + 1;
			rx = bx + 2;
			m_OrgImg[height - 1 - y][bx] = m_InImg[height - 1 - y][bx];
			m_OrgImg[height - 1 - y][gx] = m_InImg[height - 1 - y][gx];
			m_OrgImg[height - 1 - y][rx] = m_InImg[height - 1 - y][rx];
		}
	}

	pixO[0] = m_InImg[height - 1][0] * 0.114;
	pixO[1] = m_InImg[height - 1][1] * 0.587;
	pixO[2] = m_InImg[height - 1][2] * 0.299;
	pixC[0] = m_InImg[height - 1][0];
	pixC[1] = m_InImg[height - 1][1];
	pixC[2] = m_InImg[height - 1][2];
	int difr, difg, difb;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			bx = 3 * x;
			gx = bx + 1;
			rx = bx + 2;
			pix[0] = m_InImg[height - 1 - y][bx] * 0.114;
			pix[1] = m_InImg[height - 1 - y][gx] * 0.587;
			pix[2] = m_InImg[height - 1 - y][rx] * 0.299;
			pixsum2 = pix[0] + pix[1] + pix[2];
			m_InImg[height - 1 - y][bx] = pixsum2;
			m_InImg[height - 1 - y][gx] = pixsum2;
			m_InImg[height - 1 - y][rx] = pixsum2;
		}
	}

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			bx = 3 * x;
			gx = bx + 1;
			rx = bx + 2;
			pix[0] = m_InImg[height - 1 - y][bx] * 0.114;
			pix[1] = m_InImg[height - 1 - y][gx] * 0.587;
			pix[2] = m_InImg[height - 1 - y][rx] * 0.299;
			pixsum2 = pix[0] + pix[1] + pix[2];
			//if (pixsum2 > 150) pixsum2 = 255;
			//else pixsum2 = 1;
			m_InImg[height - 1 - y][bx] = pixsum2;
			m_InImg[height - 1 - y][gx] = pixsum2;
			m_InImg[height - 1 - y][rx] = pixsum2;
		}
	}
	if (m_InImg[0][0] < 10) {
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				bx = 3 * x;
				gx = bx + 1;
				rx = bx + 2;
				pix[0] = m_InImg[height - 1 - y][bx] * 0.114;
				pix[1] = m_InImg[height - 1 - y][gx] * 0.587;
				pix[2] = m_InImg[height - 1 - y][rx] * 0.299;
				pixsum2 = pix[0] + pix[1] + pix[2];
				if (pixsum2 > 150) pixsum2 = 1;
				else pixsum2 = 255;
				m_InImg[height - 1 - y][bx] = pixsum2;
				m_InImg[height - 1 - y][gx] = pixsum2;
				m_InImg[height - 1 - y][rx] = pixsum2;
			}
		}
	}

	

}


void CProject1Doc::Run3()
{
	// TODO: 여기에 구현 코드 추가.
	clock_t startc, endc;
	int pixO[3], pixC[3];
	int x, y, iy, bx, gx, rx;
	int sumO;



	startc = clock();
	bx = 3 * width/2;
	gx = bx + 1;
	rx = bx + 2;

	pixC[0] = m_InImg[height / 2][bx];
	pixC[1] = m_InImg[height / 2][gx];
	pixC[2] = m_InImg[height / 2][rx];
	sumC = pixC[0] + pixC[1] + pixC[2];
	
	//영상 그레이->이진화
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			bx = 3 * x;
			gx = bx + 1;
			rx = bx + 2;
			pix[0] = m_InImg[height - 1 - y][bx] * 0.114;
			pix[1] = m_InImg[height - 1 - y][gx] * 0.587;
			pix[2] = m_InImg[height - 1 - y][rx] * 0.299;
			pixsum2 = pix[0] + pix[1] + pix[2];
			//if (pixsum2 > 127) pixsum2 = 255;
			//else pixsum2 = 1;
			m_InImg[height - 1 - y][bx] = pixsum2;
			m_InImg[height - 1 - y][gx] = pixsum2;
			m_InImg[height - 1 - y][rx] = pixsum2;
		}
	}
	
	SetPix();
	check = 0;
	FillEllipse(height / 2, width / 2); //0


	SetPix();
	check = 1;
	FillEllipse(height / 2, width / 2); //1

	SetPix();
	check = 2;
	FillEllipse(height / 2, width / 2); //2

	SetPix();
	check = 3;
	FillEllipse(height / 2 + 1, width / 2 - 1); //3

	check = 4;
	SetPix();

	int min_x = width / 2;
	int max_x = width / 2;
	int min_y = width / 2;
	int max_y = width / 2;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			bx = 3 * x;
			gx = bx + 1;
			rx = bx + 2;
			if (m_InImg[height - 1 - y][bx] == 0) {
				if (x < min_x) {
					min_x = x;
				}
				if (x > max_x) max_x = x;
				if (y < min_y) min_y = y;
				if (y > max_y) max_y = y;
			}
		}
	}
	center_x = (max_x + min_x) / 2;
	center_y = (max_y + min_y) / 2;


	bx = 3 * center_x;
	gx = bx + 1;
	rx = bx + 2;

	m_InImg[height - 1 - center_y][bx] = 255;
	m_InImg[height - 1 - center_y][gx] = 255;
	m_InImg[height - 1 - center_y][rx] = 255;

	double distance = 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < center_x; x++) {
			bx = 3 * x;
			gx = bx + 1;
			rx = bx + 2;
			if (m_InImg[height - 1 - y][bx] == 0) {
				if (distance < sqrt(pow(center_x - x, 2) + pow(center_y - y, 2))) {
					distance = sqrt(pow(center_x - x, 2) + pow(center_y - y, 2));
					maxdis_x = x;
					maxdis_y = y;
				}
			}
		}
	}

	bx = 3 * maxdis_x;
	gx = bx + 1;
	rx = bx + 2;

	m_InImg[height - 1 - maxdis_y][bx] = 255;
	m_InImg[height - 1 - maxdis_y][gx] = 255;
	m_InImg[height - 1 - maxdis_y][rx] = 255;

	endc = clock();

	runtime = (double)(endc - startc)/CLOCKS_PER_SEC;
	
}


void CProject1Doc::FillEllipse(int y, int x)
{
	// TODO: 여기에 구현 코드 추가.
	if (m_InImg[height - 1 - y][3 * x] < 115) return;

	int bx, gx, rx;

	bx = 3 * x;
	gx = bx + 1;
	rx = bx + 2;
	m_InImg[height - 1 - y][bx] = 0;
	m_InImg[height - 1 - y][gx] = 0;
	m_InImg[height - 1 - y][rx] = 0;

	switch (check) {
	case 0:
		FillEllipse(y - 1, x);
		FillEllipse(y, x - 1);
		break;
	case 1:
		FillEllipse(y + 1, x);
		FillEllipse(y, x + 1);
		break;
	case 2:
		FillEllipse(y - 1, x);
		FillEllipse(y, x + 1);
		break;
	case 3:
		FillEllipse(y + 1, x);
		FillEllipse(y, x - 1);
		break;
	}
}


void CProject1Doc::SetPix()
{
	// TODO: 여기에 구현 코드 추가.
	int bx, gx, rx;

	bx = 3 * width / 2;
	gx = bx + 1;
	rx = bx + 2;

	m_InImg[height - 1 - height / 2][bx] = 180;
	m_InImg[height - 1 - height / 2][gx] = 180;
	m_InImg[height - 1 - height / 2][rx] = 180;

	m_InImg[height - 1 - height / 2][bx+1] = 180;
	m_InImg[height - 1 - height / 2][gx+1] = 180;
	m_InImg[height - 1 - height / 2][rx+1] = 180;

	m_InImg[height - 1 - height / 2][bx-1] = 180;
	m_InImg[height - 1 - height / 2][gx-1] = 180;
	m_InImg[height - 1 - height / 2][rx-1] = 180;

	m_InImg[height - 1 - height / 2 + 1][bx] = 180;
	m_InImg[height - 1 - height / 2 + 1][gx] = 180;
	m_InImg[height - 1 - height / 2 + 1][rx] = 180;

	m_InImg[height - 1 - height / 2 - 1][bx] = 180;
	m_InImg[height - 1 - height / 2 - 1][gx] = 180;
	m_InImg[height - 1 - height / 2 - 1][rx] = 180;

	if (check == 4) {
		m_InImg[height - 1 - height / 2][bx] = 0;
		m_InImg[height - 1 - height / 2][gx] = 0;
		m_InImg[height - 1 - height / 2][rx] = 0;

		m_InImg[height - 1 - height / 2][bx + 1] = 0;
		m_InImg[height - 1 - height / 2][gx + 1] = 0;
		m_InImg[height - 1 - height / 2][rx + 1] = 0;

		m_InImg[height - 1 - height / 2][bx - 1] = 0;
		m_InImg[height - 1 - height / 2][gx - 1] = 0;
		m_InImg[height - 1 - height / 2][rx - 1] = 0;

		m_InImg[height - 1 - height / 2 + 1][bx] = 0;
		m_InImg[height - 1 - height / 2 + 1][gx] = 0;
		m_InImg[height - 1 - height / 2 + 1][rx] = 0;

		m_InImg[height - 1 - height / 2 - 1][bx] = 0;
		m_InImg[height - 1 - height / 2 - 1][gx] = 0;
		m_InImg[height - 1 - height / 2 - 1][rx] = 0;
	}
}
