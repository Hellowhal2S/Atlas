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
    float u=0, v=0;
};

struct FVertexTexture
{
	float x, y, z;    // Position
	float u, v; // Texture
};
struct FGridParameters
{
	float gridSpacing;
	int   numGridLines;
	FVector gridOrigin;
	float pad;
};
struct FSimpleVertex
{
	float dummy; // ������ ������ ����
    float padding[11];
};
struct FOBB {
    FVector corners[8];
};
struct FBoundingBox
{
	FVector min; // Minimum extents
	float pad;
	FVector max; // Maximum extents
	float pad1;
    bool Intersect(const FVector& rayOrigin, const FVector& rayDir, float& outDistance)
    {
        float tmin = -FLT_MAX;
        float tmax = FLT_MAX;
        const float epsilon = 1e-6f;

        // X�� ó��
        if (fabs(rayDir.x) < epsilon)
        {
            // ���̰� X�� �������� ���� ������ ���,
            // ������ x�� �ڽ� [min.x, max.x] ���� ���̸� ���� ����
            if (rayOrigin.x < min.x || rayOrigin.x > max.x)
                return false;
        }
        else
        {
            float t1 = (min.x - rayOrigin.x) / rayDir.x;
            float t2 = (max.x - rayOrigin.x) / rayDir.x;
            if (t1 > t2)  std::swap(t1, t2);

            // tmin�� "��������� ���� ���� �� ���� ū min"
            tmin = (t1 > tmin) ? t1 : tmin;
            // tmax�� "��������� ���� ���� �� ���� ���� max"
            tmax = (t2 < tmax) ? t2 : tmax;
            if (tmin > tmax)
                return false;
        }

        // Y�� ó��
        if (fabs(rayDir.y) < epsilon)
        {
            if (rayOrigin.y < min.y || rayOrigin.y > max.y)
                return false;
        }
        else
        {
            float t1 = (min.y - rayOrigin.y) / rayDir.y;
            float t2 = (max.y - rayOrigin.y) / rayDir.y;
            if (t1 > t2)  std::swap(t1, t2);

            tmin = (t1 > tmin) ? t1 : tmin;
            tmax = (t2 < tmax) ? t2 : tmax;
            if (tmin > tmax)
                return false;
        }

        // Z�� ó��
        if (fabs(rayDir.z) < epsilon)
        {
            if (rayOrigin.z < min.z || rayOrigin.z > max.z)
                return false;
        }
        else
        {
            float t1 = (min.z - rayOrigin.z) / rayDir.z;
            float t2 = (max.z - rayOrigin.z) / rayDir.z;
            if (t1 > t2)  std::swap(t1, t2);

            tmin = (t1 > tmin) ? t1 : tmin;
            tmax = (t2 < tmax) ? t2 : tmax;
            if (tmin > tmax)
                return false;
        }

        // ������� ������ ���� ���� [tmin, tmax]�� ��ȿ�ϴ�.
        // tmax < 0 �̸�, ���̰� �ڽ� ���ʿ��� �����ϹǷ� ȭ��� ���⿣ ���� �� �Ѵٰ� �� �� ����
        if (tmax < 0.0f)
            return false;

        // outDistance = tmin�� 0���� ũ�� �װ� ���̰� ó������ �ڽ��� ������ ����
        // ���� tmin < 0 �̸�, ������ �������� �ڽ� ���ο� �ִٴ� �ǹ��̹Ƿ�, �Ÿ��� 0���� ó���ص� ��.
        outDistance = (tmin >= 0.0f) ? tmin : 0.0f;

        return true;
    }

};
struct FCone
{
    FVector ConeApex; // ������ ������
    float ConeRadius; // ���� �ظ� ������

    FVector ConeBaseCenter; // ���� �ظ� �߽�
    float ConeHeight; // ���� ���� (Apex�� BaseCenter �� ����)
    FVector4 Color;

    int ConeSegmentCount; // ���� �ظ� ���� ��
    float pad[3];

};
struct FPrimitiveCounts 
{
	int BoundingBoxCount;
	int pad;
	int ConeCount; 
	int pad1;
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



