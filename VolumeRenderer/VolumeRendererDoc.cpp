
// VolumeRendererDoc.cpp: CVolumeRendererDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "VolumeRenderer.h"
#endif

#include "VolumeRendererDoc.h"
// View 클래스 사용하기 위해 선언
#include "VolumeRendererView.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVolumeRendererDoc

IMPLEMENT_DYNCREATE(CVolumeRendererDoc, CDocument)

BEGIN_MESSAGE_MAP(CVolumeRendererDoc, CDocument)
	ON_COMMAND(ID_SLICERENDERING_ZDIRECTION, &CVolumeRendererDoc::OnSlicerenderingZdirection)
	ON_COMMAND(ID_SLICERENDERING_YDIRECTION, &CVolumeRendererDoc::OnSlicerenderingYdirection)
	ON_COMMAND(ID_SLICERENDERING_XDIRECTION, &CVolumeRendererDoc::OnSlicerenderingXdirection)
	ON_COMMAND(ID_SLICERENDERING_MIP, &CVolumeRendererDoc::OnSlicerenderingMip)
END_MESSAGE_MAP()


// CVolumeRendererDoc 생성/소멸

CVolumeRendererDoc::CVolumeRendererDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CVolumeRendererDoc::~CVolumeRendererDoc()
{
}

BOOL CVolumeRendererDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	// 새 창이 뜨면 data를 불러와 버퍼에 저장한다.
	FILE* fp;
	fopen_s(&fp, "data//Bighead.den", "rb");

	// volume 사이즈를 미리 계산해 둔 것이다.
	int width = 256;
	int height = 256;
	int depth = 225;
	unsigned char* temp_vol = new unsigned char[width*height*depth];
	fread(temp_vol, 1, sizeof(unsigned char)*width*height*depth, fp);

	// 클래스 변수에 볼륨을 저장하고 pointer 변수가 이를 가리키게 한다.
	m_pVolume = shared_ptr<Volume>(new Volume(temp_vol,width,height,depth));

	fclose(fp);

	delete[] temp_vol;

	printf("volume load complete\n");
	return TRUE;
}




// CVolumeRendererDoc serialization

void CVolumeRendererDoc::Serialize(CArchive& ar)
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
void CVolumeRendererDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CVolumeRendererDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CVolumeRendererDoc::SetSearchContent(const CString& value)
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

// CVolumeRendererDoc 진단

#ifdef _DEBUG
void CVolumeRendererDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVolumeRendererDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVolumeRendererDoc 명령

// volume 을 z 방향으로 자른 단면을 보여주는 기능
void CVolumeRendererDoc::OnSlicerenderingZdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 단면의 화면 버퍼를 만든다.
	int img_width = m_pVolume->getWidth();
	int img_height = m_pVolume->getHeight();
	shared_ptr<unsigned char> image = 
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);

	// 버퍼에 단면정보를 저장한다.
	for (int j = 0; j < img_height; j++)
	{
		for (int i = 0; i < img_width; i++)
		{
			// z-index 120 인 화면의 단면 정보를 저장한다.
			image.get()[img_width*j + i] = m_pVolume->getVoxel(i, j, 120);
		}
	}

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}

// volume 을 y 방향으로 자른 단면을 보여주는 기능
void CVolumeRendererDoc::OnSlicerenderingYdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int img_width = m_pVolume->getWidth();
	int img_height = m_pVolume->getDepth();
	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);

	for (int j = 0; j < img_height; j++)
	{
		for (int i = 0; i < img_width; i++)
		{// y-index 가 150 인 이미지의 단면 정보를 세팅한다.
			image.get()[j*img_width + i]
				= m_pVolume->getVoxel(i, 150, j);
		}
	}

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnSlicerenderingXdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int img_width = m_pVolume->getHeight();
	int img_height = m_pVolume->getDepth();
	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);

	for (int j = 0; j < img_height; j++)
	{
		for (int i = 0; i < img_width; i++)
		{
			// x-index 가 150 인 이미지의 단면 정보를 세팅한다.
			image.get()[img_width*j + i] = m_pVolume->getVoxel(150, i, j);
		}
	}

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnSlicerenderingMip()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int img_width = m_pVolume->getWidth();
	int img_height = m_pVolume->getHeight();
	int img_depth = m_pVolume->getDepth();

	shared_ptr<unsigned char> Mip_Image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);
	memset(Mip_Image.get(), 0, sizeof(unsigned char)*img_width*img_height);

	for (int z = 0; z < img_depth; z++)
	{
		for (int j = 0; j < img_height; j++)
		{
			for (int i = 0; i < img_width; i++)
			{
				Mip_Image.get()[img_width * j + i] =
					__max(Mip_Image.get()[img_width * j + i],
						m_pVolume->getVoxel(i, j, z));
			}
		}
	}

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(Mip_Image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}
