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
RECT enemy1;
//상대 캐릭터 생성
int g_enemyVisible = 0;
// 그라운드
RECT g_ground;
// 나무 아이템
RECT g_woodItem;
// 돌 아이템
RECT g_stoneItem;
// 풀 아이템
RECT g_grassItem;
// 방망이
RECT g_woodClub;
//돌도끼
RECT g_stoneAxe1, g_stoneAxe2;
//나무 자원 갯수
int g_wood = 0;
//돌 자원 갯수 
int g_stone = 0;
//풀 자원 갯수
int g_grass = 0;

//플레이어의 체력
int g_playerHP = 10;
//적의 체력
int g_enemyHP = 100;
//플레이어의 핸드 : 0 없음, 1 몽둥이, 2 돌도끼, 3 활
int g_hand = 0;
//공격력
int g_damage = 0;
//공격 범위
RECT g_attack;

//플레이어 스레드의 핸들
HANDLE g_hPlayer;
// 치트키
bool g_c = false;
// 적이 오기까지 남은 시간
int g_gametime;
// 게임 진행 상태
bool g_status = true;
//제작상태 
bool g_craft = false;
//화살
RECT g_arrow;
//타이머 설정값
int g_timer1, g_timer2;
// 스레드를 위한 hWnd
HWND g_hWnd;
//경고메시지
int g_waring = 0;


//적 스레드
DWORD WINAPI Enemy(LPVOID param) {
    RECT is;
    HDC hdc;
  
    hdc = GetDC(g_hWnd);

    enemy1.left = 5 + (rand() % 200);       //가로길이 100
    enemy1.top = 5 + (rand() % 120);        //세로길이 100
    enemy1.right = enemy1.left + 100;
    enemy1.bottom = enemy1.top + 100;
    Rectangle(hdc, enemy1.left, enemy1.top, enemy1.right, enemy1.bottom);

    while (true)
    {
        InvalidateRect(g_hWnd, NULL, TRUE);
        Sleep(300);
        if (g_enemyHP <= 0)
            ExitThread(0);
        else if (IntersectRect(&is, &g_me, &enemy1) == true) 
        {
            g_playerHP -= 10;
            Sleep(500);
        }

        if (g_me.left < enemy1.left)
        {
            enemy1.left -= 5;
            enemy1.right -= 5;
        }
        else
        {
            enemy1.left += 5;
            enemy1.right += 5;
        }

        if (g_me.top < enemy1.top)
        {
            enemy1.top -= 5;
            enemy1.bottom -= 5;
        }
        else
        {
            enemy1.top += 5;
            enemy1.bottom += 5;
        }
        
    }

    ReleaseDC(g_hWnd, hdc);

    ExitThread(0);
    return 0;
}

//핸드 스레드
DWORD WINAPI Hand(LPVOID param) {

    HDC hdc;
    hdc = GetDC(g_hWnd);
    
    switch (g_hand) {
    case 1:

        if (g_me.right != g_woodClub.left) {
            g_woodClub.left = g_me.right;
            g_woodClub.right = g_woodClub.left + 10;
        }
        if (g_me.top - 50 != g_woodClub.top) {
            g_woodClub.top = g_me.top - 50;
            g_woodClub.bottom = g_woodClub.top + 100;
        }
        InvalidateRect(g_hWnd, NULL, TRUE);
        break;
    case 2:
        if (g_me.right != g_stoneAxe1.left) {
            g_stoneAxe1.left = g_me.right;
            g_stoneAxe1.right = g_stoneAxe1.left + 10;
        }
        if (g_me.top - 50 != g_stoneAxe1.top) {
            g_stoneAxe1.top = g_me.top - 50;
            g_stoneAxe1.bottom = g_stoneAxe1.top + 100;
        }
        if (g_stoneAxe1.left != g_stoneAxe2.left) {
            g_stoneAxe2.left = g_stoneAxe1.left;
            g_stoneAxe2.right = g_stoneAxe2.left + 60;
        }
        if (g_stoneAxe1.top != g_stoneAxe2.top) {
            g_stoneAxe2.top = g_stoneAxe1.top;
            g_stoneAxe2.bottom = g_stoneAxe2.top + 40;
        }
        InvalidateRect(g_hWnd, NULL, TRUE);
        break;
    }
    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}

//화살 스레드
DWORD WINAPI Arrow(LPVOID param) {
    RECT is;
    HDC hdc;
    RECT arrow;
    hdc = GetDC(g_hWnd);
    switch ((WPARAM)param) {
    case VK_UP:
        arrow.left = g_me.left + 25;
        arrow.top = g_me.top - 100;
        arrow.right = arrow.left + 10;
        arrow.bottom = g_me.top;
        while (IntersectRect(&is, &enemy1, &arrow) == false) {
            if (IntersectRect(&is, &enemy1, &arrow) == true)
                ExitThread(0);
            arrow.top -= 10;
            arrow.bottom -= 10;
            Sleep(50);
            InvalidateRect(g_hWnd, NULL, TRUE);
        }
        break;
    case VK_DOWN:
        arrow.left = g_me.left + 25;
        arrow.top = g_me.bottom;
        arrow.right = arrow.left + 10;
        arrow.bottom = arrow.top + 100;
        while (IntersectRect(&is, &enemy1, &arrow) == false) {
            if (IntersectRect(&is, &enemy1, &arrow) == true)
                ExitThread(0);
            arrow.top += 10;
            arrow.bottom += 10;
            Sleep(50);
            InvalidateRect(g_hWnd, NULL, TRUE);
        }
        break;
    case VK_LEFT:
        arrow.left = g_me.left - 100;
        arrow.top = g_me.top + 45;
        arrow.right = g_me.left;
        arrow.bottom = arrow.top + 10;
        
            if (IntersectRect(&is, &enemy1, &arrow) == true)
                ExitThread(0);
            arrow.left -= 10;
            arrow.right -= 10;
            Sleep(50);
            InvalidateRect(g_hWnd, NULL, TRUE);
        
        break;
    case VK_RIGHT:
        arrow.left = g_me.right;
        arrow.top = g_me.top + 45;
        arrow.right = arrow.left + 100;
        arrow.bottom = arrow.top + 10;
        
            if (IntersectRect(&is, &enemy1, &arrow) == true)
                ExitThread(0);
            arrow.left += 10;
            arrow.right += 10;
            Sleep(50);
            InvalidateRect(g_hWnd, NULL, TRUE);
        
        break;
    }

    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}

//공격 스레드
DWORD WINAPI Attack(LPVOID param) {
    RECT is;
    HDC hdc;
    hdc = GetDC(g_hWnd);

    switch ((WPARAM)param) {
    case VK_UP:
        g_attack.left = g_me.left - 30;
        g_attack.top = g_me.top - 70;
        g_attack.right = g_me.right + 30;
        g_attack.bottom = g_me.top;

        if (IntersectRect(&is, &enemy1, &g_attack) == true)
            g_enemyHP -= g_damage;
        break;
    case VK_DOWN:
        g_attack.left = g_me.left - 30;
        g_attack.top = g_me.bottom;
        g_attack.right = g_me.right + 30;
        g_attack.bottom = g_me.bottom + 70;
        
        if (IntersectRect(&is, &enemy1, &g_attack) == true)
            g_enemyHP -= g_damage;
        break;
    case VK_LEFT:
        g_attack.left = g_me.left - 70;
        g_attack.top = g_me.top + 20;
        g_attack.right = g_me.left;
        g_attack.bottom = g_me.bottom - 20;

        if (IntersectRect(&is, &enemy1, &g_attack) == true)
            g_enemyHP -= g_damage;
        break;
    case VK_RIGHT:
        g_attack.left = g_me.right;
        g_attack.top = g_me.top + 20;
        g_attack.right = g_me.right + 70;
        g_attack.bottom = g_me.bottom - 20;

        if (IntersectRect(&is, &enemy1, &g_attack) == true)
            g_enemyHP -= g_damage;
        break;
    }

    ReleaseDC(g_hWnd, hdc);
    ExitThread(0);
    return 0;
}

//경고 메시지 스레드
DWORD WINAPI Waring(LPVOID param) {
    
    int i;
    HDC hdc;
    hdc = GetDC(g_hWnd);
    WCHAR buf[128] = { 0, };

    switch (g_waring) {
    case 1:
        for (i = 0; i < 100; i++) {
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
    case 0x41:      //A키
        g_me.left -= 10;
        g_me.right -= 10;
        break;
    case 0x44:      //D키
        g_me.left += 10;
        g_me.right += 10;
        break;
    case 0x57:      //W키
        g_me.top -= 10;
        g_me.bottom -= 10;
        break;
    case 0x53:      //S키
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

//게임오버 스레드
DWORD WINAPI Gameover(LPVOID param) {
    if (g_playerHP <= 0) {
        g_playerHP = 0;
        MessageBox(g_hWnd, L"죽었습니다", L"게임오버", MB_OK);
        SuspendThread(g_hPlayer);
    }
    ExitThread(0);
    return 0;
}

//DWORD WINAPI Enemy2(LPVOID prc) {
//    RECT is;
//    HDC hdc;
//    RECT rc = *(LPRECT)prc;
//    hdc = GetDC(g_hWnd);
//    
//    for (;;) {
//        Sleep(100);
//        Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
//        InvalidateRect(g_hWnd, NULL, TRUE);
//        if (IntersectRect(&is, &g_me, &rc) == true)
//            ExitThread(0);
//    }
//
//    ReleaseDC(g_hWnd, hdc);
//    return 0;
//}

//플레이어 일시정지 함수
void PlayerStop() {
    SuspendThread(g_hPlayer);
    Sleep(1000);
    ResumeThread(g_hPlayer);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //DWORD ThreadID;
    //int rect[] = {
    //    5 + (rand() % 200), 5 + (rand() % 120),
    //    5 + (rand() % 200), 5 + (rand() % 120),
    //    5 + (rand() % 200), 5 + (rand() % 120),
    //    5 + (rand() % 200), 5 + (rand() % 120)
    //};
    //static RECT arRect[] = {
    //    {rect[0], rect[1], rect[0] + 100, rect[1] + 100},
    //    {rect[2], rect[3], rect[2] + 100, rect[3] + 100},
    //    {rect[4], rect[5], rect[4] + 100, rect[5] + 100},
    //    {rect[6], rect[7], rect[6] + 100, rect[7] + 100}
    //};
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
        int x, y;
        RECT is, tmp;
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        tmp.left = x;
        tmp.top = y;
        tmp.right = x + 1;
        tmp.bottom = y + 1;

        
    }
    break;

    case WM_LBUTTONUP:
    {
        
        int x, y;
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        g_me.left = x;
        g_me.top = y;
        g_me.right = x + 60;
        g_me.bottom = y + 100;

        InvalidateRect(hWnd, NULL, TRUE);
        
    }
    break;

    case WM_KEYDOWN:
    {
        DWORD tid;
        RECT is;
        if (false == g_status)
            break;
        
        g_hPlayer = CreateThread(NULL, 0, Player, (LPVOID)wParam, 0, &tid);

        CreateThread(NULL, 0, Arrow, (LPVOID)wParam, 0, NULL);
        CreateThread(NULL, 0, Hand, (LPVOID)lParam, 0, NULL);
        CreateThread(NULL, 0, Attack, (LPVOID)wParam, 0, NULL);

        // 아이템 획득 확인
        switch (wParam)
        {
        case VK_SPACE:
            // 자원이랑 플레이어랑 곂치면 자원 랜덤위치에 생성
            if (true == IntersectRect(&is, &g_me, &g_woodItem))
            {
                g_woodItem.left = 10 + (rand() % 1330);
                g_woodItem.top = 10 + (rand() % 520);
                g_woodItem.right = g_woodItem.left + 70;
                g_woodItem.bottom = g_woodItem.top + 130;
                g_wood += 1;
                PlayerStop();
            }
            if (true == IntersectRect(&is, &g_me, &g_stoneItem)) {
                g_stoneItem.left = 10 + (rand() % 1320);
                g_stoneItem.top = 10 + (rand() % 570);
                g_stoneItem.right = g_stoneItem.left + 70;
                g_stoneItem.bottom = g_stoneItem.top + 70;
                g_stone += 1;
                PlayerStop();
            }if (true == IntersectRect(&is, &g_me, &g_grassItem)) {
                g_grassItem.left = 10 + (rand() % 1320);    
                g_grassItem.top = 10 + (rand() % 570);      
                g_grassItem.right = g_grassItem.left + 20;
                g_grassItem.bottom = g_grassItem.top + 60;
                g_grass += 1;
                PlayerStop();
            }
            break;
        case 0x31:      //1번키
            if (g_wood > 2) {
                PlayerStop();
                g_hand = 1;
                g_wood -= 3;
                g_damage = 2;
            }
            else {
                g_waring = 1;
                CreateThread(NULL, 0, Waring, (LPVOID)lParam, 0, NULL);
            }
            break;
        case 0x32:      //2번키
            if (g_wood > 3 && g_stone > 2) {
                PlayerStop();
                g_hand = 2;
                g_wood -= 4;
                g_stone -= 3;
                g_damage = 4;
            }
            else {
                g_waring = 1;
                CreateThread(NULL, 0, Waring, (LPVOID)lParam, 0, NULL);
            }
            break;
        case 0x33:      //3번키
            g_hand = 3;
            
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
        HDC hdc;
        hdc = GetDC(g_hWnd);

        if (1 == wParam)
        {
            g_gametime--;   // 남은 시간 감소
            
            if (0 >= g_gametime)
            {
                KillTimer(hWnd, 1);
                SetTimer(hWnd, 2, g_timer2, NULL);
                //CreateThread(NULL, 0, Enemy, g_hWnd, 0, NULL);
            }
           
        }
        else if(2 == wParam) 
        {
            if (g_playerHP <= 0)    //플레이어의 체력이 0이면 게임오버 메시지 띄움
            {
                //CreateThread(NULL, 0, Gameover, (LPVOID)wParam, 0, NULL);
                KillTimer(hWnd, 2);
            }
           //CreateThread(NULL, 0, Enemy, g_hWnd, 0, NULL);
           
        }

        ReleaseDC(g_hWnd, hdc);
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_CREATE:
    {
        g_hWnd = hWnd;
        int i;
        CreateThread(NULL, 0, Enemy, (LPVOID)wParam, 0, NULL);
        /*for (i = 0; i < 4; i++) {
            CloseHandle(CreateThread(NULL, 0, Enemy2, &arRect[i], 0, &ThreadID));
        }*/
        

        // 랜덤 시드 값 초기화
        srand(time(NULL));

        // 남은 시간 초기화
        g_gametime = 5;

        g_timer1 = 1000;    
        g_timer2 = 500;    
        SetTimer(hWnd, 1, g_timer1, NULL);  //1번으로 타이머 설정

        //플레이어
        g_me.left = 620;        //가로길이 60
        g_me.top = 225;         //세로길이 100
        g_me.right = 680;
        g_me.bottom = 325;


        //그라운드
        g_ground.left = 10;     //가로길이 1390
        g_ground.top = 10;      //세로길이 640
        g_ground.right = 1400;
        g_ground.bottom = 650;

        //나무
        g_woodItem.left = 10 + (rand() % 1320);     //가로길이 70
        g_woodItem.top = 10 + (rand() % 510);       //세로길이 130
        g_woodItem.right = g_woodItem.left + 70;
        g_woodItem.bottom = g_woodItem.top + 130;

        //돌
        g_stoneItem.left = 10 + (rand() % 1320);    //가로길이 70
        g_stoneItem.top = 10 + (rand() % 570);      //세로길이 70
        g_stoneItem.right = g_stoneItem.left + 70;
        g_stoneItem.bottom = g_stoneItem.top + 70;

        //풀
        g_grassItem.left = 10 + (rand() % 1320);    //가로길이 10
        g_grassItem.top = 10 + (rand() % 570);      //세로길이 50
        g_grassItem.right = g_grassItem.left + 20;
        g_grassItem.bottom = g_grassItem.top + 60;


    }
    break;

    case WM_PAINT:
    {
        int i;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        WCHAR buf[128] = { 0, };
        //무슨 자원인지 표시해주는 문자열
        WCHAR woodText[32] = { 0, };    
        WCHAR stoneText[32] = { 0, };   
        WCHAR grassText[32] = { 0, };

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

        // 상대 캐릭터
        if (g_enemyHP > 0)
        {
            Rectangle(hdc, enemy1.left, enemy1.top, enemy1.right, enemy1.bottom);
            wsprintf(buf, L"[ %d ]", g_enemyHP);
            TextOut(hdc, enemy1.left + 30, enemy1.top + 30, buf, lstrlen(buf));
        }

        // 나무
        //woodOs_b = (HBRUSH)SelectObject(hdc, wood_b);
        Rectangle(hdc, g_woodItem.left, g_woodItem.top, g_woodItem.right, g_woodItem.bottom);
        wsprintf(woodText, L"나무");
        TextOut(hdc, g_woodItem.left+15, g_woodItem.top+30, woodText, lstrlen(woodText));

        // 돌
        Ellipse(hdc, g_stoneItem.left, g_stoneItem.top, g_stoneItem.right, g_stoneItem.bottom);
        wsprintf(stoneText, L"돌");
        TextOut(hdc, g_stoneItem.left + 25, g_stoneItem.top + 25, stoneText, lstrlen(stoneText));

        //풀 
        Rectangle(hdc, g_grassItem.left, g_grassItem.top, g_grassItem.right, g_grassItem.bottom);
        wsprintf(grassText, L"풀");
        TextOut(hdc, g_grassItem.left+2 , g_grassItem.top + 30, grassText, lstrlen(grassText));

        // 내 캐릭터
        //meOs_b = (HBRUSH)SelectObject(hdc, me_b);
        Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);

        //방망이
        if (g_hand == 1)
            Rectangle(hdc, g_woodClub.left, g_woodClub.top, g_woodClub.right, g_woodClub.bottom);
        else if (g_hand == 2) {
            Rectangle(hdc, g_stoneAxe1.left, g_stoneAxe1.top, g_stoneAxe1.right, g_stoneAxe1.bottom);
            Rectangle(hdc, g_stoneAxe2.left, g_stoneAxe2.top, g_stoneAxe2.right, g_stoneAxe2.bottom);
        }
        else if (g_hand == 3) {
            //Rectangle(hdc, g_arrow.left, g_arrow.top, g_arrow.right, g_arrow.bottom);
        }
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
        wsprintf(buf, L"풀[ %d ]", g_grass);
        TextOut(hdc, 1330, 90, buf, lstrlen(buf));
        wsprintf(buf, L"남은 시간 : [ %d ]", g_gametime);
        TextOut(hdc, 1290, 110, buf, lstrlen(buf));
        if (g_playerHP > 0) {
            wsprintf(buf, L"남은체력[ %d ]", g_playerHP);
            TextOut(hdc, 1290, 150, buf, lstrlen(buf));
        }
        else
            g_playerHP = 0;
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
