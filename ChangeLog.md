# Release 0.4

Bugfixes:
* Using many workspaces and many windows might resulted at some point into short loops of b3 switching between the windows by its own. This is finally solved.
* Toggling a window to fullscreen is fully supported. It will stay in fullscreen even when switching between workspaces.

Features:
* Setting key bindings is now possible in a configuration file. Re-compilationis not needed any more. For the availabe commands see the [Compatibility List](https://github.com/ritschmaster/b3/blob/master/CompatibilityList.md)
* A CLI interface like in i3 (e.g. `b3.exe -d` to enable logging on debug level).

Known bugs:
* Windows in fullscreen might not be repainted when switching onto their workspace. This results into just a big white window. Switching onto the workspace again redraws the window correctly.
* Re-drawing of moved windows does not always work correctly. This is most likely due some lacking hacks when using the WIN32 API.

# Release 0.3

Bugfixes:
* The application does not crash on startup anymore! Neither does it crash during execution anymore!
* Partly fixes toggling fullscreen of a window. The window now remains fullscreen when switching to another monitor. But when switching to another workspace on the same monitor and then back again it does not get restored correctly.
* Automatically increasing the workspace number works correctly now

Features:
* New multi monitor features
  * Move a window to another monitor in some direction
  * Move a workspace to another monitor in some direction
  * Switch the focused workspace to the currently visible workspace on some other monitor

Known bugs:
* Using many workspaces and many windows might result at some point into short loops of b3 switching between the windows by its own.

New key bindings:
* Set focus to workspace on monitor on the left: Win + Ctrl + h
* Set focus to workspace on monitor on the right: Win + Ctrl + o
* Set focus to workspace on monitor on the bottom: Win + Ctrl + j
* Set focus to workspace on monitor on the top: Win + Ctrl + k
* Move currently focused window to the workspace on monitor on the left: Win + Ctrl + Shift + h
* Move currently focused window to the workspace on monitor on the right: Win + Ctrl + Shift + o
* Move currently focused window to the workspace on monitor on the bottom: Win + Ctrl + Shift + j
* Move currently focused window to the workspace on monitor on the top: Win + Ctrl + Shift + k
* Move currently focused workspace to the monitor on the left: Win + Ctrl + Shift + Alt + h
* Move currently focused workspace to the monitor on the right: Win + Ctrl + Shift + Alt + o
* Move currently focused workspace to the monitor on the bottom: Win + Ctrl + Shift + Alt + j
* Move currently focused workspace to the monitor on the top: Win + Ctrl + Shift + Alt + k


# Release 0.2

Almost ready to be tried in an production environment!

Bugfixes:
* The b3 bar is drawn correctly in a multi-monitor setup
* b3 crashed if the user pressed some window-to-workspace key binding on an empty workspace
* The arranged windows are not overlapping with the b3 bar anymore.
* If the task bar is on auto hide, then that space is consumed too correctly.
* Clicking a window on another monitor did not change the focused monitor.

Features:
* The b3 bar has received an additional indicator for the currently selected but inactive workspace on another monitor
    * Active workspace on active monitor is red
    * Active workspace on inactive monitor is dark greenish


# Release 0.1

This is the first release for the brave and the curious. Not to be used in a production environment!

Known bugs:
* A fresh start of the application might immediately result into a crash due some race condition
* The tiling of the default tiling mode is not yet like i3
* Tiling might stop during usage (possible cause might be an internal crash)
* Still much to be done to be like i3 (no config file yet, ...)
* The key bindings are still hard coded in main.c:
  * Toggle window fullscreen: Win + f
  * Toggle window floating: Win + Shift + Space
  * Kill the focused window: Win + Shift + q
  * Set focus to window left: Win + h
  * Set focus to window right: Win + o
  * Set focus to window top: Win + k
  * Set focus to window bottom: Win + j
  * Move window left: Win + Shift + h
  * Move window right: Win + Shift + o
  * Move window top: Win + Shift + k
  * Move window bottom: Win + Shift + j
  * Change to workspace 1: Win + 1
  * Change to workspace 2: Win + 2
  * Change to workspace 3: Win + 3
  * Change to workspace 4: Win + 4
  * Change to workspace 5: Win + 5
  * Change to workspace 6: Win + 6
  * Change to workspace 7: Win + 7
  * Change to workspace 8: Win + 8
  * Change to workspace 9: Win + 9
  * Move window workspace 1: Win + Shift + 1
  * Move window workspace 2: Win + Shift + 2
  * Move window workspace 3: Win + Shift + 3
  * Move window workspace 4: Win + Shift + 4
  * Move window workspace 5: Win + Shift + 5
  * Move window workspace 6: Win + Shift + 6
  * Move window workspace 7: Win + Shift + 7
  * Move window workspace 8: Win + Shift + 8
  * Move window workspace 9: Win + Shift + 9

