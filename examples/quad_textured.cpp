
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

        BHandle m_pQuadMesh;        // A pointer to a YoshiX mesh, which represents a single quad.
        BHandle m_pQuadTexture;     // A pointer to a YoshiX texture, which is part of the material covering the quad.

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
        : m_FieldOfViewY(60.0f)     // Set the vertical view angle of the camera to 60 degrees.
        , m_pQuadMesh   (nullptr)
        , m_pQuadTexture(nullptr)
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
        CreateTexture("..\\data\\images\\wall.dds", &m_pQuadTexture);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseTextures()
    {
        // -----------------------------------------------------------------------------
        // Important to release the texture again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseTexture(m_pQuadTexture);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnCreateMeshes()
    {
        // -----------------------------------------------------------------------------
        // Define the vertices of the mesh and their attributes.
        // -----------------------------------------------------------------------------
        static float s_QuadVertices[][3] =
        {
            { -4.0f, -4.0f, 0.0f, },
            {  4.0f, -4.0f, 0.0f, },
            {  4.0f,  4.0f, 0.0f, },
            { -4.0f,  4.0f, 0.0f, },
        };

        static float s_QuadTexCoords[][2] =
        {
            { 0.0f, 1.0f, },                    // Texture coordinate of vertex 0.
            { 1.0f, 1.0f, },                    // Texture coordinate of vertex 1.
            { 1.0f, 0.0f, },                    // Texture coordinate of vertex 2.
            { 0.0f, 0.0f, },                    // Texture coordinate of vertex 3.
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
        static int s_QuadIndices[][3] =
        {
            { 0, 1, 2, },
            { 0, 2, 3, },
        };

        // -----------------------------------------------------------------------------
        // Define the mesh and its material. The material defines the look of the
        // surface covering the mesh. If the material should contain normals, colors, or
        // texture coordinates then their number has to match the number of vertices.
        // If you do not support normals in a mesh, YoshiX will not apply lighting to
        // this mesh.
        // -----------------------------------------------------------------------------
        SMeshInfo MeshInfo;

        MeshInfo.m_pVertices        = &s_QuadVertices[0][0];
        MeshInfo.m_pNormals         = nullptr;                      // No normals.
        MeshInfo.m_pColors          = nullptr;                      // No colors.
        MeshInfo.m_pTexCoords       = &s_QuadTexCoords[0][0];
        MeshInfo.m_NumberOfVertices = 4;
        MeshInfo.m_NumberOfIndices  = 6;
        MeshInfo.m_pIndices         = &s_QuadIndices[0][0];
        MeshInfo.m_pTexture         = m_pQuadTexture;

        CreateMesh(MeshInfo, &m_pQuadMesh);

        return true;
    }

    // -----------------------------------------------------------------------------

    bool CApplication::InternOnReleaseMeshes()
    {
        // -----------------------------------------------------------------------------
        // Important to release the mesh again when the application is shut down.
        // -----------------------------------------------------------------------------
        ReleaseMesh(m_pQuadMesh);

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

        DrawMesh(m_pQuadMesh);

        return true;
    }
} // namespace

void main()
{
    CApplication Application;

    RunApplication(800, 600, "YoshiX Example", &Application);
}