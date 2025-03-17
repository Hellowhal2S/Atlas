#include "ShowFlags.h"
#include "World.h"
#include "PrimitiveComponent.h"

ShowFlags::ShowFlags()
{
	constexpr uint64_t AllFlags =
		static_cast<uint64_t>(EEngineShowFlags::SF_Grid) |
		static_cast<uint64_t>(EEngineShowFlags::SF_AABB) |
		static_cast<uint64_t>(EEngineShowFlags::SF_Primitives) |
		static_cast<uint64_t>(EEngineShowFlags::SF_BillboardText);
}

ShowFlags::~ShowFlags()
{
}

ShowFlags& ShowFlags::GetInstance()
{
	static ShowFlags instance;
	return instance;
}

void ShowFlags::Draw(UWorld* world)
{
	float controllWindowWidth = static_cast<float>(width) * 0.2f;
	float controllWindowHeight = static_cast<float>(height) * 0.f;

	float controllWindowPosX = (static_cast<float>(width) - controllWindowWidth) * 0.45f;
	float controllWindowPosY = (static_cast<float>(height) - controllWindowHeight) * 0.f;

	// â ũ��� ��ġ ����
	ImGui::SetNextWindowPos(ImVec2(controllWindowPosX, controllWindowPosY));
	ImGui::SetNextWindowSize(ImVec2(controllWindowWidth, controllWindowHeight), ImGuiCond_Always);

	if (ImGui::Begin("ShowFlags"))
	{
		const char* items[] = { "AABB", "Primitves","BillBoardText"};
		static bool selected[IM_ARRAYSIZE(items)] = { true, true, true };  // �� �׸��� üũ ���� ����

		if (ImGui::BeginCombo("Show Flags", "Select Show Flags"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(items); i++)
			{
				ImGui::Checkbox(items[i], &selected[i]); 
			}
			ImGui::EndCombo(); 
		}
		currentFlags = ConvertSelectionToFlags(selected);

	}
	ImGui::End(); // ������ ����
}
uint64 ShowFlags::ConvertSelectionToFlags(const bool selected[])
{
	uint64 flags = static_cast<uint64>(EEngineShowFlags::None);

	if (selected[0])
		flags |= static_cast<uint64>(EEngineShowFlags::SF_AABB);
	if (selected[1])
		flags |= static_cast<uint64>(EEngineShowFlags::SF_Primitives);
	if (selected[2])
		flags |= static_cast<uint64>(EEngineShowFlags::SF_BillboardText);

	return flags;
}

void ShowFlags::OnResize(HWND hWnd)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;
}