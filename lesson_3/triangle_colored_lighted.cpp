
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

            BHandle m_pTriangleMesh;    // A pointer to a YoshiX mesh, which represents a single triangle.

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
        : m_FieldOfViewY (60.0f)    // Set the vertical view angle of the camera to 60 degrees.
        , m_pTriangleMesh(nullptr)
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

        // -----------------------------------------------------------------------------
        // Define the position of the light source in 3D space. The fixed function 
        // pipeline of YoshiX only supports one light source.
        // -----------------------------------------------------------------------------
        float LightPosition[3] = { 0.0f, 0.0f, -4.0f, };

        SetLightPosition(LightPosition);

        // -----------------------------------------------------------------------------
        // Define the ambient, diffuse, and specular color of the light source. 
        // -----------------------------------------------------------------------------
        float LightAmbientColor [4] = { 0.1f, 0.1f, 0.1f, 1.0f, };
        float LightDiffuseColor [4] = { 0.6f, 0.6f, 0.6f, 1.0f, };
        float LightSpecularColor[4] = { 0.9f, 0.9f, 0.9f, 1.0f, };

        SetLightColor(LightAmbientColor, LightDiffuseColor, LightSpecularColor, 127);

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
        static float s_TriangleVertices[][3] =
        {
            { -4.0f, -2.0f, 0.0f, },
            {  4.0f, -2.0f, 0.0f, },
            {  0.0f,  2.0f, 0.0f, },
        };

        static float s_TriangleNormals[][3] =
        {
            { 0.0f, 0.0f, -1.0f, },             // Normal of vertex 0.
            { 0.0f, 0.0f, -1.0f, },             // Normal of vertex 1.
            { 0.0f, 0.0f, -1.0f, },             // Normal of vertex 2.
        };

        static float s_TriangleColors[][4] =
        {
            { 1.0f, 0.0f, 0.0f, 1.0f, },        // Color of vertex 0.
            { 0.0f, 1.0f, 0.0f, 1.0f, },        // Color of vertex 1.
            { 0.0f, 0.0f, 1.0f, 1.0f, },        // Color of vertex 2.
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
        static int s_TriangleIndices[][3] =
        {
            { 0, 1, 2, },
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

        MeshInfo.m_pVertices        = &s_TriangleVertices[0][0];
        MeshInfo.m_pNormals         = &s_TriangleNormals[0][0];
        MeshInfo.m_pColors          = &s_TriangleColors[0][0];
        MeshInfo.m_pTexCoords       = nullptr;                          // No texture coordinates.
        MeshInfo.m_NumberOfVertices = 3;
        MeshInfo.m_NumberOfIndices  = 3;
        MeshInfo.m_pIndices         = &s_TriangleIndices[0][0];
        MeshInfo.m_pTexture         = nullptr;                          // No texture.

        CreateMesh(MeshInfo, &m_pTriangleMesh);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseMeshes()
    {
        // -----------------------------------------------------------------------------
        // Important to release the mesh again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseMesh(m_pTriangleMesh);

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
        GetTranslationMatrix(0.0f, 0.0f, 0.0f, WorldMatrix);
        
        SetWorldMatrix(WorldMatrix);

        DrawMesh(m_pTriangleMesh);

        return true;
    }
} // namespace

void main()
{
    CApplication Application;

    RunApplication(800, 600, "YoshiX Example", &Application);
}