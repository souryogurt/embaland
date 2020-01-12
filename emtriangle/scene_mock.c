/**
 * @file
 * Mock of scene module
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "scene.h"

#include <embaland/embaland.h>

#include <cgreen/mocks.h>

enum emb_result scene_init(emb_instance emb, struct scene *scn)
{
	return (enum emb_result)mock(emb, scn);
}

void scene_destroy(struct scene *scn)
{
	(void)mock(scn);
}
