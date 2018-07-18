



void *base;


void *malloc(size_t size)
{

    t_block b,last;
    size_t s;
    s = align4(size );
    if (base) {
        /* First find a block */
        last = base;
        b = find_block (&last ,s);
        if (b) {
            /* can we split */
            if ((b->size - s) >= ( BLOCK_SIZE + 4))
                split_block (b,s);
                b->free =0;
            } else {
            /* No fitting block , extend the heap */
            b = extend_heap (last ,s);
            if (!b)
                return(NULL );
            }
        } else {
            /* first time */
            b = extend_heap (NULL ,s);
            if (!b)
                return(NULL );
            base = b;
        }

    return(b->data );
}
