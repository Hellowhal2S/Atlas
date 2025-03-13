#include "Console.h"
#include <cstdarg>
#include <cstdio>


// �̱��� �ν��Ͻ� ��ȯ
Console& Console::GetInstance() {
    static Console instance;
    return instance;
}

// ������
Console::Console() {}

// �Ҹ���
Console::~Console() {}

// �α� �ʱ�ȭ
void Console::Clear() {
    items.clear();
}

// �α� �߰�
void Console::AddLog(LogLevel level, const char* fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    items.push_back({ level, std::string(buf) });
    scrollToBottom = true;
}


// �ܼ� â ������
void Console::Draw() {
    if (!bWasOpen) return;
    // â ũ�� �� ��ġ ���
    float controllWindowWidth = static_cast<float>(width) * 0.5f;
    float expandedHeight = static_cast<float>(height) * 0.35f;
    float collapsedHeight = 25.0f; // ������ �� ����
    float controllWindowHeight = bExpand ? expandedHeight : collapsedHeight;

    float controllWindowPosX = (static_cast<float>(width) - controllWindowWidth);
    float controllWindowPosY = (static_cast<float>(height) - controllWindowHeight);

    // â ũ��� ��ġ ����
    ImGui::SetNextWindowPos(ImVec2(controllWindowPosX, controllWindowPosY));
    ImGui::SetNextWindowSize(ImVec2(controllWindowWidth, controllWindowHeight), ImGuiCond_Always);

    // â�� ǥ���ϰ� ���� ���� Ȯ��
    bExpand = ImGui::Begin("Console", &bWasOpen);
    if (!bExpand) {
        ImGui::End();
        return;
    }

    // â�� ������ ��� UI�� ǥ������ ����
    if (!bExpand) {
        ImGui::End();
        return;
    }
    


    // ��ư UI (�α� ���غ� �߰�)
    if (ImGui::Button("Add LogTemp")) { AddLog(LogLevel::Display, "LogTemp message"); }
    ImGui::SameLine();
    if (ImGui::Button("Add Warning")) { AddLog(LogLevel::Warning, "Warning message"); }
    ImGui::SameLine();
    if (ImGui::Button("Add Error")) { AddLog(LogLevel::Error, "Error message"); }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) { Clear(); }
    ImGui::SameLine();
    if (ImGui::Button("Copy")) { ImGui::LogToClipboard(); }

    ImGui::Separator();

    // ���� �Է� â
    ImGui::Text("Filter:");
    ImGui::SameLine();

    filter.Draw("##Filter", 100);
    
    ImGui::SameLine();

    // �α� ������ ������ üũ�ڽ�
    ImGui::Checkbox("Show Display", &showLogTemp);
    ImGui::SameLine();
    ImGui::Checkbox("Show Warning", &showWarning);
    ImGui::SameLine();
    ImGui::Checkbox("Show Error", &showError);

    ImGui::Separator();
    // �α� ��� (���� ����)
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& entry : items) {
        if (!filter.PassFilter(entry.message.c_str())) continue;

        // �α� ���ؿ� �´� ���͸�
        if ((entry.level == LogLevel::Display && !showLogTemp) ||
            (entry.level == LogLevel::Warning && !showWarning) ||
            (entry.level == LogLevel::Error && !showError)) {
            continue;
        }

        // ���� ����
        ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        switch (entry.level) {
        case LogLevel::Display: color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break;  // �⺻ ���
        case LogLevel::Warning: color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); break; // �����
        case LogLevel::Error:   color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); break; // ������
        }

        ImGui::TextColored(color, "%s", entry.message.c_str());
    }
    if (scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;
    }
    ImGui::EndChild();

    ImGui::Separator();

    // �Է�â
    bool reclaimFocus = false;
    if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (inputBuf[0]) {
            AddLog(LogLevel::Display, ">> %s", inputBuf);
            history.push_back(std::string(inputBuf));
            historyPos = -1;
            scrollToBottom = true; // �ڵ� ��ũ��
        }
        inputBuf[0] = '\0';
        reclaimFocus = true;
    }

    // �Է� �ʵ忡 �ڵ� ��Ŀ��
    if (reclaimFocus) {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
}
void Console::OnResize(HWND hWindow)
{
    RECT clientRect;
    GetClientRect(hWindow, &clientRect);
    width = clientRect.right - clientRect.left;
    height = clientRect.bottom - clientRect.top;
}

