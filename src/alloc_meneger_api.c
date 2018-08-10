#include "malloc_internal_api.h"


pthread_mutex_t mutex_malloc = PTHREAD_MUTEX_INITIALIZER;

struct malloc_meneger_s malloc_meneger_g =
{
    .zone_heads = {NULL, NULL, NULL}
};

void *get_zone_ptr(size_t size)
{
    void *retval;

    if (size <= TINY_TRESHHOLD) {
        retval = malloc_meneger_g.zone_heads[TINY];
    } else if (size <= SMALL_TRESHHOLD) {
        retval = malloc_meneger_g.zone_heads[SMALL];
    } else {
        retval = malloc_meneger_g.zone_heads[LARGE];
    }
    return retval;
}
