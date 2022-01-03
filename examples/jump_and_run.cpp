
#include "yoshix_fix_function.h"

#include <iostream>
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

            float   m_FieldOfViewY;     // Vertical view angle of the camera.

            BHandle m_pCubeMesh;        // A pointer to a YoshiX mesh, which represents a single cube.
            BHandle m_pCubeTexture;     // A pointer to a YoshiX texture, which is part of the material covering the cube.
            BHandle m_pSphereMesh;      // A pointer to a YoshiX mesh, which represents a single sphere.
            BHandle m_pSphereTexture;   // A pointer to a YoshiX texture, which is part of the material covering the sphere.

        private:

            void CreateCube(BHandle* _ppMesh, BHandle _pTexture, float _EdgeLength);
            void CreateSphere(BHandle* _ppMesh, BHandle _pTexture, float _Radius);

        private:

            virtual bool InternOnStartup();
            virtual bool InternOnShutdown();
            virtual bool InternOnCreateTextures();
            virtual bool InternOnReleaseTextures();
            virtual bool InternOnCreateMeshes();
            virtual bool InternOnReleaseMeshes();
            virtual bool InternOnResize(int _Width, int _Height);
            virtual bool InternOnKeyEvent(unsigned int _Key, bool _IsKeyDown, bool _IsAltDown);
            virtual bool InternOnUpdate();
            virtual bool InternOnFrame();
    };
} // namespace

namespace
{
    CApplication::CApplication()
        : m_FieldOfViewY  (60.0f)     // Set the vertical view angle of the camera to 60 degrees.
        , m_pCubeMesh     (nullptr)
        , m_pCubeTexture  (nullptr)
        , m_pSphereMesh   (nullptr)
        , m_pSphereTexture(nullptr)
    {
    }

    // -----------------------------------------------------------------------------

    CApplication::~CApplication()
    {
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnStartup()
    {
        // -----------------------------------------------------------------------------
        // Define the background color of the window. Colors are always 4D tuples,
        // whereas the components of the tuple represent the red, green, blue, and alpha 
        // channel. The alpha channel indicates the transparency of the color. A value
        // of 1 means the color is completely opaque. A value of 0 means the color is
        // completely transparent. The channels red, green, and blue also are values
        // 0..1 with 0 indicating the minimum intensity and 1 the maximum intensity.
        // -----------------------------------------------------------------------------
        float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f, };

        SetClearColor(ClearColor);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnShutdown()
    {
        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnCreateTextures()
    {
        // -----------------------------------------------------------------------------
        // Load an image from the given path and create a YoshiX texture representing
        // the image.
        // -----------------------------------------------------------------------------
        CreateTexture("..\\data\\images\\cube.dds"   , &m_pCubeTexture);
        CreateTexture("..\\data\\images\\sphere.dds" , &m_pSphereTexture);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseTextures()
    {
        // -----------------------------------------------------------------------------
        // Important to release the texture again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseTexture(m_pCubeTexture);
        ReleaseTexture(m_pSphereTexture);

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

    void CApplication::CreateCube(BHandle* _ppMesh, BHandle _pTexture, float _EdgeLength)
    {
        // -----------------------------------------------------------------------------
        // Define the vertices of the mesh and their attributes.
        // -----------------------------------------------------------------------------
        float s_HalfEdgeLength = 0.5f * _EdgeLength;

        float s_CubeVertices[][3] =
        {
            { -s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength, },
            {  s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength, },
            {  s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength, },
            { -s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength, },

            {  s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength, },
            {  s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength, },
            {  s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength, },
            {  s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength, },

            {  s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength, },
            { -s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength, },
            { -s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength, },
            {  s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength, },

            { -s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength, },
            { -s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength, },
            { -s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength, },
            { -s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength, },

            { -s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength, },
            {  s_HalfEdgeLength,  s_HalfEdgeLength, -s_HalfEdgeLength, },
            {  s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength, },
            { -s_HalfEdgeLength,  s_HalfEdgeLength,  s_HalfEdgeLength, },

            { -s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength, },
            {  s_HalfEdgeLength, -s_HalfEdgeLength,  s_HalfEdgeLength, },
            {  s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength, },
            { -s_HalfEdgeLength, -s_HalfEdgeLength, -s_HalfEdgeLength, },
        };

        float s_U[] =
        {
            0.0f / 4.0f,
            1.0f / 4.0f,
            2.0f / 4.0f,
            3.0f / 4.0f,
            4.0f / 4.0f,
        };

        float s_V[] =
        {
            3.0f / 3.0f,
            2.0f / 3.0f,
            1.0f / 3.0f,
            0.0f / 3.0f,
        };

        float s_CubeTexCoords[][2] =
        {
            { s_U[1], s_V[1], },
            { s_U[2], s_V[1], },
            { s_U[2], s_V[2], },
            { s_U[1], s_V[2], },

            { s_U[2], s_V[1], },
            { s_U[3], s_V[1], },
            { s_U[3], s_V[2], },
            { s_U[2], s_V[2], },

            { s_U[3], s_V[1], },
            { s_U[4], s_V[1], },
            { s_U[4], s_V[2], },
            { s_U[3], s_V[2], },

            { s_U[0], s_V[1], },
            { s_U[1], s_V[1], },
            { s_U[1], s_V[2], },
            { s_U[0], s_V[2], },

            { s_U[1], s_V[2], },
            { s_U[2], s_V[2], },
            { s_U[2], s_V[3], },
            { s_U[1], s_V[3], },

            { s_U[1], s_V[0], },
            { s_U[2], s_V[0], },
            { s_U[2], s_V[1], },
            { s_U[1], s_V[1], },
        };

        // -----------------------------------------------------------------------------
        // Define the topology of the mesh via indices. An index addresses a vertex from
        // the array above. Three indices represent one triangle. When defining the 
        // triangles of a mesh imagine that you are standing in front of the triangle 
        // and looking to the center of the triangle. If the mesh represents a closed
        // body such as a cube, your view position has to be outside of the body. Now
        // define the indices of the addressed vertices of the triangle in counter-
        // clockwise order.
        // -----------------------------------------------------------------------------
        int s_CubeIndices[][3] =
        {
            {  0,  1,  2, },
            {  0,  2,  3, },

            {  4,  5,  6, },
            {  4,  6,  7, },

            {  8,  9, 10, },
            {  8, 10, 11, },

            { 12, 13, 14, },
            { 12, 14, 15, },

            { 16, 17, 18, },
            { 16, 18, 19, },

            { 20, 21, 22, },
            { 20, 22, 23, },
        };

        // -----------------------------------------------------------------------------
        // Define the mesh and its material. The material defines the look of the
        // surface covering the mesh. If the material should contain normals, colors, or
        // texture coordinates then their number has to match the number of vertices.
        // If you do not support normals in a mesh, YoshiX will not apply lighting to
        // this mesh. A textured mesh always has to contain texture coordinates and a
        // handle to a texture. A mesh always has to contain vertices and indices.
        // -----------------------------------------------------------------------------
        SMeshInfo MeshInfo;

        MeshInfo.m_pVertices        = &s_CubeVertices[0][0];
        MeshInfo.m_pNormals         = nullptr;                          // No normals
        MeshInfo.m_pColors          = nullptr;                          // No colors
        MeshInfo.m_pTexCoords       = &s_CubeTexCoords[0][0];
        MeshInfo.m_NumberOfVertices = 24;
        MeshInfo.m_NumberOfIndices  = 36;
        MeshInfo.m_pIndices         = &s_CubeIndices[0][0];
        MeshInfo.m_pTexture         = _pTexture;

        CreateMesh(MeshInfo, _ppMesh);
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnCreateMeshes()
    {
        CreateCube  (&m_pCubeMesh  , m_pCubeTexture  , 1.0f);
        CreateSphere(&m_pSphereMesh, m_pSphereTexture, 1.0f);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseMeshes()
    {
        // -----------------------------------------------------------------------------
        // Important to release the mesh again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseMesh(m_pCubeMesh);
        ReleaseMesh(m_pSphereMesh);

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
        Eye[0] =  0.0f; At[0] = 0.0f; Up[0] = 0.0f;
        Eye[1] =  0.0f; At[1] = 0.0f; Up[1] = 1.0f;
        Eye[2] = -8.0f; At[2] = 0.0f; Up[2] = 0.0f;

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
        GetTranslationMatrix(-4.0f, 0.0f, 0.0f, WorldMatrix);
        
        SetWorldMatrix(WorldMatrix);

        // -----------------------------------------------------------------------------
        // Draw cube 1 with current set world matrix.
        // -----------------------------------------------------------------------------
        DrawMesh(m_pCubeMesh);

        GetTranslationMatrix(4.0f, 0.0f, 0.0f, WorldMatrix);

        SetWorldMatrix(WorldMatrix);

        // -----------------------------------------------------------------------------
        // Draw cube 2 with current set world matrix.
        // -----------------------------------------------------------------------------
        DrawMesh(m_pSphereMesh);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnKeyEvent(unsigned int _Key, bool _IsKeyDown, bool _IsAltDown)
    {
        // -----------------------------------------------------------------------------
        // Pressing the 'Space' key implies the condition to become true.
        // -----------------------------------------------------------------------------
        if (_Key == ' ')
        {
            std::cout << "Jump" << std::endl;
        }

        return true;
    }
} // namespace

void main()
{
    CApplication Application;

    RunApplication(800, 600, "YoshiX Example", &Application);
}