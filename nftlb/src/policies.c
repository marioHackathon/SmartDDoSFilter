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

#include "policies.h"
#include "elements.h"
#include "objects.h"
#include "config.h"
#include "nft.h"


static struct policy * policy_create(char *name)
{
	struct list_head *policies = obj_get_policies();

	struct policy *p = (struct policy *)malloc(sizeof(struct policy));
	if (!p) {
		syslog(LOG_ERR, "Policy memory allocation error");
		return NULL;
	}

	list_add_tail(&p->list, policies);
	obj_set_total_policies(obj_get_total_policies() + 1);

	obj_set_attribute_string(name, &p->name);

	p->type = DEFAULT_POLICY_TYPE;
	p->timeout = DEFAULT_POLICY_TIMEOUT;
	p->priority = DEFAULT_POLICY_PRIORITY;
	p->action = DEFAULT_ACTION;

	init_list_head(&p->elements);

	p->total_elem = 0;

	return p;
}

static int policy_delete(struct policy *p)
{
	element_s_delete(p);
	list_del(&p->list);

	if (p->name && strcmp(p->name, "") != 0)
		free(p->name);

	free(p);
	obj_set_total_policies(obj_get_total_policies() - 1);

	return 0;
}

static void policy_print(struct policy *p)
{
	syslog(LOG_DEBUG," [policy] ");
	syslog(LOG_DEBUG,"    [name] %s", p->name);
	syslog(LOG_DEBUG,"    [type] %s", obj_print_policy_type(p->type));
	syslog(LOG_DEBUG,"    [timeout] %d", p->timeout);
	syslog(LOG_DEBUG,"    [priority] %d", p->priority);
	syslog(LOG_DEBUG,"    [total_elem] %d", p->total_elem);
	syslog(LOG_DEBUG,"    *[action] %d", p->action);

	if (p->total_elem != 0)
		element_s_print(p);
}

void policies_s_print(void)
{
	struct list_head *policies = obj_get_policies();
	struct policy *p;

	list_for_each_entry(p, policies, list) {
		policy_print(p);
	}
}

struct policy * policy_lookup_by_name(const char *name)
{
	struct list_head *policies = obj_get_policies();
	struct policy *p;

	list_for_each_entry(p, policies, list) {
		if (strcmp(p->name, name) == 0)
			return p;
	}

	return NULL;
}

int policy_set_attribute(struct config_pair *c)
{
	struct obj_config *cur = obj_get_current_object();
	struct policy *p = cur->pptr;

	switch (c->key) {
	case KEY_NAME:
		p = policy_lookup_by_name(c->str_value);
		if (!p) {
			p = policy_create(c->str_value);
			if (!p)
				return -1;
		}
		cur->pptr = p;
		break;
	case KEY_TYPE:
		p->type = c->int_value;
		break;
	case KEY_TIMEOUT:
		p->timeout = c->int_value;
		break;
	case KEY_PRIORITY:
		p->priority = c->int_value;
		break;
	case KEY_ACTION:
		policy_set_action(p, c->int_value);
		break;
	default:
		return -1;
	}

	return 0;
}

int policy_set_action(struct policy *p, int action)
{
	syslog(LOG_DEBUG, "%s():%d: policy %s set action %d", __FUNCTION__, __LINE__, p->name, action);

	if (action == ACTION_DELETE) {
		policy_delete(p);
		return 1;
	}

	if (p->action!= action) {
		p->action = action;
		return 1;
	}

	return 0;
}

int policy_s_set_action(int action)
{
	struct list_head *policies = obj_get_policies();
	struct policy *p, *next;

	list_for_each_entry_safe(p, next, policies, list)
		policy_set_action(p, action);

	return 0;
}
