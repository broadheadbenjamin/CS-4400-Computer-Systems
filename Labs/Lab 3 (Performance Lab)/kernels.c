/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Benjamin S. Broadhead",     		/* Full name */
  "broadhead.benjamin@gmail.com", 	/* Email address */

};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_pinwheel - The naive baseline version of pinwheel 
 */
char naive_pinwheel_descr[] = "naive_pinwheel: Naive baseline implementation";
void naive_pinwheel(int dim, pixel *src, pixel *dest)
{
  int i, j;

  for (i = 0; i < dim/2; i++)
    for (j = 0; j < dim/2; j++)
      dest[RIDX(dim/2 - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
    
  for (i = 0; i < dim/2; i++)
    for (j = 0; j < dim/2; j++)
      dest[RIDX(dim/2 - 1 - j, dim/2 + i, dim)] = src[RIDX(i, dim/2 + j, dim)];

  for (i = 0; i < dim/2; i++)
    for (j = 0; j < dim/2; j++)
      dest[RIDX(dim - 1 - j, i, dim)] = src[RIDX(dim/2 + i, j, dim)];

  for (i = 0; i < dim/2; i++)
    for (j = 0; j < dim/2; j++)
      dest[RIDX(dim - 1 - j, dim/2 + i, dim)] = src[RIDX(dim/2 + i, dim/2 + j, dim)];
}



/* 
 * pinwheel_1 - 1st optimization 
 */
char pinwheel_1_descr[] = "pinwheel_1: Pulled variables out of loop. Collapsed loop the four loops into one.";
void pinwheel_1(int dim, pixel *src, pixel *dest)
{
	int i, j;
	int dim_div_2 = dim/2;

  	for (i = 0; i < dim_div_2; i++)
    	for (j = 0; j < dim_div_2; j++){
			dest[RIDX(dim_div_2 - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
			dest[RIDX(dim_div_2 - 1 - j, dim_div_2 + i, dim)] = src[RIDX(i, dim_div_2 + j, dim)];
			dest[RIDX(dim - 1 - j, i, dim)] = src[RIDX(dim_div_2 + i, j, dim)];
			dest[RIDX(dim - 1 - j, dim_div_2 + i, dim)] = src[RIDX(dim_div_2 + i, dim_div_2 + j, dim)];
		}

}


/* 
 * pinwheel_2 - 2nd optimization 
 */
char pinwheel_2_descr[] = "pinwheel_2: pinwheel_1, but with pointers for array access.";
void pinwheel_2(int dim, pixel *src, pixel *dest)
{
	int i, j;
	int dim_div_2 = dim/2;

  	for (i = 0; i < dim_div_2; i++)
    	for (j = 0; j < dim_div_2; j++){
			//dest[RIDX(dim_div_2 - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
			*(dest + ((dim_div_2 - 1 - j) * dim + i)) = *(src + (i * dim + j));

			//dest[RIDX(dim_div_2 - 1 - j, dim_div_2 + i, dim)] = src[RIDX(i, dim_div_2 + j, dim)];
			*(dest + ((dim_div_2 - 1 - j) * dim + (dim_div_2 + i))) = *(src + (i * dim + (dim_div_2 + j)));

			//dest[RIDX(dim - 1 - j, i, dim)] = src[RIDX(dim_div_2 + i, j, dim)];
			*(dest + ((dim - 1 - j) * dim + i)) = *(src + ((dim_div_2 + i) * dim + j));

			//dest[RIDX(dim - 1 - j, dim_div_2 + i, dim)] = src[RIDX(dim_div_2 + i, dim_div_2 + j, dim)];
			*(dest + ((dim - 1 - j) * dim + (dim_div_2 + i))) = *(src + ((dim_div_2 + i) * dim + (dim_div_2 + j)));
		}

}


/* 
 * pinwheel_3 - 3rd optimization 
 */
char pinwheel_3_descr[] = "pinwheel_3: 2 loops instead of 1. Fast for some odd reason.";
void pinwheel_3(int dim, pixel *src, pixel *dest)
{
	int i, j;
	int dim_div_2 = dim/2;

  	for (i = 0; i < dim_div_2; i++)
    	for (j = 0; j < dim_div_2; j++){
			dest[RIDX(dim/2 - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
			dest[RIDX(dim/2 - 1 - j, dim/2 + i, dim)] = src[RIDX(i, dim/2 + j, dim)];

		}

	for (i = 0; i < dim_div_2; i++)
    	for (j = 0; j < dim_div_2; j++){
			dest[RIDX(dim - 1 - j, i, dim)] = src[RIDX(dim_div_2 + i, j, dim)];
			dest[RIDX(dim - 1 - j, dim_div_2 + i, dim)] = src[RIDX(dim_div_2 + i, dim_div_2 + j, dim)];
		}

}

/* 
 * pinwheel_4 - 4th Optimization
 */
char pinwheel_4_descr[] = "pinwheel_4: Unrolled the naive loop 16 times. Reduced memory reads.";
void pinwheel_4(int dim, pixel *src, pixel *dest)
{
  int i, j;
  int dimDiv2 = dim>>1;

  for (i = 0; i < dimDiv2; i+=16)
    for (j = 0; j < dimDiv2; j++){
      dest[RIDX(dimDiv2 - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+1, dim)] = src[RIDX(i+1, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+2, dim)] = src[RIDX(i+2, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+3, dim)] = src[RIDX(i+3, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+4, dim)] = src[RIDX(i+4, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+5, dim)] = src[RIDX(i+5, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+6, dim)] = src[RIDX(i+6, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+7, dim)] = src[RIDX(i+7, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+8, dim)] = src[RIDX(i+8, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+9, dim)] = src[RIDX(i+9, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+10, dim)] = src[RIDX(i+10, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+11, dim)] = src[RIDX(i+11, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+12, dim)] = src[RIDX(i+12, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+13, dim)] = src[RIDX(i+13, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+14, dim)] = src[RIDX(i+14, j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, i+15, dim)] = src[RIDX(i+15, j, dim)];
      }
    
  for (i = 0; i < dimDiv2; i+=16)
    for (j = 0; j < dimDiv2; j++){
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i, dim)] = src[RIDX(i, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+1, dim)] = src[RIDX(i+1, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+2, dim)] = src[RIDX(i+2, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+3, dim)] = src[RIDX(i+3, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+4, dim)] = src[RIDX(i+4, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+5, dim)] = src[RIDX(i+5, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+6, dim)] = src[RIDX(i+6, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+7, dim)] = src[RIDX(i+7, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+8, dim)] = src[RIDX(i+8, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+9, dim)] = src[RIDX(i+9, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+10, dim)] = src[RIDX(i+10, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+11, dim)] = src[RIDX(i+11, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+12, dim)] = src[RIDX(i+12, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+13, dim)] = src[RIDX(i+13, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+14, dim)] = src[RIDX(i+14, dimDiv2 + j, dim)];
      dest[RIDX(dimDiv2 - 1 - j, dimDiv2 + i+15, dim)] = src[RIDX(i+15, dimDiv2 + j, dim)];
      }


  for (i = 0; i < dimDiv2; i+=16)
    for (j = 0; j < dimDiv2; j++){
      dest[RIDX(dim - 1 - j, i, dim)] = src[RIDX(dimDiv2 + i, j, dim)];
      dest[RIDX(dim - 1 - j, i+1, dim)] = src[RIDX(dimDiv2 + i+1, j, dim)];
      dest[RIDX(dim - 1 - j, i+2, dim)] = src[RIDX(dimDiv2 + i+2, j, dim)];
      dest[RIDX(dim - 1 - j, i+3, dim)] = src[RIDX(dimDiv2 + i+3, j, dim)];
      dest[RIDX(dim - 1 - j, i+4, dim)] = src[RIDX(dimDiv2 + i+4, j, dim)];
      dest[RIDX(dim - 1 - j, i+5, dim)] = src[RIDX(dimDiv2 + i+5, j, dim)];
      dest[RIDX(dim - 1 - j, i+6, dim)] = src[RIDX(dimDiv2 + i+6, j, dim)];
      dest[RIDX(dim - 1 - j, i+7, dim)] = src[RIDX(dimDiv2 + i+7, j, dim)];
      dest[RIDX(dim - 1 - j, i+8, dim)] = src[RIDX(dimDiv2 + i+8, j, dim)];
      dest[RIDX(dim - 1 - j, i+9, dim)] = src[RIDX(dimDiv2 + i+9, j, dim)];
      dest[RIDX(dim - 1 - j, i+10, dim)] = src[RIDX(dimDiv2 + i+10, j, dim)];
      dest[RIDX(dim - 1 - j, i+11, dim)] = src[RIDX(dimDiv2 + i+11, j, dim)];
      dest[RIDX(dim - 1 - j, i+12, dim)] = src[RIDX(dimDiv2 + i+12, j, dim)];
      dest[RIDX(dim - 1 - j, i+13, dim)] = src[RIDX(dimDiv2 + i+13, j, dim)];
      dest[RIDX(dim - 1 - j, i+14, dim)] = src[RIDX(dimDiv2 + i+14, j, dim)];
      dest[RIDX(dim - 1 - j, i+15, dim)] = src[RIDX(dimDiv2 + i+15, j, dim)];
      }


  for (i = 0; i < dimDiv2; i+=16)
    for (j = 0; j < dimDiv2; j++){
      dest[RIDX(dim - 1 - j, dimDiv2 + i, dim)] = src[RIDX(dimDiv2 + i, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+1, dim)] = src[RIDX(dimDiv2 + i+1, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+2, dim)] = src[RIDX(dimDiv2 + i+2, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+3, dim)] = src[RIDX(dimDiv2 + i+3, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+4, dim)] = src[RIDX(dimDiv2 + i+4, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+5, dim)] = src[RIDX(dimDiv2 + i+5, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+6, dim)] = src[RIDX(dimDiv2 + i+6, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+7, dim)] = src[RIDX(dimDiv2 + i+7, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+8, dim)] = src[RIDX(dimDiv2 + i+8, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+9, dim)] = src[RIDX(dimDiv2 + i+9, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+10, dim)] = src[RIDX(dimDiv2 + i+10, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+11, dim)] = src[RIDX(dimDiv2 + i+11, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+12, dim)] = src[RIDX(dimDiv2 + i+12, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+13, dim)] = src[RIDX(dimDiv2 + i+13, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+14, dim)] = src[RIDX(dimDiv2 + i+14, dimDiv2 + j, dim)];
      dest[RIDX(dim - 1 - j, dimDiv2 + i+15, dim)] = src[RIDX(dimDiv2 + i+15, dimDiv2 + j, dim)];
      }  

}


/* 
 * rotate - Your current working version of pinwheel
 * IMPORTANT: This is the version you will be graded on
 */
char pinwheel_descr[] = "pinwheel_4: Current working version";
void pinwheel(int dim, pixel *src, pixel *dest)
{
  pinwheel_4(dim, src, dest);
}

/*********************************************************************
 * register_pinwheel_functions - Register all of your different versions
 *     of the pinwheel kernel with the driver by calling the
 *     add_pinwheel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_pinwheel_functions() {
	add_pinwheel_function(&pinwheel, pinwheel_descr);
	add_pinwheel_function(&naive_pinwheel, naive_pinwheel_descr);
	add_pinwheel_function(&pinwheel_1, pinwheel_1_descr);
	add_pinwheel_function(&pinwheel_2, pinwheel_2_descr);
	add_pinwheel_function(&pinwheel_3, pinwheel_3_descr);
        add_pinwheel_function(&pinwheel_4, pinwheel_4_descr);
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
  int red;
  int green;
  int blue;
} pixel_sum;

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
  sum->red = sum->green = sum->blue = 0;
  return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_weighted_sum(pixel_sum *sum, pixel p, double weight) 
{
  sum->red += (int) p.red * weight;
  sum->green += (int) p.green * weight;
  sum->blue += (int) p.blue * weight;
  return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_weighted_sum_2(pixel_sum *sum, pixel p, int weight) 
{
  sum->red += (int) p.red>>weight;
  sum->green += (int) p.green>>weight;
  sum->blue += (int) p.blue>>weight;
  return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
  current_pixel->red = (unsigned short)sum.red;
  current_pixel->green = (unsigned short)sum.green;
  current_pixel->blue = (unsigned short)sum.blue;
  return;
}

/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel_sum sum;
  pixel current_pixel;
  double weights[3][3] = { { 0.50, 0.03125, 0.00 },
                           { 0.03125, 0.25, 0.03125 },
                           { 0.00, 0.03125, 0.125 } };

  initialize_pixel_sum(&sum);
  for(ii=0; ii < 3; ii++)
    for(jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim))
        accumulate_weighted_sum(&sum,
                                src[RIDX(i+ii,j+jj,dim)],
                                weights[ii][jj]);
    
  assign_sum_to_pixel(&current_pixel, sum);
 
  return current_pixel;
}


/* 
 * faster_combo - Returns new pixel value at (i,j) 
 */
static pixel faster_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel_sum sum;
  pixel current_pixel;
  int weights[3][3] = { { 1, 5, 31 },
                           { 5, 2, 5 },
                           { 31, 5, 3 } };

  initialize_pixel_sum(&sum);
  for(ii=0; ii < 3; ii++)
    for(jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim))
        accumulate_weighted_sum_2(&sum,
                                src[RIDX(i+ii,j+jj,dim)],
                                weights[ii][jj]);
    
  assign_sum_to_pixel(&current_pixel, sum);
 
  return current_pixel;
}

/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/

/*
 * naive_motion - The naive baseline version of motion 
 */
char naive_motion_descr[] = "naive_motion: Naive baseline implementation";
void naive_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}

/*
 * motion_1 - Unrolled by 16.
 */
char motion_1_descr[] = "motion_1: Unrolled by 16";
void motion_1(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i+=16)
    for (j = 0; j < dim; j++){
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src); 
      dst[RIDX(i+1, j, dim)] = weighted_combo(dim, i+1, j, src);
      dst[RIDX(i+2, j, dim)] = weighted_combo(dim, i+2, j, src);
      dst[RIDX(i+3, j, dim)] = weighted_combo(dim, i+3, j, src);
      dst[RIDX(i+4, j, dim)] = weighted_combo(dim, i+4, j, src);
      dst[RIDX(i+5, j, dim)] = weighted_combo(dim, i+5, j, src);
      dst[RIDX(i+6, j, dim)] = weighted_combo(dim, i+6, j, src);
      dst[RIDX(i+7, j, dim)] = weighted_combo(dim, i+7, j, src);
      dst[RIDX(i+8, j, dim)] = weighted_combo(dim, i+8, j, src);
      dst[RIDX(i+9, j, dim)] = weighted_combo(dim, i+9, j, src);
      dst[RIDX(i+10, j, dim)] = weighted_combo(dim, i+10, j, src);
      dst[RIDX(i+11, j, dim)] = weighted_combo(dim, i+11, j, src);
      dst[RIDX(i+12, j, dim)] = weighted_combo(dim, i+12, j, src);
      dst[RIDX(i+13, j, dim)] = weighted_combo(dim, i+13, j, src);
      dst[RIDX(i+14, j, dim)] = weighted_combo(dim, i+14, j, src);
      dst[RIDX(i+15, j, dim)] = weighted_combo(dim, i+15, j, src);
    }
}


/*
 * motion_2 - Using altered weighted combo method
 */
char motion_2_descr[] = "motion_2: Using altered weighted combo method";
void motion_2(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = faster_combo(dim, i, j, src);
}

/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion_2: Current working version";
void motion(int dim, pixel *src, pixel *dst) 
{
  motion_2(dim, src, dst);
}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
	add_motion_function(&motion, motion_descr);
	add_motion_function(&naive_motion, naive_motion_descr);
	add_motion_function(&motion_1, motion_1_descr);
	add_motion_function(&motion_2, motion_2_descr);
}
