
#include "yoshix_fix_function.h"

#include <math.h>

using namespace gfx;

namespace
{
    class CApplication : public IApplication
    {
        public:

            CApplication();
            virtual ~CApplication();

        private:

            struct SPlanet
            {
                float   m_Translation[3];
                float   m_TranslationDelta[3];
                float   m_Rotation[3];
                float   m_RotationDelta[3];
                float   m_Radius;
                float   m_Distance;
                float   m_LocalMatrix[16];
                float   m_GlobalMatrix[16];
                BHandle m_pMesh;
                BHandle m_pTexture;
            };

            struct SCamera
            {
                float   m_Eye[3];
                float   m_EyeDelta[3];
                float   m_At[3];
                float   m_AtDelta[3];
                float   m_Up[3];
                float   m_UpDelta[3];
                float   m_Rotation[3];
                float   m_RotationDelta[3];
                float   m_Distance;
            };

        private:

            float   m_FieldOfViewY;     // Vertical view angle of the camera

            SCamera m_Camera;

            SPlanet m_Earth;
            SPlanet m_Moon;

        private:

            void CreateSphere(BHandle* _ppMesh, BHandle _pTexture, float _Radius);

        private:

            virtual bool InternOnCreateTextures();
            virtual bool InternOnReleaseTextures();
            virtual bool InternOnCreateMeshes();
            virtual bool InternOnReleaseMeshes();
            virtual bool InternOnResize(int _Width, int _Height);
            virtual bool InternOnUpdate();
            virtual bool InternOnFrame();
    };
} // namespace

float g_SpeedUp = 10.0f;

namespace
{
    CApplication::CApplication()
        : m_FieldOfViewY (60.0f)    // Set the vertical view angle of the camera to 60 degrees.
        , m_Camera{}
        , m_Earth{}
        , m_Moon{}
    {
        // ----------------------------
        // CAMERA
        // ----------------------------
        m_Camera.m_Eye[0]      = 0.0f;
        m_Camera.m_Eye[1]      = 0.0f;
        m_Camera.m_Eye[2]      = 0.0f;

        m_Camera.m_EyeDelta[0] = 0.0f * g_SpeedUp;
        m_Camera.m_EyeDelta[1] = 0.0f * g_SpeedUp;
        m_Camera.m_EyeDelta[2] = 0.0f * g_SpeedUp;

        m_Camera.m_At[0]       = 0.0f;
        m_Camera.m_At[1]       = 0.0f;
        m_Camera.m_At[2]       = 0.0f;

        m_Camera.m_AtDelta[0]  = 0.0f * g_SpeedUp;
        m_Camera.m_AtDelta[1]  = 0.0f * g_SpeedUp;
        m_Camera.m_AtDelta[2]  = 0.0f * g_SpeedUp;

        m_Camera.m_Up[0]       = 0.0f;
        m_Camera.m_Up[1]       = 1.0f;
        m_Camera.m_Up[2]       = 0.0f;

        m_Camera.m_UpDelta[0]  = 0.0f * g_SpeedUp;
        m_Camera.m_UpDelta[1]  = 0.0f * g_SpeedUp;
        m_Camera.m_UpDelta[2]  = 0.0f * g_SpeedUp;

        m_Camera.m_Rotation[0] = 0.0f;
        m_Camera.m_Rotation[1] = 0.0f;
        m_Camera.m_Rotation[2] = 0.0f;

        m_Camera.m_RotationDelta[0] = 0.0f * g_SpeedUp;
        m_Camera.m_RotationDelta[1] = 0.02f * g_SpeedUp;
        m_Camera.m_RotationDelta[2] = 0.0f * g_SpeedUp;

        m_Camera.m_Distance = 20.0f;

        // ----------------------------
        // Earth
        // ----------------------------

        m_Earth.m_Translation[0]      = 0.0f;
        m_Earth.m_Translation[1]      = 0.0f;
        m_Earth.m_Translation[2]      = 0.0f;

        m_Earth.m_TranslationDelta[0] = 0.0f * g_SpeedUp;
        m_Earth.m_TranslationDelta[1] = 0.0f * g_SpeedUp;
        m_Earth.m_TranslationDelta[2] = 0.0f * g_SpeedUp;

        m_Earth.m_Rotation[0]         = 0.0f;
        m_Earth.m_Rotation[1]         = 0.0f;
        m_Earth.m_Rotation[2]         = 0.0f;

        m_Earth.m_RotationDelta[0]    = 0.0f * g_SpeedUp;
        m_Earth.m_RotationDelta[1]    = 0.03f * g_SpeedUp;
        m_Earth.m_RotationDelta[2]    = 0.0f * g_SpeedUp;

        m_Earth.m_Radius              = 4.0f;
        m_Earth.m_Distance            = 0.0f;

        m_Earth.m_LocalMatrix[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15] = 0;
        m_Earth.m_GlobalMatrix[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15] = 0.0f;

        m_Earth.m_pMesh              = nullptr;
        m_Earth.m_pTexture           = nullptr;

        // ----------------------------
        // Moon
        // ----------------------------

        m_Moon.m_Translation[0]      = 8.0f;
        m_Moon.m_Translation[1]      = 0.0f;
        m_Moon.m_Translation[2]      = 0.0f;

        m_Moon.m_TranslationDelta[0] = 0.0f * g_SpeedUp;
        m_Moon.m_TranslationDelta[1] = 0.0f * g_SpeedUp;
        m_Moon.m_TranslationDelta[2] = 0.0f * g_SpeedUp;

        m_Moon.m_Rotation[0]         = 0.0f;
        m_Moon.m_Rotation[1]         = 0.0f;
        m_Moon.m_Rotation[2]         = 0.0f;

        m_Moon.m_RotationDelta[0]    = 0.0f * g_SpeedUp;
        m_Moon.m_RotationDelta[1]    = 0.02f * g_SpeedUp;
        m_Moon.m_RotationDelta[2]    = 0.04f * g_SpeedUp;

        m_Moon.m_Radius              = 1.0f;
        m_Moon.m_Distance            = 8.0f;

        m_Moon.m_LocalMatrix[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15] = 0;
        m_Moon.m_GlobalMatrix[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15] = 0.0f;

        m_Moon.m_pMesh               = nullptr;
        m_Moon.m_pTexture            = nullptr;

    }

    // -----------------------------------------------------------------------------

    CApplication::~CApplication()
    {
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnCreateTextures()
    {
        // -----------------------------------------------------------------------------
        // Load an image from the given path and create a YoshiX texture representing
        // the image.
        // -----------------------------------------------------------------------------
        CreateTexture("..\\data\\images\\earth.dds", &m_Earth.m_pTexture);
        CreateTexture("..\\data\\images\\moon.dds" , &m_Moon.m_pTexture);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseTextures()
    {
        // -----------------------------------------------------------------------------
        // Important to release the texture again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseTexture(m_Earth.m_pTexture);
        ReleaseTexture(m_Moon.m_pTexture);

        return true;
    }

    // -----------------------------------------------------------------------------

    void CApplication::CreateSphere(BHandle* _ppMesh, BHandle _pTexture, float _Radius)
    {
        static const float s_Pi                         = 3.1415926535897932384626433832795f;
        static const int   s_Delta                      = 10;
        static const int   s_NumberOfVerticalVertices   = 180 / s_Delta + 1;
        static const int   s_NumberOfHorizontalVertices = 360 / s_Delta + 1;
        static const int   s_NumberOfVertices           = s_NumberOfVerticalVertices * s_NumberOfHorizontalVertices;
        static const int   s_NumberOfIndices            = s_NumberOfVertices * 2 * 3;

        int   IndexOfVertex;
        int   IndexOfIndex;

        float RadiusOfSphere;
        float CenterOfSphere[3];
        float RadiusOfHorizontalCircle;
        float CenterOfHorizontalCircle[3];
        float FirstVertexOfHorizontalCircle[3];
        float Distance[3];

        int   Indices  [s_NumberOfIndices];
        float Vertices [s_NumberOfVertices * 3];
        float TexCoords[s_NumberOfVertices * 2];

        RadiusOfSphere = _Radius;

        CenterOfSphere[0] = 0.0f;
        CenterOfSphere[1] = 0.0f;
        CenterOfSphere[2] = 0.0f;

        IndexOfVertex = 0;

        for (float Alpha = 90.0f; Alpha <= 270; Alpha += s_Delta)
        {
            FirstVertexOfHorizontalCircle[0] = CenterOfSphere[0] + RadiusOfSphere * cos(s_Pi * Alpha / 180.0f);
            FirstVertexOfHorizontalCircle[1] = CenterOfSphere[1] + RadiusOfSphere * sin(s_Pi * Alpha / 180.0f);
            FirstVertexOfHorizontalCircle[2] = CenterOfSphere[2];

            CenterOfHorizontalCircle[0] = CenterOfSphere               [0];
            CenterOfHorizontalCircle[1] = FirstVertexOfHorizontalCircle[1];
            CenterOfHorizontalCircle[2] = CenterOfSphere               [2];

            Distance[0] = FirstVertexOfHorizontalCircle[0] - CenterOfHorizontalCircle[0];
            Distance[1] = FirstVertexOfHorizontalCircle[1] - CenterOfHorizontalCircle[1];
            Distance[2] = FirstVertexOfHorizontalCircle[2] - CenterOfHorizontalCircle[2];

            RadiusOfHorizontalCircle = sqrtf(Distance[0] * Distance[0] + Distance[1] * Distance[1] + Distance[2] * Distance[2]);

            for (float Gamma = 0.0f; Gamma <= 360; Gamma += s_Delta)
            {
                Vertices[IndexOfVertex * 3 + 0] = CenterOfHorizontalCircle[0] + RadiusOfHorizontalCircle * cos(s_Pi * Gamma / 180.0f);
                Vertices[IndexOfVertex * 3 + 1] = CenterOfHorizontalCircle[1];
                Vertices[IndexOfVertex * 3 + 2] = CenterOfHorizontalCircle[2] + RadiusOfHorizontalCircle * sin(s_Pi * Gamma / 180.0f);

                TexCoords[IndexOfVertex * 2 + 0] =  Gamma / 360.0f;
                TexCoords[IndexOfVertex * 2 + 1] = (Alpha -  90.0f) / 180.0f;

                ++ IndexOfVertex;
            }
        }

        IndexOfIndex = 0;

        for (int IndexOfCircle = 0; IndexOfCircle < s_NumberOfVerticalVertices; ++ IndexOfCircle)
        {
            int FirstIndexOfCircle = IndexOfCircle * s_NumberOfHorizontalVertices;

            for (int IndexOfTriangle = 0; IndexOfTriangle < s_NumberOfHorizontalVertices; ++ IndexOfTriangle)
            {
                int UpperLeft  = FirstIndexOfCircle + 0                            + IndexOfTriangle;
                int UpperRight = FirstIndexOfCircle + 0                            + IndexOfTriangle + 1;
                int LowerLeft  = FirstIndexOfCircle + s_NumberOfHorizontalVertices + IndexOfTriangle;
                int LowerRight = FirstIndexOfCircle + s_NumberOfHorizontalVertices + IndexOfTriangle + 1;

                Indices[IndexOfIndex + 0] = LowerLeft;
                Indices[IndexOfIndex + 1] = LowerRight;
                Indices[IndexOfIndex + 2] = UpperRight;

                Indices[IndexOfIndex + 3] = LowerLeft;
                Indices[IndexOfIndex + 4] = UpperRight;
                Indices[IndexOfIndex + 5] = UpperLeft;

                IndexOfIndex += 6;
            }
        }

        // -----------------------------------------------------------------------------
        // Define the mesh and its material. The material defines the look of the
        // surface covering the mesh. If the material should contain normals, colors, or
        // texture coordinates then their number has to match the number of vertices.
        // If you do not support normals in a mesh, YoshiX will not apply lighting to
        // this mesh.
        // -----------------------------------------------------------------------------
        SMeshInfo MeshInfo;

        MeshInfo.m_pVertices        = Vertices;
        MeshInfo.m_pNormals         = nullptr;
        MeshInfo.m_pColors          = nullptr;                      // No colors.
        MeshInfo.m_pTexCoords       = TexCoords;
        MeshInfo.m_NumberOfVertices = s_NumberOfVertices;
        MeshInfo.m_NumberOfIndices  = s_NumberOfIndices;
        MeshInfo.m_pIndices         = Indices;
        MeshInfo.m_pTexture         = _pTexture;

        CreateMesh(MeshInfo, _ppMesh);
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnCreateMeshes()
    {
        CreateSphere(&m_Earth.m_pMesh, m_Earth.m_pTexture, 4);
        CreateSphere(&m_Moon.m_pMesh , m_Moon.m_pTexture , 1);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseMeshes()
    {
        // -----------------------------------------------------------------------------
        // Important to release the mesh again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseMesh(m_Earth.m_pMesh);
        ReleaseMesh(m_Moon.m_pMesh);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnResize(int _Width, int _Height)
    {
        float ProjectionMatrix[16];

        // -----------------------------------------------------------------------------
        // The projection matrix defines the size of the camera frustum. The YoshiX
        // camera has the shape of a pyramid with the eye position at the top of the
        // pyramid. The horizontal view angle is defined by the vertical view angle
        // and the ratio between window width and window height.
        // -----------------------------------------------------------------------------
        GetProjectionMatrix(m_FieldOfViewY, static_cast<float>(_Width) / static_cast<float>(_Height), 0.1f, 100.0f, ProjectionMatrix);

        SetProjectionMatrix(ProjectionMatrix);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnUpdate()
    {
        static const float s_Pi = 3.1415926535897932384626433832795f;

        float ViewMatrix[16];

        // -----------------------------------------------------------------------------
        // Define position and orientation of the camera in the world.
        // -----------------------------------------------------------------------------
        
        // -----------------------------------------------------------------------------
        // Rotate the camera position around the y-axis.
        // -----------------------------------------------------------------------------
        m_Camera.m_Eye[0] = m_Camera.m_Distance * cos(m_Camera.m_Rotation[1] * s_Pi / 180.0f);
        m_Camera.m_Eye[2] = m_Camera.m_Distance * sin(m_Camera.m_Rotation[1] * s_Pi / 180.0f);

        m_Camera.m_Rotation[1] = fmodf(m_Camera.m_Rotation[1] + m_Camera.m_RotationDelta[1], 360.0f);

        GetViewMatrix(&m_Camera.m_Eye[0], &m_Camera.m_At[0], &m_Camera.m_Up[0], ViewMatrix);

        SetViewMatrix(ViewMatrix);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnFrame()
    {
        float WorldMatrix[16];
        float RotationMatrix[16];
        float TranslationMatrix[16];
        float InverseTranslationMatrix[16];
        float TmpMatrix[16];

        // -----------------------------------------------------------------------------
        // Set the position of the mesh in the world and draw it.
        // -----------------------------------------------------------------------------

        // Rotation Earth
        GetTranslationMatrix(
            m_Earth.m_Translation[0], 
            m_Earth.m_Translation[1],
            m_Earth.m_Translation[2],
            TranslationMatrix
        );
        GetRotationYMatrix(m_Earth.m_Rotation[1], RotationMatrix);
        MulMatrix(TranslationMatrix, RotationMatrix, WorldMatrix);
        // Render Earth
        SetWorldMatrix(WorldMatrix);
        DrawMesh(m_Earth.m_pMesh);

        // Rotation Moon
        GetTranslationMatrix(
            m_Moon.m_Translation[0], 
            m_Moon.m_Translation[1], 
            m_Moon.m_Translation[2], 
            TranslationMatrix);
        GetRotationYMatrix(m_Moon.m_Rotation[1], RotationMatrix);
        MulMatrix(RotationMatrix, TranslationMatrix, WorldMatrix);
        // Circulation Moon
        GetRotationZMatrix(m_Moon.m_Rotation[2], RotationMatrix);
        MulMatrix(WorldMatrix, RotationMatrix, WorldMatrix);
        // Render Moon
        SetWorldMatrix(WorldMatrix);
        DrawMesh(m_Moon.m_pMesh);

        // Add Rotationspeed Earth
        m_Earth.m_Rotation[1] += m_Earth.m_RotationDelta[1];

        // Add Rotationspeed Moon
        m_Moon.m_Rotation[1]  += m_Moon.m_RotationDelta[1];
        m_Moon.m_Rotation[2]  += m_Moon.m_RotationDelta[2];
        
        return true;
    }
} // namespace

void main()
{
    CApplication Application;

    RunApplication(800, 600, "YoshiX Example", &Application);
}