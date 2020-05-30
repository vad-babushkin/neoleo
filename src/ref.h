#pragma once
/*
 * Copyright (c) 1992, 1993 Free Software Foundation, Inc.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <string>
#include "global.h"
#include "cell.h"

char * new_value (CELLREF row, CELLREF col, const char *string);
void for_all_vars (void (*func) (const char *, struct var *));
struct var * find_var (char *string, int len);
char * new_var_value (char *v_name, int v_namelen, struct rng *tmp_rng);
void flush_variables (void);
void push_cell (CELLREF row, CELLREF col);


