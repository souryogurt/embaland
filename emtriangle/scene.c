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

	retval = emb_buffer_create(emb, points, sizeof(points), &scn->buffer);
	if (retval != EMB_SUCCESS) {
		goto err;
	}

	retval = emb_buffer_view_create(scn->buffer, 0, sizeof(points),
					sizeof(points[0]), EMB_ARRAY_BUFFER,
					&scn->bview);
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
	retval = emb_accessor_create(scn->bview, &pos_info, &scn->positions);
	if (retval != EMB_SUCCESS) {
		goto err_release_bview;
	}
	return retval;
err_release_bview:
	emb_buffer_view_release(scn->bview);
err_release_buffer:
	emb_buffer_release(scn->buffer);
err:
	return retval;
}

void scene_release(const struct scene *scn)
{
	emb_accessor_release(scn->positions);
	emb_buffer_view_release(scn->bview);
	emb_buffer_release(scn->buffer);
}
