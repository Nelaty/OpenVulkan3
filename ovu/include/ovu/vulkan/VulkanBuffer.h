/* 
 * Created by Matthias "Nelaty" Gensheimer on 17.04.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include <vulkan/vulkan.hpp>

namespace ovu
{
    class VulkanBuffer
    {
    public:
        VulkanBuffer() = default;
        ~VulkanBuffer()
        {
            cleanup();
        }

        void init(vk::PhysicalDevice physicalDevice,
                  vk::Device device,
                  uint32_t bufferSize,
                  vk::BufferUsageFlags usage,
                  vk::MemoryPropertyFlags properties)
        {
            m_physicalDevice = physicalDevice;
            m_device = device;
            m_usage = usage;
            m_memProperties = properties;
            m_bufferSize = bufferSize;

            createBuffer(bufferSize, usage, properties);
        }

        void copyFromStaging(VkDevice device,
                             VkCommandPool commandPool,
                             VkQueue graphicsQueue,
                             void* data,
                             uint32_t dataSize)
        {
            ovu::VulkanBuffer stagingBuffer;
            stagingBuffer.init(m_physicalDevice,
                               device,
                               dataSize,
                               vk::BufferUsageFlags(VK_BUFFER_USAGE_TRANSFER_SRC_BIT),
                               vk::MemoryPropertyFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

            void* dataStaging;
            vkMapMemory(device, stagingBuffer.getMemory(), 0, dataSize, 0, &dataStaging);
            memcpy(dataStaging, data, dataSize);
            vkUnmapMemory(device, stagingBuffer.getMemory());

            copyFrom(device, commandPool, graphicsQueue, stagingBuffer, dataSize);
        }

        void copyFrom(VkDevice device,
                      VkCommandPool commandPool,
                      VkQueue graphicsQueue,
                      const VulkanBuffer& buffer,
                      vk::DeviceSize size)
        {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = commandPool;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffer, &beginInfo);

            VkBufferCopy copyRegion{};
            copyRegion.srcOffset = 0;
            copyRegion.dstOffset = 0;
            copyRegion.size = size;

            vkCmdCopyBuffer(commandBuffer, buffer.getBuffer(), m_buffer, 1, &copyRegion);
            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(graphicsQueue,1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(graphicsQueue);

            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
        }

        void cleanup()
        {
            m_device.destroyBuffer(m_buffer, nullptr);
            m_device.freeMemory(m_memory, nullptr);
        }

        const vk::Buffer& getBuffer() const
        {
            return m_buffer;
        }

        const vk::DeviceMemory& getMemory() const
        {
            return m_memory;
        }

    private:
        uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
        {
            vk::PhysicalDeviceMemoryProperties memProperties;
            m_physicalDevice.getMemoryProperties(&memProperties);
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

            vk::MemoryRequirements memRequirements;
            m_device.getBufferMemoryRequirements(m_buffer, &memRequirements);

            vk::MemoryAllocateInfo allocInfo{};
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                                       properties);


            if(m_device.allocateMemory(&allocInfo, nullptr, &m_memory) != vk::Result::eSuccess){
                throw std::runtime_error("failed to allocate vertex buffer memory!");
            }

            m_device.bindBufferMemory(m_buffer, m_memory, 0);
        }

        vk::PhysicalDevice m_physicalDevice;
        vk::Device m_device;
        vk::Buffer m_buffer;
        vk::DeviceMemory m_memory;

        uint32_t m_bufferSize;
        vk::BufferUsageFlags m_usage;
        vk::MemoryPropertyFlags m_memProperties;
    };
}