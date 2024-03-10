#include "AdditionalModalDialog.h"

//вход по логину/паролю

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace std;

signInDlg* signInDlg::ptr = NULL;
HHOOK hHook = NULL;
HWND hButtonSignIn;

_TCHAR buffLogin[256] = _T("");
_TCHAR buffPassword[256] = _T("");
bool buttonPressed = false;

signInDlg::signInDlg(void) 
{
	ptr = this;
}
signInDlg::signInDlg(LPCTSTR lpStr) 
{
	ptr = this;
}

signInDlg::~signInDlg() 
{
	UnhookWindowsHookEx(hHook);
}

void signInDlg::Cls_OnClose(HWND hwnd) 
{
	EndDialog(hwnd, IDCANCEL);
}

LRESULT CALLBACK WriteToFile(int nCode, WPARAM wParam, LPARAM lParam) 
{
	if (nCode == HC_ACTION && lParam && buttonPressed) 
	{
		buttonPressed = false;

		CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;

		std::wfstream out("signIn.txt", std::ios::app);

		if (!out.is_open()) 
		{
			MessageBox(0, _T("Файл не был открыт!"), _T("Ошибка!"), MB_OK);
			return CallNextHookEx(hHook, nCode, wParam, lParam);
		}

		out << buffLogin << "\n";
		out << buffPassword << "\n\n";

		out.close();
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

BOOL signInDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	for (int i = 0; i < numEdit; i++) {
		hEditSignIn[i] = GetDlgItem(hwnd, inputIdsSignIn[i]);
	}

	// Инициализация кнопки
	hButtonSignIn = GetDlgItem(hwnd, IDOK);

	hHook = SetWindowsHookEx(WH_CALLWNDPROC, WriteToFile, NULL, GetCurrentThreadId());

	return TRUE;
}

void signInDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (IDOK == id) {
		_TCHAR buff[100] = _T("");

		int length = 0;

		for (int i = 0; i < numEdit; i++) {
			length = SendMessage(hEditSignIn[i], WM_GETTEXTLENGTH, 0, 0);

			if (length == 0) 
			{
				MessageBox(hwnd, _T("Заполните все поля!"), _T("Error!"), 0);
				break;
			}

			if (i == 0) 
			{
				GetWindowText(hEditSignIn[0], buffLogin, length + 1);
				GetWindowText(hEditSignIn[1], buffPassword, length + 1);
			}
		}

		if (Authorization(hwnd, buffLogin, buffPassword)) 
		{
			int id = MessageBox(hwnd, _T("Добро пожаловать!!!"), _T("Вход!"), MB_OK);
			buttonPressed = true;
		}
		else 
		{
			MessageBox(hwnd, _T("Неверный логин или пароль!"), _T("Вход!"), MB_OK);
		}
	}
}

bool signInDlg::Authorization(HWND hwnd, const _TCHAR* login, const _TCHAR* password)
{
	wifstream in("info.txt");
	const int max_size = 255;
	tstring temp[5];
	tstring tempChar;
	int counter = 0;
	wchar_t buff[max_size];

	while (in.getline(buff, max_size))
	{
		int len = lstrlen(buff);

		for (int i = 0; i < len; i++)
		{
			if (buff[i] == L']')
			{
				buff[i] = L']';
				buff[i + 1] = L'\0';

				wistringstream wbuff(buff);

				while (wbuff >> tempChar)
				{
					if (tempChar != _T(" ") && tempChar != _T("[") && tempChar != _T("|") && tempChar != _T("]"))
					{
						temp[counter++] = tempChar;
					}
					else if (tempChar == _T("|") || tempChar == _T("]"))
					{
						tempChar = _T("");
						counter = 0;
					}
				}

				if (_tcscmp(login, temp[3].c_str()) == 0 && _tcscmp(password, temp[4].c_str()) == 0)
					return true;

				tempChar = _T("");
				counter = 0;
			}
		}
	}

	return false;
}
INT_PTR CALLBACK signInDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) 
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}