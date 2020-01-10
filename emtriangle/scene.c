/**
 * @file
 * Triangle scene implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "scene.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
static const float points[] = {
	0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F,
};

enum emb_result scene_init(emb_instance emb, struct scene *scn)
{
	enum emb_result retval = EMB_SUCCESS;

	retval = emb_buffer_init(&scn->buffer, emb, points, sizeof(points));
	if (retval != EMB_SUCCESS) {
		goto err;
	}

	retval = emb_buffer_view_init(&scn->bview, &scn->buffer, 0,
				      sizeof(points), sizeof(points[0]),
				      EMB_ARRAY_BUFFER);
	if (retval != EMB_SUCCESS) {
		goto err_release_buffer;
	}
	const float min_pos[] = { 0.0F, 0.0F, 0.0F };
	const float max_pos[] = { 1.0F, 1.0F, 0.0F };
	const struct emb_accessor_info pos_info = {
		.byte_offset = 0,
		.comp_type = EMB_ATTRIB_COMPONENT_FLOAT,
		.is_normalized = 0,
		.attrib_count = 3,
		.attrib_type = EMB_ATTRIB_VEC3,
		.min_values = min_pos,
		.max_values = max_pos,
	};
	retval = emb_accessor_init(&scn->positions, &scn->bview, &pos_info);
	if (retval != EMB_SUCCESS) {
		goto err_release_bview;
	}

	const struct emb_attribute tri_attribute = {
		.type = EMB_ATTRIB_POSITION,
		.data = &scn->positions,
	};
	const struct emb_primitive tri_primitive = {
		.attributes = &tri_attribute,
		.attributes_count = 1,
		.indeces = NULL,
		.material = NULL,
		.mode = EMB_MODE_TRIANGLES,
		.targets = NULL,
		.targets_count = 0,
	};
	const struct emb_mesh_info mesh_info = {
		.primitives = &tri_primitive,
		.primitives_count = 1,
		.weights = NULL,
		.weights_count = 0,
	};
	retval = emb_mesh_create(emb, &mesh_info, &scn->triangle);
	if (retval != EMB_SUCCESS) {
		goto err_release_accessor;
	}
	return retval;
err_release_accessor:
	emb_accessor_release(&scn->positions);
err_release_bview:
	emb_buffer_view_release(&scn->bview);
err_release_buffer:
	emb_buffer_release(&scn->buffer);
err:
	return retval;
}

void scene_release(struct scene *scn)
{
	emb_mesh_release(scn->triangle);
	emb_accessor_release(&scn->positions);
	emb_buffer_view_release(&scn->bview);
	emb_buffer_release(&scn->buffer);
}
