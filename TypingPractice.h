//======================================================================
// 헤더 파일
//
#ifndef _WINMAIN_STYLE_4_H_
#define _WINMAIN_STYLE_4_H_
//================================================================
// 원소의 개수 반환
#define dim(x) (sizeof(x) / sizeof(x[0])) 

//----------------------------------------------------------------------
// 데이터 타입 정의
//
struct decodeUINT {   // 메시지와 함수를 대응 시키는 구조체
    UINT Code;
    LRESULT (*Fxn)(HWND, UINT, WPARAM, LPARAM);
}; 

struct decodeCMD {    // 메뉴 ID와 함수를 대응 시키는 구조체
    UINT Code;
    LRESULT (*Fxn)(HWND, WORD, HWND, WORD);
};

//----------------------------------------------------------------------
// 함수 선언
//
BOOL InitApplication (HINSTANCE);
BOOL InitInstance (HINSTANCE, LPSTR, int);
int TermInstance (HINSTANCE, int);

// 윈도우 프로시저
LRESULT CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);

// 메시지 핸들러
LRESULT DoCreateMain (HWND, UINT, WPARAM, LPARAM);
LRESULT DoPaintMain (HWND, UINT, WPARAM, LPARAM);
LRESULT DoDestroyMain (HWND, UINT, WPARAM, LPARAM);
LRESULT DoTimerMain(HWND, UINT, WPARAM, LPARAM);
LRESULT DoKeyChar(HWND, UINT, WPARAM, LPARAM);

//TIMER CALLBACK 함수
void CALLBACK CallBackTimer(HWND, UINT, UINT, DWORD);
//----------------------------------------------------------------------
// MainWndProc용 처리 메시지 분배 테이블
const struct decodeUINT MainMessages[] = {
    WM_CREATE, DoCreateMain,
    WM_PAINT, DoPaintMain,
    WM_DESTROY, DoDestroyMain,
	WM_TIMER, DoTimerMain,
	WM_CHAR, DoKeyChar
};

#endif	/* _WINMAIN_STYLE_4_H_ */