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
	if (embaland == NULL) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (info == NULL) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (info->primitives == NULL || info->primitives_count == 0) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (mesh == NULL) {
		return EMB_ERROR_INITIALIZATION_FAILED;
	}
	if (embaland->device == VK_NULL_HANDLE) {
		return EMB_ERROR_INVALID_EXTERNAL_HANDLE;
	}

	memset(mesh, 0, sizeof(*mesh));
	mesh->emb = embaland;
	list_init(&mesh->scene_node);
	mesh->primitives = info->primitives;
	mesh->primitives_count = info->primitives_count;
	mesh->weights = info->weights;
	mesh->weights_count = info->weights_count;
	list_add_tail(&embaland->meshes, &mesh->scene_node);
	return EMB_SUCCESS;
}

EMB_API void EMB_CALL emb_mesh_destroy(emb_mesh mesh)
{
	if (mesh == NULL) {
		return;
	}
	if (mesh->emb != NULL) {
		list_del_init(&mesh->scene_node);
	}
	mesh->weights = NULL;
	mesh->weights_count = 0;
	mesh->primitives = NULL;
	mesh->primitives_count = 0;
	mesh->emb = NULL;
}
