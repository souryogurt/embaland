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
	return retval;
err_release_buffer:
	emb_buffer_release(scn->buffer);
err:
	return retval;
}

void scene_release(const struct scene *scn)
{
	emb_buffer_view_release(scn->bview);
	emb_buffer_release(scn->buffer);
}
