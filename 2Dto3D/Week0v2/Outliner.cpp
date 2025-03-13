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
}
