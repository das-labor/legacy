/* 
 * Copyright © 2001 Keith Packard, member of The XFree86 Project, Inc.
 * Copyright © 2002 Hewlett Packard Company, Inc.
 * Copyright © 2006 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 * Thanks to Jim Gettys who wrote most of the client side code,
 * and part of the server code for randr.
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xrender.h>	/* we share subpixel information */
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "xrandr.h"

#if RANDR_MAJOR > 1 || (RANDR_MAJOR == 1 && RANDR_MINOR >= 2)
#define HAS_RANDR_1_2 1
#endif

static char	*program_name;
static Display	*dpy;
static Window	root;
static int	screen = -1;
static Bool	verbose = False;
static Bool	automatic = False;
//static Bool	properties = False;

static char *direction[5] = {
    "normal", 
    "left", 
    "inverted", 
    "right",
    "\n"};
#if 0
static char *reflections[5] = {
    "normal", 
    "x", 
    "y", 
    "xy",
    "\n"};

/* subpixel order */
static char *order[6] = {
    "unknown",
    "horizontal rgb",
    "horizontal bgr",
    "vertical rgb",
    "vertical bgr",
    "no subpixels"};
#endif
static const struct {
    char	    *string;
    unsigned long   flag;
} mode_flags[] = {
    { "+HSync", RR_HSyncPositive },
    { "-HSync", RR_HSyncNegative },
    { "+VSync", RR_VSyncPositive },
    { "-VSync", RR_VSyncNegative },
    { "Interlace", RR_Interlace },
    { "DoubleScan", RR_DoubleScan },
    { "CSync",	    RR_CSync },
    { "+CSync",	    RR_CSyncPositive },
    { "-CSync",	    RR_CSyncNegative },
    { NULL,	    0 }
};

static void
fatal (const char *format, ...)
{
    va_list ap;
    
    va_start (ap, format);
    fprintf (stderr, "%s: ", program_name);
    vfprintf (stderr, format, ap);
    va_end (ap);
    exit (1);
    /*NOTREACHED*/
}

static void
warning (const char *format, ...)
{
    va_list ap;
    
    va_start (ap, format);
    fprintf (stderr, "%s: ", program_name);
    vfprintf (stderr, format, ap);
    va_end (ap);
}

static char *
rotation_name (Rotation rotation)
{
    int	i;

    if ((rotation & 0xf) == 0)
	return "normal";
    for (i = 0; i < 4; i++)
	if (rotation & (1 << i))
	    return direction[i];
    return "invalid rotation";
}

static char *
reflection_name (Rotation rotation)
{
    rotation &= (RR_Reflect_X|RR_Reflect_Y);
    switch (rotation) {
    case 0:
	return "none";
    case RR_Reflect_X:
	return "X axis";
    case RR_Reflect_Y:
	return "Y axis";
    case RR_Reflect_X|RR_Reflect_Y:
	return "X and Y axis";
    }
    return "invalid reflection";
}

#if HAS_RANDR_1_2
typedef enum _policy {
    clone, extend
} policy_t;

typedef enum _relation {
    left_of, right_of, above, below, same_as,
} relation_t;

typedef struct {
    int	    x, y, width, height;
} rectangle_t;

typedef struct {
    int	    x1, y1, x2, y2;
} box_t;

typedef struct {
    int	    x, y;
} point_t;

typedef enum _changes {
    changes_none = 0,
    changes_crtc = (1 << 0),
    changes_mode = (1 << 1),
    changes_relation = (1 << 2),
    changes_position = (1 << 3),
    changes_rotation = (1 << 4),
    changes_reflection = (1 << 5),
    changes_automatic = (1 << 6),
    changes_refresh = (1 << 7),
    changes_property = (1 << 8),
    changes_transform = (1 << 9),
} changes_t;

typedef enum _name_kind {
    name_none = 0,
    name_string = (1 << 0),
    name_xid = (1 << 1),
    name_index = (1 << 2),
    name_preferred = (1 << 3),
} name_kind_t;

typedef struct {
    name_kind_t	    kind;
    char    	    *string;
    XID	    	    xid;
    int		    index;
} name_t;

typedef struct _crtc crtc_t;
typedef struct _output	output_t;
typedef struct _transform transform_t;
typedef struct _umode	umode_t;
typedef struct _output_prop output_prop_t;

struct _transform {
    XTransform	    transform;
    char	    *filter;
    int		    nparams;
    XFixed	    *params;
};

struct _crtc {
    name_t	    crtc;
    Bool	    changing;
    XRRCrtcInfo	    *crtc_info;

    XRRModeInfo	    *mode_info;
    int		    x;
    int		    y;
    Rotation	    rotation;
    output_t	    **outputs;
    int		    noutput;
    transform_t	    current_transform, pending_transform;
};

struct _output_prop {
    struct _output_prop	*next;
    char		*name;
    char		*value;
};

struct _output {
    struct _output   *next;
    
    changes_t	    changes;
    
    output_prop_t   *props;

    name_t	    output;
    XRROutputInfo   *output_info;
    
    name_t	    crtc;
    crtc_t	    *crtc_info;
    crtc_t	    *current_crtc_info;
    
    name_t	    mode;
    float	    refresh;
    XRRModeInfo	    *mode_info;
    
    name_t	    addmode;

    relation_t	    relation;
    char	    *relative_to;

    int		    x, y;
    Rotation	    rotation;
    
    Bool    	    automatic;
    transform_t	    transform;
};

typedef enum _umode_action {
    umode_create, umode_destroy, umode_add, umode_delete
} umode_action_t;


struct _umode {
    struct _umode   *next;
    
    umode_action_t  action;
    XRRModeInfo	    mode;
    name_t	    output;
    name_t	    name;
};
#if 0
static char *connection[3] = {
    "connected",
    "disconnected",
    "unknown connection"};
#endif
#define OUTPUT_NAME 1

#define CRTC_OFF    2
#define CRTC_UNSET  3
#define CRTC_INDEX  0x40000000

#define MODE_NAME   1
#define MODE_OFF    2
#define MODE_UNSET  3
#define MODE_PREF   4

#define POS_UNSET   -1

static output_t	*outputs = NULL;
static output_t	**outputs_tail = &outputs;
static crtc_t	*crtcs;
static umode_t	*umodes;
static int	num_crtcs;
static XRRScreenResources  *res;
static int	fb_width = 0, fb_height = 0;
static int	fb_width_mm = 0, fb_height_mm = 0;
static float	dpi = 0;
//static char	*dpi_output = NULL;
static Bool	dryrun = False;
static int	minWidth, maxWidth, minHeight, maxHeight;
static Bool    	has_1_2 = False;

static int
mode_height (XRRModeInfo *mode_info, Rotation rotation)
{
    switch (rotation & 0xf) {
    case RR_Rotate_0:
    case RR_Rotate_180:
	return mode_info->height;
    case RR_Rotate_90:
    case RR_Rotate_270:
	return mode_info->width;
    default:
	return 0;
    }
}

static int
mode_width (XRRModeInfo *mode_info, Rotation rotation)
{
    switch (rotation & 0xf) {
    case RR_Rotate_0:
    case RR_Rotate_180:
	return mode_info->width;
    case RR_Rotate_90:
    case RR_Rotate_270:
	return mode_info->height;
    default:
	return 0;
    }
}

Bool
transform_point (XTransform *transform, double *xp, double *yp)
{
    double  vector[3];
    double  result[3];
    int	    i, j;
    double v; // double  partial

    vector[0] = *xp;
    vector[1] = *yp;
    vector[2] = 1;
    for (j = 0; j < 3; j++)
    {
	v = 0;
	for (i = 0; i < 3; i++)
	    v += (XFixedToDouble (transform->matrix[j][i]) * vector[i]);
	if (v > 32767 || v < -32767)
	    return False;
	result[j] = v;
    }
    if (!result[2])
	return False;
    for (j = 0; j < 2; j++)
	vector[j] = result[j] / result[2];
    *xp = vector[0];
    *yp = vector[1];
    return True;
}

Bool
path_bounds (XTransform *transform, point_t *points, int npoints, box_t *box)
{
    int	    i;
    box_t   point;

    for (i = 0; i < npoints; i++) {
	double	x, y;
	x = points[i].x;
	y = points[i].y;
	transform_point (transform, &x, &y);
	point.x1 = floor (x);
	point.y1 = floor (y);
	point.x2 = ceil (x);
	point.y2 = ceil (y);
	if (i == 0)
	    *box = point;
	else {
	    if (point.x1 < box->x1) box->x1 = point.x1;
	    if (point.y1 < box->y1) box->y1 = point.y1;
	    if (point.x2 > box->x2) box->x2 = point.x2;
	    if (point.y2 > box->y2) box->y2 = point.y2;
	}
    }
    return True;
}

static void
mode_geometry (XRRModeInfo *mode_info, Rotation rotation,
	       XTransform *transform,
	       box_t *bounds)
{
    point_t rect[4];
    int	width = mode_width (mode_info, rotation);
    int height = mode_height (mode_info, rotation);

    rect[0].x = 0;
    rect[0].y = 0;
    rect[1].x = width;
    rect[1].y = 0;
    rect[2].x = width;
    rect[2].y = height;
    rect[3].x = 0;
    rect[3].y = height;
    path_bounds (transform, rect, 4, bounds);
}

/* v refresh frequency in Hz */
static float
mode_refresh (XRRModeInfo *mode_info)
{
    float rate;
    
    if (mode_info->hTotal && mode_info->vTotal)
	rate = ((float) mode_info->dotClock / 
		((float) mode_info->hTotal * (float) mode_info->vTotal));
    else
    	rate = 0;
    return rate;
}


static void
init_name (name_t *name)
{
    name->kind = name_none;
}

static void
set_name_string (name_t *name, char *string)
{
    name->kind |= name_string;
    name->string = string;
}

static void
set_name_xid (name_t *name, XID xid)
{
    name->kind |= name_xid;
    name->xid = xid;
}

static void
set_name_index (name_t *name, int index)
{
    name->kind |= name_index;
    name->index = index;
}

static void
set_name_preferred (name_t *name)
{
    name->kind |= name_preferred;
}

static void
set_name_all (name_t *name, name_t *old)
{
    if (old->kind & name_xid)
	name->xid = old->xid;
    if (old->kind & name_string)
	name->string = old->string;
    if (old->kind & name_index)
	name->index = old->index;
    name->kind |= old->kind;
}

static void
set_name (name_t *name, char *string, name_kind_t valid)
{
    XID	xid;
    int index;

    if ((valid & name_xid) && sscanf (string, "0x%x", (unsigned int *)&xid) == 1)
	set_name_xid (name, xid);
    else if ((valid & name_index) && sscanf (string, "%d", &index) == 1)
	set_name_index (name, index);
    else if (valid & name_string)
	set_name_string (name, string);
    //else
	//usage ();
}

static void
init_transform (transform_t *transform)
{
    int x;
    memset (&transform->transform, '\0', sizeof (transform->transform));
    for (x = 0; x < 3; x++)
	transform->transform.matrix[x][x] = XDoubleToFixed (1.0);
    transform->filter = "";
    transform->nparams = 0;
    transform->params = NULL;
}

static void
set_transform (transform_t  *dest,
	       XTransform   *transform,
	       char	    *filter,
	       XFixed	    *params,
	       int	    nparams)
{
    dest->transform = *transform;
    dest->filter = strdup (filter);
    dest->nparams = nparams;
    dest->params = malloc (nparams * sizeof (XFixed));
    memcpy (dest->params, params, nparams * sizeof (XFixed));
}

static void
copy_transform (transform_t *dest, transform_t *src)
{
    set_transform (dest, &src->transform,
		   src->filter, src->params, src->nparams);
}

static Bool
equal_transform (transform_t *a, transform_t *b)
{
    if (memcmp (&a->transform, &b->transform, sizeof (XTransform)) != 0)
	return False;
    if (strcmp (a->filter, b->filter) != 0)
	return False;
    if (a->nparams != b->nparams)
	return False;
    if (memcmp (a->params, b->params, a->nparams * sizeof (XFixed)) != 0)
	return False;
    return True;
}

static output_t *
add_output (void)
{
    output_t *output = calloc (1, sizeof (output_t));

    if (!output)
	fatal ("out of memory");
    output->next = NULL;
    *outputs_tail = output;
    outputs_tail = &output->next;
    return output;
}

static output_t *
find_output (name_t *name)
{
    output_t *output;

    for (output = outputs; output; output = output->next)
    {
	name_kind_t common = name->kind & output->output.kind;
	
	if ((common & name_xid) && name->xid == output->output.xid)
	    break;
	if ((common & name_string) && !strcmp (name->string, output->output.string))
	    break;
	if ((common & name_index) && name->index == output->output.index)
	    break;
    }
    return output;
}

static output_t *
find_output_by_xid (RROutput output)
{
    name_t  output_name;

    init_name (&output_name);
    set_name_xid (&output_name, output);
    return find_output (&output_name);
}

static output_t *
find_output_by_name (char *name)
{
    name_t  output_name;

    init_name (&output_name);
    set_name_string (&output_name, name);
    return find_output (&output_name);
}

static crtc_t *
find_crtc (name_t *name)
{
    int	    c;
    crtc_t  *crtc = NULL;

    for (c = 0; c < num_crtcs; c++)
    {
	name_kind_t common;
	
	crtc = &crtcs[c];
	common = name->kind & crtc->crtc.kind;
	
	if ((common & name_xid) && name->xid == crtc->crtc.xid)
	    break;
	if ((common & name_string) && !strcmp (name->string, crtc->crtc.string))
	    break;
	if ((common & name_index) && name->index == crtc->crtc.index)
	    break;
	crtc = NULL;
    }
    return crtc;
}

static crtc_t *
find_crtc_by_xid (RRCrtc crtc)
{
    name_t  crtc_name;

    init_name (&crtc_name);
    set_name_xid (&crtc_name, crtc);
    return find_crtc (&crtc_name);
}

static XRRModeInfo *
find_mode (name_t *name, float refresh)
{
    int		m;
    XRRModeInfo	*best = NULL;
    float	bestDist = 0;

    for (m = 0; m < res->nmode; m++)
    {
	XRRModeInfo *mode = &res->modes[m];
	if ((name->kind & name_xid) && name->xid == mode->id)
	{
	    best = mode;
	    break;
	}
	if ((name->kind & name_string) && !strcmp (name->string, mode->name))
	{
	    float   dist;
	    
	    if (refresh)
		dist = fabs (mode_refresh (mode) - refresh);
	    else
		dist = 0;
	    if (!best || dist < bestDist)
	    {
		bestDist = dist;
		best = mode;
	    }
	    break;
	}
    }
    return best;
}

static XRRModeInfo *
find_mode_by_xid (RRMode mode)
{
    name_t  mode_name;

    init_name (&mode_name);
    set_name_xid (&mode_name, mode);
    return find_mode (&mode_name, 0);
}

static XRRModeInfo *
find_mode_by_name (char *name)
{
    name_t  mode_name;
    init_name (&mode_name);
    set_name_string (&mode_name, name);
    return find_mode (&mode_name, 0);
}

static
XRRModeInfo *
find_mode_for_output (output_t *output, name_t *name)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    m;
    XRRModeInfo	    *best = NULL;
    float	    bestDist = 0;

    for (m = 0; m < output_info->nmode; m++)
    {
	XRRModeInfo	    *mode;
	
	mode = find_mode_by_xid (output_info->modes[m]);
	if (!mode) continue;
	if ((name->kind & name_xid) && name->xid == mode->id)
	{
	    best = mode;
	    break;
	}
	if ((name->kind & name_string) && !strcmp (name->string, mode->name))
	{
	    float   dist;
	    
	    if (output->refresh)
		dist = fabs (mode_refresh (mode) - output->refresh);
	    else
		dist = 0;
	    if (!best || dist < bestDist)
	    {
		bestDist = dist;
		best = mode;
	    }
	}
    }
    return best;
}

XRRModeInfo *
preferred_mode (output_t *output)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    m;
    XRRModeInfo	    *best;
    int		    bestDist;
    
    best = NULL;
    bestDist = 0;
    for (m = 0; m < output_info->nmode; m++)
    {
	XRRModeInfo *mode_info = find_mode_by_xid (output_info->modes[m]);
	int	    dist;
	
	if (m < output_info->npreferred)
	    dist = 0;
	else if (output_info->mm_height)
	    dist = (1000 * DisplayHeight(dpy, screen) / DisplayHeightMM(dpy, screen) -
		    1000 * mode_info->height / output_info->mm_height);
	else
	    dist = DisplayHeight(dpy, screen) - mode_info->height;

        if (dist < 0) dist = -dist;
	if (!best || dist < bestDist)
	{
	    best = mode_info;
	    bestDist = dist;
	}
    }
    return best;
}

static Bool
output_can_use_crtc (output_t *output, crtc_t *crtc)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    c;

    for (c = 0; c < output_info->ncrtc; c++)
	if (output_info->crtcs[c] == crtc->crtc.xid)
	    return True;
    return False;
}

static Bool
output_can_use_mode (output_t *output, XRRModeInfo *mode)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    m;

    for (m = 0; m < output_info->nmode; m++)
	if (output_info->modes[m] == mode->id)
	    return True;
    return False;
}

static Bool
crtc_can_use_rotation (crtc_t *crtc, Rotation rotation)
{
    Rotation	rotations = crtc->crtc_info->rotations;
    Rotation	dir = rotation & (RR_Rotate_0|RR_Rotate_90|RR_Rotate_180|RR_Rotate_270);
    Rotation	reflect = rotation & (RR_Reflect_X|RR_Reflect_Y);
    if (((rotations & dir) != 0) && ((rotations & reflect) == reflect))
	return True;
    return False;
}

static Bool
output_can_use_rotation (output_t *output, Rotation rotation)
{
    XRROutputInfo   *output_info = output->output_info;
    int		    c;

    /* make sure all of the crtcs can use this rotation.
     * yes, this is not strictly necessary, but it is 
     * simpler,and we expect most drivers to either
     * support rotation everywhere or nowhere
     */
    for (c = 0; c < output_info->ncrtc; c++)
    {
	crtc_t	*crtc = find_crtc_by_xid (output_info->crtcs[c]);
	if (crtc && !crtc_can_use_rotation (crtc, rotation))
	    return False;
    }
    return True;
}

static void
set_output_info (output_t *output, RROutput xid, XRROutputInfo *output_info)
{
    /* sanity check output info */
    if (output_info->connection != RR_Disconnected && !output_info->nmode)
	warning ("Output %s is not disconnected but has no modes\n",
		 output_info->name);
    
    /* set output name and info */
    if (!(output->output.kind & name_xid))
	set_name_xid (&output->output, xid);
    if (!(output->output.kind & name_string))
	set_name_string (&output->output, output_info->name);
    output->output_info = output_info;
    
    /* set crtc name and info */
    if (!(output->changes & changes_crtc))
	set_name_xid (&output->crtc, output_info->crtc);
    
    if (output->crtc.kind == name_xid && output->crtc.xid == None)
	output->crtc_info = NULL;
    else
    {
	output->crtc_info = find_crtc (&output->crtc);
	if (!output->crtc_info)
	{
	    if (output->crtc.kind & name_xid)
		fatal ("cannot find crtc 0x%x\n", output->crtc.xid);
	    if (output->crtc.kind & name_index)
		fatal ("cannot find crtc %d\n", output->crtc.index);
	}
	if (!output_can_use_crtc (output, output->crtc_info))
	    fatal ("output %s cannot use crtc 0x%x\n", output->output.string,
		   output->crtc_info->crtc.xid);
    }

    /* set mode name and info */
    if (!(output->changes & changes_mode))
    {
	if (output->crtc_info)
	    set_name_xid (&output->mode, output->crtc_info->crtc_info->mode);
	else
	    set_name_xid (&output->mode, None);
	if (output->mode.xid)
	{
	    output->mode_info = find_mode_by_xid (output->mode.xid);
	    if (!output->mode_info)
		fatal ("server did not report mode 0x%x for output %s\n",
		       output->mode.xid, output->output.string);
	}
	else
	    output->mode_info = NULL;
    }
    else if (output->mode.kind == name_xid && output->mode.xid == None)
	output->mode_info = NULL;
    else
    {
	if (output->mode.kind == name_preferred)
	    output->mode_info = preferred_mode (output);
	else
	    output->mode_info = find_mode_for_output (output, &output->mode);
	if (!output->mode_info)
	{
	    if (output->mode.kind & name_preferred)
		fatal ("cannot find preferred mode\n");
	    if (output->mode.kind & name_string)
		fatal ("cannot find mode %s\n", output->mode.string);
	    if (output->mode.kind & name_xid)
		fatal ("cannot find mode 0x%x\n", output->mode.xid);
	}
	if (!output_can_use_mode (output, output->mode_info))
	    fatal ("output %s cannot use mode %s\n", output->output.string,
		   output->mode_info->name);
    }

    /* set position */
    if (!(output->changes & changes_position))
    {
	if (output->crtc_info)
	{
	    output->x = output->crtc_info->crtc_info->x;
	    output->y = output->crtc_info->crtc_info->y;
	}
	else
	{
	    output->x = 0;
	    output->y = 0;
	}
    }

    /* set rotation */
    if (!(output->changes & changes_rotation))
    {
	output->rotation &= ~0xf;
	if (output->crtc_info)
	    output->rotation |= (output->crtc_info->crtc_info->rotation & 0xf);
	else
	    output->rotation = RR_Rotate_0;
    }
    if (!(output->changes & changes_reflection))
    {
	output->rotation &= ~(RR_Reflect_X|RR_Reflect_Y);
	if (output->crtc_info)
	    output->rotation |= (output->crtc_info->crtc_info->rotation &
				 (RR_Reflect_X|RR_Reflect_Y));
    }
    if (!output_can_use_rotation (output, output->rotation))
	fatal ("output %s cannot use rotation \"%s\" reflection \"%s\"\n",
	       output->output.string,
	       rotation_name (output->rotation),
	       reflection_name (output->rotation));

    /* set transformation */
    if (!(output->changes & changes_transform))
    {
	if (output->crtc_info)
	    copy_transform (&output->transform, &output->crtc_info->current_transform);
	else
	    init_transform (&output->transform);
    }
}
    
static void
get_screen (void)
{
    if (!has_1_2)
        fatal ("Server RandR version before 1.2\n");
    
    XRRGetScreenSizeRange (dpy, root, &minWidth, &minHeight,
			   &maxWidth, &maxHeight);
    
    res = XRRGetScreenResources (dpy, root);
    if (!res) fatal ("could not get screen resources");
}

static void
get_crtcs (void)
{
    int		c;

    num_crtcs = res->ncrtc;
    crtcs = calloc (num_crtcs, sizeof (crtc_t));
    if (!crtcs) fatal ("out of memory");
    
    for (c = 0; c < res->ncrtc; c++)
    {
	XRRCrtcInfo *crtc_info = XRRGetCrtcInfo (dpy, res, res->crtcs[c]);
#if RANDR_MAJOR > 1 || RANDR_MINOR >= 3
	XRRCrtcTransformAttributes  *attr;
#endif
	//int	    x;
	set_name_xid (&crtcs[c].crtc, res->crtcs[c]);
	set_name_index (&crtcs[c].crtc, c);
	if (!crtc_info) fatal ("could not get crtc 0x%x information", res->crtcs[c]);
	crtcs[c].crtc_info = crtc_info;
	if (crtc_info->mode == None)
	{
	    crtcs[c].mode_info = NULL;
	    crtcs[c].x = 0;
	    crtcs[c].y = 0;
	    crtcs[c].rotation = RR_Rotate_0;
	}
#if RANDR_MAJOR > 1 || RANDR_MINOR >= 3
	XRRGetCrtcTransform (dpy, res->crtcs[c], &attr);
	if (attr) {
	    set_transform (&crtcs[c].current_transform,
			   &attr->currentTransform,
			   attr->currentFilter,
			   attr->currentParams,
			   attr->currentNparams);
	    XFree (attr);
	}
	else
#endif
	{
	    init_transform (&crtcs[c].current_transform);
	}
	copy_transform (&crtcs[c].pending_transform, &crtcs[c].current_transform);
   }
}

static void
crtc_add_output (crtc_t *crtc, output_t *output)
{
    if (crtc->outputs)
	crtc->outputs = realloc (crtc->outputs, (crtc->noutput + 1) * sizeof (output_t *));
    else
    {
	crtc->outputs = malloc (sizeof (output_t *));
	crtc->x = output->x;
	crtc->y = output->y;
	crtc->rotation = output->rotation;
	crtc->mode_info = output->mode_info;
	copy_transform (&crtc->pending_transform, &output->transform);
   }
    if (!crtc->outputs) fatal ("out of memory");
    crtc->outputs[crtc->noutput++] = output;
}

static void
set_crtcs (void)
{
    output_t	*output;

    for (output = outputs; output; output = output->next)
    {
	if (!output->mode_info) continue;
	crtc_add_output (output->crtc_info, output);
    }
}

static Status
crtc_disable (crtc_t *crtc)
{
    if (verbose)
    	printf ("crtc %d: disable\n", crtc->crtc.index);
	
    if (dryrun)
	return RRSetConfigSuccess;
    return XRRSetCrtcConfig (dpy, res, crtc->crtc.xid, CurrentTime,
			     0, 0, None, RR_Rotate_0, NULL, 0);
}

static void
crtc_set_transform (crtc_t *crtc, transform_t *transform)
{
    int	major, minor;

    XRRQueryVersion (dpy, &major, &minor);
    if (major > 1 || (major == 1 && minor >= 3))
	XRRSetCrtcTransform (dpy, crtc->crtc.xid,
			     &transform->transform,
			     transform->filter,
			     transform->params,
			     transform->nparams);
}

static Status
crtc_revert (crtc_t *crtc)
{
    XRRCrtcInfo	*crtc_info = crtc->crtc_info;
    
    if (verbose)
    	printf ("crtc %d: revert\n", crtc->crtc.index);
	
    if (dryrun)
	return RRSetConfigSuccess;

    crtc_set_transform (crtc, &crtc->current_transform);
    return XRRSetCrtcConfig (dpy, res, crtc->crtc.xid, CurrentTime,
			    crtc_info->x, crtc_info->y,
			    crtc_info->mode, crtc_info->rotation,
			    crtc_info->outputs, crtc_info->noutput);
}

static Status
crtc_apply (crtc_t *crtc)
{
    RROutput	*rr_outputs;
    int		o;
    Status	s;
    RRMode	mode = None;

    if (!crtc->changing || !crtc->mode_info)
	return RRSetConfigSuccess;

    rr_outputs = calloc (crtc->noutput, sizeof (RROutput));
    if (!rr_outputs)
	return BadAlloc;
    for (o = 0; o < crtc->noutput; o++)
	rr_outputs[o] = crtc->outputs[o]->output.xid;
    mode = crtc->mode_info->id;
    if (verbose) {
	printf ("crtc %d: %12s %6.1f +%d+%d", crtc->crtc.index,
		crtc->mode_info->name, mode_refresh (crtc->mode_info),
		crtc->x, crtc->y);
	for (o = 0; o < crtc->noutput; o++)
	    printf (" \"%s\"", crtc->outputs[o]->output.string);
	printf ("\n");
    }
    
    if (dryrun)
	s = RRSetConfigSuccess;
    else
    {
	crtc_set_transform (crtc, &crtc->pending_transform);
	s = XRRSetCrtcConfig (dpy, res, crtc->crtc.xid, CurrentTime,
			      crtc->x, crtc->y, mode, crtc->rotation,
			      rr_outputs, crtc->noutput);
    }
    free (rr_outputs);
    return s;
}

static void
screen_revert (void)
{
    if (verbose)
	printf ("screen %d: revert\n", screen);

    if (dryrun)
	return;
    XRRSetScreenSize (dpy, root,
		      DisplayWidth (dpy, screen),
		      DisplayHeight (dpy, screen),
		      DisplayWidthMM (dpy, screen),
		      DisplayHeightMM (dpy, screen));
}

static void
screen_apply (void)
{
    if (fb_width == DisplayWidth (dpy, screen) &&
	fb_height == DisplayHeight (dpy, screen) &&
	fb_width_mm == DisplayWidthMM (dpy, screen) &&
	fb_height_mm == DisplayHeightMM (dpy, screen))
    {
	return;
    }
    if (verbose)
	printf ("screen %d: %dx%d %dx%d mm %6.2fdpi\n", screen,
		fb_width, fb_height, fb_width_mm, fb_height_mm, dpi);
    if (dryrun)
	return;
    XRRSetScreenSize (dpy, root, fb_width, fb_height,
		      fb_width_mm, fb_height_mm);
}

static void
revert (void)
{
    int	c;

    /* first disable all crtcs */
    for (c = 0; c < res->ncrtc; c++)
	crtc_disable (&crtcs[c]);
    /* next reset screen size */
    screen_revert ();
    /* now restore all crtcs */
    for (c = 0; c < res->ncrtc; c++)
	crtc_revert (&crtcs[c]);
}

/*
 * uh-oh, something bad happened in the middle of changing
 * the configuration. Revert to the previous configuration
 * and bail
 */
static void
panic (Status s, crtc_t *crtc)
{
    int	    c = crtc->crtc.index;
    char    *message;
    
    switch (s) {
    case RRSetConfigSuccess:		message = "succeeded";		    break;
    case BadAlloc:			message = "out of memory";	    break;
    case RRSetConfigFailed:		message = "failed";		    break;
    case RRSetConfigInvalidConfigTime:	message = "invalid config time";    break;
    case RRSetConfigInvalidTime:	message = "invalid time";	    break;
    default:				message = "unknown failure";	    break;
    }
    
    fprintf (stderr, "%s: Configure crtc %d %s\n", program_name, c, message);
    revert ();
    exit (1);
}

void
apply (void)
{
    Status  s;
    int	    c;
    
    /*
     * Turn off any crtcs which are to be disabled or which are
     * larger than the target size
     */
    for (c = 0; c < res->ncrtc; c++)
    {
	crtc_t	    *crtc = &crtcs[c];
	XRRCrtcInfo *crtc_info = crtc->crtc_info;

	/* if this crtc is already disabled, skip it */
	if (crtc_info->mode == None) 
	    continue;
	
	/* 
	 * If this crtc is to be left enabled, make
	 * sure the old size fits then new screen
	 */
	if (crtc->mode_info) 
	{
	    XRRModeInfo	*old_mode = find_mode_by_xid (crtc_info->mode);
	    int x, y, w, h;
	    box_t bounds;

	    if (!old_mode) 
		panic (RRSetConfigFailed, crtc);
	    
	    /* old position and size information */
	    mode_geometry (old_mode, crtc_info->rotation,
			   &crtc->current_transform.transform,
			   &bounds);

	    x = crtc_info->x + bounds.x1;
	    y = crtc_info->y + bounds.y1;
	    w = bounds.x2 - bounds.x1;
	    h = bounds.y2 - bounds.y1;

	    /* if it fits, skip it */
	    if (x + w <= fb_width && y + h <= fb_height) 
		continue;
	    crtc->changing = True;
	}
	s = crtc_disable (crtc);
	if (s != RRSetConfigSuccess)
	    panic (s, crtc);
    }

    /*
     * Hold the server grabbed while messing with
     * the screen so that apps which notice the resize
     * event and ask for xinerama information from the server
     * receive up-to-date information
     */
    XGrabServer (dpy);
    
    /*
     * Set the screen size
     */
    screen_apply ();
    
    /*
     * Set crtcs
     */

    for (c = 0; c < res->ncrtc; c++)
    {
	crtc_t	*crtc = &crtcs[c];
	
	s = crtc_apply (crtc);
	if (s != RRSetConfigSuccess)
	    panic (s, crtc);
    }
    /*
     * Release the server grab and let all clients
     * respond to the updated state
     */
    XUngrabServer (dpy);
}

/*
 * Use current output state to complete the output list
 */
void
get_outputs (void)
{
    int		o;
    
    for (o = 0; o < res->noutput; o++)
    {
	XRROutputInfo	*output_info = XRRGetOutputInfo (dpy, res, res->outputs[o]);
	output_t	*output;
	name_t		output_name;
	if (!output_info) fatal ("could not get output 0x%x information", res->outputs[o]);
	set_name_xid (&output_name, res->outputs[o]);
	set_name_index (&output_name, o);
	set_name_string (&output_name, output_info->name);
	output = find_output (&output_name);
	if (!output)
	{
	    output = add_output ();
	    set_name_all (&output->output, &output_name);
	    /*
	     * When global --automatic mode is set, turn on connected but off
	     * outputs, turn off disconnected but on outputs
	     */
	    if (automatic)
	    {
		switch (output_info->connection) {
		case RR_Connected:
		    if (!output_info->crtc) {
			output->changes |= changes_automatic;
			output->automatic = True;
		    }
		    break;
		case RR_Disconnected:
		    if (output_info->crtc)
		    {
			output->changes |= changes_automatic;
			output->automatic = True;
		    }
		    break;
		}
	    }
	}

	/*
	 * Automatic mode -- track connection state and enable/disable outputs
	 * as necessary
	 */
	if (output->automatic)
	{
	    switch (output_info->connection) {
	    case RR_Connected:
	    case RR_UnknownConnection:
		if ((!(output->changes & changes_mode)))
		{
		    set_name_preferred (&output->mode);
		    output->changes |= changes_mode;
		}
		break;
	    case RR_Disconnected:
		if ((!(output->changes & changes_mode)))
		{
		    set_name_xid (&output->mode, None);
		    set_name_xid (&output->crtc, None);
		    output->changes |= changes_mode;
		    output->changes |= changes_crtc;
		}
		break;
	    }
	}

	set_output_info (output, res->outputs[o], output_info);
    }
}

void
mark_changing_crtcs (void)
{
    int	c;

    for (c = 0; c < num_crtcs; c++)
    {
	crtc_t	    *crtc = &crtcs[c];
	int	    o;
	output_t    *output;

	/* walk old output list (to catch disables) */
	for (o = 0; o < crtc->crtc_info->noutput; o++)
	{
	    output = find_output_by_xid (crtc->crtc_info->outputs[o]);
	    if (!output) fatal ("cannot find output 0x%x\n",
				crtc->crtc_info->outputs[o]);
	    if (output->changes)
		crtc->changing = True;
	}
	/* walk new output list */
	for (o = 0; o < crtc->noutput; o++)
	{
	    output = crtc->outputs[o];
	    if (output->changes)
		crtc->changing = True;
	}
    }
}

/*
 * Test whether 'crtc' can be used for 'output'
 */
Bool
check_crtc_for_output (crtc_t *crtc, output_t *output, Bool ignore_state)
{
    int		c;
    int		l;
    output_t    *other;
    
    for (c = 0; c < output->output_info->ncrtc; c++)
	if (output->output_info->crtcs[c] == crtc->crtc.xid)
	    break;
    if (c == output->output_info->ncrtc)
	return False;
    for (other = outputs; other; other = other->next)
    {
	if (other == output)
	    continue;

	if (other->mode_info == NULL)
	    continue;

	if (other->crtc_info != crtc)
	    continue;

	/* see if the output connected to the crtc can clone to this output */
	for (l = 0; l < output->output_info->nclone; l++)
	    if (output->output_info->clones[l] == other->output.xid)
		break;
	/* not on the list, can't clone */
	if (l == output->output_info->nclone) 
	    return False;
    }

    if (ignore_state)
	return True;

    if (crtc->noutput)
    {
	/* make sure the state matches */
	if (crtc->mode_info != output->mode_info)
	    return False;
	if (crtc->x != output->x)
	    return False;
	if (crtc->y != output->y)
	    return False;
	if (crtc->rotation != output->rotation)
	    return False;
	if (!equal_transform (&crtc->current_transform, &output->transform))
	    return False;
    }
    else if (crtc->crtc_info->noutput)
    {
	/* make sure the state matches the already used state */
	XRRModeInfo *mode = find_mode_by_xid (crtc->crtc_info->mode);

	if (mode != output->mode_info)
	    return False;
	if (crtc->crtc_info->x != output->x)
	    return False;
	if (crtc->crtc_info->y != output->y)
	    return False;
	if (crtc->crtc_info->rotation != output->rotation)
	    return False;
    }
    return True;
}

crtc_t *
find_crtc_for_output (output_t *output)
{
    int	    c;

    for (c = 0; c < output->output_info->ncrtc; c++)
    {
	crtc_t	    *crtc;

	crtc = find_crtc_by_xid (output->output_info->crtcs[c]);
	if (!crtc) fatal ("cannot find crtc 0x%x\n", output->output_info->crtcs[c]);

	if (check_crtc_for_output (crtc, output, False))
	    return crtc;
    }
    return NULL;
}

static void
set_positions (void)
{
    output_t	*output;
    Bool	keep_going;
    Bool	any_set;
    int		min_x, min_y;

    for (;;)
    {
	any_set = False;
	keep_going = False;
	for (output = outputs; output; output = output->next)
	{
	    output_t    *relation;
	    name_t	relation_name;

	    if (!(output->changes & changes_relation)) continue;
	    
	    if (output->mode_info == NULL) continue;

	    init_name (&relation_name);
	    set_name_string (&relation_name, output->relative_to);
	    relation = find_output (&relation_name);
	    if (!relation) fatal ("cannot find output \"%s\"\n", output->relative_to);
	    
	    if (relation->mode_info == NULL) 
	    {
		output->x = 0;
		output->y = 0;
		output->changes |= changes_position;
		any_set = True;
		continue;
	    }
	    /*
	     * Make sure the dependent object has been set in place
	     */
	    if ((relation->changes & changes_relation) && 
		!(relation->changes & changes_position))
	    {
		keep_going = True;
		continue;
	    }
	    
	    switch (output->relation) {
	    case left_of:
		output->y = relation->y;
		output->x = relation->x - mode_width (output->mode_info, output->rotation);
		break;
	    case right_of:
		output->y = relation->y;
		output->x = relation->x + mode_width (relation->mode_info, relation->rotation);
		break;
	    case above:
		output->x = relation->x;
		output->y = relation->y - mode_height (output->mode_info, output->rotation);
		break;
	    case below:
		output->x = relation->x;
		output->y = relation->y + mode_height (relation->mode_info, relation->rotation);
		break;
	    case same_as:
		output->x = relation->x;
		output->y = relation->y;
	    }
	    output->changes |= changes_position;
	    any_set = True;
	}
	if (!keep_going)
	    break;
	if (!any_set)
	    fatal ("loop in relative position specifications\n");
    }

    /*
     * Now normalize positions so the upper left corner of all outputs is at 0,0
     */
    min_x = 32768;
    min_y = 32768;
    for (output = outputs; output; output = output->next)
    {
	if (output->mode_info == NULL) continue;
	
	if (output->x < min_x) min_x = output->x;
	if (output->y < min_y) min_y = output->y;
    }
    if (min_x || min_y)
    {
	/* move all outputs */
	for (output = outputs; output; output = output->next)
	{
	    if (output->mode_info == NULL) continue;

	    output->x -= min_x;
	    output->y -= min_y;
	    output->changes |= changes_position;
	}
    }
}

static void
set_screen_size (void)
{
    output_t	*output;
    Bool	fb_specified = fb_width != 0 && fb_height != 0;
    
    for (output = outputs; output; output = output->next)
    {
	XRRModeInfo *mode_info = output->mode_info;
	int	    x, y, w, h;
	box_t	    bounds;
	
	if (!mode_info) continue;
	
	mode_geometry (mode_info, output->rotation,
		       &output->transform.transform,
		       &bounds);
	x = output->x + bounds.x1;
	y = output->y + bounds.y1;
	w = bounds.x2 - bounds.x1;
	h = bounds.y2 - bounds.y1;
	/* make sure output fits in specified size */
	if (fb_specified)
	{
	    if (x + w > fb_width || y + h > fb_height)
		fatal ("specified screen %dx%d not large enough for output %s (%dx%d+%d+%d)\n",
		       fb_width, fb_height, output->output.string, w, h, x, y);
	}
	/* fit fb to output */
	else
	{
	    if (x + w > fb_width) fb_width = x + w;
	    if (y + h > fb_height) fb_height = y + h;
	}
    }	

    if (fb_width > maxWidth || fb_height > maxHeight)
        fatal ("screen cannot be larger than %dx%d (desired size %dx%d)\n",
	       maxWidth, maxHeight, fb_width, fb_height);
    if (fb_specified)
    {
	if (fb_width < minWidth || fb_height < minHeight)
	    fatal ("screen must be at least %dx%d\n", minWidth, minHeight);
    }
    else
    {
	if (fb_width < minWidth) fb_width = minWidth;
	if (fb_height < minHeight) fb_height = minHeight;
    }
}
    
#endif
    
void
disable_outputs (output_t *outputs)
{
    while (outputs)
    {
	outputs->crtc_info = NULL;
	outputs = outputs->next;
    }
}

/*
 * find the best mapping from output to crtc available
 */
int
pick_crtcs_score (output_t *outputs)
{
    output_t	*output;
    int		best_score;
    int		my_score;
    int		score;
    crtc_t	*best_crtc;
    int		c;
    
    if (!outputs)
	return 0;
    
    output = outputs;
    outputs = outputs->next;
    /*
     * Score with this output disabled
     */
    output->crtc_info = NULL;
    best_score = pick_crtcs_score (outputs);
    if (output->mode_info == NULL)
	return best_score;

    best_crtc = NULL;
    /* 
     * Now score with this output any valid crtc
     */
    for (c = 0; c < output->output_info->ncrtc; c++)
    {
	crtc_t	    *crtc;

	crtc = find_crtc_by_xid (output->output_info->crtcs[c]);
	if (!crtc)
	    fatal ("cannot find crtc 0x%x\n", output->output_info->crtcs[c]);
	
	/* reset crtc allocation for following outputs */
	disable_outputs (outputs);
	if (!check_crtc_for_output (crtc, output, True))
	    continue;
	
	my_score = 1000;
	/* slight preference for existing connections */
	if (crtc == output->current_crtc_info)
	    my_score++;

	output->crtc_info = crtc;
	score = my_score + pick_crtcs_score (outputs);
	if (score > best_score)
	{
	    best_crtc = crtc;
	    best_score = score;
	}
    }
    if (output->crtc_info != best_crtc)
	output->crtc_info = best_crtc;
    /*
     * Reset other outputs based on this one using the best crtc
     */
    (void) pick_crtcs_score (outputs);

    return best_score;
}

/*
 * Pick crtcs for any changing outputs that don't have one
 */
void
pick_crtcs (void)
{
    output_t	*output;

    /*
     * First try to match up newly enabled outputs with spare crtcs
     */
    for (output = outputs; output; output = output->next)
    {
	if (output->changes && output->mode_info)
	{
	    if (output->crtc_info) {
		if (output->crtc_info->crtc_info->noutput > 0 &&
		    (output->crtc_info->crtc_info->noutput > 1 ||
		     output != find_output_by_xid (output->crtc_info->crtc_info->outputs[0])))
		    break;
	    } else {
		output->crtc_info = find_crtc_for_output (output);
		if (!output->crtc_info)
		    break;
	    }
	}
    }
    /*
     * Everyone is happy
     */
    if (!output)
	return;
    /*
     * When the simple way fails, see if there is a way
     * to swap crtcs around and make things work
     */
    for (output = outputs; output; output = output->next)
	output->current_crtc_info = output->crtc_info;
    pick_crtcs_score (outputs);
    for (output = outputs; output; output = output->next)
    {
	if (output->mode_info && !output->crtc_info)
	    fatal ("cannot find crtc for output %s\n", output->output.string);
	if (!output->changes && output->crtc_info != output->current_crtc_info)
	    output->changes |= changes_crtc;
    }
}


int init_xrandr(void){
    int		major, minor;
    char          *display_name = NULL;

    fb_width = 0; fb_height = 0;
    fb_width_mm = 0; fb_height_mm = 0;
    dpi = 0;
//outputs = NULL;
//*outputs_tail = &outputs;
    crtcs = NULL;
    umodes = NULL;

    program_name = "xrandr";
    dpy = XOpenDisplay (display_name);

    if (dpy == NULL) {
	fprintf (stderr, "Can't open display %s\n", XDisplayName(display_name));
	return 1;
    }

    screen = DefaultScreen (dpy);
    
    if (screen >= ScreenCount (dpy)) {
	fprintf (stderr, "Invalid screen number %d (display has %d)\n",
		 screen, ScreenCount (dpy));
	return 1;
    }

    root = RootWindow (dpy, screen);
    
    if (!XRRQueryVersion (dpy, &major, &minor))
    {
	fprintf (stderr, "RandR extension missing\n");
	return 1;
    }
    
    if (major > 1 || (major == 1 && minor >= 2))
    {
        has_1_2 = True;
    }
    return 0;
}

int
add_custom_mode ( vga_t *vga_ptr, int pixelclk, int height, int hsync, int vsync )
{
	init_xrandr();
	
    if (has_1_2)
    {
    	XRRModeInfo *e;
	output_t	*o;

	umode_t  *m = malloc (sizeof (umode_t));
    
	m->mode.name = vga_ptr->activemode;
	m->mode.nameLength = strlen (vga_ptr->activemode);
	m->mode.dotClock = (float)pixelclk * 1e6;
	m->mode.width = 1400 - 7;
	m->mode.hSyncStart = m->mode.width+1; /* for fglrx */
	m->mode.hSyncEnd = m->mode.hSyncStart + hsync;
	m->mode.hTotal = m->mode.hSyncEnd + 1; /* for mesa */
	m->mode.height = height - 3;
	m->mode.vSyncStart = m->mode.height+0;
	m->mode.vSyncEnd = m->mode.vSyncStart + vsync;
	m->mode.vTotal = m->mode.vSyncEnd + 1;
	m->mode.modeFlags = RR_HSyncPositive|RR_VSyncPositive;

	get_screen ();
	get_crtcs ();
	get_outputs ();
	o = find_output_by_name ((char *)&vga_ptr->outputname);
	if (!o) {
	o = add_output ();
	set_name (&o->output, (char *)&vga_ptr->outputname, name_string|name_xid);
	}
	if (!o)
	fatal ("cannot find output \"%s\"\n", vga_ptr->outputname);
	e = find_mode_by_name(vga_ptr->activemode);
	if (e){
		vga_t new_config;
		memcpy( &new_config, vga_ptr, sizeof(vga_t) );
		memcpy( new_config.activemode,"1024x768", sizeof("1024x768") );
		enable_output( &new_config );
		disable_output( &new_config );
		memcpy( new_config.activemode,"newmode", sizeof("newmode") );
		rm_mode( &new_config );
		init_xrandr();
		get_screen ();
		get_crtcs ();
		get_outputs ();
		o = find_output_by_name ((char *)&vga_ptr->outputname);
		if (!o) {
		o = add_output ();
		set_name (&o->output, (char *)&vga_ptr->outputname, name_string|name_xid);
		}
		if (!o)
		fatal ("cannot find output \"%s\"\n", vga_ptr->outputname);
	}
	   
    	XRRCreateMode (dpy, root, &m->mode);
    	XSync (dpy, False);
    	get_screen ();
	get_crtcs ();
	get_outputs ();
	set_name (&m->output, (char *)&vga_ptr->outputname, name_string|name_xid);
	set_name (&m->name, m->mode.name, name_string|name_xid);
	e = find_mode_by_name(vga_ptr->activemode);
	if (!e){
		printf("failed to create mode\n");
		exit(0);
	}	
	XRRAddOutputMode (dpy, o->output.xid, e->id);
	XSync (dpy, False);
	return 0;

    }
    return 1;
}

int
enable_output ( vga_t * vga_ptr )

{
	output_t	*output = NULL;
	
	init_xrandr();
	
	output = find_output_by_name ((char *)&vga_ptr->outputname);
	if (!output) {
		output = add_output ();
		set_name (&output->output, (char *)&vga_ptr->outputname, name_string|name_xid);
	}

	set_name_xid (&output->mode, None);
	set_name_xid (&output->crtc, None);
	
	if(strstr(&vga_ptr->activemode[0], "auto")){
		output->automatic = True;
		output->changes |= changes_automatic;	
	}
	else
	{
		set_name (&output->mode, (char *)&vga_ptr->activemode, name_string|name_xid);
		output->changes |= changes_mode;
	}
  	output->x = vga_ptr->pos_x;
  	output->y = vga_ptr->pos_y;

	output->changes |= changes_position;
	
	get_screen ();
	get_crtcs ();
	get_outputs ();
	set_positions ();
	set_screen_size ();
	
	pick_crtcs ();
	
	/*
	 * Assign outputs to crtcs
	 */
	set_crtcs ();
	
	/*
	 * Mark changing crtcs
	 */
	mark_changing_crtcs ();

	/*
	 * Compute physical screen size
	 */
	if (fb_width_mm == 0 || fb_height_mm == 0)
	{
	    if (fb_width != DisplayWidth (dpy, screen) ||
		fb_height != DisplayHeight (dpy, screen) || dpi != 0.0)
	    {
		if (dpi <= 0)
		    dpi = (25.4 * DisplayHeight (dpy, screen)) / DisplayHeightMM(dpy, screen);
	
		fb_width_mm = (25.4 * fb_width) / dpi;
		fb_height_mm = (25.4 * fb_height) / dpi;
	    }
	    else
	    {
		fb_width_mm = DisplayWidthMM (dpy, screen);
		fb_height_mm = DisplayHeightMM (dpy, screen);
	    }
	}
	
	
	/*
	 * Now apply all of the changes
	 */
	apply ();
	
	XSync (dpy, False);
	return 0;

}

int find_VGA_output( vga_t * vga_ptr )
{
	output_t	*output = NULL;

	init_xrandr();
	
	get_screen ();
	get_crtcs ();
	get_outputs ();
	
	for (output = outputs; output; output = output->next)
	{
	    XRROutputInfo   *output_info = output->output_info;
	    crtc_t	    *crtc = output->crtc_info;
	    XRRCrtcInfo	    *crtc_info = crtc ? crtc->crtc_info : NULL;
	    XRRModeInfo	    *mode = output->mode_info;
	
	    /* look for VGA or CRT */
	    if( strstr(output_info->name, "VGA") || strstr(output_info->name, "CRT")){
	    	    memcpy(&vga_ptr->outputname,output_info->name, strlen(output_info->name));
	    	    
		    if (mode)
		    {
			if (crtc_info) {
			    vga_ptr->pos_x = crtc_info->x;
			    vga_ptr->pos_y = crtc_info->y;
			} else {
			    vga_ptr->pos_x = output->x;
			    vga_ptr->pos_y = output->y;
			}
			
			memcpy(&vga_ptr->activemode,mode->name,strlen(mode->name));
		    }

		    return 0;
	    }
	    
	  
	   
	}
	return 1;
}


void disable_output( vga_t *vga_ptr )
{
	output_t	*output = NULL;

	init_xrandr();
	
	output = find_output_by_name ((char *)&vga_ptr->outputname);
	if (!output) {
	output = add_output ();
	set_name (&output->output, (char *)&vga_ptr->outputname, name_string|name_xid);
	}

	set_name_xid (&output->mode, None);
	set_name_xid (&output->crtc, None);
	output->changes |= changes_mode;
	
	get_screen ();
	get_crtcs ();
	get_outputs ();
	set_positions ();
	set_screen_size ();
	
	pick_crtcs ();
	
	/*
	 * Assign outputs to crtcs
	 */
	set_crtcs ();
	
	/*
	 * Mark changing crtcs
	 */
	mark_changing_crtcs ();

	/*
	 * Compute physical screen size
	 */
	if (fb_width_mm == 0 || fb_height_mm == 0)
	{
	    if (fb_width != DisplayWidth (dpy, screen) ||
		fb_height != DisplayHeight (dpy, screen) || dpi != 0.0)
	    {
		if (dpi <= 0)
		    dpi = (25.4 * DisplayHeight (dpy, screen)) / DisplayHeightMM(dpy, screen);
	
		fb_width_mm = (25.4 * fb_width) / dpi;
		fb_height_mm = (25.4 * fb_height) / dpi;
	    }
	    else
	    {
		fb_width_mm = DisplayWidthMM (dpy, screen);
		fb_height_mm = DisplayHeightMM (dpy, screen);
	    }
	}
	
	/*
	 * Now apply all of the changes
	 */
	apply ();
	XSync (dpy, False);	
}

int rm_mode ( vga_t * vga_ptr )
{
	output_t	*output = NULL;
	disable_output( vga_ptr );
	
	get_screen ();
	get_crtcs ();
	get_outputs ();
	
	XRRModeInfo *e;
	e = find_mode_by_name((char *)&vga_ptr->activemode);
	output = find_output_by_name ((char *)&vga_ptr->outputname);
	XRRDeleteOutputMode (dpy, output->output.xid, e->id);
	XSync (dpy, False);
	
	get_screen ();
	get_crtcs ();
	get_outputs ();
	XRRDestroyMode (dpy, e->id);
	XSync (dpy, False);
	return 0;
}
