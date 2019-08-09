#ifndef EMBALAND_REF_H
#define EMBALAND_REF_H 1

#include "platform.h"
#include <stdatomic.h>
#include <stdbool.h>

struct emb_ref {
	atomic_int_fast32_t count;
};

typedef void (*emb_ref_release)(struct emb_ref *ref);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize reference counter.
 * @param ref is counter to initialize
 */
EMB_LOCAL inline void emb_ref_init(struct emb_ref *ref)
{
	atomic_init(&ref->count, 1);
}

/**
 * Increment reference counter.
 * @param ref is counter to increment
 */
EMB_LOCAL inline void emb_ref_get(struct emb_ref *ref)
{
	atomic_fetch_add_explicit(&ref->count, 1, memory_order_relaxed);
}

/**
 * Decrement reference counter.
 * @param ref is counter to decrement
 * @param release is pointer to the function that is called when counter become
 *                zero
 * @returns true if the object was released
 */
EMB_LOCAL inline bool emb_ref_put(struct emb_ref *ref, emb_ref_release release)
{
	atomic_int_fast32_t old =
		atomic_fetch_sub_explicit(&ref->count, 1, memory_order_acq_rel);
	if (old == 1) {
		release(ref);
		return true;
	}
	return false;
}

#ifdef __cplusplus
}
#endif
#endif
