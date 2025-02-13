#include "TorusDefines.h"
#include <iostream>
#include <deque>

const std::string g_torusText[4] = {
    "RRR",
    "GGG",
    "BBB",
    "MMM",
};

void gotoxy( int x, int y )
{
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
}

void SetColor( WORD fore, WORD back )
{
    HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

    WORD attribute;
    attribute = ( back & 0x0f ) << 4 | ( fore & 0x0f );
    SetConsoleTextAttribute( hConsole, attribute );

    /** predefined flags
        FOREGROUND_BLUE
        FOREGROUND_GREEN
        FOREGROUND_RED
        FOREGROUND_INTENSITY
        BACKGROUND_BLUE
        BACKGROUND_GREEN
        BACKGROUND_RED
        BACKGROUND_INTENSITY
        */
}

void ShowConsoleCursor( bool showFlag )
{
    HANDLE out = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo( out, &cursorInfo );
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo( out, &cursorInfo );
}

//void DrawText(int x, int y, const std::string& text)
//{
//    gotoxy(x, y);
//    std::cout << text.c_str();
//}
//
//void DrawText(int x, int y, int numRows, const std::string& text)
//{
//    int tx = x;
//    int ty = y;
//    for (int i = 0; i < numRows; ++i) {
//        DrawText(tx, ty, text);
//        ty += 1;
//    }
//}
//
//void DrawDeque(std::deque<TORUS> g, int x, int y, int maxQueue )
//{
//    int cx = x;
//    int cy = y;
//    std::deque<TORUS>::iterator it;
//    const int numEmpty = maxQueue - (int)g.size();
//    const int numTorus = maxQueue - numEmpty;
//    for (int i = 0; i < numEmpty; i++)
//    {
//        DrawText(cx, cy, "---");
//        cy += 1;
//    }
//    cy += numTorus-1;
//    for (it = g.begin(); it != g.end(); it++)
//    {
//        DrawText(cx, cy, g_torusText[*it]);
//        cy -= 1;
//    }
//}
