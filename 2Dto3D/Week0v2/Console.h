#pragma once
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#include "Define.h"

enum class LogLevel { Display, Warning, Error };

class Console {
private:
    Console();
    ~Console();
public:
    static Console& GetInstance();

    void Clear();
    void AddLog(LogLevel level, const char* fmt, ...);
    void Draw();

public:
    struct LogEntry {
        LogLevel level;
        FString message;
    };

    TArray<LogEntry> items;
    TArray<FString> history;
    int32 historyPos = -1;
    char inputBuf[256] = "";
    bool scrollToBottom = false;

    ImGuiTextFilter filter;  // ���͸��� ���� ImGuiTextFilter

    // �߰��� ��� ������
    bool showLogTemp = true;   // LogTemp üũ�ڽ�
    bool showWarning = true;   // Warning üũ�ڽ�
    bool showError = true;     // Error üũ�ڽ�

    // ���� ����
    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;
    Console(Console&&) = delete;
    Console& operator=(Console&&) = delete;
};
