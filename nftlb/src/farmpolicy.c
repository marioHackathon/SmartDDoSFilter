/*
 *   This file is part of nftlb, nftables load balancer.
 *
 *   Copyright (C) ZEVENET SL.
 *   Author: Laura Garcia <laura.garcia@zevenet.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as
 *   published by the Free Software Foundation, either version 3 of the
 *   License, or any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "farmpolicy.h"
#include "farms.h"
#include "objects.h"
#include "network.h"


static struct farmpolicy * farmpolicy_create(struct farm *f, char *name)
{
	struct farmpolicy *b = (struct farmpolicy *)malloc(sizeof(struct farmpolicy));
	if (!b) {
		syslog(LOG_ERR, "Farm Policy memory allocation error");
		return NULL;
	}

	list_add_tail(&b->list, &f->policies);

	obj_set_attribute_string(name, &b->name);

	b->action = DEFAULT_ACTION;

	return b;
}

static int farmpolicy_delete_node(struct farmpolicy *b)
{
	list_del(&b->list);
	if (b->name)
		free(b->name);

	free(b);

	return 0;
}

static int farmpolicy_delete(struct farmpolicy *b)
{
	farmpolicy_delete_node(b);

	return 0;
}

void farmpolicy_s_print(struct farm *f)
{
	struct farmpolicy *b;

	list_for_each_entry(b, &f->policies, list) {
		syslog(LOG_DEBUG,"    [policy] ");
		syslog(LOG_DEBUG,"       [name] %s", b->name);

		syslog(LOG_DEBUG,"       *[action] %d", b->action);
	}
}

struct farmpolicy * farmpolicy_lookup_by_name(struct farm *f, const char *name)
{
	struct farmpolicy *b;

	list_for_each_entry(b, &f->policies, list) {
		if (strcmp(b->name, name) == 0)
			return b;
	}

	return NULL;
}

int farmpolicy_set_action(struct farmpolicy *b, int action)
{
	if (action == ACTION_DELETE) {
		farmpolicy_delete(b);
		return 1;
	}

	if (b->action != action) {
		b->action = action;
		return 1;
	}

	return 0;
}

int farmpolicy_s_set_action(struct farm *f, int action)
{
	struct farmpolicy *b, *next;

	list_for_each_entry_safe(b, next, &f->policies, list)
		farmpolicy_set_action(b, action);

	return 0;
}

int farmpolicy_s_delete(struct farm *f)
{
	struct farmpolicy *b, *next;

	list_for_each_entry_safe(b, next, &f->policies, list)
		farmpolicy_delete(b);

	return 0;
}

int farmpolicy_set_attribute(struct config_pair *c)
{
	struct obj_config *cur = obj_get_current_object();
	struct farmpolicy *b = cur->fpptr;

	if (!cur->fptr)
		return -1;

	switch (c->key) {
	case KEY_NAME:
		b = farmpolicy_lookup_by_name(cur->fptr, c->str_value);
		if (!b) {
			b = farmpolicy_create(cur->fptr, c->str_value);
			if (!b)
				return -1;
		}
		cur->fpptr = b;
		break;
	case KEY_ACTION:
		farmpolicy_set_action(b, c->int_value);
		break;
	default:
		return -1;
	}

	return 0;
}
