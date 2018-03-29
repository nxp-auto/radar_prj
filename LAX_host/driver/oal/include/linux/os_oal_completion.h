/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OS_OAL_COMPLETION_H
#define OS_OAL_COMPLETION_H

#include <linux/completion.h>

struct OAL_Completion {
	struct completion c;
};
typedef struct OAL_Completion OAL_Completion_t;

static inline int OAL_init_completion(OAL_Completion_t *c)
{
	int ret = 0;

	if (c != NULL) {
		init_completion(&c->c);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

static inline int OAL_wait_for_completion(OAL_Completion_t *c)
{
	int ret = 0;

	if (c != NULL) {
		wait_for_completion(&c->c);
	} else {
		ret = -EINVAL;
	}

	return ret;
}


static inline unsigned long OAL_wait_for_completion_timeout(OAL_Completion_t *c,
	unsigned long timeout)
{
	unsigned long ret = 0;

	if (c != NULL) {
		ret = wait_for_completion_timeout(&c->c, timeout);
	} else {
		ret = 0;
	}

	return ret;
}

static inline int OAL_complete(OAL_Completion_t *c)
{
	int ret = 0;

	if (c != NULL) {
		complete(&c->c);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

static inline int OAL_complete_all(OAL_Completion_t *c)
{
	int ret = 0;

	if (c != NULL) {
		complete_all(&c->c);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

#endif /* OS_OAL_COMPLETION_H */
