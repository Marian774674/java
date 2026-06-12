#pragma once
#include <string>
#include <vector>
#include <functional>

// 控制台颜色
enum class Color {
    Default = 7,
    Red = 12,
    Green = 10,
    Yellow = 14,
    Blue = 9,
    Magenta = 13,
    Cyan = 11,
    White = 15,
    DarkGray = 8
};

// 控制台 ASCII 渲染器
class Renderer {
public:
    static constexpr int CONSOLE_WIDTH = 80;
    static constexpr int CONSOLE_HEIGHT = 30;

    Renderer();

    // 清空缓冲区
    void clear();

    // 清空控制台屏幕（仅启动时调用）
    void clearScreen();

    // 设置光标位置
    void setCursor(int x, int y);

    // 设置颜色
    void setColor(Color fg, Color bg = Color::Default);

    // 重置颜色
    void resetColor();

    // 在指定位置绘制字符
    void drawChar(int x, int y, char ch, Color color = Color::Default);

    // 在指定位置绘制字符串
    void drawString(int x, int y, const std::string& str, Color color = Color::Default);

    // 绘制带颜色的字符
    void drawCharColored(int x, int y, char ch, Color color);

    // 绘制进度条 ██████░░░
    void drawBar(int x, int y, int width, int current, int max,
                 Color fillColor = Color::Green, Color emptyColor = Color::DarkGray);

    // 绘制矩形框 ╔═╗║╚═╝
    void drawBox(int left, int top, int right, int bottom,
                 Color borderColor = Color::Cyan);

    // 居中文本
    void drawCentered(int y, const std::string& str, Color color = Color::Default);

    // 清空一行
    void clearLine(int y);

    // 在指定位置绘制字符串，自动截断到 maxX
    void drawStringBounded(int x, int y, int maxX, const std::string& str, Color color = Color::Default);

    // 等待按键
    int waitForKey();

    // 渲染副屏幕（双缓冲切换）
    void present();

    // 系统相关
    void enableConsoleMode();

    // 计算字符串的显示宽度（ASCII=1列，CJK=2列）
    static int displayWidth(const std::string& str);

private:
    std::vector<std::vector<char>> m_buffer;
    std::vector<std::vector<Color>> m_colorBuffer;
    bool m_virtualTerminalEnabled = false;
#ifdef _WIN32
    void* m_hConsole = nullptr;  // Windows console handle
#endif
};
