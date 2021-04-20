/* 
 * Created by Matthias "Nelaty" Gensheimer on 26.03.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include "ovu/config/VersionInfo.h"
#include "ovu/config/ResourcePathProvider.h"
#include "ovu/entity/Vertex.h"
#include "ovu/vulkan/VulkanBuffer.h"

#include <vulkan/vulkan.hpp>

#define GLM_FORCE_RADIANS // TODO: Move to compiler constants
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <optional>
#include <fstream>

static const ovu::VersionInfo APP_VERSION(1, 0, 0, 0);
static const ovu::VersionInfo ENGINE_VERSION(1, 0, 0, 0);
static const ovu::VersionInfo VULKAN_VERSION(1, 2, 0, 0);

static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices
{
    std::optional<uint32_t> m_graphicsFamily;
    std::optional<uint32_t> m_presentFamily;

    bool isComplete() const
    {
        return m_graphicsFamily.has_value()
            && m_presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject
{
    alignas(16) glm::mat4 m_model;
    alignas(16) glm::mat4 m_view;
    alignas(16) glm::mat4 m_proj;
};

class HelloTriangleApplication
{
public:
    void run();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << "\n";
        return VK_FALSE;
    }

private:

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        app->m_framebufferResized = true;
    }


    void initWindow();
    void initVulkan();

    void mainLoop();
    void cleanup();


    void drawFrame();
    void createSyncObjects();

    // Instance and validation layers
    void createInstance();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    // Physical device
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);

    // Logical device
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    void createLogicalDevice();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions(const std::vector<VkExtensionProperties>& availableExtensions);

    // WMI
    void createSurface();

    // Swap chain: Todo: class VulkanSwapChainWrapper
    void createSwapChain();
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createFramebuffers();

    void recreateSwapChain();

    void cleanupSwapChain();

    // Render pipeline
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();

    static std::vector<char> readFile(const std::string& filename); // Todo: move -> Utility
    VkShaderModule createShaderModule(const std::vector<char>& code);

    // Uniforms
    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();

    VkDescriptorPool m_descriptorPool;
    std::vector<VkDescriptorSet> m_descriptorSets;

    // Commands
    void createCommandPool();
    void createCommandBuffers();

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    GLFWwindow* m_window;
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_logicalDevice;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    VkSurfaceKHR m_surface;


    // Swap chain
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    bool m_framebufferResized{false};

    // Pipeline
    VkPipeline m_graphicsPipeline;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;
    VkRenderPass m_renderPass;

    // Commands
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;

    // Synchronization (drawing)
    size_t m_currentFrame{0};
    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;

    // Vertices
    const std::vector<Vertex> m_vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> m_indices = {
        0, 1, 2, 2, 3, 0
    };


    ovu::VulkanBuffer m_vertexBuffer;
    ovu::VulkanBuffer m_indexBuffer;
    std::vector<ovu::VulkanBuffer> m_uniformBuffers;

    //std::vector<VkBuffer> m_uniformBuffers;
    //std::vector<VkDeviceMemory> m_uniformBuffersMemory;


    void createVertexBuffers();
    void createIndexBuffers();
    void createUniformBuffers();

    void updateUniformBuffer(uint32_t currentImage);


    void createBuffer(VkDeviceSize size,
                      VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory);


    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};
