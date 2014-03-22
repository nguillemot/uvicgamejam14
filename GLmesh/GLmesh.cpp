#include "GLmesh.hpp"

#include <tiny_obj_loader.h>

namespace GLmesh
{

void StaticMesh::LoadShape(const tinyobj::shape_t& shape)
{
    if (shape.mesh.indices.size() % 3 != 0)
    {
        throw std::runtime_error("Expected 3d vertices.");
    }

    std::shared_ptr<GLplus::Buffer> newIndices;
    std::shared_ptr<GLplus::Buffer> newPositions;
    std::shared_ptr<GLplus::Buffer> newNormals;
    std::shared_ptr<GLplus::Buffer> newTexcoords;

    newIndices.reset(new GLplus::Buffer(GL_ELEMENT_ARRAY_BUFFER));
    newIndices->Upload(
              shape.mesh.indices.size() * sizeof(shape.mesh.indices[0]),
              shape.mesh.indices.data(), GL_STATIC_DRAW);

    if (!shape.mesh.positions.empty())
    {
        newPositions.reset(new GLplus::Buffer(GL_ARRAY_BUFFER));
        newPositions->Upload(
                    shape.mesh.positions.size() * sizeof(shape.mesh.positions[0]),
                    shape.mesh.positions.data(), GL_STATIC_DRAW);
    }

    if (!shape.mesh.normals.empty())
    {
        newPositions.reset(new GLplus::Buffer(GL_ARRAY_BUFFER));
        newPositions->Upload(
                    shape.mesh.normals.size() * sizeof(shape.mesh.normals[0]),
                    shape.mesh.normals.data(), GL_STATIC_DRAW);
    }

    if (!shape.mesh.texcoords.empty())
    {
        newPositions.reset(new GLplus::Buffer(GL_ARRAY_BUFFER));
        newPositions->Upload(
                    shape.mesh.texcoords.size() * sizeof(shape.mesh.texcoords[0]),
                    shape.mesh.texcoords.data(), GL_STATIC_DRAW);
    }

    mVertexCount = shape.mesh.indices.size() / 3;

    mIndices = std::move(newIndices);
    mPositions = std::move(newPositions);
    mTexcoords = std::move(newTexcoords);
    mNormals = std::move(newNormals);
}

void StaticMesh::Render(const GLplus::Program& program) const
{
    GLplus::VertexArray vertexArray;

    vertexArray.SetIndexBuffer(mIndices, GL_UNSIGNED_INT);

    if (mPositions)
    {
        GLint positionLoc;
        if (program.TryGetAttributeLocation("position", positionLoc))
        {
            vertexArray.SetAttribute(
                        positionLoc, mPositions,
                        3, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }

    if (mNormals)
    {
        GLint normalLoc;
        if (program.TryGetAttributeLocation("normal", normalLoc))
        {
            vertexArray.SetAttribute(
                        normalLoc, mNormals,
                        3, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }

    if (mTexcoords)
    {
        GLint texcoord0Loc;
        if (program.TryGetAttributeLocation("texcoord0", texcoord0Loc))
        {
            vertexArray.SetAttribute(
                        texcoord0Loc, mTexcoords,
                        2, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }

    DrawElements(program, vertexArray,
                 GL_TRIANGLES, 0, mVertexCount);
}

} // end namespace GLmesh
