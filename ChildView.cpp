﻿
// ChildView.cpp: CChildView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "CutAudio.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	MessageBox(_T("hello"), _T("helloworld"), MB_OK);
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
	SetDlgItemText(ID_THRESHOLD, _T("50"));
	SetDlgItemText(ID_SPERETIME, _T("1"));
	return 0;
}
