#include "stdafx.h"
#include "page.h"

CPage::CPage(void)
{
    m_hWnd = NULL;
}

CPage::~CPage(void)
{
}

void CPage::Activate(void)
{
    ShowWindow(m_hWnd, SW_SHOWNORMAL);
}

void CPage::DeActivate(void)
{
    ShowWindow(m_hWnd, SW_HIDE);
}

void CPage::DoCommand(WPARAM wParam, LPARAM lParam)
{    
}

void CPage::RefreshUI(UINT32 u4Para1, UINT32 u4Para2)
{
    
}