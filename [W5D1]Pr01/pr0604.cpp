
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <stdio.h>
#include <string>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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


	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 여섯 번째 윈도우"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

BOOL CALLBACK Calcu(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	static TCHAR num[100];
	static int count = 0, funcCount = 0;

	static float num1 = 0, num2 = 0, result = 0;

	static bool point = FALSE;

	static std::wstring resultStr;
	static const wchar_t* resultCStr;
	static std::wstring tempNum1;

	static TCHAR func;


	switch (iMsg) {
	case WM_INITDIALOG:
		return 1;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_0:
			num[count] += '0';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_1:
			num[count] += '1';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_2:
			num[count] += '2';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_3:
			num[count] += '3';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_4:
			num[count] += '4';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_5:
			num[count] += '5';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_6:
			num[count] += '6';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_7:
			num[count] += '7';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_8:
			num[count] += '8';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_9:
			num[count] += '9';
			SetDlgItemText(hDlg, IDC_EDIT, num);
			count += 1;
			break;

		case IDC_BUTTON_POINT:
			if (!point) {
				num[count++] += '.';
				SetDlgItemText(hDlg, IDC_EDIT, num);
				point = TRUE;
				break;
			}


		case IDC_BUTTON_PLUS:
			if (funcCount == 0) {
				GetDlgItemText(hDlg, IDC_EDIT, num, 100);
				num1 = std::stof(num);

				for (int i = 0; i < 100; i++) {
					num[i] = NULL;
				}

				SetDlgItemText(hDlg, IDC_EDIT, _T(""));
				point = FALSE;
				count = 0;
				func = 'P';
				funcCount++;
			}
			else {
				GetDlgItemText(hDlg, IDC_EDIT, num, 100);
				num2 = std::stof(num);

				result = num1 + num2;
				resultStr = std::to_wstring(result);
				resultCStr = resultStr.c_str();
				num1 = result;
				result = 0;
				SetDlgItemText(hDlg, IDC_EDIT, resultCStr);
				num2 = 0;
				count = 0;
				for (int i = 0; i < 100; i++) {
					num[i] = NULL;
				}
				break;
			}
			break;

		case IDC_BUTTON_MINUS:
			if (funcCount == 0) {
				GetDlgItemText(hDlg, IDC_EDIT, num, 100);
				num1 = std::stof(num);

				for (int i = 0; i < 100; i++) {
					num[i] = NULL;
				}

				SetDlgItemText(hDlg, IDC_EDIT, _T(""));
				point = FALSE;
				count = 0;
				func = 'M';
				funcCount++;

			}
			else {
				GetDlgItemText(hDlg, IDC_EDIT, num, 100);
				num2 = std::stof(num);

				result = num1 - num2;
				resultStr = std::to_wstring(result);
				resultCStr = resultStr.c_str();
				num1 = result;
				result = 0;
				SetDlgItemText(hDlg, IDC_EDIT, resultCStr);
				num2 = 0;
				count = 0;
				for (int i = 0; i < 100; i++) {
					num[i] = NULL;
				}
				break;
			}
			break;

			

		case IDC_BUTTON_MUL:

			if (funcCount == 0) {
				GetDlgItemText(hDlg, IDC_EDIT, num, 100);
				num1 = std::stof(num);

				for (int i = 0; i < 100; i++) {
					num[i] = NULL;
				}

				SetDlgItemText(hDlg, IDC_EDIT, _T(""));
				point = FALSE;
				count = 0;
				func = 'K';
				funcCount++;

			}
			else {
				GetDlgItemText(hDlg, IDC_EDIT, num, 100);
				num2 = std::stof(num);

				result = num1 * num2;
				resultStr = std::to_wstring(result);
				resultCStr = resultStr.c_str();
				num1 = result;
				result = 0;
				SetDlgItemText(hDlg, IDC_EDIT, resultCStr);
				num2 = 0;
				count = 0;
				for (int i = 0; i < 100; i++) {
					num[i] = NULL;
				}
				break;
			}

		
			break;


		case IDC_BUTTON_DIV:


			if (funcCount == 0) {
				GetDlgItemText(hDlg, IDC_EDIT, num, 100);
				num1 = std::stof(num);

				for (int i = 0; i < 100; i++) {
					num[i] = NULL;
				}

				SetDlgItemText(hDlg, IDC_EDIT, _T(""));
				point = FALSE;
				count = 0;
				func = 'D';
				funcCount++;

			}
			else {
				GetDlgItemText(hDlg, IDC_EDIT, num, 100);
				num2 = std::stof(num);

				result = num1 / num2;
				resultStr = std::to_wstring(result);
				resultCStr = resultStr.c_str();
				num1 = result;
				result = 0;
				SetDlgItemText(hDlg, IDC_EDIT, resultCStr);
				num2 = 0;
				count = 0;
				for (int i = 0; i < 100; i++) {
					num[i] = NULL;
				}
				break;
			}

		
			break;


		case IDC_BUTTON_EQU:
			GetDlgItemText(hDlg, IDC_EDIT, num, 100);
			num2 = std::stof(num);

			switch (func) {
			case 'P':
				result = num1 + num2;
				break;
			case 'M':
				result = num1 - num2;
				break;
			case 'K':
				result = num1 * num2;
				break;
			case 'D':
				if (num2 != 0)  // 분모가 0이면 나누지 않도록 수정
					result = num1 / num2;
				break;
			}

			resultStr = std::to_wstring(result);
			resultCStr = resultStr.c_str();

			SetDlgItemText(hDlg, IDC_EDIT, resultCStr);

			for (int i = 0; i < 100; i++) {
				num[i] = NULL;
			}


			num1 = result;  // 계산 결과를 num1에 저장
			funcCount = 0; 
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
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Calcu);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}