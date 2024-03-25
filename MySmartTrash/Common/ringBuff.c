#include "ringBuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#ifndef NULL
#define NULL (void*)0
#endif

#define IS_BD_LEGAL(x) (x >= 0 && x < MAX_RINGBUFFER_NUM)

struct ringbuffer_t
{
    int     buffer_description;
    void    *buffer;
    int     each_item_len;
    int     item_total_num;
    int     read_pos;
    int     write_pos;
    int     is_cover;
    int     item_exit_num;
};

static int assign_buffer_description(void);

static struct ringbuffer_t *ringbuffer_arr[MAX_RINGBUFFER_NUM] = {NULL};

static int assign_buffer_description(void)
{
    int index = 0;
    for(index = 0; index < MAX_RINGBUFFER_NUM; index++)
    {
        if(ringbuffer_arr[index] == NULL)
        {
            return index;
        }
    }
    return RINGBUFFER_FULL;
}

bool rb_init(int item_num, int each_item_len, int iscover)
{
    int ringbuffer_len;
    int buffer_description = assign_buffer_description();
    if(IS_BD_LEGAL(buffer_description))
    {
        ringbuffer_arr[buffer_description] = (struct ringbuffer_t*)malloc(sizeof(struct ringbuffer_t));
        ringbuffer_arr[buffer_description]->item_total_num = item_num;
        ringbuffer_arr[buffer_description]->each_item_len = each_item_len;
        ringbuffer_len = ringbuffer_arr[buffer_description]->item_total_num * ringbuffer_arr[buffer_description]->each_item_len;
        ringbuffer_arr[buffer_description]->buffer = (void*)malloc(ringbuffer_len);
        ringbuffer_arr[buffer_description]->read_pos = 0;
        ringbuffer_arr[buffer_description]->write_pos = 0;
        ringbuffer_arr[buffer_description]->is_cover = iscover;
        ringbuffer_arr[buffer_description]->item_exit_num = 0;
    }
    return buffer_description;
}

int  rb_delete(int bd)
{
    if(!IS_BD_LEGAL(bd) || ringbuffer_arr[bd] == NULL)
    {
        printf("ringbuffer_delete, ringbuffer_arr[%d] is not exist\n", bd);
        return RINGBUFFER_PARAMETER_LEAGAL;
    }
    free((void*)ringbuffer_arr[bd]->buffer);
    free((void*)ringbuffer_arr[bd]);
    ringbuffer_arr[bd] = NULL;

    return RINGBUFER_OK;
}

int rb_write(int bd, void *data)
{
    if(!IS_BD_LEGAL(bd) || ringbuffer_arr[bd] == NULL)
    {
        printf("ringbuffer_delete, ringbuffer_arr[%d] is not exist\n", bd);
        return RINGBUFFER_PARAMETER_LEAGAL;
    }

    if(ringbuffer_arr[bd]->item_exit_num < ringbuffer_arr[bd]->item_total_num)
    {
        void* pos = (void*)((char*)ringbuffer_arr[bd]->buffer + (ptrdiff_t)(ringbuffer_arr[bd]->write_pos * ringbuffer_arr[bd]->each_item_len));
        memcpy(pos, (void*)data, ringbuffer_arr[bd]->each_item_len);
        ringbuffer_arr[bd]->write_pos = (ringbuffer_arr[bd]->write_pos + 1) % ringbuffer_arr[bd]->item_total_num;
        ringbuffer_arr[bd]->item_exit_num++;
        return RINGBUFER_OK;
    }
    else if(ringbuffer_arr[bd]->is_cover)
    {
        void *pos = (void*)((char*)ringbuffer_arr[bd]->buffer + (ptrdiff_t)(ringbuffer_arr[bd]->write_pos * ringbuffer_arr[bd]->each_item_len));
        memcpy(pos, (void*)data, ringbuffer_arr[bd]->each_item_len);
        ringbuffer_arr[bd]->write_pos = (ringbuffer_arr[bd]->write_pos + 1) % ringbuffer_arr[bd]->item_total_num;
        ringbuffer_arr[bd]->read_pos = (ringbuffer_arr[bd]->write_pos + 1) % ringbuffer_arr[bd]->item_total_num;
    }

    return RINGBUFFER_FULL;
}

int rb_read(int bd, void *data)
{
    if(!IS_BD_LEGAL(bd) || ringbuffer_arr[bd] == NULL)
    {
        printf("ringbuffer_delete, ringbuffer_arr[%d] is not exist\n", bd);
        return RINGBUFFER_PARAMETER_LEAGAL;
    }

    if(ringbuffer_arr[bd]->item_exit_num)
    {
        void *pos = (void*)((char*)ringbuffer_arr[bd]->buffer + (ptrdiff_t)(ringbuffer_arr[bd]->read_pos * ringbuffer_arr[bd]->each_item_len));
        memcpy((void*)data, pos, ringbuffer_arr[bd]->each_item_len);
        ringbuffer_arr[bd]->item_exit_num--;
        return RINGBUFER_OK;
    }
    return RINGBUFFER_EMPTY;
}

int rb_write_items(int bd, void *data, int size)
{
    int len = 0;
    void *pos = data;
    if(!IS_BD_LEGAL(bd) || ringbuffer_arr[bd] == NULL || size == 0)
    {
        printf("ringbuffer_delete, ringbuffer_arr[%d] is not exist\n", bd);
        return RINGBUFFER_PARAMETER_LEAGAL;
    }

    for(len = 0; len < size; len++)
    {
        if(rb_write(bd, pos) != 0) break;
        pos = (char*)pos + (ptrdiff_t)ringbuffer_arr[bd]->each_item_len;
    }
    return len;
}

int rb_read_items(int bd, void *data, int size)
{
    int len = 0;
    void* pos = data;
    if (!IS_BD_LEGAL(bd) || ringbuffer_arr[bd] == NULL || size == 0)
    {
        printf("ringbuffer_read_items, ringbuffer_arr[%d] is not exist\n", bd);
        return RINGBUFFER_PARAMETER_LEAGAL;
    }
    
    for(len = 0; len < size; len++)
    {
		if(rb_read(bd, pos) != 0)    break;
        pos = (char*)pos + (ptrdiff_t)ringbuffer_arr[bd]->each_item_len;
    } 
    
    return len;
}

int rb_clean(int bd)
{
    if (!IS_BD_LEGAL(bd) || ringbuffer_arr[bd] == NULL)
    {
        printf("ringbuffer_clean, ringbuffer_arr[%d] is not exist\n", bd);
        return RINGBUFFER_PARAMETER_LEAGAL;
    }

    ringbuffer_arr[bd]->read_pos  = 0;
    ringbuffer_arr[bd]->write_pos = 0;
    ringbuffer_arr[bd]->item_exit_num = 0;
    
    return RINGBUFER_OK;
}

int rb_get_write_num(int bd)
{
    if (!IS_BD_LEGAL(bd) || ringbuffer_arr[bd] == NULL)
    {
        printf("ringbuffer_get_write_num, ringbuffer_arr[%d] is not exist\n", bd);
        return RINGBUFFER_PARAMETER_LEAGAL;
    }

    return ringbuffer_arr[bd]->item_exit_num;
}



