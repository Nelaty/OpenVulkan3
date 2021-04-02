/* 
 * Created by Matthias "Nelaty" Gensheimer on 26.03.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "ovu/config/VersionInfo.h"
#include "ovu/config/ResourcePathProvider.h"

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

static const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

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
    // TODO: class VulkanValidationLayerWrapper
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                 const VkAllocationCallbacks* pAllocator,
                                                 VkDebugUtilsMessengerEXT* pDebugMessenger);

    static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                              VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks* pAllocator);

    void initWindow();
    void initVulkan();

    void mainLoop();
    void cleanup();


    void drawFrame(){
        uint32_t imageIndex;
        vkAcquireNextImageKHR(m_logicalDevice, m_swapChain, UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if(vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS){
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {m_swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        presentInfo.pResults = nullptr;

        vkQueuePresentKHR(m_presentQueue, &presentInfo);
        vkQueueWaitIdle(m_presentQueue);
    }

    void createSemaphore(){
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if(vkCreateSemaphore(m_logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS
            || vkCreateSemaphore(m_logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphores!");
        }
    }

    // Instance and validation layers
    void createInstance();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    bool checkValidationLayerSupport();

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

    // Render pipeline
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();

    static std::vector<char> readFile(const std::string& filename); // Todo: move -> Utility
    VkShaderModule createShaderModule(const std::vector<char>& code);

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

    // Pipeline
    VkPipeline m_graphicsPipeline;
    VkPipelineLayout m_pipelineLayout;
    VkRenderPass m_renderPass;

    // Commands
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;
    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
};
