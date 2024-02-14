
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASS wndclass;
	hInst = hInstance;
	HWND hwnd;
	MSG msg;


	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = TEXT("HelloClass");
	if (!RegisterClass(&wndclass)) return 1;


	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 6 번째 윈도우"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

BOOL CALLBACK Dlg6_2Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_BUTTON_PRINT:
			hdc = GetDC(hDlg);
			TextOut(hdc, 0, 0, _T("Hello World!"), 11);
			ReleaseDC(hDlg, hdc);
			break;
		case ID_BUTTON_END:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}


BOOL CALLBACK Dlg6_3Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	HWND hButton;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		hButton = GetDlgItem(hDlg, ID_PAUSE);
		EnableWindow(hButton, FALSE);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_START:
			hButton = GetDlgItem(hDlg, ID_START);
			EnableWindow(hButton, FALSE);
			hButton = GetDlgItem(hDlg, ID_PAUSE);
			EnableWindow(hButton, TRUE);
			break;
		case ID_PAUSE:
			hButton = GetDlgItem(hDlg, ID_START);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, ID_PAUSE);
			EnableWindow(hButton, FALSE);
			break;

		case ID_CLOSE:
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}


BOOL CALLBACK Dlg6_4Proc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	TCHAR word[100];

	switch (iMsg) {
	case WM_INITDIALOG:
		return 1;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_BUTTON_COPY:
			GetDlgItemText(hDlg, IDC_EDIT_SOURCE, word, 100);
			SetDlgItemText(hDlg, IDC_EDIT_COPY, word);
			break;

		case ID_BUTTON_CLEAR:
			SetDlgItemText(hDlg, IDC_EDIT_SOURCE, _T(""));
			SetDlgItemText(hDlg, IDC_EDIT_COPY, _T(""));
			break;

		case ID_BUTTON_END:
			EndDialog(hDlg, 0);
			break;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	} 
	return 0;
}


BOOL CALLBACK Dlg6_5Proc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	static int Check[3], Radio;
	TCHAR hobby[][30] = { _T("독서"), _T("음악감상"), _T("인터넷") };
	TCHAR sex[][30] = { _T("여성"), _T("남성") };
	TCHAR output[200];

	switch (iMsg) {
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO_FEMALE, IDC_RADIO_MALE, IDC_RADIO_FEMALE);
		return 1;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CHECK_READING:
			Check[0] = 1 - Check[0];
			break;

		case IDC_CHECK_MUSIC:
			Check[1] = 1 - Check[1];
			break;

		case IDC_CHECK_INTERNET:
			Check[2] = 1 - Check[2];
			break;

		case IDC_RADIO_FEMALE:
			Radio = 0;
			break;
			
		case IDC_RADIO_MALE:
			Radio = 1;
			break;

		case IDC_BUTTON_OUTPUT:
			_stprintf_s(output, _T("선택한 취미는 %s %s %s입니다. \r\n")
				_T("선택한 성별은 %s입니다."),
				Check[0] ? hobby[0] : _T(""), Check[1] ? hobby[1] : _T(""), Check[2] ? hobby[2] : _T(""), sex[Radio]);
			SetDlgItemText(hDlg, IDC_EDIT_OUTPUT, output);
			break;

		case IDC_BUTTON_CLOSE:
			EndDialog(hDlg, 0);
			break;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}


BOOL CALLBACK Dlg6_6Proc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	static HWND hCombo;
	static int selection;
	TCHAR name[20];

	switch (iMsg) {
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO_LIST);
		return 1;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_INSERT:
			GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
			if (_tcscmp(name, _T("")))
				SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)name);
			SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
			return 0;

		case IDC_BUTTON_DELETE:
			SendMessage(hCombo, CB_DELETESTRING, selection, 0);
			SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
			return 0;
		
		case IDC_COMBO_LIST:
			if (HIWORD(wParam) == CBN_SELCHANGE)
				selection = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			break;
		
		case IDCLOSE:
			EndDialog(hDlg, 0);
			break;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}


BOOL CALLBACK Dlg6_7Proc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	static HWND hList;
	static int selection;
	TCHAR name[20];

	switch (iMsg) {
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_LIST_NAME);
		return 1;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_INSERTT:
			GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
			if (_tcscmp(name, _T("")))
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)name);
			SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
			return 0;

		case IDC_BUTTON_DELETE:
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
			return 0;

		case IDC_LIST_NAME:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = SendMessage(hList, LB_GETCURSEL, 0, 0);
			break;

		case IDCLOSE:
			EndDialog(hDlg, 0);
			break;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message) {
	case WM_CREATE:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_6_3_MENU:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6_3), hwnd, Dlg6_3Proc);
			break;

		case ID_6_4_MENU:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6_4), hwnd, Dlg6_4Proc);
			break;

		case ID_6_5_MENU:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6_5), hwnd, Dlg6_5Proc);
			break;
			
		case ID_6_6_MENU:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6_6), hwnd, Dlg6_6Proc);
			break;

		case ID_6_7_MENU:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6_7), hwnd, Dlg6_7Proc);
			break;
		}


	case WM_LBUTTONDOWN:
		/*DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6_1), hwnd, Dlg6_1Proc);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6_2), hwnd, Dlg6_2Proc);*/
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}