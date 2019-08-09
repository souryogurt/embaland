/**
 * @file
 * Reference counter implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "embaland/ref.h"

EMB_LOCAL extern inline void emb_ref_init(struct emb_ref *ref);
EMB_LOCAL extern void emb_ref_get(struct emb_ref *ref);
EMB_LOCAL extern bool emb_ref_put(struct emb_ref *ref, emb_ref_release release);
