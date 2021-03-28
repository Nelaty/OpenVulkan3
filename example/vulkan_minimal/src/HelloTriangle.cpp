/* 
 * Created by Matthias "Nelaty" Gensheimer on 27.03.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#include "HelloTriangle.h"
void HelloTriangleApplication::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initWindow()
{
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("GLFW initialization");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Do not use default OpenGL API
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Resizing needs additional care

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    if (m_window == nullptr) {
        throw std::runtime_error("Window creation");
    }
}

void HelloTriangleApplication::initVulkan()
{
    createInstance();
    setupDebugMessenger();
}

void HelloTriangleApplication::createInstance()
{
    if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
        throw std::runtime_error("Validation layers requested, but not available");
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
    std::cout << "Available extensions:\n";
    for (const auto& it : availableExtensions) {
        std::cout << "\t" << it.extensionName << "\n";
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = APP_VERSION.toVulkanVersion();
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = ENGINE_VERSION.toVulkanVersion();
    appInfo.apiVersion = VULKAN_VERSION.toVulkanVersion();

    auto extensions = getRequiredExtensions(availableExtensions);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_instance)) {
        throw std::runtime_error("ERROR: Create Vulkan instance");
    }
}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    bool result = true;
    for (const char* layerName : VALIDATION_LAYERS) {
        auto
            found = std::find_if(availableLayers.begin(), availableLayers.end(), [&](const VkLayerProperties& available)
        {
            return strcmp(layerName, available.layerName);
        });
        if (found == availableLayers.end()) {
            std::cerr << "ERROR: Validation layer not supported: " << layerName << "\n";
            result = false;
        }
    }
    return result;
}

void HelloTriangleApplication::mainLoop()
{
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::cleanup()
{
    if(ENABLE_VALIDATION_LAYERS){
        DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
    }
    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

std::vector<const char*>
HelloTriangleApplication::getRequiredExtensions(const std::vector<VkExtensionProperties>& availableExtensions)
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (ENABLE_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
        if (std::find_if(availableExtensions.begin(),
                         availableExtensions.end(),
                         [&](const VkExtensionProperties& ext)
                         { return strcmp(ext.extensionName, glfwExtensions[i]); })
            == availableExtensions.end()) {

            std::cerr << "ERROR: Extension not supported: " << glfwExtensions[i] << "\n";
        }
    }

    return extensions;
}

void HelloTriangleApplication::setupDebugMessenger()
{
    if (!ENABLE_VALIDATION_LAYERS) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    if(CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS){
        throw std::runtime_error("failed to set up debug messenger");
    }
}

void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
}

VkResult HelloTriangleApplication::CreateDebugUtilsMessengerEXT(VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(func != nullptr){
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void HelloTriangleApplication::DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                             VkDebugUtilsMessengerEXT debugMessenger,
                                                             const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func != nullptr){
        func(instance, debugMessenger, pAllocator);
    }
}

