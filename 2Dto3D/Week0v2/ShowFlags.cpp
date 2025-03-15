#include "ShowFlags.h"
#include "World.h"
#include "PrimitiveComponent.h"

ShowFlags::ShowFlags()
{
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
	float controllWindowHeight = static_cast<float>(height) * 0.35f;

	float controllWindowPosX = (static_cast<float>(width) - controllWindowWidth) * 0.45f;
	float controllWindowPosY = (static_cast<float>(height) - controllWindowHeight) * 0.f;

	// â ũ��� ��ġ ����
	ImGui::SetNextWindowPos(ImVec2(controllWindowPosX, controllWindowPosY));
	ImGui::SetNextWindowSize(ImVec2(controllWindowWidth, controllWindowHeight), ImGuiCond_Always);

	if (ImGui::Begin("ShowFlags"))
	{
		const char* items[] = { "Item 1", "Item 2", "Item 3" };
		static bool selected[IM_ARRAYSIZE(items)] = { false, false, false };  // �� �׸��� üũ ���� ����

		if (ImGui::BeginCombo("Select Items", "Select multiple"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(items); i++)
			{
				ImGui::Checkbox(items[i], &selected[i]); 
			}
			ImGui::EndCombo(); 
		}


	}
	ImGui::End(); // ������ ����
}

void ShowFlags::OnResize(HWND hWnd)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;
}