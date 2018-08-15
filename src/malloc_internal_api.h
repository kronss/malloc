#ifndef MALLOC_INTERNAL_API_H
# define MALLOC_INTERNAL_API_H

# include "malloc.h"
# include "libft.h"

# include <sys/user.h> /* PAGE_SIZE */
# include <sys/mman.h>
# include <pthread.h>
# include <stdint.h> /*uint8_t*/
#include <unistd.h> /*getpagesize*/
//#include <stddef.h> /*offsetof*/
#include <unistd.h>


#define TINY_ZONE  (8LU * getpagesize())
#define	SMALL_ZONE (1500LU * getpagesize())

#define TINY_TRESHHOLD ((TINY_ZONE) / 100)
#define SMALL_TRESHHOLD ((SMALL_ZONE) / 100)


# define PROT_FLAGS	(PROT_EXEC | PROT_READ | PROT_WRITE)
# define MAP_FLAGS	(MAP_ANONYMOUS | MAP_PRIVATE)

#define ALIGN_PAGE_SIZE(len_) (len_) = ((PAGE_SIZE - 1) & (len_)) ? (((len_) + PAGE_SIZE) & ~(PAGE_SIZE - 1)) : (len_)
#define METABLOCK_SIZE sizeof(struct s_block)
#define ALIGN_WORD(x) (((((x) - 1) >> 2) << 2) + 4)  //TODO: re work
#define ALIGN_META_INFO(x) (x) = ALIGN_WORD((x) + sizeof(struct s_block))  //TODO: re work

#define OFFSETOF(st, m) ((size_t)&(((st *)0)->m))

enum e_zone_type
{
	MIN_ZONE_TYPE,
	TINY = MIN_ZONE_TYPE,
	SMALL,
	LARGE,

	MAX_ZONE_TYPE
};

/* block struct */
struct s_block {
	size_t           alloc_size;           /* size + meta_info */
	int              free     ;
	struct s_block  *next;
	struct s_block  *prev;

	uint8_t          data[];        /* A pointer to the user data. Should be last */
};

/*
 *
 */
struct			s_zone
{
	size_t				origin_size;
	size_t				space_left;
	struct s_zone		*next;
	struct s_zone		*prev;
	struct s_block		md_block_head; /* A pointer to the head blocks. Should be last */
};


struct malloc_meneger_s {
    struct s_zone *zone_heads[MAX_ZONE_TYPE];
//    size_t zone_size[MAX_ZONE_TYPE - 1];
    size_t print_total_alloc;
};






// silversearcher-ag
// ag




/**********************************/
void free_and_return_block_to_pull(struct s_zone *zone_ptr, struct s_block *block_ptr);
void *get_ptr_to_md(void *ptr);
int check_block_ptr(struct s_zone *zone_ptr, struct s_block *block_ptr);
int validate_md(struct s_zone **zone_ptr, struct s_block **block_ptr);
void *get_zone_head(size_t size);
void *get_ptr(size_t size);
//void remove_cur_zone_to_pull(struct zone_s *zone_ptr);
//int is_all_blocks_free(struct zone_s *zone_ptr);
void free_defragment_unmap(struct s_zone *zone_ptr, struct s_block *block_ptr);
//void *create_new_block(struct zone_s *zone_ptr, struct block_s *block_ptr, size_t size);
void	*find_zone(size_t size_request, struct s_zone *zone_head, enum e_zone_type zone_type);
void	*new_zone(enum e_zone_type zone_type, struct s_zone *prev_zone, size_t size_request);
void	*find_available_block(struct s_zone *zone_ptr, size_t size);






extern struct malloc_meneger_s g_alloc_mnr;
extern pthread_mutex_t g_mutex_malloc;

#endif /*MALLOC_INTERNAL_API_H*/
