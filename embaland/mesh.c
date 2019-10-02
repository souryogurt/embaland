/**
 * @file
 * Mesh implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "compiler.h"
#include "embaland.h"
#include "instance.h"
#include "mesh.h"

static void emb_mesh_destroy(struct emb_object *obj)
{
	struct emb_mesh *mesh = container_of(obj, struct emb_mesh, obj);
	free(mesh);
}

static const struct emb_type mesh_type = {
	.release = emb_mesh_destroy,
};

EMB_API enum emb_result EMB_CALL emb_mesh_create(
	emb_instance embaland, const struct emb_mesh_info *info, emb_mesh *mesh)
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
	if (mesh == NULL) {
		result = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}

	struct emb_mesh *new_mesh = malloc(sizeof(*new_mesh));
	if (new_mesh == NULL) {
		result = EMB_ERROR_OUT_OF_HOST_MEMORY;
		goto err;
	}
	emb_object_init(&new_mesh->obj, &mesh_type);
	emb_object_add(&new_mesh->obj, &embaland->obj);

	*mesh = new_mesh;
err:
	return result;
}

EMB_API void EMB_CALL emb_mesh_release(emb_mesh mesh)
{
	if (mesh != NULL) {
		emb_object_put(&mesh->obj);
	}
}
