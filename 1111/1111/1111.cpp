// 1111.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1111.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1111, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1111));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1111));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1111);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

HWND g_you;
HWND g_note;
HWND g_child;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// 시작과 동시에 상대를 찾는다.
        g_you = FindWindow(NULL, L"child");
        /// 시작과 동시에 메모장을 찾는다.
        g_note = FindWindow(NULL, L"제목 없음 - Windows 메모장");
        /// 메모장의 자식 윈도우 중 "Edit" 클래스를 찾는다.
        g_child = FindWindowEx(g_note, NULL, L"Edit", NULL);
    }
        break;

    case WM_RBUTTONDOWN:
    {
        if (NULL == g_you)
            break;
        PostMessage(g_you, WM_USER + 3, 333, 777);
    }
        break;

    case WM_KEYDOWN:
    {
        SendMessage(g_child, WM_CHAR, wParam, 0);
    }
        break;
    case WM_COPYDATA:
    {
        
    }
        break;
    case WM_LBUTTONDOWN:
    {
        /*
        if (NULL == g_you)
            break;
        /// 동기 통신
        //SendMessage(you, WM_LBUTTONDOWN, wParam, lParam);
        /// 비동기 통신
        PostMessage(g_you, WM_LBUTTONDOWN, wParam, lParam);
        //MessageBox(hWnd, L"전송 완료", L"보냈음", MB_OK);
        */

        /// VM을 피해 문자열 전송
        /*
        wchar_t buf[] = L"ㅁ란여ㅗㅓㅏㄻ호러ㅏㅁ녀ㅗㄻ녀ㅛ랴ㅐ면ㅅ7ㄹ8ㅎㅈ34ㄴ러ㅑㄴㅇㄹ";
        COPYDATASTRUCT  cds;
        cds.cbData = lstrlenW(buf);
        cds.dwData = lstrlenW(buf);
        cds.lpData = buf;
        PostMessage(g_you, WM_COPYDATA, (WPARAM)&cds, 0);
        */

        /// 메모장으로 키보드 입력 전달
        SendMessage(g_child, WM_CHAR, 0x65, 0);
        /*
        if (NULL != you)
        {
            MessageBox(hWnd, L"찾았다.", L"asdf", MB_OK);
        }
        */
        
    }
        break;

    case WM_USER:
    case WM_USER+1:     // 메뉴에서 선그리기 선택했다.
        break;
    case WM_USER+2:     // 메뉴에서 사각형 그리기 선택.
        break;
    case WM_USER+3:     // 메뉴에서 타원 그리기 선택.
        break;
    /// ...
    case WM_USER+255:
    {
        /*
        * 보낼 정보 : 그리기 종류 (0-선, 1-사각, 2-타원, 3-자유)
        * 선 굵기 : 0, 10, 5, 3
        * 색 정보 : (R, G, B), (R, G, B), (R, G, B), ...
        */
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
