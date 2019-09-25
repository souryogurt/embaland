/**
 * @file
 * Object implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>

#include "compiler.h"
#include "object.h"

EMB_LOCAL void emb_object_init(struct emb_object *obj,
			       const struct emb_type *type)
{
	assert(obj != NULL);
	assert(type != NULL);
	assert(type->release != NULL);
	emb_ref_init(&obj->ref);
	obj->type = type;
	obj->parent = NULL;
}

EMB_LOCAL struct emb_object *emb_object_get(struct emb_object *obj)
{
	if (obj != NULL) {
		emb_ref_get(&obj->ref);
	}
	return obj;
}

static void object_release(struct emb_ref *ref)
{
	assert(ref != NULL);
	struct emb_object *obj = container_of(ref, struct emb_object, ref);
	const struct emb_type *type = obj->type;
	struct emb_object *parent = obj->parent;
	type->release(obj);
	if (parent != NULL) {
		emb_object_put(parent);
	}
}

EMB_LOCAL void emb_object_put(struct emb_object *obj)
{
	assert(obj != NULL);
	emb_ref_put(&obj->ref, object_release);
}

EMB_LOCAL void emb_object_add(struct emb_object *obj, struct emb_object *parent)
{
	assert(obj != NULL);
	assert(parent != NULL);
	emb_object_get(parent);
	if (obj->parent != NULL) {
		emb_object_put(obj->parent);
	}
	obj->parent = parent;
}

EMB_LOCAL void emb_object_del(struct emb_object *obj)
{
	assert(obj != NULL);
	if (obj->parent != NULL) {
		emb_object_put(obj->parent);
		obj->parent = NULL;
	}
}
