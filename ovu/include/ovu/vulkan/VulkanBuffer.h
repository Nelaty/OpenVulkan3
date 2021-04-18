/* 
 * Created by Matthias "Nelaty" Gensheimer on 17.04.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include <vulkan/vulkan.hpp>

namespace ovu
{
    /*
    class VulkanBuffer
    {
    public:
        VulkanBuffer() = default;
        ~VulkanBuffer(){
            cleanup();
        }

        void init(vk::Device device,
                  uint32_t bufferSize,
                  VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties)
        {
            m_device = device;
            m_usage = usage;
            m_memProperties = properties;
            m_bufferSize = bufferSize;

            createBuffer(bufferSize, usage, properties);
        }

        void cleanup()
        {
            m_device.destroyBuffer(m_buffer, nullptr);
            m_device.freeMemory(m_memory, nullptr);
        }

        const vk::Buffer& getBuffer() const{
            return m_buffer;
        }

        const vk::DeviceMemory& getMemory() const{
            return m_memory;
        }

    private:
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);
            for(uint32_t i = 0; i < memProperties.memoryTypeCount; ++i){
                if(typeFilter & (i << i)
                    && (memProperties.memoryTypes[i].propertyFlags & properties) == properties){
                    return i;
                }
            }
        }

        void createBuffer(vk::DeviceSize size,
                          vk::BufferUsageFlags usage,
                          vk::MemoryPropertyFlags properties)
        {
            vk::BufferCreateInfo bufferInfo;
            bufferInfo.setSize(size);
            bufferInfo.setUsage(usage);
            bufferInfo.setSharingMode(vk::SharingMode::eExclusive);

            if(m_device.createBuffer(&bufferInfo, nullptr, &m_buffer) != vk::Result::eSuccess){
                throw std::runtime_error("failed to create vertex buffer!");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(m_device,  m_buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                                       properties);

            if(m_device.allocateMemory(&allocInfo, nullptr, &m_memory) != vk::Result::eSuccess){
                throw std::runtime_error("failed to allocate vertex buffer memory!");
            }

            vkBindBufferMemory(m_device, buffer, m_memory, 0);
        }

        vk::Device m_device;
        vk::Buffer m_buffer;
        vk::DeviceMemory m_memory;

        uint32_t m_bufferSize;
        vk::BufferUsageFlags m_usage;
        vk::MemoryPropertyFlags m_memProperties;
    };
     */
}