#pragma once

#include "Define.h"

// �ݽð�
// Arrow vertices (without indices)
FVertexSimple arrow_x_vertices[] = {
   {-0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},
    {0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, -0.1f, 0.7f, 0, 0, 1},
    {-0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, -0.1f, 0.7f, 0, 0, 1},
    {-0.7f, 0.1f, -0.1f, 0.7f, 0, 0, 1},
    {-0.7f, -0.1f, 0.1f, 0.7f, 0, 0, 1},
    {-0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {-0.7f, -0.1f, 0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {0.7f, -0.1f, 0.1f, 0.7f, 0, 0, 1},
    {-0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},
    {-0.7f, 0.1f, -0.1f, 0.7f, 0, 0, 1},
    {-0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {-0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},
    {-0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {-0.7f, -0.1f, 0.1f, 0.7f, 0, 0, 1},
    {0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},
    {0.7f, -0.1f, 0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, -0.1f, 0.7f, 0, 0, 1},
    {-0.7f, 0.1f, -0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, -0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {-0.7f, 0.1f, -0.1f, 0.7f, 0, 0, 1},
    {0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {-0.7f, 0.1f, 0.1f, 0.7f, 0, 0, 1},
    {-0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},
    {-0.7f, -0.1f, 0.1f, 0.7f, 0, 0, 1},
    {0.7f, -0.1f, 0.1f, 0.7f, 0, 0, 1},
    {-0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},
    {0.7f, -0.1f, 0.1f, 0.7f, 0, 0, 1},
    {0.7f, -0.1f, -0.1f, 0.7f, 0, 0, 1},

    // ȭ��ǥ �밡��
    // ȭ��ǥ �밡�� (�� ����)
    {0.7f, -0.2f, -0.2f, 1, 0, 0, 1},
    {0.7f, 0.2f, -0.2f, 1, 0, 0, 1},
    {0.7f, 0.2f, 0.2f, 1, 0, 0, 1},

    {0.7f, -0.2f, -0.2f, 1, 0, 0, 1},
    {0.7f, 0.2f, 0.2f, 1, 0, 0, 1},
    {0.7f, -0.2f, 0.2f, 1, 0, 0, 1},

    // ���� ������ �ϴ� �κ� (�� �� �����ϰ� �����)
    {1.0f, 0.f, 0.f, 1, 0, 0, 1},
    {0.7f, 0.2f, -0.2f, 1, 0, 0, 1},
    {0.7f, -0.2f, -0.2f, 1, 0, 0, 1},

    {1.0f, 0.f, 0.f, 1, 0, 0, 1},
    {0.7f, 0.2f, 0.2f, 1, 0, 0, 1},
    {0.7f, 0.2f, -0.2f, 1, 0, 0, 1},

    {1.0f, 0.f, 0.f, 1, 0, 0, 1},
    {0.7f, -0.2f, 0.2f, 1, 0, 0, 1},
    {0.7f, 0.2f, 0.2f, 1, 0, 0, 1},

    {1.0f, 0.f, 0.f, 1, 0, 0, 1},
    {0.7f, -0.2f, -0.2f, 1, 0, 0, 1},
    {0.7f, -0.2f, 0.2f, 1, 0, 0, 1},
};


// ȭ��ǥ ���ؽ� ������ (��ü: ������ü, �Ӹ�: �簢��, Y���� �ٶ󺸰�)

FVertexSimple arrow_y_vertices[] = {
    // ��ü (������ü) - �� ���� 2���� �ﰢ������ ����
    // �ո� (CCW)
    {-0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},
    {0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},
    {0.1f, -0.7f, 0.1f, 0, 0.7, 0, 1},

    {-0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},
    {0.1f, -0.7f, 0.1f, 0, 0.7, 0, 1},
    {-0.1f, -0.7f, 0.1f, 0, 0.7, 0, 1},

    // �޸� (CW)
    {-0.1f, 0.7f, -0.1f, 0, 0.7, 0, 1},
    {0.1f, 0.7f, -0.1f, 0, 0.7, 0, 1},
    {0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},

    {-0.1f, 0.7f, -0.1f, 0, 0.7, 0, 1},
    {0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},
    {-0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},

    // ���ʸ� (CCW)
    {-0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},
    {-0.1f, -0.7f, 0.1f, 0, 0.7, 0, 1},
    {-0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},

    {-0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},
    {-0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},
    {-0.1f, 0.7f, -0.1f, 0, 0.7, 0, 1},

    // �����ʸ� (CW)
    {0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},
    {0.1f, -0.7f, 0.1f, 0, 0.7, 0, 1},
    {0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},

    {0.1f, 0.7f, -0.1f, 0, 0.7, 0, 1},
    {0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},
    {0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},

    // ���� (CCW)
    {-0.1f, -0.7f, 0.1f, 0, 0.7, 0, 1},
    {0.1f, -0.7f, 0.1f, 0, 0.7, 0, 1},
    {0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},

    {-0.1f, -0.7f, 0.1f, 0, 0.7, 0, 1},
    {0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},
    {-0.1f, 0.7f, 0.1f, 0, 0.7, 0, 1},

    // �Ʒ��� (CW)
    {0.1f, 0.7f, -0.1f, 0, 0.7, 0, 1},
    {0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},
    {-0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},

    {-0.1f, 0.7f, -0.1f, 0, 0.7, 0, 1},
    {0.1f, 0.7f, -0.1f, 0, 0.7, 0, 1},
    {-0.1f, -0.7f, -0.1f, 0, 0.7, 0, 1},

    // ȭ��ǥ �밡�� (�簢��)
    // �簢�� �ﰢ���� (CCW �������� ȸ����Ŵ)

    // �ո� �ﰢ�� (1)
    {0.0f, 1.2f, 0.0f, 0, 1, 0, 1},    // ����� (3��)
    {0.1f, 0.7f, -0.1f, 0, 1, 0, 1},   // �ٴ� (2��)
    {-0.1f, 0.7f, -0.1f, 0, 1, 0, 1},  // �ٴ� (1��)

    // �����ʸ� �ﰢ�� (2)
    {0.0f, 1.2f, 0.0f, 0, 1, 0, 1},    // ����� (4��)
    {0.1f, 0.7f, 0.1f, 0, 1, 0, 1},   // �ٴ� (3��)
    {0.1f, 0.7f, -0.1f, 0, 1, 0, 1},  // �ٴ� (2��)

    // �޸� �ﰢ�� (3)
    {0.0f, 1.2f, 0.0f, 0, 1, 0, 1},    // ����� (5��)
    {-0.1f, 0.7f, 0.1f, 0, 1, 0, 1},  // �ٴ� (4��)
    {0.1f, 0.7f, 0.1f, 0, 1, 0, 1},  // �ٴ� (3��)

    // ���ʸ� �ﰢ�� (4)
    {0.0f, 1.2f, 0.0f, 0, 1, 0, 1},    // ����� (6��)
    {-0.1f, 0.7f, -0.1f, 0, 1, 0, 1},   // �ٴ� (1��)
    {-0.1f, 0.7f, 0.1f, 0, 1, 0, 1}, // �ٴ� (4��)
};

FVertexSimple arrow_z_vertices[] = {
    // ��ü (������ü) - Y�� ���� 90�� ȸ��
    // �ո� (CCW)
    {0.1f, 0.1f, -0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},
    {-0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},

    {-0.1f, 0.1f, -0.7f, 0, 0, 0.7, 1},
    {0.1f, 0.1f, -0.7f, 0, 0, 0.7, 1},
    {-0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},

    // �޸� (CW)
    {0.1f, 0.1f, 0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},
    {-0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},

    {-0.1f, 0.1f, 0.7f, 0, 0, 0.7, 1},
    {0.1f, 0.1f, 0.7f, 0, 0, 0.7, 1},
    {-0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},

    // ���ʸ� (CCW)
    {-0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},
    {-0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},
    {-0.1f, 0.1f, 0.7f, 0, 0, 0.7, 1},

    {-0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},
    {-0.1f, 0.1f, 0.7f, 0, 0, 0.7, 1},
    {-0.1f, 0.1f, -0.7f, 0, 0, 0.7, 1},

    // �����ʸ� (CW)
    {0.1f, 0.1f, 0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},

    {0.1f, 0.1f, -0.7f, 0, 0, 0.7, 1},
    {0.1f, 0.1f, 0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},

    // ���� (CCW) // �ְ� �̻���
    {0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},
    {-0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},

    {-0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},
    {-0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},

    // �Ʒ��� (CCW)
    {-0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},

    {-0.1f, -0.1f, -0.7f, 0, 0, 0.7, 1},
    {0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},
    {-0.1f, -0.1f, 0.7f, 0, 0, 0.7, 1},

    // ȭ��ǥ �밡�� (�簢��)
    // ���� �Ӹ��� Z���� ���ϵ��� �ﰢ���� ����

    // �ո� �ﰢ�� (1)
    {0.0f, 0.0f, 1.2f, 0, 0, 1, 1},    // ����� (3��)
    {0.1f, 0.1f, 0.7f, 0, 0, 1, 1},   // �ٴ� (2��)
    {-0.1f, 0.1f, 0.7f, 0, 0, 1, 1},  // �ٴ� (1��)

    // �����ʸ� �ﰢ�� (2)
    {0.0f, 0.0f, 1.2f, 0, 0, 1, 1},    // ����� (4��)
    {0.1f, -0.1f, 0.7f, 0, 0, 1, 1},   // �ٴ� (3��)
    {0.1f, 0.1f, 0.7f, 0, 0, 1, 1},  // �ٴ� (2��)

    // �޸� �ﰢ�� (3)
    {0.0f, 0.0f, 1.2f, 0, 0, 1, 1},    // ����� (5��)
    {-0.1f, -0.1f, 0.7f, 0, 0, 1, 1},  // �ٴ� (4��)
    {0.1f, -0.1f, 0.7f, 0, 0, 1, 1},  // �ٴ� (3��)

    // ���ʸ� �ﰢ�� (4)
    {0.0f, 0.0f, 1.2f, 0, 0, 1, 1},    // ����� (6��)
    {-0.1f, 0.1f, 0.7f, 0, 0, 1, 1},   // �ٴ� (1��)
    {-0.1f, -0.1f, 0.7f, 0, 0, 1, 1}, // �ٴ� (4��)
};
