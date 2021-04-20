/* 
 * Created by Matthias "Nelaty" Gensheimer on 19.04.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "ovu/entity/Vertex.h"

#include <climits>
#include <vector>

namespace ovu
{
    enum class GeometryCountType
    {
        UINT_16,
        UINT_32,

        TYPE_COUNT
    };

#ifdef OVU_LOW_VERTEX_COUNT
    using index_count_t = uint16_t;
    static constexpr uint32_t GeometryCountType OVU_GEOMETRY_COUNT_TYPE = GeometryCountType::UINT_16;
#else
    using index_count_t = uint32_t;
    static constexpr uint32_t GeometryCountType OVU_GEOMETRY_COUNT_TYPE = GeometryCountType::UINT_32;
#endif

    class Geometry
    {
    public:
        static uint32_t getMaxIndexCount(){
            return std::numeric_limits<index_count_t>()::max();
        }

        const uint64_t getSize() const{
            return sizeof(Vertex) * m_vertices.size()
                + sizeof(index_count_t) * m_indices.size();
        }

        const std::vector<Vertex>& getVertices() const{
            return m_vertices;
        }

        const std::vector<vertex_count_t>& getIndices() const{
            return m_indices;
        }

    protected:
        std::vector<Vertex> m_vertices;
        std::vector<index_count_t> m_indices;
    };
}