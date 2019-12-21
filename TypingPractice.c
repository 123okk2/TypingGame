//======================================================================
// Win32 API Template - Douglas Boling �ڵ�
//
//======================================================================
#include <windows.h>                 // ������ ���� ����
#include "TypingPractice.h"          // ���α׷� ���� ����
#include <time.h>
#include <stdlib.h>

//----------------------------------------------------------------------
// ���� ������
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

static const LPCWSTR szAppName = TEXT("TypingPractice");   // ���α׷� �̸�
HINSTANCE hInst;								// ���α׷� �ν��Ͻ� �ڵ�
HWND hwndMain;									// ���� ������ �ڵ�

												//======================================================================
												// ���α׷� ������
												//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	int rc = 0;

	// ���ø����̼� �ʱ�ȭ
	if (!InitApplication(hInstance))
		return 0;

	// �ν��Ͻ� �ʱ�ȭ
	if (!InitInstance(hInstance, lpCmdLine, nCmdShow))
		return 0;

// �޽��� ����
while (GetMessage(&msg, NULL, 0, 0)) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

// �ν��Ͻ� �Ұ�
return TermInstance(hInstance, msg.wParam);
}

//----------------------------------------------------------------------
// InitApp - ���ø����̼� �ʱ�ȭ
//
BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASS wc;

	// ���� ������ �ν��Ͻ� �ڵ� ����
	hInst = hInstance;

	// �� ������ Ŭ���� ���
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // ������ ��Ÿ��
	wc.lpfnWndProc = MainWndProc;             // ������ ���ν���
	wc.cbClsExtra = 0;                        // �߰� Ŭ���� ������
	wc.cbWndExtra = 0;                        // �߰� ������ ������
	wc.hInstance = hInstance;                 // ������ �ڵ�
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // ���α׷� ������
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);// �⺻ Ŀ��
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;                  // �޴� �̸�
	wc.lpszClassName = (LPWSTR)szAppName;     // ������ Ŭ���� �̸�

	if (!RegisterClass(&wc))
		return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------
// InitInstance - �ν��Ͻ� �ʱ�ȭ
//
BOOL InitInstance(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �� ������ ����
	hwndMain = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,           // ������ Ŭ����
		szAppName,     // ������ Ÿ��Ʋ
					   // ��Ÿ�� �÷���
		WS_OVERLAPPEDWINDOW,  //WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT,       // x ��ǥ
		CW_USEDEFAULT,       // y ��ǥ
		400,       // �ʱ� �ʺ�
		500,       // �ʱ� ����
		NULL,                // �θ� ������ �ڵ�
		NULL,                // �޴� (NULL�� ����)
		hInstance,           // �������α׷� �ν��Ͻ�
		NULL);               // ���� �Ű����� ������

	if (!IsWindow(hwndMain))
		return FALSE;  // ������ ���� ���н� �۵� ����

					   // ������ ǥ�� �� ����
	ShowWindow(hwndMain, nCmdShow);
	UpdateWindow(hwndMain);
	return TRUE;
}

//----------------------------------------------------------------------
// TermInstance - ���α׷� �Ұ�
//
int TermInstance(HINSTANCE hInstance, int nDefRC)
{
	return nDefRC;
}

//======================================================================
// �� �����츦 ���� �޽��� ó�� �ڵ鷯
//
//----------------------------------------------------------------------
// MainWndProc - �� �������� �ݹ� �Լ�
//
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	int i;
	//
	// �޽��� �й� ���̺��� �˻��Ͽ� �ش� �޽����� ó������ Ȯ���� ��
	// �ش� �޽��� �ڵ鷯�� ȣ��
	//
	for (i = 0; i < dim(MainMessages); i++) {
		if (wMsg == MainMessages[i].Code)
			return (*MainMessages[i].Fxn)(hWnd, wMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

//----------------------------------------------------------------------
// DoCreateMain - WM_CREATE �޽��� ó��
//
LRESULT DoCreateMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{

	for (int i = 0; i < 10; i++) word[i].isExist = FALSE;

	SetTimer(hWnd, ID_TIMER, 500, NULL);
	SetTimer(hWnd, INPUT_TIMER, 1, CallBackTimer);

	return 0;
}

//----------------------------------------------------------------------
// DoPaintMain - WM_PAINT �޽��� ó��
//
LRESULT DoPaintMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rect;
	RECT bottomLine;
	HDC hdc;
	TCHAR lifeAndScore[20];

	static int timerCount = 0;

	// Ŭ���̾�Ʈ ������ ũ�� ���
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
// DoTimerMain - WM_TIMER �޽��� ó��
//
LRESULT DoTimerMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	InvalidateRect(hWnd, NULL, TRUE);

	return 0;
}

//----------------------------------------------------------------------
// DoKeyChar - WM_CHAR �޽��� ó��
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
// DoDestroyMain - WM_DESTROY �޽��� ó����
//
LRESULT DoDestroyMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	KillTimer(hWnd, ID_TIMER);
	if(life == 0 ) MessageBox(hWnd, TEXT("���ӿ���"), TEXT("���ӿ���"), MB_OK);
	PostQuitMessage(0);
	return 0;
}

void CALLBACK CallBackTimer(HWND hWnd, UINT nID, UINT nEl, DWORD time)
{
	HDC hdc = GetDC(hWnd);

	TextOut(hdc, 20, 435, inputs, inputStringLength);
}