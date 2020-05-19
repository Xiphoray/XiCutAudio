
// ChildView.cpp: CChildView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "CutAudio.h"
#include "ChildView.h"
#include "XiWave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void	CStringToChar(CString str, char ch[])
{
	char* tmpch;
	int wLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	tmpch = new char[wLen + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, tmpch, wLen, NULL, NULL);
	for (int i = 0; i < wLen; ++i)
		ch[i] = tmpch[i];
}

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_INPUTBUTTON, OnBtnDown)
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}

void CChildView::OnBtnDown()
{
	XiWave wave;
	int input;

	CString szFilter;
	szFilter = "wav|*.wav|*.*|*.*||";
	CFileDialog FD(TRUE, _T("wav"), _T("*.wav"), OFN_HIDEREADONLY, szFilter);
	if (FD.DoModal() == IDOK) {
		char filename[300];
		char filepath[200];
		CString filepa;
		CStringToChar(FD.GetPathName(), filename);
		if (!wave.readwav(filename)) {
			MessageBox(_T("错误文件。"));
			return;
		}
		filepa = FD.GetFolderPath() + _T("\\test.wav");
		CStringToChar(filepa, filepath);

		CString str;
		char ss[20];
		GetDlgItemText(ID_SPERETIME, str);
		CStringToChar(str, ss);
		double d = atof(ss);

		input = GetDlgItemInt(ID_THRESHOLD);
		
		wave.SetLast(d);
		wave.SetThreshold(input);

		if (!wave.cutpro()) {
			MessageBox(_T("剪辑有问题。"));
			return;
		}

		wave.writewav(filepath);

		MessageBox(_T("完成"));

		
	}

}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	inputbutton.Create(
		_T("一个按钮"),//按钮的标题 相当于属性里面的Caption
		WS_VISIBLE ,//按钮的风格
		CRect(280, 240, 400, 300),//按钮所在的位置和大小
		this,//父窗口指针
		ID_INPUTBUTTON//控件的ID
	);
	inputbutton.ShowWindow(SW_SHOWNORMAL);//显示出来

	speretimetext.Create(_T("间隔时间/s"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(80, 100, 180, 120), this, ID_SPERETIMETEXT);
	speretime.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, CRect(180, 100, 280, 120), this, ID_SPERETIME);
	thresholdtext.Create(_T("音量阈值"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(380, 100, 480, 120), this, ID_THRESHOLDTEXT);
	threshold.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, CRect(480, 100, 580, 120), this, ID_THRESHOLD);
	SetDlgItemText(ID_THRESHOLD, _T("5"));
	SetDlgItemText(ID_SPERETIME, _T("1"));
	return 0;
}
