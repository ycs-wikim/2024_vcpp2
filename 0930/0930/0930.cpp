// 0930.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0930.h"
#include <stdlib.h>
#include <time.h>


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
    LoadStringW(hInstance, IDC_MY0930, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0930));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0930));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0930);
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

/// 0 - 선그리기, 1 - 사각형 그리기, 2 - 타원 그리기, 3 - 자유선
int g_drawType = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        srand(time(NULL));
        CreateWindow(L"button", L"PUSH", WS_VISIBLE | WS_CHILD, 10, 10, 200, 100, hWnd, (HMENU)3, hInst, NULL);
        break;
    case WM_RBUTTONDOWN:
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_LBUTTONDOWN:
    //case WM_MOUSEMOVE:
    {
        int x, y;
        HDC hdc;
        HPEN myPen;     /// 내가 생성할 객체
        HPEN osPen;     /// OS가 현재 사용하고 있는 객체
        HBRUSH myBrush, osBrush;

        x = LOWORD(lParam);
        y = HIWORD(lParam);

        /// 1. 펜 생성
        /// PS_SOLID - 실선, 다른 선도 적용 가능
        /// 5 - 굻기가 5px
        /// RGB( Red, Green, Blue ) - 값은 0 ~ 255
        myPen = CreatePen(PS_SOLID, 3, RGB(rand()%256, rand() % 256, rand() % 256));

        /// 1. 브러시 생성
        myBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        hdc = GetDC(hWnd);
        
        /// 2. 펜 객체 전달 및 현재 객체 반환
        osPen = (HPEN)SelectObject(hdc, myPen);
        /// 2. 브러시 객체 전달 및 현재 객체 반환
        osBrush = (HBRUSH)SelectObject(hdc, myBrush);

        /// 3. 사용
        if (0 == g_drawType)
        {
            MoveToEx(hdc, 0, 0, NULL);
            LineTo(hdc, x, y);
        }
        else if (1 == g_drawType)
        {
            Rectangle(hdc, 10, 10, x, y);
        }
        else if (2 == g_drawType)
        {
            Ellipse(hdc, 10, 10, x, y);
        }
        
        /// 사용 종료
        /// 4. OS의 펜 객체 전달 및 나의 객체 반환
        SelectObject(hdc, osPen);
        /// 4. OS의 브러시 객체 전달 및 나의 객체 반환
        SelectObject(hdc, osBrush);

        /// 5. 해제 : 생성한 펜 객체의 해제를 OS에 요청
        DeleteObject(myPen);
        /// 5. 해제 : 생성한 브러시 객체의 해제를 OS에 요청
        DeleteObject(myBrush);

        ReleaseDC(hWnd, hdc);
    }
        break;
    case WM_COMMAND:        /// 버튼 클릭시에도 발생
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_32771:
                g_drawType = 1;
                //MessageBox(hWnd, L"버튼", L"32771", MB_OK);
                break;
            case ID_32773:
                g_drawType = 2;
                //MessageBox(hWnd, L"버튼", L"32773", MB_OK);
                break;
            case 3:
                MessageBox(hWnd, L"버튼", L"동작", MB_OK);
                break;
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
