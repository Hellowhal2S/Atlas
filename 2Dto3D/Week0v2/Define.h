#pragma once
#include <cmath>
#include <algorithm>

// �⺻ Ÿ�� 
#include "CoreTypes.h"


// ���� ����
#include "MathDefines.h"
#include "Vector.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Quat.h"

// ��ü ��ũ�� ���� 
#include "ObjectMacros.h"
#include "ObjectTypes.h"


#pragma once


#define UE_LOG Console::GetInstance().AddLog
#define Super __super


#include "Console.h"
#include "NameTypes.h"

struct FVertexSimple
{
	float x, y, z;    // Position
	float r, g, b, a; // Color
	float nx, ny, nz;
};

struct FVertexTexture
{
	float x, y, z;    // Position
	float u, v; // Texture
};
struct FLighting
{
	float lightDirX, lightDirY, lightDirZ; // ���� ����
	float pad1;                      // 16����Ʈ ���Ŀ� �е�
	float lightColorX, lightColorY, lightColorZ;    // ���� ����
	float pad2;                      // 16����Ʈ ���Ŀ� �е�
	float AmbientFactor;             // ambient ���
	float pad3; // 16����Ʈ ���� ���� �߰� �е�
	float pad4; // 16����Ʈ ���� ���� �߰� �е�
	float pad5; // 16����Ʈ ���� ���� �߰� �е�
};



