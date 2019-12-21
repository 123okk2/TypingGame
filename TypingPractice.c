//======================================================================
// Win32 API Template - Douglas Boling 코드
//
//======================================================================
#include <windows.h>                 // 윈도우 관련 정의
#include "TypingPractice.h"          // 프로그램 관련 정의
#include <time.h>
#include <stdlib.h>

//----------------------------------------------------------------------
// 전역 데이터
//
typedef struct Words {
	TCHAR words[10];
	RECT wordRect;
	boolean isExist;
}Words;

Words word[10];
TCHAR inputs[10];
int inputStringLength = 0;

LPTSTR words[10] = { TEXT("son"),TEXT("daughter"),TEXT("mother"),TEXT("father"),TEXT("family"),
TEXT("home"),TEXT("house"),TEXT("puppy"),TEXT("kitty"),TEXT("dog") };


int score = 0;
int life = 3;


#define ID_TIMER	1
#define INPUT_TIMER 2

static const LPCWSTR szAppName = TEXT("TypingPractice");   // 프로그램 이름
HINSTANCE hInst;								// 프로그램 인스턴스 핸들
HWND hwndMain;									// 메인 윈도우 핸들

												//======================================================================
												// 프로그램 시작점
												//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	int rc = 0;

	// 어플리케이션 초기화
	if (!InitApplication(hInstance))
		return 0;

	// 인스턴스 초기화
	if (!InitInstance(hInstance, lpCmdLine, nCmdShow))
		return 0;

// 메시지 루프
while (GetMessage(&msg, NULL, 0, 0)) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

// 인스턴스 소거
return TermInstance(hInstance, msg.wParam);
}

//----------------------------------------------------------------------
// InitApp - 어플리케이션 초기화
//
BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASS wc;

	// 전역 변수에 인스턴스 핸들 보관
	hInst = hInstance;

	// 주 윈도우 클래스 등록
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // 윈도우 스타일
	wc.lpfnWndProc = MainWndProc;             // 윈도우 프로시저
	wc.cbClsExtra = 0;                        // 추가 클래스 데이터
	wc.cbWndExtra = 0;                        // 추가 윈도우 데이터
	wc.hInstance = hInstance;                 // 소유자 핸들
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // 프로그램 아이콘
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);// 기본 커서
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;                  // 메뉴 이름
	wc.lpszClassName = (LPWSTR)szAppName;     // 윈도우 클래스 이름

	if (!RegisterClass(&wc))
		return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------
// InitInstance - 인스턴스 초기화
//
BOOL InitInstance(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 주 윈도우 생성
	hwndMain = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,           // 윈도우 클래스
		szAppName,     // 윈도우 타이틀
					   // 스타일 플래그
		WS_OVERLAPPEDWINDOW,  //WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT,       // x 좌표
		CW_USEDEFAULT,       // y 좌표
		400,       // 초기 너비
		500,       // 초기 높이
		NULL,                // 부모 윈도우 핸들
		NULL,                // 메뉴 (NULL로 설정)
		hInstance,           // 응용프로그램 인스턴스
		NULL);               // 생성 매개변수 포인터

	if (!IsWindow(hwndMain))
		return FALSE;  // 윈도우 생성 실패시 작동 실패

					   // 윈도우 표시 및 갱신
	ShowWindow(hwndMain, nCmdShow);
	UpdateWindow(hwndMain);
	return TRUE;
}

//----------------------------------------------------------------------
// TermInstance - 프로그램 소거
//
int TermInstance(HINSTANCE hInstance, int nDefRC)
{
	return nDefRC;
}

//======================================================================
// 주 윈도우를 위한 메시지 처리 핸들러
//
//----------------------------------------------------------------------
// MainWndProc - 주 윈도우의 콜백 함수
//
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	int i;
	//
	// 메시지 분배 테이블을 검사하여 해당 메시지를 처리할지 확인한 후
	// 해당 메시지 핸들러를 호출
	//
	for (i = 0; i < dim(MainMessages); i++) {
		if (wMsg == MainMessages[i].Code)
			return (*MainMessages[i].Fxn)(hWnd, wMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

//----------------------------------------------------------------------
// DoCreateMain - WM_CREATE 메시지 처리
//
LRESULT DoCreateMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{

	for (int i = 0; i < 10; i++) word[i].isExist = FALSE;

	SetTimer(hWnd, ID_TIMER, 500, NULL);
	SetTimer(hWnd, INPUT_TIMER, 1, CallBackTimer);

	return 0;
}

//----------------------------------------------------------------------
// DoPaintMain - WM_PAINT 메시지 처리
//
LRESULT DoPaintMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rect;
	RECT bottomLine;
	HDC hdc;
	TCHAR lifeAndScore[20];

	static int timerCount = 0;

	// 클라이언트 영역의 크기 계산
	GetClientRect(hWnd, &rect);
	hdc = BeginPaint(hWnd, &ps);

	SetRect(&bottomLine, rect.left, rect.bottom - 30, rect.right, rect.bottom - 40);
	FillRect(hdc, &bottomLine, CreateSolidBrush(RGB(255,0,0)));
	if (timerCount % 5 == 0) {
		for (int i = 0; i < 10; i++) {
			if (!word[i].isExist) {
				int randomLocate = rand() % (rect.right - 100);
				SetRect(&word[i].wordRect, randomLocate, rect.top + 30, randomLocate + 100, rect.top + 50);
				randomLocate %= 10;
				wsprintf(word[i].words, words[randomLocate]);
				word[i].isExist = TRUE;
				break;
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		if (word[i].isExist) {
			if (bottomLine.top <= word[i].wordRect.bottom) {
				word[i].isExist = FALSE;
				life--;

				for (int i = 0; i < 10; i++) {
					if (!word[i].isExist) {
						int randomLocate = rand() % (rect.right - 100);
						SetRect(&word[i].wordRect, randomLocate, rect.top + 30, randomLocate + 100, rect.top + 50);
						randomLocate %= 10;
						wsprintf(word[i].words, words[randomLocate]);
						word[i].isExist = TRUE;
						break;
					}
				}
			}
			else {
				DrawText(hdc, word[i].words, -1, &word[i].wordRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
				word[i].wordRect.top += 20;
				word[i].wordRect.bottom += 20;
			}
		}
	}

	wsprintf(lifeAndScore, TEXT("Life=%d / Score=%d :"), life, score);
	DrawText(hdc, lifeAndScore, -1, &rect, DT_TOP | DT_LEFT | DT_SINGLELINE);

	if(life == 0) DoDestroyMain(hWnd, wMsg, wParam, lParam);

	timerCount++;

	EndPaint(hWnd, &ps);
	return 0;
}

//----------------------------------------------------------------------
// DoTimerMain - WM_TIMER 메시지 처리
//
LRESULT DoTimerMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	InvalidateRect(hWnd, NULL, TRUE);

	return 0;
}

//----------------------------------------------------------------------
// DoKeyChar - WM_CHAR 메시지 처리
//
LRESULT DoKeyChar(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR code;
	HDC hdc = GetDC(hWnd);
	boolean isCmp = TRUE;
	int index[5];
	int indexCnt = 0;
	int frontWord;

	code = wParam;

	if (isspace(code)) {
		for (int i = 0; i < 10; i++) {
			boolean isCorrect = FALSE;

			if (word[i].isExist) {
				for (int j = 0; j < lstrlen(word[i].words); j++) {
					if (word[i].words[j] != inputs[j]) break;
					if (j == lstrlen(word[i].words)-1 && word[i].words[j] == inputs[j]) {
						isCorrect = TRUE;
						word[i].isExist = FALSE;
						score += 10;
					}
				}
			}

			if (isCorrect) break;
		}

		inputStringLength = 0;
	}
	else if (code == 8) inputStringLength--;
	else {
		inputs[inputStringLength++] = wParam;
	}

	return 0;
}

//----------------------------------------------------------------------
// DoDestroyMain - WM_DESTROY 메시지 처리ㅇ
//
LRESULT DoDestroyMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	KillTimer(hWnd, ID_TIMER);
	if(life == 0 ) MessageBox(hWnd, TEXT("게임오버"), TEXT("게임오버"), MB_OK);
	PostQuitMessage(0);
	return 0;
}

void CALLBACK CallBackTimer(HWND hWnd, UINT nID, UINT nEl, DWORD time)
{
	HDC hdc = GetDC(hWnd);

	TextOut(hdc, 20, 435, inputs, inputStringLength);
}