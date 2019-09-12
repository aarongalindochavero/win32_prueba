// TicTacToe.cpp : Define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include "TicTacToe.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal
const int CELL_SIZE = 100;
HICON hIcon1, hIcon2;
char textReaded[20];

BOOL GetGameBoardRect(HWND hWnd, RECT *pRect) {
	RECT rc;
	if (GetClientRect(hWnd, &rc)) {
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		pRect-> left = (width - CELL_SIZE * 3) / 2;
		pRect->top = (height - CELL_SIZE * 3) / 2;
		pRect->right = pRect->left + CELL_SIZE * 3;
		pRect->bottom = pRect->top + CELL_SIZE * 3;
	}
	return TRUE;
}

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}
// Declaraciones de funciones adelantadas incluidas en este módulo de código:
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

    // TODO: Colocar código aquí.

    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TICTACTOE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TICTACTOE));

    MSG msg;

    // Bucle principal de mensajes:
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
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TICTACTOE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground = (HBRUSH)(GetStockObject(GRAY_BRUSH));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TICTACTOE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Almacenar identificador de instancia en una variable global

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
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//

int GetCellNumberFromPoint(HWND hWnd, int x, int y) {
	RECT rc;
	POINT pt = { x,y };
	if (GetGameBoardRect(hWnd, &rc)) {
		if (PtInRect(&rc, pt)) {
			x = pt.x - rc.left;
			y = pt.y - rc.top;
			int column = x / CELL_SIZE;
			int row = y / CELL_SIZE;
			return column + row * 3;
		}
	}
	return -1;
}

bool GetCellRect(HWND hWnd, int index, RECT *pRect) {
	RECT rcBoard;
	SetRectEmpty(pRect);
	if (index < 0 || index > 8) return false;
	if (GetGameBoardRect(hWnd, &rcBoard)) {
		int y = index / 3;
		int x = index % 3;
		pRect->left = rcBoard.left + x * CELL_SIZE;
		pRect->top = rcBoard.top + y * CELL_SIZE;
		pRect->right = pRect->left + CELL_SIZE;
		pRect->bottom = pRect->top + CELL_SIZE;
		return TRUE;
	}
	return FALSE;
}

HWND TextBox;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE: {
		hIcon1 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
		hIcon2 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TICTACTOE));
		TextBox = CreateWindowA("EDIT", " ", WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 10, 400, 20, hWnd, NULL, NULL, NULL);
		CreateWindowA("BUTTON", "Oki Doki", WS_VISIBLE | WS_CHILD | WS_BORDER, 420, 10, 70, 20, hWnd, (HMENU)1, NULL, NULL);

	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizar las selecciones de menú:
            switch (wmId)
            {
			case 1: {
				int length = GetWindowTextLength(TextBox) + 1;
				GetWindowTextA(TextBox, textReaded, length);
				MessageBeep(MB_ICONASTERISK);
				MessageBoxA(hWnd, textReaded, textReaded, MB_OK);
				break;
			}
			case ID_FILE_NEWGAME: {
				MessageBoxA(hWnd, "Estás sure", "Really", MB_YESNO | MB_ICONQUESTION);
				break;
			}
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
	case WM_LBUTTONDOWN: {
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		int index = GetCellNumberFromPoint(hWnd, xPos, yPos);
		HDC hdc = GetDC(hWnd);
		if (hdc) {
			/*WCHAR temp[100];
			wsprintf(temp, L"index = %d", index);
			TextOut(hdc, xPos, yPos, temp, lstrlen(temp));
			ReleaseDC(hWnd, hdc);*/
			if (index != -1) {
				RECT rcCell;
				if (GetCellRect(hWnd, index, &rcCell)) {
					//FillRect(hdc, &rcCell, GetStockBrush(BLACK_BRUSH));
					DrawIcon(hdc, rcCell.left + (CELL_SIZE / 2)-16, rcCell.top + (CELL_SIZE / 2) -16, hIcon1);
					DrawIcon(hdc, rcCell.left + (CELL_SIZE / 2) - 16, rcCell.top + (CELL_SIZE / 2) - 16, hIcon2);
				}
			}
		}
		break;
	}
	case WM_GETMINMAXINFO: {
		MINMAXINFO *pMinMax = (MINMAXINFO*)lParam;
		pMinMax->ptMinTrackSize.x = CELL_SIZE * 5;
		pMinMax->ptMinTrackSize.y = CELL_SIZE * 5;
	}

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps); 
			RECT rc;
			if (GetGameBoardRect(hWnd, &rc)) {
				RECT rcClient;
				if (GetClientRect(hWnd, &rcClient)) {
					const WCHAR szText[] = L"Holi";
					SetBkMode(hdc, TRANSPARENT);
					SetTextColor(hdc, RGB(5,96,6));
					TextOut(hdc, rcClient.right - 100, 200, szText, ARRAYSIZE(szText));
					SetTextColor(hdc, RGB(51,64,65));
					TextOut(hdc, rcClient.right - 100, 300, szText, ARRAYSIZE(szText));
				}
				FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
				//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			}
			
			for (int i = 0; i < 4; i++){
				//Líneas verticales
				DrawLine(hdc, rc.left + CELL_SIZE * i, rc.top, rc.left + CELL_SIZE * i, rc.bottom);
				DrawLine(hdc, rc.left, rc.top + CELL_SIZE * i, rc.right, rc.top + CELL_SIZE * i);
			}
            // TODO: Agregar cualquier código de dibujo que use hDC aquí...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DestroyIcon(hIcon1);
		DestroyIcon(hIcon2);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
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
