#pragma once
#include "ControlPaner.h"
#include "World.h"
#include "CameraComponent.h"
#include "ImGuiManager.h"
#include "SceneMgr.h"
#include "Player.h"
#include "EditorViewportClient.h"
#include "EditorWindow.h"
// #include "ImGUI\imgui.h"
#include "IWindowToggleable.h"
//#include "Font\IconDefs.h"
//#include "Font/RawFonts.h"
extern FEngineLoop GEngineLoop;
ControlPanel::ControlPanel()
{
	
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::Draw(UWorld* world, double elapsedTime )
{	
	float controllWindowWidth = static_cast<float>(width) * 0.3f;
	float controllWindowHeight = static_cast<float>(height) * 0.36f;

	float controllWindowPosX = (static_cast<float>(width) - controllWindowWidth) * 0.f;
	float controllWindowPosY = (static_cast<float>(height) - controllWindowHeight) * 0.f;

	// â ũ��� ��ġ ����
	ImGui::SetNextWindowPos(ImVec2(controllWindowPosX, controllWindowPosY));
	ImGui::SetNextWindowSize(ImVec2(controllWindowWidth, controllWindowHeight), ImGuiCond_Always);


	ImGui::Begin("Jungle Control Panel");
	ImGui::Text("Hello Jungle World!");
	double fps = 1000.0 / elapsedTime;
	ImGui::Text("FPS %.2f (%.2fms)", fps, elapsedTime);
	ImGui::Separator();
	static int32 primitiveType = 0;
	const char* primitives[] = { "Sphere", "Cube", "Triangle" };
	ImGui::Combo("Primitive", &primitiveType, primitives, IM_ARRAYSIZE(primitives));

	int32 SpawnCount = world->GetObjectArr().size();
	ImGui::InputInt("Number of Spawn", &SpawnCount, 0, 0);
	if (ImGui::Button("Spawn"))
	{
		world->SpawnObject(static_cast<OBJECTS>(primitiveType));
	}

	ImGui::Separator();
	ImGuiIO& io = ImGui::GetIO();
	ImFont* UnicodeFont = io.Fonts->Fonts[FEATHER_FONT];

	ImVec2 ControlButtonSize = ImVec2(32, 32);
	ImGui::PushFont(UnicodeFont);
	ImVec4 ActiveColor = ImVec4(0, 0.5, 0, 0.6f);

	UPlayer* player = static_cast<UPlayer*>(world->GetPlayer());
	// ���� ��� ���� ���� ( �� ������ ���� �����θ� ����Ѵ�)
	if (!player) return;
	static ControlMode selectedMode = CM_TRANSLATION;

	//PropertyPanel* propPanel = world->GetPropertyPanel(); // PropertyPanel ��������
	//bool isTranslationActive = (PrimaryGizmo && PrimaryGizmo->GetCurrentGizmo() == EGizmoType::Translation);
	//if (isTranslationActive)
	//	ImGui::PushStyleColor(ImGuiCol_Button, ActiveColor); // Ȱ�� ���� ����
	if (ImGui::Button(u8"\ue9bc", ControlButtonSize))
	{
		selectedMode = CM_TRANSLATION;
		player->SetMode(CM_TRANSLATION); // ���� ��带 TRANSLATION ���� ����

		//propPanel->SetMode(CM_TRANSLATION);

		//if (PrimaryGizmo)
		//{
		//	PrimaryGizmo->SetGizmoType(EGizmoType::Translation);
		//}
	}
	
	ImGui::SameLine();

	if (ImGui::Button(u8"\ue9d3", ControlButtonSize))
	{
		selectedMode = CM_ROTATION;
		player->SetMode(CM_ROTATION);
		//if (PrimaryGizmo)
		//{
		//	PrimaryGizmo->SetGizmoType(EGizmoType::Rotation);
		//}
	}
	
	ImGui::SameLine();

	if (ImGui::Button(u8"\ue9ab", ControlButtonSize))
	{
		selectedMode = CM_SCALE;
		player->SetMode(CM_SCALE);
	}

	ImGui::Separator();

	if (ImGui::Button(u8"\ue9b7"))
	{
		Console::GetInstance().bWasOpen = !Console::GetInstance().bWasOpen;
	}
	/// Toggle Control Panel
	//if (ImGui::Button(ICON_MONITOR))
	//{
	//	Console::GetInstance().bWasOpen = !Console::GetInstance().bWasOpen;  //  ���� ����
	//}

	ImGui::PopFont();

	ImGui::Separator();

	static char sceneName[64] = "Default";
	ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));

	if (ImGui::Button("New scene")) {
		world->NewScene();
	}
	if (ImGui::Button("Save scene")) {

		FString SceneName(sceneName);
		SceneData SaveData = world->SaveData();
		FSceneMgr::SaveSceneToFile(SceneName, SaveData);
	}
	if (ImGui::Button("Load scene")) {
		FString SceneName(sceneName);
		FString LoadJsonData = FSceneMgr::LoadSceneFromFile(SceneName);
		SceneData LoadData = FSceneMgr::ParseSceneData(LoadJsonData);
		world->LoadData(LoadData);
	}
	ImGui::Separator();

	float sp = UPrimitiveBatch::GetInstance().GetSpacing();
	ImGui::SliderFloat("Grid Spacing", &sp, 1.0f, 300.0f);
	UPrimitiveBatch::GetInstance().SetSpacing(sp);

	sp = GEngineLoop.GetViewportClient()->GetCameraSpeedScalar();
	ImGui::SliderFloat("Camera Speed", &sp, 0.198f, 192.0f);
	GEngineLoop.GetViewportClient()->SetCameraSpeedScalar(sp);

	ImGui::Separator();

	ImGui::Text("Orthogonal");
	ImGui::SliderFloat("FOV", &world->GetCamera()->GetFov(), 30.0f, 120.0f);

	float cameraLocation[3] = { world->GetCamera()->GetWorldLocation().x, world->GetCamera()->GetWorldLocation().y, world->GetCamera()->GetWorldLocation().z };
	ImGui::InputFloat3("Camera Location", cameraLocation);

	float cameraRotation[3] = { world->GetCamera()->GetWorldRotation().x, world->GetCamera()->GetWorldRotation().y, world->GetCamera()->GetWorldRotation().z };
	ImGui::InputFloat3("Camera Rotation", cameraRotation);

	world->GetCamera()->SetLocation(FVector(cameraLocation[0], cameraLocation[1], cameraLocation[2]));
	world->GetCamera()->SetRotation(FVector(cameraRotation[0], cameraRotation[1], cameraRotation[2]));

	ImGui::End();
}

void ControlPanel::OnResize(HWND hWindow)
{
	RECT clientRect;
	GetClientRect(hWindow, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;
}

