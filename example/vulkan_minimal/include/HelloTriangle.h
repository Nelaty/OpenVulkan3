/* 
 * Created by Matthias "Nelaty" Gensheimer on 26.03.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <fmt/format.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>

struct VersionInfo {
    VersionInfo(uint32_t major, uint32_t minor,
        uint32_t patch, uint32_t revision)
        : m_major{major},
        m_minor{minor},
        m_patch{patch},
        m_revision{revision}{
    }

    uint32_t toVulkanVersion() const {
        return VK_MAKE_VERSION(m_major, m_minor, m_patch);
    }

    std::string toString() const {
        return fmt::format("{}.{}.{}.{}", m_major, m_minor, m_patch, m_revision);
    }

    uint32_t m_major;
    uint32_t m_minor;
    uint32_t m_patch;
    uint32_t m_revision;
};

static const VersionInfo APP_VERSION(1, 0, 0, 0);
static const VersionInfo ENGINE_VERSION(1, 0, 0, 0);
static const VersionInfo VULKAN_VERSION(1, 2, 0, 0);

static const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};
#ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

class HelloTriangleApplication {
public:
    void run();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData){
        std::cerr << "validation layer: " << pCallbackData->pMessage << "\n";
        return VK_FALSE;
    }

private:
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDebugUtilsMessengerEXT* pDebugMessenger);

    static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator);

    void initWindow();
    void initVulkan();

    void createInstance();

    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    std::vector<const char*> getRequiredExtensions(const std::vector<VkExtensionProperties>& availableExtensions);

    bool checkValidationLayerSupport();

    void mainLoop();
    void cleanup();

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    GLFWwindow* m_window;
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
};
