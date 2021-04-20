/* 
 * Created by Matthias "Nelaty" Gensheimer on 20.04.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "VulkanBuffer.h"

#include <vulkan/vulkan.hpp>

namespace ovu
{
    struct BufferCreationFlagsInfo
    {
        BufferCreationInfo(vk::BufferUsageFlags usage,
                           vk::MemoryPropertyFlags properties)
            : m_usage(usage),
            m_properties(properties)
        {}
        ~BufferCreationInfo() = default;

        vk::BufferUsageFlags m_usage;
        vk::MemoryPropertyFlags m_properties;
    };

    /*
    struct VertexBufferCreationContextInfo
    {

    };
     */

    class VulkanBufferFactory
    {
    public:
        static const BufferCreationFlagsInfo defaultVertexInfo{
            vk::BufferUsageFlags(VK_BUFFER_USAGE_TRANSFER_SRC_BIT),
            vk::MemoryPropertyFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        };
        static const BufferCreationFlagsInfo defaultIndexInfo{
            vk::BufferUsageFlags(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT),
            vk::MemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        };
        static const BufferCreationFlagsInfo defaultUniformInfo{
            vk::BufferUsageFlags(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT),
            vk::MemoryPropertyFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        };

        ~VulkanBufferFactory() = default;

        VulkanBuffer createVertexBuffer(BufferCreationFlagsInfo info = defaultVertexInfo);
        VulkanBuffer createIndexBuffer(BufferCreationFlagsInfo info = defaultIndexInfo);
        VulkanBuffer createUniformBuffer(BufferCreationFlagsInfo info = defaultUniformInfo);

    private:
        VulkanBufferFactory() = default;
    };
}