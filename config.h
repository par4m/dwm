/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 0;     /* 0 means bottom bar */
static const char *fonts[]          = { "curie:style=Regular" };
static const char dmenufont[]       = "curie:style=Regular";
//
// The color schemes in dwm by default defines:
//
//     a foreground color (e.g. text color)
//
//     a background color (used by the bar) and
//
//     a border color (used for the border around windows)
//
// If we take SchemeSel as an example, the default configuration has this as:
//
//     [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
//
// where col_cyan is simply a variable:
//
// static const char col_cyan[]        = "#005577";
//
// Many people starting out get confused by this and they end up changing the variable, e.g.
//
// static const char col_cyan[]        = "#FF0000"; // it's red
//
// So now col_cyan no longer has the color of cyan, and both the bar background and the window border for the selected client will be red because the variable is used in two places.
//
// I suppose the idea is that you create your own variables and substitute them in the scheme as you want, e.g.
//
// static const char col_red[]         = "#FF0000";
//
// and
//
//     [SchemeSel]  = { col_gray4, col_red,  col_cyan  },
//
// or if you do not want to use variables then you can just add the colors directly, e.g.
//
//     [SchemeSel]  = { "#FFFFFF", "#FF0000",  "#00FF00"  },
//
static const char col_gray1[]       = "#2E3440"; // not selected bg
static const char col_gray2[]       = "#3B4252"; // not selected border
static const char col_gray3[]       = "#bebebe"; // not selected fg
static const char col_gray4[]       = "#282828"; // selected tag fg
static const char col_cyan[]        = "#81A1C1"; // selected tag bg and border 
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
	[SchemeTabActive]  = { col_gray3 , col_gray1 ,  col_gray2 }, // selected tab 
	[SchemeTabInactive]  = { col_gray3 ,col_gray1 , col_gray2 } // not selected tab
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     iscentered   isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           1,           -1 },
	{ "firefox",  NULL,       NULL,       1,            0,           0,           -1 },
	{ "firefoxdeveloperedition",  NULL,       NULL,       1,            0,           0,           -1 },
	{ "Brave-browser",  NULL,       NULL,       1,            0,           0,           -1 },
	{ "Chromium",  NULL,       NULL,       1,            0,           0,           -1 },
  //
	{ "St",       NULL,       NULL,       2,            0,           0,           -1 },
	{ "kitty",  NULL,       NULL,         2,            0,           0,           -1 }, 
	{ "Termite",  NULL,       NULL,         2,            0,           0,           -1 }, 
	{ "Alacritty",  NULL,       NULL,         2,            0,           0,           -1 }, 
  //
	{ "obsidian",  NULL,       NULL,       1 << 2,            0,           0,           -1 }, 
	{ "VNote",  NULL,       NULL,       1 << 2,            0,           0,           -1 },
	{ "Zathura",  NULL,       NULL,       1 << 2,            0,           0,           -1 }, 
  //
	// { "Thunar",  NULL,       NULL,       1 << 3,            0,           0,           -1 },  // added to scratchpad
  //
  //
	{ "Spotify",  NULL,       NULL,       1 << 4,            0,           0,           -1 }, // Not working
	{ "spotify",  NULL,       NULL,       1 << 4,            0,           0,           -1 }, // Not working
	{ "Youtube Music",  NULL,       NULL,       1 << 4,            0,           0,           -1 },
  //
	{ "discord",  NULL,       NULL,       1 << 5,            0,           0,           -1 }, 
	{ "TelegramDesktop",  NULL,       NULL,       1 << 5,            0,           0,           -1 }, 
  // Scratchpad
	// { "Lxtask",  NULL,       NULL,       SCRATCHPAD_MASK_1,            0,           1,           -1 }, 
	// { "Thunar",  NULL,       NULL,       SCRATCHPAD_MASK_2,            0,           1,           -1 }, 
	// { "Emacs",  NULL,       NULL,       SCRATCHPAD_MASK_3,            0,           1,           -1 }, 
	// { "obsidian",  NULL,       NULL,       SCRATCHPAD_MASK_4,            0,           1,           -1 }, 
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

/* Bartabgroups properties */
#define BARTAB_BORDERS 1       // 0 = off, 1 = on
#define BARTAB_BOTTOMBORDER 0  // 0 = off, 1 = on
#define BARTAB_TAGSINDICATOR 1 // 0 = off, 1 = on if >1 client/view tag, 2 = always on
#define BARTAB_TAGSPX 5        // # pixels for tag grid boxes
#define BARTAB_TAGSROWS 3      // # rows in tag grid (9 tags, e.g. 3x3)
static void (*bartabmonfns[])(Monitor *) = { monocle /* , customlayoutfn */ };
static void (*bartabfloatfns[])(Monitor *) = { NULL /* , customlayoutfn */ };

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
// #define MODKEY Mod1Mask
// Mod4 - Windows Key | Mod1 - Alt
#define MODKEY Mod4Mask // windows key as mod
#define MODKEY2 Mod1Mask // alt as mod
#define MODKEY3 Mod3Mask // ctrl as mod // not working
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *upvol[]               = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%",     NULL };
static const char *downvol[]             = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%",     NULL };
static const char *mutevol[]             = { "/usr/bin/pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle",  NULL };
// static const char *dmenu_bluetooth[] = {"dblue -b -fn 'curie' -nb '#2E3440' -sf '#282828' -sb '#81A1C1' -nf '#bebebe'", NULL};
// Media def
//
// playprev and playnext not working ;; Todo
// static const char *playprev[]               = { "/usr/bin/dbus-send --print-reply --dest=org.mpris.MediaPlayer2.playerctld /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Previous", NULL };
// static const char *playnext[]               = { "/usr/bin/dbus-send --print-reply --dest=org.mpris.MediaPlayer2.playerctld /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Next", NULL };
//
static const char *playpause[]               = { "/usr/bin/playerctl", "play-pause", NULL };
// static const char *runspotify[]               = { "LD_PRELOAD=/usr/local/lib/spotify-adblock.so spotify", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          SHCMD("j4-menu") },
	{ MODKEY2,                       XK_b,      spawn,          SHCMD("dblue -b -fn 'curie' -nb '#2E3440' -sf '#282828' -sb '#81A1C1' -nf '#bebebe'") },
	{ MODKEY,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_Tab,       shiftviewclients, { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_backslash, shiftviewclients, { .i = -1 } },
	{ MODKEY2|ShiftMask,             XK_s,    spawn,           SHCMD("skippy-xd") },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      layoutscroll,   {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      layoutscroll,   {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,    spawn,           SHCMD("pkill dwm") }, // WindowsKey + Shift + e - kill dwm
	{ MODKEY2|ShiftMask,             XK_r,      quit,           {0} }, // Alt + Shift + r - restart dwm
  // Volume Keys
  { 0,                            XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
	{ 0,                            XF86XK_AudioMute, spawn, {.v = mutevol } },
	{ MODKEY,                       XK_minus, spawn, {.v = downvol } },
	{ MODKEY,                       XK_equal, spawn, {.v = upvol   } },
	{ MODKEY,                       XK_BackSpace, spawn, {.v = mutevol } },
  // Media + Spotify keys
	// { MODKEY|ShiftMask ,             XK_v,    spawn,           SHCMD("LD_PRELOAD=/usr/local/lib/spotify-adblock.so spotify") }, // WindowsKey+Shift+v - Run spotify
	{ MODKEY|ShiftMask ,             XK_v,    spawn,           SHCMD("spotify") }, // WindowsKey+Shift+v - Run spotify
	{ MODKEY2 ,             XK_space,    spawn,           {.v = playpause } }, // Alt + Space - Play / Pause
	{ MODKEY2,             XK_period,    spawn,           SHCMD("/usr/bin/dbus-send --print-reply --dest=org.mpris.MediaPlayer2.playerctld /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Next") }, // Alt + . - Next
	{ MODKEY2 ,             XK_comma,    spawn,           SHCMD("/usr/bin/dbus-send --print-reply --dest=org.mpris.MediaPlayer2.playerctld /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Previous") }, // Alt + , - Previous
  // Scripts etc
	{ MODKEY2 ,             XK_q,    spawn,           SHCMD("books") }, // Alt + q - Rofi Books
	{ MODKEY2 ,             XK_z,    spawn,           SHCMD("screenshot") }, // Alt + z - screenshot
	{ MODKEY ,             XK_z,    spawn,           SHCMD("rofi -drun-use-desktop-cache -no-lazy-grab -show drun -modi drun") }, // WindowsKey + z - screenshot
	{ MODKEY2 ,             XK_Return,    spawn,           SHCMD("kitty --single-instance") }, // Alt + Enter - kitty
	{ MODKEY2|ShiftMask ,             XK_l,    spawn,           SHCMD("betterlockscreen -l") }, // Alt + l - betterlockscreen lock
	{ MODKEY2 ,             XK_w,    spawn,           SHCMD("firefox") }, // Alt + w - firefox
	{ MODKEY2 ,             XK_t,    spawn,           SHCMD("lxtask") }, // Alt + t - lxtask
	{ MODKEY2 ,             XK_o,    spawn,           SHCMD("obsidian") }, // Alt + o - obsidian
	{ MODKEY2 ,             XK_n,    spawn,           SHCMD("networkmanager_dmenu") }, // Alt + n - Dmenu network manager
	{ MODKEY2 ,             XK_e,    spawn,           SHCMD("emacsclient -c") }, // Alt + e - Doom Emacs
	{ MODKEY2|ShiftMask ,             XK_k,    spawn,           SHCMD("thunar") }, // Alt + k - Thunar
	{ MODKEY2 ,             XK_s,       shiftviewclients, { .i = +1 } }, // Alt + s - next tag
	{ MODKEY2 ,             XK_a,       shiftviewclients, { .i = -1 } }, // Alt + s - previous tag
  // Scratchpad (hide = add to scratchpad)
    { MODKEY,                       XK_s, scratchpad_show, {.i = 1} },
    { MODKEY,                       XK_y, scratchpad_show, {.i = 2} },
    { MODKEY,                       XK_u, scratchpad_show, {.i = 3} },
    { MODKEY,                       XK_a, scratchpad_show, {.i = 4} },
    { MODKEY|ShiftMask,             XK_s, scratchpad_hide, {.i = 1} },
    { MODKEY|ShiftMask,             XK_y, scratchpad_hide, {.i = 2} },
    { MODKEY|ShiftMask,             XK_u, scratchpad_hide, {.i = 3} },
    { MODKEY|ShiftMask,             XK_a, scratchpad_hide, {.i = 4} },
	  { MODKEY|ShiftMask,             XK_r, scratchpad_remove, {0} },


  //
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,      dragmfact,      {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

