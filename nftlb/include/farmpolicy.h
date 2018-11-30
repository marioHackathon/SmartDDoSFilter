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

#ifndef _FARMPOLICY_H_
#define _FARMPOLICY_H_

#include "farms.h"
#include "policies.h"

struct farmpolicy {
	struct list_head	list;
	char				*name;
	struct policy		*p;
	int					action;
};

void farmpolicy_s_print(struct farm *f);
struct farmpolicy * farmpolicy_lookup_by_name(struct farm *f, const char *name);
int farmpolicy_set_attribute(struct config_pair *c);

int farmpolicy_set_action(struct farmpolicy *b, int action);
int farmpolicy_s_set_action(struct farm *f, int action);

int farmpolicy_s_delete(struct farm *f);

#endif /* _FARMPOLICY_H_ */
