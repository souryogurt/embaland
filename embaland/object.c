/**
 * @file
 * Object implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "compiler.h"
#include "object.h"
#include "string.h"

EMB_LOCAL void emb_object_init(struct emb_object *obj, struct emb_type *type)
{
	assert(obj != NULL);
	assert(type != NULL);
	assert(type->release != NULL);
	emb_ref_init(&obj->ref);
	list_init(&obj->entry);
	obj->type = type;
	obj->name = NULL;
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
	free((void *)obj->name);
}

EMB_LOCAL void emb_object_put(struct emb_object *obj)
{
	assert(obj != NULL);
	emb_ref_put(&obj->ref, object_release);
}

EMB_LOCAL enum emb_object_result emb_object_add(struct emb_object *obj,
						struct emb_object *parent,
						const char *fmt, ...)
{
	assert(obj != NULL);
	va_list args;
	va_start(args, fmt);
	const char *name = emb_string_fmtv(fmt, args);
	va_end(args);
	if (name == NULL) {
		return EMB_OBJECT_ERROR_NAME;
	}
	free((void *)obj->name);
	obj->name = name;

	if (parent != NULL) {
		emb_object_get(parent);
	}
	if (obj->parent != NULL) {
		emb_object_put(obj->parent);
	}
	obj->parent = parent;
	return EMB_OBJECT_SUCCESS;
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
	list_add_tail(&set->list, &obj->entry);
	obj->set = set;
}

EMB_LOCAL void emb_object_leave_set(struct emb_object *obj)
{
	assert(obj != NULL);
	if (obj->set != NULL) {
		list_del_init(&obj->entry);
		obj->set = NULL;
	}
}
