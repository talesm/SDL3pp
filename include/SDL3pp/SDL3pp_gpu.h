#ifndef SDL3PP_GPU_H_
#define SDL3PP_GPU_H_

#include <SDL3/SDL_gpu.h>
#include "SDL3pp_pixels.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_surface.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategoryGPU 3D Rendering and GPU Compute
 *
 * The GPU API offers a cross-platform way for apps to talk to modern graphics
 * hardware. It offers both 3D graphics and compute support, in the style of
 * Metal, Vulkan, and Direct3D 12.
 *
 * A basic workflow might be something like this:
 *
 * The app creates a GPU device with GPUDevice.GPUDevice(), and assigns it to
 * a window with GPUDevice.ClaimWindow()--although strictly speaking you
 * can render offscreen entirely, perhaps for image processing, and not use a
 * window at all.
 *
 * Next, the app prepares static data (things that are created once and used
 * over and over). For example:
 *
 * - Shaders (programs that run on the GPU): use GPUShader.GPUShader().
 * - Vertex buffers (arrays of geometry data) and other rendering data: use
 *   GPUBuffer.GPUBuffer() and GPUCopyPass.UploadToBuffer().
 * - Textures (images): use GPUTexture.GPUTexture() and
 *   GPUCopyPass.UploadToTexture().
 * - Samplers (how textures should be read from): use GPUSampler.GPUSampler().
 * - Render pipelines (precalculated rendering state): use
 *   GPUGraphicsPipeline.GPUGraphicsPipeline()
 *
 * To render, the app creates one or more command buffers, with
 * GPUDevice.AcquireCommandBuffer(). Command buffers collect rendering
 * instructions that will be submitted to the GPU in batch. Complex scenes can
 * use multiple command buffers, maybe configured across multiple threads in
 * parallel, as long as they are submitted in the correct order, but many apps
 * will just need one command buffer per frame.
 *
 * Rendering can happen to a texture (what other APIs call a "render target")
 * or it can happen to the swapchain texture (which is just a special texture
 * that represents a window's contents). The app can use
 * GPUCommandBuffer.WaitAndAcquireSwapchainTexture() to render to the window.
 *
 * Rendering actually happens in a Render Pass, which is encoded into a
 * command buffer. One can encode multiple render passes (or alternate between
 * render and compute passes) in a single command buffer, but many apps might
 * simply need a single render pass in a single command buffer. Render Passes
 * can render to up to four color textures and one depth texture
 * simultaneously. If the set of textures being rendered to needs to change,
 * the Render Pass must be ended and a new one must be begun.
 *
 * The app calls GPUCommandBuffer.BeginRenderPass(). Then it sets states it
 * needs for each draw:
 *
 * - GPURenderPass.BindPipeline()
 * - GPURenderPass.SetViewport()
 * - GPURenderPass.BindVertexBuffers()
 * - GPURenderPass.BindVertexSamplers()
 * - etc
 *
 * Then, make the actual draw commands with these states:
 *
 * - GPURenderPass.DrawPrimitives()
 * - GPURenderPass.DrawPrimitivesIndirect()
 * - GPURenderPass.DrawIndexedPrimitivesIndirect()
 * - etc
 *
 * After all the drawing commands for a pass are complete, the app should call
 * GPURenderPass.End(). Once a render pass ends all render-related state is
 * reset.
 *
 * The app can begin new Render Passes and make new draws in the same command
 * buffer until the entire scene is rendered.
 *
 * Once all of the render commands for the scene are complete, the app calls
 * GPUCommandBuffer.Submit() to send it to the GPU for processing.
 *
 * If the app needs to read back data from texture or buffers, the API has an
 * efficient way of doing this, provided that the app is willing to tolerate
 * some latency. When the app uses GPUCopyPass.DownloadFromTexture() or
 * GPUCopyPass.DownloadFromBuffer(), submitting the command buffer with
 * GPUCommandBuffer.SubmitAndAcquireFence() will return a fence handle that
 * the app can poll or wait on in a thread. Once the fence indicates that the
 * command buffer is done processing, it is safe to read the downloaded data.
 * Make sure to call GPUDevice.ReleaseFence() when done with the fence.
 *
 * The API also has "compute" support. The app calls
 * GPUCommandBuffer.BeginComputePass() with compute-writeable textures and/or
 * buffers, which can be written to in a compute shader. Then it sets states it
 * needs for the compute dispatches:
 *
 * - GPUComputePass.BindPipeline()
 * - GPUComputePass.BindStorageBuffers()
 * - GPUComputePass.BindStorageTextures()
 *
 * Then, dispatch compute work:
 *
 * - GPUComputePass.Dispatch()
 *
 * For advanced users, this opens up powerful GPU-driven workflows.
 *
 * Graphics and compute pipelines require the use of shaders, which as
 * mentioned above are small programs executed on the GPU. Each backend
 * (Vulkan, Metal, D3D12) requires a different shader format. When the app
 * creates the GPU device, the app lets the device know which shader formats
 * the app can provide. It will then select the appropriate backend depending
 * on the available shader formats and the backends available on the platform.
 * When creating shaders, the app must provide the correct shader format for
 * the selected backend. If you would like to learn more about why the API
 * works this way, there is a detailed
 * [blog post](https://moonside.games/posts/layers-all-the-way-down/)
 * explaining this situation.
 *
 * It is optimal for apps to pre-compile the shader formats they might use,
 * but for ease of use SDL provides a separate project,
 * [SDL_shadercross](https://github.com/libsdl-org/SDL_shadercross)
 * , for performing runtime shader cross-compilation. It also has a CLI
 * interface for offline precompilation as well.
 *
 * This is an extremely quick overview that leaves out several important
 * details. Already, though, one can see that GPU programming can be quite
 * complex! If you just need simple 2D graphics, the
 * [Render API](https://wiki.libsdl.org/SDL3/CategoryRender)
 * is much easier to use but still hardware-accelerated. That said, even for
 * 2D applications the performance benefits and expressiveness of the GPU API
 * are significant.
 *
 * The GPU API targets a feature set with a wide range of hardware support and
 * ease of portability. It is designed so that the app won't have to branch
 * itself by querying feature support. If you need cutting-edge features with
 * limited hardware support, this API is probably not for you.
 *
 * Examples demonstrating proper usage of this API can be found
 * [here](https://github.com/TheSpydog/SDL_gpu_examples).
 *
 * ## Performance considerations
 *
 * Here are some basic tips for maximizing your rendering performance.
 *
 * - Beginning a new render pass is relatively expensive. Use as few render
 *   passes as you can.
 * - Minimize the amount of state changes. For example, binding a pipeline is
 *   relatively cheap, but doing it hundreds of times when you don't need to
 *   will slow the performance significantly.
 * - Perform your data uploads as early as possible in the frame.
 * - Don't churn resources. Creating and releasing resources is expensive.
 *   It's better to create what you need up front and cache it.
 * - Don't use uniform buffers for large amounts of data (more than a matrix
 *   or so). Use a storage buffer instead.
 * - Use cycling correctly. There is a detailed explanation of cycling further
 *   below.
 * - Use culling techniques to minimize pixel writes. The less writing the GPU
 *   has to do the better. Culling can be a very advanced topic but even
 *   simple culling techniques can boost performance significantly.
 *
 * In general try to remember the golden rule of performance: doing things is
 * more expensive than not doing things. Don't Touch The Driver!
 *
 * ## FAQ
 *
 * **Question: When are you adding more advanced features, like ray tracing or
 * mesh shaders?**
 *
 * Answer: We don't have immediate plans to add more bleeding-edge features,
 * but we certainly might in the future, when these features prove worthwhile,
 * and reasonable to implement across several platforms and underlying APIs.
 * So while these things are not in the "never" category, they are definitely
 * not "near future" items either.
 *
 * **Question: Why is my shader not working?**
 *
 * Answer: A common oversight when using shaders is not properly laying out
 * the shader resources/registers correctly. The GPU API is very strict with
 * how it wants resources to be laid out and it's difficult for the API to
 * automatically validate shaders to see if they have a compatible layout. See
 * the documentation for GPUShader.GPUShader() and
 * GPUComputePipeline.GPUComputePipeline() for information on the expected
 * layout.
 *
 * Another common issue is not setting the correct number of samplers,
 * textures, and buffers in GPUShaderCreateInfo. If possible use shader
 * reflection to extract the required information from the shader
 * automatically instead of manually filling in the struct's values.
 *
 * **Question: My application isn't performing very well. Is this the GPU
 * API's fault?**
 *
 * Answer: No. Long answer: The GPU API is a relatively thin layer over the
 * underlying graphics API. While it's possible that we have done something
 * inefficiently, it's very unlikely especially if you are relatively
 * inexperienced with GPU rendering. Please see the performance tips above and
 * make sure you are following them. Additionally, tools like RenderDoc can be
 * very helpful for diagnosing incorrect behavior and performance issues.
 *
 * ## System Requirements
 *
 * **Vulkan:** Supported on Windows, Linux, Nintendo Switch, and certain
 * Android devices. Requires Vulkan 1.0 with the following extensions and
 * device features:
 *
 * - `VK_KHR_swapchain`
 * - `VK_KHR_maintenance1`
 * - `independentBlend`
 * - `imageCubeArray`
 * - `depthClamp`
 * - `shaderClipDistance`
 * - `drawIndirectFirstInstance`
 *
 * **D3D12:** Supported on Windows 10 or newer, Xbox One (GDK), and Xbox
 * Series X|S (GDK). Requires a GPU that supports DirectX 12 Feature Level 11_0
 * and Resource Binding Tier 2 or above.
 *
 * **Metal:** Supported on macOS 10.14+ and iOS/tvOS 13.0+. Hardware
 * requirements vary by operating system:
 *
 * - macOS requires an Apple Silicon or
 *   [Intel Mac2
 * family](https://developer.apple.com/documentation/metal/mtlfeatureset/mtlfeatureset_macos_gpufamily2_v1?language=objc)
 *   GPU
 * - iOS/tvOS requires an A9 GPU or newer
 * - iOS Simulator and tvOS Simulator are unsupported
 *
 * ## Uniform Data
 *
 * Uniforms are for passing data to shaders. The uniform data will be constant
 * across all executions of the shader.
 *
 * There are 4 available uniform slots per shader stage (where the stages are
 * vertex, fragment, and compute). Uniform data pushed to a slot on a stage
 * keeps its value throughout the command buffer until you call the relevant
 * Push function on that slot again.
 *
 * For example, you could write your vertex shaders to read a camera matrix
 * from uniform binding slot 0, push the camera matrix at the start of the
 * command buffer, and that data will be used for every subsequent draw call.
 *
 * It is valid to push uniform data during a render or compute pass.
 *
 * Uniforms are best for pushing small amounts of data. If you are pushing
 * more than a matrix or two per call you should consider using a storage
 * buffer instead.
 *
 * ## A Note On Cycling
 *
 * When using a command buffer, operations do not occur immediately - they
 * occur some time after the command buffer is submitted.
 *
 * When a resource is used in a pending or active command buffer, it is
 * considered to be "bound". When a resource is no longer used in any pending
 * or active command buffers, it is considered to be "unbound".
 *
 * If data resources are bound, it is unspecified when that data will be
 * unbound unless you acquire a fence when submitting the command buffer and
 * wait on it. However, this doesn't mean you need to track resource usage
 * manually.
 *
 * All of the functions and structs that involve writing to a resource have a
 * "cycle" bool. GPUTransferBuffer, GPUBuffer, and GPUTexture all
 * effectively function as ring buffers on internal resources. When cycle is
 * true, if the resource is bound, the cycle rotates to the next unbound
 * internal resource, or if none are available, a new one is created. This
 * means you don't have to worry about complex state tracking and
 * synchronization as long as cycling is correctly employed.
 *
 * For example: you can call GPUDevice.MapTransferBuffer(), write texture data,
 * GPUDevice.UnmapTransferBuffer(), and then GPUCopyPass.UploadToTexture(). The
 * next time you write texture data to the transfer buffer, if you set the cycle
 * param to true, you don't have to worry about overwriting any data that is
 * not yet uploaded.
 *
 * Another example: If you are using a texture in a render pass every frame,
 * this can cause a data dependency between frames. If you set cycle to true
 * in the GPUColorTargetInfo struct, you can prevent this data dependency.
 *
 * Cycling will never undefine already bound data. When cycling, all data in
 * the resource is considered to be undefined for subsequent commands until
 * that data is written again. You must take care not to read undefined data.
 *
 * Note that when cycling a texture, the entire texture will be cycled, even
 * if only part of the texture is used in the call, so you must consider the
 * entire texture to contain undefined data after cycling.
 *
 * You must also take care not to overwrite a section of data that has been
 * referenced in a command without cycling first. It is OK to overwrite
 * unreferenced data in a bound resource without cycling, but overwriting a
 * section of data that has already been referenced will produce unexpected
 * results.
 *
 * @{
 */

// Forward decl
struct GPUDevice;

/// Alias to raw representation for GPUDevice.
using GPUDeviceRaw = SDL_GPUDevice*;

// Forward decl
struct GPUDeviceRef;

/// Safely wrap GPUDevice for non owning parameters
struct GPUDeviceParam
{
  GPUDeviceRaw value; ///< parameter's GPUDeviceRaw

  /// Constructs from GPUDeviceRaw
  constexpr GPUDeviceParam(GPUDeviceRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr GPUDeviceParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const GPUDeviceParam& other) const = default;

  /// Converts to underlying GPUDeviceRaw
  constexpr operator GPUDeviceRaw() const { return value; }
};

/// Alias to raw representation for GPUBuffer.
using GPUBufferRaw = SDL_GPUBuffer*;

/// Alias to raw representation for GPUTransferBuffer.
using GPUTransferBufferRaw = SDL_GPUTransferBuffer*;

/// Alias to raw representation for GPUTexture.
using GPUTextureRaw = SDL_GPUTexture*;

/// Alias to raw representation for GPUSampler.
using GPUSamplerRaw = SDL_GPUSampler*;

/// Alias to raw representation for GPUShader.
using GPUShaderRaw = SDL_GPUShader*;

/// Alias to raw representation for GPUComputePipeline.
using GPUComputePipelineRaw = SDL_GPUComputePipeline*;

/// Alias to raw representation for GPUGraphicsPipeline.
using GPUGraphicsPipelineRaw = SDL_GPUGraphicsPipeline*;

/// Alias to raw representation for GPUCommandBuffer.
using GPUCommandBufferRaw = SDL_GPUCommandBuffer*;

/// Alias to raw representation for GPURenderPass.
using GPURenderPassRaw = SDL_GPURenderPass*;

/// Alias to raw representation for GPUComputePass.
using GPUComputePassRaw = SDL_GPUComputePass*;

/// Alias to raw representation for GPUCopyPass.
using GPUCopyPassRaw = SDL_GPUCopyPass*;

/**
 * A structure specifying the parameters of a buffer.
 *
 * Usage flags can be bitwise OR'd together for combinations of usages. Note
 * that certain combinations are invalid, for example VERTEX and INDEX.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUBuffer.GPUBuffer
 * @sa GPUBufferUsageFlags
 */
using GPUBufferCreateInfo = SDL_GPUBufferCreateInfo;

/**
 * An opaque handle representing a buffer.
 *
 * Used for vertices, indices, indirect draw commands, and general compute
 * data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUBuffer.GPUBuffer
 * @sa GPUCopyPass.UploadToBuffer
 * @sa GPUCopyPass.DownloadFromBuffer
 * @sa GPUCopyPass.CopyBufferToBuffer
 * @sa GPURenderPass.BindVertexBuffers
 * @sa GPURenderPass.BindIndexBuffer
 * @sa GPURenderPass.BindVertexStorageBuffers
 * @sa GPURenderPass.BindFragmentStorageBuffers
 * @sa GPURenderPass.DrawPrimitivesIndirect
 * @sa GPURenderPass.DrawIndexedPrimitivesIndirect
 * @sa GPUComputePass.BindStorageBuffers
 * @sa GPUComputePass.DispatchIndirect
 * @sa GPUDevice.ReleaseBuffer
 */
class GPUBuffer
{
  GPUBufferRaw m_gPUBuffer;

public:
  /**
   * Wraps GPUBuffer.
   *
   * @param gPUBuffer the value to be wrapped
   */
  constexpr GPUBuffer(GPUBufferRaw gPUBuffer = {})
    : m_gPUBuffer(gPUBuffer)
  {
  }

  /**
   * Creates a buffer object to be used in graphics or compute workflows.
   *
   * The contents of this buffer are undefined until data is written to the
   * buffer.
   *
   * Note that certain combinations of usage flags are invalid. For example, a
   * buffer cannot have both the VERTEX and INDEX flags.
   *
   * If you use a STORAGE flag, the data in the buffer must respect std140
   * layout conventions. In practical terms this means you must ensure that vec3
   * and vec4 fields are 16-byte aligned.
   *
   * For better understanding of underlying concepts and memory management with
   * SDL GPU API, you may refer
   * [this blog post](https://moonside.games/posts/sdl-gpu-concepts-cycling/)
   * .
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUBuffer.CREATE_NAME_STRING`: a name that can be displayed in
   *   debugging tools.
   *
   * @param device a GPU Context.
   * @param createinfo a struct describing the state of the buffer to create.
   * @post a buffer object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCopyPass.UploadToBuffer
   * @sa GPUCopyPass.DownloadFromBuffer
   * @sa GPUCopyPass.CopyBufferToBuffer
   * @sa GPURenderPass.BindVertexBuffers
   * @sa GPURenderPass.BindIndexBuffer
   * @sa GPURenderPass.BindVertexStorageBuffers
   * @sa GPURenderPass.BindFragmentStorageBuffers
   * @sa GPURenderPass.DrawPrimitivesIndirect
   * @sa GPURenderPass.DrawIndexedPrimitivesIndirect
   * @sa GPUComputePass.BindStorageBuffers
   * @sa GPUComputePass.DispatchIndirect
   * @sa GPUDevice.ReleaseBuffer
   */
  GPUBuffer(GPUDeviceParam device, const GPUBufferCreateInfo& createinfo)
    : m_gPUBuffer(CheckError(SDL_CreateGPUBuffer(device, &createinfo)))
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUBuffer& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUBufferRaw gPUBuffer) const
  {
    return operator==(GPUBuffer(gPUBuffer));
  }

  /**
   * Unwraps to the underlying GPUBuffer.
   *
   * @returns the underlying GPUBufferRaw.
   */
  constexpr operator GPUBufferRaw() const { return m_gPUBuffer; }
};

/**
 * A structure specifying the parameters of a transfer buffer.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUTransferBuffer.GPUTransferBuffer
 */
using GPUTransferBufferCreateInfo = SDL_GPUTransferBufferCreateInfo;

/**
 * An opaque handle representing a transfer buffer.
 *
 * Used for transferring data to and from the device.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUTransferBuffer.GPUTransferBuffer
 * @sa GPUDevice.MapTransferBuffer
 * @sa GPUDevice.UnmapTransferBuffer
 * @sa GPUCopyPass.UploadToBuffer
 * @sa GPUCopyPass.UploadToTexture
 * @sa GPUCopyPass.DownloadFromBuffer
 * @sa GPUCopyPass.DownloadFromTexture
 * @sa GPUDevice.ReleaseTransferBuffer
 */
class GPUTransferBuffer
{
  GPUTransferBufferRaw m_gPUTransferBuffer;

public:
  /**
   * Wraps GPUTransferBuffer.
   *
   * @param gPUTransferBuffer the value to be wrapped
   */
  constexpr GPUTransferBuffer(GPUTransferBufferRaw gPUTransferBuffer = {})
    : m_gPUTransferBuffer(gPUTransferBuffer)
  {
  }

  /**
   * Creates a transfer buffer to be used when uploading to or downloading from
   * graphics resources.
   *
   * Download buffers can be particularly expensive to create, so it is good
   * practice to reuse them if data will be downloaded regularly.
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUTransferBuffer.CREATE_NAME_STRING`: a name that can be
   *   displayed in debugging tools.
   *
   * @param device a GPU Context.
   * @param createinfo a struct describing the state of the transfer buffer to
   *                   create.
   * @post a transfer buffer on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCopyPass.UploadToBuffer
   * @sa GPUCopyPass.DownloadFromBuffer
   * @sa GPUCopyPass.UploadToTexture
   * @sa GPUCopyPass.DownloadFromTexture
   * @sa GPUDevice.ReleaseTransferBuffer
   */
  GPUTransferBuffer(GPUDeviceParam device,
                    const GPUTransferBufferCreateInfo& createinfo)
    : m_gPUTransferBuffer(
        CheckError(SDL_CreateGPUTransferBuffer(device, &createinfo)))
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUTransferBuffer& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUTransferBufferRaw gPUTransferBuffer) const
  {
    return operator==(GPUTransferBuffer(gPUTransferBuffer));
  }

  /**
   * Unwraps to the underlying GPUTransferBuffer.
   *
   * @returns the underlying GPUTransferBufferRaw.
   */
  constexpr operator GPUTransferBufferRaw() const
  {
    return m_gPUTransferBuffer;
  }
};

/**
 * A structure specifying the parameters of a texture.
 *
 * Usage flags can be bitwise OR'd together for combinations of usages. Note
 * that certain usage combinations are invalid, for example SAMPLER and
 * GRAPHICS_STORAGE.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUTexture.GPUTexture
 * @sa GPUTextureType
 * @sa GPUTextureFormat
 * @sa GPUTextureUsageFlags
 * @sa GPUSampleCount
 */
using GPUTextureCreateInfo = SDL_GPUTextureCreateInfo;

/**
 * An opaque handle representing a texture.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUTexture.GPUTexture
 * @sa GPUCopyPass.UploadToTexture
 * @sa GPUCopyPass.DownloadFromTexture
 * @sa GPUCopyPass.CopyTextureToTexture
 * @sa GPURenderPass.BindVertexSamplers
 * @sa GPURenderPass.BindVertexStorageTextures
 * @sa GPURenderPass.BindFragmentSamplers
 * @sa GPURenderPass.BindFragmentStorageTextures
 * @sa GPUComputePass.BindStorageTextures
 * @sa GPUCommandBuffer.GenerateMipmapsForTexture
 * @sa GPUCommandBuffer.BlitTexture
 * @sa GPUDevice.ReleaseTexture
 */
class GPUTexture
{
  GPUTextureRaw m_gPUTexture;

public:
  /**
   * Wraps GPUTexture.
   *
   * @param gPUTexture the value to be wrapped
   */
  constexpr GPUTexture(GPUTextureRaw gPUTexture = {})
    : m_gPUTexture(gPUTexture)
  {
  }

  /**
   * Creates a texture object to be used in graphics or compute workflows.
   *
   * The contents of this texture are undefined until data is written to the
   * texture.
   *
   * Note that certain combinations of usage flags are invalid. For example, a
   * texture cannot have both the SAMPLER and GRAPHICS_STORAGE_READ flags.
   *
   * If you request a sample count higher than the hardware supports, the
   * implementation will automatically fall back to the highest available sample
   * count.
   *
   * There are optional properties that can be provided through
   * GPUTextureCreateInfo's `props`. These are the supported properties:
   *
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_R_FLOAT`: (Direct3D 12 only) if
   *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
   *   to a color with this red intensity. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_G_FLOAT`: (Direct3D 12 only) if
   *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
   *   to a color with this green intensity. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_B_FLOAT`: (Direct3D 12 only) if
   *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
   *   to a color with this blue intensity. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_A_FLOAT`: (Direct3D 12 only) if
   *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
   *   to a color with this alpha intensity. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_DEPTH_FLOAT`: (Direct3D 12 only)
   *   if the texture usage is GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET, clear
   *   the texture to a depth of this value. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_STENCIL_NUMBER`: (Direct3D 12
   *   only) if the texture usage is GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
   *   clear the texture to a stencil of this Uint8 value. Defaults to zero.
   * - `prop::GPUTexture.CREATE_NAME_STRING`: a name that can be displayed
   *   in debugging tools.
   *
   * @param device a GPU Context.
   * @param createinfo a struct describing the state of the texture to create.
   * @post a texture object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCopyPass.UploadToTexture
   * @sa GPUCopyPass.DownloadFromTexture
   * @sa GPURenderPass.BindVertexSamplers
   * @sa GPURenderPass.BindVertexStorageTextures
   * @sa GPURenderPass.BindFragmentSamplers
   * @sa GPURenderPass.BindFragmentStorageTextures
   * @sa GPUComputePass.BindStorageTextures
   * @sa GPUCommandBuffer.BlitTexture
   * @sa GPUDevice.ReleaseTexture
   * @sa GPUDevice.TextureSupportsFormat
   */
  GPUTexture(GPUDeviceParam device, const GPUTextureCreateInfo& createinfo)
    : m_gPUTexture(CheckError(SDL_CreateGPUTexture(device, &createinfo)))
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUTexture& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUTextureRaw gPUTexture) const
  {
    return operator==(GPUTexture(gPUTexture));
  }

  /**
   * Unwraps to the underlying GPUTexture.
   *
   * @returns the underlying GPUTextureRaw.
   */
  constexpr operator GPUTextureRaw() const { return m_gPUTexture; }
};

/**
 * A structure specifying the parameters of a sampler.
 *
 * Note that mip_lod_bias is a no-op for the Metal driver. For Metal, LOD bias
 * must be applied via shader instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUSampler.GPUSampler
 * @sa GPUFilter
 * @sa GPUSamplerMipmapMode
 * @sa GPUSamplerAddressMode
 * @sa GPUCompareOp
 */
using GPUSamplerCreateInfo = SDL_GPUSamplerCreateInfo;

/**
 * An opaque handle representing a sampler.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUSampler.GPUSampler
 * @sa GPURenderPass.BindVertexSamplers
 * @sa GPURenderPass.BindFragmentSamplers
 * @sa GPUDevice.ReleaseSampler
 */
class GPUSampler
{
  GPUSamplerRaw m_gPUSampler;

public:
  /**
   * Wraps GPUSampler.
   *
   * @param gPUSampler the value to be wrapped
   */
  constexpr GPUSampler(GPUSamplerRaw gPUSampler = {})
    : m_gPUSampler(gPUSampler)
  {
  }

  /**
   * Creates a sampler object to be used when binding textures in a graphics
   * workflow.
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUSampler.CREATE_NAME_STRING`: a name that can be displayed
   *   in debugging tools.
   *
   * @param device a GPU Context.
   * @param createinfo a struct describing the state of the sampler to create.
   * @post a sampler object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPURenderPass.BindVertexSamplers
   * @sa GPURenderPass.BindFragmentSamplers
   * @sa GPUDevice.ReleaseSampler
   */
  GPUSampler(GPUDeviceParam device, const GPUSamplerCreateInfo& createinfo)
    : m_gPUSampler(CheckError(SDL_CreateGPUSampler(device, &createinfo)))
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUSampler& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUSamplerRaw gPUSampler) const
  {
    return operator==(GPUSampler(gPUSampler));
  }

  /**
   * Unwraps to the underlying GPUSampler.
   *
   * @returns the underlying GPUSamplerRaw.
   */
  constexpr operator GPUSamplerRaw() const { return m_gPUSampler; }
};

/**
 * A structure specifying code and metadata for creating a shader object.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
using GPUShaderCreateInfo = SDL_GPUShaderCreateInfo;

/**
 * An opaque handle representing a compiled shader object.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 * @sa GPUDevice.ReleaseShader
 */
class GPUShader
{
  GPUShaderRaw m_gPUShader;

public:
  /**
   * Wraps GPUShader.
   *
   * @param gPUShader the value to be wrapped
   */
  constexpr GPUShader(GPUShaderRaw gPUShader = {})
    : m_gPUShader(gPUShader)
  {
  }

  /**
   * Creates a shader to be used when creating a graphics pipeline.
   *
   * Shader resource bindings must be authored to follow a particular order
   * depending on the shader format.
   *
   * For SPIR-V shaders, use the following resource sets:
   *
   * For vertex shaders:
   *
   * - 0: Sampled textures, followed by storage textures, followed by storage
   *   buffers
   * - 1: Uniform buffers
   *
   * For fragment shaders:
   *
   * - 2: Sampled textures, followed by storage textures, followed by storage
   *   buffers
   * - 3: Uniform buffers
   *
   * For DXBC and DXIL shaders, use the following register order:
   *
   * For vertex shaders:
   *
   * - (t[n], space0): Sampled textures, followed by storage textures, followed
   *   by storage buffers
   * - (s[n], space0): Samplers with indices corresponding to the sampled
   *   textures
   * - (b[n], space1): Uniform buffers
   *
   * For pixel shaders:
   *
   * - (t[n], space2): Sampled textures, followed by storage textures, followed
   *   by storage buffers
   * - (s[n], space2): Samplers with indices corresponding to the sampled
   *   textures
   * - (b[n], space3): Uniform buffers
   *
   * For MSL/metallib, use the following order:
   *
   * - [[texture]]: Sampled textures, followed by storage textures
   * - [[sampler]]: Samplers with indices corresponding to the sampled textures
   * - [[buffer]]: Uniform buffers, followed by storage buffers. Vertex buffer 0
   *   is bound at [[buffer(14)]], vertex buffer 1 at [[buffer(15)]], and so on.
   *   Rather than manually authoring vertex buffer indices, use the
   *   [[stage_in]] attribute which will automatically use the vertex input
   *   information from the GPUGraphicsPipeline.
   *
   * Shader semantics other than system-value semantics do not matter in D3D12
   * and for ease of use the SDL implementation assumes that non system-value
   * semantics will all be TEXCOORD. If you are using HLSL as the shader source
   * language, your vertex semantics should start at TEXCOORD0 and increment
   * like so: TEXCOORD1, TEXCOORD2, etc. If you wish to change the semantic
   * prefix to something other than TEXCOORD you can use
   * prop::GpuDevice.CREATE_D3D12_SEMANTIC_NAME_STRING with
   * GPUDevice.GPUDevice().
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUShader.CREATE_NAME_STRING`: a name that can be displayed in
   *   debugging tools.
   *
   * @param device a GPU Context.
   * @param createinfo a struct describing the state of the shader to create.
   * @post a shader object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
   * @sa GPUDevice.ReleaseShader
   */
  GPUShader(GPUDeviceParam device, const GPUShaderCreateInfo& createinfo)
    : m_gPUShader(CheckError(SDL_CreateGPUShader(device, &createinfo)))
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUShader& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUShaderRaw gPUShader) const
  {
    return operator==(GPUShader(gPUShader));
  }

  /**
   * Unwraps to the underlying GPUShader.
   *
   * @returns the underlying GPUShaderRaw.
   */
  constexpr operator GPUShaderRaw() const { return m_gPUShader; }
};

/**
 * A structure specifying the parameters of a compute pipeline state.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUComputePipeline.GPUComputePipeline
 * @sa GPUShaderFormat
 */
using GPUComputePipelineCreateInfo = SDL_GPUComputePipelineCreateInfo;

/**
 * An opaque handle representing a compute pipeline.
 *
 * Used during compute passes.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUComputePipeline.GPUComputePipeline
 * @sa GPUComputePass.BindPipeline
 * @sa GPUDevice.ReleaseComputePipeline
 */
class GPUComputePipeline
{
  GPUComputePipelineRaw m_gPUComputePipeline;

public:
  /**
   * Wraps GPUComputePipeline.
   *
   * @param gPUComputePipeline the value to be wrapped
   */
  constexpr GPUComputePipeline(GPUComputePipelineRaw gPUComputePipeline = {})
    : m_gPUComputePipeline(gPUComputePipeline)
  {
  }

  /**
   * Creates a pipeline object to be used in a compute workflow.
   *
   * Shader resource bindings must be authored to follow a particular order
   * depending on the shader format.
   *
   * For SPIR-V shaders, use the following resource sets:
   *
   * - 0: Sampled textures, followed by read-only storage textures, followed by
   *   read-only storage buffers
   * - 1: Read-write storage textures, followed by read-write storage buffers
   * - 2: Uniform buffers
   *
   * For DXBC and DXIL shaders, use the following register order:
   *
   * - (t[n], space0): Sampled textures, followed by read-only storage textures,
   *   followed by read-only storage buffers
   * - (u[n], space1): Read-write storage textures, followed by read-write
   *   storage buffers
   * - (b[n], space2): Uniform buffers
   *
   * For MSL/metallib, use the following order:
   *
   * - [[buffer]]: Uniform buffers, followed by read-only storage buffers,
   *   followed by read-write storage buffers
   * - [[texture]]: Sampled textures, followed by read-only storage textures,
   *   followed by read-write storage textures
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUComputePipeline.CREATE_NAME_STRING`: a name that can be
   *   displayed in debugging tools.
   *
   * @param device a GPU Context.
   * @param createinfo a struct describing the state of the compute pipeline to
   *                   create.
   * @post a compute pipeline object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUComputePass.BindPipeline
   * @sa GPUDevice.ReleaseComputePipeline
   */
  GPUComputePipeline(GPUDeviceParam device,
                     const GPUComputePipelineCreateInfo& createinfo)
    : m_gPUComputePipeline(
        CheckError(SDL_CreateGPUComputePipeline(device, &createinfo)))
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUComputePipeline& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUComputePipelineRaw gPUComputePipeline) const
  {
    return operator==(GPUComputePipeline(gPUComputePipeline));
  }

  /**
   * Unwraps to the underlying GPUComputePipeline.
   *
   * @returns the underlying GPUComputePipelineRaw.
   */
  constexpr operator GPUComputePipelineRaw() const
  {
    return m_gPUComputePipeline;
  }
};

/**
 * A structure specifying the parameters of a graphics pipeline state.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 * @sa GPUShader
 * @sa GPUVertexInputState
 * @sa GPUPrimitiveType
 * @sa GPURasterizerState
 * @sa GPUMultisampleState
 * @sa GPUDepthStencilState
 * @sa GPUGraphicsPipelineTargetInfo
 */
using GPUGraphicsPipelineCreateInfo = SDL_GPUGraphicsPipelineCreateInfo;

/**
 * An opaque handle representing a graphics pipeline.
 *
 * Used during render passes.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 * @sa GPURenderPass.BindPipeline
 * @sa GPUDevice.ReleaseGraphicsPipeline
 */
class GPUGraphicsPipeline
{
  GPUGraphicsPipelineRaw m_gPUGraphicsPipeline;

public:
  /**
   * Wraps GPUGraphicsPipeline.
   *
   * @param gPUGraphicsPipeline the value to be wrapped
   */
  constexpr GPUGraphicsPipeline(GPUGraphicsPipelineRaw gPUGraphicsPipeline = {})
    : m_gPUGraphicsPipeline(gPUGraphicsPipeline)
  {
  }

  /**
   * Creates a pipeline object to be used in a graphics workflow.
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUGraphicsPipeline.CREATE_NAME_STRING`: a name that can be
   *   displayed in debugging tools.
   *
   * @param device a GPU Context.
   * @param createinfo a struct describing the state of the graphics pipeline to
   *                   create.
   * @post a graphics pipeline object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   * @sa GPURenderPass.BindPipeline
   * @sa GPUDevice.ReleaseGraphicsPipeline
   */
  GPUGraphicsPipeline(GPUDeviceParam device,
                      const GPUGraphicsPipelineCreateInfo& createinfo)
    : m_gPUGraphicsPipeline(
        CheckError(SDL_CreateGPUGraphicsPipeline(device, &createinfo)))
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUGraphicsPipeline& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUGraphicsPipelineRaw gPUGraphicsPipeline) const
  {
    return operator==(GPUGraphicsPipeline(gPUGraphicsPipeline));
  }

  /**
   * Unwraps to the underlying GPUGraphicsPipeline.
   *
   * @returns the underlying GPUGraphicsPipelineRaw.
   */
  constexpr operator GPUGraphicsPipelineRaw() const
  {
    return m_gPUGraphicsPipeline;
  }
};

/**
 * A structure specifying a viewport.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPURenderPass.SetViewport
 */
using GPUViewport = SDL_GPUViewport;

/**
 * A structure specifying parameters in a buffer binding call.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPURenderPass.BindVertexBuffers
 * @sa GPURenderPass.BindIndexBuffer
 */
using GPUBufferBinding = SDL_GPUBufferBinding;

/**
 * Specifies the size of elements in an index buffer.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUIndexElementSize = SDL_GPUIndexElementSize;

constexpr GPUIndexElementSize GPU_INDEXELEMENTSIZE_16BIT =
  SDL_GPU_INDEXELEMENTSIZE_16BIT; ///< The index elements are 16-bit.

constexpr GPUIndexElementSize GPU_INDEXELEMENTSIZE_32BIT =
  SDL_GPU_INDEXELEMENTSIZE_32BIT; ///< The index elements are 32-bit.

/**
 * A structure specifying parameters in a sampler binding call.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPURenderPass.BindVertexSamplers
 * @sa GPURenderPass.BindFragmentSamplers
 */
using GPUTextureSamplerBinding = SDL_GPUTextureSamplerBinding;

/**
 * An opaque handle representing a render pass.
 *
 * This handle is transient and should not be held or referenced after
 * GPURenderPass.End is called.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BeginRenderPass
 * @sa GPURenderPass.End
 */
class GPURenderPass
{
  GPURenderPassRaw m_gPURenderPass;

public:
  /**
   * Wraps GPURenderPass.
   *
   * @param gPURenderPass the value to be wrapped
   */
  constexpr GPURenderPass(GPURenderPassRaw gPURenderPass = {})
    : m_gPURenderPass(gPURenderPass)
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPURenderPass& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPURenderPassRaw gPURenderPass) const
  {
    return operator==(GPURenderPass(gPURenderPass));
  }

  /**
   * Unwraps to the underlying GPURenderPass.
   *
   * @returns the underlying GPURenderPassRaw.
   */
  constexpr operator GPURenderPassRaw() const { return m_gPURenderPass; }

  /**
   * Binds a graphics pipeline on a render pass to be used in rendering.
   *
   * A graphics pipeline must be bound before making any draw calls.
   *
   * @param graphics_pipeline the graphics pipeline to bind.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void BindPipeline(GPUGraphicsPipeline graphics_pipeline);

  /**
   * Sets the current viewport state on a command buffer.
   *
   * @param viewport the viewport to set.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetViewport(const GPUViewport& viewport);

  /**
   * Sets the current scissor state on a command buffer.
   *
   * @param scissor the scissor area to set.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetScissor(const RectRaw& scissor);

  /**
   * Sets the current blend constants on a command buffer.
   *
   * @param blend_constants the blend constant color.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPU_BLENDFACTOR_CONSTANT_COLOR
   * @sa GPU_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR
   */
  void SetBlendConstants(FColorRaw blend_constants);

  /**
   * Sets the current stencil reference value on a command buffer.
   *
   * @param reference the stencil reference value to set.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetStencilReference(Uint8 reference);

  /**
   * Binds vertex buffers on a command buffer for use with subsequent draw
   * calls.
   *
   * @param first_slot the vertex buffer slot to begin binding from.
   * @param bindings an array of GPUBufferBinding structs containing vertex
   *                 buffers and offset values.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void BindVertexBuffers(Uint32 first_slot,
                         std::span<const GPUBufferBinding> bindings);

  /**
   * Binds an index buffer on a command buffer for use with subsequent draw
   * calls.
   *
   * @param binding a pointer to a struct containing an index buffer and offset.
   * @param index_element_size whether the index values in the buffer are 16- or
   *                           32-bit.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void BindIndexBuffer(const GPUBufferBinding& binding,
                       GPUIndexElementSize index_element_size);

  /**
   * Binds texture-sampler pairs for use on the vertex shader.
   *
   * The textures must have been created with GPU_TEXTUREUSAGE_SAMPLER.
   *
   * Be sure your shader is set up according to the requirements documented in
   * GPUShader.GPUShader().
   *
   * @param first_slot the vertex sampler slot to begin binding from.
   * @param texture_sampler_bindings an array of texture-sampler binding
   *                                 structs.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   */
  void BindVertexSamplers(
    Uint32 first_slot,
    std::span<const GPUTextureSamplerBinding> texture_sampler_bindings);

  /**
   * Binds storage textures for use on the vertex shader.
   *
   * These textures must have been created with
   * GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ.
   *
   * Be sure your shader is set up according to the requirements documented in
   * GPUShader.GPUShader().
   *
   * @param first_slot the vertex storage texture slot to begin binding from.
   * @param storage_textures an array of storage textures.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   */
  void BindVertexStorageTextures(Uint32 first_slot,
                                 SpanRef<const GPUTextureRaw> storage_textures);

  /**
   * Binds storage buffers for use on the vertex shader.
   *
   * These buffers must have been created with
   * GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ.
   *
   * Be sure your shader is set up according to the requirements documented in
   * GPUShader.GPUShader().
   *
   * @param first_slot the vertex storage buffer slot to begin binding from.
   * @param storage_buffers an array of buffers.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   */
  void BindVertexStorageBuffers(Uint32 first_slot,
                                SpanRef<const GPUBufferRaw> storage_buffers);

  /**
   * Binds texture-sampler pairs for use on the fragment shader.
   *
   * The textures must have been created with GPU_TEXTUREUSAGE_SAMPLER.
   *
   * Be sure your shader is set up according to the requirements documented in
   * GPUShader.GPUShader().
   *
   * @param first_slot the fragment sampler slot to begin binding from.
   * @param texture_sampler_bindings an array of texture-sampler binding
   *                                 structs.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   */
  void BindFragmentSamplers(
    Uint32 first_slot,
    std::span<const GPUTextureSamplerBinding> texture_sampler_bindings);

  /**
   * Binds storage textures for use on the fragment shader.
   *
   * These textures must have been created with
   * GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ.
   *
   * Be sure your shader is set up according to the requirements documented in
   * GPUShader.GPUShader().
   *
   * @param first_slot the fragment storage texture slot to begin binding from.
   * @param storage_textures an array of storage textures.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   */
  void BindFragmentStorageTextures(
    Uint32 first_slot,
    SpanRef<const GPUTextureRaw> storage_textures);

  /**
   * Binds storage buffers for use on the fragment shader.
   *
   * These buffers must have been created with
   * GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ.
   *
   * Be sure your shader is set up according to the requirements documented in
   * GPUShader.GPUShader().
   *
   * @param first_slot the fragment storage buffer slot to begin binding from.
   * @param storage_buffers an array of storage buffers.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   */
  void BindFragmentStorageBuffers(Uint32 first_slot,
                                  SpanRef<const GPUBufferRaw> storage_buffers);

  /**
   * Draws data using bound graphics state with an index buffer and instancing
   * enabled.
   *
   * You must not call this function before binding a graphics pipeline.
   *
   * Note that the `first_vertex` and `first_instance` parameters are NOT
   * compatible with built-in vertex/instance ID variables in shaders (for
   * example, SV_VertexID); GPU APIs and shader languages do not define these
   * built-in variables consistently, so if your shader depends on them, the
   * only way to keep behavior consistent and portable is to always pass 0 for
   * the correlating parameter in the draw calls.
   *
   * @param num_indices the number of indices to draw per instance.
   * @param num_instances the number of instances to draw.
   * @param first_index the starting index within the index buffer.
   * @param vertex_offset value added to vertex index before indexing into the
   *                      vertex buffer.
   * @param first_instance the ID of the first instance to draw.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void DrawIndexedPrimitives(Uint32 num_indices,
                             Uint32 num_instances,
                             Uint32 first_index,
                             Sint32 vertex_offset,
                             Uint32 first_instance);

  /**
   * Draws data using bound graphics state.
   *
   * You must not call this function before binding a graphics pipeline.
   *
   * Note that the `first_vertex` and `first_instance` parameters are NOT
   * compatible with built-in vertex/instance ID variables in shaders (for
   * example, SV_VertexID); GPU APIs and shader languages do not define these
   * built-in variables consistently, so if your shader depends on them, the
   * only way to keep behavior consistent and portable is to always pass 0 for
   * the correlating parameter in the draw calls.
   *
   * @param num_vertices the number of vertices to draw.
   * @param num_instances the number of instances that will be drawn.
   * @param first_vertex the index of the first vertex to draw.
   * @param first_instance the ID of the first instance to draw.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void DrawPrimitives(Uint32 num_vertices,
                      Uint32 num_instances,
                      Uint32 first_vertex,
                      Uint32 first_instance);

  /**
   * Draws data using bound graphics state and with draw parameters set from a
   * buffer.
   *
   * The buffer must consist of tightly-packed draw parameter sets that each
   * match the layout of GPUIndirectDrawCommand. You must not call this
   * function before binding a graphics pipeline.
   *
   * @param buffer a buffer containing draw parameters.
   * @param offset the offset to start reading from the draw buffer.
   * @param draw_count the number of draw parameter sets that should be read
   *                   from the draw buffer.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void DrawPrimitivesIndirect(GPUBuffer buffer,
                              Uint32 offset,
                              Uint32 draw_count);

  /**
   * Draws data using bound graphics state with an index buffer enabled and with
   * draw parameters set from a buffer.
   *
   * The buffer must consist of tightly-packed draw parameter sets that each
   * match the layout of GPUIndexedIndirectDrawCommand. You must not call
   * this function before binding a graphics pipeline.
   *
   * @param buffer a buffer containing draw parameters.
   * @param offset the offset to start reading from the draw buffer.
   * @param draw_count the number of draw parameter sets that should be read
   *                   from the draw buffer.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void DrawIndexedPrimitivesIndirect(GPUBuffer buffer,
                                     Uint32 offset,
                                     Uint32 draw_count);

  /**
   * Ends the given render pass.
   *
   * All bound graphics state on the render pass command buffer is unset. The
   * render pass handle is now invalid.
   *
   *
   * @since This function is available since SDL 3.2.0.
   */
  void End();
};

/**
 * An opaque handle representing a compute pass.
 *
 * This handle is transient and should not be held or referenced after
 * GPUComputePass.End is called.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BeginComputePass
 * @sa GPUComputePass.End
 */
class GPUComputePass
{
  GPUComputePassRaw m_gPUComputePass;

public:
  /**
   * Wraps GPUComputePass.
   *
   * @param gPUComputePass the value to be wrapped
   */
  constexpr GPUComputePass(GPUComputePassRaw gPUComputePass = {})
    : m_gPUComputePass(gPUComputePass)
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUComputePass& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUComputePassRaw gPUComputePass) const
  {
    return operator==(GPUComputePass(gPUComputePass));
  }

  /**
   * Unwraps to the underlying GPUComputePass.
   *
   * @returns the underlying GPUComputePassRaw.
   */
  constexpr operator GPUComputePassRaw() const { return m_gPUComputePass; }

  /**
   * Binds a compute pipeline on a command buffer for use in compute dispatch.
   *
   * @param compute_pipeline a compute pipeline to bind.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void BindPipeline(GPUComputePipeline compute_pipeline);

  /**
   * Binds texture-sampler pairs for use on the compute shader.
   *
   * The textures must have been created with GPU_TEXTUREUSAGE_SAMPLER.
   *
   * Be sure your shader is set up according to the requirements documented in
   * GPUShader.GPUShader().
   *
   * @param first_slot the compute sampler slot to begin binding from.
   * @param texture_sampler_bindings an array of texture-sampler binding
   *                                 structs.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   */
  void BindSamplers(
    Uint32 first_slot,
    std::span<const GPUTextureSamplerBinding> texture_sampler_bindings);

  /**
   * Binds storage textures as readonly for use on the compute pipeline.
   *
   * These textures must have been created with
   * GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ.
   *
   * Be sure your shader is set up according to the requirements documented in
   * GPUShader.GPUShader().
   *
   * @param first_slot the compute storage texture slot to begin binding from.
   * @param storage_textures an array of storage textures.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   */
  void BindStorageTextures(Uint32 first_slot,
                           SpanRef<const GPUTextureRaw> storage_textures);

  /**
   * Binds storage buffers as readonly for use on the compute pipeline.
   *
   * These buffers must have been created with
   * GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ.
   *
   * Be sure your shader is set up according to the requirements documented in
   * GPUShader.GPUShader().
   *
   * @param first_slot the compute storage buffer slot to begin binding from.
   * @param storage_buffers an array of storage buffer binding structs.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   */
  void BindStorageBuffers(Uint32 first_slot,
                          SpanRef<const GPUBufferRaw> storage_buffers);

  /**
   * Dispatches compute work.
   *
   * You must not call this function before binding a compute pipeline.
   *
   * A VERY IMPORTANT NOTE If you dispatch multiple times in a compute pass, and
   * the dispatches write to the same resource region as each other, there is no
   * guarantee of which order the writes will occur. If the write order matters,
   * you MUST end the compute pass and begin another one.
   *
   * @param groupcount_x number of local workgroups to dispatch in the X
   *                     dimension.
   * @param groupcount_y number of local workgroups to dispatch in the Y
   *                     dimension.
   * @param groupcount_z number of local workgroups to dispatch in the Z
   *                     dimension.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Dispatch(Uint32 groupcount_x, Uint32 groupcount_y, Uint32 groupcount_z);

  /**
   * Dispatches compute work with parameters set from a buffer.
   *
   * The buffer layout should match the layout of
   * GPUIndirectDispatchCommand. You must not call this function before
   * binding a compute pipeline.
   *
   * A VERY IMPORTANT NOTE If you dispatch multiple times in a compute pass, and
   * the dispatches write to the same resource region as each other, there is no
   * guarantee of which order the writes will occur. If the write order matters,
   * you MUST end the compute pass and begin another one.
   *
   * @param buffer a buffer containing dispatch parameters.
   * @param offset the offset to start reading from the dispatch buffer.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void DispatchIndirect(GPUBuffer buffer, Uint32 offset);

  /**
   * Ends the current compute pass.
   *
   * All bound compute state on the command buffer is unset. The compute pass
   * handle is now invalid.
   *
   *
   * @since This function is available since SDL 3.2.0.
   */
  void End();
};

/**
 * A structure specifying a region of a buffer.
 *
 * Used when transferring data to or from buffers.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.UploadToBuffer
 * @sa GPUCopyPass.DownloadFromBuffer
 */
using GPUBufferRegion = SDL_GPUBufferRegion;

/**
 * A structure specifying a location in a texture.
 *
 * Used when copying data from one texture to another.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.CopyTextureToTexture
 */
using GPUTextureLocation = SDL_GPUTextureLocation;

/**
 * A structure specifying a location in a buffer.
 *
 * Used when copying data between buffers.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.CopyBufferToBuffer
 */
using GPUBufferLocation = SDL_GPUBufferLocation;

/**
 * A structure specifying a region of a texture.
 *
 * Used when transferring data to or from a texture.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.UploadToTexture
 * @sa GPUCopyPass.DownloadFromTexture
 * @sa GPUTexture.GPUTexture
 */
using GPUTextureRegion = SDL_GPUTextureRegion;

/**
 * A structure specifying parameters related to transferring data to or from a
 * texture.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.UploadToTexture
 * @sa GPUCopyPass.DownloadFromTexture
 */
using GPUTextureTransferInfo = SDL_GPUTextureTransferInfo;

/**
 * A structure specifying a location in a transfer buffer.
 *
 * Used when transferring buffer data to or from a transfer buffer.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.UploadToBuffer
 * @sa GPUCopyPass.DownloadFromBuffer
 */
using GPUTransferBufferLocation = SDL_GPUTransferBufferLocation;

/**
 * An opaque handle representing a copy pass.
 *
 * This handle is transient and should not be held or referenced after
 * GPUCopyPass.End is called.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BeginCopyPass
 * @sa GPUCopyPass.End
 */
class GPUCopyPass
{
  GPUCopyPassRaw m_gPUCopyPass;

public:
  /**
   * Wraps GPUCopyPass.
   *
   * @param gPUCopyPass the value to be wrapped
   */
  constexpr GPUCopyPass(GPUCopyPassRaw gPUCopyPass = {})
    : m_gPUCopyPass(gPUCopyPass)
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUCopyPass& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUCopyPassRaw gPUCopyPass) const
  {
    return operator==(GPUCopyPass(gPUCopyPass));
  }

  /**
   * Unwraps to the underlying GPUCopyPass.
   *
   * @returns the underlying GPUCopyPassRaw.
   */
  constexpr operator GPUCopyPassRaw() const { return m_gPUCopyPass; }

  /**
   * Uploads data from a transfer buffer to a texture.
   *
   * The upload occurs on the GPU timeline. You may assume that the upload has
   * finished in subsequent commands.
   *
   * You must align the data in the transfer buffer to a multiple of the texel
   * size of the texture format.
   *
   * @param source the source transfer buffer with image layout information.
   * @param destination the destination texture region.
   * @param cycle if true, cycles the texture if the texture is bound, otherwise
   *              overwrites the data.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void UploadToTexture(const GPUTextureTransferInfo& source,
                       const GPUTextureRegion& destination,
                       bool cycle);

  /**
   * Uploads data from a transfer buffer to a buffer.
   *
   * The upload occurs on the GPU timeline. You may assume that the upload has
   * finished in subsequent commands.
   *
   * @param source the source transfer buffer with offset.
   * @param destination the destination buffer with offset and size.
   * @param cycle if true, cycles the buffer if it is already bound, otherwise
   *              overwrites the data.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void UploadToBuffer(const GPUTransferBufferLocation& source,
                      const GPUBufferRegion& destination,
                      bool cycle);

  /**
   * Performs a texture-to-texture copy.
   *
   * This copy occurs on the GPU timeline. You may assume the copy has finished
   * in subsequent commands.
   *
   * @param source a source texture region.
   * @param destination a destination texture region.
   * @param w the width of the region to copy.
   * @param h the height of the region to copy.
   * @param d the depth of the region to copy.
   * @param cycle if true, cycles the destination texture if the destination
   *              texture is bound, otherwise overwrites the data.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void CopyTextureToTexture(const GPUTextureLocation& source,
                            const GPUTextureLocation& destination,
                            Uint32 w,
                            Uint32 h,
                            Uint32 d,
                            bool cycle);

  /**
   * Performs a buffer-to-buffer copy.
   *
   * This copy occurs on the GPU timeline. You may assume the copy has finished
   * in subsequent commands.
   *
   * @param source the buffer and offset to copy from.
   * @param destination the buffer and offset to copy to.
   * @param size the length of the buffer to copy.
   * @param cycle if true, cycles the destination buffer if it is already bound,
   *              otherwise overwrites the data.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void CopyBufferToBuffer(const GPUBufferLocation& source,
                          const GPUBufferLocation& destination,
                          Uint32 size,
                          bool cycle);

  /**
   * Copies data from a texture to a transfer buffer on the GPU timeline.
   *
   * This data is not guaranteed to be copied until the command buffer fence is
   * signaled.
   *
   * @param source the source texture region.
   * @param destination the destination transfer buffer with image layout
   *                    information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void DownloadFromTexture(const GPUTextureRegion& source,
                           const GPUTextureTransferInfo& destination);

  /**
   * Copies data from a buffer to a transfer buffer on the GPU timeline.
   *
   * This data is not guaranteed to be copied until the command buffer fence is
   * signaled.
   *
   * @param source the source buffer with offset and size.
   * @param destination the destination transfer buffer with offset.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void DownloadFromBuffer(const GPUBufferRegion& source,
                          const GPUTransferBufferLocation& destination);

  /**
   * Ends the current copy pass.
   *
   *
   * @since This function is available since SDL 3.2.0.
   */
  void End();
};

/**
 * A structure specifying the parameters of a color target used by a render
 * pass.
 *
 * The load_op field determines what is done with the texture at the beginning
 * of the render pass.
 *
 * - LOAD: Loads the data currently in the texture. Not recommended for
 *   multisample textures as it requires significant memory bandwidth.
 * - CLEAR: Clears the texture to a single color.
 * - DONT_CARE: The driver will do whatever it wants with the texture memory.
 *   This is a good option if you know that every single pixel will be touched
 *   in the render pass.
 *
 * The store_op field determines what is done with the color results of the
 * render pass.
 *
 * - STORE: Stores the results of the render pass in the texture. Not
 *   recommended for multisample textures as it requires significant memory
 *   bandwidth.
 * - DONT_CARE: The driver will do whatever it wants with the texture memory.
 *   This is often a good option for depth/stencil textures.
 * - RESOLVE: Resolves a multisample texture into resolve_texture, which must
 *   have a sample count of 1. Then the driver may discard the multisample
 *   texture memory. This is the most performant method of resolving a
 *   multisample target.
 * - RESOLVE_AND_STORE: Resolves a multisample texture into the
 *   resolve_texture, which must have a sample count of 1. Then the driver
 *   stores the multisample texture's contents. Not recommended as it requires
 *   significant memory bandwidth.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BeginRenderPass
 */
using GPUColorTargetInfo = SDL_GPUColorTargetInfo;

/**
 * A structure specifying the parameters of a depth-stencil target used by a
 * render pass.
 *
 * The load_op field determines what is done with the depth contents of the
 * texture at the beginning of the render pass.
 *
 * - LOAD: Loads the depth values currently in the texture.
 * - CLEAR: Clears the texture to a single depth.
 * - DONT_CARE: The driver will do whatever it wants with the memory. This is
 *   a good option if you know that every single pixel will be touched in the
 *   render pass.
 *
 * The store_op field determines what is done with the depth results of the
 * render pass.
 *
 * - STORE: Stores the depth results in the texture.
 * - DONT_CARE: The driver will do whatever it wants with the depth results.
 *   This is often a good option for depth/stencil textures that don't need to
 *   be reused again.
 *
 * The stencil_load_op field determines what is done with the stencil contents
 * of the texture at the beginning of the render pass.
 *
 * - LOAD: Loads the stencil values currently in the texture.
 * - CLEAR: Clears the stencil values to a single value.
 * - DONT_CARE: The driver will do whatever it wants with the memory. This is
 *   a good option if you know that every single pixel will be touched in the
 *   render pass.
 *
 * The stencil_store_op field determines what is done with the stencil results
 * of the render pass.
 *
 * - STORE: Stores the stencil results in the texture.
 * - DONT_CARE: The driver will do whatever it wants with the stencil results.
 *   This is often a good option for depth/stencil textures that don't need to
 *   be reused again.
 *
 * Note that depth/stencil targets do not support multisample resolves.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BeginRenderPass
 */
using GPUDepthStencilTargetInfo = SDL_GPUDepthStencilTargetInfo;

/**
 * A structure specifying parameters related to binding textures in a compute
 * pass.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BeginComputePass
 */
using GPUStorageTextureReadWriteBinding = SDL_GPUStorageTextureReadWriteBinding;

/**
 * A structure specifying parameters related to binding buffers in a compute
 * pass.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BeginComputePass
 */
using GPUStorageBufferReadWriteBinding = SDL_GPUStorageBufferReadWriteBinding;

/**
 * A structure containing parameters for a blit command.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BlitTexture
 */
using GPUBlitInfo = SDL_GPUBlitInfo;

/**
 * An opaque handle representing a fence.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.SubmitAndAcquireFence
 * @sa GPUDevice.QueryFence
 * @sa GPUDevice.WaitForFences
 * @sa GPUDevice.ReleaseFence
 */
using GPUFence = SDL_GPUFence;

/**
 * An opaque handle representing a command buffer.
 *
 * Most state is managed via command buffers. When setting state using a
 * command buffer, that state is local to the command buffer.
 *
 * Commands only begin execution on the GPU once GPUCommandBuffer.Submit is
 * called. Once the command buffer is submitted, it is no longer valid to use
 * it.
 *
 * Command buffers are executed in submission order. If you submit command
 * buffer A and then command buffer B all commands in A will begin executing
 * before any command in B begins executing.
 *
 * In multi-threading scenarios, you should only access a command buffer on
 * the thread you acquired it from.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUDevice.AcquireCommandBuffer
 * @sa GPUCommandBuffer.Submit
 * @sa GPUCommandBuffer.SubmitAndAcquireFence
 */
class GPUCommandBuffer
{
  GPUCommandBufferRaw m_gPUCommandBuffer;

public:
  /**
   * Wraps GPUCommandBuffer.
   *
   * @param gPUCommandBuffer the value to be wrapped
   */
  constexpr GPUCommandBuffer(GPUCommandBufferRaw gPUCommandBuffer = {})
    : m_gPUCommandBuffer(gPUCommandBuffer)
  {
  }

  /// Default comparison operator
  constexpr bool operator==(const GPUCommandBuffer& other) const = default;

  /// Compares with the underlying type
  constexpr bool operator==(GPUCommandBufferRaw gPUCommandBuffer) const
  {
    return operator==(GPUCommandBuffer(gPUCommandBuffer));
  }

  /**
   * Unwraps to the underlying GPUCommandBuffer.
   *
   * @returns the underlying GPUCommandBufferRaw.
   */
  constexpr operator GPUCommandBufferRaw() const { return m_gPUCommandBuffer; }

  /**
   * Inserts an arbitrary string label into the command buffer callstream.
   *
   * Useful for debugging.
   *
   * @param text a UTF-8 string constant to insert as the label.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void InsertDebugLabel(StringParam text);

  /**
   * Begins a debug group with an arbitrary name.
   *
   * Used for denoting groups of calls when viewing the command buffer
   * callstream in a graphics debugging tool.
   *
   * Each call to GPUCommandBuffer.PushDebugGroup must have a corresponding call
   * to GPUCommandBuffer.PopDebugGroup.
   *
   * On some backends (e.g. Metal), pushing a debug group during a
   * render/blit/compute pass will create a group that is scoped to the native
   * pass rather than the command buffer. For best results, if you push a debug
   * group during a pass, always pop it in the same pass.
   *
   * @param name a UTF-8 string constant that names the group.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.PopDebugGroup
   */
  void PushDebugGroup(StringParam name);

  /**
   * Ends the most-recently pushed debug group.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.PushDebugGroup
   */
  void PopDebugGroup();

  /**
   * Pushes data to a vertex uniform slot on the command buffer.
   *
   * Subsequent draw calls will use this uniform data.
   *
   * The data being pushed must respect std140 layout conventions. In practical
   * terms this means you must ensure that vec3 and vec4 fields are 16-byte
   * aligned.
   *
   * @param slot_index the vertex uniform slot to push data to.
   * @param data client data to write.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void PushVertexUniformData(Uint32 slot_index, SourceBytes data);

  /**
   * Pushes data to a fragment uniform slot on the command buffer.
   *
   * Subsequent draw calls will use this uniform data.
   *
   * The data being pushed must respect std140 layout conventions. In practical
   * terms this means you must ensure that vec3 and vec4 fields are 16-byte
   * aligned.
   *
   * @param slot_index the fragment uniform slot to push data to.
   * @param data client data to write.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void PushFragmentUniformData(Uint32 slot_index, SourceBytes data);

  /**
   * Pushes data to a uniform slot on the command buffer.
   *
   * Subsequent draw calls will use this uniform data.
   *
   * The data being pushed must respect std140 layout conventions. In practical
   * terms this means you must ensure that vec3 and vec4 fields are 16-byte
   * aligned.
   *
   * @param slot_index the uniform slot to push data to.
   * @param data client data to write.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void PushComputeUniformData(Uint32 slot_index, SourceBytes data);

  /**
   * Begins a render pass on a command buffer.
   *
   * A render pass consists of a set of texture subresources (or depth slices in
   * the 3D texture case) which will be rendered to during the render pass,
   * along with corresponding clear values and load/store operations. All
   * operations related to graphics pipelines must take place inside of a render
   * pass. A default viewport and scissor state are automatically set when this
   * is called. You cannot begin another render pass, or begin a compute pass or
   * copy pass until you have ended the render pass.
   *
   * @param color_target_infos an array of texture subresources with
   *                           corresponding clear values and load/store ops.
   * @param depth_stencil_target_info a texture subresource with corresponding
   *                                  clear value and load/store ops, may be
   *                                  nullptr.
   * @returns a render pass handle.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPURenderPass.End
   */
  GPURenderPass BeginRenderPass(
    std::span<const GPUColorTargetInfo> color_target_infos,
    OptionalRef<const GPUDepthStencilTargetInfo> depth_stencil_target_info);

  /**
   * Begins a compute pass on a command buffer.
   *
   * A compute pass is defined by a set of texture subresources and buffers that
   * may be written to by compute pipelines. These textures and buffers must
   * have been created with the COMPUTE_STORAGE_WRITE bit or the
   * COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE bit. If you do not create a texture
   * with COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE, you must not read from the
   * texture in the compute pass. All operations related to compute pipelines
   * must take place inside of a compute pass. You must not begin another
   * compute pass, or a render pass or copy pass before ending the compute pass.
   *
   * A VERY IMPORTANT NOTE - Reads and writes in compute passes are NOT
   * implicitly synchronized. This means you may cause data races by both
   * reading and writing a resource region in a compute pass, or by writing
   * multiple times to a resource region. If your compute work depends on
   * reading the completed output from a previous dispatch, you MUST end the
   * current compute pass and begin a new one before you can safely access the
   * data. Otherwise you will receive unexpected results. Reading and writing a
   * texture in the same compute pass is only supported by specific texture
   * formats. Make sure you check the format support!
   *
   * @param storage_texture_bindings an array of writeable storage texture
   *                                 binding structs.
   * @param storage_buffer_bindings an array of writeable storage buffer binding
   *                                structs.
   * @returns a compute pass handle.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUComputePass.End
   */
  GPUComputePass BeginComputePass(
    std::span<const GPUStorageTextureReadWriteBinding> storage_texture_bindings,
    std::span<const GPUStorageBufferReadWriteBinding> storage_buffer_bindings);

  /**
   * Begins a copy pass on a command buffer.
   *
   * All operations related to copying to or from buffers or textures take place
   * inside a copy pass. You must not begin another copy pass, or a render pass
   * or compute pass before ending the copy pass.
   *
   * @returns a copy pass handle.
   *
   * @since This function is available since SDL 3.2.0.
   */
  GPUCopyPass BeginCopyPass();

  /**
   * Generates mipmaps for the given texture.
   *
   * This function must not be called inside of any pass.
   *
   * @param texture a texture with more than 1 mip level.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void GenerateMipmapsForTexture(GPUTexture texture);

  /**
   * Blits from a source texture region to a destination texture region.
   *
   * This function must not be called inside of any pass.
   *
   * @param info the blit info struct containing the blit parameters.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void BlitTexture(const GPUBlitInfo& info);

  /**
   * Acquire a texture to use in presentation.
   *
   * When a swapchain texture is acquired on a command buffer, it will
   * automatically be submitted for presentation when the command buffer is
   * submitted. The swapchain texture should only be referenced by the command
   * buffer used to acquire it.
   *
   * This function will fill the swapchain texture handle with nullptr if too
   * many frames are in flight. This is not an error.
   *
   * If you use this function, it is possible to create a situation where many
   * command buffers are allocated while the rendering context waits for the GPU
   * to catch up, which will cause memory usage to grow. You should use
   * GPUCommandBuffer.WaitAndAcquireSwapchainTexture() unless you know what you
   * are doing with timing.
   *
   * The swapchain texture is managed by the implementation and must not be
   * freed by the user. You MUST NOT call this function from any thread other
   * than the one that created the window.
   *
   * @param window a window that has been claimed.
   * @param swapchain_texture_width a pointer filled in with the swapchain
   *                                texture width, may be nullptr.
   * @param swapchain_texture_height a pointer filled in with the swapchain
   *                                 texture height, may be nullptr.
   * @returns a swapchain texture handle.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called from the thread that
   *               created the window.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.ClaimWindow
   * @sa GPUCommandBuffer.Submit
   * @sa GPUCommandBuffer.SubmitAndAcquireFence
   * @sa GPUCommandBuffer.Cancel
   * @sa Window.GetSizeInPixels
   * @sa GPUDevice.WaitForSwapchain
   * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
   * @sa GPUDevice.SetAllowedFramesInFlight
   */
  GPUTexture AcquireSwapchainTexture(
    WindowParam window,
    Uint32* swapchain_texture_width = nullptr,
    Uint32* swapchain_texture_height = nullptr);

  /**
   * Blocks the thread until a swapchain texture is available to be acquired,
   * and then acquires it.
   *
   * When a swapchain texture is acquired on a command buffer, it will
   * automatically be submitted for presentation when the command buffer is
   * submitted. The swapchain texture should only be referenced by the command
   * buffer used to acquire it. It is an error to call
   * GPUCommandBuffer.Cancel() after a swapchain texture is acquired.
   *
   * This function can fill the swapchain texture handle with nullptr in certain
   * cases, for example if the window is minimized. This is not an error. You
   * should always make sure to check whether the pointer is nullptr before
   * actually using it.
   *
   * The swapchain texture is managed by the implementation and must not be
   * freed by the user. You MUST NOT call this function from any thread other
   * than the one that created the window.
   *
   * The swapchain texture is write-only and cannot be used as a sampler or for
   * another reading operation.
   *
   * @param window a window that has been claimed.
   * @param swapchain_texture_width a pointer filled in with the swapchain
   *                                texture width, may be nullptr.
   * @param swapchain_texture_height a pointer filled in with the swapchain
   *                                 texture height, may be nullptr.
   * @returns a swapchain texture handle.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called from the thread that
   *               created the window.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.Submit
   * @sa GPUCommandBuffer.SubmitAndAcquireFence
   * @sa GPUCommandBuffer.AcquireSwapchainTexture
   */
  GPUTexture WaitAndAcquireSwapchainTexture(
    WindowParam window,
    Uint32* swapchain_texture_width = nullptr,
    Uint32* swapchain_texture_height = nullptr);

  /**
   * Submits a command buffer so its commands can be processed on the GPU.
   *
   * It is invalid to use the command buffer after this is called.
   *
   * This must be called from the thread the command buffer was acquired on.
   *
   * All commands in the submission are guaranteed to begin executing before any
   * command in a subsequent submission begins executing.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.AcquireCommandBuffer
   * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
   * @sa GPUCommandBuffer.AcquireSwapchainTexture
   * @sa GPUCommandBuffer.SubmitAndAcquireFence
   */
  void Submit();

  /**
   * Submits a command buffer so its commands can be processed on the GPU, and
   * acquires a fence associated with the command buffer.
   *
   * You must release this fence when it is no longer needed or it will cause a
   * leak. It is invalid to use the command buffer after this is called.
   *
   * This must be called from the thread the command buffer was acquired on.
   *
   * All commands in the submission are guaranteed to begin executing before any
   * command in a subsequent submission begins executing.
   *
   * @returns a fence associated with the command buffer, or nullptr on failure;
   *          call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.AcquireCommandBuffer
   * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
   * @sa GPUCommandBuffer.AcquireSwapchainTexture
   * @sa GPUCommandBuffer.Submit
   * @sa GPUDevice.ReleaseFence
   */
  GPUFence* SubmitAndAcquireFence();

  /**
   * Cancels a command buffer.
   *
   * None of the enqueued commands are executed.
   *
   * It is an error to call this function after a swapchain texture has been
   * acquired.
   *
   * This must be called from the thread the command buffer was acquired on.
   *
   * You must not reference the command buffer after calling this function.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
   * @sa GPUDevice.AcquireCommandBuffer
   * @sa GPUCommandBuffer.AcquireSwapchainTexture
   */
  void Cancel();
};

/**
 * Specifies the format of shader code.
 *
 * Each format corresponds to a specific backend that accepts it.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
using GPUShaderFormat = Uint32;

constexpr GPUShaderFormat GPU_SHADERFORMAT_INVALID =
  SDL_GPU_SHADERFORMAT_INVALID; ///< INVALID

constexpr GPUShaderFormat GPU_SHADERFORMAT_PRIVATE =
  SDL_GPU_SHADERFORMAT_PRIVATE; ///< Shaders for NDA'd platforms.

constexpr GPUShaderFormat GPU_SHADERFORMAT_SPIRV =
  SDL_GPU_SHADERFORMAT_SPIRV; ///< SPIR-V shaders for Vulkan.

constexpr GPUShaderFormat GPU_SHADERFORMAT_DXBC =
  SDL_GPU_SHADERFORMAT_DXBC; ///< DXBC SM5_1 shaders for D3D12.

constexpr GPUShaderFormat GPU_SHADERFORMAT_DXIL =
  SDL_GPU_SHADERFORMAT_DXIL; ///< DXIL SM6_0 shaders for D3D12.

constexpr GPUShaderFormat GPU_SHADERFORMAT_MSL =
  SDL_GPU_SHADERFORMAT_MSL; ///< MSL shaders for Metal.

constexpr GPUShaderFormat GPU_SHADERFORMAT_METALLIB =
  SDL_GPU_SHADERFORMAT_METALLIB; ///< Precompiled metallib shaders for Metal.

/**
 * Specifies the texture format and colorspace of the swapchain textures.
 *
 * SDR will always be supported. Other compositions may not be supported on
 * certain systems.
 *
 * It is recommended to query GPUDevice.WindowSupportsSwapchainComposition after
 * claiming the window if you wish to change the swapchain composition from
 * SDR.
 *
 * - SDR: B8G8R8A8 or R8G8B8A8 swapchain. Pixel values are in sRGB encoding.
 * - SDR_LINEAR: B8G8R8A8_SRGB or R8G8B8A8_SRGB swapchain. Pixel values are
 *   stored in memory in sRGB encoding but accessed in shaders in "linear
 *   sRGB" encoding which is sRGB but with a linear transfer function.
 * - HDR_EXTENDED_LINEAR: R16G16B16A16_FLOAT swapchain. Pixel values are in
 *   extended linear sRGB encoding and permits values outside of the [0, 1]
 *   range.
 * - HDR10_ST2084: A2R10G10B10 or A2B10G10R10 swapchain. Pixel values are in
 *   BT.2020 ST2084 (PQ) encoding.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUDevice.SetSwapchainParameters
 * @sa GPUDevice.WindowSupportsSwapchainComposition
 * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
 */
using GPUSwapchainComposition = SDL_GPUSwapchainComposition;

constexpr GPUSwapchainComposition GPU_SWAPCHAINCOMPOSITION_SDR =
  SDL_GPU_SWAPCHAINCOMPOSITION_SDR; ///< GPU_SWAPCHAINCOMPOSITION_SDR

constexpr GPUSwapchainComposition GPU_SWAPCHAINCOMPOSITION_SDR_LINEAR =
  SDL_GPU_SWAPCHAINCOMPOSITION_SDR_LINEAR; ///< GPU_SWAPCHAINCOMPOSITION_SDR_LINEAR

constexpr GPUSwapchainComposition GPU_SWAPCHAINCOMPOSITION_HDR_EXTENDED_LINEAR =
  SDL_GPU_SWAPCHAINCOMPOSITION_HDR_EXTENDED_LINEAR; ///< GPU_SWAPCHAINCOMPOSITION_HDR_EXTENDED_LINEAR

constexpr GPUSwapchainComposition GPU_SWAPCHAINCOMPOSITION_HDR10_ST2084 =
  SDL_GPU_SWAPCHAINCOMPOSITION_HDR10_ST2084; ///< GPU_SWAPCHAINCOMPOSITION_HDR10_ST2084

/**
 * Specifies the timing that will be used to present swapchain textures to the
 * OS.
 *
 * VSYNC mode will always be supported. IMMEDIATE and MAILBOX modes may not be
 * supported on certain systems.
 *
 * It is recommended to query GPUDevice.WindowSupportsPresentMode after claiming
 * the window if you wish to change the present mode to IMMEDIATE or MAILBOX.
 *
 * - VSYNC: Waits for vblank before presenting. No tearing is possible. If
 *   there is a pending image to present, the new image is enqueued for
 *   presentation. Disallows tearing at the cost of visual latency.
 * - IMMEDIATE: Immediately presents. Lowest latency option, but tearing may
 *   occur.
 * - MAILBOX: Waits for vblank before presenting. No tearing is possible. If
 *   there is a pending image to present, the pending image is replaced by the
 *   new image. Similar to VSYNC, but with reduced visual latency.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUDevice.SetSwapchainParameters
 * @sa GPUDevice.WindowSupportsPresentMode
 * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
 */
using GPUPresentMode = SDL_GPUPresentMode;

constexpr GPUPresentMode GPU_PRESENTMODE_VSYNC =
  SDL_GPU_PRESENTMODE_VSYNC; ///< GPU_PRESENTMODE_VSYNC

constexpr GPUPresentMode GPU_PRESENTMODE_IMMEDIATE =
  SDL_GPU_PRESENTMODE_IMMEDIATE; ///< GPU_PRESENTMODE_IMMEDIATE

constexpr GPUPresentMode GPU_PRESENTMODE_MAILBOX =
  SDL_GPU_PRESENTMODE_MAILBOX; ///< GPU_PRESENTMODE_MAILBOX

/**
 * Specifies the pixel format of a texture.
 *
 * Texture format support varies depending on driver, hardware, and usage
 * flags. In general, you should use GPUDevice.TextureSupportsFormat to query if
 * a format is supported before using it. However, there are a few guaranteed
 * formats.
 *
 * FIXME: Check universal support for 32-bit component formats FIXME: Check
 * universal support for SIMULTANEOUS_READ_WRITE
 *
 * For SAMPLER usage, the following formats are universally supported:
 *
 * - R8G8B8A8_UNORM
 * - B8G8R8A8_UNORM
 * - R8_UNORM
 * - R8_SNORM
 * - R8G8_UNORM
 * - R8G8_SNORM
 * - R8G8B8A8_SNORM
 * - R16_FLOAT
 * - R16G16_FLOAT
 * - R16G16B16A16_FLOAT
 * - R32_FLOAT
 * - R32G32_FLOAT
 * - R32G32B32A32_FLOAT
 * - R11G11B10_UFLOAT
 * - R8G8B8A8_UNORM_SRGB
 * - B8G8R8A8_UNORM_SRGB
 * - D16_UNORM
 *
 * For COLOR_TARGET usage, the following formats are universally supported:
 *
 * - R8G8B8A8_UNORM
 * - B8G8R8A8_UNORM
 * - R8_UNORM
 * - R16_FLOAT
 * - R16G16_FLOAT
 * - R16G16B16A16_FLOAT
 * - R32_FLOAT
 * - R32G32_FLOAT
 * - R32G32B32A32_FLOAT
 * - R8_UINT
 * - R8G8_UINT
 * - R8G8B8A8_UINT
 * - R16_UINT
 * - R16G16_UINT
 * - R16G16B16A16_UINT
 * - R8_INT
 * - R8G8_INT
 * - R8G8B8A8_INT
 * - R16_INT
 * - R16G16_INT
 * - R16G16B16A16_INT
 * - R8G8B8A8_UNORM_SRGB
 * - B8G8R8A8_UNORM_SRGB
 *
 * For STORAGE usages, the following formats are universally supported:
 *
 * - R8G8B8A8_UNORM
 * - R8G8B8A8_SNORM
 * - R16G16B16A16_FLOAT
 * - R32_FLOAT
 * - R32G32_FLOAT
 * - R32G32B32A32_FLOAT
 * - R8G8B8A8_UINT
 * - R16G16B16A16_UINT
 * - R8G8B8A8_INT
 * - R16G16B16A16_INT
 *
 * For DEPTH_STENCIL_TARGET usage, the following formats are universally
 * supported:
 *
 * - D16_UNORM
 * - Either (but not necessarily both!) D24_UNORM or D32_FLOAT
 * - Either (but not necessarily both!) D24_UNORM_S8_UINT or D32_FLOAT_S8_UINT
 *
 * Unless D16_UNORM is sufficient for your purposes, always check which of
 * D24/D32 is supported before creating a depth-stencil texture!
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUTexture.GPUTexture
 * @sa GPUDevice.TextureSupportsFormat
 */
using GPUTextureFormat = SDL_GPUTextureFormat;

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_INVALID =
  SDL_GPU_TEXTUREFORMAT_INVALID; ///< GPU_TEXTUREFORMAT_INVALID

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_A8_UNORM =
  SDL_GPU_TEXTUREFORMAT_A8_UNORM; ///< GPU_TEXTUREFORMAT_A8_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8_UNORM =
  SDL_GPU_TEXTUREFORMAT_R8_UNORM; ///< GPU_TEXTUREFORMAT_R8_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8G8_UNORM =
  SDL_GPU_TEXTUREFORMAT_R8G8_UNORM; ///< GPU_TEXTUREFORMAT_R8G8_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8G8B8A8_UNORM =
  SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM; ///< GPU_TEXTUREFORMAT_R8G8B8A8_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16_UNORM =
  SDL_GPU_TEXTUREFORMAT_R16_UNORM; ///< GPU_TEXTUREFORMAT_R16_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16_UNORM =
  SDL_GPU_TEXTUREFORMAT_R16G16_UNORM; ///< GPU_TEXTUREFORMAT_R16G16_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16B16A16_UNORM =
  SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UNORM; ///< GPU_TEXTUREFORMAT_R16G16B16A16_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R10G10B10A2_UNORM =
  SDL_GPU_TEXTUREFORMAT_R10G10B10A2_UNORM; ///< GPU_TEXTUREFORMAT_R10G10B10A2_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_B5G6R5_UNORM =
  SDL_GPU_TEXTUREFORMAT_B5G6R5_UNORM; ///< GPU_TEXTUREFORMAT_B5G6R5_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_B5G5R5A1_UNORM =
  SDL_GPU_TEXTUREFORMAT_B5G5R5A1_UNORM; ///< GPU_TEXTUREFORMAT_B5G5R5A1_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_B4G4R4A4_UNORM =
  SDL_GPU_TEXTUREFORMAT_B4G4R4A4_UNORM; ///< GPU_TEXTUREFORMAT_B4G4R4A4_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_B8G8R8A8_UNORM =
  SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM; ///< GPU_TEXTUREFORMAT_B8G8R8A8_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC1_RGBA_UNORM =
  SDL_GPU_TEXTUREFORMAT_BC1_RGBA_UNORM; ///< GPU_TEXTUREFORMAT_BC1_RGBA_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC2_RGBA_UNORM =
  SDL_GPU_TEXTUREFORMAT_BC2_RGBA_UNORM; ///< GPU_TEXTUREFORMAT_BC2_RGBA_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC3_RGBA_UNORM =
  SDL_GPU_TEXTUREFORMAT_BC3_RGBA_UNORM; ///< GPU_TEXTUREFORMAT_BC3_RGBA_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC4_R_UNORM =
  SDL_GPU_TEXTUREFORMAT_BC4_R_UNORM; ///< GPU_TEXTUREFORMAT_BC4_R_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC5_RG_UNORM =
  SDL_GPU_TEXTUREFORMAT_BC5_RG_UNORM; ///< GPU_TEXTUREFORMAT_BC5_RG_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC7_RGBA_UNORM =
  SDL_GPU_TEXTUREFORMAT_BC7_RGBA_UNORM; ///< GPU_TEXTUREFORMAT_BC7_RGBA_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC6H_RGB_FLOAT =
  SDL_GPU_TEXTUREFORMAT_BC6H_RGB_FLOAT; ///< GPU_TEXTUREFORMAT_BC6H_RGB_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC6H_RGB_UFLOAT =
  SDL_GPU_TEXTUREFORMAT_BC6H_RGB_UFLOAT; ///< GPU_TEXTUREFORMAT_BC6H_RGB_UFLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8_SNORM =
  SDL_GPU_TEXTUREFORMAT_R8_SNORM; ///< GPU_TEXTUREFORMAT_R8_SNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8G8_SNORM =
  SDL_GPU_TEXTUREFORMAT_R8G8_SNORM; ///< GPU_TEXTUREFORMAT_R8G8_SNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8G8B8A8_SNORM =
  SDL_GPU_TEXTUREFORMAT_R8G8B8A8_SNORM; ///< GPU_TEXTUREFORMAT_R8G8B8A8_SNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16_SNORM =
  SDL_GPU_TEXTUREFORMAT_R16_SNORM; ///< GPU_TEXTUREFORMAT_R16_SNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16_SNORM =
  SDL_GPU_TEXTUREFORMAT_R16G16_SNORM; ///< GPU_TEXTUREFORMAT_R16G16_SNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16B16A16_SNORM =
  SDL_GPU_TEXTUREFORMAT_R16G16B16A16_SNORM; ///< GPU_TEXTUREFORMAT_R16G16B16A16_SNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16_FLOAT =
  SDL_GPU_TEXTUREFORMAT_R16_FLOAT; ///< GPU_TEXTUREFORMAT_R16_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16_FLOAT =
  SDL_GPU_TEXTUREFORMAT_R16G16_FLOAT; ///< GPU_TEXTUREFORMAT_R16G16_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT =
  SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT; ///< GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R32_FLOAT =
  SDL_GPU_TEXTUREFORMAT_R32_FLOAT; ///< GPU_TEXTUREFORMAT_R32_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R32G32_FLOAT =
  SDL_GPU_TEXTUREFORMAT_R32G32_FLOAT; ///< GPU_TEXTUREFORMAT_R32G32_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT =
  SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT; ///< GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R11G11B10_UFLOAT =
  SDL_GPU_TEXTUREFORMAT_R11G11B10_UFLOAT; ///< GPU_TEXTUREFORMAT_R11G11B10_UFLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8_UINT =
  SDL_GPU_TEXTUREFORMAT_R8_UINT; ///< GPU_TEXTUREFORMAT_R8_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8G8_UINT =
  SDL_GPU_TEXTUREFORMAT_R8G8_UINT; ///< GPU_TEXTUREFORMAT_R8G8_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8G8B8A8_UINT =
  SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UINT; ///< GPU_TEXTUREFORMAT_R8G8B8A8_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16_UINT =
  SDL_GPU_TEXTUREFORMAT_R16_UINT; ///< GPU_TEXTUREFORMAT_R16_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16_UINT =
  SDL_GPU_TEXTUREFORMAT_R16G16_UINT; ///< GPU_TEXTUREFORMAT_R16G16_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16B16A16_UINT =
  SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UINT; ///< GPU_TEXTUREFORMAT_R16G16B16A16_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R32_UINT =
  SDL_GPU_TEXTUREFORMAT_R32_UINT; ///< GPU_TEXTUREFORMAT_R32_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R32G32_UINT =
  SDL_GPU_TEXTUREFORMAT_R32G32_UINT; ///< GPU_TEXTUREFORMAT_R32G32_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R32G32B32A32_UINT =
  SDL_GPU_TEXTUREFORMAT_R32G32B32A32_UINT; ///< GPU_TEXTUREFORMAT_R32G32B32A32_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8_INT =
  SDL_GPU_TEXTUREFORMAT_R8_INT; ///< GPU_TEXTUREFORMAT_R8_INT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8G8_INT =
  SDL_GPU_TEXTUREFORMAT_R8G8_INT; ///< GPU_TEXTUREFORMAT_R8G8_INT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8G8B8A8_INT =
  SDL_GPU_TEXTUREFORMAT_R8G8B8A8_INT; ///< GPU_TEXTUREFORMAT_R8G8B8A8_INT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16_INT =
  SDL_GPU_TEXTUREFORMAT_R16_INT; ///< GPU_TEXTUREFORMAT_R16_INT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16_INT =
  SDL_GPU_TEXTUREFORMAT_R16G16_INT; ///< GPU_TEXTUREFORMAT_R16G16_INT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R16G16B16A16_INT =
  SDL_GPU_TEXTUREFORMAT_R16G16B16A16_INT; ///< GPU_TEXTUREFORMAT_R16G16B16A16_INT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R32_INT =
  SDL_GPU_TEXTUREFORMAT_R32_INT; ///< GPU_TEXTUREFORMAT_R32_INT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R32G32_INT =
  SDL_GPU_TEXTUREFORMAT_R32G32_INT; ///< GPU_TEXTUREFORMAT_R32G32_INT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R32G32B32A32_INT =
  SDL_GPU_TEXTUREFORMAT_R32G32B32A32_INT; ///< GPU_TEXTUREFORMAT_R32G32B32A32_INT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_R8G8B8A8_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_R8G8B8A8_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_B8G8R8A8_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_B8G8R8A8_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC1_RGBA_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_BC1_RGBA_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_BC1_RGBA_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC2_RGBA_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_BC2_RGBA_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_BC2_RGBA_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC3_RGBA_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_BC3_RGBA_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_BC3_RGBA_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_BC7_RGBA_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_BC7_RGBA_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_BC7_RGBA_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_D16_UNORM =
  SDL_GPU_TEXTUREFORMAT_D16_UNORM; ///< GPU_TEXTUREFORMAT_D16_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_D24_UNORM =
  SDL_GPU_TEXTUREFORMAT_D24_UNORM; ///< GPU_TEXTUREFORMAT_D24_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_D32_FLOAT =
  SDL_GPU_TEXTUREFORMAT_D32_FLOAT; ///< GPU_TEXTUREFORMAT_D32_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT =
  SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT; ///< GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT =
  SDL_GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT; ///< GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_4x4_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_4x4_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_4x4_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_5x4_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_5x4_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_5x4_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_5x5_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_5x5_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_5x5_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_6x5_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_6x5_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_6x5_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_6x6_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_6x6_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_6x6_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_8x5_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_8x5_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_8x5_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_8x6_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_8x6_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_8x6_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_8x8_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_8x8_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_8x8_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x5_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x5_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_10x5_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x6_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x6_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_10x6_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x8_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x8_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_10x8_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x10_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x10_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_10x10_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_12x10_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_12x10_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_12x10_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_12x12_UNORM =
  SDL_GPU_TEXTUREFORMAT_ASTC_12x12_UNORM; ///< GPU_TEXTUREFORMAT_ASTC_12x12_UNORM

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_4x4_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_4x4_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_4x4_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_5x4_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_5x4_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_5x4_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_5x5_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_5x5_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_5x5_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_6x5_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_6x5_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_6x5_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_6x6_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_6x6_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_6x6_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_8x5_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_8x5_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_8x5_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_8x6_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_8x6_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_8x6_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_8x8_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_8x8_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_8x8_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x5_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x5_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_10x5_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x6_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x6_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_10x6_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x8_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x8_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_10x8_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x10_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x10_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_10x10_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_12x10_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_12x10_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_12x10_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_12x12_UNORM_SRGB =
  SDL_GPU_TEXTUREFORMAT_ASTC_12x12_UNORM_SRGB; ///< GPU_TEXTUREFORMAT_ASTC_12x12_UNORM_SRGB

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_4x4_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_4x4_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_4x4_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_5x4_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_5x4_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_5x4_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_5x5_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_5x5_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_5x5_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_6x5_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_6x5_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_6x5_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_6x6_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_6x6_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_6x6_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_8x5_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_8x5_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_8x5_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_8x6_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_8x6_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_8x6_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_8x8_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_8x8_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_8x8_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x5_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x5_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_10x5_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x6_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x6_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_10x6_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x8_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x8_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_10x8_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_10x10_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_10x10_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_10x10_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_12x10_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_12x10_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_12x10_FLOAT

constexpr GPUTextureFormat GPU_TEXTUREFORMAT_ASTC_12x12_FLOAT =
  SDL_GPU_TEXTUREFORMAT_ASTC_12x12_FLOAT; ///< GPU_TEXTUREFORMAT_ASTC_12x12_FLOAT

/**
 * Specifies the type of a texture.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUTexture.GPUTexture
 */
using GPUTextureType = SDL_GPUTextureType;

constexpr GPUTextureType GPU_TEXTURETYPE_2D =
  SDL_GPU_TEXTURETYPE_2D; ///< The texture is a 2-dimensional image.

constexpr GPUTextureType GPU_TEXTURETYPE_2D_ARRAY =
  SDL_GPU_TEXTURETYPE_2D_ARRAY; ///< The texture is a 2-dimensional array image.

constexpr GPUTextureType GPU_TEXTURETYPE_3D =
  SDL_GPU_TEXTURETYPE_3D; ///< The texture is a 3-dimensional image.

constexpr GPUTextureType GPU_TEXTURETYPE_CUBE =
  SDL_GPU_TEXTURETYPE_CUBE; ///< The texture is a cube image.

constexpr GPUTextureType GPU_TEXTURETYPE_CUBE_ARRAY =
  SDL_GPU_TEXTURETYPE_CUBE_ARRAY; ///< The texture is a cube array image.

/**
 * Specifies how a texture is intended to be used by the client.
 *
 * A texture must have at least one usage flag. Note that some usage flag
 * combinations are invalid.
 *
 * With regards to compute storage usage, READ | WRITE means that you can have
 * shader A that only writes into the texture and shader B that only reads
 * from the texture and bind the same texture to either shader respectively.
 * SIMULTANEOUS means that you can do reads and writes within the same shader
 * or compute pass. It also implies that atomic ops can be used, since those
 * are read-modify-write operations. If you use SIMULTANEOUS, you are
 * responsible for avoiding data races, as there is no data synchronization
 * within a compute pass. Note that SIMULTANEOUS usage is only supported by a
 * limited number of texture formats.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GPUTexture.GPUTexture
 */
using GPUTextureUsageFlags = Uint32;

constexpr GPUTextureUsageFlags GPU_TEXTUREUSAGE_SAMPLER =
  SDL_GPU_TEXTUREUSAGE_SAMPLER; ///< Texture supports sampling.

constexpr GPUTextureUsageFlags GPU_TEXTUREUSAGE_COLOR_TARGET =
  SDL_GPU_TEXTUREUSAGE_COLOR_TARGET; ///< Texture is a color render target.

constexpr GPUTextureUsageFlags GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET =
  SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET; ///< Texture is a depth stencil
                                             ///< target.

constexpr GPUTextureUsageFlags GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ =
  SDL_GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ; ///< Texture supports storage
                                              ///< reads in graphics stages.

/// Texture supports storage reads in the compute stage.
constexpr GPUTextureUsageFlags GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ =
  SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ;

/// Texture supports storage writes in the compute stage.
constexpr GPUTextureUsageFlags GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE =
  SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE;

/**
 * Texture supports reads and writes in the same compute shader. This is NOT
 * equivalent to READ | WRITE.
 */
constexpr GPUTextureUsageFlags
  GPU_TEXTUREUSAGE_COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE =
    SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE;

/**
 * Specifies the sample count of a texture.
 *
 * Used in multisampling. Note that this value only applies when the texture
 * is used as a render target.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUTexture.GPUTexture
 * @sa GPUDevice.TextureSupportsSampleCount
 */
using GPUSampleCount = SDL_GPUSampleCount;

constexpr GPUSampleCount GPU_SAMPLECOUNT_1 =
  SDL_GPU_SAMPLECOUNT_1; ///< No multisampling.

constexpr GPUSampleCount GPU_SAMPLECOUNT_2 =
  SDL_GPU_SAMPLECOUNT_2; ///< MSAA 2x.

constexpr GPUSampleCount GPU_SAMPLECOUNT_4 =
  SDL_GPU_SAMPLECOUNT_4; ///< MSAA 4x.

constexpr GPUSampleCount GPU_SAMPLECOUNT_8 =
  SDL_GPU_SAMPLECOUNT_8; ///< MSAA 8x.

/**
 * An opaque handle representing the SDL_GPU context.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class GPUDevice
{
  GPUDeviceRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr GPUDevice() = default;

  /**
   * Constructs from GPUDeviceParam.
   *
   * @param resource a GPUDeviceRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit GPUDevice(const GPUDeviceRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr GPUDevice(const GPUDevice& other) = delete;

  /// Move constructor
  constexpr GPUDevice(GPUDevice&& other)
    : GPUDevice(other.release())
  {
  }

  constexpr GPUDevice(const GPUDeviceRef& other) = delete;

  constexpr GPUDevice(GPUDeviceRef&& other) = delete;

  /**
   * Creates a GPU context.
   *
   * @param format_flags a bitflag indicating which shader formats the app is
   *                     able to provide.
   * @param debug_mode enable debug mode properties and validations.
   * @param name the preferred GPU driver, or nullptr to let SDL pick the
   * optimal driver.
   * @post a GPU context on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.GetShaderFormats
   * @sa GPUDevice.GetDriver
   * @sa GPUDevice.Destroy
   * @sa GPUSupportsShaderFormats
   */
  GPUDevice(GPUShaderFormat format_flags, bool debug_mode, StringParam name)
    : m_resource(
        CheckError(SDL_CreateGPUDevice(format_flags, debug_mode, name)))
  {
  }

  /**
   * Creates a GPU context.
   *
   * These are the supported properties:
   *
   * - `prop::GpuDevice.CREATE_DEBUGMODE_BOOLEAN`: enable debug mode
   *   properties and validations, defaults to true.
   * - `prop::GpuDevice.CREATE_PREFERLOWPOWER_BOOLEAN`: enable to prefer
   *   energy efficiency over maximum GPU performance, defaults to false.
   * - `prop::GpuDevice.CREATE_NAME_STRING`: the name of the GPU driver to
   *   use, if a specific one is desired.
   *
   * These are the current shader format properties:
   *
   * - `prop::GpuDevice.CREATE_SHADERS_PRIVATE_BOOLEAN`: The app is able to
   *   provide shaders for an NDA platform.
   * - `prop::GpuDevice.CREATE_SHADERS_SPIRV_BOOLEAN`: The app is able to
   *   provide SPIR-V shaders if applicable.
   * - `prop::GpuDevice.CREATE_SHADERS_DXBC_BOOLEAN`: The app is able to
   *   provide DXBC shaders if applicable
   * - `prop::GpuDevice.CREATE_SHADERS_DXIL_BOOLEAN`: The app is able to
   *   provide DXIL shaders if applicable.
   * - `prop::GpuDevice.CREATE_SHADERS_MSL_BOOLEAN`: The app is able to
   *   provide MSL shaders if applicable.
   * - `prop::GpuDevice.CREATE_SHADERS_METALLIB_BOOLEAN`: The app is able to
   *   provide Metal shader libraries if applicable.
   *
   * With the D3D12 renderer:
   *
   * - `prop::GpuDevice.CREATE_D3D12_SEMANTIC_NAME_STRING`: the prefix to
   *   use for all vertex semantics, default is "TEXCOORD".
   *
   * @param props the properties to use.
   * @post a GPU context on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.GetShaderFormats
   * @sa GPUDevice.GetDriver
   * @sa GPUDevice.Destroy
   * @sa GPUSupportsProperties
   */
  GPUDevice(PropertiesParam props)
    : m_resource(CheckError(SDL_CreateGPUDeviceWithProperties(props)))
  {
  }

  /// Destructor
  ~GPUDevice() { SDL_DestroyGPUDevice(m_resource); }

  /// Assignment operator.
  GPUDevice& operator=(GPUDevice other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying GPUDeviceRaw.
  constexpr GPUDeviceRaw get() const { return m_resource; }

  /// Retrieves underlying GPUDeviceRaw and clear this.
  constexpr GPUDeviceRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const GPUDevice& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to GPUDeviceParam
  constexpr operator GPUDeviceParam() const { return {m_resource}; }

  /**
   * Destroys a GPU context previously returned by GPUDevice.GPUDevice.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.GPUDevice
   */
  void Destroy();

  /**
   * Returns the name of the backend used to create this GPU context.
   *
   * @returns the name of the device's driver, or nullptr on error.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const char* GetDriver();

  /**
   * Returns the supported shader formats for this GPU context.
   *
   * @returns a bitflag indicating which shader formats the driver is able to
   *          consume.
   *
   * @since This function is available since SDL 3.2.0.
   */
  GPUShaderFormat GetShaderFormats();

  /**
   * Creates a pipeline object to be used in a compute workflow.
   *
   * Shader resource bindings must be authored to follow a particular order
   * depending on the shader format.
   *
   * For SPIR-V shaders, use the following resource sets:
   *
   * - 0: Sampled textures, followed by read-only storage textures, followed by
   *   read-only storage buffers
   * - 1: Read-write storage textures, followed by read-write storage buffers
   * - 2: Uniform buffers
   *
   * For DXBC and DXIL shaders, use the following register order:
   *
   * - (t[n], space0): Sampled textures, followed by read-only storage textures,
   *   followed by read-only storage buffers
   * - (u[n], space1): Read-write storage textures, followed by read-write
   *   storage buffers
   * - (b[n], space2): Uniform buffers
   *
   * For MSL/metallib, use the following order:
   *
   * - [[buffer]]: Uniform buffers, followed by read-only storage buffers,
   *   followed by read-write storage buffers
   * - [[texture]]: Sampled textures, followed by read-only storage textures,
   *   followed by read-write storage textures
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUComputePipeline.CREATE_NAME_STRING`: a name that can be
   *   displayed in debugging tools.
   *
   * @param createinfo a struct describing the state of the compute pipeline to
   *                   create.
   * @returns a compute pipeline object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUComputePass.BindPipeline
   * @sa GPUDevice.ReleaseComputePipeline
   */
  GPUComputePipeline CreateComputePipeline(
    const GPUComputePipelineCreateInfo& createinfo);

  /**
   * Creates a pipeline object to be used in a graphics workflow.
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUGraphicsPipeline.CREATE_NAME_STRING`: a name that can be
   *   displayed in debugging tools.
   *
   * @param createinfo a struct describing the state of the graphics pipeline to
   *                   create.
   * @returns a graphics pipeline object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUShader.GPUShader
   * @sa GPURenderPass.BindPipeline
   * @sa GPUDevice.ReleaseGraphicsPipeline
   */
  GPUGraphicsPipeline CreateGraphicsPipeline(
    const GPUGraphicsPipelineCreateInfo& createinfo);

  /**
   * Creates a sampler object to be used when binding textures in a graphics
   * workflow.
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUSampler.CREATE_NAME_STRING`: a name that can be displayed
   *   in debugging tools.
   *
   * @param createinfo a struct describing the state of the sampler to create.
   * @returns a sampler object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPURenderPass.BindVertexSamplers
   * @sa GPURenderPass.BindFragmentSamplers
   * @sa GPUDevice.ReleaseSampler
   */
  GPUSampler CreateSampler(const GPUSamplerCreateInfo& createinfo);

  /**
   * Creates a shader to be used when creating a graphics pipeline.
   *
   * Shader resource bindings must be authored to follow a particular order
   * depending on the shader format.
   *
   * For SPIR-V shaders, use the following resource sets:
   *
   * For vertex shaders:
   *
   * - 0: Sampled textures, followed by storage textures, followed by storage
   *   buffers
   * - 1: Uniform buffers
   *
   * For fragment shaders:
   *
   * - 2: Sampled textures, followed by storage textures, followed by storage
   *   buffers
   * - 3: Uniform buffers
   *
   * For DXBC and DXIL shaders, use the following register order:
   *
   * For vertex shaders:
   *
   * - (t[n], space0): Sampled textures, followed by storage textures, followed
   *   by storage buffers
   * - (s[n], space0): Samplers with indices corresponding to the sampled
   *   textures
   * - (b[n], space1): Uniform buffers
   *
   * For pixel shaders:
   *
   * - (t[n], space2): Sampled textures, followed by storage textures, followed
   *   by storage buffers
   * - (s[n], space2): Samplers with indices corresponding to the sampled
   *   textures
   * - (b[n], space3): Uniform buffers
   *
   * For MSL/metallib, use the following order:
   *
   * - [[texture]]: Sampled textures, followed by storage textures
   * - [[sampler]]: Samplers with indices corresponding to the sampled textures
   * - [[buffer]]: Uniform buffers, followed by storage buffers. Vertex buffer 0
   *   is bound at [[buffer(14)]], vertex buffer 1 at [[buffer(15)]], and so on.
   *   Rather than manually authoring vertex buffer indices, use the
   *   [[stage_in]] attribute which will automatically use the vertex input
   *   information from the GPUGraphicsPipeline.
   *
   * Shader semantics other than system-value semantics do not matter in D3D12
   * and for ease of use the SDL implementation assumes that non system-value
   * semantics will all be TEXCOORD. If you are using HLSL as the shader source
   * language, your vertex semantics should start at TEXCOORD0 and increment
   * like so: TEXCOORD1, TEXCOORD2, etc. If you wish to change the semantic
   * prefix to something other than TEXCOORD you can use
   * prop::GpuDevice.CREATE_D3D12_SEMANTIC_NAME_STRING with
   * GPUDevice.GPUDevice().
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUShader.CREATE_NAME_STRING`: a name that can be displayed in
   *   debugging tools.
   *
   * @param createinfo a struct describing the state of the shader to create.
   * @returns a shader object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
   * @sa GPUDevice.ReleaseShader
   */
  GPUShader CreateShader(const GPUShaderCreateInfo& createinfo);

  /**
   * Creates a texture object to be used in graphics or compute workflows.
   *
   * The contents of this texture are undefined until data is written to the
   * texture.
   *
   * Note that certain combinations of usage flags are invalid. For example, a
   * texture cannot have both the SAMPLER and GRAPHICS_STORAGE_READ flags.
   *
   * If you request a sample count higher than the hardware supports, the
   * implementation will automatically fall back to the highest available sample
   * count.
   *
   * There are optional properties that can be provided through
   * GPUTextureCreateInfo's `props`. These are the supported properties:
   *
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_R_FLOAT`: (Direct3D 12 only) if
   *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
   *   to a color with this red intensity. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_G_FLOAT`: (Direct3D 12 only) if
   *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
   *   to a color with this green intensity. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_B_FLOAT`: (Direct3D 12 only) if
   *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
   *   to a color with this blue intensity. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_A_FLOAT`: (Direct3D 12 only) if
   *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
   *   to a color with this alpha intensity. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_DEPTH_FLOAT`: (Direct3D 12 only)
   *   if the texture usage is GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET, clear
   *   the texture to a depth of this value. Defaults to zero.
   * - `prop::GPUTexture.CREATE_D3D12_CLEAR_STENCIL_NUMBER`: (Direct3D 12
   *   only) if the texture usage is GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
   *   clear the texture to a stencil of this Uint8 value. Defaults to zero.
   * - `prop::GPUTexture.CREATE_NAME_STRING`: a name that can be displayed
   *   in debugging tools.
   *
   * @param createinfo a struct describing the state of the texture to create.
   * @returns a texture object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCopyPass.UploadToTexture
   * @sa GPUCopyPass.DownloadFromTexture
   * @sa GPURenderPass.BindVertexSamplers
   * @sa GPURenderPass.BindVertexStorageTextures
   * @sa GPURenderPass.BindFragmentSamplers
   * @sa GPURenderPass.BindFragmentStorageTextures
   * @sa GPUComputePass.BindStorageTextures
   * @sa GPUCommandBuffer.BlitTexture
   * @sa GPUDevice.ReleaseTexture
   * @sa GPUDevice.TextureSupportsFormat
   */
  GPUTexture CreateTexture(const GPUTextureCreateInfo& createinfo);

  /**
   * Creates a buffer object to be used in graphics or compute workflows.
   *
   * The contents of this buffer are undefined until data is written to the
   * buffer.
   *
   * Note that certain combinations of usage flags are invalid. For example, a
   * buffer cannot have both the VERTEX and INDEX flags.
   *
   * If you use a STORAGE flag, the data in the buffer must respect std140
   * layout conventions. In practical terms this means you must ensure that vec3
   * and vec4 fields are 16-byte aligned.
   *
   * For better understanding of underlying concepts and memory management with
   * SDL GPU API, you may refer
   * [this blog post](https://moonside.games/posts/sdl-gpu-concepts-cycling/).
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUBuffer.CREATE_NAME_STRING`: a name that can be displayed in
   *   debugging tools.
   *
   * @param createinfo a struct describing the state of the buffer to create.
   * @returns a buffer object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCopyPass.UploadToBuffer
   * @sa GPUCopyPass.DownloadFromBuffer
   * @sa GPUCopyPass.CopyBufferToBuffer
   * @sa GPURenderPass.BindVertexBuffers
   * @sa GPURenderPass.BindIndexBuffer
   * @sa GPURenderPass.BindVertexStorageBuffers
   * @sa GPURenderPass.BindFragmentStorageBuffers
   * @sa GPURenderPass.DrawPrimitivesIndirect
   * @sa GPURenderPass.DrawIndexedPrimitivesIndirect
   * @sa GPUComputePass.BindStorageBuffers
   * @sa GPUComputePass.DispatchIndirect
   * @sa GPUDevice.ReleaseBuffer
   */
  GPUBuffer CreateBuffer(const GPUBufferCreateInfo& createinfo);

  /**
   * Creates a transfer buffer to be used when uploading to or downloading from
   * graphics resources.
   *
   * Download buffers can be particularly expensive to create, so it is good
   * practice to reuse them if data will be downloaded regularly.
   *
   * There are optional properties that can be provided through `props`. These
   * are the supported properties:
   *
   * - `prop::GPUTransferBuffer.CREATE_NAME_STRING`: a name that can be
   *   displayed in debugging tools.
   *
   * @param createinfo a struct describing the state of the transfer buffer to
   *                   create.
   * @returns a transfer buffer on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCopyPass.UploadToBuffer
   * @sa GPUCopyPass.DownloadFromBuffer
   * @sa GPUCopyPass.UploadToTexture
   * @sa GPUCopyPass.DownloadFromTexture
   * @sa GPUDevice.ReleaseTransferBuffer
   */
  GPUTransferBuffer CreateTransferBuffer(
    const GPUTransferBufferCreateInfo& createinfo);

  /**
   * Sets an arbitrary string constant to label a buffer.
   *
   * You should use prop::GPUBuffer.CREATE_NAME_STRING with
   * GPUBuffer.GPUBuffer instead of this function to avoid thread safety issues.
   *
   * @param buffer a buffer to attach the name to.
   * @param text a UTF-8 string constant to mark as the name of the buffer.
   *
   * @threadsafety This function is not thread safe, you must make sure the
   *               buffer is not simultaneously used by any other thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUBuffer.GPUBuffer
   */
  void SetBufferName(GPUBuffer buffer, StringParam text);

  /**
   * Sets an arbitrary string constant to label a texture.
   *
   * You should use prop::GPUTexture.CREATE_NAME_STRING with
   * GPUTexture.GPUTexture instead of this function to avoid thread safety
   * issues.
   *
   * @param texture a texture to attach the name to.
   * @param text a UTF-8 string constant to mark as the name of the texture.
   *
   * @threadsafety This function is not thread safe, you must make sure the
   *               texture is not simultaneously used by any other thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUTexture.GPUTexture
   */
  void SetTextureName(GPUTexture texture, StringParam text);

  /**
   * Frees the given texture as soon as it is safe to do so.
   *
   * You must not reference the texture after calling this function.
   *
   * @param texture a texture to be destroyed.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReleaseTexture(GPUTexture texture);

  /**
   * Frees the given sampler as soon as it is safe to do so.
   *
   * You must not reference the sampler after calling this function.
   *
   * @param sampler a sampler to be destroyed.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReleaseSampler(GPUSampler sampler);

  /**
   * Frees the given buffer as soon as it is safe to do so.
   *
   * You must not reference the buffer after calling this function.
   *
   * @param buffer a buffer to be destroyed.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReleaseBuffer(GPUBuffer buffer);

  /**
   * Frees the given transfer buffer as soon as it is safe to do so.
   *
   * You must not reference the transfer buffer after calling this function.
   *
   * @param transfer_buffer a transfer buffer to be destroyed.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReleaseTransferBuffer(GPUTransferBuffer transfer_buffer);

  /**
   * Frees the given compute pipeline as soon as it is safe to do so.
   *
   * You must not reference the compute pipeline after calling this function.
   *
   * @param compute_pipeline a compute pipeline to be destroyed.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReleaseComputePipeline(GPUComputePipeline compute_pipeline);

  /**
   * Frees the given shader as soon as it is safe to do so.
   *
   * You must not reference the shader after calling this function.
   *
   * @param shader a shader to be destroyed.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReleaseShader(GPUShader shader);

  /**
   * Frees the given graphics pipeline as soon as it is safe to do so.
   *
   * You must not reference the graphics pipeline after calling this function.
   *
   * @param graphics_pipeline a graphics pipeline to be destroyed.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void ReleaseGraphicsPipeline(GPUGraphicsPipeline graphics_pipeline);

  /**
   * Acquire a command buffer.
   *
   * This command buffer is managed by the implementation and should not be
   * freed by the user. The command buffer may only be used on the thread it was
   * acquired on. The command buffer should be submitted on the thread it was
   * acquired on.
   *
   * It is valid to acquire multiple command buffers on the same thread at once.
   * In fact a common design pattern is to acquire two command buffers per frame
   * where one is dedicated to render and compute passes and the other is
   * dedicated to copy passes and other preparatory work such as generating
   * mipmaps. Interleaving commands between the two command buffers reduces the
   * total amount of passes overall which improves rendering performance.
   *
   * @returns a command buffer, or nullptr on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.Submit
   * @sa GPUCommandBuffer.SubmitAndAcquireFence
   */
  GPUCommandBuffer AcquireCommandBuffer();

  /**
   * Maps a transfer buffer into application address space.
   *
   * You must unmap the transfer buffer before encoding upload commands. The
   * memory is owned by the graphics driver - do NOT call free() on the
   * returned pointer.
   *
   * @param transfer_buffer a transfer buffer.
   * @param cycle if true, cycles the transfer buffer if it is already bound.
   * @returns the address of the mapped transfer buffer memory, or nullptr on
   *          failure; call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void* MapTransferBuffer(GPUTransferBuffer transfer_buffer, bool cycle);

  /**
   * Unmaps a previously mapped transfer buffer.
   *
   * @param transfer_buffer a previously mapped transfer buffer.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void UnmapTransferBuffer(GPUTransferBuffer transfer_buffer);

  /**
   * Determines whether a swapchain composition is supported by the window.
   *
   * The window must be claimed before calling this function.
   *
   * @param window an Window.
   * @param swapchain_composition the swapchain composition to check.
   * @returns true if supported, false if unsupported.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.ClaimWindow
   */
  bool WindowSupportsSwapchainComposition(
    WindowParam window,
    GPUSwapchainComposition swapchain_composition);

  /**
   * Determines whether a presentation mode is supported by the window.
   *
   * The window must be claimed before calling this function.
   *
   * @param window an Window.
   * @param present_mode the presentation mode to check.
   * @returns true if supported, false if unsupported.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.ClaimWindow
   */
  bool WindowSupportsPresentMode(WindowParam window,
                                 GPUPresentMode present_mode);

  /**
   * Claims a window, creating a swapchain structure for it.
   *
   * This must be called before GPUCommandBuffer.AcquireSwapchainTexture is
   * called using the window. You should only call this function from the thread
   * that created the window.
   *
   * The swapchain will be created with GPU_SWAPCHAINCOMPOSITION_SDR and
   * GPU_PRESENTMODE_VSYNC. If you want to have different swapchain
   * parameters, you must call GPUDevice.SetSwapchainParameters after claiming
   * the window.
   *
   * @param window an Window.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called from the thread that
   *               created the window.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
   * @sa GPUDevice.ReleaseWindow
   * @sa GPUDevice.WindowSupportsPresentMode
   * @sa GPUDevice.WindowSupportsSwapchainComposition
   */
  void ClaimWindow(WindowParam window);

  /**
   * Unclaims a window, destroying its swapchain structure.
   *
   * @param window an Window that has been claimed.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.ClaimWindow
   */
  void ReleaseWindow(WindowParam window);

  /**
   * Changes the swapchain parameters for the given claimed window.
   *
   * This function will fail if the requested present mode or swapchain
   * composition are unsupported by the device. Check if the parameters are
   * supported via GPUDevice.WindowSupportsPresentMode /
   * GPUDevice.WindowSupportsSwapchainComposition prior to calling this
   * function.
   *
   * GPU_PRESENTMODE_VSYNC with GPU_SWAPCHAINCOMPOSITION_SDR are always
   * supported.
   *
   * @param window an Window that has been claimed.
   * @param swapchain_composition the desired composition of the swapchain.
   * @param present_mode the desired present mode for the swapchain.
   * @returns true if successful, false on error; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.WindowSupportsPresentMode
   * @sa GPUDevice.WindowSupportsSwapchainComposition
   */
  bool SetSwapchainParameters(WindowParam window,
                              GPUSwapchainComposition swapchain_composition,
                              GPUPresentMode present_mode);

  /**
   * Configures the maximum allowed number of frames in flight.
   *
   * The default value when the device is created is 2. This means that after
   * you have submitted 2 frames for presentation, if the GPU has not finished
   * working on the first frame, GPUCommandBuffer.AcquireSwapchainTexture() will
   * fill the swapchain texture pointer with nullptr, and
   * GPUCommandBuffer.WaitAndAcquireSwapchainTexture() will block.
   *
   * Higher values increase throughput at the expense of visual latency. Lower
   * values decrease visual latency at the expense of throughput.
   *
   * Note that calling this function will stall and flush the command queue to
   * prevent synchronization issues.
   *
   * The minimum value of allowed frames in flight is 1, and the maximum is 3.
   *
   * @param allowed_frames_in_flight the maximum number of frames that can be
   *                                 pending on the GPU.
   * @returns true if successful, false on error; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool SetAllowedFramesInFlight(Uint32 allowed_frames_in_flight);

  /**
   * Obtains the texture format of the swapchain for the given window.
   *
   * Note that this format can change if the swapchain parameters change.
   *
   * @param window an Window that has been claimed.
   * @returns the texture format of the swapchain.
   *
   * @since This function is available since SDL 3.2.0.
   */
  GPUTextureFormat GetSwapchainTextureFormat(WindowParam window);

  /**
   * Blocks the thread until a swapchain texture is available to be acquired.
   *
   * @param window a window that has been claimed.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called from the thread that
   *               created the window.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.AcquireSwapchainTexture
   * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
   * @sa GPUDevice.SetAllowedFramesInFlight
   */
  void WaitForSwapchain(WindowParam window);

  /**
   * Blocks the thread until the GPU is completely idle.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUDevice.WaitForFences
   */
  void WaitForIdle();

  /**
   * Blocks the thread until the given fences are signaled.
   *
   * @param wait_all if 0, wait for any fence to be signaled, if 1, wait for all
   *                 fences to be signaled.
   * @param fences an array of fences to wait on.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.SubmitAndAcquireFence
   * @sa GPUDevice.WaitForIdle
   */
  void WaitForFences(bool wait_all, std::span<GPUFence* const> fences);

  /**
   * Checks the status of a fence.
   *
   * @param fence a fence.
   * @returns true if the fence is signaled, false if it is not.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.SubmitAndAcquireFence
   */
  bool QueryFence(GPUFence* fence);

  /**
   * Releases a fence obtained from GPUCommandBuffer.SubmitAndAcquireFence.
   *
   * You must not reference the fence after calling this function.
   *
   * @param fence a fence.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GPUCommandBuffer.SubmitAndAcquireFence
   */
  void ReleaseFence(GPUFence* fence);

  /**
   * Determines whether a texture format is supported for a given type and
   * usage.
   *
   * @param format the texture format to check.
   * @param type the type of texture (2D, 3D, Cube).
   * @param usage a bitmask of all usage scenarios to check.
   * @returns whether the texture format is supported for this type and usage.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool TextureSupportsFormat(GPUTextureFormat format,
                             GPUTextureType type,
                             GPUTextureUsageFlags usage);

  /**
   * Determines if a sample count for a texture format is supported.
   *
   * @param format the texture format to check.
   * @param sample_count the sample count to check.
   * @returns whether the sample count is supported for this texture format.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool TextureSupportsSampleCount(GPUTextureFormat format,
                                  GPUSampleCount sample_count);
};

/// Semi-safe reference for GPUDevice.
struct GPUDeviceRef : GPUDevice
{
  /**
   * Constructs from GPUDeviceParam.
   *
   * @param resource a GPUDeviceRaw or GPUDevice.
   *
   * This does not takes ownership!
   */
  GPUDeviceRef(GPUDeviceParam resource)
    : GPUDevice(resource.value)
  {
  }

  /// Copy constructor.
  GPUDeviceRef(const GPUDeviceRef& other)
    : GPUDevice(other.get())
  {
  }

  /// Destructor
  ~GPUDeviceRef() { release(); }
};

/**
 * Specifies the primitive topology of a graphics pipeline.
 *
 * If you are using POINTLIST you must include a point size output in the
 * vertex shader.
 *
 * - For HLSL compiling to SPIRV you must decorate a float output with
 *   [[vk::builtin("PointSize")]].
 * - For GLSL you must set the gl_PointSize builtin.
 * - For MSL you must include a float output with the [[point_size]]
 *   decorator.
 *
 * Note that sized point topology is totally unsupported on D3D12. Any size
 * other than 1 will be ignored. In general, you should avoid using point
 * topology for both compatibility and performance reasons. You WILL regret
 * using it.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUPrimitiveType = SDL_GPUPrimitiveType;

constexpr GPUPrimitiveType GPU_PRIMITIVETYPE_TRIANGLELIST =
  SDL_GPU_PRIMITIVETYPE_TRIANGLELIST; ///< A series of separate triangles.

constexpr GPUPrimitiveType GPU_PRIMITIVETYPE_TRIANGLESTRIP =
  SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP; ///< A series of connected triangles.

constexpr GPUPrimitiveType GPU_PRIMITIVETYPE_LINELIST =
  SDL_GPU_PRIMITIVETYPE_LINELIST; ///< A series of separate lines.

constexpr GPUPrimitiveType GPU_PRIMITIVETYPE_LINESTRIP =
  SDL_GPU_PRIMITIVETYPE_LINESTRIP; ///< A series of connected lines.

constexpr GPUPrimitiveType GPU_PRIMITIVETYPE_POINTLIST =
  SDL_GPU_PRIMITIVETYPE_POINTLIST; ///< A series of separate points.

/**
 * Specifies how the contents of a texture attached to a render pass are
 * treated at the beginning of the render pass.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BeginRenderPass
 */
using GPULoadOp = SDL_GPULoadOp;

/// The previous contents of the texture will be preserved.
constexpr GPULoadOp GPU_LOADOP_LOAD = SDL_GPU_LOADOP_LOAD;

/// The contents of the texture will be cleared to a color.
constexpr GPULoadOp GPU_LOADOP_CLEAR = SDL_GPU_LOADOP_CLEAR;

/**
 * The previous contents of the texture need not be preserved.  The contents
 * will be undefined.
 */
constexpr GPULoadOp GPU_LOADOP_DONT_CARE = SDL_GPU_LOADOP_DONT_CARE;

/**
 * Specifies how the contents of a texture attached to a render pass are
 * treated at the end of the render pass.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BeginRenderPass
 */
using GPUStoreOp = SDL_GPUStoreOp;

/// The contents generated during the render pass will be written to memory.
constexpr GPUStoreOp GPU_STOREOP_STORE = SDL_GPU_STOREOP_STORE;

/**
 * The contents generated during the render pass are not needed and may be
 * discarded.  The contents will be undefined.
 */
constexpr GPUStoreOp GPU_STOREOP_DONT_CARE = SDL_GPU_STOREOP_DONT_CARE;

/**
 * The multisample contents generated during the render pass will be resolved to
 * a non-multisample texture.  The contents in the multisample texture may then
 * be discarded and will be undefined.
 */
constexpr GPUStoreOp GPU_STOREOP_RESOLVE = SDL_GPU_STOREOP_RESOLVE;

/**
 * The multisample contents generated during the render pass will be resolved to
 * a non-multisample texture.  The contents in the multisample texture will be
 * written to memory.
 */
constexpr GPUStoreOp GPU_STOREOP_RESOLVE_AND_STORE =
  SDL_GPU_STOREOP_RESOLVE_AND_STORE;

/**
 * Specifies the face of a cube map.
 *
 * Can be passed in as the layer field in texture-related structs.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using GPUCubeMapFace = SDL_GPUCubeMapFace;

constexpr GPUCubeMapFace GPU_CUBEMAPFACE_POSITIVEX =
  SDL_GPU_CUBEMAPFACE_POSITIVEX; ///< GPU_CUBEMAPFACE_POSITIVEX

constexpr GPUCubeMapFace GPU_CUBEMAPFACE_NEGATIVEX =
  SDL_GPU_CUBEMAPFACE_NEGATIVEX; ///< GPU_CUBEMAPFACE_NEGATIVEX

constexpr GPUCubeMapFace GPU_CUBEMAPFACE_POSITIVEY =
  SDL_GPU_CUBEMAPFACE_POSITIVEY; ///< GPU_CUBEMAPFACE_POSITIVEY

constexpr GPUCubeMapFace GPU_CUBEMAPFACE_NEGATIVEY =
  SDL_GPU_CUBEMAPFACE_NEGATIVEY; ///< GPU_CUBEMAPFACE_NEGATIVEY

constexpr GPUCubeMapFace GPU_CUBEMAPFACE_POSITIVEZ =
  SDL_GPU_CUBEMAPFACE_POSITIVEZ; ///< GPU_CUBEMAPFACE_POSITIVEZ

constexpr GPUCubeMapFace GPU_CUBEMAPFACE_NEGATIVEZ =
  SDL_GPU_CUBEMAPFACE_NEGATIVEZ; ///< GPU_CUBEMAPFACE_NEGATIVEZ

/**
 * Specifies how a buffer is intended to be used by the client.
 *
 * A buffer must have at least one usage flag. Note that some usage flag
 * combinations are invalid.
 *
 * Unlike textures, READ | WRITE can be used for simultaneous read-write
 * usage. The same data synchronization concerns as textures apply.
 *
 * If you use a STORAGE flag, the data in the buffer must respect std140
 * layout conventions. In practical terms this means you must ensure that vec3
 * and vec4 fields are 16-byte aligned.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GPUBuffer.GPUBuffer
 */
using GPUBufferUsageFlags = Uint32;

constexpr GPUBufferUsageFlags GPU_BUFFERUSAGE_VERTEX =
  SDL_GPU_BUFFERUSAGE_VERTEX; ///< Buffer is a vertex buffer.

constexpr GPUBufferUsageFlags GPU_BUFFERUSAGE_INDEX =
  SDL_GPU_BUFFERUSAGE_INDEX; ///< Buffer is an index buffer.

constexpr GPUBufferUsageFlags GPU_BUFFERUSAGE_INDIRECT =
  SDL_GPU_BUFFERUSAGE_INDIRECT; ///< Buffer is an indirect buffer.

constexpr GPUBufferUsageFlags GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ =
  SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ; ///< Buffer supports storage reads
                                             ///< in graphics stages.

/// Buffer supports storage reads in the compute stage.
constexpr GPUBufferUsageFlags GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ =
  SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ;

/// Buffer supports storage writes in the compute stage.
constexpr GPUBufferUsageFlags GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE =
  SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE;

/**
 * Specifies how a transfer buffer is intended to be used by the client.
 *
 * Note that mapping and copying FROM an upload transfer buffer or TO a
 * download transfer buffer is undefined behavior.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUTransferBuffer.GPUTransferBuffer
 */
using GPUTransferBufferUsage = SDL_GPUTransferBufferUsage;

constexpr GPUTransferBufferUsage GPU_TRANSFERBUFFERUSAGE_UPLOAD =
  SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD; ///< GPU_TRANSFERBUFFERUSAGE_UPLOAD

constexpr GPUTransferBufferUsage GPU_TRANSFERBUFFERUSAGE_DOWNLOAD =
  SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD; ///< GPU_TRANSFERBUFFERUSAGE_DOWNLOAD

/**
 * Specifies which stage a shader program corresponds to.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
using GPUShaderStage = SDL_GPUShaderStage;

constexpr GPUShaderStage GPU_SHADERSTAGE_VERTEX =
  SDL_GPU_SHADERSTAGE_VERTEX; ///< GPU_SHADERSTAGE_VERTEX

constexpr GPUShaderStage GPU_SHADERSTAGE_FRAGMENT =
  SDL_GPU_SHADERSTAGE_FRAGMENT; ///< GPU_SHADERSTAGE_FRAGMENT

/**
 * Specifies the format of a vertex attribute.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUVertexElementFormat = SDL_GPUVertexElementFormat;

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_INVALID =
  SDL_GPU_VERTEXELEMENTFORMAT_INVALID; ///< GPU_VERTEXELEMENTFORMAT_INVALID

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_INT =
  SDL_GPU_VERTEXELEMENTFORMAT_INT; ///< GPU_VERTEXELEMENTFORMAT_INT

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_INT2 =
  SDL_GPU_VERTEXELEMENTFORMAT_INT2; ///< GPU_VERTEXELEMENTFORMAT_INT2

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_INT3 =
  SDL_GPU_VERTEXELEMENTFORMAT_INT3; ///< GPU_VERTEXELEMENTFORMAT_INT3

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_INT4 =
  SDL_GPU_VERTEXELEMENTFORMAT_INT4; ///< GPU_VERTEXELEMENTFORMAT_INT4

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_UINT =
  SDL_GPU_VERTEXELEMENTFORMAT_UINT; ///< GPU_VERTEXELEMENTFORMAT_UINT

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_UINT2 =
  SDL_GPU_VERTEXELEMENTFORMAT_UINT2; ///< GPU_VERTEXELEMENTFORMAT_UINT2

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_UINT3 =
  SDL_GPU_VERTEXELEMENTFORMAT_UINT3; ///< GPU_VERTEXELEMENTFORMAT_UINT3

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_UINT4 =
  SDL_GPU_VERTEXELEMENTFORMAT_UINT4; ///< GPU_VERTEXELEMENTFORMAT_UINT4

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_FLOAT =
  SDL_GPU_VERTEXELEMENTFORMAT_FLOAT; ///< GPU_VERTEXELEMENTFORMAT_FLOAT

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_FLOAT2 =
  SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2; ///< GPU_VERTEXELEMENTFORMAT_FLOAT2

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_FLOAT3 =
  SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3; ///< GPU_VERTEXELEMENTFORMAT_FLOAT3

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_FLOAT4 =
  SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4; ///< GPU_VERTEXELEMENTFORMAT_FLOAT4

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_BYTE2 =
  SDL_GPU_VERTEXELEMENTFORMAT_BYTE2; ///< GPU_VERTEXELEMENTFORMAT_BYTE2

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_BYTE4 =
  SDL_GPU_VERTEXELEMENTFORMAT_BYTE4; ///< GPU_VERTEXELEMENTFORMAT_BYTE4

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_UBYTE2 =
  SDL_GPU_VERTEXELEMENTFORMAT_UBYTE2; ///< GPU_VERTEXELEMENTFORMAT_UBYTE2

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_UBYTE4 =
  SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4; ///< GPU_VERTEXELEMENTFORMAT_UBYTE4

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_BYTE2_NORM =
  SDL_GPU_VERTEXELEMENTFORMAT_BYTE2_NORM; ///< GPU_VERTEXELEMENTFORMAT_BYTE2_NORM

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_BYTE4_NORM =
  SDL_GPU_VERTEXELEMENTFORMAT_BYTE4_NORM; ///< GPU_VERTEXELEMENTFORMAT_BYTE4_NORM

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_UBYTE2_NORM =
  SDL_GPU_VERTEXELEMENTFORMAT_UBYTE2_NORM; ///< GPU_VERTEXELEMENTFORMAT_UBYTE2_NORM

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM =
  SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM; ///< GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_SHORT2 =
  SDL_GPU_VERTEXELEMENTFORMAT_SHORT2; ///< GPU_VERTEXELEMENTFORMAT_SHORT2

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_SHORT4 =
  SDL_GPU_VERTEXELEMENTFORMAT_SHORT4; ///< GPU_VERTEXELEMENTFORMAT_SHORT4

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_USHORT2 =
  SDL_GPU_VERTEXELEMENTFORMAT_USHORT2; ///< GPU_VERTEXELEMENTFORMAT_USHORT2

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_USHORT4 =
  SDL_GPU_VERTEXELEMENTFORMAT_USHORT4; ///< GPU_VERTEXELEMENTFORMAT_USHORT4

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_SHORT2_NORM =
  SDL_GPU_VERTEXELEMENTFORMAT_SHORT2_NORM; ///< GPU_VERTEXELEMENTFORMAT_SHORT2_NORM

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_SHORT4_NORM =
  SDL_GPU_VERTEXELEMENTFORMAT_SHORT4_NORM; ///< GPU_VERTEXELEMENTFORMAT_SHORT4_NORM

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_USHORT2_NORM =
  SDL_GPU_VERTEXELEMENTFORMAT_USHORT2_NORM; ///< GPU_VERTEXELEMENTFORMAT_USHORT2_NORM

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_USHORT4_NORM =
  SDL_GPU_VERTEXELEMENTFORMAT_USHORT4_NORM; ///< GPU_VERTEXELEMENTFORMAT_USHORT4_NORM

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_HALF2 =
  SDL_GPU_VERTEXELEMENTFORMAT_HALF2; ///< GPU_VERTEXELEMENTFORMAT_HALF2

constexpr GPUVertexElementFormat GPU_VERTEXELEMENTFORMAT_HALF4 =
  SDL_GPU_VERTEXELEMENTFORMAT_HALF4; ///< GPU_VERTEXELEMENTFORMAT_HALF4

/**
 * Specifies the rate at which vertex attributes are pulled from buffers.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUVertexInputRate = SDL_GPUVertexInputRate;

/// Attribute addressing is a function of the vertex index.
constexpr GPUVertexInputRate GPU_VERTEXINPUTRATE_VERTEX =
  SDL_GPU_VERTEXINPUTRATE_VERTEX;

/// Attribute addressing is a function of the instance index.
constexpr GPUVertexInputRate GPU_VERTEXINPUTRATE_INSTANCE =
  SDL_GPU_VERTEXINPUTRATE_INSTANCE;

/**
 * Specifies the fill mode of the graphics pipeline.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUFillMode = SDL_GPUFillMode;

constexpr GPUFillMode GPU_FILLMODE_FILL =
  SDL_GPU_FILLMODE_FILL; ///< Polygons will be rendered via rasterization.

constexpr GPUFillMode GPU_FILLMODE_LINE =
  SDL_GPU_FILLMODE_LINE; ///< Polygon edges will be drawn as line segments.

/**
 * Specifies the facing direction in which triangle faces will be culled.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUCullMode = SDL_GPUCullMode;

constexpr GPUCullMode GPU_CULLMODE_NONE =
  SDL_GPU_CULLMODE_NONE; ///< No triangles are culled.

constexpr GPUCullMode GPU_CULLMODE_FRONT =
  SDL_GPU_CULLMODE_FRONT; ///< Front-facing triangles are culled.

constexpr GPUCullMode GPU_CULLMODE_BACK =
  SDL_GPU_CULLMODE_BACK; ///< Back-facing triangles are culled.

/**
 * Specifies the vertex winding that will cause a triangle to be determined to
 * be front-facing.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUFrontFace = SDL_GPUFrontFace;

/**
 * A triangle with counter-clockwise vertex winding will be considered
 * front-facing.
 */
constexpr GPUFrontFace GPU_FRONTFACE_COUNTER_CLOCKWISE =
  SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;

/// A triangle with clockwise vertex winding will be considered front-facing.
constexpr GPUFrontFace GPU_FRONTFACE_CLOCKWISE = SDL_GPU_FRONTFACE_CLOCKWISE;

/**
 * Specifies a comparison operator for depth, stencil and sampler operations.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUCompareOp = SDL_GPUCompareOp;

constexpr GPUCompareOp GPU_COMPAREOP_INVALID =
  SDL_GPU_COMPAREOP_INVALID; ///< GPU_COMPAREOP_INVALID

constexpr GPUCompareOp GPU_COMPAREOP_NEVER =
  SDL_GPU_COMPAREOP_NEVER; ///< The comparison always evaluates false.

constexpr GPUCompareOp GPU_COMPAREOP_LESS =
  SDL_GPU_COMPAREOP_LESS; ///< The comparison evaluates reference < test.

constexpr GPUCompareOp GPU_COMPAREOP_EQUAL =
  SDL_GPU_COMPAREOP_EQUAL; ///< The comparison evaluates reference == test.

constexpr GPUCompareOp GPU_COMPAREOP_LESS_OR_EQUAL =
  SDL_GPU_COMPAREOP_LESS_OR_EQUAL; ///< The comparison evaluates reference <=
                                   ///< test.

constexpr GPUCompareOp GPU_COMPAREOP_GREATER =
  SDL_GPU_COMPAREOP_GREATER; ///< The comparison evaluates reference > test.

constexpr GPUCompareOp GPU_COMPAREOP_NOT_EQUAL =
  SDL_GPU_COMPAREOP_NOT_EQUAL; ///< The comparison evaluates reference != test.

constexpr GPUCompareOp GPU_COMPAREOP_GREATER_OR_EQUAL =
  SDL_GPU_COMPAREOP_GREATER_OR_EQUAL; ///< The comparison evaluates reference >=
                                      ///< test.

constexpr GPUCompareOp GPU_COMPAREOP_ALWAYS =
  SDL_GPU_COMPAREOP_ALWAYS; ///< The comparison always evaluates true.

/**
 * Specifies what happens to a stored stencil value if stencil tests fail or
 * pass.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUStencilOp = SDL_GPUStencilOp;

constexpr GPUStencilOp GPU_STENCILOP_INVALID =
  SDL_GPU_STENCILOP_INVALID; ///< GPU_STENCILOP_INVALID

constexpr GPUStencilOp GPU_STENCILOP_KEEP =
  SDL_GPU_STENCILOP_KEEP; ///< Keeps the current value.

constexpr GPUStencilOp GPU_STENCILOP_ZERO =
  SDL_GPU_STENCILOP_ZERO; ///< Sets the value to 0.

constexpr GPUStencilOp GPU_STENCILOP_REPLACE =
  SDL_GPU_STENCILOP_REPLACE; ///< Sets the value to reference.

/// Increments the current value and clamps to the maximum value.
constexpr GPUStencilOp GPU_STENCILOP_INCREMENT_AND_CLAMP =
  SDL_GPU_STENCILOP_INCREMENT_AND_CLAMP;

constexpr GPUStencilOp GPU_STENCILOP_DECREMENT_AND_CLAMP =
  SDL_GPU_STENCILOP_DECREMENT_AND_CLAMP; ///< Decrements the current value and
                                         ///< clamps to 0.

constexpr GPUStencilOp GPU_STENCILOP_INVERT =
  SDL_GPU_STENCILOP_INVERT; ///< Bitwise-inverts the current value.

constexpr GPUStencilOp GPU_STENCILOP_INCREMENT_AND_WRAP =
  SDL_GPU_STENCILOP_INCREMENT_AND_WRAP; ///< Increments the current value and
                                        ///< wraps back to 0.

/// Decrements the current value and wraps to the maximum value.
constexpr GPUStencilOp GPU_STENCILOP_DECREMENT_AND_WRAP =
  SDL_GPU_STENCILOP_DECREMENT_AND_WRAP;

/**
 * Specifies the operator to be used when pixels in a render target are
 * blended with existing pixels in the texture.
 *
 * The source color is the value written by the fragment shader. The
 * destination color is the value currently existing in the texture.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUBlendOp = SDL_GPUBlendOp;

constexpr GPUBlendOp GPU_BLENDOP_INVALID =
  SDL_GPU_BLENDOP_INVALID; ///< GPU_BLENDOP_INVALID

///(source * source_factor) + (destination * destination_factor)
constexpr GPUBlendOp GPU_BLENDOP_ADD = SDL_GPU_BLENDOP_ADD;

///(source * source_factor) - (destination * destination_factor)
constexpr GPUBlendOp GPU_BLENDOP_SUBTRACT = SDL_GPU_BLENDOP_SUBTRACT;

///(destination * destination_factor) - (source * source_factor)
constexpr GPUBlendOp GPU_BLENDOP_REVERSE_SUBTRACT =
  SDL_GPU_BLENDOP_REVERSE_SUBTRACT;

constexpr GPUBlendOp GPU_BLENDOP_MIN =
  SDL_GPU_BLENDOP_MIN; ///< min(source, destination)

constexpr GPUBlendOp GPU_BLENDOP_MAX =
  SDL_GPU_BLENDOP_MAX; ///< max(source, destination)

/**
 * Specifies a blending factor to be used when pixels in a render target are
 * blended with existing pixels in the texture.
 *
 * The source color is the value written by the fragment shader. The
 * destination color is the value currently existing in the texture.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUBlendFactor = SDL_GPUBlendFactor;

constexpr GPUBlendFactor GPU_BLENDFACTOR_INVALID =
  SDL_GPU_BLENDFACTOR_INVALID; ///< GPU_BLENDFACTOR_INVALID

constexpr GPUBlendFactor GPU_BLENDFACTOR_ZERO = SDL_GPU_BLENDFACTOR_ZERO; ///< 0

constexpr GPUBlendFactor GPU_BLENDFACTOR_ONE = SDL_GPU_BLENDFACTOR_ONE; ///< 1

constexpr GPUBlendFactor GPU_BLENDFACTOR_SRC_COLOR =
  SDL_GPU_BLENDFACTOR_SRC_COLOR; ///< source color

constexpr GPUBlendFactor GPU_BLENDFACTOR_ONE_MINUS_SRC_COLOR =
  SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_COLOR; ///< 1 - source color

constexpr GPUBlendFactor GPU_BLENDFACTOR_DST_COLOR =
  SDL_GPU_BLENDFACTOR_DST_COLOR; ///< destination color

constexpr GPUBlendFactor GPU_BLENDFACTOR_ONE_MINUS_DST_COLOR =
  SDL_GPU_BLENDFACTOR_ONE_MINUS_DST_COLOR; ///< 1 - destination color

constexpr GPUBlendFactor GPU_BLENDFACTOR_SRC_ALPHA =
  SDL_GPU_BLENDFACTOR_SRC_ALPHA; ///< source alpha

constexpr GPUBlendFactor GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA =
  SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA; ///< 1 - source alpha

constexpr GPUBlendFactor GPU_BLENDFACTOR_DST_ALPHA =
  SDL_GPU_BLENDFACTOR_DST_ALPHA; ///< destination alpha

constexpr GPUBlendFactor GPU_BLENDFACTOR_ONE_MINUS_DST_ALPHA =
  SDL_GPU_BLENDFACTOR_ONE_MINUS_DST_ALPHA; ///< 1 - destination alpha

constexpr GPUBlendFactor GPU_BLENDFACTOR_CONSTANT_COLOR =
  SDL_GPU_BLENDFACTOR_CONSTANT_COLOR; ///< blend constant

constexpr GPUBlendFactor GPU_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR =
  SDL_GPU_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR; ///< 1 - blend constant

constexpr GPUBlendFactor GPU_BLENDFACTOR_SRC_ALPHA_SATURATE =
  SDL_GPU_BLENDFACTOR_SRC_ALPHA_SATURATE; ///< min(source alpha, 1 - destination
                                          ///< alpha)

/**
 * Specifies which color components are written in a graphics pipeline.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 */
using GPUColorComponentFlags = Uint8;

constexpr GPUColorComponentFlags GPU_COLORCOMPONENT_R =
  SDL_GPU_COLORCOMPONENT_R; ///< the red component

constexpr GPUColorComponentFlags GPU_COLORCOMPONENT_G =
  SDL_GPU_COLORCOMPONENT_G; ///< the green component

constexpr GPUColorComponentFlags GPU_COLORCOMPONENT_B =
  SDL_GPU_COLORCOMPONENT_B; ///< the blue component

constexpr GPUColorComponentFlags GPU_COLORCOMPONENT_A =
  SDL_GPU_COLORCOMPONENT_A; ///< the alpha component

/**
 * Specifies a filter operation used by a sampler.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUSampler.GPUSampler
 */
using GPUFilter = SDL_GPUFilter;

constexpr GPUFilter GPU_FILTER_NEAREST =
  SDL_GPU_FILTER_NEAREST; ///< Point filtering.

constexpr GPUFilter GPU_FILTER_LINEAR =
  SDL_GPU_FILTER_LINEAR; ///< Linear filtering.

/**
 * Specifies a mipmap mode used by a sampler.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUSampler.GPUSampler
 */
using GPUSamplerMipmapMode = SDL_GPUSamplerMipmapMode;

constexpr GPUSamplerMipmapMode GPU_SAMPLERMIPMAPMODE_NEAREST =
  SDL_GPU_SAMPLERMIPMAPMODE_NEAREST; ///< Point filtering.

constexpr GPUSamplerMipmapMode GPU_SAMPLERMIPMAPMODE_LINEAR =
  SDL_GPU_SAMPLERMIPMAPMODE_LINEAR; ///< Linear filtering.

/**
 * Specifies behavior of texture sampling when the coordinates exceed the 0-1
 * range.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GPUSampler.GPUSampler
 */
using GPUSamplerAddressMode = SDL_GPUSamplerAddressMode;

constexpr GPUSamplerAddressMode GPU_SAMPLERADDRESSMODE_REPEAT =
  SDL_GPU_SAMPLERADDRESSMODE_REPEAT; ///< Specifies that the coordinates will
                                     ///< wrap around.

/// Specifies that the coordinates will wrap around mirrored.
constexpr GPUSamplerAddressMode GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT =
  SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;

/// Specifies that the coordinates will clamp to the 0-1 range.
constexpr GPUSamplerAddressMode GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE =
  SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

/**
 * A structure specifying a region of a texture used in the blit operation.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.BlitTexture
 */
using GPUBlitRegion = SDL_GPUBlitRegion;

/**
 * A structure specifying the parameters of an indirect draw command.
 *
 * Note that the `first_vertex` and `first_instance` parameters are NOT
 * compatible with built-in vertex/instance ID variables in shaders (for
 * example, SV_VertexID); GPU APIs and shader languages do not define these
 * built-in variables consistently, so if your shader depends on them, the
 * only way to keep behavior consistent and portable is to always pass 0 for
 * the correlating parameter in the draw calls.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPURenderPass.DrawPrimitivesIndirect
 */
using GPUIndirectDrawCommand = SDL_GPUIndirectDrawCommand;

/**
 * A structure specifying the parameters of an indexed indirect draw command.
 *
 * Note that the `first_vertex` and `first_instance` parameters are NOT
 * compatible with built-in vertex/instance ID variables in shaders (for
 * example, SV_VertexID); GPU APIs and shader languages do not define these
 * built-in variables consistently, so if your shader depends on them, the
 * only way to keep behavior consistent and portable is to always pass 0 for
 * the correlating parameter in the draw calls.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPURenderPass.DrawIndexedPrimitivesIndirect
 */
using GPUIndexedIndirectDrawCommand = SDL_GPUIndexedIndirectDrawCommand;

/**
 * A structure specifying the parameters of an indexed dispatch command.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUComputePass.DispatchIndirect
 */
using GPUIndirectDispatchCommand = SDL_GPUIndirectDispatchCommand;

/**
 * A structure specifying the parameters of vertex buffers used in a graphics
 * pipeline.
 *
 * When you call GPURenderPass.BindVertexBuffers, you specify the binding slots
 * of the vertex buffers. For example if you called
 * GPURenderPass.BindVertexBuffers with a first_slot of 2 and num_bindings of 3,
 * the binding slots 2, 3, 4 would be used by the vertex buffers you pass in.
 *
 * Vertex attributes are linked to buffers via the buffer_slot field of
 * GPUVertexAttribute. For example, if an attribute has a buffer_slot of
 * 0, then that attribute belongs to the vertex buffer bound at slot 0.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUVertexAttribute
 * @sa GPUVertexInputRate
 */
using GPUVertexBufferDescription = SDL_GPUVertexBufferDescription;

/**
 * A structure specifying a vertex attribute.
 *
 * All vertex attribute locations provided to an GPUVertexInputState must
 * be unique.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUVertexBufferDescription
 * @sa GPUVertexInputState
 * @sa GPUVertexElementFormat
 */
using GPUVertexAttribute = SDL_GPUVertexAttribute;

/**
 * A structure specifying the parameters of a graphics pipeline vertex input
 * state.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipelineCreateInfo
 * @sa GPUVertexBufferDescription
 * @sa GPUVertexAttribute
 */
using GPUVertexInputState = SDL_GPUVertexInputState;

/**
 * A structure specifying the stencil operation state of a graphics pipeline.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUDepthStencilState
 */
using GPUStencilOpState = SDL_GPUStencilOpState;

/**
 * A structure specifying the blend state of a color target.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUColorTargetDescription
 */
using GPUColorTargetBlendState = SDL_GPUColorTargetBlendState;

/**
 * A structure specifying the parameters of the graphics pipeline rasterizer
 * state.
 *
 * Note that GPU_FILLMODE_LINE is not supported on many Android devices.
 * For those devices, the fill mode will automatically fall back to FILL.
 *
 * Also note that the D3D12 driver will enable depth clamping even if
 * enable_depth_clip is true. If you need this clamp+clip behavior, consider
 * enabling depth clip and then manually clamping depth in your fragment
 * shaders on Metal and Vulkan.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipelineCreateInfo
 */
using GPURasterizerState = SDL_GPURasterizerState;

/**
 * A structure specifying the parameters of the graphics pipeline multisample
 * state.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipelineCreateInfo
 */
using GPUMultisampleState = SDL_GPUMultisampleState;

/**
 * A structure specifying the parameters of the graphics pipeline depth
 * stencil state.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipelineCreateInfo
 */
using GPUDepthStencilState = SDL_GPUDepthStencilState;

/**
 * A structure specifying the parameters of color targets used in a graphics
 * pipeline.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipelineTargetInfo
 */
using GPUColorTargetDescription = SDL_GPUColorTargetDescription;

/**
 * A structure specifying the descriptions of render targets used in a
 * graphics pipeline.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipelineCreateInfo
 * @sa GPUColorTargetDescription
 * @sa GPUTextureFormat
 */
using GPUGraphicsPipelineTargetInfo = SDL_GPUGraphicsPipelineTargetInfo;

/**
 * Checks for GPU runtime support.
 *
 * @param format_flags a bitflag indicating which shader formats the app is
 *                     able to provide.
 * @param name the preferred GPU driver, or nullptr to let SDL pick the optimal
 *             driver.
 * @returns true if supported, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.GPUDevice
 */
inline bool GPUSupportsShaderFormats(GPUShaderFormat format_flags,
                                     StringParam name)
{
  return SDL_GPUSupportsShaderFormats(format_flags, name);
}

/**
 * Checks for GPU runtime support.
 *
 * @param props the properties to use.
 * @returns true if supported, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.GPUDevice
 */
inline bool GPUSupportsProperties(PropertiesParam props)
{
  return SDL_GPUSupportsProperties(props);
}

/**
 * Creates a GPU context.
 *
 * @param format_flags a bitflag indicating which shader formats the app is
 *                     able to provide.
 * @param debug_mode enable debug mode properties and validations.
 * @param name the preferred GPU driver, or nullptr to let SDL pick the optimal
 *             driver.
 * @returns a GPU context on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.GetShaderFormats
 * @sa GPUDevice.GetDriver
 * @sa GPUDevice.Destroy
 * @sa GPUSupportsShaderFormats
 */
inline GPUDevice CreateGPUDevice(GPUShaderFormat format_flags,
                                 bool debug_mode,
                                 StringParam name)
{
  return GPUDevice(format_flags, debug_mode, std::move(name));
}

/**
 * Creates a GPU context.
 *
 * These are the supported properties:
 *
 * - `prop::GpuDevice.CREATE_DEBUGMODE_BOOLEAN`: enable debug mode
 *   properties and validations, defaults to true.
 * - `prop::GpuDevice.CREATE_PREFERLOWPOWER_BOOLEAN`: enable to prefer
 *   energy efficiency over maximum GPU performance, defaults to false.
 * - `prop::GpuDevice.CREATE_NAME_STRING`: the name of the GPU driver to
 *   use, if a specific one is desired.
 *
 * These are the current shader format properties:
 *
 * - `prop::GpuDevice.CREATE_SHADERS_PRIVATE_BOOLEAN`: The app is able to
 *   provide shaders for an NDA platform.
 * - `prop::GpuDevice.CREATE_SHADERS_SPIRV_BOOLEAN`: The app is able to
 *   provide SPIR-V shaders if applicable.
 * - `prop::GpuDevice.CREATE_SHADERS_DXBC_BOOLEAN`: The app is able to
 *   provide DXBC shaders if applicable
 * - `prop::GpuDevice.CREATE_SHADERS_DXIL_BOOLEAN`: The app is able to
 *   provide DXIL shaders if applicable.
 * - `prop::GpuDevice.CREATE_SHADERS_MSL_BOOLEAN`: The app is able to
 *   provide MSL shaders if applicable.
 * - `prop::GpuDevice.CREATE_SHADERS_METALLIB_BOOLEAN`: The app is able to
 *   provide Metal shader libraries if applicable.
 *
 * With the D3D12 renderer:
 *
 * - `prop::GpuDevice.CREATE_D3D12_SEMANTIC_NAME_STRING`: the prefix to
 *   use for all vertex semantics, default is "TEXCOORD".
 *
 * @param props the properties to use.
 * @returns a GPU context on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.GetShaderFormats
 * @sa GPUDevice.GetDriver
 * @sa GPUDevice.Destroy
 * @sa GPUSupportsProperties
 */
inline GPUDevice CreateGPUDeviceWithProperties(PropertiesParam props)
{
  return GPUDevice(props);
}

namespace prop::GpuDevice {

constexpr auto CREATE_DEBUGMODE_BOOLEAN =
  SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN;

constexpr auto CREATE_PREFERLOWPOWER_BOOLEAN =
  SDL_PROP_GPU_DEVICE_CREATE_PREFERLOWPOWER_BOOLEAN;

constexpr auto CREATE_NAME_STRING = SDL_PROP_GPU_DEVICE_CREATE_NAME_STRING;

constexpr auto CREATE_SHADERS_PRIVATE_BOOLEAN =
  SDL_PROP_GPU_DEVICE_CREATE_SHADERS_PRIVATE_BOOLEAN;

constexpr auto CREATE_SHADERS_SPIRV_BOOLEAN =
  SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN;

constexpr auto CREATE_SHADERS_DXBC_BOOLEAN =
  SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXBC_BOOLEAN;

constexpr auto CREATE_SHADERS_DXIL_BOOLEAN =
  SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN;

constexpr auto CREATE_SHADERS_MSL_BOOLEAN =
  SDL_PROP_GPU_DEVICE_CREATE_SHADERS_MSL_BOOLEAN;

constexpr auto CREATE_SHADERS_METALLIB_BOOLEAN =
  SDL_PROP_GPU_DEVICE_CREATE_SHADERS_METALLIB_BOOLEAN;

constexpr auto CREATE_D3D12_SEMANTIC_NAME_STRING =
  SDL_PROP_GPU_DEVICE_CREATE_D3D12_SEMANTIC_NAME_STRING;

} // namespace prop::GpuDevice

/**
 * Destroys a GPU context previously returned by GPUDevice.GPUDevice.
 *
 * @param device a GPU Context to destroy.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.GPUDevice
 */
inline void DestroyGPUDevice(GPUDeviceRaw device)
{
  SDL_DestroyGPUDevice(device);
}

inline void GPUDevice::Destroy() { DestroyGPUDevice(release()); }

/**
 * Get the number of GPU drivers compiled into SDL.
 *
 * @returns the number of built in GPU drivers.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGPUDriver
 */
inline int GetNumGPUDrivers() { return SDL_GetNumGPUDrivers(); }

/**
 * Get the name of a built in GPU driver.
 *
 * The GPU drivers are presented in the order in which they are normally
 * checked during initialization.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "vulkan",
 * "metal" or "direct3d12". These never have Unicode characters, and are not
 * meant to be proper names.
 *
 * @param index the index of a GPU driver.
 * @returns the name of the GPU driver with the given **index**.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumGPUDrivers
 */
inline const char* GetGPUDriver(int index) { return SDL_GetGPUDriver(index); }

/**
 * Returns the name of the backend used to create this GPU context.
 *
 * @param device a GPU context to query.
 * @returns the name of the device's driver, or nullptr on error.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetGPUDeviceDriver(GPUDeviceParam device)
{
  return SDL_GetGPUDeviceDriver(device);
}

inline const char* GPUDevice::GetDriver()
{
  return SDL::GetGPUDeviceDriver(m_resource);
}

/**
 * Returns the supported shader formats for this GPU context.
 *
 * @param device a GPU context to query.
 * @returns a bitflag indicating which shader formats the driver is able to
 *          consume.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline GPUShaderFormat GetGPUShaderFormats(GPUDeviceParam device)
{
  return SDL_GetGPUShaderFormats(device);
}

inline GPUShaderFormat GPUDevice::GetShaderFormats()
{
  return SDL::GetGPUShaderFormats(m_resource);
}

inline GPUComputePipeline GPUDevice::CreateComputePipeline(
  const GPUComputePipelineCreateInfo& createinfo)
{
  return GPUComputePipeline(m_resource, createinfo);
}

/**
 * Creates a pipeline object to be used in a compute workflow.
 *
 * Shader resource bindings must be authored to follow a particular order
 * depending on the shader format.
 *
 * For SPIR-V shaders, use the following resource sets:
 *
 * - 0: Sampled textures, followed by read-only storage textures, followed by
 *   read-only storage buffers
 * - 1: Read-write storage textures, followed by read-write storage buffers
 * - 2: Uniform buffers
 *
 * For DXBC and DXIL shaders, use the following register order:
 *
 * - (t[n], space0): Sampled textures, followed by read-only storage textures,
 *   followed by read-only storage buffers
 * - (u[n], space1): Read-write storage textures, followed by read-write
 *   storage buffers
 * - (b[n], space2): Uniform buffers
 *
 * For MSL/metallib, use the following order:
 *
 * - [[buffer]]: Uniform buffers, followed by read-only storage buffers,
 *   followed by read-write storage buffers
 * - [[texture]]: Sampled textures, followed by read-only storage textures,
 *   followed by read-write storage textures
 *
 * There are optional properties that can be provided through `props`. These
 * are the supported properties:
 *
 * - `prop::GPUComputePipeline.CREATE_NAME_STRING`: a name that can be
 *   displayed in debugging tools.
 *
 * @param device a GPU Context.
 * @param createinfo a struct describing the state of the compute pipeline to
 *                   create.
 * @returns a compute pipeline object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUComputePass.BindPipeline
 * @sa GPUDevice.ReleaseComputePipeline
 */
inline GPUComputePipeline CreateGPUComputePipeline(
  GPUDeviceParam device,
  const GPUComputePipelineCreateInfo& createinfo)
{
  return GPUComputePipeline(device, createinfo);
}

namespace prop::GPUComputePipeline {

constexpr auto CREATE_NAME_STRING =
  SDL_PROP_GPU_COMPUTEPIPELINE_CREATE_NAME_STRING;

} // namespace prop::GPUComputePipeline

inline GPUGraphicsPipeline GPUDevice::CreateGraphicsPipeline(
  const GPUGraphicsPipelineCreateInfo& createinfo)
{
  return GPUGraphicsPipeline(m_resource, createinfo);
}

/**
 * Creates a pipeline object to be used in a graphics workflow.
 *
 * There are optional properties that can be provided through `props`. These
 * are the supported properties:
 *
 * - `prop::GPUGraphicsPipeline.CREATE_NAME_STRING`: a name that can be
 *   displayed in debugging tools.
 *
 * @param device a GPU Context.
 * @param createinfo a struct describing the state of the graphics pipeline to
 *                   create.
 * @returns a graphics pipeline object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 * @sa GPURenderPass.BindPipeline
 * @sa GPUDevice.ReleaseGraphicsPipeline
 */
inline GPUGraphicsPipeline CreateGPUGraphicsPipeline(
  GPUDeviceParam device,
  const GPUGraphicsPipelineCreateInfo& createinfo)
{
  return GPUGraphicsPipeline(device, createinfo);
}

namespace prop::GPUGraphicsPipeline {

constexpr auto CREATE_NAME_STRING =
  SDL_PROP_GPU_GRAPHICSPIPELINE_CREATE_NAME_STRING;

} // namespace prop::GPUGraphicsPipeline

inline GPUSampler GPUDevice::CreateSampler(
  const GPUSamplerCreateInfo& createinfo)
{
  return GPUSampler(m_resource, createinfo);
}

/**
 * Creates a sampler object to be used when binding textures in a graphics
 * workflow.
 *
 * There are optional properties that can be provided through `props`. These
 * are the supported properties:
 *
 * - `prop::GPUSampler.CREATE_NAME_STRING`: a name that can be displayed
 *   in debugging tools.
 *
 * @param device a GPU Context.
 * @param createinfo a struct describing the state of the sampler to create.
 * @returns a sampler object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPURenderPass.BindVertexSamplers
 * @sa GPURenderPass.BindFragmentSamplers
 * @sa GPUDevice.ReleaseSampler
 */
inline GPUSampler CreateGPUSampler(GPUDeviceParam device,
                                   const GPUSamplerCreateInfo& createinfo)
{
  return GPUSampler(device, createinfo);
}

namespace prop::GPUSampler {

constexpr auto CREATE_NAME_STRING = SDL_PROP_GPU_SAMPLER_CREATE_NAME_STRING;

} // namespace prop::GPUSampler

inline GPUShader GPUDevice::CreateShader(const GPUShaderCreateInfo& createinfo)
{
  return GPUShader(m_resource, createinfo);
}

/**
 * Creates a shader to be used when creating a graphics pipeline.
 *
 * Shader resource bindings must be authored to follow a particular order
 * depending on the shader format.
 *
 * For SPIR-V shaders, use the following resource sets:
 *
 * For vertex shaders:
 *
 * - 0: Sampled textures, followed by storage textures, followed by storage
 *   buffers
 * - 1: Uniform buffers
 *
 * For fragment shaders:
 *
 * - 2: Sampled textures, followed by storage textures, followed by storage
 *   buffers
 * - 3: Uniform buffers
 *
 * For DXBC and DXIL shaders, use the following register order:
 *
 * For vertex shaders:
 *
 * - (t[n], space0): Sampled textures, followed by storage textures, followed
 *   by storage buffers
 * - (s[n], space0): Samplers with indices corresponding to the sampled
 *   textures
 * - (b[n], space1): Uniform buffers
 *
 * For pixel shaders:
 *
 * - (t[n], space2): Sampled textures, followed by storage textures, followed
 *   by storage buffers
 * - (s[n], space2): Samplers with indices corresponding to the sampled
 *   textures
 * - (b[n], space3): Uniform buffers
 *
 * For MSL/metallib, use the following order:
 *
 * - [[texture]]: Sampled textures, followed by storage textures
 * - [[sampler]]: Samplers with indices corresponding to the sampled textures
 * - [[buffer]]: Uniform buffers, followed by storage buffers. Vertex buffer 0
 *   is bound at [[buffer(14)]], vertex buffer 1 at [[buffer(15)]], and so on.
 *   Rather than manually authoring vertex buffer indices, use the
 *   [[stage_in]] attribute which will automatically use the vertex input
 *   information from the GPUGraphicsPipeline.
 *
 * Shader semantics other than system-value semantics do not matter in D3D12
 * and for ease of use the SDL implementation assumes that non system-value
 * semantics will all be TEXCOORD. If you are using HLSL as the shader source
 * language, your vertex semantics should start at TEXCOORD0 and increment
 * like so: TEXCOORD1, TEXCOORD2, etc. If you wish to change the semantic
 * prefix to something other than TEXCOORD you can use
 * prop::GpuDevice.CREATE_D3D12_SEMANTIC_NAME_STRING with
 * GPUDevice.GPUDevice().
 *
 * There are optional properties that can be provided through `props`. These
 * are the supported properties:
 *
 * - `prop::GPUShader.CREATE_NAME_STRING`: a name that can be displayed in
 *   debugging tools.
 *
 * @param device a GPU Context.
 * @param createinfo a struct describing the state of the shader to create.
 * @returns a shader object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUGraphicsPipeline.GPUGraphicsPipeline
 * @sa GPUDevice.ReleaseShader
 */
inline GPUShader CreateGPUShader(GPUDeviceParam device,
                                 const GPUShaderCreateInfo& createinfo)
{
  return GPUShader(device, createinfo);
}

namespace prop::GPUShader {

constexpr auto CREATE_NAME_STRING = SDL_PROP_GPU_SHADER_CREATE_NAME_STRING;

} // namespace prop::GPUShader

inline GPUTexture GPUDevice::CreateTexture(
  const GPUTextureCreateInfo& createinfo)
{
  return GPUTexture(m_resource, createinfo);
}

/**
 * Creates a texture object to be used in graphics or compute workflows.
 *
 * The contents of this texture are undefined until data is written to the
 * texture.
 *
 * Note that certain combinations of usage flags are invalid. For example, a
 * texture cannot have both the SAMPLER and GRAPHICS_STORAGE_READ flags.
 *
 * If you request a sample count higher than the hardware supports, the
 * implementation will automatically fall back to the highest available sample
 * count.
 *
 * There are optional properties that can be provided through
 * GPUTextureCreateInfo's `props`. These are the supported properties:
 *
 * - `prop::GPUTexture.CREATE_D3D12_CLEAR_R_FLOAT`: (Direct3D 12 only) if
 *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
 *   to a color with this red intensity. Defaults to zero.
 * - `prop::GPUTexture.CREATE_D3D12_CLEAR_G_FLOAT`: (Direct3D 12 only) if
 *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
 *   to a color with this green intensity. Defaults to zero.
 * - `prop::GPUTexture.CREATE_D3D12_CLEAR_B_FLOAT`: (Direct3D 12 only) if
 *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
 *   to a color with this blue intensity. Defaults to zero.
 * - `prop::GPUTexture.CREATE_D3D12_CLEAR_A_FLOAT`: (Direct3D 12 only) if
 *   the texture usage is GPU_TEXTUREUSAGE_COLOR_TARGET, clear the texture
 *   to a color with this alpha intensity. Defaults to zero.
 * - `prop::GPUTexture.CREATE_D3D12_CLEAR_DEPTH_FLOAT`: (Direct3D 12 only)
 *   if the texture usage is GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET, clear
 *   the texture to a depth of this value. Defaults to zero.
 * - `prop::GPUTexture.CREATE_D3D12_CLEAR_STENCIL_NUMBER`: (Direct3D 12
 *   only) if the texture usage is GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
 *   clear the texture to a stencil of this Uint8 value. Defaults to zero.
 * - `prop::GPUTexture.CREATE_NAME_STRING`: a name that can be displayed
 *   in debugging tools.
 *
 * @param device a GPU Context.
 * @param createinfo a struct describing the state of the texture to create.
 * @returns a texture object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.UploadToTexture
 * @sa GPUCopyPass.DownloadFromTexture
 * @sa GPURenderPass.BindVertexSamplers
 * @sa GPURenderPass.BindVertexStorageTextures
 * @sa GPURenderPass.BindFragmentSamplers
 * @sa GPURenderPass.BindFragmentStorageTextures
 * @sa GPUComputePass.BindStorageTextures
 * @sa GPUCommandBuffer.BlitTexture
 * @sa GPUDevice.ReleaseTexture
 * @sa GPUDevice.TextureSupportsFormat
 */
inline GPUTexture CreateGPUTexture(GPUDeviceParam device,
                                   const GPUTextureCreateInfo& createinfo)
{
  return GPUTexture(device, createinfo);
}

namespace prop::GPUTexture {

constexpr auto CREATE_D3D12_CLEAR_R_FLOAT =
  SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_R_FLOAT;

constexpr auto CREATE_D3D12_CLEAR_G_FLOAT =
  SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_G_FLOAT;

constexpr auto CREATE_D3D12_CLEAR_B_FLOAT =
  SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_B_FLOAT;

constexpr auto CREATE_D3D12_CLEAR_A_FLOAT =
  SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_A_FLOAT;

constexpr auto CREATE_D3D12_CLEAR_DEPTH_FLOAT =
  SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_DEPTH_FLOAT;

constexpr auto CREATE_D3D12_CLEAR_STENCIL_NUMBER =
  SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_STENCIL_NUMBER;

constexpr auto CREATE_NAME_STRING = SDL_PROP_GPU_TEXTURE_CREATE_NAME_STRING;

} // namespace prop::GPUTexture

inline GPUBuffer GPUDevice::CreateBuffer(const GPUBufferCreateInfo& createinfo)
{
  return GPUBuffer(m_resource, createinfo);
}

/**
 * Creates a buffer object to be used in graphics or compute workflows.
 *
 * The contents of this buffer are undefined until data is written to the
 * buffer.
 *
 * Note that certain combinations of usage flags are invalid. For example, a
 * buffer cannot have both the VERTEX and INDEX flags.
 *
 * If you use a STORAGE flag, the data in the buffer must respect std140
 * layout conventions. In practical terms this means you must ensure that vec3
 * and vec4 fields are 16-byte aligned.
 *
 * For better understanding of underlying concepts and memory management with
 * SDL GPU API, you may refer
 * [this blog post](https://moonside.games/posts/sdl-gpu-concepts-cycling/).
 *
 * There are optional properties that can be provided through `props`. These
 * are the supported properties:
 *
 * - `prop::GPUBuffer.CREATE_NAME_STRING`: a name that can be displayed in
 *   debugging tools.
 *
 * @param device a GPU Context.
 * @param createinfo a struct describing the state of the buffer to create.
 * @returns a buffer object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.UploadToBuffer
 * @sa GPUCopyPass.DownloadFromBuffer
 * @sa GPUCopyPass.CopyBufferToBuffer
 * @sa GPURenderPass.BindVertexBuffers
 * @sa GPURenderPass.BindIndexBuffer
 * @sa GPURenderPass.BindVertexStorageBuffers
 * @sa GPURenderPass.BindFragmentStorageBuffers
 * @sa GPURenderPass.DrawPrimitivesIndirect
 * @sa GPURenderPass.DrawIndexedPrimitivesIndirect
 * @sa GPUComputePass.BindStorageBuffers
 * @sa GPUComputePass.DispatchIndirect
 * @sa GPUDevice.ReleaseBuffer
 */
inline GPUBuffer CreateGPUBuffer(GPUDeviceParam device,
                                 const GPUBufferCreateInfo& createinfo)
{
  return GPUBuffer(device, createinfo);
}

namespace prop::GPUBuffer {

constexpr auto CREATE_NAME_STRING = SDL_PROP_GPU_BUFFER_CREATE_NAME_STRING;

} // namespace prop::GPUBuffer

inline GPUTransferBuffer GPUDevice::CreateTransferBuffer(
  const GPUTransferBufferCreateInfo& createinfo)
{
  return GPUTransferBuffer(m_resource, createinfo);
}

/**
 * Creates a transfer buffer to be used when uploading to or downloading from
 * graphics resources.
 *
 * Download buffers can be particularly expensive to create, so it is good
 * practice to reuse them if data will be downloaded regularly.
 *
 * There are optional properties that can be provided through `props`. These
 * are the supported properties:
 *
 * - `prop::GPUTransferBuffer.CREATE_NAME_STRING`: a name that can be
 *   displayed in debugging tools.
 *
 * @param device a GPU Context.
 * @param createinfo a struct describing the state of the transfer buffer to
 *                   create.
 * @returns a transfer buffer on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.UploadToBuffer
 * @sa GPUCopyPass.DownloadFromBuffer
 * @sa GPUCopyPass.UploadToTexture
 * @sa GPUCopyPass.DownloadFromTexture
 * @sa GPUDevice.ReleaseTransferBuffer
 */
inline GPUTransferBuffer CreateGPUTransferBuffer(
  GPUDeviceParam device,
  const GPUTransferBufferCreateInfo& createinfo)
{
  return GPUTransferBuffer(device, createinfo);
}

namespace prop::GPUTransferBuffer {

constexpr auto CREATE_NAME_STRING =
  SDL_PROP_GPU_TRANSFERBUFFER_CREATE_NAME_STRING;

} // namespace prop::GPUTransferBuffer

/**
 * Sets an arbitrary string constant to label a buffer.
 *
 * You should use prop::GPUBuffer.CREATE_NAME_STRING with
 * GPUBuffer.GPUBuffer instead of this function to avoid thread safety issues.
 *
 * @param device a GPU Context.
 * @param buffer a buffer to attach the name to.
 * @param text a UTF-8 string constant to mark as the name of the buffer.
 *
 * @threadsafety This function is not thread safe, you must make sure the
 *               buffer is not simultaneously used by any other thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUBuffer.GPUBuffer
 */
inline void SetGPUBufferName(GPUDeviceParam device,
                             GPUBuffer buffer,
                             StringParam text)
{
  SDL_SetGPUBufferName(device, buffer, text);
}

inline void GPUDevice::SetBufferName(GPUBuffer buffer, StringParam text)
{
  SDL::SetGPUBufferName(m_resource, buffer, std::move(text));
}

/**
 * Sets an arbitrary string constant to label a texture.
 *
 * You should use prop::GPUTexture.CREATE_NAME_STRING with
 * GPUTexture.GPUTexture instead of this function to avoid thread safety
 * issues.
 *
 * @param device a GPU Context.
 * @param texture a texture to attach the name to.
 * @param text a UTF-8 string constant to mark as the name of the texture.
 *
 * @threadsafety This function is not thread safe, you must make sure the
 *               texture is not simultaneously used by any other thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUTexture.GPUTexture
 */
inline void SetGPUTextureName(GPUDeviceParam device,
                              GPUTexture texture,
                              StringParam text)
{
  SDL_SetGPUTextureName(device, texture, text);
}

inline void GPUDevice::SetTextureName(GPUTexture texture, StringParam text)
{
  SDL::SetGPUTextureName(m_resource, texture, std::move(text));
}

/**
 * Inserts an arbitrary string label into the command buffer callstream.
 *
 * Useful for debugging.
 *
 * @param command_buffer a command buffer.
 * @param text a UTF-8 string constant to insert as the label.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void InsertGPUDebugLabel(GPUCommandBuffer command_buffer,
                                StringParam text)
{
  SDL_InsertGPUDebugLabel(command_buffer, text);
}

inline void GPUCommandBuffer::InsertDebugLabel(StringParam text)
{
  SDL::InsertGPUDebugLabel(m_gPUCommandBuffer, std::move(text));
}

/**
 * Begins a debug group with an arbitrary name.
 *
 * Used for denoting groups of calls when viewing the command buffer
 * callstream in a graphics debugging tool.
 *
 * Each call to GPUCommandBuffer.PushDebugGroup must have a corresponding call
 * to GPUCommandBuffer.PopDebugGroup.
 *
 * On some backends (e.g. Metal), pushing a debug group during a
 * render/blit/compute pass will create a group that is scoped to the native
 * pass rather than the command buffer. For best results, if you push a debug
 * group during a pass, always pop it in the same pass.
 *
 * @param command_buffer a command buffer.
 * @param name a UTF-8 string constant that names the group.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.PopDebugGroup
 */
inline void PushGPUDebugGroup(GPUCommandBuffer command_buffer, StringParam name)
{
  SDL_PushGPUDebugGroup(command_buffer, name);
}

inline void GPUCommandBuffer::PushDebugGroup(StringParam name)
{
  SDL::PushGPUDebugGroup(m_gPUCommandBuffer, std::move(name));
}

/**
 * Ends the most-recently pushed debug group.
 *
 * @param command_buffer a command buffer.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.PushDebugGroup
 */
inline void PopGPUDebugGroup(GPUCommandBuffer command_buffer)
{
  SDL_PopGPUDebugGroup(command_buffer);
}

inline void GPUCommandBuffer::PopDebugGroup()
{
  SDL::PopGPUDebugGroup(m_gPUCommandBuffer);
}

/**
 * Frees the given texture as soon as it is safe to do so.
 *
 * You must not reference the texture after calling this function.
 *
 * @param device a GPU context.
 * @param texture a texture to be destroyed.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReleaseGPUTexture(GPUDeviceParam device, GPUTexture texture)
{
  SDL_ReleaseGPUTexture(device, texture);
}

inline void GPUDevice::ReleaseTexture(GPUTexture texture)
{
  SDL::ReleaseGPUTexture(m_resource, texture);
}

/**
 * Frees the given sampler as soon as it is safe to do so.
 *
 * You must not reference the sampler after calling this function.
 *
 * @param device a GPU context.
 * @param sampler a sampler to be destroyed.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReleaseGPUSampler(GPUDeviceParam device, GPUSampler sampler)
{
  SDL_ReleaseGPUSampler(device, sampler);
}

inline void GPUDevice::ReleaseSampler(GPUSampler sampler)
{
  SDL::ReleaseGPUSampler(m_resource, sampler);
}

/**
 * Frees the given buffer as soon as it is safe to do so.
 *
 * You must not reference the buffer after calling this function.
 *
 * @param device a GPU context.
 * @param buffer a buffer to be destroyed.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReleaseGPUBuffer(GPUDeviceParam device, GPUBuffer buffer)
{
  SDL_ReleaseGPUBuffer(device, buffer);
}

inline void GPUDevice::ReleaseBuffer(GPUBuffer buffer)
{
  SDL::ReleaseGPUBuffer(m_resource, buffer);
}

/**
 * Frees the given transfer buffer as soon as it is safe to do so.
 *
 * You must not reference the transfer buffer after calling this function.
 *
 * @param device a GPU context.
 * @param transfer_buffer a transfer buffer to be destroyed.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReleaseGPUTransferBuffer(GPUDeviceParam device,
                                     GPUTransferBuffer transfer_buffer)
{
  SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

inline void GPUDevice::ReleaseTransferBuffer(GPUTransferBuffer transfer_buffer)
{
  SDL::ReleaseGPUTransferBuffer(m_resource, transfer_buffer);
}

/**
 * Frees the given compute pipeline as soon as it is safe to do so.
 *
 * You must not reference the compute pipeline after calling this function.
 *
 * @param device a GPU context.
 * @param compute_pipeline a compute pipeline to be destroyed.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReleaseGPUComputePipeline(GPUDeviceParam device,
                                      GPUComputePipeline compute_pipeline)
{
  SDL_ReleaseGPUComputePipeline(device, compute_pipeline);
}

inline void GPUDevice::ReleaseComputePipeline(
  GPUComputePipeline compute_pipeline)
{
  SDL::ReleaseGPUComputePipeline(m_resource, compute_pipeline);
}

/**
 * Frees the given shader as soon as it is safe to do so.
 *
 * You must not reference the shader after calling this function.
 *
 * @param device a GPU context.
 * @param shader a shader to be destroyed.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReleaseGPUShader(GPUDeviceParam device, GPUShader shader)
{
  SDL_ReleaseGPUShader(device, shader);
}

inline void GPUDevice::ReleaseShader(GPUShader shader)
{
  SDL::ReleaseGPUShader(m_resource, shader);
}

/**
 * Frees the given graphics pipeline as soon as it is safe to do so.
 *
 * You must not reference the graphics pipeline after calling this function.
 *
 * @param device a GPU context.
 * @param graphics_pipeline a graphics pipeline to be destroyed.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReleaseGPUGraphicsPipeline(GPUDeviceParam device,
                                       GPUGraphicsPipeline graphics_pipeline)
{
  SDL_ReleaseGPUGraphicsPipeline(device, graphics_pipeline);
}

inline void GPUDevice::ReleaseGraphicsPipeline(
  GPUGraphicsPipeline graphics_pipeline)
{
  SDL::ReleaseGPUGraphicsPipeline(m_resource, graphics_pipeline);
}

/**
 * Acquire a command buffer.
 *
 * This command buffer is managed by the implementation and should not be
 * freed by the user. The command buffer may only be used on the thread it was
 * acquired on. The command buffer should be submitted on the thread it was
 * acquired on.
 *
 * It is valid to acquire multiple command buffers on the same thread at once.
 * In fact a common design pattern is to acquire two command buffers per frame
 * where one is dedicated to render and compute passes and the other is
 * dedicated to copy passes and other preparatory work such as generating
 * mipmaps. Interleaving commands between the two command buffers reduces the
 * total amount of passes overall which improves rendering performance.
 *
 * @param device a GPU context.
 * @returns a command buffer, or nullptr on failure; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.Submit
 * @sa GPUCommandBuffer.SubmitAndAcquireFence
 */
inline GPUCommandBuffer AcquireGPUCommandBuffer(GPUDeviceParam device)
{
  return SDL_AcquireGPUCommandBuffer(device);
}

inline GPUCommandBuffer GPUDevice::AcquireCommandBuffer()
{
  return SDL::AcquireGPUCommandBuffer(m_resource);
}

/**
 * Pushes data to a vertex uniform slot on the command buffer.
 *
 * Subsequent draw calls will use this uniform data.
 *
 * The data being pushed must respect std140 layout conventions. In practical
 * terms this means you must ensure that vec3 and vec4 fields are 16-byte
 * aligned.
 *
 * @param command_buffer a command buffer.
 * @param slot_index the vertex uniform slot to push data to.
 * @param data client data to write.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void PushGPUVertexUniformData(GPUCommandBuffer command_buffer,
                                     Uint32 slot_index,
                                     SourceBytes data)
{
  SDL_PushGPUVertexUniformData(
    command_buffer, slot_index, data.data(), data.size_bytes());
}

inline void GPUCommandBuffer::PushVertexUniformData(Uint32 slot_index,
                                                    SourceBytes data)
{
  SDL::PushGPUVertexUniformData(
    m_gPUCommandBuffer, slot_index, std::move(data));
}

/**
 * Pushes data to a fragment uniform slot on the command buffer.
 *
 * Subsequent draw calls will use this uniform data.
 *
 * The data being pushed must respect std140 layout conventions. In practical
 * terms this means you must ensure that vec3 and vec4 fields are 16-byte
 * aligned.
 *
 * @param command_buffer a command buffer.
 * @param slot_index the fragment uniform slot to push data to.
 * @param data client data to write.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void PushGPUFragmentUniformData(GPUCommandBuffer command_buffer,
                                       Uint32 slot_index,
                                       SourceBytes data)
{
  SDL_PushGPUFragmentUniformData(
    command_buffer, slot_index, data.data(), data.size_bytes());
}

inline void GPUCommandBuffer::PushFragmentUniformData(Uint32 slot_index,
                                                      SourceBytes data)
{
  SDL::PushGPUFragmentUniformData(
    m_gPUCommandBuffer, slot_index, std::move(data));
}

/**
 * Pushes data to a uniform slot on the command buffer.
 *
 * Subsequent draw calls will use this uniform data.
 *
 * The data being pushed must respect std140 layout conventions. In practical
 * terms this means you must ensure that vec3 and vec4 fields are 16-byte
 * aligned.
 *
 * @param command_buffer a command buffer.
 * @param slot_index the uniform slot to push data to.
 * @param data client data to write.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void PushGPUComputeUniformData(GPUCommandBuffer command_buffer,
                                      Uint32 slot_index,
                                      SourceBytes data)
{
  SDL_PushGPUComputeUniformData(
    command_buffer, slot_index, data.data(), data.size_bytes());
}

inline void GPUCommandBuffer::PushComputeUniformData(Uint32 slot_index,
                                                     SourceBytes data)
{
  SDL::PushGPUComputeUniformData(
    m_gPUCommandBuffer, slot_index, std::move(data));
}

/**
 * Begins a render pass on a command buffer.
 *
 * A render pass consists of a set of texture subresources (or depth slices in
 * the 3D texture case) which will be rendered to during the render pass,
 * along with corresponding clear values and load/store operations. All
 * operations related to graphics pipelines must take place inside of a render
 * pass. A default viewport and scissor state are automatically set when this
 * is called. You cannot begin another render pass, or begin a compute pass or
 * copy pass until you have ended the render pass.
 *
 * @param command_buffer a command buffer.
 * @param color_target_infos an array of texture subresources with
 *                           corresponding clear values and load/store ops.
 * @param depth_stencil_target_info a texture subresource with corresponding
 *                                  clear value and load/store ops, may be
 *                                  nullptr.
 * @returns a render pass handle.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPURenderPass.End
 */
inline GPURenderPass BeginGPURenderPass(
  GPUCommandBuffer command_buffer,
  std::span<const GPUColorTargetInfo> color_target_infos,
  OptionalRef<const GPUDepthStencilTargetInfo> depth_stencil_target_info)
{
  return SDL_BeginGPURenderPass(command_buffer,
                                color_target_infos.data(),
                                color_target_infos.size(),
                                depth_stencil_target_info);
}

inline GPURenderPass GPUCommandBuffer::BeginRenderPass(
  std::span<const GPUColorTargetInfo> color_target_infos,
  OptionalRef<const GPUDepthStencilTargetInfo> depth_stencil_target_info)
{
  return SDL::BeginGPURenderPass(
    m_gPUCommandBuffer, color_target_infos, depth_stencil_target_info);
}

/**
 * Binds a graphics pipeline on a render pass to be used in rendering.
 *
 * A graphics pipeline must be bound before making any draw calls.
 *
 * @param render_pass a render pass handle.
 * @param graphics_pipeline the graphics pipeline to bind.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void BindGPUGraphicsPipeline(GPURenderPass render_pass,
                                    GPUGraphicsPipeline graphics_pipeline)
{
  SDL_BindGPUGraphicsPipeline(render_pass, graphics_pipeline);
}

inline void GPURenderPass::BindPipeline(GPUGraphicsPipeline graphics_pipeline)
{
  SDL::BindGPUGraphicsPipeline(m_gPURenderPass, graphics_pipeline);
}

/**
 * Sets the current viewport state on a command buffer.
 *
 * @param render_pass a render pass handle.
 * @param viewport the viewport to set.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetGPUViewport(GPURenderPass render_pass,
                           const GPUViewport& viewport)
{
  SDL_SetGPUViewport(render_pass, &viewport);
}

inline void GPURenderPass::SetViewport(const GPUViewport& viewport)
{
  SDL::SetGPUViewport(m_gPURenderPass, viewport);
}

/**
 * Sets the current scissor state on a command buffer.
 *
 * @param render_pass a render pass handle.
 * @param scissor the scissor area to set.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetGPUScissor(GPURenderPass render_pass, const RectRaw& scissor)
{
  SDL_SetGPUScissor(render_pass, &scissor);
}

inline void GPURenderPass::SetScissor(const RectRaw& scissor)
{
  SDL::SetGPUScissor(m_gPURenderPass, scissor);
}

/**
 * Sets the current blend constants on a command buffer.
 *
 * @param render_pass a render pass handle.
 * @param blend_constants the blend constant color.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPU_BLENDFACTOR_CONSTANT_COLOR
 * @sa GPU_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR
 */
inline void SetGPUBlendConstants(GPURenderPass render_pass,
                                 FColorRaw blend_constants)
{
  SDL_SetGPUBlendConstants(render_pass, blend_constants);
}

inline void GPURenderPass::SetBlendConstants(FColorRaw blend_constants)
{
  SDL::SetGPUBlendConstants(m_gPURenderPass, blend_constants);
}

/**
 * Sets the current stencil reference value on a command buffer.
 *
 * @param render_pass a render pass handle.
 * @param reference the stencil reference value to set.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetGPUStencilReference(GPURenderPass render_pass, Uint8 reference)
{
  SDL_SetGPUStencilReference(render_pass, reference);
}

inline void GPURenderPass::SetStencilReference(Uint8 reference)
{
  SDL::SetGPUStencilReference(m_gPURenderPass, reference);
}

/**
 * Binds vertex buffers on a command buffer for use with subsequent draw
 * calls.
 *
 * @param render_pass a render pass handle.
 * @param first_slot the vertex buffer slot to begin binding from.
 * @param bindings an array of GPUBufferBinding structs containing vertex
 *                 buffers and offset values.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void BindGPUVertexBuffers(GPURenderPass render_pass,
                                 Uint32 first_slot,
                                 std::span<const GPUBufferBinding> bindings)
{
  SDL_BindGPUVertexBuffers(
    render_pass, first_slot, bindings.data(), bindings.size());
}

inline void GPURenderPass::BindVertexBuffers(
  Uint32 first_slot,
  std::span<const GPUBufferBinding> bindings)
{
  SDL::BindGPUVertexBuffers(m_gPURenderPass, first_slot, bindings);
}

/**
 * Binds an index buffer on a command buffer for use with subsequent draw
 * calls.
 *
 * @param render_pass a render pass handle.
 * @param binding a pointer to a struct containing an index buffer and offset.
 * @param index_element_size whether the index values in the buffer are 16- or
 *                           32-bit.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void BindGPUIndexBuffer(GPURenderPass render_pass,
                               const GPUBufferBinding& binding,
                               GPUIndexElementSize index_element_size)
{
  SDL_BindGPUIndexBuffer(render_pass, &binding, index_element_size);
}

inline void GPURenderPass::BindIndexBuffer(
  const GPUBufferBinding& binding,
  GPUIndexElementSize index_element_size)
{
  SDL::BindGPUIndexBuffer(m_gPURenderPass, binding, index_element_size);
}

/**
 * Binds texture-sampler pairs for use on the vertex shader.
 *
 * The textures must have been created with GPU_TEXTUREUSAGE_SAMPLER.
 *
 * Be sure your shader is set up according to the requirements documented in
 * GPUShader.GPUShader().
 *
 * @param render_pass a render pass handle.
 * @param first_slot the vertex sampler slot to begin binding from.
 * @param texture_sampler_bindings an array of texture-sampler binding
 *                                 structs.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
inline void BindGPUVertexSamplers(
  GPURenderPass render_pass,
  Uint32 first_slot,
  std::span<const GPUTextureSamplerBinding> texture_sampler_bindings)
{
  SDL_BindGPUVertexSamplers(render_pass,
                            first_slot,
                            texture_sampler_bindings.data(),
                            texture_sampler_bindings.size());
}

inline void GPURenderPass::BindVertexSamplers(
  Uint32 first_slot,
  std::span<const GPUTextureSamplerBinding> texture_sampler_bindings)
{
  SDL::BindGPUVertexSamplers(
    m_gPURenderPass, first_slot, texture_sampler_bindings);
}

/**
 * Binds storage textures for use on the vertex shader.
 *
 * These textures must have been created with
 * GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ.
 *
 * Be sure your shader is set up according to the requirements documented in
 * GPUShader.GPUShader().
 *
 * @param render_pass a render pass handle.
 * @param first_slot the vertex storage texture slot to begin binding from.
 * @param storage_textures an array of storage textures.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
inline void BindGPUVertexStorageTextures(
  GPURenderPass render_pass,
  Uint32 first_slot,
  SpanRef<const GPUTextureRaw> storage_textures)
{
  SDL_BindGPUVertexStorageTextures(
    render_pass, first_slot, storage_textures.data(), storage_textures.size());
}

inline void GPURenderPass::BindVertexStorageTextures(
  Uint32 first_slot,
  SpanRef<const GPUTextureRaw> storage_textures)
{
  SDL::BindGPUVertexStorageTextures(
    m_gPURenderPass, first_slot, storage_textures);
}

/**
 * Binds storage buffers for use on the vertex shader.
 *
 * These buffers must have been created with
 * GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ.
 *
 * Be sure your shader is set up according to the requirements documented in
 * GPUShader.GPUShader().
 *
 * @param render_pass a render pass handle.
 * @param first_slot the vertex storage buffer slot to begin binding from.
 * @param storage_buffers an array of buffers.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
inline void BindGPUVertexStorageBuffers(
  GPURenderPass render_pass,
  Uint32 first_slot,
  SpanRef<const GPUBufferRaw> storage_buffers)
{
  SDL_BindGPUVertexStorageBuffers(
    render_pass, first_slot, storage_buffers.data(), storage_buffers.size());
}

inline void GPURenderPass::BindVertexStorageBuffers(
  Uint32 first_slot,
  SpanRef<const GPUBufferRaw> storage_buffers)
{
  SDL::BindGPUVertexStorageBuffers(
    m_gPURenderPass, first_slot, storage_buffers);
}

/**
 * Binds texture-sampler pairs for use on the fragment shader.
 *
 * The textures must have been created with GPU_TEXTUREUSAGE_SAMPLER.
 *
 * Be sure your shader is set up according to the requirements documented in
 * GPUShader.GPUShader().
 *
 * @param render_pass a render pass handle.
 * @param first_slot the fragment sampler slot to begin binding from.
 * @param texture_sampler_bindings an array of texture-sampler binding
 *                                 structs.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
inline void BindGPUFragmentSamplers(
  GPURenderPass render_pass,
  Uint32 first_slot,
  std::span<const GPUTextureSamplerBinding> texture_sampler_bindings)
{
  SDL_BindGPUFragmentSamplers(render_pass,
                              first_slot,
                              texture_sampler_bindings.data(),
                              texture_sampler_bindings.size());
}

inline void GPURenderPass::BindFragmentSamplers(
  Uint32 first_slot,
  std::span<const GPUTextureSamplerBinding> texture_sampler_bindings)
{
  SDL::BindGPUFragmentSamplers(
    m_gPURenderPass, first_slot, texture_sampler_bindings);
}

/**
 * Binds storage textures for use on the fragment shader.
 *
 * These textures must have been created with
 * GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ.
 *
 * Be sure your shader is set up according to the requirements documented in
 * GPUShader.GPUShader().
 *
 * @param render_pass a render pass handle.
 * @param first_slot the fragment storage texture slot to begin binding from.
 * @param storage_textures an array of storage textures.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
inline void BindGPUFragmentStorageTextures(
  GPURenderPass render_pass,
  Uint32 first_slot,
  SpanRef<const GPUTextureRaw> storage_textures)
{
  SDL_BindGPUFragmentStorageTextures(
    render_pass, first_slot, storage_textures.data(), storage_textures.size());
}

inline void GPURenderPass::BindFragmentStorageTextures(
  Uint32 first_slot,
  SpanRef<const GPUTextureRaw> storage_textures)
{
  SDL::BindGPUFragmentStorageTextures(
    m_gPURenderPass, first_slot, storage_textures);
}

/**
 * Binds storage buffers for use on the fragment shader.
 *
 * These buffers must have been created with
 * GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ.
 *
 * Be sure your shader is set up according to the requirements documented in
 * GPUShader.GPUShader().
 *
 * @param render_pass a render pass handle.
 * @param first_slot the fragment storage buffer slot to begin binding from.
 * @param storage_buffers an array of storage buffers.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
inline void BindGPUFragmentStorageBuffers(
  GPURenderPass render_pass,
  Uint32 first_slot,
  SpanRef<const GPUBufferRaw> storage_buffers)
{
  SDL_BindGPUFragmentStorageBuffers(
    render_pass, first_slot, storage_buffers.data(), storage_buffers.size());
}

inline void GPURenderPass::BindFragmentStorageBuffers(
  Uint32 first_slot,
  SpanRef<const GPUBufferRaw> storage_buffers)
{
  SDL::BindGPUFragmentStorageBuffers(
    m_gPURenderPass, first_slot, storage_buffers);
}

/**
 * Draws data using bound graphics state with an index buffer and instancing
 * enabled.
 *
 * You must not call this function before binding a graphics pipeline.
 *
 * Note that the `first_vertex` and `first_instance` parameters are NOT
 * compatible with built-in vertex/instance ID variables in shaders (for
 * example, SV_VertexID); GPU APIs and shader languages do not define these
 * built-in variables consistently, so if your shader depends on them, the
 * only way to keep behavior consistent and portable is to always pass 0 for
 * the correlating parameter in the draw calls.
 *
 * @param render_pass a render pass handle.
 * @param num_indices the number of indices to draw per instance.
 * @param num_instances the number of instances to draw.
 * @param first_index the starting index within the index buffer.
 * @param vertex_offset value added to vertex index before indexing into the
 *                      vertex buffer.
 * @param first_instance the ID of the first instance to draw.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void DrawGPUIndexedPrimitives(GPURenderPass render_pass,
                                     Uint32 num_indices,
                                     Uint32 num_instances,
                                     Uint32 first_index,
                                     Sint32 vertex_offset,
                                     Uint32 first_instance)
{
  SDL_DrawGPUIndexedPrimitives(render_pass,
                               num_indices,
                               num_instances,
                               first_index,
                               vertex_offset,
                               first_instance);
}

inline void GPURenderPass::DrawIndexedPrimitives(Uint32 num_indices,
                                                 Uint32 num_instances,
                                                 Uint32 first_index,
                                                 Sint32 vertex_offset,
                                                 Uint32 first_instance)
{
  SDL::DrawGPUIndexedPrimitives(m_gPURenderPass,
                                num_indices,
                                num_instances,
                                first_index,
                                vertex_offset,
                                first_instance);
}

/**
 * Draws data using bound graphics state.
 *
 * You must not call this function before binding a graphics pipeline.
 *
 * Note that the `first_vertex` and `first_instance` parameters are NOT
 * compatible with built-in vertex/instance ID variables in shaders (for
 * example, SV_VertexID); GPU APIs and shader languages do not define these
 * built-in variables consistently, so if your shader depends on them, the
 * only way to keep behavior consistent and portable is to always pass 0 for
 * the correlating parameter in the draw calls.
 *
 * @param render_pass a render pass handle.
 * @param num_vertices the number of vertices to draw.
 * @param num_instances the number of instances that will be drawn.
 * @param first_vertex the index of the first vertex to draw.
 * @param first_instance the ID of the first instance to draw.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void DrawGPUPrimitives(GPURenderPass render_pass,
                              Uint32 num_vertices,
                              Uint32 num_instances,
                              Uint32 first_vertex,
                              Uint32 first_instance)
{
  SDL_DrawGPUPrimitives(
    render_pass, num_vertices, num_instances, first_vertex, first_instance);
}

inline void GPURenderPass::DrawPrimitives(Uint32 num_vertices,
                                          Uint32 num_instances,
                                          Uint32 first_vertex,
                                          Uint32 first_instance)
{
  SDL::DrawGPUPrimitives(
    m_gPURenderPass, num_vertices, num_instances, first_vertex, first_instance);
}

/**
 * Draws data using bound graphics state and with draw parameters set from a
 * buffer.
 *
 * The buffer must consist of tightly-packed draw parameter sets that each
 * match the layout of GPUIndirectDrawCommand. You must not call this
 * function before binding a graphics pipeline.
 *
 * @param render_pass a render pass handle.
 * @param buffer a buffer containing draw parameters.
 * @param offset the offset to start reading from the draw buffer.
 * @param draw_count the number of draw parameter sets that should be read
 *                   from the draw buffer.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void DrawGPUPrimitivesIndirect(GPURenderPass render_pass,
                                      GPUBuffer buffer,
                                      Uint32 offset,
                                      Uint32 draw_count)
{
  SDL_DrawGPUPrimitivesIndirect(render_pass, buffer, offset, draw_count);
}

inline void GPURenderPass::DrawPrimitivesIndirect(GPUBuffer buffer,
                                                  Uint32 offset,
                                                  Uint32 draw_count)
{
  SDL::DrawGPUPrimitivesIndirect(m_gPURenderPass, buffer, offset, draw_count);
}

/**
 * Draws data using bound graphics state with an index buffer enabled and with
 * draw parameters set from a buffer.
 *
 * The buffer must consist of tightly-packed draw parameter sets that each
 * match the layout of GPUIndexedIndirectDrawCommand. You must not call
 * this function before binding a graphics pipeline.
 *
 * @param render_pass a render pass handle.
 * @param buffer a buffer containing draw parameters.
 * @param offset the offset to start reading from the draw buffer.
 * @param draw_count the number of draw parameter sets that should be read
 *                   from the draw buffer.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void DrawGPUIndexedPrimitivesIndirect(GPURenderPass render_pass,
                                             GPUBuffer buffer,
                                             Uint32 offset,
                                             Uint32 draw_count)
{
  SDL_DrawGPUIndexedPrimitivesIndirect(render_pass, buffer, offset, draw_count);
}

inline void GPURenderPass::DrawIndexedPrimitivesIndirect(GPUBuffer buffer,
                                                         Uint32 offset,
                                                         Uint32 draw_count)
{
  SDL::DrawGPUIndexedPrimitivesIndirect(
    m_gPURenderPass, buffer, offset, draw_count);
}

/**
 * Ends the given render pass.
 *
 * All bound graphics state on the render pass command buffer is unset. The
 * render pass handle is now invalid.
 *
 * @param render_pass a render pass handle.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EndGPURenderPass(GPURenderPass render_pass)
{
  SDL_EndGPURenderPass(render_pass);
}

inline void GPURenderPass::End() { SDL::EndGPURenderPass(m_gPURenderPass); }

/**
 * Begins a compute pass on a command buffer.
 *
 * A compute pass is defined by a set of texture subresources and buffers that
 * may be written to by compute pipelines. These textures and buffers must
 * have been created with the COMPUTE_STORAGE_WRITE bit or the
 * COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE bit. If you do not create a texture
 * with COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE, you must not read from the
 * texture in the compute pass. All operations related to compute pipelines
 * must take place inside of a compute pass. You must not begin another
 * compute pass, or a render pass or copy pass before ending the compute pass.
 *
 * A VERY IMPORTANT NOTE - Reads and writes in compute passes are NOT
 * implicitly synchronized. This means you may cause data races by both
 * reading and writing a resource region in a compute pass, or by writing
 * multiple times to a resource region. If your compute work depends on
 * reading the completed output from a previous dispatch, you MUST end the
 * current compute pass and begin a new one before you can safely access the
 * data. Otherwise you will receive unexpected results. Reading and writing a
 * texture in the same compute pass is only supported by specific texture
 * formats. Make sure you check the format support!
 *
 * @param command_buffer a command buffer.
 * @param storage_texture_bindings an array of writeable storage texture
 *                                 binding structs.
 * @param storage_buffer_bindings an array of writeable storage buffer binding
 *                                structs.
 * @returns a compute pass handle.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUComputePass.End
 */
inline GPUComputePass BeginGPUComputePass(
  GPUCommandBuffer command_buffer,
  std::span<const GPUStorageTextureReadWriteBinding> storage_texture_bindings,
  std::span<const GPUStorageBufferReadWriteBinding> storage_buffer_bindings)
{
  return SDL_BeginGPUComputePass(command_buffer,
                                 storage_texture_bindings.data(),
                                 storage_texture_bindings.size(),
                                 storage_buffer_bindings.data(),
                                 storage_buffer_bindings.size());
}

inline GPUComputePass GPUCommandBuffer::BeginComputePass(
  std::span<const GPUStorageTextureReadWriteBinding> storage_texture_bindings,
  std::span<const GPUStorageBufferReadWriteBinding> storage_buffer_bindings)
{
  return SDL::BeginGPUComputePass(
    m_gPUCommandBuffer, storage_texture_bindings, storage_buffer_bindings);
}

/**
 * Binds a compute pipeline on a command buffer for use in compute dispatch.
 *
 * @param compute_pass a compute pass handle.
 * @param compute_pipeline a compute pipeline to bind.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void BindGPUComputePipeline(GPUComputePass compute_pass,
                                   GPUComputePipeline compute_pipeline)
{
  SDL_BindGPUComputePipeline(compute_pass, compute_pipeline);
}

inline void GPUComputePass::BindPipeline(GPUComputePipeline compute_pipeline)
{
  SDL::BindGPUComputePipeline(m_gPUComputePass, compute_pipeline);
}

/**
 * Binds texture-sampler pairs for use on the compute shader.
 *
 * The textures must have been created with GPU_TEXTUREUSAGE_SAMPLER.
 *
 * Be sure your shader is set up according to the requirements documented in
 * GPUShader.GPUShader().
 *
 * @param compute_pass a compute pass handle.
 * @param first_slot the compute sampler slot to begin binding from.
 * @param texture_sampler_bindings an array of texture-sampler binding
 *                                 structs.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
inline void BindGPUComputeSamplers(
  GPUComputePass compute_pass,
  Uint32 first_slot,
  std::span<const GPUTextureSamplerBinding> texture_sampler_bindings)
{
  SDL_BindGPUComputeSamplers(compute_pass,
                             first_slot,
                             texture_sampler_bindings.data(),
                             texture_sampler_bindings.size());
}

inline void GPUComputePass::BindSamplers(
  Uint32 first_slot,
  std::span<const GPUTextureSamplerBinding> texture_sampler_bindings)
{
  SDL::BindGPUComputeSamplers(
    m_gPUComputePass, first_slot, texture_sampler_bindings);
}

/**
 * Binds storage textures as readonly for use on the compute pipeline.
 *
 * These textures must have been created with
 * GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ.
 *
 * Be sure your shader is set up according to the requirements documented in
 * GPUShader.GPUShader().
 *
 * @param compute_pass a compute pass handle.
 * @param first_slot the compute storage texture slot to begin binding from.
 * @param storage_textures an array of storage textures.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
inline void BindGPUComputeStorageTextures(
  GPUComputePass compute_pass,
  Uint32 first_slot,
  SpanRef<const GPUTextureRaw> storage_textures)
{
  SDL_BindGPUComputeStorageTextures(
    compute_pass, first_slot, storage_textures.data(), storage_textures.size());
}

inline void GPUComputePass::BindStorageTextures(
  Uint32 first_slot,
  SpanRef<const GPUTextureRaw> storage_textures)
{
  SDL::BindGPUComputeStorageTextures(
    m_gPUComputePass, first_slot, storage_textures);
}

/**
 * Binds storage buffers as readonly for use on the compute pipeline.
 *
 * These buffers must have been created with
 * GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ.
 *
 * Be sure your shader is set up according to the requirements documented in
 * GPUShader.GPUShader().
 *
 * @param compute_pass a compute pass handle.
 * @param first_slot the compute storage buffer slot to begin binding from.
 * @param storage_buffers an array of storage buffer binding structs.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUShader.GPUShader
 */
inline void BindGPUComputeStorageBuffers(
  GPUComputePass compute_pass,
  Uint32 first_slot,
  SpanRef<const GPUBufferRaw> storage_buffers)
{
  SDL_BindGPUComputeStorageBuffers(
    compute_pass, first_slot, storage_buffers.data(), storage_buffers.size());
}

inline void GPUComputePass::BindStorageBuffers(
  Uint32 first_slot,
  SpanRef<const GPUBufferRaw> storage_buffers)
{
  SDL::BindGPUComputeStorageBuffers(
    m_gPUComputePass, first_slot, storage_buffers);
}

/**
 * Dispatches compute work.
 *
 * You must not call this function before binding a compute pipeline.
 *
 * A VERY IMPORTANT NOTE If you dispatch multiple times in a compute pass, and
 * the dispatches write to the same resource region as each other, there is no
 * guarantee of which order the writes will occur. If the write order matters,
 * you MUST end the compute pass and begin another one.
 *
 * @param compute_pass a compute pass handle.
 * @param groupcount_x number of local workgroups to dispatch in the X
 *                     dimension.
 * @param groupcount_y number of local workgroups to dispatch in the Y
 *                     dimension.
 * @param groupcount_z number of local workgroups to dispatch in the Z
 *                     dimension.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void DispatchGPUCompute(GPUComputePass compute_pass,
                               Uint32 groupcount_x,
                               Uint32 groupcount_y,
                               Uint32 groupcount_z)
{
  SDL_DispatchGPUCompute(
    compute_pass, groupcount_x, groupcount_y, groupcount_z);
}

inline void GPUComputePass::Dispatch(Uint32 groupcount_x,
                                     Uint32 groupcount_y,
                                     Uint32 groupcount_z)
{
  SDL::DispatchGPUCompute(
    m_gPUComputePass, groupcount_x, groupcount_y, groupcount_z);
}

/**
 * Dispatches compute work with parameters set from a buffer.
 *
 * The buffer layout should match the layout of
 * GPUIndirectDispatchCommand. You must not call this function before
 * binding a compute pipeline.
 *
 * A VERY IMPORTANT NOTE If you dispatch multiple times in a compute pass, and
 * the dispatches write to the same resource region as each other, there is no
 * guarantee of which order the writes will occur. If the write order matters,
 * you MUST end the compute pass and begin another one.
 *
 * @param compute_pass a compute pass handle.
 * @param buffer a buffer containing dispatch parameters.
 * @param offset the offset to start reading from the dispatch buffer.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void DispatchGPUComputeIndirect(GPUComputePass compute_pass,
                                       GPUBuffer buffer,
                                       Uint32 offset)
{
  SDL_DispatchGPUComputeIndirect(compute_pass, buffer, offset);
}

inline void GPUComputePass::DispatchIndirect(GPUBuffer buffer, Uint32 offset)
{
  SDL::DispatchGPUComputeIndirect(m_gPUComputePass, buffer, offset);
}

/**
 * Ends the current compute pass.
 *
 * All bound compute state on the command buffer is unset. The compute pass
 * handle is now invalid.
 *
 * @param compute_pass a compute pass handle.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EndGPUComputePass(GPUComputePass compute_pass)
{
  SDL_EndGPUComputePass(compute_pass);
}

inline void GPUComputePass::End() { SDL::EndGPUComputePass(m_gPUComputePass); }

/**
 * Maps a transfer buffer into application address space.
 *
 * You must unmap the transfer buffer before encoding upload commands. The
 * memory is owned by the graphics driver - do NOT call free() on the
 * returned pointer.
 *
 * @param device a GPU context.
 * @param transfer_buffer a transfer buffer.
 * @param cycle if true, cycles the transfer buffer if it is already bound.
 * @returns the address of the mapped transfer buffer memory, or nullptr on
 *          failure; call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void* MapGPUTransferBuffer(GPUDeviceParam device,
                                  GPUTransferBuffer transfer_buffer,
                                  bool cycle)
{
  return SDL_MapGPUTransferBuffer(device, transfer_buffer, cycle);
}

inline void* GPUDevice::MapTransferBuffer(GPUTransferBuffer transfer_buffer,
                                          bool cycle)
{
  return SDL::MapGPUTransferBuffer(m_resource, transfer_buffer, cycle);
}

/**
 * Unmaps a previously mapped transfer buffer.
 *
 * @param device a GPU context.
 * @param transfer_buffer a previously mapped transfer buffer.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void UnmapGPUTransferBuffer(GPUDeviceParam device,
                                   GPUTransferBuffer transfer_buffer)
{
  SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
}

inline void GPUDevice::UnmapTransferBuffer(GPUTransferBuffer transfer_buffer)
{
  SDL::UnmapGPUTransferBuffer(m_resource, transfer_buffer);
}

/**
 * Begins a copy pass on a command buffer.
 *
 * All operations related to copying to or from buffers or textures take place
 * inside a copy pass. You must not begin another copy pass, or a render pass
 * or compute pass before ending the copy pass.
 *
 * @param command_buffer a command buffer.
 * @returns a copy pass handle.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline GPUCopyPass BeginGPUCopyPass(GPUCommandBuffer command_buffer)
{
  return SDL_BeginGPUCopyPass(command_buffer);
}

inline GPUCopyPass GPUCommandBuffer::BeginCopyPass()
{
  return SDL::BeginGPUCopyPass(m_gPUCommandBuffer);
}

/**
 * Uploads data from a transfer buffer to a texture.
 *
 * The upload occurs on the GPU timeline. You may assume that the upload has
 * finished in subsequent commands.
 *
 * You must align the data in the transfer buffer to a multiple of the texel
 * size of the texture format.
 *
 * @param copy_pass a copy pass handle.
 * @param source the source transfer buffer with image layout information.
 * @param destination the destination texture region.
 * @param cycle if true, cycles the texture if the texture is bound, otherwise
 *              overwrites the data.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void UploadToGPUTexture(GPUCopyPass copy_pass,
                               const GPUTextureTransferInfo& source,
                               const GPUTextureRegion& destination,
                               bool cycle)
{
  SDL_UploadToGPUTexture(copy_pass, &source, &destination, cycle);
}

inline void GPUCopyPass::UploadToTexture(const GPUTextureTransferInfo& source,
                                         const GPUTextureRegion& destination,
                                         bool cycle)
{
  SDL::UploadToGPUTexture(m_gPUCopyPass, source, destination, cycle);
}

/**
 * Uploads data from a transfer buffer to a buffer.
 *
 * The upload occurs on the GPU timeline. You may assume that the upload has
 * finished in subsequent commands.
 *
 * @param copy_pass a copy pass handle.
 * @param source the source transfer buffer with offset.
 * @param destination the destination buffer with offset and size.
 * @param cycle if true, cycles the buffer if it is already bound, otherwise
 *              overwrites the data.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void UploadToGPUBuffer(GPUCopyPass copy_pass,
                              const GPUTransferBufferLocation& source,
                              const GPUBufferRegion& destination,
                              bool cycle)
{
  SDL_UploadToGPUBuffer(copy_pass, &source, &destination, cycle);
}

inline void GPUCopyPass::UploadToBuffer(const GPUTransferBufferLocation& source,
                                        const GPUBufferRegion& destination,
                                        bool cycle)
{
  SDL::UploadToGPUBuffer(m_gPUCopyPass, source, destination, cycle);
}

/**
 * Performs a texture-to-texture copy.
 *
 * This copy occurs on the GPU timeline. You may assume the copy has finished
 * in subsequent commands.
 *
 * @param copy_pass a copy pass handle.
 * @param source a source texture region.
 * @param destination a destination texture region.
 * @param w the width of the region to copy.
 * @param h the height of the region to copy.
 * @param d the depth of the region to copy.
 * @param cycle if true, cycles the destination texture if the destination
 *              texture is bound, otherwise overwrites the data.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void CopyGPUTextureToTexture(GPUCopyPass copy_pass,
                                    const GPUTextureLocation& source,
                                    const GPUTextureLocation& destination,
                                    Uint32 w,
                                    Uint32 h,
                                    Uint32 d,
                                    bool cycle)
{
  SDL_CopyGPUTextureToTexture(copy_pass, &source, &destination, w, h, d, cycle);
}

inline void GPUCopyPass::CopyTextureToTexture(
  const GPUTextureLocation& source,
  const GPUTextureLocation& destination,
  Uint32 w,
  Uint32 h,
  Uint32 d,
  bool cycle)
{
  SDL::CopyGPUTextureToTexture(
    m_gPUCopyPass, source, destination, w, h, d, cycle);
}

/**
 * Performs a buffer-to-buffer copy.
 *
 * This copy occurs on the GPU timeline. You may assume the copy has finished
 * in subsequent commands.
 *
 * @param copy_pass a copy pass handle.
 * @param source the buffer and offset to copy from.
 * @param destination the buffer and offset to copy to.
 * @param size the length of the buffer to copy.
 * @param cycle if true, cycles the destination buffer if it is already bound,
 *              otherwise overwrites the data.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void CopyGPUBufferToBuffer(GPUCopyPass copy_pass,
                                  const GPUBufferLocation& source,
                                  const GPUBufferLocation& destination,
                                  Uint32 size,
                                  bool cycle)
{
  SDL_CopyGPUBufferToBuffer(copy_pass, &source, &destination, size, cycle);
}

inline void GPUCopyPass::CopyBufferToBuffer(
  const GPUBufferLocation& source,
  const GPUBufferLocation& destination,
  Uint32 size,
  bool cycle)
{
  SDL::CopyGPUBufferToBuffer(m_gPUCopyPass, source, destination, size, cycle);
}

/**
 * Copies data from a texture to a transfer buffer on the GPU timeline.
 *
 * This data is not guaranteed to be copied until the command buffer fence is
 * signaled.
 *
 * @param copy_pass a copy pass handle.
 * @param source the source texture region.
 * @param destination the destination transfer buffer with image layout
 *                    information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void DownloadFromGPUTexture(GPUCopyPass copy_pass,
                                   const GPUTextureRegion& source,
                                   const GPUTextureTransferInfo& destination)
{
  SDL_DownloadFromGPUTexture(copy_pass, &source, &destination);
}

inline void GPUCopyPass::DownloadFromTexture(
  const GPUTextureRegion& source,
  const GPUTextureTransferInfo& destination)
{
  SDL::DownloadFromGPUTexture(m_gPUCopyPass, source, destination);
}

/**
 * Copies data from a buffer to a transfer buffer on the GPU timeline.
 *
 * This data is not guaranteed to be copied until the command buffer fence is
 * signaled.
 *
 * @param copy_pass a copy pass handle.
 * @param source the source buffer with offset and size.
 * @param destination the destination transfer buffer with offset.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void DownloadFromGPUBuffer(GPUCopyPass copy_pass,
                                  const GPUBufferRegion& source,
                                  const GPUTransferBufferLocation& destination)
{
  SDL_DownloadFromGPUBuffer(copy_pass, &source, &destination);
}

inline void GPUCopyPass::DownloadFromBuffer(
  const GPUBufferRegion& source,
  const GPUTransferBufferLocation& destination)
{
  SDL::DownloadFromGPUBuffer(m_gPUCopyPass, source, destination);
}

/**
 * Ends the current copy pass.
 *
 * @param copy_pass a copy pass handle.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EndGPUCopyPass(GPUCopyPass copy_pass)
{
  SDL_EndGPUCopyPass(copy_pass);
}

inline void GPUCopyPass::End() { SDL::EndGPUCopyPass(m_gPUCopyPass); }

/**
 * Generates mipmaps for the given texture.
 *
 * This function must not be called inside of any pass.
 *
 * @param command_buffer a command_buffer.
 * @param texture a texture with more than 1 mip level.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GenerateMipmapsForGPUTexture(GPUCommandBuffer command_buffer,
                                         GPUTexture texture)
{
  SDL_GenerateMipmapsForGPUTexture(command_buffer, texture);
}

inline void GPUCommandBuffer::GenerateMipmapsForTexture(GPUTexture texture)
{
  SDL::GenerateMipmapsForGPUTexture(m_gPUCommandBuffer, texture);
}

/**
 * Blits from a source texture region to a destination texture region.
 *
 * This function must not be called inside of any pass.
 *
 * @param command_buffer a command buffer.
 * @param info the blit info struct containing the blit parameters.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void BlitGPUTexture(GPUCommandBuffer command_buffer,
                           const GPUBlitInfo& info)
{
  SDL_BlitGPUTexture(command_buffer, &info);
}

inline void GPUCommandBuffer::BlitTexture(const GPUBlitInfo& info)
{
  SDL::BlitGPUTexture(m_gPUCommandBuffer, info);
}

/**
 * Determines whether a swapchain composition is supported by the window.
 *
 * The window must be claimed before calling this function.
 *
 * @param device a GPU context.
 * @param window an Window.
 * @param swapchain_composition the swapchain composition to check.
 * @returns true if supported, false if unsupported.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.ClaimWindow
 */
inline bool WindowSupportsGPUSwapchainComposition(
  GPUDeviceParam device,
  WindowParam window,
  GPUSwapchainComposition swapchain_composition)
{
  return SDL_WindowSupportsGPUSwapchainComposition(
    device, window, swapchain_composition);
}

inline bool GPUDevice::WindowSupportsSwapchainComposition(
  WindowParam window,
  GPUSwapchainComposition swapchain_composition)
{
  return SDL::WindowSupportsGPUSwapchainComposition(
    m_resource, window, swapchain_composition);
}

/**
 * Determines whether a presentation mode is supported by the window.
 *
 * The window must be claimed before calling this function.
 *
 * @param device a GPU context.
 * @param window an Window.
 * @param present_mode the presentation mode to check.
 * @returns true if supported, false if unsupported.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.ClaimWindow
 */
inline bool WindowSupportsGPUPresentMode(GPUDeviceParam device,
                                         WindowParam window,
                                         GPUPresentMode present_mode)
{
  return SDL_WindowSupportsGPUPresentMode(device, window, present_mode);
}

inline bool GPUDevice::WindowSupportsPresentMode(WindowParam window,
                                                 GPUPresentMode present_mode)
{
  return SDL::WindowSupportsGPUPresentMode(m_resource, window, present_mode);
}

/**
 * Claims a window, creating a swapchain structure for it.
 *
 * This must be called before GPUCommandBuffer.AcquireSwapchainTexture is called
 * using the window. You should only call this function from the thread that
 * created the window.
 *
 * The swapchain will be created with GPU_SWAPCHAINCOMPOSITION_SDR and
 * GPU_PRESENTMODE_VSYNC. If you want to have different swapchain
 * parameters, you must call GPUDevice.SetSwapchainParameters after claiming the
 * window.
 *
 * @param device a GPU context.
 * @param window an Window.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called from the thread that
 *               created the window.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
 * @sa GPUDevice.ReleaseWindow
 * @sa GPUDevice.WindowSupportsPresentMode
 * @sa GPUDevice.WindowSupportsSwapchainComposition
 */
inline void ClaimWindowForGPUDevice(GPUDeviceParam device, WindowParam window)
{
  CheckError(SDL_ClaimWindowForGPUDevice(device, window));
}

inline void GPUDevice::ClaimWindow(WindowParam window)
{
  SDL::ClaimWindowForGPUDevice(m_resource, window);
}

/**
 * Unclaims a window, destroying its swapchain structure.
 *
 * @param device a GPU context.
 * @param window an Window that has been claimed.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.ClaimWindow
 */
inline void ReleaseWindowFromGPUDevice(GPUDeviceParam device,
                                       WindowParam window)
{
  SDL_ReleaseWindowFromGPUDevice(device, window);
}

inline void GPUDevice::ReleaseWindow(WindowParam window)
{
  SDL::ReleaseWindowFromGPUDevice(m_resource, window);
}

/**
 * Changes the swapchain parameters for the given claimed window.
 *
 * This function will fail if the requested present mode or swapchain
 * composition are unsupported by the device. Check if the parameters are
 * supported via GPUDevice.WindowSupportsPresentMode /
 * GPUDevice.WindowSupportsSwapchainComposition prior to calling this function.
 *
 * GPU_PRESENTMODE_VSYNC with GPU_SWAPCHAINCOMPOSITION_SDR are always
 * supported.
 *
 * @param device a GPU context.
 * @param window an Window that has been claimed.
 * @param swapchain_composition the desired composition of the swapchain.
 * @param present_mode the desired present mode for the swapchain.
 * @returns true if successful, false on error; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.WindowSupportsPresentMode
 * @sa GPUDevice.WindowSupportsSwapchainComposition
 */
inline bool SetGPUSwapchainParameters(
  GPUDeviceParam device,
  WindowParam window,
  GPUSwapchainComposition swapchain_composition,
  GPUPresentMode present_mode)
{
  return SDL_SetGPUSwapchainParameters(
    device, window, swapchain_composition, present_mode);
}

inline bool GPUDevice::SetSwapchainParameters(
  WindowParam window,
  GPUSwapchainComposition swapchain_composition,
  GPUPresentMode present_mode)
{
  return SDL::SetGPUSwapchainParameters(
    m_resource, window, swapchain_composition, present_mode);
}

/**
 * Configures the maximum allowed number of frames in flight.
 *
 * The default value when the device is created is 2. This means that after
 * you have submitted 2 frames for presentation, if the GPU has not finished
 * working on the first frame, GPUCommandBuffer.AcquireSwapchainTexture() will
 * fill the swapchain texture pointer with nullptr, and
 * GPUCommandBuffer.WaitAndAcquireSwapchainTexture() will block.
 *
 * Higher values increase throughput at the expense of visual latency. Lower
 * values decrease visual latency at the expense of throughput.
 *
 * Note that calling this function will stall and flush the command queue to
 * prevent synchronization issues.
 *
 * The minimum value of allowed frames in flight is 1, and the maximum is 3.
 *
 * @param device a GPU context.
 * @param allowed_frames_in_flight the maximum number of frames that can be
 *                                 pending on the GPU.
 * @returns true if successful, false on error; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool SetGPUAllowedFramesInFlight(GPUDeviceParam device,
                                        Uint32 allowed_frames_in_flight)
{
  return SDL_SetGPUAllowedFramesInFlight(device, allowed_frames_in_flight);
}

inline bool GPUDevice::SetAllowedFramesInFlight(Uint32 allowed_frames_in_flight)
{
  return SDL::SetGPUAllowedFramesInFlight(m_resource, allowed_frames_in_flight);
}

/**
 * Obtains the texture format of the swapchain for the given window.
 *
 * Note that this format can change if the swapchain parameters change.
 *
 * @param device a GPU context.
 * @param window an Window that has been claimed.
 * @returns the texture format of the swapchain.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline GPUTextureFormat GetGPUSwapchainTextureFormat(GPUDeviceParam device,
                                                     WindowParam window)
{
  return SDL_GetGPUSwapchainTextureFormat(device, window);
}

inline GPUTextureFormat GPUDevice::GetSwapchainTextureFormat(WindowParam window)
{
  return SDL::GetGPUSwapchainTextureFormat(m_resource, window);
}

/**
 * Acquire a texture to use in presentation.
 *
 * When a swapchain texture is acquired on a command buffer, it will
 * automatically be submitted for presentation when the command buffer is
 * submitted. The swapchain texture should only be referenced by the command
 * buffer used to acquire it.
 *
 * This function will fill the swapchain texture handle with nullptr if too many
 * frames are in flight. This is not an error.
 *
 * If you use this function, it is possible to create a situation where many
 * command buffers are allocated while the rendering context waits for the GPU
 * to catch up, which will cause memory usage to grow. You should use
 * GPUCommandBuffer.WaitAndAcquireSwapchainTexture() unless you know what you
 * are doing with timing.
 *
 * The swapchain texture is managed by the implementation and must not be
 * freed by the user. You MUST NOT call this function from any thread other
 * than the one that created the window.
 *
 * @param command_buffer a command buffer.
 * @param window a window that has been claimed.
 * @param swapchain_texture_width a pointer filled in with the swapchain
 *                                texture width, may be nullptr.
 * @param swapchain_texture_height a pointer filled in with the swapchain
 *                                 texture height, may be nullptr.
 * @returns a swapchain texture handle.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called from the thread that
 *               created the window.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.ClaimWindow
 * @sa GPUCommandBuffer.Submit
 * @sa GPUCommandBuffer.SubmitAndAcquireFence
 * @sa GPUCommandBuffer.Cancel
 * @sa Window.GetSizeInPixels
 * @sa GPUDevice.WaitForSwapchain
 * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
 * @sa GPUDevice.SetAllowedFramesInFlight
 */
inline GPUTexture AcquireGPUSwapchainTexture(
  GPUCommandBuffer command_buffer,
  WindowParam window,
  Uint32* swapchain_texture_width = nullptr,
  Uint32* swapchain_texture_height = nullptr)
{
  GPUTextureRaw texture;
  CheckError(SDL_AcquireGPUSwapchainTexture(command_buffer,
                                            window,
                                            &texture,
                                            swapchain_texture_width,
                                            swapchain_texture_height));
  return texture;
}

inline GPUTexture GPUCommandBuffer::AcquireSwapchainTexture(
  WindowParam window,
  Uint32* swapchain_texture_width,
  Uint32* swapchain_texture_height)
{
  return SDL::AcquireGPUSwapchainTexture(m_gPUCommandBuffer,
                                         window,
                                         swapchain_texture_width,
                                         swapchain_texture_height);
}

/**
 * Blocks the thread until a swapchain texture is available to be acquired.
 *
 * @param device a GPU context.
 * @param window a window that has been claimed.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called from the thread that
 *               created the window.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.AcquireSwapchainTexture
 * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
 * @sa GPUDevice.SetAllowedFramesInFlight
 */
inline void WaitForGPUSwapchain(GPUDeviceParam device, WindowParam window)
{
  CheckError(SDL_WaitForGPUSwapchain(device, window));
}

inline void GPUDevice::WaitForSwapchain(WindowParam window)
{
  SDL::WaitForGPUSwapchain(m_resource, window);
}

/**
 * Blocks the thread until a swapchain texture is available to be acquired,
 * and then acquires it.
 *
 * When a swapchain texture is acquired on a command buffer, it will
 * automatically be submitted for presentation when the command buffer is
 * submitted. The swapchain texture should only be referenced by the command
 * buffer used to acquire it. It is an error to call
 * GPUCommandBuffer.Cancel() after a swapchain texture is acquired.
 *
 * This function can fill the swapchain texture handle with nullptr in certain
 * cases, for example if the window is minimized. This is not an error. You
 * should always make sure to check whether the pointer is nullptr before
 * actually using it.
 *
 * The swapchain texture is managed by the implementation and must not be
 * freed by the user. You MUST NOT call this function from any thread other
 * than the one that created the window.
 *
 * The swapchain texture is write-only and cannot be used as a sampler or for
 * another reading operation.
 *
 * @param command_buffer a command buffer.
 * @param window a window that has been claimed.
 * @param swapchain_texture_width a pointer filled in with the swapchain
 *                                texture width, may be nullptr.
 * @param swapchain_texture_height a pointer filled in with the swapchain
 *                                 texture height, may be nullptr.
 * @returns a swapchain texture handle.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called from the thread that
 *               created the window.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.Submit
 * @sa GPUCommandBuffer.SubmitAndAcquireFence
 * @sa GPUCommandBuffer.AcquireSwapchainTexture
 */
inline GPUTexture WaitAndAcquireGPUSwapchainTexture(
  GPUCommandBuffer command_buffer,
  WindowParam window,
  Uint32* swapchain_texture_width = nullptr,
  Uint32* swapchain_texture_height = nullptr)
{
  GPUTextureRaw texture;
  CheckError(SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer,
                                                   window,
                                                   &texture,
                                                   swapchain_texture_width,
                                                   swapchain_texture_height));
  return texture;
}

inline GPUTexture GPUCommandBuffer::WaitAndAcquireSwapchainTexture(
  WindowParam window,
  Uint32* swapchain_texture_width,
  Uint32* swapchain_texture_height)
{
  return SDL::WaitAndAcquireGPUSwapchainTexture(m_gPUCommandBuffer,
                                                window,
                                                swapchain_texture_width,
                                                swapchain_texture_height);
}

/**
 * Submits a command buffer so its commands can be processed on the GPU.
 *
 * It is invalid to use the command buffer after this is called.
 *
 * This must be called from the thread the command buffer was acquired on.
 *
 * All commands in the submission are guaranteed to begin executing before any
 * command in a subsequent submission begins executing.
 *
 * @param command_buffer a command buffer.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.AcquireCommandBuffer
 * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
 * @sa GPUCommandBuffer.AcquireSwapchainTexture
 * @sa GPUCommandBuffer.SubmitAndAcquireFence
 */
inline void SubmitGPUCommandBuffer(GPUCommandBuffer command_buffer)
{
  CheckError(SDL_SubmitGPUCommandBuffer(command_buffer));
}

inline void GPUCommandBuffer::Submit()
{
  SDL::SubmitGPUCommandBuffer(m_gPUCommandBuffer);
}

/**
 * Submits a command buffer so its commands can be processed on the GPU, and
 * acquires a fence associated with the command buffer.
 *
 * You must release this fence when it is no longer needed or it will cause a
 * leak. It is invalid to use the command buffer after this is called.
 *
 * This must be called from the thread the command buffer was acquired on.
 *
 * All commands in the submission are guaranteed to begin executing before any
 * command in a subsequent submission begins executing.
 *
 * @param command_buffer a command buffer.
 * @returns a fence associated with the command buffer, or nullptr on failure;
 *          call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.AcquireCommandBuffer
 * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
 * @sa GPUCommandBuffer.AcquireSwapchainTexture
 * @sa GPUCommandBuffer.Submit
 * @sa GPUDevice.ReleaseFence
 */
inline GPUFence* SubmitGPUCommandBufferAndAcquireFence(
  GPUCommandBuffer command_buffer)
{
  return SDL_SubmitGPUCommandBufferAndAcquireFence(command_buffer);
}

inline GPUFence* GPUCommandBuffer::SubmitAndAcquireFence()
{
  return SDL::SubmitGPUCommandBufferAndAcquireFence(m_gPUCommandBuffer);
}

/**
 * Cancels a command buffer.
 *
 * None of the enqueued commands are executed.
 *
 * It is an error to call this function after a swapchain texture has been
 * acquired.
 *
 * This must be called from the thread the command buffer was acquired on.
 *
 * You must not reference the command buffer after calling this function.
 *
 * @param command_buffer a command buffer.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.WaitAndAcquireSwapchainTexture
 * @sa GPUDevice.AcquireCommandBuffer
 * @sa GPUCommandBuffer.AcquireSwapchainTexture
 */
inline void CancelGPUCommandBuffer(GPUCommandBuffer command_buffer)
{
  CheckError(SDL_CancelGPUCommandBuffer(command_buffer));
}

inline void GPUCommandBuffer::Cancel()
{
  SDL::CancelGPUCommandBuffer(m_gPUCommandBuffer);
}

/**
 * Blocks the thread until the GPU is completely idle.
 *
 * @param device a GPU context.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUDevice.WaitForFences
 */
inline void WaitForGPUIdle(GPUDeviceParam device)
{
  CheckError(SDL_WaitForGPUIdle(device));
}

inline void GPUDevice::WaitForIdle() { SDL::WaitForGPUIdle(m_resource); }

/**
 * Blocks the thread until the given fences are signaled.
 *
 * @param device a GPU context.
 * @param wait_all if 0, wait for any fence to be signaled, if 1, wait for all
 *                 fences to be signaled.
 * @param fences an array of fences to wait on.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.SubmitAndAcquireFence
 * @sa GPUDevice.WaitForIdle
 */
inline void WaitForGPUFences(GPUDeviceParam device,
                             bool wait_all,
                             std::span<GPUFence* const> fences)
{
  CheckError(
    SDL_WaitForGPUFences(device, wait_all, fences.data(), fences.size()));
}

inline void GPUDevice::WaitForFences(bool wait_all,
                                     std::span<GPUFence* const> fences)
{
  SDL::WaitForGPUFences(m_resource, wait_all, fences);
}

/**
 * Checks the status of a fence.
 *
 * @param device a GPU context.
 * @param fence a fence.
 * @returns true if the fence is signaled, false if it is not.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.SubmitAndAcquireFence
 */
inline bool QueryGPUFence(GPUDeviceParam device, GPUFence* fence)
{
  return SDL_QueryGPUFence(device, fence);
}

inline bool GPUDevice::QueryFence(GPUFence* fence)
{
  return SDL::QueryGPUFence(m_resource, fence);
}

/**
 * Releases a fence obtained from GPUCommandBuffer.SubmitAndAcquireFence.
 *
 * You must not reference the fence after calling this function.
 *
 * @param device a GPU context.
 * @param fence a fence.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCommandBuffer.SubmitAndAcquireFence
 */
inline void ReleaseGPUFence(GPUDeviceParam device, GPUFence* fence)
{
  SDL_ReleaseGPUFence(device, fence);
}

inline void GPUDevice::ReleaseFence(GPUFence* fence)
{
  SDL::ReleaseGPUFence(m_resource, fence);
}

/**
 * Obtains the texel block size for a texture format.
 *
 * @param format the texture format you want to know the texel size of.
 * @returns the texel block size of the texture format.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GPUCopyPass.UploadToTexture
 */
inline Uint32 GPUTextureFormatTexelBlockSize(GPUTextureFormat format)
{
  return SDL_GPUTextureFormatTexelBlockSize(format);
}

/**
 * Determines whether a texture format is supported for a given type and
 * usage.
 *
 * @param device a GPU context.
 * @param format the texture format to check.
 * @param type the type of texture (2D, 3D, Cube).
 * @param usage a bitmask of all usage scenarios to check.
 * @returns whether the texture format is supported for this type and usage.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GPUTextureSupportsFormat(GPUDeviceParam device,
                                     GPUTextureFormat format,
                                     GPUTextureType type,
                                     GPUTextureUsageFlags usage)
{
  return SDL_GPUTextureSupportsFormat(device, format, type, usage);
}

inline bool GPUDevice::TextureSupportsFormat(GPUTextureFormat format,
                                             GPUTextureType type,
                                             GPUTextureUsageFlags usage)
{
  return SDL::GPUTextureSupportsFormat(m_resource, format, type, usage);
}

/**
 * Determines if a sample count for a texture format is supported.
 *
 * @param device a GPU context.
 * @param format the texture format to check.
 * @param sample_count the sample count to check.
 * @returns whether the sample count is supported for this texture format.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GPUTextureSupportsSampleCount(GPUDeviceParam device,
                                          GPUTextureFormat format,
                                          GPUSampleCount sample_count)
{
  return SDL_GPUTextureSupportsSampleCount(device, format, sample_count);
}

inline bool GPUDevice::TextureSupportsSampleCount(GPUTextureFormat format,
                                                  GPUSampleCount sample_count)
{
  return SDL::GPUTextureSupportsSampleCount(m_resource, format, sample_count);
}

/**
 * Calculate the size in bytes of a texture format with dimensions.
 *
 * @param format a texture format.
 * @param width width in pixels.
 * @param height height in pixels.
 * @param depth_or_layer_count depth for 3D textures or layer count otherwise.
 * @returns the size of a texture with this format and dimensions.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint32 CalculateGPUTextureFormatSize(GPUTextureFormat format,
                                            Uint32 width,
                                            Uint32 height,
                                            Uint32 depth_or_layer_count)
{
  return SDL_CalculateGPUTextureFormatSize(
    format, width, height, depth_or_layer_count);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_GPU_H_ */
