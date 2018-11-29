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

#ifndef _POLICIES_H_
#define _POLICIES_H_

#include "list.h"
#include "config.h"

enum type {
	VALUE_TYPE_BLACK,
	VALUE_TYPE_WHITE,
};

struct policy {
	struct list_head	list;
	char				*name;
	int					type;
	int					timeout;
	int					priority;
	int					total_elem;
	int					action;
	struct list_head	elements;
};

void policies_s_print(void);
struct policy * policy_lookup_by_name(const char *name);
int policy_set_attribute(struct config_pair *c);
int policy_set_action(struct policy *p, int action);
int policy_s_set_action(int action);

#endif /* _POLICIES_H_ */
