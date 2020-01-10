#ifndef EMTRIANGLE_SCENE_H
#define EMTRIANGLE_SCENE_H 1

#include <embaland/embaland.h>
#include <embaland/buffer.h>
#include <embaland/buffer_view.h>
#include <embaland/accessor.h>

struct scene {
	struct emb_buffer buffer;
	struct emb_buffer_view bview;
	struct emb_accessor positions;
	emb_mesh triangle;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize triangle scene.
 * @param emb is the handle of the embaland instance
 * @param scn pointer to scene to initialize
 * @retval EMB_SUCCESS scene initialized
 * @sa scene_release()
 */
enum emb_result scene_init(emb_instance emb, struct scene *scn);

/**
 * Release triangle scene.
 * @param scn pointer to scene to release
 */
void scene_release(struct scene *scn);

#ifdef __cplusplus
}
#endif
#endif
