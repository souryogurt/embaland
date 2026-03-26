/**
 * @file
 * Triangle scene implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "scene.h"

#include <stdint.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
static const float points[] = {
	0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F,
};
static const uint16_t indeces[] = { 0, 1, 2 };

enum emb_result scene_init(emb_instance emb, struct scene *scn)
{
	enum emb_result retval = EMB_SUCCESS;

	retval = emb_buffer_init(&scn->vertex_buffer, emb, points,
				 sizeof(points));
	if (retval != EMB_SUCCESS) {
		goto err;
	}

	retval = emb_buffer_view_init(&scn->vertex_bview, &scn->vertex_buffer, 0,
				      sizeof(points), sizeof(points[0]) * 3U,
				      EMB_ARRAY_BUFFER);
	if (retval != EMB_SUCCESS) {
		goto err_destroy_vertex_buffer;
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
	retval = emb_accessor_init(&scn->positions, &scn->vertex_bview, &pos_info);
	if (retval != EMB_SUCCESS) {
		goto err_destroy_vertex_bview;
	}

	retval = emb_buffer_init(&scn->index_buffer, emb, indeces,
				 sizeof(indeces));
	if (retval != EMB_SUCCESS) {
		goto err_destroy_positions;
	}

	retval = emb_buffer_view_init(&scn->index_bview, &scn->index_buffer, 0,
				      sizeof(indeces), 0,
				      EMB_ELEMENT_ARRAY_BUFFER);
	if (retval != EMB_SUCCESS) {
		goto err_destroy_index_buffer;
	}

	const struct emb_accessor_info index_info = {
		.byte_offset = 0,
		.comp_type = EMB_ATTRIB_COMPONENT_USHORT,
		.is_normalized = 0,
		.attrib_count = ARRAY_SIZE(indeces),
		.attrib_type = EMB_ATTRIB_SCALAR,
		.min_values = NULL,
		.max_values = NULL,
	};
	retval = emb_accessor_init(&scn->indeces, &scn->index_bview, &index_info);
	if (retval != EMB_SUCCESS) {
		goto err_destroy_index_bview;
	}

	scn->tri_attribute = (const struct emb_attribute){
		.type = EMB_ATTRIB_POSITION,
		.data = &scn->positions,
	};
	scn->tri_primitive = (const struct emb_primitive){
		.attributes = &scn->tri_attribute,
		.attributes_count = 1,
		.indeces = &scn->indeces,
		.material = NULL,
		.mode = EMB_MODE_TRIANGLES,
		.targets = NULL,
		.targets_count = 0,
	};
	const struct emb_mesh_info mesh_info = {
		.primitives = &scn->tri_primitive,
		.primitives_count = 1,
		.weights = NULL,
		.weights_count = 0,
	};
	retval = emb_mesh_init(&scn->triangle, emb, &mesh_info);
	if (retval != EMB_SUCCESS) {
		goto err_destroy_indeces;
	}
	return retval;
err_destroy_indeces:
	emb_accessor_destroy(&scn->indeces);
err_destroy_index_bview:
	emb_buffer_view_destroy(&scn->index_bview);
err_destroy_index_buffer:
	emb_buffer_destroy(&scn->index_buffer);
err_destroy_positions:
	emb_accessor_destroy(&scn->positions);
err_destroy_vertex_bview:
	emb_buffer_view_destroy(&scn->vertex_bview);
err_destroy_vertex_buffer:
	emb_buffer_destroy(&scn->vertex_buffer);
err:
	return retval;
}

void scene_destroy(struct scene *scn)
{
	emb_mesh_destroy(&scn->triangle);
	emb_accessor_destroy(&scn->indeces);
	emb_buffer_view_destroy(&scn->index_bview);
	emb_buffer_destroy(&scn->index_buffer);
	emb_accessor_destroy(&scn->positions);
	emb_buffer_view_destroy(&scn->vertex_bview);
	emb_buffer_destroy(&scn->vertex_buffer);
}
