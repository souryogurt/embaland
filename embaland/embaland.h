#ifndef EMBALAND_EMBALAND_H
#define EMBALAND_EMBALAND_H 1

#include "platform.h"

#include <vulkan/vulkan_core.h>

/**
 * The embaland API result.
 */
enum emb_result {
	EMB_SUCCESS = 0,
	EMB_ERROR_OUT_OF_HOST_MEMORY = -1,
	EMB_ERROR_INITIALIZATION_FAILED = -2,
	EMB_ERROR_INVALID_EXTERNAL_HANDLE = -3,
};

enum emb_buffer_target {
	EMB_ARRAY_BUFFER = 34962,
	EMB_ELEMENT_ARRAY_BUFFER = 34963,
};

/**
 * Opaque embaland instance handle.
 */
typedef struct emb_instance *emb_instance;

/**
 * Opaque viewport handle.
 */
typedef struct emb_viewport *emb_viewport;

/**
 * Opaque buffer handle.
 */
typedef struct emb_buffer *emb_buffer;

/**
 * Opaque buffer view handle.
 */
typedef struct emb_buffer_view *emb_buffer_view;

/**
 * Opaque accessor handle.
 */
typedef struct emb_accessor *emb_accessor;

/**
 * Opaque material handle.
 */
typedef struct emb_material *emb_material;

/**
 * Opaque mesh handle.
 */
typedef struct emb_mesh *emb_mesh;

/**
 * Accessor parameters
 */
struct emb_accessor_info {
	/** The offset relative to the start of the buffer view in bytes */
	uint32_t byte_offset;
	/** The datatype of components in the attribute */
	uint32_t comp_type;
	/** Specifies whether integer data values should be normalized */
	uint8_t is_normalized;
	/** The number of attributes referenced by this accessor */
	uint32_t attrib_count;
	/** Specifies if the attribute is a scalar, vector, or matrix */
	uint32_t attrib_type;
	/** Minimum value of each component in this attribute */
	const void *min_values;
	/** Maximum value of each component in this attribute */
	const void *max_values;
};

/**
 * Attribute connection
 */
struct emb_attribute {
	uint32_t type;
	emb_accessor data;
};

/**
 * Primitive parameters
 */
struct emb_primitive {
	/** An array of attribute-data connections */
	const struct emb_attribute *attributes;
	/** Number of attributes in @a attributes array */
	uint32_t attributes_count;
	/** The accessor that contains the indices */
	emb_accessor indeces;
	/** The material to apply to this primitive when rendering */
	emb_material material;
	/** The type of primitives to render */
	uint32_t mode;
	/** An array of Morph Targets */
	const struct emb_attribute *targets;
	/** Number of elements in @a targets array */
	uint32_t targets_count;
};

/**
 * Mesh parameters
 */
struct emb_mesh_info {
	/** An array of primitives, each defining geometry to be rendered */
	const struct emb_primitive *primitives;
	/** The number of primitves in @a primitives array */
	uint32_t primitives_count;
	/** An array of weights to be applied to the Morph Targets */
	const float *weights;
	/** The number of weights in @a weights array */
	uint32_t weights_count;
};

#define EMB_ATTRIB_COMPONENT_BYTE 5120
#define EMB_ATTRIB_COMPONENT_UBYTE 5121
#define EMB_ATTRIB_COMPONENT_SHORT 5122
#define EMB_ATTRIB_COMPONENT_USHORT 5123
#define EMB_ATTRIB_COMPONENT_UINT 5125
#define EMB_ATTRIB_COMPONENT_FLOAT 5126

#define EMB_ATTRIB_SCALAR 0
#define EMB_ATTRIB_VEC2 1
#define EMB_ATTRIB_VEC3 2
#define EMB_ATTRIB_VEC4 3
#define EMB_ATTRIB_MAT2 4
#define EMB_ATTRIB_MAT3 5
#define EMB_ATTRIB_MAT4 6

#define EMB_MODE_POINTS 0
#define EMB_MODE_LINES 1
#define EMB_MODE_LINE_LOOP 2
#define EMB_MODE_LINE_STRIP 3
#define EMB_MODE_TRIANGLES 4
#define EMB_MODE_TRIANGLE_STRIP 5
#define EMB_MODE_TRIANGLE_FAN 6

#define EMB_ATTRIB_POSITION 0
#define EMB_ATTRIB_NORMAL 1
#define EMB_ATTRIB_TANGENT 2
#define EMB_ATTRIB_TEXCOORD_0 3
#define EMB_ATTRIB_TEXCOORD_1 4
#define EMB_ATTRIB_TEXCOORD_2 5
#define EMB_ATTRIB_TEXCOORD_3 6
#define EMB_ATTRIB_TEXCOORD_4 7
#define EMB_ATTRIB_TEXCOORD_5 8
#define EMB_ATTRIB_TEXCOORD_6 9
#define EMB_ATTRIB_TEXCOORD_7 10
#define EMB_ATTRIB_TEXCOORD_8 11
#define EMB_ATTRIB_TEXCOORD_9 12
#define EMB_ATTRIB_TEXCOORD_10 13
#define EMB_ATTRIB_TEXCOORD_11 14
#define EMB_ATTRIB_TEXCOORD_12 15
#define EMB_ATTRIB_TEXCOORD_13 16
#define EMB_ATTRIB_TEXCOORD_14 17
#define EMB_ATTRIB_TEXCOORD_15 18
#define EMB_ATTRIB_COLOR_0 19
#define EMB_ATTRIB_COLOR_1 20
#define EMB_ATTRIB_COLOR_2 21
#define EMB_ATTRIB_COLOR_3 22
#define EMB_ATTRIB_COLOR_4 23
#define EMB_ATTRIB_COLOR_5 24
#define EMB_ATTRIB_COLOR_6 25
#define EMB_ATTRIB_COLOR_7 26
#define EMB_ATTRIB_COLOR_8 27
#define EMB_ATTRIB_COLOR_9 28
#define EMB_ATTRIB_COLOR_10 29
#define EMB_ATTRIB_COLOR_11 30
#define EMB_ATTRIB_COLOR_12 31
#define EMB_ATTRIB_COLOR_13 32
#define EMB_ATTRIB_COLOR_14 33
#define EMB_ATTRIB_COLOR_15 34

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create embaland instance.
 * @param vulkan is the handle of the vulkan instance to use
 * @param[out] embaland points a emb_instance handle in which the resulting instance is returned
 * @retval EMB_SUCCESS embaland instance succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED target pointer is null
 * @retval EMB_ERROR_INVALID_EXTERNAL_HANDLE vulkan handle is invalid
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_destroy()
 */
EMB_API enum emb_result EMB_CALL emb_create(VkInstance vulkan,
					    emb_instance *embaland);

/**
 * Destroy embaland instance.
 * @param embaland is the handle of the instance to destroy
 */
EMB_API void EMB_CALL emb_destroy(emb_instance embaland);

/**
 * Create embaland viewport.
 * @param embaland is handle of the embaland instance
 * @param surface is vulkan surface to create viewport on
 * @param[out] viewport points a emb_viewport handle in which the resulting
 *                      viewport is returned
 * @retval EMB_SUCCESS viewport succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED instance handle or target are null
 * @retval EMB_ERROR_INVALID_EXTERNAL_HANDLE surface handle is invalid
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_viewport_release()
 */
EMB_API enum emb_result EMB_CALL emb_viewport_create(emb_instance embaland,
						     VkSurfaceKHR surface,
						     emb_viewport *viewport);

/**
 * Release reference to embaland viewport.
 * @param viewport is the handle of the viewport to release
 */
EMB_API void EMB_CALL emb_viewport_release(emb_viewport viewport);

/** Render viewport.
 * @param viewport is handle of the viewport to render
 * @param timeout specifies how long the function can wait in nanoseconds
 * @retval EMB_SUCCESS viewport is successfully rendered
 */
EMB_API enum emb_result EMB_CALL emb_viewport_render(emb_viewport viewport,
						     uint64_t timeout);

/**
 * Create embaland buffer.
 * @param embaland is handle of the embaland instance
 * @param bytes is a pointer to initial buffer's data
 * @param nbytes is the size in bytes of @data
 * @param[out] buffer points a emb_buffer handle in which the resulting
 *                    buffer is returned
 * @retval EMB_SUCCESS buffer succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED instance handle or target are null
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_buffer_release()
 */
EMB_API enum emb_result EMB_CALL emb_buffer_create(emb_instance embaland,
						   const void *bytes,
						   size_t nbytes,
						   emb_buffer *buffer);
/**
 * Release embaland buffer.
 * @param buffer is the handle of buffer to release
 */
EMB_API void EMB_CALL emb_buffer_release(emb_buffer buffer);

/**
 * Create embaland buffer view.
 * @param buffer is handle of the embaland buffer to create view for
 * @param offset is offset in bytes in buffer
 * @param nbytes is the size in bytes of the view
 * @param stride is the size in bytes between vertex attributes
 * @param target is the target that the GPU buffer should be bound to
 * @param[out] bview points a emb_buffer_view handle in which the resulting
 *                   buffer view is returned
 * @retval EMB_SUCCESS buffer view succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED instance handle or target are null
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_buffer_view_release()
 */
EMB_API enum emb_result EMB_CALL emb_buffer_view_create(
	emb_buffer buffer, size_t offset, size_t nbytes, uint8_t stride,
	enum emb_buffer_target target, emb_buffer_view *bview);
/**
 * Release embaland buffer view.
 * @param bview is the handle of buffer view to release
 */
EMB_API void EMB_CALL emb_buffer_view_release(emb_buffer_view bview);

/**
 * Create data accessor.
 * @param bview is buffer view to access data from
 * @param[in] info is accessor parameters info
 * @param[out] acsr points to emb_accessor handle in which the resulting
 *                  accessor is returned
 * @retval EMB_SUCCESS accessor succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED buffer view handle is NULL
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_accessor_release()
 */
EMB_API enum emb_result EMB_CALL
emb_accessor_create(emb_buffer_view bview, const struct emb_accessor_info *info,
		    emb_accessor *acsr);

/**
 * Release data accessor.
 * @param acsr is the handle of accessor to release
 */
EMB_API void EMB_CALL emb_accessor_release(emb_accessor acsr);

/**
 * Create mesh.
 * @param embaland is handle of the embaland instance
 * @param[in] info is mesh parameters info
 * @param[out] mesh points to emb_mesh handle in which the resulting mesh is
 *                  returned
 * @retval EMB_SUCCESS mesh succefully created
 * @retval EMB_ERROR_INITIALIZATION_FAILED instance handle or info are null
 * @retval EMB_ERROR_OUT_OF_HOST_MEMORY can't allocate memory
 * @sa emb_mesh_release()
 */
EMB_API enum emb_result EMB_CALL
emb_mesh_create(emb_instance embaland, const struct emb_mesh_info *info,
		emb_mesh *mesh);

/**
 * Release mesh.
 * @param mesh is the handle of mesh to release
 */
EMB_API void EMB_CALL emb_mesh_release(emb_mesh mesh);

/**
 * Release material
 * @param mat is the handle of material to release
 */
EMB_API void EMB_CALL emb_material_release(emb_material mat);

#ifdef __cplusplus
}
#endif
#endif
