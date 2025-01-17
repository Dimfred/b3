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

/**
 * @author Richard Bäck <richard.baeck@mailbox.org>
 * @date 2020-02-26
 * @brief File contains the window class definition
 */

#ifndef B3_WSMAN_H
#define B3_WSMAN_H

#include <windows.h>
#include <collectc/cc_array.h>

#include "ws_factory.h"

typedef struct b3_wsman_s b3_wsman_t;

struct b3_wsman_s
{
	int (*b3_wsman_free)(b3_wsman_t *wsman);
	b3_win_t *(*b3_wsman_get_win_at_pos)(b3_wsman_t *wsman, POINT *position);


	HANDLE global_mutex;

	b3_ws_factory_t *ws_factory;

	b3_ws_t *focused_ws;

	/**
	 * CC_Array of b3_ws_t *
	 */
	CC_Array *ws_arr;
};

/**
 * @brief Creates a new workspace manager 
 * @param ws_factory A workspace factory. It will not be freed by freeing the
 * workspace manager!
 * @return A new workspace manager or NULL if allocation failed
 */
extern b3_wsman_t *
b3_wsman_new(b3_ws_factory_t *ws_factory);

/**
 * @brief Deletes a workspace manager 
 * @return Non-0 if the deletion failed
 */
extern int
b3_wsman_free(b3_wsman_t *wsman);

/**
 * @return The workspace if the workspace was added, do not free it! Otherwise NULL.
 */
extern b3_ws_t *
b3_wsman_add(b3_wsman_t *wsman, const char *ws_id);

/**
 * @return 0 if the workspace was removed. Otherwise non-0.
 */
extern int
b3_wsman_remove(b3_wsman_t *wsman, const char *ws_id);

/**
 * @return The workspace if found. NULL otherwise. Do not free the returned
 * workspace!
 */
extern b3_ws_t *
b3_wsman_contains_ws(b3_wsman_t *wsman, const char *ws_id);

/**
 * @return The currently focused workspace of the workspace maanger. If no
 * workspace is focused, then NULL is returned. Do not free it!
 */
extern b3_ws_t *
b3_wsman_get_focused_ws(b3_wsman_t *wsman);

/**
 * @return Less than 0 if the workspaces was already focused. 0 if the focusing
 * was successful. Greater than 0 if the focusing failed.
 */
extern int
b3_wsman_set_focused_ws(b3_wsman_t *wsman, const char *ws_id);

/**
 * @param win The object will not be freed.
 * @return 0 if added. Non-0 otherwise.
 */
extern int
b3_wsman_remove_win(b3_wsman_t *wsman, b3_win_t *win);

/**
 * @return The workspace the window is placed on or NULL if it cannot be found.
 * Do not free it!
 */
extern b3_ws_t *
b3_wsman_find_win(b3_wsman_t *wsman, const b3_win_t *win);

/**
 * @return Non-0 if any window is currently maximized. 0 otherwise.
 */
extern int
b3_wsman_any_win_has_state(b3_wsman_t *wsman, b3_win_state_t state);

extern int
b3_wsman_remove_empty_ws(b3_wsman_t *wsman);

/**
 * Iterate over the workspace array contained within the workspace manager. This
 * is necessary as multiple threads may alter the contained workspaces at the
 * same time as this method is thread safe.
 *
 * @param visitor A function that will visit each workspace.
 */
extern int
b3_wsman_iterate_ws_arr(b3_wsman_t *wsman, void (*visitor)(b3_ws_t *ws));

/**
 * @return Returns the window at position. If no window can be found at the
 * given position, then NULL is returned.
 */
extern b3_win_t *
b3_wsman_get_win_at_pos(b3_wsman_t *wsman, POINT *position);

#endif // B3_WSMAN_H
