
// Project1Doc.h: CProject1Doc 클래스의 인터페이스
//


#pragma once
#define Y (aheight - 1 - (i / awidth) % aheight)
#define X (i % awidth)


class CProject1Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CProject1Doc() noexcept;
	DECLARE_DYNCREATE(CProject1Doc)

// 특성입니다.
public:
	BITMAPFILEHEADER dibHf; // 입력영상
	BITMAPINFOHEADER dibHi;
	unsigned char** m_InImg;
	int height;
	int width;
	BITMAPFILEHEADER t_dibHf; // 템플레이트 영상
	BITMAPINFOHEADER t_dibHi;
	unsigned char** m_TmpImg;
	int t_height;
	int t_width;
	int m_vote[8][8][4];
	int vote[8][8][4];
	int row, col;
	typedef struct _my {
		unsigned char** pixel;
	} MY;
	MY** arr;

	/*
	int m_histo_x[800]; // x축 방향 라인 히스토그램
	int m_histo_y[40]; // y축 방향 라인 히스토그램
	int sy, ey; // 줄 시작, 줄 끝
	int sx[50], ex[50]; // 문자 시작 , 문자 끝
	int dx[50], letter_no; // 문자폭, 검출문자수
	int width, height; // 문자 평균폭, 문자 높이
	int pattern[50][50], pat_h, pat_w; // 패턴 저장, 높이/폭
	int img[50][50], same_no[50];
	*/
	
	//복수라인
	unsigned char** m_OrgImg;
	int* m_histo_x; // x축 방향 라인 히스토그램
	int* m_histo_y; // y축 방향 라인 히스토그램
	int line;
	int sy[1000], ey[1000]; // 줄 수, 줄 시작, 줄 끝
	int letter_sx[1000][1000], letter_ex[1000][1000]; // 문자 시작 , 문자 끝
	int letter_dx[1000][1000], letter_no[1000]; // 문자폭, 검출문자수
	int avwidth, avheight; // 문자 평균폭, 문자 높
	int pattern[1000][1000], pat_h, pat_w; // 패턴 저장, 높이/폭
	int img[1000][1000], same_no[1000][1000];
	unsigned char pix[3];
	int pixsum, pixsum2;
	int sumC;
	unsigned int histoin[256];
	unsigned int histoout[256];
	int check = 0;
	//
	int center_x;
	int center_y;
	int maxdis_x;
	int maxdis_y;
	int check_center = 0;
	float runtime;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CProject1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void Run();
	void Swap(MY* a, MY* b);
	void DispImage(unsigned char** resultimg, MY** resultarr);
	void Run2();
	void Test();
	void Run3();
	void FillEllipse(int y, int x);
	void SetPix();
};
