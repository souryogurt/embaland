/**
 * @file
 * Buffer view implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "compiler.h"
#include "embaland.h"
#include "buffer.h"
#include "buffer_view.h"

static void emb_buffer_view_destroy(struct emb_object *obj)
{
	struct emb_buffer_view *bview =
		container_of(obj, struct emb_buffer_view, obj);
	emb_buffer_release(bview->buffer);
	free(bview);
}

static const struct emb_type buffer_view_type = {
	.release = emb_buffer_view_destroy,
};

EMB_API enum emb_result EMB_CALL emb_buffer_view_create(
	emb_buffer buffer, size_t offset, size_t nbytes, uint8_t stride,
	enum emb_buffer_target target, emb_buffer_view *bview)
{
	enum emb_result result = EMB_SUCCESS;
	if (buffer == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	if (bview == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	if ((offset + nbytes) > buffer->capacity) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}

	struct emb_buffer_view *new_bview = malloc(sizeof(*new_bview));
	if (new_bview == NULL) {
		result = EMB_ERROR_OUT_OF_HOST_MEMORY;
		goto err;
	}
	emb_object_init(&new_bview->obj, &buffer_view_type);
	emb_object_add(&new_bview->obj, buffer->obj.parent);

	emb_object_get(&buffer->obj);
	new_bview->buffer = buffer;
	new_bview->offset = offset;
	new_bview->nbytes = nbytes;
	new_bview->stride = stride;
	new_bview->target = target;
	*bview = new_bview;
err:
	return result;
}

EMB_API void EMB_CALL emb_buffer_view_release(emb_buffer_view bview)
{
	if (bview != NULL) {
		emb_object_put(&bview->obj);
	}
}
