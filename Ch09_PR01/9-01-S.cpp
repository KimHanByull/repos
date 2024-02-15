#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WSADATA wsadata;
	SOCKET s;
	TCHAR message[300];
	SOCKADDR_IN addr = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(s, (LPSOCKADDR)&addr, sizeof(addr));
	if (listen(s, 5) == -1)
		return 0;
	do {
		SOCKADDR_IN c_addr;
		char buffer[100];
#ifdef _UNICODE
		TCHAR wbuffer[100];
#endif // _UNICODE
		int msgLen;
		int size = sizeof(c_addr);
		SOCKET cs = accept(s, (LPSOCKADDR)&c_addr, &size);
		msgLen = recv(cs, buffer, 100, 0);
		buffer[msgLen] = NULL;
#ifdef _UNICODE
		msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer),
			NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), wbuffer, msgLen);
		wbuffer[msgLen] = NULL;
		_stprintf_s(message, _T("클라이언트 메시지: %s, 서버를 종료하시겠습니까?"), wbuffer);

#else
		sprintf_s(message, _T("클라이언트 메시지 %s, 서버를 종료하시겠습니가?"), buffer);
#endif // _UNICODE
	} while (MessageBox(NULL, message, _T("Server 메시지"), MB_YESNO) == IDNO);
		closesocket(s);
		WSACleanup();
		return 1;

}