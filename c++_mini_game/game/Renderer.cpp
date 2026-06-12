#include "Renderer.h"
#include <iostream>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

Renderer::Renderer() {
    m_buffer.resize(CONSOLE_HEIGHT, std::vector<char>(CONSOLE_WIDTH, ' '));
    m_colorBuffer.resize(CONSOLE_HEIGHT, std::vector<Color>(CONSOLE_WIDTH, Color::Default));
#ifdef _WIN32
    m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(m_hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(m_hConsole, &cursorInfo);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void Renderer::clear() {
    for (auto& row : m_buffer) {
        std::fill(row.begin(), row.end(), ' ');
    }
    for (auto& row : m_colorBuffer) {
        std::fill(row.begin(), row.end(), Color::Default);
    }
    // Note: does NOT clear the actual console screen.
    // present() will overwrite the previous frame with new content.
}

void Renderer::clearScreen() {
#ifdef _WIN32
    COORD topLeft = {0, 0};
    DWORD written;
    FillConsoleOutputCharacterA(m_hConsole, ' ', CONSOLE_WIDTH * CONSOLE_HEIGHT, topLeft, &written);
    FillConsoleOutputAttribute(m_hConsole, 7, CONSOLE_WIDTH * CONSOLE_HEIGHT, topLeft, &written);
    SetConsoleCursorPosition(m_hConsole, topLeft);
#else
    std::cout << "\033[2J\033[H" << std::flush;
#endif
}

void Renderer::setCursor(int x, int y) {}
void Renderer::setColor(Color, Color) {}
void Renderer::resetColor() {}
void Renderer::enableConsoleMode() {}

void Renderer::drawChar(int x, int y, char ch, Color color) {
    if (x < 0 || x >= CONSOLE_WIDTH || y < 0 || y >= CONSOLE_HEIGHT) return;
    m_buffer[y][x] = ch;
    m_colorBuffer[y][x] = color;
}

int Renderer::displayWidth(const std::string& str) {
    int width = 0;
    size_t i = 0;
    while (i < str.length()) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (c < 0x80) {
            width += 1;
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            width += 1;
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            width += 2;  // CJK 字符占 2 列
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            width += 2;  // emoji / 罕见汉字
            i += 4;
        } else {
            // 连续字节或无效字节，跳过
            width += 1;
            i += 1;
        }
    }
    return width;
}

void Renderer::drawString(int x, int y, const std::string& str, Color color) {
    int col = x;
    size_t i = 0;
    while (i < str.length() && col < CONSOLE_WIDTH) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        int bytes = 1;
        if (c < 0x80) {
            bytes = 1;
        } else if ((c & 0xE0) == 0xC0) {
            bytes = 2;
        } else if ((c & 0xF0) == 0xE0) {
            bytes = 3;
        } else if ((c & 0xF8) == 0xF0) {
            bytes = 4;
        }
        // 写入该 UTF-8 字符的所有字节
        for (int b = 0; b < bytes && (i + b) < str.length() && (col + b) < CONSOLE_WIDTH; ++b) {
            drawChar(col + b, y, str[i + b], color);
        }
        i += bytes;
        col += bytes;
    }
}

void Renderer::drawCharColored(int x, int y, char ch, Color color) {
    drawChar(x, y, ch, color);
}

void Renderer::drawBar(int x, int y, int width, int current, int max,
                        Color fillColor, Color emptyColor) {
    int filled = (max > 0) ? (current * width / max) : 0;
    for (int i = 0; i < width; ++i) {
        char ch = (i < filled) ? '\xDB' : '\xB0';
        Color c = (i < filled) ? fillColor : emptyColor;
        drawChar(x + i, y, ch, c);
    }
}

void Renderer::drawBox(int left, int top, int right, int bottom,
                        Color borderColor) {
    drawChar(left, top, '\xDA', borderColor);
    drawChar(right, top, '\xBF', borderColor);
    drawChar(left, bottom, '\xC0', borderColor);
    drawChar(right, bottom, '\xD9', borderColor);
    for (int x = left + 1; x < right; ++x) {
        drawChar(x, top, '\xCD', borderColor);
        drawChar(x, bottom, '\xCD', borderColor);
    }
    for (int y = top + 1; y < bottom; ++y) {
        drawChar(left, y, '\xBA', borderColor);
        drawChar(right, y, '\xBA', borderColor);
    }
}

void Renderer::drawCentered(int y, const std::string& str, Color color) {
    int w = displayWidth(str);
    int x = (CONSOLE_WIDTH - w) / 2;
    if (x < 0) x = 0;
    drawString(x, y, str, color);
}

void Renderer::drawStringBounded(int x, int y, int maxX, const std::string& str, Color color) {
    int visualMax = maxX;
    int byteCol = x;
    int visualCol = x;
    size_t i = 0;

    while (i < str.length() && byteCol < CONSOLE_WIDTH) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        int bytes = 1;
        int charWidth = 1;

        if (c < 0x80) {
            bytes = 1; charWidth = 1;
        } else if ((c & 0xE0) == 0xC0) {
            bytes = 2; charWidth = 1;
        } else if ((c & 0xF0) == 0xE0) {
            bytes = 3; charWidth = 2;  // CJK
        } else if ((c & 0xF8) == 0xF0) {
            bytes = 4; charWidth = 2;
        }

        // 如果该字符会超出视觉边界，停止
        if (visualCol + charWidth > visualMax) break;

        // 写入该 UTF-8 字符的所有字节
        for (int b = 0; b < bytes && (i + b) < str.length() && (byteCol + b) < CONSOLE_WIDTH; ++b) {
            drawChar(byteCol + b, y, str[i + b], color);
        }

        i += bytes;
        byteCol += bytes;
        visualCol += charWidth;
    }
}

void Renderer::clearLine(int y) {
    for (int x = 0; x < CONSOLE_WIDTH; ++x) {
        drawChar(x, y, ' ');
    }
}

int Renderer::waitForKey() {
#ifdef _WIN32
    return _getch();
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

void Renderer::present() {
#ifdef _WIN32
    for (int y = 0; y < CONSOLE_HEIGHT; ++y) {
        COORD pos = {0, static_cast<SHORT>(y)};
        SetConsoleCursorPosition(m_hConsole, pos);
        for (int x = 0; x < CONSOLE_WIDTH; ++x) {
            SetConsoleTextAttribute(m_hConsole, static_cast<WORD>(m_colorBuffer[y][x]));
            std::cout << m_buffer[y][x];
        }
    }
    SetConsoleTextAttribute(m_hConsole, 7);
    COORD endPos = {0, static_cast<SHORT>(CONSOLE_HEIGHT)};
    SetConsoleCursorPosition(m_hConsole, endPos);
#else
    for (int y = 0; y < CONSOLE_HEIGHT; ++y) {
        std::cout << "\033[" << (y + 1) << ";1H";
        for (int x = 0; x < CONSOLE_WIDTH; ++x) {
            std::cout << m_buffer[y][x];
        }
    }
#endif
    std::cout << std::flush;
}
