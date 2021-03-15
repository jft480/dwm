/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
/* static unsigned int borderpx  = 3;        /1* border pixel of windows *1/ */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
/* static char *fonts[]          = { "monospace:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true"  }; */
static char *fonts[]          = { "Inconsolata-g:size=12", "JoyPixels:pixelsize=10:antialias=true:autohint=true"  };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;

const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ TERMCLASS,   NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ NULL,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,      "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */
 	{ "[]=",	tile },			/* Default: Master on left, slaves on right */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
 	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION) \
    { MOD, XK_d,                    ACTION##stack,  {.i = INC(-1) } }, \
    { MOD, XK_h,                    ACTION##stack,  {.i = 0 } }, \
    { MOD, XK_t,                    ACTION##stack,  {.i = 1 } }, \
    { MOD, XK_n,                    ACTION##stack,  {.i = 2 } }, \
    { MOD, XK_s,                    ACTION##stack,  {.i = PREVSEL } }, \
    /* { MOD, XK_t,                    ACTION##stack,  {.i = INC(-1) } }, \ */
    /* { MOD, XK_n,                    ACTION##stack,  {.i = INC(+1) } }, \ */
    /* { MOD, XK_space,                ACTION##stack,  {.i = PREVSEL } }, \ */
    /* { MOD, XK_ampersand,            ACTION##stack,  {.i = 1 } }, \ */
    /* { MOD, XK_bracketleft,          ACTION##stack,  {.i = 2 } }, \ */
    /* { MOD, XK_braceleft,            ACTION##stack,  {.i = 3 } }, \ */
    /* { MOD, XK_braceright,           ACTION##stack,  {.i = 4 } }, \ */
    /* { MOD, XK_parenright,           ACTION##stack,  {.i = 5 } }, \ */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

ResourcePref resources[] = {
        { "dwm.color0",      STRING, &normbordercolor },
        { "dwm.color17",     STRING, &selbordercolor },
        { "dwm.color0",      STRING, &normbgcolor },
        { "dwm.color15",     STRING, &normfgcolor },
        { "dwm.color17",     STRING, &selfgcolor },
        { "dwm.color16",     STRING, &selbgcolor }, /* topbar bg color*/
        { "borderpx",        INTEGER, &borderpx },
        { "snap",            INTEGER, &snap },
        { "showbar",         INTEGER, &showbar },
        { "topbar",          INTEGER, &topbar },
        { "nmaster",         INTEGER, &nmaster },
        { "resizehints",     INTEGER, &resizehints },
        { "mfact",           FLOAT,  &mfact },
        { "gappih",          INTEGER, &gappih },
        { "gappiv",          INTEGER, &gappiv },
        { "gappoh",          INTEGER, &gappoh },
        { "gappov",          INTEGER, &gappov },
        { "swallowfloating", INTEGER, &swallowfloating },
        { "smartgaps",       INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
	/* modifier                     key                 function        argument */

	STACKKEYS(MODKEY,                                   focus)
	STACKKEYS(MODKEY|ShiftMask,                         push)

	{ MODKEY,			            XK_BackSpace,	    spawn,		    SHCMD("sysact") },
	{ MODKEY|ShiftMask,			    XK_BackSpace,	    killclient,		{0} },

	/* { MODKEY,			            XK_Tab,		        view,		    {0} }, /1* Switch to previous tag *1/ */
	{ MODKEY,			            XK_Tab,		        setlayout,	    {.v = &layouts[5]} }, /* monocle */
	{ MODKEY|ShiftMask,		        XK_Tab,		        setlayout,	    {.v = &layouts[0]} }, /* bstack */
	{ MODKEY|ControlMask,			XK_Tab,		        setmfact,	    {.f = -0.05} },
	{ MODKEY,			            XK_comma,		    shiftview,	    { .i = -1 } },
	{ MODKEY|ShiftMask,		        XK_comma,		    shifttag,	    { .i = -1 } },
	{ MODKEY,			            XK_period,	        shiftview,	    { .i = 1 } },
	{ MODKEY|ShiftMask,		        XK_period,	        shifttag,	    { .i = 1 } },
	{ MODKEY,			            XK_f,		        togglefullscr,	{0} },
	{ MODKEY,			            XK_backslash,		setlayout,	    {.v = &layouts[1]} }, /* tile */
	{ MODKEY|ShiftMask,		        XK_backslash,		setlayout,	    {.v = &layouts[4]} }, /* deck */
	{ MODKEY|ControlMask,	        XK_backslash,	    setmfact,      	{.f = +0.05} },

	{ MODKEY,			            XK_a,		        togglegaps,	    {0} },
	{ MODKEY|ShiftMask,		        XK_a,		        defaultgaps,	{0} },
    { MODKEY,                       XK_o,               view,           {.ui = ~0 } }, /* Show all tags */
    { MODKEY|ShiftMask,             XK_o,               tag,            {.ui = ~0 } }, /* Stick to tags 1-9 */
	{ MODKEY,			            XK_i,		        incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,		        XK_i,		        incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,			    XK_d,		        spawn,          SHCMD("dmenu_run") },
    TAGKEYS(                        XK_minus,           8)

    TAGKEYS(                        XK_g,               0)
    TAGKEYS(                        XK_c,               1)
    TAGKEYS(                        XK_r,               2)
    TAGKEYS(                        XK_l,               3)
	{ MODKEY|ControlMask,			XK_l,		        togglesticky,	{0} },
    TAGKEYS(                        XK_m,               4)
    TAGKEYS(                        XK_w,               5)
    TAGKEYS(                        XK_v,               6)
    TAGKEYS(                        XK_z,               7)


	{ MODKEY,			            XK_Return,	        spawn,		    {.v = termcmd } },
	{ MODKEY|ShiftMask,		        XK_Return,	        togglescratch,	{.ui = 0} },

	{ MODKEY,			            XK_apostrophe,	    togglescratch,	{.ui = 1} },

	{ MODKEY,			            XK_k,	            spawn,		    SHCMD("xscreensaver-command --activate") },
	{ MODKEY|ControlMask,			XK_x,		        incrgaps,	    {.i = -3 } },
	{ MODKEY,			            XK_b,		        togglebar,	    {0} },

	{ MODKEY|ControlMask,			XK_z,		        incrgaps,	    {.i = +3 } },

	{ MODKEY,			            XK_space,		    view,		    {0} }, /* Switch to previous tag */
	/* { MODKEY,			            XK_space,	        view,		    {0} }, /1* Switch to previous tag *1/ */





	{ MODKEY,			XK_Left,	focusmon,	{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_Left,	tagmon,		{.i = -1 } },
	{ MODKEY,			XK_Right,	focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_Right,	tagmon,		{.i = +1 } },

	{ MODKEY,			XK_Page_Up,	shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,		XK_Page_Up,	shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_Page_Down,	shiftview,	{ .i = +1 } },
	{ MODKEY|ShiftMask,		XK_Page_Down,	shifttag,	{ .i = +1 } },
	{ MODKEY,			XK_Insert,	spawn,		SHCMD("xdotool type $(cat ~/.local/share/larbs/snippets | dmenu -i -l 50 | cut -d' ' -f1)") },

	{ MODKEY,			XK_F1,		spawn,		SHCMD("groff -mom /usr/local/share/dwm/larbs.mom -Tpdf | zathura -") },
	{ MODKEY,			XK_F2,		spawn,		SHCMD("tutorialvids") },
	{ MODKEY,			XK_F3,		spawn,		SHCMD("displayselect") },
	{ MODKEY,			XK_F4,		spawn,		SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") },
	/* { MODKEY,			XK_F5,		xrdb,		{.v = NULL } }, */
	{ MODKEY,			XK_F6,		spawn,		SHCMD("torwrap") },
	{ MODKEY,			XK_F7,		spawn,		SHCMD("td-toggle") },
	{ MODKEY,			XK_F8,		spawn,		SHCMD("mw -Y") },
	{ MODKEY,			XK_F9,		spawn,		SHCMD("dmenumount") },
	{ MODKEY,			XK_F10,		spawn,		SHCMD("dmenuumount") },
	{ MODKEY,			XK_F11,		spawn,		SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
	/* { MODKEY,			XK_F12,		xrdb,		{.v = NULL } }, */
	/* { MODKEY,			XK_space,	zoom,		{0} }, */
	/* { MODKEY|ShiftMask,		XK_space,	togglefloating,	{0} }, */

	{ 0,				XK_Print,	spawn,		SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,			XK_Print,	spawn,		SHCMD("maimpick") },
	{ MODKEY,			XK_Print,	spawn,		SHCMD("dmenurecord") },
	{ MODKEY|ShiftMask,		XK_Print,	spawn,		SHCMD("dmenurecord kill") },
	{ MODKEY,			XK_Delete,	spawn,		SHCMD("dmenurecord kill") },
	{ MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey &") },

	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,		spawn,		SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("mpc play") },
	{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,	spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,	spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	{ 0, XF86XK_AudioMicMute,	spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0, XF86XK_PowerOff,		spawn,		SHCMD("sysact") },
	{ 0, XF86XK_Calculator,		spawn,		SHCMD(TERMINAL " -e bc -l") },
	{ 0, XF86XK_Sleep,		spawn,		SHCMD("sudo -A zzz") },
	{ 0, XF86XK_WWW,		spawn,		SHCMD("$BROWSER") },
	{ 0, XF86XK_DOS,		spawn,		SHCMD(TERMINAL) },
	{ 0, XF86XK_ScreenSaver,	spawn,		SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
	{ 0, XF86XK_TaskPane,		spawn,		SHCMD(TERMINAL " -e htop") },
	{ 0, XF86XK_Mail,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0, XF86XK_MyComputer,		spawn,		SHCMD(TERMINAL " -e lf /") },
	/* { 0, XF86XK_Battery,		spawn,		SHCMD("") }, */
	{ 0, XF86XK_Launch1,		spawn,		SHCMD("xset dpms force off") },
	{ 0, XF86XK_TouchpadToggle,	spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOff,	spawn,		SHCMD("synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOn,		spawn,		SHCMD("synclient TouchpadOff=0") },
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("xbacklight -inc 15") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("xbacklight -dec 15") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
	{ ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button2,	togglebar,	{0} },
};




/* #define STACKKEYS(MOD,ACTION) \ */
/* 	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \ */
/* 	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \ */
/* 	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \ */
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */



/*
 * Xresources preferences to load at startup
 */
/* ResourcePref resources[] = { */
/* 		{ "color0",		STRING,	&normbordercolor }, */
/* 		{ "color8",		STRING,	&selbordercolor }, */
/* 		{ "color0",		STRING,	&normbgcolor }, */
/* 		{ "color4",		STRING,	&normfgcolor }, */
/* 		{ "color0",		STRING,	&selfgcolor }, */
/* 		{ "color4",		STRING,	&selbgcolor }, */
/* 		{ "borderpx",		INTEGER, &borderpx }, */
/* 		{ "snap",		INTEGER, &snap }, */
/* 		{ "showbar",		INTEGER, &showbar }, */
/* 		{ "topbar",		INTEGER, &topbar }, */
/* 		{ "nmaster",		INTEGER, &nmaster }, */
/* 		{ "resizehints",	INTEGER, &resizehints }, */
/* 		{ "mfact",		FLOAT,	&mfact }, */
/* 		{ "gappih",		INTEGER, &gappih }, */
/* 		{ "gappiv",		INTEGER, &gappiv }, */
/* 		{ "gappoh",		INTEGER, &gappoh }, */
/* 		{ "gappov",		INTEGER, &gappov }, */
/* 		{ "swallowfloating",	INTEGER, &swallowfloating }, */
/* 		{ "smartgaps",		INTEGER, &smartgaps }, */
/* }; */


    /* $&[{}(=*)+]!# */
    /* ~%7531902468` */

    /* TAGKEYS(                XK_ampersand,       0) */
    /* TAGKEYS(                XK_bracketleft,     1) */
    /* TAGKEYS(                XK_braceleft,       2) */
    /* TAGKEYS(                XK_braceright,      3) */
    /* TAGKEYS(                XK_parenleft,       4) */
    /* TAGKEYS(                XK_equal,           5) */
    /* TAGKEYS(                XK_asterisk,        6) */
    /* TAGKEYS(                XK_parenright,      7) */
    /* TAGKEYS(                XK_plus,            8) */
    /* { MODKEY,               XK_bracketright,    view,   {.ui = ~0 } }, /1* Show all tags *1/ */
    /* { MODKEY|ShiftMask,     XK_bracketright,    tag,    {.ui = ~0 } }, /1* Stick to tags 1-9 *1/ */
