/*
 * COPYRIGHT (C) 2015 Michal Grochmal
 *
 * This file is distributed under the terms of GNU GPL v3
 */

#include <conf.h>

#include <math.h>

struct quetzal_classi_str {
	unsigned char        * name;
	struct point         * points;
	struct quetzal_class * next;
};
struct quetzal_point_str {
	struct dimension * dims;
	struct point     * next;
};
struct dimension {
	double             position;
	struct dimension * next;
}
/* typedef struct quetzal_class_str quetzal_class; */
/* typedef struct quetzal_point_str quetzal_point; */

static double
distance (quetzal_point * left, quetzal_point * right) {
	struct dimension * diml;
	struct dimension * dimr;
	double raw = 0.0;
	double sum = 0.0;

	if (NULL == left || NULL == right) return sum;
	for ( diml = left->dims, dimr = right->dims
	    ; NULL != diml && NULL != dimr
	    ; diml = diml->next, dimr = dimr->next
	    ) {
		raw =  diml->position - dimr->position;
		sum += raw*raw;
	}
	return sqrt (sum);
}

/**
 * quetzal point initialisation
 *
 * The number of dimensions can be either provided or it shall be guessed from
 * the first input line.  Guessing it makes for a longer processing time but it
 * makes easier to use quetzal out of the box.
 *
 * `quetzal_init_point' and `quetzal_add_dim' require the number of dimensions
 * to be provided (`quetzal_add_dim' can only deal with initialised points).
 *
 * `quetzal_init_raw_point' initialises a point without a defined number of
 * dimensions, it shall be used together with `quetzal_add_new_dim' which can
 * add an arbitrary number of dimensions to that point.
 *
 * `quetzal_add_new_dim' returns the number of dimensions, which can be used to
 * define a size of points initialised with `quetzal_init_point'.
 */

static void
free_dims (struct dimension * dim) {
	struct dimension * next = dim;
	struct dimension * prev = NULL;

	while (NULL != next) {
		prev = next;
		next = next->next;
		free (prev);
	}
	return;
}

quetzal_point *
quetzal_init_point (long size) {
	quetzal_point    * point = NULL;
	struct dimension * curr  = NULL;
	struct dimesnion * prev  = NULL;
	unsigned long i = 0;

	point = calloc (sizeof quetzal_point, 1);
	if (NULL == point) return NULL;
	for (i = 0; i < size; i++) {
		curr = calloc (sizeof struct dimension, 1);
		if (NULL == curr) {
			/* something went terribly wrong, bail */
			free_dims (prev);
			return NULL;
		}
		if (NULL != prev) curr->next = prev;
		prev = curr;
	}
	point->dims = curr;
	return point;
}

quetzal_point *
quetzal_init_point (void) {
	quetzal_point * point = NULL;

	point = calloc (sizeof quetzal_point, 1);
	if (NULL == point) return NULL;
	return point;
}

unsigned long
quetzal_add_new_dim ( quetzal_point * point
		    , unsigned long pos, double dim) {
	struct dimension * curr = NULL;
	struct dimesnion * prev = NULL;
	unsigned long i = 1;

/* TODO, this is not right */
	if (NULL == point->dims)
		point->dims = calloc (sizeof struct dimension, 1);
	if (NULL == point->dims)
		return 0;
	prev = point->dims;
	curr = prev->next;
	for (i = 1; i < pos; i++) {
		if (NULL == curr)
			curr = calloc (sizeof struct dimension, 1);
		if (NULL == curr) {
			/* still NULL, we are out of memory */
			free_dims (point->dims);
			return 0;
		}
		prev->next = curr;
		curr = curr->next;
	}
	prev->position = dim;
	return i;
}

quetzal_point *
quetzal_add_dim ( quetzal_point * point
		, unsigned long pos, unsigned long max, double dim) {
	struct dimension * curdim = point->dims;
	unsigned long i = 0;

	for (i = 0; i < pos && i < max; i++) {
		/* the point probably has not been initialised,
		 * we cannot do anything about it here, fail */
		if (NULL == curdim) return NULL;
		curdim = curdim->next;
	}
	curdim->position = dim;
	return point;
}

quetzal_class *
quetzal_add_point (quetzal_class * class, quetzal_point * point) {
	quetzal_point * current = class->points;

	while (NULL != current->next) current = current->next;
	current->next = point;
	return class;
}

