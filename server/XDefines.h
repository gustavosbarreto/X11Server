#ifndef _XDEFINES_H_
#define _XDEFINES_H_

/*
 * Events
 */
#define KEY_PRESS_EVENT                 (1 << 0)
#define KEY_RELEASE_EVENT               (1 << 1)
#define BUTTON_PRESS_EVENT              (1 << 2)
#define BUTTON_RELEASE_EVENT            (1 << 3)
#define ENTER_WINDOW_EVENT              (1 << 4)
#define LEAVE_WINDOW_EVENT              (1 << 5)
#define POINTER_MOTION_EVENT            (1 << 6)
#define POINTER_MOTION_HINT_EVENT       (1 << 7)
#define BUTTON_1_MOTION_EVENT           (1 << 8)
#define BUTTON_2_MOTION_EVENT           (1 << 9)
#define BUTTON_3_MOTION_EVENT           (1 << 10)
#define BUTTON_4_MOTION_EVENT           (1 << 11)
#define BUTTON_5_MOTION_EVENT           (1 << 12)
#define BUTTON_MOTION_EVENT             (1 << 13)
#define KEY_MAP_STATE_EVENT             (1 << 14)
#define EXPOSURE_EVENT                  (1 << 15)
#define VISIBILITY_CHANGE_EVENT         (1 << 16)
#define STRUCTURE_NOTIFY_EVENT          (1 << 17)
#define RESIZE_REDIRECT_EVENT           (1 << 18)
#define SUBSTRUCTURE_NOTIFY_EVENT       (1 << 19)
#define SUBSTRUCTURE_REDIRECT_EVENT     (1 << 20)
#define FOCUS_CHANGE_EVENT              (1 << 21)
#define PROPERTY_CHANGE_EVENT           (1 << 22)
#define COLORMAP_CHANGE_EVENT           (1 << 23)
#define OWNERGRAB_BUTTON_EVENT          (1 << 24)

/*
 * Window Attributes
 */

#define X_ATTRIBUTE_BACKGROUND_PIXMAP       0x01
#define X_ATTRIBUTE_BACKGROUND_PIXEL        0x02
#define X_ATTRIBUTE_BORDER_PIXMAP           0x04
#define X_ATTRIBUTE_BORDER_PIXEL            0x08
#define X_ATTRIBUTE_BIT_GRAVITY             0x10
#define X_ATTRIBUTE_WIN_GRAVITY             0x20
#define X_ATTRIBUTE_BACKING_STORE           0x40
#define X_ATTRIBUTE_BACKING_PLANES          0x80
#define X_ATTRIBUTE_BACKING_PIXEL           0x100
#define X_ATTRIBUTE_OVERRIDE_REDIRECT       0x200
#define X_ATTRIBUTE_SAVE_UNDER              0x400
#define X_ATTRIBUTE_EVENT_MASK              0x800
#define X_ATTRIBUTE_DO_NOT_PROPAGATE_MASK   0x1000
#define X_ATTRIBUTE_COLORMAP                0x2000
#define X_ATTRIBUTE_CURSOR                  0x4000

#define X_EVENT_KEY_PRESS                   0x01
#define X_EVENT_KEY_RELEASE                 0x02
#define X_EVENT_BUTTON_PRESS                0x04
#define X_EVENT_BUTTON_RELEASE              0x08
#define X_EVENT_ENTER_WINDOW                0x10
#define X_EVENT_LEAVE_WINDOW                0x20
#define X_EVENT_POINTER_MOTION              0x40
#define X_EVENT_POINTER_MOTION_HINT         0x80
#define X_EVENT_BUTTON_1_MOTION             0x100
#define X_EVENT_BUTTON_2_MOTION             0x200
#define X_EVENT_BUTTON_3_MOTION             0x400
#define X_EVENT_BUTTON_4_MOTION             0x800
#define X_EVENT_BUTTON_5_MOTION             0x1000
#define X_EVENT_BUTTON_MOTION               0x2000
#define X_EVENT_KEYMAP_STATE                0x4000
#define X_EVENT_EXPOSURE                    0x8000
#define X_EVENT_VISIBILITY_CHANGE           0x10000
#define X_EVENT_STRUCTURE_NOTIFY            0x20000
#define X_EVENT_RESIZE_REDIRECT             0x40000
#define X_EVENT_SUBSTRUCTURE_NOTIFY         0x80000
#define X_EVENT_SUBSTRUCTURE_REDIRECT       0x100000
#define X_EVENT_FOCUS_CHANGE                0x200000
#define X_EVENT_PROPERTY_CHANGE             0x400000
#define X_EVENT_COLORMAP_CHANGE             0x800000
#define X_EVENT_OWNER_GRAB_BUTTON           0x1000000

#define REQUEST_CREATEWINDOW                0
#define REQUEST_DESTROYWINDOW               4
#define REQUEST_MAPWINDOW                   10

#define ASYNC_POINTER                       0
#define SYNC_POINTER                        1
#define REPLAY_POINTER                      2
#define ASYNC_KEYBOARD                      3
#define SYNC_KEYBOARD                       4
#define REPLAY_KEYBOARD                     5
#define ASYNC_BOTH                          6
#define SYNC_BOTH                           7
#endif