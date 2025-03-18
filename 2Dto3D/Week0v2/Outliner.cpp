#include "Outliner.h"
#include "World.h"
#include "PrimitiveComponent.h"

Outliner::Outliner()
{
}

Outliner::~Outliner()
{
}

Outliner& Outliner::GetInstance()
{
	static Outliner instance;
	return instance;
}

void Outliner::Draw(UWorld* world)
{
    float controllWindowWidth = static_cast<float>(width) * 0.178f;
    float controllWindowHeight = static_cast<float>(height) * 0.15f;

    float controllWindowPosX = (static_cast<float>(width) - controllWindowWidth) * 0.f;
    float controllWindowPosY = (static_cast<float>(height) - controllWindowHeight) * 0.47f;

    // â ũ��� ��ġ ����
    ImGui::SetNextWindowPos(ImVec2(controllWindowPosX, controllWindowPosY));
    ImGui::SetNextWindowSize(ImVec2(controllWindowWidth, controllWindowHeight), ImGuiCond_Always);

    if (ImGui::Begin("OutLiner")) // ������ ����
    {
        if (ImGui::TreeNode("Primitives")) // Ʈ�� ��� ����
        {
            static int selected = -1; // ���õ� �׸� ����� ����

            // ������Ʈ ����Ʈ
            for (int32 i = 0; i < world->GetObjectArr().size();i++)
            {
                if(!world->GetObjectArr()[i]->IsA(UPrimitiveComponent::StaticClass()))
                    continue;
                // ���� ���� �׸� (Selectable)
                if (ImGui::Selectable(world->GetObjectArr()[i]->GetName().ToString().c_str(), selected == i))
                {
                    selected = i; // ���õ� ������ ������Ʈ
                    world->SetPickingObj(world->GetObjectArr()[i]);
                }
            }
            ImGui::TreePop(); // Ʈ�� �ݱ�
        }
    }
    ImGui::End(); // ������ ����
}

void Outliner::OnResize(HWND hWnd)
{
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    width = clientRect.right - clientRect.left;
    height = clientRect.bottom - clientRect.top;

}