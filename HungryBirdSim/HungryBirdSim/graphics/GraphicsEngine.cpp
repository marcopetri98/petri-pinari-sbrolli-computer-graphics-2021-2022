//#include <iostream>
//#include <vector>
//#include <map>
//#include <fstream>
//#include "GraphicsEngine.h"
//#include "../utils/Errors.h"
//#include <set>
//#include <algorithm> // Necessary for std::clamp
//
//#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//const std::string MODEL_PATH = "models/viking_room.obj";
//
//struct Vertex {
//	glm::vec3 pos;
//	glm::vec3 norm;
//	glm::vec2 texCoord;
//
//	static VkVertexInputBindingDescription getBindingDescription() {
//		VkVertexInputBindingDescription bindingDescription{};
//		bindingDescription.binding = 0;
//		bindingDescription.stride = sizeof(Vertex);
//		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//
//		return bindingDescription;
//	}
//
//};
//
//
//
//using std::vector;
//using std::cout;
//using std::endl;
//using errors::getErrorStr;
//using errors::Error;
//
//namespace graphics
//{
//
//	std::vector<Vertex> vertices;
//	std::vector<uint32_t> indices;
//	VkBuffer vertexBuffer;
//	VkDeviceMemory vertexBufferMemory;
//
//
//	void loadModel() {
//		tinyobj::attrib_t attrib;
//		std::vector<tinyobj::shape_t> shapes;
//		std::vector<tinyobj::material_t> materials;
//		std::string warn, err;
//		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
//			throw std::runtime_error(warn + err);
//		}
//
//		for (const auto& shape : shapes) {
//			for (const auto& index : shape.mesh.indices) {
//				Vertex vertex{};
//
//				vertex.pos = {
//					attrib.vertices[3 * index.vertex_index + 0],
//					attrib.vertices[3 * index.vertex_index + 1],
//					attrib.vertices[3 * index.vertex_index + 2]
//				};
//
//				vertex.texCoord = {
//					attrib.texcoords[2 * index.texcoord_index + 0],
//					1 - attrib.texcoords[2 * index.texcoord_index + 1]
//				};
//
//				vertex.norm = {
//					attrib.normals[3 * index.normal_index + 0],
//					attrib.normals[3 * index.normal_index + 1],
//					attrib.normals[3 * index.normal_index + 2]
//				};
//
//				vertices.push_back(vertex);
//				indices.push_back(vertices.size() - 1);
//			}
//		}
//
//
//	}
//
//
//
//	GraphicsEngine::GraphicsEngine(string title, int width, int height)
//	{
//		this->title = title;
//		this->width = width;
//		this->height = height;
//		useValidationLayers = true;
//	}
//	
//	void GraphicsEngine::run()
//	{
//		initWindow();
//		initVulkan();
//		mainLoop();
//		cleanup();
//	}
//
//	void GraphicsEngine::setUseValidationLayers(bool val)
//	{
//		useValidationLayers = val;
//	}
//
//	void GraphicsEngine::initWindow()
//	{
//		glfwInit();
//
//		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//
//		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
//	}
//
//	void GraphicsEngine::initVulkan()
//	{
//		createInstance();
//		setupDebugMessenger();
//		createSurface();
//		pickPhysicalDevice();
//		createLogicalDevice();
//		createSwapChain();
//		createImageViews();
//		createRenderPass();
//		createGraphicsPipeline();
//		createFramebuffers();
//		createCommandPool();
//		createCommandBuffer();
//		createSyncObjects();
//	}
//
//	void GraphicsEngine::createInstance()
//	{
//		// first of all, we check if the validation layer are requested and available
//		if (useValidationLayers && !checkValidationLayerSupport())
//		{
//			throw std::runtime_error(getErrorStr(Error::VULKAN_FAIL_VALIDATION_LAYERS));
//		}
//		checkSupportedExtensions();
//
//		VkApplicationInfo appInfo{};
//		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//		appInfo.pApplicationName = title.c_str();
//		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//		appInfo.pEngineName = "No Engine";
//		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//		appInfo.apiVersion = VK_API_VERSION_1_0;
//
//		VkInstanceCreateInfo createInfo{};
//		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//		createInfo.pApplicationInfo = &appInfo;
//
//		auto extensions = getRequiredExtensions();
//		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
//		createInfo.ppEnabledExtensionNames = extensions.data();
//
//		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
//		if (useValidationLayers)
//		{
//			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()); 
//			createInfo.ppEnabledLayerNames = validationLayers.data();
//			// debug messager is attached as extension to debug instance creation
//			populateDebugMessengerCreateInfo(debugCreateInfo);
//			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
//		}
//		else
//		{
//			createInfo.enabledLayerCount = 0;
//			createInfo.pNext = nullptr;
//		}
//
//		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
//		{
//			throw std::runtime_error(getErrorStr(Error::VULKAN_FAIL_CREATE_INSTANCE));
//		}
//	}
//
//	vector<const char*> GraphicsEngine::getRequiredExtensions()
//	{
//		uint32_t glfwExtensionCount = 0;
//		const char** glfwExtensions;
//		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//		vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
//
//		if (useValidationLayers)
//		{
//			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//		}
//
//		return extensions;
//	}
//
//	void GraphicsEngine::checkSupportedExtensions()
//	{
//		uint32_t extensionCount = 0;
//		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//		vector<VkExtensionProperties> supportedExtensions(extensionCount);
//		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data());
//
//		printAvailableExtensions(supportedExtensions);
//		checkGLFWRequiredExtensions(supportedExtensions);
//	}
//
//	void GraphicsEngine::printAvailableExtensions(vector<VkExtensionProperties> extensions)
//	{
//		cout << "All the available Vulkan extensions are:" << endl;
//
//		for (const auto& extension : extensions)
//		{
//			cout << '\t' << extension.extensionName << endl;
//		}
//	}
//
//	void GraphicsEngine::checkGLFWRequiredExtensions(vector<VkExtensionProperties> extensions)
//	{
//		uint32_t requiredExtensionCount = 0;
//		glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
//		const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
//
//		cout << "Check if all the GLFW extensions are supported:" << endl;
//
//		int unsupportedExtensions = 0;
//		for (unsigned int i = 0; i < requiredExtensionCount; i++)
//		{
//			bool isSupported = false;
//			for (const auto& extension : extensions)
//			{
//				if (strcmp(requiredExtensions[i], extension.extensionName) == 0)
//				{
//					isSupported = true;
//					break;
//				}
//			}
//
//			if (!isSupported)
//			{
//				unsupportedExtensions++;
//			}
//		}
//
//		if (unsupportedExtensions == 0)
//		{
//			cout << "All GLFW required extensions are supported!" << endl;
//		}
//		else
//		{
//			cout << "There are " << unsupportedExtensions << " unsupported extensions of GLFW!" << endl;
//		}
//	}
//
//	bool GraphicsEngine::checkValidationLayerSupport()
//	{
//		uint32_t layerCount;
//		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
//		vector<VkLayerProperties> availableLayers(layerCount);
//		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
//
//		for (const char* layerName : validationLayers)
//		{
//			bool layerFound = false;
//
//			for (const auto& layerProperties : availableLayers)
//			{
//				if (strcmp(layerName, layerProperties.layerName) == 0)
//				{
//					layerFound = true;
//					break;
//				}
//			}
//
//			if (!layerFound)
//			{
//				return false;
//			}
//		}
//
//		return true;
//	}
//
//	void GraphicsEngine::setupDebugMessenger()
//	{
//		if (useValidationLayers)
//		{
//			VkDebugUtilsMessengerCreateInfoEXT createInfo;
//			populateDebugMessengerCreateInfo(createInfo);
//
//			if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
//			{
//				throw std::runtime_error(getErrorStr(Error::VULKAN_FAIL_DEBUG_MESSENGER));
//			}
//		}
//	}
//
//	void GraphicsEngine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
//	{
//		createInfo = {};
//		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
//		createInfo.pfnUserCallback = debugCallback;
//		createInfo.pUserData = nullptr;
//	}
//
//	VkResult GraphicsEngine::CreateDebugUtilsMessengerEXT(VkInstance instance,
//														  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
//														  const VkAllocationCallbacks* pAllocator,
//														  VkDebugUtilsMessengerEXT* pDebugMessenger)
//	{
//		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
//		if (func != nullptr)
//		{
//			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
//		}
//		else
//		{
//			return VK_ERROR_EXTENSION_NOT_PRESENT;
//		}
//	}
//
//	void GraphicsEngine::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
//	{
//		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
//		if (func != nullptr)
//		{
//			func(instance, debugMessenger, pAllocator);
//		}
//	}
//
//	VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsEngine::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//																 VkDebugUtilsMessageTypeFlagsEXT messageType,
//																 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
//																 void* pUserData)
//	{
//		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
//		{
//			std::cerr << "Validation layer: " << pCallbackData->pMessage << endl;
//		}
//
//		return VK_FALSE;
//	}
//
//	void GraphicsEngine::pickPhysicalDevice()
//	{
//		uint32_t deviceCount = 0;
//		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
//
//		if (deviceCount == 0)
//		{
//			throw std::runtime_error(getErrorStr(Error::VULKAN_FAIL_NO_GPU_PRESENT));
//		}
//		else
//		{
//			vector<VkPhysicalDevice> devices(deviceCount);
//			vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
//			std::multimap<int, VkPhysicalDevice> candidates;
//
//			for (const auto &device : devices)
//			{
//				// if it is suitable, we rate it
//				if (isDeviceSuitable(device))
//				{
//					int score = rateDeviceSuitability(device);
//					candidates.insert(std::make_pair(score, device));
//				}
//			}
//
//			// rbegin returns an iterator from the highest key to the lowest key
//			// if there is at least one element, rbegin and rend point to different memory areas
//			if (candidates.rbegin() != candidates.rend())
//			{
//				physicalDevice = candidates.rbegin()->second;
//			}
//
//			if (physicalDevice == VK_NULL_HANDLE)
//			{
//				throw std::runtime_error(getErrorStr(Error::VULKAN_FAIL_NO_GPU_IS_SUITABLE));
//			}
//		}
//	}
//
//	void GraphicsEngine::createLogicalDevice() {
//		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
//
//
//		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
//		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
//
//		float queuePriority = 1.0f;
//		for (uint32_t queueFamily : uniqueQueueFamilies) {
//			VkDeviceQueueCreateInfo queueCreateInfo{};
//			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//			queueCreateInfo.queueFamilyIndex = queueFamily;
//			queueCreateInfo.queueCount = 1;
//			queueCreateInfo.pQueuePriorities = &queuePriority;
//			queueCreateInfos.push_back(queueCreateInfo);
//		}
//
//		VkPhysicalDeviceFeatures deviceFeatures{};
//		VkDeviceCreateInfo createInfo{};
//		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//		createInfo.pQueueCreateInfos = queueCreateInfos.data();
//		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
//
//		createInfo.pEnabledFeatures = &deviceFeatures;
//		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
//		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
//
//		if (useValidationLayers) {
//			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//			createInfo.ppEnabledLayerNames = validationLayers.data();
//		}
//		else {
//			createInfo.enabledLayerCount = 0;
//		}
//		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create logical device!");
//		}
//		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
//		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
//	}
//
//	bool GraphicsEngine::isDeviceSuitable(VkPhysicalDevice device)
//	{
//		// TODO: is it not parametric in any way and it should be
//		VkPhysicalDeviceFeatures deviceFeatures;
//		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
//		QueueFamilyIndices indices = findQueueFamilies(device);
//
//		bool swapChainAdequate = false;
//		bool extensionsSupported = checkDeviceExtensionSupport(device);
//		if (extensionsSupported) {
//			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
//			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
//		}
//
//		return deviceFeatures.geometryShader && indices.isComplete() && extensionsSupported && swapChainAdequate;
//	}
//
//	int GraphicsEngine::rateDeviceSuitability(VkPhysicalDevice device)
//	{
//		// TODO: it is not parametric in any way and it should be
//		int score = 0;
//		VkPhysicalDeviceProperties deviceProperties;
//		vkGetPhysicalDeviceProperties(device, &deviceProperties);
//
//		// if the GPU is dedicated, it is much better
//		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
//		{
//			score += 1000;
//		}
//
//		return score;
//	}
//
//	QueueFamilyIndices GraphicsEngine::findQueueFamilies(VkPhysicalDevice device)
//	{
//		QueueFamilyIndices indices;
//
//		uint32_t queueFamilyCount = 0;
//		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
//
//		vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
//
//		int i = 0;
//		for (const auto& queueFamily : queueFamilies)
//		{
//			VkBool32 presentSupport = false;
//			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
//
//			if (presentSupport) {
//				indices.presentFamily = i;
//			}
//
//			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
//			{
//				indices.graphicsFamily = i;
//			}
//
//			if (indices.isComplete())
//			{
//				break;
//			}
//			i++;
//		}
//
//		return indices;
//	}
//
//	void GraphicsEngine::createSurface() {
//		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create window surface!");
//		}
//	}
//
//	bool GraphicsEngine::checkDeviceExtensionSupport(VkPhysicalDevice device) {
//		//TODO required extension may be passed as argument to this function instead of being declared here
//
//		uint32_t extensionCount;
//		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
//
//		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
//
//		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
//
//		for (const auto& extension : availableExtensions) {
//			requiredExtensions.erase(extension.extensionName);
//		}
//
//		return requiredExtensions.empty();
//	}
//
//	SwapChainSupportDetails GraphicsEngine::querySwapChainSupport(VkPhysicalDevice device) {
//		SwapChainSupportDetails details;
//		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
//		uint32_t formatCount;
//		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
//
//		if (formatCount != 0) {
//			details.formats.resize(formatCount);
//			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
//		}
//		uint32_t presentModeCount;
//		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
//
//		if (presentModeCount != 0) {
//			details.presentModes.resize(presentModeCount);
//			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
//		}
//
//		return details;
//	}
//
//	VkSurfaceFormatKHR GraphicsEngine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
//		for (const auto& availableFormat : availableFormats) {
//			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
//				return availableFormat;
//			}
//		}
//
//		return availableFormats[0];
//	}
//
//	VkPresentModeKHR GraphicsEngine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
//		for (const auto& availablePresentMode : availablePresentModes) {
//			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
//				return availablePresentMode;
//			}
//		}
//
//		return VK_PRESENT_MODE_FIFO_KHR;
//	}
//
//	VkExtent2D GraphicsEngine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
//		if (capabilities.currentExtent.width != UINT32_MAX) {
//			return capabilities.currentExtent;
//		}
//		else {
//			int width, height;
//			glfwGetFramebufferSize(window, &width, &height);
//
//			VkExtent2D actualExtent = {
//				static_cast<uint32_t>(width),
//				static_cast<uint32_t>(height)
//			};
//
//			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
//			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
//
//			return actualExtent;
//		}
//	}
//
//	void GraphicsEngine::createSwapChain() {
//		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
//
//		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
//		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
//		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
//		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
//		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
//			imageCount = swapChainSupport.capabilities.maxImageCount;
//		}
//		VkSwapchainCreateInfoKHR createInfo{};
//		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//		createInfo.surface = surface;
//		createInfo.minImageCount = imageCount;
//		createInfo.imageFormat = surfaceFormat.format;
//		createInfo.imageColorSpace = surfaceFormat.colorSpace;
//		createInfo.imageExtent = extent;
//		createInfo.imageArrayLayers = 1;
//		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
//		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
//		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
//
//		if (indices.graphicsFamily != indices.presentFamily) {
//			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
//			createInfo.queueFamilyIndexCount = 2;
//			createInfo.pQueueFamilyIndices = queueFamilyIndices;
//		}
//		else {
//			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//			createInfo.queueFamilyIndexCount = 0; // Optional
//			createInfo.pQueueFamilyIndices = nullptr; // Optional
//		}
//		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
//		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//		createInfo.presentMode = presentMode;
//		createInfo.clipped = VK_TRUE;
//		createInfo.oldSwapchain = VK_NULL_HANDLE;
//		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create swap chain!");
//		}
//		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
//		swapChainImages.resize(imageCount);
//		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
//		swapChainImageFormat = surfaceFormat.format;
//		swapChainExtent = extent;
//	}
//
//	void GraphicsEngine::createImageViews() {
//		swapChainImageViews.resize(swapChainImages.size());
//		for (size_t i = 0; i < swapChainImages.size(); i++) {
//			VkImageViewCreateInfo createInfo{};
//			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//			createInfo.image = swapChainImages[i];
//			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
//			createInfo.format = swapChainImageFormat;
//			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
//			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
//			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
//			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
//			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//			createInfo.subresourceRange.baseMipLevel = 0;
//			createInfo.subresourceRange.levelCount = 1;
//			createInfo.subresourceRange.baseArrayLayer = 0;
//			createInfo.subresourceRange.layerCount = 1;
//			if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
//				throw std::runtime_error("failed to create image views!");
//			}
//		}
//	}
//
//
//
//	void GraphicsEngine::createGraphicsPipeline() {
//		auto vertShaderCode = readFile("shaders/vert.spv");
//		auto fragShaderCode = readFile("shaders/frag.spv");
//		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
//		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
//
//		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
//		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
//		vertShaderStageInfo.module = vertShaderModule;
//		vertShaderStageInfo.pName = "main";
//
//		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
//		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//		fragShaderStageInfo.module = fragShaderModule;
//		fragShaderStageInfo.pName = "main";
//
//		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
//
//		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
//		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//		vertexInputInfo.vertexBindingDescriptionCount = 0;
//		vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
//		vertexInputInfo.vertexAttributeDescriptionCount = 0;
//		vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional
//
//		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
//		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//		inputAssembly.primitiveRestartEnable = VK_FALSE;
//
//		VkViewport viewport{};
//		viewport.x = 0.0f;
//		viewport.y = 0.0f;
//		viewport.width = (float)swapChainExtent.width;
//		viewport.height = (float)swapChainExtent.height;
//		viewport.minDepth = 0.0f;
//		viewport.maxDepth = 1.0f;
//
//		VkRect2D scissor{};
//		scissor.offset = { 0, 0 };
//		scissor.extent = swapChainExtent;
//
//		VkPipelineViewportStateCreateInfo viewportState{};
//		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//		viewportState.viewportCount = 1;
//		viewportState.pViewports = &viewport;
//		viewportState.scissorCount = 1;
//		viewportState.pScissors = &scissor;
//
//		VkPipelineRasterizationStateCreateInfo rasterizer{};
//		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//		rasterizer.depthClampEnable = VK_FALSE;
//		rasterizer.rasterizerDiscardEnable = VK_FALSE;
//		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
//		rasterizer.lineWidth = 1.0f;
//		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
//		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
//		rasterizer.depthBiasEnable = VK_FALSE;
//		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
//		rasterizer.depthBiasClamp = 0.0f; // Optional
//		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
//
//		VkPipelineMultisampleStateCreateInfo multisampling{};
//		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//		multisampling.sampleShadingEnable = VK_FALSE;
//		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//		multisampling.minSampleShading = 1.0f; // Optional
//		multisampling.pSampleMask = nullptr; // Optional
//		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
//		multisampling.alphaToOneEnable = VK_FALSE; // Optional
//
//		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
//		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//		colorBlendAttachment.blendEnable = VK_TRUE;
//		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
//		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
//		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
//		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
//		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
//
//		VkPipelineColorBlendStateCreateInfo colorBlending{};
//		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//		colorBlending.logicOpEnable = VK_FALSE;
//		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
//		colorBlending.attachmentCount = 1;
//		colorBlending.pAttachments = &colorBlendAttachment;
//		colorBlending.blendConstants[0] = 0.0f; // Optional
//		colorBlending.blendConstants[1] = 0.0f; // Optional
//		colorBlending.blendConstants[2] = 0.0f; // Optional
//		colorBlending.blendConstants[3] = 0.0f; // Optional
//
//		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
//		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//		pipelineLayoutInfo.setLayoutCount = 0; // Optional
//		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
//		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
//		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
//
//		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create pipeline layout!");
//		}
//
//		VkGraphicsPipelineCreateInfo pipelineInfo{};
//		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//		pipelineInfo.stageCount = 2;
//		pipelineInfo.pStages = shaderStages;
//		pipelineInfo.pVertexInputState = &vertexInputInfo;
//		pipelineInfo.pInputAssemblyState = &inputAssembly;
//		pipelineInfo.pViewportState = &viewportState;
//		pipelineInfo.pRasterizationState = &rasterizer;
//		pipelineInfo.pMultisampleState = &multisampling;
//		pipelineInfo.pDepthStencilState = nullptr; // Optional
//		pipelineInfo.pColorBlendState = &colorBlending;
//		pipelineInfo.pDynamicState = nullptr; // Optional
//		pipelineInfo.layout = pipelineLayout;
//		pipelineInfo.renderPass = renderPass;
//		pipelineInfo.subpass = 0;
//		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
//		pipelineInfo.basePipelineIndex = -1; // Optional
//
//		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create graphics pipeline!");
//		}
//
//		vkDestroyShaderModule(device, fragShaderModule, nullptr);
//		vkDestroyShaderModule(device, vertShaderModule, nullptr);
//	}
//
//	void GraphicsEngine::createRenderPass() {
//		VkAttachmentDescription colorAttachment{};
//		colorAttachment.format = swapChainImageFormat;
//		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//
//		VkAttachmentReference colorAttachmentRef{};
//		colorAttachmentRef.attachment = 0;
//		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//		VkSubpassDescription subpass{};
//		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//		subpass.colorAttachmentCount = 1;
//		subpass.pColorAttachments = &colorAttachmentRef;
//
//		VkRenderPassCreateInfo renderPassInfo{};
//		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//		renderPassInfo.attachmentCount = 1;
//		renderPassInfo.pAttachments = &colorAttachment;
//		renderPassInfo.subpassCount = 1;
//		renderPassInfo.pSubpasses = &subpass;
//
//		VkSubpassDependency dependency{};
//		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//		dependency.dstSubpass = 0;
//		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//		dependency.srcAccessMask = 0;
//		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//		renderPassInfo.dependencyCount = 1;
//		renderPassInfo.pDependencies = &dependency;
//
//		if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create render pass!");
//		}
//
//	}
//
//	void GraphicsEngine::createFramebuffers() {
//		swapChainFramebuffers.resize(swapChainImageViews.size());
//		for (size_t i = 0; i < swapChainImageViews.size(); i++) {
//			VkImageView attachments[] = {
//				swapChainImageViews[i]
//			};
//
//			VkFramebufferCreateInfo framebufferInfo{};
//			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//			framebufferInfo.renderPass = renderPass;
//			framebufferInfo.attachmentCount = 1;
//			framebufferInfo.pAttachments = attachments;
//			framebufferInfo.width = swapChainExtent.width;
//			framebufferInfo.height = swapChainExtent.height;
//			framebufferInfo.layers = 1;
//
//			if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
//				throw std::runtime_error("failed to create framebuffer!");
//			}
//		}
//	}
//
//	void GraphicsEngine::createCommandPool() {
//		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);
//
//		VkCommandPoolCreateInfo poolInfo{};
//		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
//		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
//		if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create command pool!");
//		}
//	}
//
//	void GraphicsEngine::createCommandBuffer() {
//		commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
//
//		VkCommandBufferAllocateInfo allocInfo{};
//		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//		allocInfo.commandPool = commandPool;
//		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
//
//		if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
//			throw std::runtime_error("failed to allocate command buffers!");
//		}
//	}
//	
//	void GraphicsEngine::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
//		VkCommandBufferBeginInfo beginInfo{};
//		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//		beginInfo.flags = 0; // Optional
//		beginInfo.pInheritanceInfo = nullptr; // Optional
//
//		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
//			throw std::runtime_error("failed to begin recording command buffer!");
//		}
//
//		VkRenderPassBeginInfo renderPassInfo{};
//		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//		renderPassInfo.renderPass = renderPass;
//		renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
//		renderPassInfo.renderArea.offset = { 0, 0 };
//		renderPassInfo.renderArea.extent = swapChainExtent;
//
//		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
//		renderPassInfo.clearValueCount = 1;
//		renderPassInfo.pClearValues = &clearColor;
//
//		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
//		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
//		vkCmdEndRenderPass(commandBuffer);
//		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
//			throw std::runtime_error("failed to record command buffer!");
//		}
//	}
//
//	void GraphicsEngine::drawFrame() {
//		vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
//		vkResetFences(device, 1, &inFlightFences[currentFrame]);
//		uint32_t imageIndex;
//		vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
//		vkResetCommandBuffer(commandBuffers[currentFrame], 0);
//		recordCommandBuffer(commandBuffers[currentFrame], imageIndex);
//		VkSubmitInfo submitInfo{};
//		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//
//		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
//		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
//		submitInfo.waitSemaphoreCount = 1;
//		submitInfo.pWaitSemaphores = waitSemaphores;
//		submitInfo.pWaitDstStageMask = waitStages;
//		submitInfo.commandBufferCount = 1;
//		submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
//
//		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
//		submitInfo.signalSemaphoreCount = 1;
//		submitInfo.pSignalSemaphores = signalSemaphores;
//		 
//		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
//			throw std::runtime_error("failed to submit draw command buffer!");
//		}
//
//		VkPresentInfoKHR presentInfo{};
//		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//
//		presentInfo.waitSemaphoreCount = 1;
//		presentInfo.pWaitSemaphores = signalSemaphores;
//
//		VkSwapchainKHR swapChains[] = { swapChain };
//		presentInfo.swapchainCount = 1;
//		presentInfo.pSwapchains = swapChains;
//		presentInfo.pImageIndices = &imageIndex;
//		presentInfo.pResults = nullptr; // Optional
//		vkQueuePresentKHR(presentQueue, &presentInfo);
//		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
//	}
//
//	void GraphicsEngine::createSyncObjects() {
//		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
//		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
//		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
//
//		VkSemaphoreCreateInfo semaphoreInfo{};
//		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
//
//		VkFenceCreateInfo fenceInfo{};
//		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
//		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
//
//		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
//				vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
//				vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
//
//				throw std::runtime_error("failed to create synchronization objects for a frame!");
//			}
//		}
//	}
//
//	std::vector<char> GraphicsEngine::readFile(const std::string& filename) {
//		std::ifstream file(filename, std::ios::ate | std::ios::binary);
//
//		if (!file.is_open()) {
//			throw std::runtime_error("failed to open file!");
//		}
//
//		size_t fileSize = (size_t)file.tellg();
//		std::vector<char> buffer(fileSize);
//		file.seekg(0);
//		file.read(buffer.data(), fileSize);
//		file.close();
//
//		return buffer;
//	}
//
//	VkShaderModule GraphicsEngine::createShaderModule(const std::vector<char>& code) {
//		VkShaderModuleCreateInfo createInfo{};
//		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//		createInfo.codeSize = code.size();
//		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
//		VkShaderModule shaderModule;
//		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create shader module!");
//		}
//		return shaderModule;
//	}
//
//	void GraphicsEngine::mainLoop()
//	{
//		while (!glfwWindowShouldClose(window))
//		{
//			glfwPollEvents();
//			drawFrame();
//		}
//
//		vkDeviceWaitIdle(device);
//	}
//
//	void GraphicsEngine::cleanup()
//	{
//		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//			vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
//			vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
//			vkDestroyFence(device, inFlightFences[i], nullptr);
//		}
//
//		vkDestroyCommandPool(device, commandPool, nullptr);
//
//		for (auto framebuffer : swapChainFramebuffers) {
//			vkDestroyFramebuffer(device, framebuffer, nullptr);
//		}
//
//		vkDestroyPipeline(device, graphicsPipeline, nullptr);
//
//		vkDestroyRenderPass(device, renderPass, nullptr);
//
//		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
//
//		for (auto imageView : swapChainImageViews) {
//			vkDestroyImageView(device, imageView, nullptr);
//		}
//
//		vkDestroyDevice(device, nullptr);
//
//		if (useValidationLayers)
//		{
//			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
//		}
//
//		vkDestroySurfaceKHR(instance, surface, nullptr);
//
//		vkDestroyInstance(instance, nullptr);
//
//		glfwDestroyWindow(window);
//
//		glfwTerminate();
//	}
//}