#pragma once

// ��ü Ŭ���� ��ũ��
#define DECLARE_CLASS(ClassName, ParentClassName) \
public: \
    static UClass* StaticClass() { \
        static UClass ClassInfo(#ClassName, ParentClassName::StaticClass()); \
        return &ClassInfo; \
    } \
    virtual UClass* GetClass() const override { return StaticClass(); }