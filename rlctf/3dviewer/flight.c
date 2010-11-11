#include "flight.h"

void flightroute_iterate_percent (froute_t *io_flight_p, float in_percent)
{
	uint_fast8_t i;
	double p_inv, p_pos;
	static fpoint_t tmp;

	if (io_flight_p->next == NULL)
	{
		printf("eof(1)\n");
		return; /* flight ends here */
	}

	p_pos = io_flight_p->progress + in_percent;
	if (p_pos > 1.0f)
	{
		p_pos = io_flight_p->progress + in_percent - 1.0f;
		io_flight_p->progress = 0.0f; /* reset prog. count */
		memcpy (&tmp, io_flight_p->next->p, sizeof(fpoint_t));
		memcpy (io_flight_p, io_flight_p->next, sizeof(froute_t));
		io_flight_p->p = &tmp;
		
		if (io_flight_p->next == NULL)
		{
			printf("eof(2)\n");
			return; /* flight ends here */
		}
	}
	io_flight_p->progress = p_pos;
	//p_pos = sin(p_pos*M_PI/2);
	p_inv = 1.0f - p_pos;
	
	for (i=0;i<3;i++)
	{
		io_flight_p->p->target[i] = io_flight_p->next->p->target[i] - io_flight_p->transv.target[i] * p_inv;
		io_flight_p->p->eye[i] = io_flight_p->next->p->eye[i] - io_flight_p->transv.eye[i] * p_inv;
		io_flight_p->p->up[i] = io_flight_p->next->p->up[i] - io_flight_p->transv.up[i] * p_inv;
	}
}

froute_t* froute_around_point (vec3_t in_p, int num_points)
{
	froute_t *new_r = malloc (sizeof(froute_t));
	froute_t *tmp = new_r;
	int i;
	
	for (i=0;i<num_points;i++)
	{
		tmp->p = malloc(sizeof(fpoint_t));
		memcpy (&tmp->p->target, in_p, sizeof(vec3_t));
		
		tmp->progress = 0.0f;

		tmp->p->eye[0] = in_p[0] + sin(((float) i/ (float) num_points) * 2 * M_PI) * 8.0f;
		tmp->p->eye[1] = in_p[1] + cos(((float) i/ (float) num_points) * 2 * M_PI) * 8.0f;
		tmp->p->eye[2] = 4.0f;

		tmp->p->up[0] = 0.0f;
		tmp->p->up[1] = 0.0f;
		tmp->p->up[2] = 1.0f;

		if (i != num_points-1)
		{
			tmp->next = malloc(sizeof(froute_t));
		} else
		{
			tmp->next = new_r; /* link to first node */
		}
		tmp = tmp->next;
	}
	
	/* generate transition vectors */
	tmp = new_r;
	for (i=0;i<num_points;i++)
	{
		flight_gen_transv (tmp);
		printf("\t\t %p %p\n", tmp->next, tmp);
		tmp = tmp->next;
	}
	return new_r;
}

void flightroute_set_view (froute_t *in_p)
{
	/*
	printf ("sview: [ %f %f %f ] [ %f %f %f ] [ %f %f %f ], %f\n",
		in_p->p->eye[0], in_p->p->eye[1], in_p->p->eye[2],
		in_p->p->target[0], in_p->p->target[1], in_p->p->target[2],
		in_p->p->up[0], in_p->p->up[1], in_p->p->up[2], in_p->progress
	);
	*/
	glColor3f (1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f (in_p->p->target[0], in_p->p->target[1], 10.0f);
		glVertex3f (in_p->p->target[0], in_p->p->target[1], in_p->p->target[2]);
	glEnd();

	gluLookAt (
		in_p->p->eye[0], in_p->p->eye[1], in_p->p->eye[2],
		in_p->p->target[0], in_p->p->target[1], in_p->p->target[2],
		in_p->p->up[0], in_p->p->up[1], in_p->p->up[2]
	);
		
}

/* rotate the plane coords around given axis */
void rotate_plane_axis (flight_t *fp, vec3_t in_axis, float in_angle)
{
	
}

void flight_set_roll (flight_t *fp, int in_roll)
{
	fp->roll += ((float) in_roll / 256.0f);
	if (fp->roll > 1.0f)
		fp->roll = 1.0f;
	if (fp->roll < -1.0f)
		fp->roll = -1.0f;
}

void flight_set_pitch (flight_t *fp, int in_pitch)
{
	fp->pitch += ((float) in_pitch / 256.0f);
	if (fp->pitch > 1.0f)
		fp->pitch = 1.0f;
	if (fp->pitch < -1.0f)
		fp->pitch = -1.0f;
}

void flight_set_xydelta (flight_t *in_f, int in_x, int in_y)
{
	return;
}
void flight_gen_transv (froute_t *in_r)
{
	if (in_r->next == NULL)
	{
		return;
	}
	vec3sub (in_r->transv.eye, in_r->next->p->eye, in_r->p->eye);
	vec3sub (in_r->transv.target, in_r->next->p->target, in_r->p->target);
	vec3sub (in_r->transv.up, in_r->next->p->up, in_r->p->up);
}

void flight_iterate_percent (flight_t *in_f, float in_percent)
{
	vec3_t dirv, tmp;
	vec3sub (dirv, in_f->dir.target, in_f->dir.eye);
	vec3normalize (dirv);

	vec3rotateaxis (in_f->dir.up, dirv, in_f->dir.up, in_f->roll);
	vec3rotateaxis (in_f->left, dirv, in_f->left, in_f->roll);
	vec3normalize (in_f->left);
	vec3normalize (in_f->dir.up);
//	vec3normalize (dirv);
	vec3rotateaxis (in_f->dir.target, in_f->left, in_f->dir.target, in_f->pitch);
	vec3rotateaxis (in_f->dir.up, in_f->left, in_f->dir.up, in_f->pitch);
	printf("pitch: %010f , roll: %010f, speed %010f\n", in_f->pitch, in_f->roll, in_f->speed);
	vec3ortho (dirv, in_f->left, in_f->dir.up);
	if (in_f->speed != 0.0f)
	{
		vec3normalize(dirv);
		vec3scale (dirv, (double) in_percent * - in_f->speed);
		vec3add (in_f->dir.eye, in_f->dir.eye, dirv);
		vec3add (in_f->dir.target, in_f->dir.eye, dirv);
		printf("dv: %f %f %f", dirv[0], dirv[1], dirv[2]);
	}
	in_f->roll *= 0.8f;
	in_f->pitch *= 0.8f;
	//vec3normalize(dirv);
	//vec3add (in_f->dir.target, in_f->dir.eye, dirv);
	return;
/*
	vec3scale (dirv, (double) in_percent);
	vec3add (in_f->dir.eye, in_f->dir.eye, dirv);
	vec3add (in_f->dir.target, in_f->dir.target, dirv);
*/
}

void flight_set_view (flight_t *in_f)
{
	gluLookAt (
		in_f->dir.eye[0], in_f->dir.eye[1], in_f->dir.eye[2],
		in_f->dir.target[0], in_f->dir.target[1], in_f->dir.target[2],
		in_f->dir.up[0], in_f->dir.up[1], in_f->dir.up[2]
	);
}

void flight_init (flight_t *in_f)
{
	in_f->dir.eye[0] = 0.0f;
	in_f->dir.eye[1] = 0.0f;
	in_f->dir.eye[2] = 1.0f;
	in_f->dir.target[0] = 1.0f;
	in_f->dir.target[1] = 0.0f;
	in_f->dir.target[2] = 1.0f;
	in_f->dir.up[0] = 0.0f;
	in_f->dir.up[1] = 0.0f;
	in_f->dir.up[2] = 1.0f;

	in_f->pitch = 0.0000f;
	in_f->roll = 0.0f;
	in_f->yaw = 0.0f;
	in_f->speed = 0.00000f;

	in_f->left[0] = 0.0f;
	in_f->left[1] = -1.0f;
	in_f->left[2] = 0.0f;
}


