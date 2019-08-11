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

EMB_LOCAL void emb_object_init(struct emb_object *obj, struct emb_type *type)
{
	assert(obj != NULL);
	assert(type != NULL);
	assert(type->release != NULL);
	emb_ref_init(&obj->ref);
	list_init(&obj->entry);
	obj->type = type;
	obj->parent = NULL;
	obj->set = NULL;
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
	emb_object_leave_set(obj);
	emb_object_del(obj);
	type->release(obj);
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

EMB_LOCAL void emb_set_init(struct emb_set *set, struct emb_type *type)
{
	assert(set != NULL);
	emb_object_init(&set->obj, type);
	list_init(&set->list);
}

EMB_LOCAL void emb_set_join(struct emb_set *set, struct emb_object *obj)
{
	assert(set != NULL);
	assert(obj != NULL);
	emb_object_get(&set->obj);
	emb_object_leave_set(obj);
	list_add_tail(&set->list, &obj->entry);
	obj->set = set;
}

EMB_LOCAL void emb_object_leave_set(struct emb_object *obj)
{
	assert(obj != NULL);
	if (obj->set != NULL) {
		list_del_init(&obj->entry);
		emb_object_put(&obj->set->obj);
		obj->set = NULL;
	}
}
