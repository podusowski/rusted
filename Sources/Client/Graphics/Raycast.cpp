#include <boost/optional.hpp>

#include <OgreSubMesh.h>
#include <OgreEntity.h>

#include "Cake/Diagnostics/Logger.hpp"

#include "Raycast.hpp"

using namespace Client::Graphics;

Raycast::Raycast(Ogre::SceneManager & sceneManager)
{
    m_raySceneQuery = sceneManager.createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
    m_raySceneQuery->setSortByDistance(true);
}

RaycastResult Raycast::cast(const Ogre::Ray & ray)
{
    auto ret = RaycastResult();
    cast(ray, [&](Ogre::Entity * entity, Ogre::Vector3 position) -> bool
    {
        ret.entity = entity;
        ret.position = position;
        ret.valid = true;
        return false;
    });
    return ret;
}

void Raycast::cast(const Ogre::Ray & ray, std::function<bool(Ogre::Entity *, Ogre::Vector3)> function)
{
    auto ret = RaycastResult();

    m_raySceneQuery->setRay(ray);

    // reference of the result is persistent in Ogre::RaySceneQuery until
    // next execute() call
    Ogre::RaySceneQueryResult & result = m_raySceneQuery->execute();

    if (result.empty())
    {
        return;
    }

    boost::optional<Ogre::Real> minDistance;
    bool finished = false;

    LOG_DEBUG << "Found " << result.size() << " candidates";

    for (auto it: result)
    {
        if (finished)
        {
            break;
        }

        LOG_DEBUG << "Potential match, distance: " << it.distance;

        // if it's farther that previous found one, just break the algorithm
        #if 0
        if (minDistance && *minDistance < it.distance)
        {
            break;
        }
        #endif

        if (it.movable != nullptr && it.movable->getMovableType() == "Entity")
        {
            Ogre::Entity & entity = dynamic_cast<Ogre::Entity&>(*it.movable);

            minDistance = boost::none;

            // mesh data to retrieve
            size_t vertex_count;
            size_t index_count;
            Ogre::Vector3 *vertices;
            unsigned long *indices;

            // get the mesh information
            getMeshInformation(
                entity.getMesh(), vertex_count, vertices, index_count, indices,
                entity.getParentNode()->_getDerivedPosition(),
                entity.getParentNode()->_getDerivedOrientation(),
                entity.getParentNode()->_getDerivedScale());

            // test for hitting individual triangles on the mesh
            for (int i = 0; i < static_cast<int>(index_count); i += 3)
            {
                // check for a hit against this triangle
                std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray,
                    vertices[indices[i]],
                    vertices[indices[i+1]],
                    vertices[indices[i+2]],
                    true, false);

                // if it was a hit check if its the closest
                if (hit.first)
                {
                    if (!minDistance || hit.second < *minDistance)
                    {
                        minDistance = hit.second;
                        ret.entity = &entity;
                        ret.valid = true;
                        ret.position = ray.getOrigin() + (ray.getDirection().normalisedCopy() * hit.second);
                    }
                }
            }

            if (ret.valid)
            {
                LOG_DEBUG << "Raycast matched entity, exact position: " << ret.position;

                finished = ! function(ret.entity, ret.position);

                if (finished)
                {
                    LOG_DEBUG << "  user function accepted the raycast";
                    break;
                }
                else
                {
                    LOG_DEBUG << "  user function rejected the raycast, looking deeper";
                }
            }

            // free the verticies and indicies memory
            delete[] vertices;
            delete[] indices;
        }
    }
}

// algo from http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Raycasting+to+the+polygon+level&structure=Cookbook
void Raycast::getMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;
 
    vertex_count = index_count = 0;
 
    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh( i );
 
        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
 
        // Add the indices
        index_count += submesh->indexData->indexCount;
    }
 
 
    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];
 
    added_shared = false;
 
    // Run through the submeshes again, adding the data into the arrays
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
        if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
 
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
 
            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
            float* pReal;
 
            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
 
                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
 
                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }
 
            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }
 
 
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
        if( ibuf.isNull() ) continue; // need to check if index buffer is valid (which will be not if the mesh doesn't have triangles like a pointcloud)
 
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
 
        unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
 
 
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
        size_t index_start = index_data->indexStart;
        size_t last_index = numTris*3 + index_start;
 
        if (use32bitindexes)
            for (size_t k = index_start; k < last_index; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>( offset );
            }
 
        else
            for (size_t k = index_start; k < last_index; ++k)
            {
                indices[ index_offset++ ] = static_cast<unsigned long>( pShort[k] ) +
                    static_cast<unsigned long>( offset );
            }
 
        ibuf->unlock();
        current_offset = next_offset;
    }
}

