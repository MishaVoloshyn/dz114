#pragma once
#include "header.h"

const int numEdit = 5;

class signUpDlg {
public:
	int inputIdsSignUp[numEdit] = { IDC_EDIT1, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5};
	HWND hEditSignUp[numEdit] = { 0 };
	HWND hButtonSignUp;

	signUpDlg(void);
	signUpDlg(LPCTSTR);
	~signUpDlg();
	bool isLoginExist(HWND hwnd, const _TCHAR* login);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static signUpDlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
};
