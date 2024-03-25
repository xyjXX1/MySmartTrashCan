#ifndef __RING_BUFF_H
#define __RING_BUFF_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#define MAX_RINGBUFFER_NUM		      	(10)
#define RINGBUFER_OK			      			(0)
#define RINGBUFFER_PARAMETER_LEAGAL	  (-1)
#define RINGBUFFER_EMPTY			  			(-2)
#define RINGBUFFER_FULL				  			(-3)
#define RINGBUFFER_BD_FULL			  		(-4)

enum iscover_t
{
	uncover = 0,
	cover = 1,
};

bool rb_init(int item_num, int each_item_len, int iscover);
int  rb_delete(int bd);
int rb_write(int bd, void *data);
int rb_read(int bd, void *data);
int rb_write_items(int bd, void *data, int size);
int rb_read_items(int bd, void *data, int size);
int rb_clean(int bd);
int rb_get_write_num(int bd);

#ifdef __cplusplus
}
#endif

#endif   //__RING_BUFF_H
