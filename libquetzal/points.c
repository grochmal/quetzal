/*
 * COPYRIGHT (C) 2015 Michal Grochmal
 *
 * This file is distributed under the terms of GNU GPL v3
 */

#include <quetzal.h>

#include <conf.h>

#include <stdlib.h>  /* NULL */
#include <math.h>    /* sqrt */

struct quetzal_class_str {
	unsigned char            * name;
	struct quetzal_point_str * inst;
	struct quetzal_class_str * next;
};
struct quetzal_point_str {
	unsigned int               max;
	struct quetzal_pdims_str * points[POINT_CACHE];
	struct quetzal_point_str * next;
};
struct quetzal_ptdim_str {
	unsigned int               max;
	dim_prec_t                 position[DIM_CACHE];
	struct quetzal_ptdim_str * next;
}
/* typedef struct quetzal_class_str quetzal_class; */
/* typedef struct quetzal_point_str quetzal_point; */
/* typedef struct quetzal_ptdim_str quetzal_ptdim; */

/**
 * quetzal_num_ptdim - returns the number of dimensions in a quetzal_ptdim
 *
 * output parameters (value undefined if execution failed):
 *   offset - will be set to the number of dimensions
 *
 * input parameters:
 *   ptdim - the quetzal_ptdim for which the number
 *           of dimensions is required
 *
 * return: 0 (success) / -1 (failure)
 */
int
quetzal_num_ptdim (p_offset_t * offset, quetzal_ptdim * ptdim) {
	quetzal_ptdim p = ptdim;
	p_offset_t    i = 0;
	unsigned int  j = 0;

	if (NULL == offset || NULL == ptdim)
		return -1;
	while (NULL != p->next) {
		i += DIM_CACHE;
		p = p->next;
	}
	for (j = 0; j < p->max; j++)
		i++;  /* lint does not complain this way */
	*offset = i;
	return 0;
}

/**
 * quetzal_next_ptdim - moves to the next dimension value
 *
 * output parameters (value undefined if execution failed):
 *   value - if not NULL will be set to the value
 *           in the next dimension
 *   ptdim - will be set to the structure where the next
 *           dimension resides
 *   offset - will be set to the offset in the structure
 *
 * Either `ptdim' or `offset' will change every time this
 * function is called.  It shall be called with the same
 * parameters to iterate through all records.  When called
 * after the last record the function will return
 * QUETZAL_STREND.
 *
 * return:  0              (success)
 *         -1              (failure)
 *         QUETZAL_STREND  (no more dimensions)
 */
int
quetzal_next_ptdim ( dim_prec_t    * value  /* possibly NULL */
		   , quetzal_ptdim * ptdim
		   , p_offset_t    * offset) {
	if (NULL == ptdim || NULL == offset)
		return -1;
	if (NULL == ptdim->next && ptdim->max <= *offset)
		return QUETZAL_STREND;
	if (DIM_CACHE <= *offset + 1) {
		ptdim  = ptdim->next;
		*offset = 0;
	}
	else {
		*offset += 1;
	}
	if (NULL != value)
		*value = ptdim->position[offset];
	return 0;
}

/* TODO finished here */
/* TODO, need to work on allocating and adding new dims from the public API */

dim_prec_t
quetzal_ptdim_at (quetzal_ptdim * ptdim, p_offset_t offset) {
	quetzal_ptdim * p = ptdim;

	while (offset > DIM_CACHE) {
		p = p->next;
		offset %= DIM_CACHE;
	}
	if (NULL == p)
		return NULL;  /* something went very wrong */
	return p->position[offset];
}

static dim_prec_t
distance (quetzal_ptdim * left, quetzal_ptdim * right) {
	quetzal_ptdim * diml = left;
	quetzal_ptdim * dimr = right;
	p_offset_t      offl = 0;
	p_offset_t      offr = 0;
	dim_prec_t      rawl = 0.0;
	dim_prec_t      rawr = 0.0;
	dim_prec_t       sum = 0.0;

	if (NULL == diml || NULL == dimr)
		return 0.0;
	for ( rawl = diml->position[offset], rawl = dimr->position[offset]
	    ; 
	quetzal_next_ptdim (diml, &offl);
	quetzal_next_ptdim (dimr, &offr);
	/*for ( diml = left, dimr = right
	    ; NULL != diml && NULL != dimr
	    ; diml = diml->next, dimr = dimr->next
	    ) {*/
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

