/**
 * @file
 * Mock of libembaland library
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <embaland/embaland.h>

#include <cgreen/mocks.h>

EMB_API enum emb_result EMB_CALL emb_create(emb_instance *instance)
{
	return (enum emb_result)mock(instance);
}

EMB_API void EMB_CALL emb_destroy(emb_instance instance)
{
	(void)mock(instance);
}
