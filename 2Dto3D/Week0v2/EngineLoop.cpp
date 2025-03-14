#include "EngineLoop.h"
#include "ImGuiManager.h"
#include "World.h"
#include "CameraComponent.h"
#include "JungleMath.h"
#include "ControlPaner.h"
#include "PropertyPanel.h"
#include "Outliner.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:

		if (wParam != SIZE_MINIMIZED) {
			//UGraphicsDevice ��ü�� OnResize �Լ� ȣ��
			if (FEngineLoop::graphicDevice.SwapChain) {
				FEngineLoop::graphicDevice.OnResize(hWnd);
			}
		}
		Console::GetInstance().OnResize(hWnd);
		ControlPanel::GetInstance().OnResize(hWnd);
		PropertyPanel::GetInstance().OnResize(hWnd);
		Outliner::GetInstance().OnResize(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

FGraphicsDevice FEngineLoop::graphicDevice;
FRenderer FEngineLoop::renderer;
FResourceMgr FEngineLoop::resourceMgr;
uint32 FEngineLoop::TotalAllocationBytes= 0;
uint32 FEngineLoop::TotalAllocationCount = 0;
FEngineLoop::FEngineLoop()
{
}

FEngineLoop::~FEngineLoop()
{
}

int32 FEngineLoop::PreInit()
{
	return 0;
}

int32 FEngineLoop::Init(HINSTANCE hInstance)
{
	WindowInit(hInstance);
	graphicDevice.Initialize(hWnd);
	renderer.Initialize(&graphicDevice);

	UIMgr = new UImGuiManager;
	UIMgr->Initialize(hWnd,graphicDevice.Device, graphicDevice.DeviceContext);
	resourceMgr.Initialize(&renderer);
	GWorld = new UWorld;
	GWorld->Initialize();
	return 0;
}



void FEngineLoop::Tick()
{
	LARGE_INTEGER frequency;
	const double targetFrameTime = 1000.0 / targetFPS; // �� �������� ��ǥ �ð� (�и��� ����)

	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER startTime, endTime;
	double elapsedTime = 1.0;

	while (bIsExit == false)
	{
		QueryPerformanceCounter(&startTime);

		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // Ű���� �Է� �޽����� ���ڸ޽����� ����
			DispatchMessage(&msg); // �޽����� WndProc�� ����

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}
		GWorld->Tick(elapsedTime);

		UCameraComponent* Camera = static_cast<UCameraComponent*>(GWorld->GetCamera());
		View = JungleMath::CreateViewMatrix(GWorld->GetCamera()->GetWorldLocation(),
			GWorld->GetCamera()->GetWorldLocation() + GWorld->GetCamera()->GetForwardVector(),
			{ 0, 0, 1 });
		Projection = JungleMath::CreateProjectionMatrix(
			Camera->GetFov() * (3.141592f / 180.0f),
			GetAspectRatio(graphicDevice.SwapChain), 
			0.1f,
			1000.0f
		);

		graphicDevice.Prepare();
		renderer.PrepareShader();

		//GWorld->Render();
		Render();

		UIMgr->BeginFrame();

		Console::GetInstance().Draw();
		ControlPanel::GetInstance().Draw(GetWorld(),elapsedTime);
		PropertyPanel::GetInstance().Draw(GetWorld());
		Outliner::GetInstance().Draw(GetWorld());
		UIMgr->EndFrame();

		GWorld->CleanUp();

		graphicDevice.SwapBuffer();
		do
		{
			Sleep(0);
			QueryPerformanceCounter(&endTime);
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
		} while (elapsedTime < targetFrameTime);
	}
}
float a = 5;
void FEngineLoop::Render()
{
	

	UPrimitiveBatch::GetInstance().Begin();
	UPrimitiveBatch::GetInstance().AddGrid(50);
	UPrimitiveBatch::GetInstance().AddWorldGizmo();
	GWorld->Render();
	UPrimitiveBatch::GetInstance().End(View, Projection);
	GWorld->RenderBaseObject();
}
float FEngineLoop::GetAspectRatio(IDXGISwapChain* swapChain)
{
	DXGI_SWAP_CHAIN_DESC desc;
	swapChain->GetDesc(&desc);
	return static_cast<float>(desc.BufferDesc.Width) / static_cast<float>(desc.BufferDesc.Height);
}

void FEngineLoop::Exit()
{
	GWorld->Release();
	delete GWorld;
	UIMgr->Shutdown();
	delete UIMgr;
	resourceMgr.Release(&renderer);
	renderer.Release();
	graphicDevice.Release();
}

void FEngineLoop::WindowInit(HINSTANCE hInstance)
{
	WCHAR WindowClass[] = L"JungleWindowClass";

	WCHAR Title[] = L"Game Tech Lab";

	WNDCLASSW wndclass = { 0 };
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = WindowClass;

	RegisterClassW(&wndclass);

	hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1500, 1500,
		nullptr, nullptr, hInstance, nullptr);
}

