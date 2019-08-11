#ifndef EMBALAND_OBJECT_H
#define EMBALAND_OBJECT_H 1

#include "platform.h"
#include "list.h"
#include "ref.h"

enum emb_object_result {
	EMB_OBJECT_SUCCESS = 0,
	EMB_OBJECT_ERROR_NAME = -1,
};

struct emb_set;
struct emb_type;

struct emb_object {
	const char *name;
	struct list_head entry;
	struct emb_object *parent;
	struct emb_set *set;
	struct emb_type *type;
	struct emb_ref ref;
};

struct emb_type {
	void (*const release)(struct emb_object *obj);
};

struct emb_set {
	struct list_head list;
	struct emb_object obj;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the object.
 * @param obj is object to initialize
 * @param type is the type of new object
 */
EMB_LOCAL void emb_object_init(struct emb_object *obj, struct emb_type *type);

/**
 * Get reference to object.
 * @param obj is the object to get reference for
 * @returns the @a obj as is
 */
EMB_LOCAL struct emb_object *emb_object_get(struct emb_object *obj);

/**
 * Release the object.
 * @param obj is the object to release
 */
EMB_LOCAL void emb_object_put(struct emb_object *obj);

/**
 * Add object to hierarhy with specified name.
 * @param obj is the object to add
 * @param parent is the object that will be parent of @a obj
 * @param fmt is format specifier for object name
 * @retval EMB_OBJECT_SUCCESS when object is succefully added
 */
EMB_LOCAL enum emb_object_result emb_object_add(struct emb_object *obj,
						struct emb_object *parent,
						const char *fmt, ...);

/**
 * Remove object from hierarhy.
 * @param obj is the object to remove
 */
EMB_LOCAL void emb_object_del(struct emb_object *obj);

/**
 * Initialize empty object set.
 * @param set is the set to initialize
 * @param type is the type of the set
 */
EMB_LOCAL void emb_set_init(struct emb_set *set, struct emb_type *type);

/**
 * Add object to the set.
 * @param set is the set to add object to
 * @param obj is the object to add
 */
EMB_LOCAL void emb_set_join(struct emb_set *set, struct emb_object *obj);

/**
 * Remove object from set.
 * @param obj is the object to remove
 */
EMB_LOCAL void emb_object_leave_set(struct emb_object *obj);

#ifdef __cplusplus
}
#endif
#endif
