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

static void emb_accessor_destroy(struct emb_object *obj)
{
	struct emb_accessor *acsr = container_of(obj, struct emb_accessor, obj);
	emb_buffer_view_release(acsr->bview);
	free(acsr);
}

static const struct emb_type accessor_type = {
	.release = emb_accessor_destroy,
};

EMB_API enum emb_result EMB_CALL
emb_accessor_create(emb_buffer_view bview, const struct emb_accessor_info *info,
		    emb_accessor *acsr)
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

	struct emb_accessor *new_acsr = malloc(sizeof(*new_acsr));
	if (new_acsr == NULL) {
		result = EMB_ERROR_OUT_OF_HOST_MEMORY;
		goto err;
	}
	emb_object_init(&new_acsr->obj, &accessor_type);
	emb_object_add(&new_acsr->obj, bview->obj.parent);

	emb_object_get(&bview->obj);
	new_acsr->bview = bview;
	new_acsr->byte_offset = info->byte_offset;
	new_acsr->comp_type = info->comp_type;
	new_acsr->is_normalized = info->is_normalized;
	new_acsr->attrib_count = info->attrib_count;
	new_acsr->attrib_type = info->attrib_type;
	*acsr = new_acsr;
err:
	return result;
}

EMB_API void EMB_CALL emb_accessor_release(emb_accessor acsr)
{
	if (acsr != NULL) {
		emb_object_put(&acsr->obj);
	}
}

EMB_LOCAL emb_accessor emb_accessor_get(emb_accessor acsr)
{
	if (acsr != NULL) {
		emb_object_get(&acsr->obj);
	}
	return acsr;
}
