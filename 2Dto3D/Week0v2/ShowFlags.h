#pragma once
#include "Define.h"
#include <Windows.h>
#include "IWindowToggleable.h"
#include <cstdint>
enum class EEngineShowFlags : uint64
{
	None = 0,
	SF_AABB = 1ULL << 0,
	SF_Primitives = 1ULL << 1,
	SF_BillboardText = 1ULL << 2,
	SF_UUIDText = 1ULL << 3
};


class UWorld;
class ShowFlags : public IWindowToggleable
{
private:
	ShowFlags();

public:
	~ShowFlags();

	static ShowFlags& GetInstance();

	void Draw(UWorld* world);
	uint64 ConvertSelectionToFlags(const bool selected[]);
	void OnResize(HWND hWnd);
	void Toggle() override {
		if (bWasOpen) {
			bWasOpen = false;
		}
	}
	uint64 currentFlags;
private:
	bool bWasOpen = true;
	UINT width;
	UINT height;
};