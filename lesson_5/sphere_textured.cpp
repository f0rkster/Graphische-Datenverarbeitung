
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

            float   m_FieldOfViewY;     // Vertical view angle of the camera

            BHandle m_pEarthMesh;       // A pointer to a YoshiX mesh, which represents a sphere.
            BHandle m_pEarthTexture;    // A pointer to a YoshiX texture, which is part of the material covering the sphere.

            BHandle m_pMoonMesh;
            BHandle m_pMoonTexture;


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

namespace
{
    CApplication::CApplication()
        : m_FieldOfViewY (60.0f)    // Set the vertical view angle of the camera to 60 degrees.
        , m_pEarthMesh   (nullptr)
        , m_pEarthTexture(nullptr)
        , m_pMoonMesh    (nullptr)
        , m_pMoonTexture (nullptr)
    {
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
        CreateTexture("..\\data\\images\\earth.dds", &m_pEarthTexture);
        CreateTexture("..\\data\\images\\moon.dds" , &m_pMoonTexture);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseTextures()
    {
        // -----------------------------------------------------------------------------
        // Important to release the texture again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseTexture(m_pEarthTexture);
        ReleaseTexture(m_pMoonTexture);

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
        CreateSphere(&m_pEarthMesh, m_pEarthTexture, 4);
        CreateSphere(&m_pMoonMesh , m_pMoonTexture , 1);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseMeshes()
    {
        // -----------------------------------------------------------------------------
        // Important to release the mesh again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseMesh(m_pEarthMesh);
        ReleaseMesh(m_pMoonMesh);

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
        float Eye[3];
        float At [3];
        float Up [3];

        float ViewMatrix[16];

        // -----------------------------------------------------------------------------
        // Define position and orientation of the camera in the world.
        // -----------------------------------------------------------------------------
        Eye[0] =   0.0f; At[0] = 0.0f; Up[0] = 0.0f;
        Eye[1] =   0.0f; At[1] = 0.0f; Up[1] = 1.0f;
        Eye[2] = -20.0f; At[2] = 0.0f; Up[2] = 0.0f;

        GetViewMatrix(Eye, At, Up, ViewMatrix);

        SetViewMatrix(ViewMatrix);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnFrame()
    {
        float WorldMatrix[16];

        // -----------------------------------------------------------------------------
        // Set the position of the mesh in the world and draw it.
        // -----------------------------------------------------------------------------
        GetTranslationMatrix(0.0f, 0.0f, 0.0f, WorldMatrix);

        SetWorldMatrix(WorldMatrix);

        DrawMesh(m_pEarthMesh);

        GetTranslationMatrix(8.0f, 0.0f, 0.0f, WorldMatrix);

        SetWorldMatrix(WorldMatrix);

        DrawMesh(m_pMoonMesh);

        return true;
    }
} // namespace

void main()
{
    CApplication Application;

    RunApplication(800, 600, "YoshiX Example", &Application);
}