
#include "yoshix_fix_function.h"
#include "SNode.h"

#include <math.h>
#include <windows.h>

using namespace gfx;

namespace
{
    double    g_Frequency;
    long long g_StartTick;
} // namespace

namespace
{
    void GetFrequency()
    {
        long long Frequency;

        ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&Frequency));

        g_Frequency = static_cast<double>(Frequency);
    }

    // -----------------------------------------------------------------------------

    void StartTime()
    {
        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&g_StartTick));
    }

    // -----------------------------------------------------------------------------

    double GetTimeInSeconds()
    {
        long long CurrentRealTimeTick;

        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&CurrentRealTimeTick));

        return static_cast<double>(CurrentRealTimeTick - g_StartTick) / g_Frequency;
    }
} // namespace

namespace
{
    class CApplication : public IApplication
    {
        public:

            CApplication();
            virtual ~CApplication();


        private:

            static constexpr float s_CuboidHeight = 8.0f;

        private:

            float   m_FieldOfViewY;     // Vertical view angle of the camera.
            BHandle m_pCubeMesh;        // A pointer to a YoshiX mesh, which represents a single cube.
            SNode   m_Nodes[3];

        private:

            virtual bool InternOnStartup();
            virtual bool InternOnShutdown();
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
        : m_FieldOfViewY(60.0f)     // Set the vertical view angle of the camera to 60 degrees.
        , m_pCubeMesh   (nullptr)
        , m_Nodes{}
    {
        m_Nodes[0].m_Translation[0] = 0.0f;
        m_Nodes[0].m_Translation[1] = s_CuboidHeight * 0.5f;
        m_Nodes[0].m_Translation[2] = 0.0f;

        m_Nodes[1].m_Translation[0] = 0.0f;
        m_Nodes[1].m_Translation[1] = s_CuboidHeight;
        m_Nodes[1].m_Translation[2] = 0.0f;

        m_Nodes[2].m_Translation[0] = 0.0f;
        m_Nodes[2].m_Translation[1] = s_CuboidHeight;
        m_Nodes[2].m_Translation[2] = 0.0f;

        m_Nodes[0].m_TranslationDelta[0] = 0.05f;
        m_Nodes[0].m_TranslationDelta[1] = 0.0f;
        m_Nodes[0].m_TranslationDelta[2] = 0.0f;

        m_Nodes[1].m_TranslationDelta[0] = 0.0f;
        m_Nodes[1].m_TranslationDelta[1] = 0.0f;
        m_Nodes[1].m_TranslationDelta[2] = 0.0f;

        m_Nodes[2].m_TranslationDelta[0] = 0.0f;
        m_Nodes[2].m_TranslationDelta[1] = 0.0f;
        m_Nodes[2].m_TranslationDelta[2] = 0.0f;

        m_Nodes[0].m_Scale[0] = 5.0f;
        m_Nodes[0].m_Scale[1] = s_CuboidHeight;
        m_Nodes[0].m_Scale[2] = 5.0f;

        m_Nodes[1].m_Scale[0] = 3.0f;
        m_Nodes[1].m_Scale[1] = s_CuboidHeight;
        m_Nodes[1].m_Scale[2] = 3.0f;

        m_Nodes[2].m_Scale[0] = 1.0f;
        m_Nodes[2].m_Scale[1] = s_CuboidHeight;
        m_Nodes[2].m_Scale[2] = 1.0f;

        m_Nodes[0].m_AngleZ = 0.0f;
        m_Nodes[1].m_AngleZ = 0.0f;
        m_Nodes[2].m_AngleZ = 0.0f;

        m_Nodes[0].m_AngleDeltaZ = 0.0f;
        m_Nodes[1].m_AngleDeltaZ = -0.225f;
        m_Nodes[2].m_AngleDeltaZ = -0.225f;

        m_Nodes[0].m_Child = &m_Nodes[1];
        m_Nodes[1].m_Child = &m_Nodes[2];
        m_Nodes[2].m_Child = nullptr;

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

    bool CApplication::InternOnCreateMeshes()
    {
        // -----------------------------------------------------------------------------
        // Define the vertices of the mesh and their attributes.
        // -----------------------------------------------------------------------------
        static const float s_HalfEdgeLength = 0.5f;
        
        static float s_CubeVertices[][3] =
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

        static float s_CubeColors[][4] =
        {
            { 1.0f, 0.0f, 0.0f, 1.0f, },
            { 1.0f, 0.0f, 0.0f, 1.0f, },
            { 1.0f, 0.0f, 0.0f, 1.0f, },
            { 1.0f, 0.0f, 0.0f, 1.0f, },

            { 0.0f, 1.0f, 0.0f, 1.0f, },
            { 0.0f, 1.0f, 0.0f, 1.0f, },
            { 0.0f, 1.0f, 0.0f, 1.0f, },
            { 0.0f, 1.0f, 0.0f, 1.0f, },

            { 0.0f, 0.0f, 1.0f, 1.0f, },
            { 0.0f, 0.0f, 1.0f, 1.0f, },
            { 0.0f, 0.0f, 1.0f, 1.0f, },
            { 0.0f, 0.0f, 1.0f, 1.0f, },

            { 1.0f, 1.0f, 0.0f, 1.0f, },
            { 1.0f, 1.0f, 0.0f, 1.0f, },
            { 1.0f, 1.0f, 0.0f, 1.0f, },
            { 1.0f, 1.0f, 0.0f, 1.0f, },

            { 0.0f, 1.0f, 1.0f, 1.0f, },
            { 0.0f, 1.0f, 1.0f, 1.0f, },
            { 0.0f, 1.0f, 1.0f, 1.0f, },
            { 0.0f, 1.0f, 1.0f, 1.0f, },

            { 1.0f, 1.0f, 1.0f, 1.0f, },
            { 1.0f, 1.0f, 1.0f, 1.0f, },
            { 1.0f, 1.0f, 1.0f, 1.0f, },
            { 1.0f, 1.0f, 1.0f, 1.0f, },
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
        static int s_CubeIndices[][3] =
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
        MeshInfo.m_pColors          = &s_CubeColors[0][0];
        MeshInfo.m_pTexCoords       = nullptr;                          // No texture coordinates
        MeshInfo.m_NumberOfVertices = 24;
        MeshInfo.m_NumberOfIndices  = 36;
        MeshInfo.m_pIndices         = &s_CubeIndices[0][0];
        MeshInfo.m_pTexture         = nullptr;                          // No texture

        CreateMesh(MeshInfo, &m_pCubeMesh);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseMeshes()
    {
        // -----------------------------------------------------------------------------
        // Important to release the mesh again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseMesh(m_pCubeMesh);

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
        Eye[2] = -50.0f; At[2] = 0.0f; Up[2] = 0.0f;

        GetViewMatrix(Eye, At, Up, ViewMatrix);

        SetViewMatrix(ViewMatrix);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnFrame()
    {
        float ScaleMatrix[16];
        float RotationMatrix[16];
        float TranslationMatrix[16];
        float InversTranslationMatrix[16];
        float WorldMatrix[16];
        float TmpMatrix[16];

        // Node 0
        GetTranslationMatrix(
            m_Nodes[0].m_Translation[0],
            m_Nodes[0].m_Translation[1],
            m_Nodes[0].m_Translation[2],
            m_Nodes[0].m_LocalMatrix);

        memcpy(m_Nodes[0].m_GlobalMatrix, m_Nodes[0].m_LocalMatrix, sizeof(m_Nodes[0].m_GlobalMatrix));

        GetScaleMatrix(
            m_Nodes[0].m_Scale[0],
            m_Nodes[0].m_Scale[1],
            m_Nodes[0].m_Scale[2],
            ScaleMatrix
        );

        MulMatrix(ScaleMatrix, m_Nodes[0].m_GlobalMatrix, WorldMatrix);

        SetWorldMatrix(WorldMatrix);
        DrawMesh(m_pCubeMesh);

        // Node 1
        // Rotation Node 1
        GetTranslationMatrix(0.0f, s_CuboidHeight * 0.5f, 0.0f, TranslationMatrix);
        GetRotationZMatrix(m_Nodes[1].m_AngleZ, RotationMatrix);
        GetTranslationMatrix(0.0f, -s_CuboidHeight * 0.5f, 0.0f, InversTranslationMatrix);
        MulMatrix(TranslationMatrix, RotationMatrix, TmpMatrix);
        MulMatrix(TmpMatrix, InversTranslationMatrix, RotationMatrix);

        // Translation Node 1
        GetTranslationMatrix(
            m_Nodes[1].m_Translation[0],
            m_Nodes[1].m_Translation[1],
            m_Nodes[1].m_Translation[2],
            TranslationMatrix
        );


        // Local Node 1
        MulMatrix(RotationMatrix, TranslationMatrix, m_Nodes[1].m_LocalMatrix);


        // Global Node 1
        MulMatrix(m_Nodes[0].m_Child->m_LocalMatrix, m_Nodes[0].m_GlobalMatrix, m_Nodes[1].m_GlobalMatrix);

        // World Node 1
        GetScaleMatrix(
            m_Nodes[1].m_Scale[0],
            m_Nodes[1].m_Scale[1],
            m_Nodes[1].m_Scale[2],
            ScaleMatrix
        );


        MulMatrix(ScaleMatrix, m_Nodes[1].m_GlobalMatrix, WorldMatrix);


        SetWorldMatrix(WorldMatrix);
        DrawMesh(m_pCubeMesh);

        // Node 2
        // Rotation Node 2
        GetTranslationMatrix(0.0f, s_CuboidHeight * 0.5f, 0.0f, TranslationMatrix);
        GetRotationZMatrix(m_Nodes[2].m_AngleZ, RotationMatrix);
        GetTranslationMatrix(0.0f, -s_CuboidHeight * 0.5f, 0.0f, InversTranslationMatrix);
        MulMatrix(TranslationMatrix, RotationMatrix, TmpMatrix);
        MulMatrix(TmpMatrix, InversTranslationMatrix, RotationMatrix);

        // Translation Node 2
        GetTranslationMatrix(
            m_Nodes[2].m_Translation[0],
            m_Nodes[2].m_Translation[1],
            m_Nodes[2].m_Translation[2],
            TranslationMatrix
        );

        // Local Node 2
        MulMatrix(RotationMatrix, TranslationMatrix, m_Nodes[2].m_LocalMatrix);

        // Global Node 2
        MulMatrix(m_Nodes[1].m_Child->m_LocalMatrix, m_Nodes[1].m_GlobalMatrix, m_Nodes[2].m_GlobalMatrix);

        // World Node 2
        GetScaleMatrix(
            m_Nodes[2].m_Scale[0],
            m_Nodes[2].m_Scale[1],
            m_Nodes[2].m_Scale[2],
            ScaleMatrix
        );

        MulMatrix(ScaleMatrix, m_Nodes[2].m_GlobalMatrix, WorldMatrix);

        SetWorldMatrix(WorldMatrix);
        DrawMesh(m_pCubeMesh);

        m_Nodes[0].m_Translation[0] += m_Nodes[0].m_TranslationDelta[0];
        m_Nodes[0].m_Translation[1] += m_Nodes[0].m_TranslationDelta[1];
        m_Nodes[0].m_Translation[2] += m_Nodes[0].m_TranslationDelta[2];

        m_Nodes[1].m_AngleZ += m_Nodes[1].m_AngleDeltaZ;
        m_Nodes[2].m_AngleZ += m_Nodes[2].m_AngleDeltaZ;

        if ((m_Nodes[0].m_Translation[0] < -10.0f) || (m_Nodes[0].m_Translation[0] > 10.0f))
        {
            m_Nodes[0].m_TranslationDelta[0] = -m_Nodes[0].m_TranslationDelta[0];

            m_Nodes[1].m_AngleDeltaZ = -m_Nodes[1].m_AngleDeltaZ;
            m_Nodes[2].m_AngleDeltaZ = -m_Nodes[2].m_AngleDeltaZ;
        }


        return true;
    }
} // namespace

void main()
{
    GetFrequency();
    StartTime();

    CApplication Application;

    RunApplication(800, 600, "YoshiX Example", &Application);
}