/**
 * @file
 * Object implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "object.h"
#include "compiler.h"
#include <assert.h>

EMB_LOCAL void emb_object_init(struct emb_object *obj, struct emb_type *type)
{
	assert(obj != NULL);
	assert(type != NULL);
	assert(type->release != NULL);
	emb_ref_init(&obj->ref);
	list_init(&obj->entry);
	obj->type = type;
}

EMB_LOCAL struct emb_object *emb_object_get(struct emb_object *obj)
{
	assert(obj != NULL);
	emb_ref_get(&obj->ref);
	return obj;
}

static void object_release(struct emb_ref *ref)
{
	assert(ref != NULL);
	struct emb_object *obj = container_of(ref, struct emb_object, ref);
	struct emb_type *type = obj->type;
	type->release(obj);
}

EMB_LOCAL void emb_object_put(struct emb_object *obj)
{
	assert(obj != NULL);
	emb_ref_put(&obj->ref, object_release);
}
