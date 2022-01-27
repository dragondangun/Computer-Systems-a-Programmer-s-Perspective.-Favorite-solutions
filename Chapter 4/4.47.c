void bubble_p(long *data, long count) {
    for (long last = count-1; last > 0; last--) {
        for (long i  = 0; i < last; i++, data++){
            if (*(data+1) < *data) {
                /* Swap adjacent elements */
                long t = *(data+1);
                *(data+1) = *data;
                *data = t;
            }
        }
        data-=last;
    }
}