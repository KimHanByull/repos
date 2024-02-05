#include <windows.h>
#include "resource.h";
#include <cstdio>
#include <tchar.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASS wndclass;
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
	hwnd = CreateWindow(TEXT("HelloClass"), TEXT("김한별의 네 번째 윈도우"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, SW_MAXIMIZE);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;



}

#include <math.h>
#define BSIZE 200

struct OBJ
{
	char Shape = NULL;
	int fx = 0, fy = 0;
	int lx = 0, ly = 0;
	COLORREF penColor = RGB(0, 0, 0);
	COLORREF brushColor = RGB(0, 0, 0);

};

void OutFromFile(TCHAR filename[], HWND hwnd) {
	FILE* fPtr;
	HDC hdc;
	int line;
	TCHAR buffer[500];
	line = 0;
	hdc = GetDC(hwnd);
#ifdef _UNICODE
	_tfopen_s(&fPtr, filename, TEXT("r, css = UNICODE"));
#else
	_tfopen_s(&fPtr, filename, TEXT("r"));
#endif 
	while (_fgetts(buffer, 1000, fPtr) != NULL) {
		if (buffer[_tcslen(buffer)-1] == TEXT('\n'))
			buffer[_tcslen(buffer) - 1] = NULL;
		TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
		line++;
	}
	fclose(fPtr);
	ReleaseDC(hwnd, hdc);
}

BOOL InRectangle(OBJ object, int x, int y)
{
	if (object.fx <= x && object.fy <= y && object.lx >= x && object.ly >= y)
	{
		return true;
	}
	else return false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	static OBJ OBJList[100];
	OBJ tmpOBJ;		// 임시 객체
	static int counter; 

	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static int fx, fy; // 좌상단
	static int lx, ly; // 우하단
	static char currentShape; // L E R. 선택한 도형에 따라 타입을 저장
	CHOOSECOLOR COLOR;
	static COLORREF tmp[16], lineColor, brushColor;

	static bool selector;
	static int currentSelect;

	OPENFILENAME OFN, SFN;
	TCHAR str[100], lpstrFile[100] = TEXT("");
	TCHAR filter[] = TEXT("Every File(*.*) \0*.*\Text File\0*.txt;*.doc\0");


	switch (iMsg)
	{
	case WM_CREATE:
		selector = false;
		currentSelect = -1;
		counter = 0;
		currentShape = 'L';
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < counter; i++)
		{
			hBrush = CreateSolidBrush(OBJList[i].brushColor);
			hPen = CreatePen(PS_SOLID, 1, OBJList[i].penColor);
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			oldPen = (HPEN)SelectObject(hdc, hPen);
			switch (OBJList[i].Shape)
			{
			case 'L':
				MoveToEx(hdc, OBJList[i].fx, OBJList[i].fy, NULL);
				LineTo(hdc, OBJList[i].lx, OBJList[i].ly);
				break;
			case 'E':
				Ellipse(hdc, OBJList[i].fx, OBJList[i].fy, OBJList[i].lx, OBJList[i].ly);
				break;
			case 'R':
				Rectangle(hdc, OBJList[i].fx, OBJList[i].fy, OBJList[i].lx, OBJList[i].ly);
				break;
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
		}
		if (selector == true)
		{
			Rectangle(hdc, OBJList[currentSelect].fx - 5, OBJList[currentSelect].fy - 5, OBJList[currentSelect].fx, OBJList[currentSelect].fy);
			Rectangle(hdc, OBJList[currentSelect].lx, OBJList[currentSelect].fy - 5, OBJList[currentSelect].lx + 5, OBJList[currentSelect].fy);
			Rectangle(hdc, OBJList[currentSelect].fx - 5, OBJList[currentSelect].ly + 5, OBJList[currentSelect].fx, OBJList[currentSelect].ly);
			Rectangle(hdc, OBJList[currentSelect].lx, OBJList[currentSelect].ly, OBJList[currentSelect].lx + 5, OBJList[currentSelect].ly + 5);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case ID_FileOpen:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFilter = filter;
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 100;
			OFN.lpstrInitialDir = TEXT(".");
			if (GetOpenFileName(&OFN) != 0) {
				/*_stprintf_s(str, TEXT("%s 파일을 열겠습니까?"), OFN.lpstrFile);
				MessageBox(hwnd, str, TEXT("열기 선택"), MB_OK);*/
				OutFromFile(OFN.lpstrFile, hwnd);
			}
			break;


		case ID_FileSave:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hwnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = lpstrFile;
			SFN.nMaxFile = 256;
			SFN.lpstrInitialDir = TEXT(".");
			if (GetSaveFileName(&SFN) != 0) {
				_stprintf_s(str, TEXT("%s 파일을 저장하시겠습니까?"), SFN.lpstrFile);
				MessageBox(hwnd, str, TEXT("저장하기 선택"), MB_OK);
			}
			break;


		case ID_SELECT:
			if (selector == true)
			{
				selector = false;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else selector = true;
			break;
		
		case ID_Line:
			currentShape = 'L';
			break;
		case ID_Ellipse:
			currentShape = 'E';
			break;
		case ID_Rectangle:
			currentShape = 'R';
			break;
		case ID_LineColor:
			for (int i = 0; i < 16; i++)
			{
				tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hwnd;
			COLOR.lpCustColors = tmp;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0)
			{
				lineColor = COLOR.rgbResult;
				if (selector == true) OBJList[currentSelect].penColor = lineColor;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		case ID_FaceColor:
			for (int i = 0; i < 16; i++)
			{
				tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hwnd;
			COLOR.lpCustColors = tmp;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0)
			{
				brushColor = COLOR.rgbResult;
				if (selector == true) OBJList[currentSelect].brushColor = brushColor;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		
		}
		break;
	case WM_LBUTTONDOWN:
		fx = LOWORD(lParam);
		fy = HIWORD(lParam);
		if (selector == true)
		{
			for (int i = 0; i < counter; i++)
			{
				if (OBJList[i].Shape == 'R' && InRectangle(OBJList[i], fx, fy))
				{
					currentSelect = i;
					break;
				}
			}
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONUP:
		if (selector == false)
		{
			lx = LOWORD(lParam);
			ly = HIWORD(lParam);
			tmpOBJ = { currentShape, fx,fy,lx,ly, lineColor, brushColor };
			OBJList[counter] = tmpOBJ;
			counter++;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

