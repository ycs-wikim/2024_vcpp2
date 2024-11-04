// 1028.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1028.h"

//#include "new.h"

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

HANDLE g_process;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    /// OS 영역에서 Mutex 객체 획득을 이름으로 시도
    HANDLE h = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"asdf");
    if (NULL == h)
    {
        /// 그런 이름을 갖는 Mutex는 존재하지 않는다.
        ///MessageBox(NULL, L"이름이 없다.", L"누구냐 넌", MB_OK);
        /// OS 레벨에서 확인 가능한 이름을 등록
        g_process = CreateMutex(NULL, TRUE, L"asdf");
    }
    else
    {
        /// 동일한 이름을 갖는 Mutex가 존재한다.
        ///MessageBox(NULL, L"무슨 일이죠", L"어떻게 오셨나요", MB_OK);
        /// main 함수에서 return 0 ==> 정상 종료
        return 0;
    }

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1028, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1028));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1028));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1028);
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

HWND g_hWnd;
/// 1. 공유 자원
int g_x;

//////////////////////////////////////////////////////////////
// SW적인 해결 방법을 위한 또 다른 공유 자원
//////////////////////////////////////////////////////////////
BOOL g_use = FALSE;

DWORD WINAPI threadfunction(LPVOID param)
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);

    HDC hdc = GetDC(g_hWnd);

    for (i = 0; i < y; i++)
    {
        while (g_use) {};
        /// context switching 발생하면 g_use 값을 여러 스레드가 FALSE로 보게된다.
        g_use = TRUE;
        ///////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        g_use = FALSE;
        ///////////////////////////// 임계 영역 종료
        Sleep(30);
    }

    ReleaseDC(g_hWnd, hdc);

    /// 스레드의 동작을 종료하도록 OS에 요청 ==> 스레드가 생성한 모든 자원을 회수
    ExitThread(0);
    return 0;
}

//////////////////////////////////////////////////////////////
// SW적인 해결 방법을 위한 또 다른 공유 자원
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// OS에서 제공하는 방법 - Mutex
//////////////////////////////////////////////////////////////
/// 1. 자료 구조
HANDLE g_mux;

DWORD WINAPI thread_mux(LPVOID param)
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);

    HDC hdc = GetDC(g_hWnd);

    for (i = 0; i < y; i++)
    {
        /// 3. 임계영역 진입
        WaitForSingleObject(g_mux, INFINITE);

        ///////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        /// 4. 임계영역 해제
        ReleaseMutex(g_mux);

        ///////////////////////////// 임계 영역 종료
        Sleep(30);          /// Context Switching
    }

    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}
//////////////////////////////////////////////////////////////
// OS에서 제공하는 방법 - Mutex
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// OS에서 제공하는 방법 - Semaphore
//////////////////////////////////////////////////////////////
/// 1. 자료 구조
HANDLE g_sem;

DWORD WINAPI thread_sem(LPVOID param)
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);

    HDC hdc = GetDC(g_hWnd);

    /// 3. 임계영역 진입
    WaitForSingleObject(g_sem, INFINITE);
    for (i = 0; i < y; i++)
    {
        ///////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        ///////////////////////////// 임계 영역 종료
        Sleep(30);          /// Context Switching
    }
    /// 4. 임계영역 해제
    ReleaseSemaphore(g_sem, 1, NULL);

    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}
//////////////////////////////////////////////////////////////
// OS에서 제공하는 방법 - Semaphore
//////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////
// OS에서 제공하는 방법 - Critical Section
//////////////////////////////////////////////////////////////
/// 1. 자료 구조
CRITICAL_SECTION g_cs;

DWORD WINAPI thread_cs(LPVOID param)
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);

    HDC hdc = GetDC(g_hWnd);

    for (i = 0; i < y; i++)
    {
        /// 3. 임계영역 진입
        EnterCriticalSection(&g_cs);
        ///////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        /// 4. 임계영역 해제
        LeaveCriticalSection(&g_cs);
        ///////////////////////////// 임계 영역 종료
        Sleep(30);          /// Context Switching
    }

    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}
//////////////////////////////////////////////////////////////
// OS에서 제공하는 방법 - Critical Section
//////////////////////////////////////////////////////////////



class asdf
{
public:
    asdf();
    void asdflajsfkldjalsfdj(){}
};

asdf* ptr;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        ptr = new asdf();
        /// 2. Critical Section 객체 생성
        InitializeCriticalSection(&g_cs);
        /// 2. Semaphore 객체 생성
        /// 보안 속성 : 지정하지 않으면 프로세스의 권한을 가짐
        /// 초기 카운트 : 최초에 사용할 수 있는 자원의 개수
        /// 최대 카운트 : 전체 자원의 개수
        /// 이름 : 지역(내 프로세스 내부)적으로 사용되는 이름
        g_sem = CreateSemaphore(NULL, 3, 3, NULL);
        /// 2. Mutex 객체 생성
        /// 보안 속성 : 지정하지 않으면 프로세스의 권한을 가짐
        /// 초기 소유자 : 누군가 자원을 사용하고 있는가?
        /// 이름 : 전역(OS)적으로 사용되는 이름
        g_mux = CreateMutex(NULL, FALSE, NULL);
    }
        break;
    case WM_LBUTTONDOWN:
    {
        ptr->asdflajsfkldjalsfdj();
        g_hWnd = hWnd;
        CreateThread(NULL, 0, thread_mux, (LPVOID)lParam, 0, NULL);
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
        /// 5. Critical Section 삭제
        DeleteCriticalSection(&g_cs);
        /// 5. Semaphore 삭제
        CloseHandle(g_sem);
        /// 5. Mutex 삭제
        CloseHandle(g_mux);
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
