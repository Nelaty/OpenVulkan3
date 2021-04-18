/* 
 * Created by Matthias "Nelaty" Gensheimer on 17.04.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "Vertex.h"

#include <vector>

class VertexBuffer
{
public:

    void init(VkPhysicalDevice physicalDevice, VkDevice device){
        createVertexBuffers(physicalDevice, device);
    }

    void add(const Vertex& vertex){
        m_data.emplace_back(vertex);
    }

    void setData(const std::vector<Vertex>& data){
        m_data = data;
    }

    std::vector<Vertex>& getData(){
        return m_data;
    }

    const std::vector<Vertex>& getData() const{
        return m_data;
    }

private:
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice,
                            uint32_t typeFilter,
                            VkMemoryPropertyFlags properties){
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
        for(uint32_t i = 0; i < memProperties.memoryTypeCount; ++i){
            if(typeFilter & (i << i)
                && (memProperties.memoryTypes[i].propertyFlags & properties) == properties){
                return i;
            }
        }
    }

    void createVertexBuffers(VkPhysicalDevice physicalDevice, VkDevice device){
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(m_data[0]) * m_data.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if(vkCreateBuffer(device, &bufferInfo, nullptr, &m_vertexBuffer) != VK_SUCCESS){
            throw std::runtime_error("failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device,  m_vertexBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(physicalDevice,
                                                   memRequirements.memoryTypeBits,
                                                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                                       | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if(vkAllocateMemory(device, &allocInfo, nullptr, &m_vertexBufferMemory) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(device, m_vertexBuffer, m_vertexBufferMemory, 0);

        void* data;
        vkMapMemory(device, m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, m_data.data(), (size_t)bufferInfo.size);
        vkUnmapMemory(device, m_vertexBufferMemory);
    }

    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;

    std::vector<Vertex> m_data;
};