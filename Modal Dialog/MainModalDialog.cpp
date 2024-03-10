#include "MainModalDialog.h"
#include "AdditionalModalDialog.h"
#include <string>
#include <tchar.h>
#include <fstream>


//регистрация

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

signUpDlg* signUpDlg::ptr = NULL;

signUpDlg::signUpDlg(void) 
{
	ptr = this;
}
signUpDlg::signUpDlg(LPCTSTR lpStr) 
{
	ptr = this;
}

signUpDlg::~signUpDlg() 
{
}

void signUpDlg::Cls_OnClose(HWND hwnd) 
{
	EndDialog(hwnd, IDCANCEL);
}

BOOL signUpDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	for (int i = 0; i < numEdit; i++) 
	{
		hEditSignUp[i] = GetDlgItem(hwnd, inputIdsSignUp[i]);
	}

	hButtonSignUp = GetDlgItem(hwnd, IDOK);

	return TRUE;
}

void signUpDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) 
{
	if (IDOK == id) 
	{
		_TCHAR buff[256] = _T("");
		int length = SendMessage(hEditSignUp[3], WM_GETTEXTLENGTH, 0, 0);
		GetWindowText(hEditSignUp[3], buff, length + 1);

		if (!isLoginExist(hwnd, buff)) 
		{
			int index = MessageBox(hwnd, _T("Хотите авторизоваться?"), _T("Такой логин уже есть!"), MB_YESNO);

			if (index == IDYES) 
			{
				signInDlg dlg;
				INT_PTR result = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hwnd, signInDlg::DlgProc);
			}

			return;
		}
		tfstream out(TEXT("info.txt"), std::ios::app);

		for (int i = 0; i < numEdit; i++) 
		{
			length = SendMessage(hEditSignUp[i], WM_GETTEXTLENGTH, 0, 0);

			if (length == 0) 
			{
				MessageBox(hwnd, _T("Заполните все поля!"), _T("Info!"), 0);
				break;
			}

			GetWindowText(hEditSignUp[i], buff, length + 1);

			out << (i == 0 ? "[ " : (i != numEdit - 1 ? "" : " ")) << buff << (i != numEdit - 1 ? " | " : " ]\n");

		}
	}
}

INT_PTR CALLBACK signUpDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) 
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}

bool signUpDlg::isLoginExist(HWND hwnd, const _TCHAR* login) 
{
	tifstream in("Database.txt");

	const int max_size = 256;
	int counter = 0;

	tstring temp[5];
	tstring tempChar;
	int index = 0;

	_TCHAR buff[max_size] = _T("");

	do {
		in >> buff[index];

		if (buff[index] == ']') 
		{
			buff[index + 1] = '\0';

			for (int j = 0; j < _tcslen(buff); j++) 
			{
				if (buff[j] != ' ' && buff[j] != '[' && buff[j] != '|' && buff[j] != ']') 
				{
					tempChar += buff[j];
				}
				else if (buff[j] == '|') 
				{
					temp[counter++] = tempChar;
					tempChar = _T("");
				}
			}

			tempChar = _T("");
			counter = 0;
			index = 0;
		}
		else {
			index++;
		}

		if (_tcscmp(login, temp[3].c_str()) == 0)
		{
			return false;
		}
			
	} while (in);

	return true;
}
