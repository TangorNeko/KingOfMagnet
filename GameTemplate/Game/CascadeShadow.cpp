#include "stdafx.h"
#include "CascadeShadow.h"

CascadeShadow* CascadeShadow::m_instance = nullptr;

namespace
{
    const float RANGE_SHORT = 500.0f;
    const float RANGE_MEDIUM = 2000.0f;
    const float RANGE_FAR = 10000.0f;
}

void CascadeShadow::Init()
{
    Vector3 Dir = { -1.0f,-1.0f,1.0f };
    Dir.Normalize();
    Vector3 Pos1 = { 760.0f,600.0f,400.0f };
    Vector3 Target = Pos1 + Dir * 600.0f;
    m_lightCamera[0].SetPosition(Pos1);
    m_lightCamera[0].SetTarget(Target);
    m_lightCamera[0].SetUp({ 1.0f,0.0f,0.0f });
    m_lightCamera[0].SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
    m_lightCamera[0].SetWidth(5000.0f);
    m_lightCamera[0].SetHeight(5000.0f);
    m_lightCamera[0].SetNear(1.0f);
    m_lightCamera[0].SetFar(RANGE_FAR);
    m_lightCamera[0].Update();

    m_lightCamera[1].SetPosition({ -760.0f,600.0f,-400.0f });
    m_lightCamera[1].SetTarget({ -760.0f,0.0f,-400.0f });
    m_lightCamera[1].SetUp({ 1.0f,0.0f,0.0f });
    m_lightCamera[1].SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
    m_lightCamera[1].SetWidth(5000.0f);
    m_lightCamera[1].SetHeight(5000.0f);
    m_lightCamera[1].SetNear(1.0f);
    m_lightCamera[1].SetFar(RANGE_FAR);
    m_lightCamera[1].Update();

    m_areaRangeTable[0] = RANGE_SHORT;
    m_areaRangeTable[1] = RANGE_MEDIUM;
    m_areaRangeTable[2] = RANGE_FAR;



    float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

    for (int screenNo = 0; screenNo < 2; screenNo++)
    {
        m_shadowMaps[screenNo][enShort].Create(
            2048,
            2048,
            1,
            1,
            DXGI_FORMAT_R32_FLOAT,
            DXGI_FORMAT_D32_FLOAT,
            clearColor
        );

        m_shadowMaps[screenNo][enMedium].Create(
            1024,
            1024,
            1,
            1,
            DXGI_FORMAT_R32_FLOAT,
            DXGI_FORMAT_D32_FLOAT,
            clearColor
        );

        m_shadowMaps[screenNo][enLong].Create(
            512,
            512,
            1,
            1,
            DXGI_FORMAT_R32_FLOAT,
            DXGI_FORMAT_D32_FLOAT,
            clearColor
        );
    }
}

void CascadeShadow::DrawShadowMap()
{
    //�e��ʕ��̃V���h�E�}�b�v��`��
    for (int screenNo = 0; screenNo < 2; screenNo++)
    {
        //���C�g�J�����̃r���[�v���W�F�N�V�����s����擾
        const Matrix& lvpMatrix = m_lightCamera[screenNo].GetViewProjectionMatrix();

        //��ʂ̃J�����̌������擾
        const Vector3& cameraForward = g_camera3D[screenNo]->GetForward();
        const Vector3& cameraRight = g_camera3D[screenNo]->GetRight();
        Vector3 cameraUp;
        cameraUp.Cross(cameraForward, cameraRight);

        //�ŏ��̃G���A(�ߋ���)�̍ŏ��[�x�̓J�����̋ߕ��ʁB
        float nearDepth = g_camera3D[screenNo]->GetNear();

        //�G���A���ƂɃV���h�E�}�b�v��`��
        for (int areaNo = 0; areaNo < 3; areaNo++)
        {
            //�߂����̕��ʂ̒��S�����[�܂ł̒���
            float nearY = tanf(g_camera3D[screenNo]->GetViewAngle() * 0.5f) * nearDepth;
            //�߂����̕��ʂ̒��S���痼�[�܂ł̒���
            float nearX = nearY * g_camera3D[screenNo]->GetAspect();
            //�������̕��ʂ̒��S�����[�܂ł̒���
            float farY = tanf(g_camera3D[screenNo]->GetViewAngle() * 0.5f) * m_areaRangeTable[areaNo];
            //�������̕��ʂ̒��S���痼�[�܂ł̒���
            float farX = farY * g_camera3D[screenNo]->GetAspect();

            //�߂����̕��ʂ̒��S
            Vector3 nearPos = g_camera3D[screenNo]->GetPosition() + cameraForward * nearDepth;
            //�������̕��ʂ̒��S
            Vector3 farPos = g_camera3D[screenNo]->GetPosition() + cameraForward * m_areaRangeTable[areaNo];

            //�G���A��8���_�����߂Ă���
            Vector3 vertex[8];

            //�߂����̕���
            //�E��
            vertex[0] += nearPos + cameraUp * nearY + cameraRight * nearX;
            //����
            vertex[1] += nearPos + cameraUp * nearY + cameraRight * -nearX;
            //�E��
            vertex[2] += nearPos + cameraUp * -nearY + cameraRight * nearX;
            //����
            vertex[3] += nearPos + cameraUp * -nearY + cameraRight * -nearX;

            //�������̕���
            //�E��
            vertex[4] += farPos + cameraUp * farY + cameraRight * farX;
            //����
            vertex[5] += farPos + cameraUp * farY + cameraRight * -farX;
            //�E��
            vertex[6] += farPos + cameraUp * -farY + cameraRight * farX;
            //����
            vertex[7] += farPos + cameraUp * -farY + cameraRight * -farX;

            //8���_�̒��ł̍ő���W�A�ŏ����W�����߂�(�ő���W�A�ŏ����W������悤�ɐ؂蔲������)
            Vector3 vMax, vMin;
            vMax = { -FLT_MAX,-FLT_MAX,-FLT_MAX };
            vMin = { FLT_MAX,FLT_MAX,FLT_MAX };

            for (Vector3& v : vertex)
            {
                lvpMatrix.Apply(v);
                vMax.Max(v);
                vMin.Min(v);
            }

            //�ő���W�A�ŏ����W�����܂�悤�ȃN���b�v�s����쐬����
            float xScale = 2.0f / (vMax.x - vMin.x);
            float yScale = 2.0f / (vMax.y - vMin.y);
            float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
            float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;

            Matrix clopMatrix;
            clopMatrix.m[0][0] = xScale;
            clopMatrix.m[1][1] = yScale;
            clopMatrix.m[3][0] = xOffset;
            clopMatrix.m[3][1] = yOffset;

            //���C�g�r���[�v���W�F�N�V�����s��ɃN���b�v�s�����Z����
            m_lvpcMatrix[screenNo][areaNo] = lvpMatrix * clopMatrix;



            //�V���h�E�}�b�v��`��
            RenderContext& rc = g_graphicsEngine->GetRenderContext();
            rc.WaitUntilToPossibleSetRenderTarget(m_shadowMaps[screenNo][areaNo]);
            rc.SetRenderTargetAndViewport(m_shadowMaps[screenNo][areaNo]);
            rc.ClearRenderTargetView(m_shadowMaps[screenNo][areaNo].GetRTVCpuDescriptorHandle(), m_shadowMaps[screenNo][areaNo].GetRTVClearColor());
            rc.ClearDepthStencilView(m_shadowMaps[screenNo][areaNo].GetDSVCpuDescriptorHandle(), m_shadowMaps[screenNo][areaNo].GetDSVClearValue());
            D3D12_RECT shadowRect;
            shadowRect.left = 0;
            shadowRect.top = 0;
            shadowRect.right = 2048;
            shadowRect.bottom = 2048;
            rc.SetScissorRect(shadowRect);

            //�e���f���̃h���[
            GameObjectManager::GetInstance()->ExecuteShadowRender(rc, g_matIdentity, m_lvpcMatrix[screenNo][areaNo],(int)screenNo,(int)areaNo);

            rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMaps[screenNo][areaNo]);


            // ���̃G���A�̋ߕ��ʂ܂ł̋�����������
            nearDepth = m_areaRangeTable[areaNo];
        }
    }
}