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
#include "buffer.h"

EMB_API enum emb_result EMB_CALL emb_buffer_init(emb_buffer buffer,
						 emb_instance embaland,
						 const void *bytes,
						 size_t nbytes)
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

	/* TODO: Copy bytes to internal buffer */
	buffer->capacity = nbytes;
	buffer->emb = embaland;
err:
	return result;
}

EMB_API void EMB_CALL emb_buffer_release(emb_buffer buffer)
{
	(void)buffer;
}
