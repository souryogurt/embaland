/**
 * @file
 * Mesh implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "embaland.h"
#include "instance.h"
#include "mesh.h"
#include "accessor.h"
#include "material.h"

EMB_API enum emb_result EMB_CALL emb_mesh_init(emb_mesh mesh,
					       emb_instance embaland,
					       const struct emb_mesh_info *info)
{
	enum emb_result result = EMB_SUCCESS;
	if (embaland == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	if (info == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	if (info->primitives == NULL || info->primitives_count == 0) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	if (mesh == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}

	mesh->emb = embaland;
	mesh->primitives = info->primitives;
	mesh->primitives_count = info->primitives_count;
	mesh->weights = info->weights;
	mesh->weights_count = info->weights_count;
err:
	return result;
}

EMB_API void EMB_CALL emb_mesh_destroy(emb_mesh mesh)
{
	(void)mesh;
}
