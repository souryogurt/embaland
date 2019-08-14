/**
 * @file
 * Presenter implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "presenter.h"
#include "instance.h"
#include "compiler.h"

static void emb_presenter_destroy(struct emb_object *obj)
{
	struct emb_set *set = container_of(obj, struct emb_set, obj);
	struct emb_presenter *presenter =
		container_of(set, struct emb_presenter, set);
	/* TODO: set emb->presenter to NULL? */
	free(presenter);
}

static const struct emb_type presenter_type = {
	.release = emb_presenter_destroy,
};

EMB_LOCAL enum emb_result emb_presenter_create(struct emb_instance *emb,
					       struct emb_presenter **presenter)
{
	enum emb_result retval = EMB_SUCCESS;
	struct emb_presenter *new_presenter = malloc(sizeof(*new_presenter));
	if (new_presenter == NULL) {
		retval = EMB_ERROR_OUT_OF_HOST_MEMORY;
		goto err;
	}
	emb_set_init(&new_presenter->set, &presenter_type);
	emb_object_add(&new_presenter->set.obj, &emb->obj);
	*presenter = new_presenter;
err:
	return retval;
}
