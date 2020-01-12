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

EMB_API enum emb_result EMB_CALL emb_buffer_view_init(
	emb_buffer_view bview, emb_buffer buffer, size_t offset, size_t nbytes,
	uint8_t stride, enum emb_buffer_target target)
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

	bview->emb = buffer->emb;
	bview->buffer = buffer;
	bview->offset = offset;
	bview->nbytes = nbytes;
	bview->stride = stride;
	bview->target = target;
err:
	return result;
}

EMB_API void EMB_CALL emb_buffer_view_destroy(emb_buffer_view bview)
{
	(void)bview;
}
