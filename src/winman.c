/******************************************************************************
  This file is part of b3.

  Copyright 2020 Richard Paul Baeck <richard.baeck@mailbox.org>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*******************************************************************************/

#include "winman.h"

/**
 * @author Richard Bäck
 * @date 26 January 2020
 * @brief File contains the window manager class implementation and private methods
 */

#include <stdlib.h>
#include <string.h>
#include <w32bindkeys/logger.h>

static wbk_logger_t logger = { "winman" };

static int
b3_winman_free_impl(b3_winman_t *winman);

static void
b3_winman_traverse_impl(b3_winman_t *winman,
						 void visitor(b3_winman_t *winman, void *data),
						 void *data);

static int
b3_winman_add_winman_impl(b3_winman_t *root, b3_winman_t *winman);

static int
b3_winman_remove_winman_impl(b3_winman_t *root, b3_winman_t *winman);

static CC_Array *
b3_winman_get_winman_arr_impl(b3_winman_t *winman);

static int
b3_winman_set_win_impl(b3_winman_t *winman, b3_win_t *win);

static b3_win_t *
b3_winman_get_win_impl(b3_winman_t *winman);

static b3_winman_mode_t
b3_winman_get_mode_impl(b3_winman_t *winman);

static b3_winman_t *
b3_winman_get_parent_impl(b3_winman_t *root, b3_winman_t *winman);

static b3_winman_t *
b3_winman_contains_win_impl(b3_winman_t *winman, const b3_win_t *win);

static b3_winman_t *
b3_winman_get_winman_rel_to_winman_impl(b3_winman_t *root,
										b3_winman_t *winman,
										b3_winman_get_rel_t direction,
										char rolling);

static int
b3_winman_is_empty_impl(b3_winman_t *root, char check_deeply);

static int
b3_winman_reorg_impl(b3_winman_t *winman);

static b3_win_t *
b3_winman_get_maximized_impl(b3_winman_t *winman);

static b3_win_t *
b3_winman_get_win_at_pos_impl(b3_winman_t *winman, POINT *position);

b3_winman_t *
b3_winman_new(b3_winman_mode_t mode)
{
	b3_winman_t *winman;

	winman = NULL;
	winman = malloc(sizeof(b3_winman_t));

	if (winman) {
		winman->b3_winman_free = b3_winman_free_impl;
		winman->b3_winman_traverse = b3_winman_traverse_impl;
		winman->b3_winman_add_winman = b3_winman_add_winman_impl;
		winman->b3_winman_remove_winman = b3_winman_remove_winman_impl;
		winman->b3_winman_get_winman_arr = b3_winman_get_winman_arr_impl;
		winman->b3_winman_set_win = b3_winman_set_win_impl;
		winman->b3_winman_get_win = b3_winman_get_win_impl;
		winman->b3_winman_get_mode = b3_winman_get_mode_impl;
		winman->b3_winman_get_parent = b3_winman_get_parent_impl;
		winman->b3_winman_contains_win = b3_winman_contains_win_impl;
		winman->b3_winman_get_winman_rel_to_winman = b3_winman_get_winman_rel_to_winman_impl;
		winman->b3_winman_is_empty = b3_winman_is_empty_impl;
		winman->b3_winman_reorg = b3_winman_reorg_impl;
		winman->b3_winman_get_maximized = b3_winman_get_maximized_impl;
		winman->b3_winman_get_win_at_pos = b3_winman_get_win_at_pos_impl;

		cc_array_new(&(winman->winman_arr));
		winman->win = NULL;
		winman->mode = mode;
	}

	return winman;
}

int
b3_winman_free(b3_winman_t *winman)
{
	return winman->b3_winman_free(winman);
}

void
b3_winman_traverse(b3_winman_t *winman,
					void visitor(b3_winman_t *winman, void *data),
					void *data)
{
	return winman->b3_winman_traverse(winman, visitor, data);
}

int
b3_winman_add_winman(b3_winman_t *root, b3_winman_t *winman)
{
	return winman->b3_winman_add_winman(root, winman);
}

int
b3_winman_remove_winman(b3_winman_t *root, b3_winman_t *winman)
{
	return winman->b3_winman_remove_winman(root, winman);
}

CC_Array *
b3_winman_get_winman_arr(b3_winman_t *winman)
{
	return winman->b3_winman_get_winman_arr(winman);
}

int
b3_winman_set_win(b3_winman_t *winman, b3_win_t *win)
{
	return winman->b3_winman_set_win(winman, win);
}

b3_win_t *
b3_winman_get_win(b3_winman_t *winman)
{
	return winman->b3_winman_get_win(winman);
}

b3_winman_mode_t
b3_winman_get_mode(b3_winman_t *winman)
{
	return winman->b3_winman_get_mode(winman);
}

b3_winman_t *
b3_winman_get_parent(b3_winman_t *root, b3_winman_t *winman)
{
	return winman->b3_winman_get_parent(root, winman);
}

b3_winman_t *
b3_winman_contains_win(b3_winman_t *winman, const b3_win_t *win)
{
	return winman->b3_winman_contains_win(winman, win);
}

b3_winman_t *
b3_winman_get_winman_rel_to_winman(b3_winman_t *root,
								   b3_winman_t *winman,
								   b3_winman_get_rel_t direction,
								   char rolling)
{
	return root->b3_winman_get_winman_rel_to_winman(root, winman, direction, rolling);
}

int
b3_winman_is_empty(b3_winman_t *root, char check_deeply)
{
	return root->b3_winman_is_empty(root, check_deeply);
}

int
b3_winman_reorg(b3_winman_t *winman)
{
	return winman->b3_winman_reorg(winman);
}

b3_win_t *
b3_winman_get_maximized(b3_winman_t *winman)
{
	return winman->b3_winman_get_maximized(winman);
}

b3_win_t *
b3_winman_get_win_at_pos(b3_winman_t *winman, POINT *position)
{
	return winman->b3_winman_get_win_at_pos(winman, position);
}

int
b3_winman_free_impl(b3_winman_t *winman)
{
	CC_ArrayIter iter;
	b3_winman_t *winman_iter;
	b3_win_t *win_iter;

	/**
	 * Remove and traverse all window manager objects
	 */
	cc_array_iter_init(&iter, winman->winman_arr);
    while (cc_array_iter_next(&iter, (void*) &winman_iter) != CC_ITER_END) {
    	cc_array_iter_remove(&iter, NULL);
    	b3_winman_free(winman_iter);
    }
	cc_array_destroy(winman->winman_arr);
	winman->winman_arr = NULL;

	winman->win = NULL;

	free(winman);

	return 0;
}

void
b3_winman_traverse_impl(b3_winman_t *winman,
						 void visitor(b3_winman_t *winman, void *data),
						 void *data)
{
	CC_ArrayIter iter;
	b3_winman_t *winman_iter;

	visitor(winman, data);

	cc_array_iter_init(&iter, b3_winman_get_winman_arr(winman));
	while (cc_array_iter_next(&iter, (void*) &winman_iter) != CC_ITER_END) {
		b3_winman_traverse(winman_iter, visitor, data);
	}
}

int
b3_winman_add_winman_impl(b3_winman_t *root, b3_winman_t *winman)
{
	int error;

	error = cc_array_add(root->winman_arr, winman);

	return error;
}

int
b3_winman_remove_winman_impl(b3_winman_t *root, b3_winman_t *winman)
{
	int error;
	b3_winman_t *container;
	CC_ArrayIter iter;
	b3_winman_t *winman_iter;

	error = 1;
	container = b3_winman_get_parent(root, winman);
	if (container) {
		cc_array_iter_init(&iter, b3_winman_get_winman_arr(container));
		while (error && cc_array_iter_next(&iter, (void*) &winman_iter) != CC_ITER_END) {
			if (winman_iter == winman) {
				cc_array_iter_remove(&iter, NULL);
				error = 0;
			}
		}
	}

	return error;
}

CC_Array *
b3_winman_get_winman_arr_impl(b3_winman_t *winman)
{
	return winman->winman_arr;
}

int
b3_winman_set_win_impl(b3_winman_t *winman, b3_win_t *win)
{
	winman->win = win;
	return 0;
}

b3_win_t *
b3_winman_get_win_impl(b3_winman_t *winman)
{
	return winman->win;
}

b3_winman_mode_t
b3_winman_get_mode_impl(b3_winman_t *winman)
{
	return winman->mode;
}

b3_winman_t *
b3_winman_get_parent_impl(b3_winman_t *root, b3_winman_t *winman)
{
	b3_winman_t *container;
	CC_ArrayIter iter;
	b3_winman_t *winman_iter;

	container = NULL;

	cc_array_iter_init(&iter, b3_winman_get_winman_arr(root));
	while (container == NULL && cc_array_iter_next(&iter, (void*) &winman_iter) != CC_ITER_END) {
		if (winman_iter == winman) {
			container = root;
		} else {
			container = b3_winman_get_parent(winman_iter, winman);
		}
	}

	return container;
}

b3_winman_t *
b3_winman_contains_win_impl(b3_winman_t *winman, const b3_win_t *win)
{
	b3_winman_t *container;
	b3_win_t *winman_win;
	CC_ArrayIter iter;
	b3_winman_t *winman_iter;

	container = NULL;

	winman_win = b3_winman_get_win(winman);
	if (winman_win
		&& b3_win_compare(winman_win, win) == 0) {
		container = winman;
	}

	if (container == NULL) {
		cc_array_iter_init(&iter, b3_winman_get_winman_arr(winman));
		while (container == NULL && cc_array_iter_next(&iter, (void*) &winman_iter) != CC_ITER_END) {
			container = b3_winman_contains_win(winman_iter, win);
		}
	}

	return container;
}

b3_winman_t *
b3_winman_get_winman_rel_to_winman_impl(b3_winman_t *root,
										b3_winman_t *winman,
										b3_winman_get_rel_t direction,
										char rolling)
{
	b3_winman_t *found;
	b3_winman_t *winman_iter;
	int arr_len;
	int i;
	int found_pos;

	found = NULL;

	arr_len = cc_array_size(b3_winman_get_winman_arr(root));
	found_pos = -1;
	for (i = 0; found_pos < 0 && i < arr_len; i++) {
		cc_array_get_at(b3_winman_get_winman_arr(root), i, (void *) &winman_iter);
		if (winman_iter == winman) {
			found_pos = i;
		}
	}

	if (found_pos >= 0) {
		if (direction == PREVIOUS) {
			found_pos--;
		} else if (direction == NEXT) {
			found_pos++;
		} else {
			wbk_logger_log(&logger, SEVERE, "Unknown value for 'direction': %d\n", direction);
			found_pos = -1;
		}

		if (rolling) {
			if (found_pos >= arr_len) {
				found_pos = 0;
			} else if (found_pos < 0) {
				found_pos = arr_len - 1;
			}
		}
	} else {
		wbk_logger_log(&logger, SEVERE, "'winman' cannot be found in 'root'\n");
	}

	if (found_pos >= 0 && found_pos < arr_len) {
		cc_array_get_at(b3_winman_get_winman_arr(root), found_pos, (void *) &found);
	}

	return found;
}

int
b3_winman_is_empty_impl(b3_winman_t *root, char check_deeply)
{
	int is_empty;
	CC_ArrayIter iter;
	b3_winman_t *winman_iter;

	is_empty = 0;

	if (b3_winman_get_win(root) == NULL) {
		is_empty = 1;
		cc_array_iter_init(&iter, b3_winman_get_winman_arr(root));
		while (is_empty && cc_array_iter_next(&iter, (void*) &winman_iter) != CC_ITER_END) {
			if (b3_winman_get_win(winman_iter)) {
				is_empty = 0;
			} else if (check_deeply) {
				is_empty = b3_winman_is_empty(winman_iter, check_deeply);
			}
		}
	}

	return is_empty;
}

int
b3_winman_reorg_impl(b3_winman_t *winman)
{
	int error;
	CC_ArrayIter iter;
	b3_winman_t *winman_iter;

	error = 0;

	cc_array_iter_init(&iter, b3_winman_get_winman_arr(winman));
	while (!error && cc_array_iter_next(&iter, (void*) &winman_iter) != CC_ITER_END) {
		if (b3_winman_is_empty(winman_iter, 1)) {
			cc_array_iter_remove(&iter, NULL);
		} else {
			error = b3_winman_reorg(winman_iter);
		}
	}

	return error;
}

b3_win_t *
b3_winman_get_maximized_impl(b3_winman_t *winman)
{
	b3_win_t *maximized;
	CC_ArrayIter iter;
	b3_winman_t *winman_iter;

	maximized = b3_winman_get_win(winman);
	if (maximized) {
		if (b3_win_get_state(maximized) != MAXIMIZED) {
			maximized = NULL;
		}
	} else {
		cc_array_iter_init(&iter, b3_winman_get_winman_arr(winman));
		while (maximized == NULL && cc_array_iter_next(&iter, (void*) &winman_iter) != CC_ITER_END) {
			maximized = b3_winman_get_maximized(winman_iter);
		}
	}

	return maximized;
}

b3_win_t *
b3_winman_get_win_at_pos_impl(b3_winman_t *winman, POINT *position)
{
	b3_win_t *win_at_pos;
	CC_ArrayIter iter;
	b3_winman_t *winman_iter;

	win_at_pos = b3_winman_get_win(winman);
	if (win_at_pos) {
		if (!b3_win_is_point_in_rect(win_at_pos, position)) {
			win_at_pos = NULL;
		}
	} else {
		cc_array_iter_init(&iter, b3_winman_get_winman_arr(winman));
		while (win_at_pos == NULL && cc_array_iter_next(&iter, (void*) &winman_iter) != CC_ITER_END) {
			win_at_pos = b3_winman_get_win_at_pos_impl(winman_iter, position);
		}
	}

	return win_at_pos;
}
