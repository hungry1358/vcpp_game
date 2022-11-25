// 09_22.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "201907065.h"

#include <stdlib.h>     // 랜덤 함수 사용
#include <time.h>       // 랜덤 시드 값을 변경

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
    LoadStringW(hInstance, IDC_MY201907065, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY201907065));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY201907065));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY201907065);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

// 내 캐릭터
RECT g_me;
// 상대 캐릭터
RECT g_you;
// 그라운드
RECT g_ground;
// 나무 아이템
RECT g_woodItem;
// 돌 아이템
RECT g_stoneItem;
// 돌도끼
RECT g_stoneAxe;
// 게임 시간
int g_gametime;
// 게임 시간 표시 사각형
RECT g_game;
//나무 자원 갯수
int g_wood = 0;
//돌 자원 갯수 
int g_stone = 0;
//플레이어의 체력
int g_playerHP = 100;
//적의 체력
int g_enemyHP = 100;
//플레이어의 핸드 : 0 없음, 1 몽둥이, 2 돌도끼, 3 새총, 4 활과 화살
int g_hand = 0;
//플레이어 스레드의 핸들
HANDLE g_hPlayer;
// 치트키
bool g_c = false;
// 게임 진행 상태
bool g_status = true;
//타이머 설정값
int g_timer;
// 스레드를 위한 hWnd
HWND g_hWnd;
//경고메시지
int g_waring = 0;
//적 스레드
DWORD WINAPI Enemy(LPVOID param) {

    RECT is;
    HDC hdc;

    hdc = GetDC(g_hWnd);

    while (IntersectRect(&is, &g_me, &g_you) == false)
    {
        if (g_me.left < g_you.left)
        {
            g_you.left -= 10;
            g_you.right -= 10;
        }
        else
        {
            g_you.left += 10;
            g_you.right += 10;
        }

        if (g_me.top < g_you.top)
        {
            g_you.top -= 10;
            g_you.bottom -= 10;
        }
        else
        {
            g_you.top += 10;
            g_you.bottom += 10;
        }

        InvalidateRect(g_hWnd, NULL, TRUE);
        Sleep(300);
    }

    ReleaseDC(g_hWnd, hdc);

    ExitThread(0);
    return 0;
}

//핸드 스레드
DWORD WINAPI Hand(LPVOID param) {

    HDC hdc;
    hdc = GetDC(g_hWnd);

    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}

//돌도끼 스레드 
DWORD WINAPI StoneAxe(LPVOID param) {

    HDC hdc;
    hdc = GetDC(g_hWnd);
    
    if (g_me.right != g_stoneAxe.left) {
        g_stoneAxe.left = g_me.right;
        g_stoneAxe.right = g_stoneAxe.left + 10;
    }
    if (g_me.top - 50 != g_stoneAxe.top) {
        g_stoneAxe.top = g_me.top - 50;
        g_stoneAxe.bottom = g_stoneAxe.top + 100;
    }
    
    InvalidateRect(g_hWnd, NULL, TRUE);
    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}

DWORD WINAPI Waring(LPVOID param) {
    
    int i;
    HDC hdc;
    hdc = GetDC(g_hWnd);
    WCHAR buf[128] = { 0, };

    switch (g_waring) {
    case 1:
        for (i = 0; i < 300; i++) {
            wsprintf(buf, L"자원이 부족합니다");
            TextOut(hdc, 1200, 200, buf, lstrlen(buf));
            Sleep(1);
        }
        break;
    }
    

    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}

//플레이어 스레드
DWORD WINAPI Player(LPVOID param) {

    RECT is;
    HDC hdc;

    hdc = GetDC(g_hWnd);

    switch ((WPARAM)param) {
    case 0x41:
        g_me.left -= 10;
        g_me.right -= 10;
        break;
    case 0x44:
        g_me.left += 10;
        g_me.right += 10;
        break;
    case 0x57:
        g_me.top -= 10;
        g_me.bottom -= 10;
        break;
    case 0x53:
        g_me.top += 10;
        g_me.bottom += 10;
        break;
    }

    if (10 > g_me.left)
    {
        g_me.left = 10;
        g_me.right = 70;
    }
    if (10 > g_me.top)
    {
        g_me.top = 10;
        g_me.bottom = 110;
    }
    if (1400 < g_me.right)
    {
        g_me.right = 1400;
        g_me.left = 1330;
    }
    if (650 < g_me.bottom)
    {
        g_me.bottom = 650;
        g_me.top = 550;
    }

    InvalidateRect(g_hWnd, NULL, TRUE);

    ReleaseDC(g_hWnd, hdc);

    ExitThread(0);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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

    case WM_LBUTTONDOWN:
    {
        
        /*
        int x, y;
        RECT is, tmp;
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        tmp.left = x;
        tmp.top = y;
        tmp.right = x + 1;
        tmp.bottom = y + 1;

        if (true == IntersectRect(&is, &tmp, &g_me))
        {
            //MessageBox(hWnd, L"눌렀다!", L"ok", MB_OK);
        }
        */
    }
    break;

    case WM_LBUTTONUP:
    {
        /*
        int x, y;
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        g_me.left = x;
        g_me.top = y;
        g_me.right = x + 60;
        g_me.bottom = y + 100;

        InvalidateRect(hWnd, NULL, TRUE);
        */
    }
    break;

    /// 키를 입력하면 전달되는 메시지
/*    case WM_KEYDOWN:
    {
        MessageBox(hWnd, L"키가 눌렸어요", L"54rtg", MB_OK);
    }

    break;
*/
    case WM_KEYDOWN:
    {

        RECT is;
        
        if (false == g_status)
            break;

        g_hPlayer = CreateThread(NULL, 0, Player, (LPVOID)wParam, 0, NULL);
        CreateThread(NULL, 0, StoneAxe, (LPVOID)lParam, 0, NULL);
        // 아이템 획득 확인
        switch (wParam)
        {
        case VK_SPACE:
            // 나무랑 플레이어랑 곂치면 나무 자원 랜덤위치에 생성
            if (true == IntersectRect(&is, &g_me, &g_woodItem))
            {
                g_woodItem.left = 10 + (rand() % 1330);
                g_woodItem.top = 10 + (rand() % 520);
                g_woodItem.right = g_woodItem.left + 70;
                g_woodItem.bottom = g_woodItem.top + 130;
                g_wood += 1;

                //플레이어 스레드 일시정지
                SuspendThread(g_hPlayer);
                Sleep(1000);
                ResumeThread(g_hPlayer);
            }
            if (true == IntersectRect(&is, &g_me, &g_stoneItem)) {
                g_stoneItem.left = 10 + (rand() % 1320);
                g_stoneItem.top = 10 + (rand() % 570);
                g_stoneItem.right = g_stoneItem.left + 70;
                g_stoneItem.bottom = g_stoneItem.top + 70;
                g_stone += 1;

                SuspendThread(g_hPlayer);
                Sleep(1000);
                ResumeThread(g_hPlayer);
            }
            break;
        case 0x31:
            if (g_wood > 1) {
                g_hand = 1;
            }
            else {
                g_waring = 1;
                CreateThread(NULL, 0, Waring, (LPVOID)lParam, 0, NULL);
            }
            break;
        }
        /*
    case VK_LEFT:
        g_me.left -= 10;
        g_me.right -= 10;
        break;
    case VK_RIGHT:
        g_me.left += 10;
        g_me.right += 10;
        break;
    case VK_UP:
        g_me.top -= 10;
        g_me.bottom -= 10;
        break;
    case VK_DOWN:
        g_me.top += 10;
        g_me.bottom += 10;
        break;

    }

    // 계산된 좌표가 그라운드를 벗어나는지 확인

    if (10 > g_me.left)
    {
        g_me.left = 10;
        g_me.right = 70;
    }
    if (10 > g_me.top)
    {
        g_me.top = 10;
        g_me.bottom = 110;
    }
    if (1700 < g_me.right)
    {
        g_me.right = 1700;
        g_me.left = 1630;
    }
    if (950 < g_me.bottom)
    {
        g_me.bottom = 950;
        g_me.top = 850;
    }
    */

        InvalidateRect(hWnd, NULL, true);
    }
    break;

    case WM_TIMER:
    {
        /*
        if (1 == wParam)
        {
            RECT is;

            if (g_timer >= 200)
                g_timer -= 100;
            KillTimer(hWnd, 1);
            SetTimer(hWnd, 1, g_timer, NULL);

            if (g_me.left < g_you.left)
            {
                g_you.left -= 10;
                g_you.right -= 10;
            }
            else
            {
                g_you.left += 10;
                g_you.right += 10;
            }
            if (g_me.top < g_you.top)
            {
                g_you.top -= 10;
                g_you.bottom -= 10;
            }
            else
            {
                g_you.top += 10;
                g_you.bottom += 10;
            }
            if (true == IntersectRect(&is, &g_me, &g_you))
            {
                KillTimer(hWnd, 1);
                MessageBox(hWnd, L"잡혔습니다.", L"KIM윤재", MB_OK);
            }



        }
        else if (2 == wParam)
        {
            g_gametime--;   // 게임 시간 감소
            if (0 >= g_gametime)
            {
                g_status = false;
                g_gametime = 0;
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);
            }
            g_game.right = 10 + (g_gametime * 10);
        }
        InvalidateRect(hWnd, NULL, true);*/
    }
    break;
    case WM_CREATE:
    {
        g_hWnd = hWnd;

        //CreateThread(NULL, 0, Enemy, (LPVOID)wParam, 0, NULL);

        // 랜덤 시드 값 초기화
        srand(time(NULL));

        // 게임 시간 초기화
        g_gametime = 10;

        g_timer = 1000;
        SetTimer(hWnd, 1, g_timer, NULL);
        SetTimer(hWnd, 2, 1000, NULL);

        //플레이어
        g_me.left = 620;
        g_me.top = 225;
        g_me.right = 680;
        g_me.bottom = 325;

        //적
        g_you.left = 300;
        g_you.top = 300;
        g_you.right = 400;
        g_you.bottom = 400;

        //그라운드
        g_ground.left = 10;
        g_ground.top = 10;
        g_ground.right = 1400;
        g_ground.bottom = 650;

        //나무
        g_woodItem.left = 10 + (rand() % 1320);
        g_woodItem.top = 10 + (rand() % 510);
        g_woodItem.right = g_woodItem.left + 70;
        g_woodItem.bottom = g_woodItem.top + 130;

        //돌
        g_stoneItem.left = 10 + (rand() % 1320);
        g_stoneItem.top = 10 + (rand() % 570);
        g_stoneItem.right = g_stoneItem.left + 70;
        g_stoneItem.bottom = g_stoneItem.top + 70;

        //돌도끼

        g_game.left = 10;
        g_game.top = 520;
        g_game.right = g_gametime * 10;
        g_game.bottom = 560;


    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        WCHAR buf[128] = { 0, };
        //무슨 자원인지 표시해주는 문자열
        WCHAR woodText[32] = { 0, };    
        WCHAR stoneText[32] = { 0, };   

        // 초록색 그라운드를 만들기 위한 브러시
        //HBRUSH ground_b, groundOs_b;
        //나무를 만들기 위한 브러시
        //HBRUSH wood_b, woodOs_b;
        //나를 만들기 위한 브러시
        //HBRUSH me_b, meOs_b;

        //브러시 생성
        //ground_b = CreateSolidBrush(RGB(0, 150, 0));
        //wood_b = CreateSolidBrush(RGB(150, 75, 0));
        //me_b = CreateSolidBrush(RGB(0, 0, 0));

        //그라운드
        //groundOs_b = (HBRUSH)SelectObject(hdc, ground_b);
        Rectangle(hdc, g_ground.left, g_ground.top, g_ground.right, g_ground.bottom);

        //상대
        Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);

        // 나무
        //woodOs_b = (HBRUSH)SelectObject(hdc, wood_b);
        Rectangle(hdc, g_woodItem.left, g_woodItem.top, g_woodItem.right, g_woodItem.bottom);
        wsprintf(woodText, L"나무");
        TextOut(hdc, g_woodItem.left+15, g_woodItem.top+30, woodText, lstrlen(woodText));

        // 돌
        Ellipse(hdc, g_stoneItem.left, g_stoneItem.top, g_stoneItem.right, g_stoneItem.bottom);
        wsprintf(stoneText, L"돌");
        TextOut(hdc, g_stoneItem.left + 25, g_stoneItem.top + 25, stoneText, lstrlen(stoneText));

        // 내 캐릭터
        //meOs_b = (HBRUSH)SelectObject(hdc, me_b);
        Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);

        //돌도끼
        if (g_wood > 2 && g_stone > 1)
            Rectangle(hdc, g_stoneAxe.left, g_stoneAxe.top, g_stoneAxe.right, g_stoneAxe.bottom);

        // 상대 캐릭터
        //Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);

        // 게임 시간 표시
        //Rectangle(hdc, g_game.left, g_game.top, g_game.right, g_game.bottom);
        //EndPaint(hWnd, &ps);

        //브러시 교체
        //SelectObject(hdc, groundOs_b);
        //SelectObject(hdc, woodOs_b);
        //SelectObject(hdc, meOs_b);
        //브러시 해제
        //DeleteObject(ground_b);
        //DeleteObject(wood_b);
        //DeleteObject(me_b);

        

        wsprintf(buf, L"나무[ %d ]", g_wood);
        TextOut(hdc, 1330, 50, buf, lstrlen(buf));
        wsprintf(buf, L"돌[ %d ]", g_stone);
        TextOut(hdc, 1330, 70, buf, lstrlen(buf));
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
