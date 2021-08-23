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
    //各画面分のシャドウマップを描画
    for (int screenNo = 0; screenNo < 2; screenNo++)
    {
        //ライトカメラのビュープロジェクション行列を取得
        const Matrix& lvpMatrix = m_lightCamera[screenNo].GetViewProjectionMatrix();

        //画面のカメラの向きを取得
        const Vector3& cameraForward = g_camera3D[screenNo]->GetForward();
        const Vector3& cameraRight = g_camera3D[screenNo]->GetRight();
        Vector3 cameraUp;
        cameraUp.Cross(cameraForward, cameraRight);

        //最初のエリア(近距離)の最小深度はカメラの近平面。
        float nearDepth = g_camera3D[screenNo]->GetNear();

        //エリアごとにシャドウマップを描画
        for (int areaNo = 0; areaNo < 3; areaNo++)
        {
            //近い側の平面の中心から上端までの長さ
            float nearY = tanf(g_camera3D[screenNo]->GetViewAngle() * 0.5f) * nearDepth;
            //近い側の平面の中心から両端までの長さ
            float nearX = nearY * g_camera3D[screenNo]->GetAspect();
            //遠い側の平面の中心から上端までの長さ
            float farY = tanf(g_camera3D[screenNo]->GetViewAngle() * 0.5f) * m_areaRangeTable[areaNo];
            //遠い側の平面の中心から両端までの長さ
            float farX = farY * g_camera3D[screenNo]->GetAspect();

            //近い側の平面の中心
            Vector3 nearPos = g_camera3D[screenNo]->GetPosition() + cameraForward * nearDepth;
            //遠い側の平面の中心
            Vector3 farPos = g_camera3D[screenNo]->GetPosition() + cameraForward * m_areaRangeTable[areaNo];

            //エリアの8頂点を求めていく
            Vector3 vertex[8];

            //近い側の平面
            //右上
            vertex[0] += nearPos + cameraUp * nearY + cameraRight * nearX;
            //左上
            vertex[1] += nearPos + cameraUp * nearY + cameraRight * -nearX;
            //右下
            vertex[2] += nearPos + cameraUp * -nearY + cameraRight * nearX;
            //左下
            vertex[3] += nearPos + cameraUp * -nearY + cameraRight * -nearX;

            //遠い側の平面
            //右上
            vertex[4] += farPos + cameraUp * farY + cameraRight * farX;
            //左上
            vertex[5] += farPos + cameraUp * farY + cameraRight * -farX;
            //右下
            vertex[6] += farPos + cameraUp * -farY + cameraRight * farX;
            //左下
            vertex[7] += farPos + cameraUp * -farY + cameraRight * -farX;

            //8頂点の中での最大座標、最小座標を求める(最大座標、最小座標が入るように切り抜くため)
            Vector3 vMax, vMin;
            vMax = { -FLT_MAX,-FLT_MAX,-FLT_MAX };
            vMin = { FLT_MAX,FLT_MAX,FLT_MAX };

            for (Vector3& v : vertex)
            {
                lvpMatrix.Apply(v);
                vMax.Max(v);
                vMin.Min(v);
            }

            //最大座標、最小座標が収まるようなクロップ行列を作成する
            float xScale = 2.0f / (vMax.x - vMin.x);
            float yScale = 2.0f / (vMax.y - vMin.y);
            float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
            float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;

            Matrix clopMatrix;
            clopMatrix.m[0][0] = xScale;
            clopMatrix.m[1][1] = yScale;
            clopMatrix.m[3][0] = xOffset;
            clopMatrix.m[3][1] = yOffset;

            //ライトビュープロジェクション行列にクロップ行列を乗算する
            m_lvpcMatrix[screenNo][areaNo] = lvpMatrix * clopMatrix;



            //シャドウマップを描画
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

            //影モデルのドロー
            GameObjectManager::GetInstance()->ExecuteShadowRender(rc, g_matIdentity, m_lvpcMatrix[screenNo][areaNo],(int)screenNo,(int)areaNo);

            rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMaps[screenNo][areaNo]);


            // 次のエリアの近平面までの距離を代入する
            nearDepth = m_areaRangeTable[areaNo];
        }
    }
}