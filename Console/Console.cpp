﻿// Console.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

//필요한 헤더 파일 참조
#include "pch.h"
#include "framework.h"
#include "Console.h"

//전처리기 매크로 정의를 확인 후 컴파일
// _DEBUG가 있다면 new DEBUG_NEW로 재정의 의미
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 애플리케이션 개체입니다.
// 전용 객체
CWinApp theApp;

using namespace std;

//프로그램의 실행 시작점
int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
        //AfcWinInit - 초기화함수
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 여기에 애플리케이션 동작을 코딩합니다.
            wprintf(L"심각한 오류: MFC 초기화 실패\n");
            nRetCode = 1;
        }
        else
        {

            //-----------------------------LoadString 연습 ----------------------   
#if 0
            // TODO: 여기에 애플리케이션 동작을 코딩합니다.
            //CString 객체 하나 만들고 LoadString 함수 호출하여 정의된 문자열 로드
            //LoadString - Resource에서 ID 값으로 캡션 데이터 호출
            CString str;
            str.LoadString(IDS_APP_TITLE);
            //_tprintf는 printf의 매크로 버전으로 유니코드 지원, _T는 전달된 문자열을 프로젝트 설정에 따라
            // ANSI 또는 유니코드 방식으로 정의하게 해주는 메크로
            _tprintf(_T("Hello from %s!\n"), (LPCTSTR)str);
            //한글 출력 위해선 tsetlocale 필수
            _tsetlocale(LC_ALL, _T(""));
            _tprintf(_T("한국어 출력합니다\n"));
#endif

            //-----------------------------CString 객체 생성과 초기화 연습 ----------------------        
#if 0
            CString str1;
            str1 = _T("안녕하세요."); // 문자열을 직접 대입한다.
            CString str2(_T("오늘은")); // 문자열을 생성자 인자로 전달한다.
            CString str3(str2); // CString 객체를 생성자 인자로 전달한다.
            // CString 객체와 문자열을 붙인다.
            CString str4 = str1 + _T(" ") + str2 + _T(" 즐거운 날입니다.");
            _tprintf(_T("%s\n"), (LPCTSTR)str1);
            _tprintf(_T("%s\n"), (LPCTSTR)str2);
            _tprintf(_T("%s\n"), (LPCTSTR)str3);
            _tprintf(_T("%s\n"), (LPCTSTR)str4);
            // += 연산자를 이용하여 기존 문자열에 새로운 문자열을 덧붙인다.
            str4 += _T(" 하하하");
            _tprintf(_T("%s\n"), (LPCTSTR)str4);

            getchar();
#endif 

            //-----------------------------CString::Format() 함수 연습 ----------------------
#if 0
            CString str;
            str.Format(_T("x=%d, y=%d"), 100, 200);
            MessageBox(NULL, str, _T("CString::Format() 연습"), MB_OK);
            str.LoadString(IDS_TEST); // 문자열 리소스를 로드한다.
            str.Insert(0, _T("Hello from ")); // 맨 앞에 문자열을 삽입한다.
            str.Append(_T("!")); // 맨 끝에 문자열을 덧붙인다.
            MessageBox(NULL, str, _T("CString::LoadString() 연습"), MB_OK);
#endif               

            //-----------------------------(CPoint 객체 생성 연습) ---------------------------
#if 0   
            CPoint pt1(10, 20); // x, y 좌표를 생성자 인자로 전달한다.
            POINT pt = { 30, 40 };
            CPoint pt2(pt); // POINT 타입 변수를 생성자 인자로 전달한다.
            _tprintf(_T("%d, %d\n"), pt1.x, pt1.y);
            _tprintf(_T("%d, %d\n"), pt2.x, pt2.y);
            pt1.Offset(40, 30); // x, y 좌표에 각각 40, 30을 더한다.
            pt2.Offset(20, 10); // x, y 좌표에 각각 20, 10을 더한다.
            _tprintf(_T("%d, %d\n"), pt1.x, pt1.y);
            _tprintf(_T("%d, %d\n"), pt2.x, pt2.y);
            if (pt1 == pt2) // 두 객체의 내용이 같은지 확인한다.
                _tprintf(_T("두 점의 좌표가 같습니다.\n"));
            else
                _tprintf(_T("두 점의 좌표가 다릅니다.\n"));
#endif
   
            //-----------------------------(CRect 객체 생성 연습) ---------------------------
#if 0
            _tsetlocale(LC_ALL, _T("")); // 유니코드 한국어 출력에 필요
            CRect rect1(0, 0, 200, 100); // 직사각형의 좌표를 생성자의 인자로 전달한다.
            CRect rect2;
            rect2.SetRect(0, 0, 200, 100); // 직사각형의 좌표를 실행 중에 설정한다.
            if (rect1 == rect2) // 두 객체의 내용이 같은지 확인한다.
                _tprintf(_T("두 직사각형의 좌표가 같습니다.\n"));
            else
                _tprintf(_T("두 직사각형의 좌표가 다릅니다.\n"));
            RECT rect = { 100, 100, 300, 200 };
            CRect rect3(rect); // RECT 타입 변수를 생성자 인자로 전달한다.
            _tprintf(_T("%d, %d\n"), rect3.Width(), rect3.Height());

            CPoint pt(200, 150);
            if (rect3.PtInRect(pt)) // 점이 직사각형 내부에 있는지 판단한다.
                _tprintf(_T("점이 직사각형 내부에 있습니다.\n"));
            else
                _tprintf(_T("점이 직사각형 외부에 있습니다.\n"));
            getchar();
#endif
            //-----------------------------(CSize 객체 생성 연습) ---------------------------
#if 0
            _tsetlocale(LC_ALL, _T("")); // 유니코드 한국어 출력에 필요
            CSize size1(100, 200); // 폭과 높이를 생성자 인자로 전달한다.
            SIZE size = { 100, 200 };
            CSize size2(size); // SIZE 타입 변수를 생성자 인자로 전달한다.
            _tprintf(_T("%d, %d\n"), size2.cx, size2.cy);
            if (size1 == size2) // 두 객체의 내용이 같은지 확인한다.
                _tprintf(_T("크기가 같습니다.\n"));
            else
                _tprintf(_T("크기가 다릅니다.\n"));
#endif 
            //-----------------------------(CTime 객체 생성 연습) ---------------------------
#if 0
            _tsetlocale(LC_ALL, _T("")); // 유니코드 한국어 출력에 필요
            // CTime::GetCurrentTime() 함수로 현재 시각을 구한다.
            CTime tm;
            tm = CTime::GetCurrentTime();
            // 여러 형식으로 화면에 출력한다.
            CString str = tm.Format(_T("%A, %B %d, %Y"));
            _tprintf(_T("%s\n"), (LPCTSTR)str);
            str.Format(_T("현재 시각은 %d시 %d분 %d초입니다."),
                tm.GetHour(), tm.GetMinute(), tm.GetSecond());
            _tprintf(_T("%s\n"), (LPCTSTR)str);
#endif
            //-----------------------------(CTimeSpan 객체 생성 연습) ---------------------------
#if 1
            _tsetlocale(LC_ALL, _T("")); // 유니코드 한국어 출력에 필요
            CTime startTime = CTime::GetCurrentTime();
            Sleep(2000); // 2000 밀리초 지연
            CTime endTime = CTime::GetCurrentTime();
            CTimeSpan elapsedTime = endTime - startTime;
            CString str;
            str.Format(_T("%d초 지남!"), elapsedTime.GetTotalSeconds());
            _tprintf(_T("%s\n"), (LPCTSTR)str);


            CTimeSpan span(2, 5, 30, 15);  // 2일 5시간 30분 15초

            int days = span.GetDays();  // 2
            int hours = span.GetHours();  // 5
            int minutes = span.GetMinutes();  // 30
            int seconds = span.GetSeconds();  // 15
            double totalDays = span.GetTotalMinutes();  // 2.229641
            double totalHours = span.GetTotalHours();  // 53.495833
            // 그 외의 함수들도 비슷하게 사용 가능

            CString result;
            result.Format(_T("%d days, %d hours, %d minutes, %d seconds, %lf totaldays, %lf totalHours"), days, hours, minutes, seconds, totalDays, totalHours);

            // 출력
            _tprintf(_T("Time Span: %s\n"),result);


#endif // 0




        }
    }
    else
    {
        // TODO: 오류 코드를 필요에 따라 수정합니다.
        wprintf(L"심각한 오류: GetModuleHandle 실패\n");
        nRetCode = 1;
    }

    return nRetCode;
}
