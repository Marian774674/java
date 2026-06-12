#include "game/GameEngine.h"
#include <iostream>
#include <locale>
#include"E:\develop\C++Code\console_utf8.h"

#ifdef _WIN32
#include <windows.h>
#endif

// ═══════════════════════════════════════════════════
//  代码地牢 (Code Dungeon)
//  C++ 课程大作业 — 控制台策略 Roguelike 游戏
// ═══════════════════════════════════════════════════

int main() {
#ifdef _WIN32
    // Windows 控制台 UTF-8 支持
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 设置控制台标题
    SetConsoleTitleW(L"Code Dungeon");
#endif

    auto& engine = GameEngine::instance();

    try {
        engine.init();
        engine.run();
    } catch (const std::exception& e) {
        std::cerr << "Game exception: " << e.what() << std::endl;
        return 1;
    }

    engine.shutdown();
    return 0;
}
