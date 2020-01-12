/**
 * @file
 * Attributes accessor implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "compiler.h"
#include "embaland.h"
#include "buffer_view.h"
#include "accessor.h"

EMB_API enum emb_result EMB_CALL
emb_accessor_init(emb_accessor acsr, emb_buffer_view bview,
		  const struct emb_accessor_info *info)
{
	enum emb_result result = EMB_SUCCESS;
	if (bview == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	if (info == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}

	acsr->emb = bview->emb;
	acsr->bview = bview;
	acsr->byte_offset = info->byte_offset;
	acsr->comp_type = info->comp_type;
	acsr->is_normalized = info->is_normalized;
	acsr->attrib_count = info->attrib_count;
	acsr->attrib_type = info->attrib_type;
err:
	return result;
}

EMB_API void EMB_CALL emb_accessor_destroy(emb_accessor acsr)
{
	(void)acsr;
}
