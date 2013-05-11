/*
 * mybuff.c - MYBUFF device
 *
 * Copyright (C) 2011 Ericsson AB
 *
 * Author: Paolo Rovelli <paolo.rovelli@ericsson.com>
 */

#ifndef __MYBUFF_H__
#define __MYBUFF_H__

struct mybuff;

extern int mybuff_create(struct mybuff **mybuff, int size);
extern int mybuff_delete(struct mybuff *mybuff);
extern int mybuff_read(struct mybuff *mybuff, char *buff, int count);
extern int mybuff_write(struct mybuff *mybuff, char *buff, int count);
extern int mybuff_clear(struct mybuff *mybuff);
extern int mybuff_size(struct mybuff *mybuff);
extern int mybuff_free(struct mybuff *mybuff);
extern int mybuff_ready(struct mybuff *mybuff);

#endif /* __MYBUFF_H__ */

