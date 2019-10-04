/**
 * @file
 * Material implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "compiler.h"
#include "embaland.h"
#include "material.h"

static void emb_material_destroy(struct emb_object *obj)
{
	struct emb_material *mat = container_of(obj, struct emb_material, obj);
	free(mat);
}

static const struct emb_type material_type = {
	.release = emb_material_destroy,
};

EMB_API void EMB_CALL emb_material_release(emb_material mat)
{
	if (mat != NULL) {
		emb_object_put(&mat->obj);
	}
}

EMB_LOCAL emb_material emb_material_get(emb_material mat)
{
	if (mat != NULL) {
		emb_object_get(&mat->obj);
	}
	return mat;
}
