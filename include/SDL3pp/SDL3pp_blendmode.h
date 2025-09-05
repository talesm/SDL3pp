#ifndef SDL3PP_BLENDMODE_H_
#define SDL3PP_BLENDMODE_H_

#include <SDL3/SDL_blendmode.h>

namespace SDL {

/**
 * @defgroup CategoryBlendmode Category Blendmode
 *
 * Blend modes decide how two colors will mix together. There are both
 * standard modes for basic needs and a means to create custom modes,
 * dictating what sort of math to do on what color components.
 *
 * @{
 */

/**
 * A set of blend modes used in drawing operations.
 *
 * These predefined blend modes are supported everywhere.
 *
 * Additional values may be obtained from ComposeCustomBlendMode.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa ComposeCustomBlendMode
 */
using BlendMode = SDL_BlendMode;

/**
 * no blending: dstRGBA = srcRGBA
 */
#define SDL_BLENDMODE_NONE 0x00000000u

/**
 * alpha blending: dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA)), dstA = srcA +
 * (dstA * (1-srcA))
 */
#define SDL_BLENDMODE_BLEND 0x00000001u

/**
 * pre-multiplied alpha blending: dstRGBA = srcRGBA + (dstRGBA * (1-srcA))
 */
#define SDL_BLENDMODE_BLEND_PREMULTIPLIED 0x00000010u

/**
 * additive blending: dstRGB = (srcRGB * srcA) + dstRGB, dstA = dstA
 */
#define SDL_BLENDMODE_ADD 0x00000002u

/**
 * pre-multiplied additive blending: dstRGB = srcRGB + dstRGB, dstA = dstA
 */
#define SDL_BLENDMODE_ADD_PREMULTIPLIED 0x00000020u

/**
 * color modulate: dstRGB = srcRGB * dstRGB, dstA = dstA
 */
#define SDL_BLENDMODE_MOD 0x00000004u

/**
 * color multiply: dstRGB = (srcRGB * dstRGB) + (dstRGB * (1-srcA)), dstA = dstA
 */
#define SDL_BLENDMODE_MUL 0x00000008u

#define SDL_BLENDMODE_INVALID 0x7FFFFFFFu

/**
 * The blend operation used when combining source and destination pixel
 * components.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using BlendOperation = SDL_BlendOperation;

constexpr BlendOperation BLENDOPERATION_ADD =
  SDL_BLENDOPERATION_ADD; ///< dst + src: supported by all renderers

/**
 * src - dst : supported by D3D, OpenGL, OpenGLES, and Vulkan
 */
constexpr BlendOperation BLENDOPERATION_SUBTRACT = SDL_BLENDOPERATION_SUBTRACT;

/**
 * dst - src : supported by D3D, OpenGL, OpenGLES, and Vulkan
 */
constexpr BlendOperation BLENDOPERATION_REV_SUBTRACT =
  SDL_BLENDOPERATION_REV_SUBTRACT;

/**
 * min(dst, src) : supported by D3D, OpenGL, OpenGLES, and Vulkan
 */
constexpr BlendOperation BLENDOPERATION_MINIMUM = SDL_BLENDOPERATION_MINIMUM;

/**
 * max(dst, src) : supported by D3D, OpenGL, OpenGLES, and Vulkan
 */
constexpr BlendOperation BLENDOPERATION_MAXIMUM = SDL_BLENDOPERATION_MAXIMUM;

/**
 * The normalized factor used to multiply pixel components.
 *
 * The blend factors are multiplied with the pixels from a drawing operation
 * (src) and the pixels from the render target (dst) before the blend
 * operation. The comma-separated factors listed above are always applied in
 * the component order red, green, blue, and alpha.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using BlendFactor = SDL_BlendFactor;

constexpr BlendFactor BLENDFACTOR_ZERO = SDL_BLENDFACTOR_ZERO; ///< 0, 0, 0, 0

constexpr BlendFactor BLENDFACTOR_ONE = SDL_BLENDFACTOR_ONE; ///< 1, 1, 1, 1

constexpr BlendFactor BLENDFACTOR_SRC_COLOR =
  SDL_BLENDFACTOR_SRC_COLOR; ///< srcR, srcG, srcB, srcA

constexpr BlendFactor BLENDFACTOR_ONE_MINUS_SRC_COLOR =
  SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR; ///< 1-srcR, 1-srcG, 1-srcB, 1-srcA

constexpr BlendFactor BLENDFACTOR_SRC_ALPHA =
  SDL_BLENDFACTOR_SRC_ALPHA; ///< srcA, srcA, srcA, srcA

constexpr BlendFactor BLENDFACTOR_ONE_MINUS_SRC_ALPHA =
  SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA; ///< 1-srcA, 1-srcA, 1-srcA, 1-srcA

constexpr BlendFactor BLENDFACTOR_DST_COLOR =
  SDL_BLENDFACTOR_DST_COLOR; ///< dstR, dstG, dstB, dstA

constexpr BlendFactor BLENDFACTOR_ONE_MINUS_DST_COLOR =
  SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR; ///< 1-dstR, 1-dstG, 1-dstB, 1-dstA

constexpr BlendFactor BLENDFACTOR_DST_ALPHA =
  SDL_BLENDFACTOR_DST_ALPHA; ///< dstA, dstA, dstA, dstA

constexpr BlendFactor BLENDFACTOR_ONE_MINUS_DST_ALPHA =
  SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA; ///< 1-dstA, 1-dstA, 1-dstA, 1-dstA

/**
 * Compose a custom blend mode for renderers.
 *
 * The functions SetRenderDrawBlendMode and SetTextureBlendMode accept
 * the BlendMode returned by this function if the renderer supports it.
 *
 * A blend mode controls how the pixels from a drawing operation (source) get
 * combined with the pixels from the render target (destination). First, the
 * components of the source and destination pixels get multiplied with their
 * blend factors. Then, the blend operation takes the two products and
 * calculates the result that will get stored in the render target.
 *
 * Expressed in pseudocode, it would look like this:
 *
 * ```c
 * dstRGB = colorOperation(srcRGB * srcColorFactor, dstRGB * dstColorFactor);
 * dstA = alphaOperation(srcA * srcAlphaFactor, dstA * dstAlphaFactor);
 * ```
 *
 * Where the functions `colorOperation(src, dst)` and `alphaOperation(src,
 * dst)` can return one of the following:
 *
 * - `src + dst`
 * - `src - dst`
 * - `dst - src`
 * - `min(src, dst)`
 * - `max(src, dst)`
 *
 * The red, green, and blue components are always multiplied with the first,
 * second, and third components of the BlendFactor, respectively. The
 * fourth component is not used.
 *
 * The alpha component is always multiplied with the fourth component of the
 * BlendFactor. The other components are not used in the alpha
 * calculation.
 *
 * Support for these blend modes varies for each renderer. To check if a
 * specific BlendMode is supported, create a renderer and pass it to
 * either SetRenderDrawBlendMode or SetTextureBlendMode. They will
 * return with an error if the blend mode is not supported.
 *
 * This list describes the support of custom blend modes for each renderer.
 * All renderers support the four blend modes listed in the BlendMode
 * enumeration.
 *
 * - **direct3d**: Supports all operations with all factors. However, some
 *   factors produce unexpected results with `BLENDOPERATION_MINIMUM` and
 *   `BLENDOPERATION_MAXIMUM`.
 * - **direct3d11**: Same as Direct3D 9.
 * - **opengl**: Supports the `BLENDOPERATION_ADD` operation with all
 *   factors. OpenGL versions 1.1, 1.2, and 1.3 do not work correctly here.
 * - **opengles2**: Supports the `BLENDOPERATION_ADD`,
 *   `BLENDOPERATION_SUBTRACT`, `BLENDOPERATION_REV_SUBTRACT`
 *   operations with all factors.
 * - **psp**: No custom blend mode support.
 * - **software**: No custom blend mode support.
 *
 * Some renderers do not provide an alpha component for the default render
 * target. The `BLENDFACTOR_DST_ALPHA` and
 * `BLENDFACTOR_ONE_MINUS_DST_ALPHA` factors do not have an effect in this
 * case.
 *
 * @param srcColorFactor the BlendFactor applied to the red, green, and
 *                       blue components of the source pixels.
 * @param dstColorFactor the BlendFactor applied to the red, green, and
 *                       blue components of the destination pixels.
 * @param colorOperation the BlendOperation used to combine the red,
 *                       green, and blue components of the source and
 *                       destination pixels.
 * @param srcAlphaFactor the BlendFactor applied to the alpha component of
 *                       the source pixels.
 * @param dstAlphaFactor the BlendFactor applied to the alpha component of
 *                       the destination pixels.
 * @param alphaOperation the BlendOperation used to combine the alpha
 *                       component of the source and destination pixels.
 * @returns an BlendMode that represents the chosen factors and
 *          operations.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetRenderDrawBlendMode
 * @sa GetRenderDrawBlendMode
 * @sa SetTextureBlendMode
 * @sa GetTextureBlendMode
 */
inline BlendMode ComposeCustomBlendMode(BlendFactor srcColorFactor,
                                        BlendFactor dstColorFactor,
                                        BlendOperation colorOperation,
                                        BlendFactor srcAlphaFactor,
                                        BlendFactor dstAlphaFactor,
                                        BlendOperation alphaOperation)
{
  return SDL_ComposeCustomBlendMode(srcColorFactor,
                                    dstColorFactor,
                                    colorOperation,
                                    srcAlphaFactor,
                                    dstAlphaFactor,
                                    alphaOperation);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_BLENDMODE_H_ */
