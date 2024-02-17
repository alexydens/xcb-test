/* Include guard */
#if !defined (GRAPHICS_H)
#define GRAPHICS_H

/* Base library */
#include <base.h>
#include <logging.h>

/* Vulkan, graphics API */
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

/* Consts */
#define USE_VALIDATION_LAYERS     1
const char* g_ppExtensions[2] = {
  VK_KHR_SURFACE_EXTENSION_NAME,
  VK_KHR_XCB_SURFACE_EXTENSION_NAME
};
const u32 g_extensionCount = 2;
#if USE_VALIDATION_LAYERS
const char* const g_ppLayers[2] = {
  "VK_LAYER_KHRONOS_validation"
};
const u32 g_layerCount = 1;
#else
const char** g_ppLayers = NULL;
const u32 g_layerCount = 0;
#endif

/* State of graphics stuff */
typedef struct {
  VkApplicationInfo app_info; /* Some information about the app */
  VkInstance instance;        /* The vulkan instance */
} GraphicsState;

/* Create instance */
void _create_instance(GraphicsState* state) {
  /* Fill in app info */
  state->app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  state->app_info.pNext = NULL;
  state->app_info.pApplicationName = "Vulkan test";
  state->app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  state->app_info.pEngineName = "No engine";
  state->app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  state->app_info.apiVersion = VK_VERSION_1_0;
  /* Fill in instance create info */
  VkInstanceCreateInfo createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pNext = NULL;
  createInfo.pApplicationInfo = &state->app_info;
  createInfo.flags = 0;
  /* Extensions */
  createInfo.enabledExtensionCount = g_extensionCount;
  createInfo.ppEnabledExtensionNames = g_ppExtensions;
  /* Validation layers */
  u32 layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);
  VkLayerProperties* props = malloc(layerCount * sizeof(VkLayerProperties));
  vkEnumerateInstanceLayerProperties(&layerCount, props);
  for (u32 i = 0; i < g_layerCount; i++) {
    const char* layerName = g_ppLayers[i];
    bool found = false;
    for (u32 j = 0; j < layerCount; j++) {
      VkLayerProperties layer = props[j];
      if (strcmp(layer.layerName, layerName) == 0) {
        found = true;
        break;
      }
    }
    ASSERT(found);
  }
  log_info("Printing available layers...");
  for (u32 i = 0; i < layerCount; i++) {
    VkLayerProperties layer = props[i];
    log("\t-> %s", layer.layerName);
  }
  log_newline();
  free(props);
  createInfo.enabledLayerCount = g_layerCount;
  createInfo.ppEnabledLayerNames = g_ppLayers;
  /* Actual creation */
  VkResult res = vkCreateInstance(&createInfo, NULL, &state->instance);
  if (res != VK_SUCCESS)
    log_fatal("Failed to create instance.");
}
/* Destroy instance */
void _destroy_instance(GraphicsState* state) {
  vkDestroyInstance(state->instance, NULL);
}

/* Initialize graphics */
GraphicsState init_graphics(void) {
  GraphicsState state;
  _create_instance(&state);
  return state;
}
/* Quit graphics */
void quit_graphics(GraphicsState* state) {
  _destroy_instance(state);
}

#endif /* GRAPHICS_H */
