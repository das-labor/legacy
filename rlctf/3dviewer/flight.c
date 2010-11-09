#include "flight.h"

void flight_iterate_percent (froute_t *io_flight_p, float in_percent)
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

void flight_set_view (froute_t *in_p)
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
