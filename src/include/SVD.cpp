// Copyright (C) 2009 foam
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

// from http://www.public.iastate.edu/~dicook/JSS/paper/paper.html

/************************************************************
 *                                                          *
 *  Permission is hereby granted  to  any  individual   or  *
 *  institution   for  use,  copying, or redistribution of  *
 *  this code and associated documentation,  provided       *
 *  that   such  code  and documentation are not sold  for  *
 *  profit and the  following copyright notice is retained  *
 *  in the code and documentation:                          *
 *     Copyright (c) held by Dianne Cook                    *
 *  All Rights Reserved.                                    *
 *                                                          *
 *  Questions and comments are welcome, and I request       *
 *  that you share any modifications with me.               *
 *                                                          *
 *                Dianne Cook                               *
 *             dicook@iastate.edu                           *
 *                                                          *
 ************************************************************/

/* 
 * svdcomp - SVD decomposition routine. 
 * Takes an mxn matrix a and decomposes it into udv, where u,v are
 * left and right orthogonal transformation matrices, and d is a 
 * diagonal matrix of singular values.
 *
 * This routine is adapted from svdecomp.c in XLISP-STAT 2.1 which is 
 * code from Numerical Recipes adapted by Luke Tierney and David Betz modified to work lib independently by L.K.
 *
 * Input to dsvd is as follows:
 *   a = mxn matrix to be decomposed, gets overwritten with u
 *   m = row dimension of a
 *   n = column dimension of a
 *   w = returns the vector of singular values of a
 *   v = returns the right orthogonal transformation matrix
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <list>

using namespace std;

#define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define MAX(x,y) ((x)>(y)?(x):(y))

///////////////////////////////////////////////////////

// Vector<float> SVD(Matrix<float> &m)
// {
// 	Vector<float> w(m.GetRows());
// 	Matrix<float> v(m.GetRows(),m.GetCols());
// 	dsvd(m, m.GetRows(), m.GetCols(), w.GetRawData(), v);
// 	m.SortCols(w);
// 	return w;
// }

///////////////////////////////////////////////////////

static double PYTHAG(double a, double b)
{
    double at = fabs(a), bt = fabs(b), ct, result;

    if (at > bt)       { ct = bt / at; result = at * sqrt(1.0 + ct * ct); }
    else if (bt > 0.0) { ct = at / bt; result = bt * sqrt(1.0 + ct * ct); }
    else result = 0.0;
    return(result);
}

static int dsvd(double a[][4], int m, int n, double *w, double v[][4])
{
    int flag, i, its, j, jj, k, l, nm;
    double c, f, h, s, x, y, z;
    double anorm = 0.0, g = 0.0, scale = 0.0;
    double *rv1;
  
    if (m < n) 
    {
        fprintf(stderr, "#rows must be > #cols \n");
        return(0);
    }
  
    rv1 = (double *)malloc((unsigned int) n*sizeof(double));

/* Householder reduction to bidiagonal form */
    for (i = 0; i < n; i++) 
    {
        /* left-hand reduction */
        l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        if (i < m) 
        {
            for (k = i; k < m; k++) 
                scale += fabs(a[k][i]);
            if (scale) 
            {
                for (k = i; k < m; k++) 
                {
                    a[k][i] = (a[k][i]/scale);
                    s += (a[k][i] * a[k][i]);
                }
                f = a[i][i];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                a[i][i] = (f - g);
                if (i != n - 1) 
                {
                    for (j = l; j < n; j++) 
                    {
                        for (s = 0.0, k = i; k < m; k++) 
                            s += (a[k][i] * a[k][j]);
                        f = s / h;
                        for (k = i; k < m; k++) 
                            a[k][j] += (f * a[k][i]);
                    }
                }
                for (k = i; k < m; k++) 
                    a[k][i] = (a[k][i]*scale);
            }
        }
        w[i] = (scale * g);
    
        /* right-hand reduction */
        g = s = scale = 0.0;
        if (i < m && i != n - 1) 
        {
            for (k = l; k < n; k++) 
                scale += fabs(a[i][k]);
            if (scale) 
            {
                for (k = l; k < n; k++) 
                {
                    a[i][k] = (a[i][k]/scale);
                    s += (a[i][k] * a[i][k]);
                }
                f = a[i][l];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                a[i][l] = (f - g);
                for (k = l; k < n; k++) 
                    rv1[k] = a[i][k] / h;
                if (i != m - 1) 
                {
                    for (j = l; j < m; j++) 
                    {
                        for (s = 0.0, k = l; k < n; k++) 
                            s += (a[j][k] * a[i][k]);
                        for (k = l; k < n; k++) 
                            a[j][k] += (s * rv1[k]);
                    }
                }
                for (k = l; k < n; k++) 
                    a[i][k] = (a[i][k]*scale);
            }
        }
        anorm = MAX(anorm, (fabs(w[i]) + fabs(rv1[i])));
    }
  
    /* accumulate the right-hand transformation */
    for (i = n - 1; i >= 0; i--) 
    {
        if (i < n - 1) 
        {
            if (g) 
            {
                for (j = l; j < n; j++)
                    v[j][i] = ((a[i][j] / a[i][l]) / g);
                    /* double division to avoid underflow */
                for (j = l; j < n; j++) 
                {
                    for (s = 0.0, k = l; k < n; k++) 
                        s += (a[i][k] * v[k][j]);
                    for (k = l; k < n; k++) 
                        v[k][j] += (s * v[k][i]);
                }
            }
            for (j = l; j < n; j++) 
                v[i][j] = v[j][i] = 0.0;
        }
        v[i][i] = 1.0;
        g = rv1[i];
        l = i;
    }
  
    /* accumulate the left-hand transformation */
    for (i = n - 1; i >= 0; i--) 
    {
        l = i + 1;
        g = w[i];
        if (i < n - 1) 
            for (j = l; j < n; j++) 
                a[i][j] = 0.0;
        if (g) 
        {
            g = 1.0 / g;
            if (i != n - 1) 
            {
                for (j = l; j < n; j++) 
                {
                    for (s = 0.0, k = l; k < m; k++) 
                        s += (a[k][i] * a[k][j]);
                    f = (s / a[i][i]) * g;
                    for (k = i; k < m; k++) 
                        a[k][j] += (f * a[k][i]);
                }
            }
            for (j = i; j < m; j++) 
                a[j][i] = (a[j][i]*g);
        }
        else 
        {
            for (j = i; j < m; j++) 
                a[j][i] = 0.0;
        }
        ++a[i][i];
    }

    /* diagonalize the bidiagonal form */
    for (k = n - 1; k >= 0; k--) 
    {                             /* loop over singular values */
        for (its = 0; its < 30; its++) 
        {                         /* loop over allowed iterations */
            flag = 1;
            for (l = k; l >= 0; l--) 
            {                     /* test for splitting */
                nm = l - 1;
                if (fabs(rv1[l]) + anorm == anorm) 
                {
                    flag = 0;
                    break;
                }
                if (fabs(w[nm]) + anorm == anorm) 
                    break;
            }
            if (flag) 
            {
                c = 0.0;
                s = 1.0;
                for (i = l; i <= k; i++) 
                {
                    f = s * rv1[i];
                    if (fabs(f) + anorm != anorm) 
                    {
                        g = w[i];
                        h = PYTHAG(f, g);
                        w[i] = h; 
                        h = 1.0 / h;
                        c = g * h;
                        s = (- f * h);
                        for (j = 0; j < m; j++) 
                        {
                            y = a[j][nm];
                            z = a[j][i];
                            a[j][nm] = (y * c + z * s);
                            a[j][i] = (z * c - y * s);
                        }
                    }
                }
            }
            z = w[k];
            if (l == k) 
            {                  /* convergence */
                if (z < 0.0) 
                {              /* make singular value nonnegative */
                    w[k] = (-z);
                    for (j = 0; j < n; j++) 
                        v[j][k] = (-v[j][k]);
                }
                break;
            }
            if (its >= 30) {
                free((void*) rv1);
                fprintf(stderr, "No convergence after 30,000! iterations \n");
                return(0);
            }
    
            /* shift from bottom 2 x 2 minor */
            x = w[l];
            nm = k - 1;
            y = w[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
            g = PYTHAG(f, 1.0);
            f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;
          
            /* next QR transformation */
            c = s = 1.0;
            for (j = l; j <= nm; j++) 
            {
                i = j + 1;
                g = rv1[i];
                y = w[i];
                h = s * g;
                g = c * g;
                z = PYTHAG(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y = y * c;
                for (jj = 0; jj < n; jj++) 
                {
                    x = v[jj][j];
                    z = v[jj][i];
                    v[jj][j] = (x * c + z * s);
                    v[jj][i] = (z * c - x * s);
                }
                z = PYTHAG(f, h);
                w[j] = z;
                if (z) 
                {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }
                f = (c * g) + (s * y);
                x = (c * y) - (s * g);
                for (jj = 0; jj < m; jj++) 
                {
                    y = a[jj][j];
                    z = a[jj][i];
                    a[jj][j] = (y * c + z * s);
                    a[jj][i] = (z * c - y * s);
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = x;
        }
    }
    free((void*) rv1);
    return(1);
}
