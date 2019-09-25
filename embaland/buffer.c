/**
 * @file
 * Buffer implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "compiler.h"
#include "embaland.h"
#include "instance.h"
#include "buffer.h"

static void emb_buffer_destroy(struct emb_object *obj)
{
	struct emb_buffer *buf = container_of(obj, struct emb_buffer, obj);
	free(buf);
}

static const struct emb_type buffer_type = {
	.release = emb_buffer_destroy,
};

EMB_API enum emb_result EMB_CALL emb_buffer_create(emb_instance embaland,
						   const void *bytes,
						   size_t nbytes,
						   emb_buffer *buffer)
{
	enum emb_result result = EMB_SUCCESS;
	if (embaland == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	if (buffer == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}

	struct emb_buffer *new_buffer = malloc(sizeof(*new_buffer));
	if (new_buffer == NULL) {
		result = EMB_ERROR_OUT_OF_HOST_MEMORY;
		goto err;
	}
	emb_object_init(&new_buffer->obj, &buffer_type);
	emb_object_add(&new_buffer->obj, &embaland->obj);

	/* TODO: Copy bytes to internal buffer */
	new_buffer->capacity = nbytes;
	*buffer = new_buffer;
err:
	return result;
}

EMB_API void EMB_CALL emb_buffer_release(emb_buffer buffer)
{
	if (buffer != NULL) {
		emb_object_put(&buffer->obj);
	}
}
