#include <windows.h>

typedef struct _CONSOLE_SCREEN_BUFFER_INFOEX {
  ULONG      cbSize;
  COORD      dwSize;
  COORD      dwCursorPosition;
  WORD       wAttributes;
  SMALL_RECT srWindow;
  COORD      dwMaximumWindowSize;
  WORD       wPopupAttributes;
  WINBOOL    bFullscreenSupported;
  COLORREF   ColorTable[16];
} CONSOLE_SCREEN_BUFFER_INFOEX, *PCONSOLE_SCREEN_BUFFER_INFOEX;

WINBASEAPI WINBOOL WINAPI GetConsoleScreenBufferInfoEx(
  HANDLE hConsoleOutput,
  PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx
);

WINBASEAPI WINBOOL WINAPI SetConsoleScreenBufferInfoEx(
  HANDLE hConsoleOutput,
  PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx
);

void setchar(CHAR_INFO *buffer, int pos, CHAR ch, WORD atr) {
	buffer[pos].Char.AsciiChar = ch;
	buffer[pos].Attributes = atr;
}

int main()
{
	SetConsoleTitle("Merry Chrimbus!");
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	/* Hide cursor */
	CONSOLE_CURSOR_INFO cinfo;
	cinfo.dwSize = 1;
	cinfo.bVisible = FALSE;
	
	SetConsoleCursorInfo(handle, &cinfo);
	
	/* Set window size */
	COORD coord; 
    coord.X = 81; 
    coord.Y = 40; 

    SMALL_RECT Rect; 
    Rect.Top = 0; 
    Rect.Left = 0; 
    Rect.Bottom = coord.Y - 1; 
    Rect.Right = coord.X - 1;
    SetConsoleScreenBufferSize(handle, coord);
    SetConsoleWindowInfo(handle, TRUE, &Rect);
	
	/* Buffer */
	COORD zero = { 0, 0 };
	int buffersize = coord.X * coord.Y;
	CHAR_INFO buffer[buffersize];
	
	/* Initialize buffer */
	for(int i = 0; i < buffersize; i++) {
		buffer[i].Char.AsciiChar = (CHAR)0x00;
		buffer[i].Attributes = 0x00;
	}
	
	int xcenter = 40;
	int ypos = coord.X;
	
	/* Draw star */
	setchar(buffer, xcenter + ypos, 0x9E, 0x06);
	ypos += coord.X;
	
	CONSOLE_SCREEN_BUFFER_INFOEX csbe;
	csbe.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(handle, &csbe);
	
	csbe.ColorTable[6] = 0x66DDFF;
	csbe.ColorTable[2] = 0x558800;
	
	/* Draw tree and lights */
	int height = 0,
	parity = 1,
	paritymod = 0,
	final = parity,
	offset = 0,
	ring = 0;
	char treechar[3] = { '*', '%', '&' };
	DWORD realcolor[5] = { 0x0084FF, 0xFF8400, 0xFF00EA, 0x3EE83E, 0x3E3EE8 };
	WORD ringcolor[8] = { 0x01, 0x03, 0x05, 0x09, 0x0A, 0x0B, 0x0D, 0x0E };
	srand(time(NULL));
	for (; height < 30; height++, ypos += coord.X)
	{
		for (int pos = xcenter + ypos - final; pos <= xcenter + ypos + final; pos++)
		{
			if (((pos + offset) - 5 & 0x07) == 0)
			{
				setchar(buffer, pos, 0x40, ringcolor[rand() % 8]);
				ring++;
			}
			else
			{
				setchar(buffer, pos, treechar[rand() % 3], 0x02);
			}
		}
		offset += 4;

		parity++;
		if (parity > 6 + paritymod)
		{
			parity -= 5;
			paritymod++;
		}
		final = parity + paritymod;
	}
	
	/* Draw trunk */
	csbe.ColorTable[4] = 0x00316E;
	for (int i = 0; i < 2; i++, ypos += coord.X)
	{
		int pos = xcenter + ypos;
		if(i == 1) {
			setchar(buffer, pos - 2, 192, 0x04);
			setchar(buffer, pos - 1, 193, 0x04);
			setchar(buffer, pos    , 193, 0x04);
			setchar(buffer, pos + 1, 193, 0x04);
			setchar(buffer, pos + 2, 217, 0x04);
		} else {
			setchar(buffer, pos - 2, 179, 0x04);
			setchar(buffer, pos - 1, ' ', 0x04);
			setchar(buffer, pos    , ' ', 0x04);
			setchar(buffer, pos + 1, ' ', 0x04);
			setchar(buffer, pos + 2, 179, 0x04);
		}
	}
	
	/* Draw message */
	char *message = "X Merry Chrimbus X";
	
	csbe.ColorTable[0x0C] = 0xA664FF;
	ypos += coord.X;
	for(int i = 0; i < 18; i++) {
		if(message[i] == 'X')
			setchar(buffer, xcenter - 9 + ypos + i, 0x03, 0x0C);
		else
			setchar(buffer, xcenter - 9 + ypos + i, message[i], 0x0F);
	}
	
	/* Set all coloured lights to RGB(0x66, 0x66, 0x66) */
	for(int i = 0; i < 8; i++)
		csbe.ColorTable[ringcolor[i]] = 0x666666;
	
	++csbe.srWindow.Bottom;
	++csbe.srWindow.Right;
	SetConsoleScreenBufferInfoEx(handle, &csbe);
	
	/* Write buffer to console */
	WriteConsoleOutput(handle, buffer, coord, zero, &Rect);
	
	/* Coloured lights loop */
	int last1 = -1, last2 = -1;
	int col1 = -1, col2 = -1;
	int rlast1 = -1, rlast2 = -1;
	int rcol1 = -1, rcol2 = -1;
	while(!kbhit()) {
		do col1 = rand() % 8; while (col1 == last1 || col1 == last2);
		do col2 = rand() % 8; while (col2 == last1 || col2 == last2 || col1 == col2);
		do rcol1 = rand() % 5; while (rcol1 == rlast1 || rcol1 == rlast2);
		do rcol2 = rand() % 5; while (rcol2 == rlast1 || rcol2 == rlast2 || rcol1 == rcol2);

		if (last1 > -1)
		{
			csbe.ColorTable[ringcolor[last1]] = 0x666666;
			csbe.ColorTable[ringcolor[last2]] = 0x666666;
		}

		last1 = col1;
		last2 = col2;
		rlast1 = rcol1;
		rlast2 = rcol2;

		csbe.ColorTable[ringcolor[last1]] = realcolor[rcol1];
		csbe.ColorTable[ringcolor[last2]] = realcolor[rcol2];
		
		++csbe.srWindow.Bottom;
		++csbe.srWindow.Right;
		SetConsoleScreenBufferInfoEx(handle, &csbe);
		
		Sleep(1250);
	}
	
	getch();
	return 0;
}