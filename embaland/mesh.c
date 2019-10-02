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

static void emb_mesh_destroy(struct emb_object *obj)
{
	struct emb_mesh *mesh = container_of(obj, struct emb_mesh, obj);
	free(mesh);
}

static const struct emb_type mesh_type = {
	.release = emb_mesh_destroy,
};

static enum emb_result emb_primitive_init(struct emb_primitive *primitive,
					  const struct emb_primitive *source)
{
	enum emb_result retval = EMB_SUCCESS;
	size_t a_size = sizeof(*source->attributes) * source->attributes_count;
	if (a_size == 0) {
		retval = EMB_ERROR_INITIALIZATION_FAILED;
		goto err;
	}
	struct emb_attribute *attributes = malloc(a_size);
	if (attributes == NULL) {
		retval = EMB_ERROR_OUT_OF_HOST_MEMORY;
		goto err;
	}
	memcpy(attributes, source->attributes, a_size);
	for (size_t i = 0; i < source->attributes_count; i++) {
		emb_object_get(&attributes[i].data->obj);
	}
	primitive->attributes = attributes;
	primitive->attributes_count = source->attributes_count;

	primitive->targets = NULL;
	primitive->targets_count = 0;
	size_t t_size = sizeof(*primitive->targets) * source->targets_count;
	if (t_size > 0) {
		struct emb_attribute *targets = malloc(t_size);
		if (targets == NULL) {
			retval = EMB_ERROR_OUT_OF_HOST_MEMORY;
			goto err_free_attributes;
		}
		memcpy(targets, source->targets, t_size);
		for (size_t i = 0; i < source->targets_count; i++) {
			emb_object_get(&targets[i].data->obj);
		}
		primitive->targets = targets;
		primitive->targets_count = source->targets_count;
	}

	primitive->indeces = source->indeces;
	if (primitive->indeces != NULL) {
		emb_object_get(&primitive->indeces->obj);
	}
	primitive->material = source->material;
	if (primitive->material != NULL) {
		emb_object_get(&primitive->material->obj);
	}
	primitive->mode = source->mode;

	return retval;
err_free_attributes:
	for (size_t i = 0; i < primitive->attributes_count; i++) {
		emb_object_put(&primitive->attributes[i].data->obj);
	}
	free((void *)primitive->attributes);
err:
	return retval;
}

static void emb_primitive_destroy(const struct emb_primitive *primitive)
{
}

static enum emb_result
emb_mesh_primitives_init(struct emb_primitive *primitives,
			 const struct emb_mesh_info *info)
{
	enum emb_result retval = EMB_SUCCESS;
	size_t idx = 0;
	for (idx = 0; idx < info->primitives_count; idx++) {
		struct emb_primitive *primitive = &primitives[idx];
		const struct emb_primitive *source = &info->primitives[idx];
		retval = emb_primitive_init(primitive, source);
		if (retval != EMB_SUCCESS) {
			goto err;
		}
	}
	return retval;
err:
	for (size_t i = 0; i < idx; i++) {
		emb_primitive_destroy(&primitives[i]);
	}
	return retval;
}

static enum emb_result emb_mesh_init(struct emb_mesh *mesh,
				     const struct emb_mesh_info *info)
{
	enum emb_result retval = EMB_SUCCESS;
	mesh->weights = NULL;

	size_t pri_size = sizeof(*mesh->primitives) * info->primitives_count;
	struct emb_primitive *primitives = malloc(pri_size);
	if (primitives == NULL) {
		retval = EMB_ERROR_OUT_OF_HOST_MEMORY;
		goto err;
	}
	retval = emb_mesh_primitives_init(primitives, info);
	if (retval != EMB_SUCCESS) {
		goto err_free_primitives;
	}
	mesh->primitives = primitives;
	mesh->primitives_count = info->primitives_count;

	size_t weights_size = sizeof(*mesh->weights) * info->weights_count;
	if (weights_size > 0) {
		float *weights = malloc(weights_size);
		if (weights == NULL) {
			goto err_destroy_primitives;
		}
		memcpy(weights, info->weights, weights_size);
		mesh->weights = weights;
	}
	mesh->weights_count = info->weights_count;

	return retval;
err_destroy_primitives:
	for (size_t i = 0; i < mesh->primitives_count; i++) {
		emb_primitive_destroy(&mesh->primitives[i]);
	}
err_free_primitives:
	free((void *)mesh->primitives);
err:
	return retval;
}

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
	if (info->primitives == NULL || info->primitives_count == 0) {
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

	result = emb_mesh_init(new_mesh, info);
	if (result != EMB_SUCCESS) {
		goto err_free_mesh;
	}

	*mesh = new_mesh;
	return result;
err_free_mesh:
	emb_object_del(&new_mesh->obj);
	free(new_mesh);
err:
	return result;
}

EMB_API void EMB_CALL emb_mesh_release(emb_mesh mesh)
{
	if (mesh != NULL) {
		emb_object_put(&mesh->obj);
	}
}
