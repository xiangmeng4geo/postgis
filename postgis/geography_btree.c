/**********************************************************************
 *
 * PostGIS - Spatial Types for PostgreSQL
 * http://postgis.net
 *
 * PostGIS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * PostGIS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PostGIS.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************
 *
 * Copyright 2009 Paul Ramsey <pramsey@cleverelephant.ca>
 *
 **********************************************************************/


#include "postgres.h"
#include "access/hash.h"

#include "../postgis_config.h"

#include "liblwgeom.h"         /* For standard geometry types. */
#include "liblwgeom_internal.h"         /* For FP comparators. */
#include "lwgeom_pg.h"       /* For debugging macros. */
#include "gserialized_gist.h"
#include "geography.h"	     /* For utility functions. */

Datum geography_lt(PG_FUNCTION_ARGS);
Datum geography_le(PG_FUNCTION_ARGS);
Datum geography_eq(PG_FUNCTION_ARGS);
Datum geography_ge(PG_FUNCTION_ARGS);
Datum geography_gt(PG_FUNCTION_ARGS);
Datum geography_cmp(PG_FUNCTION_ARGS);


/*
** Utility function to return the center point of a
** geocentric bounding box. We don't divide by two
** because we're only using the values for comparison.
*/
static void geography_gidx_center(const GIDX *gidx, POINT3D *p)
{
	p->x = GIDX_GET_MIN(gidx, 0) + GIDX_GET_MAX(gidx, 0);
	p->y = GIDX_GET_MIN(gidx, 1) + GIDX_GET_MAX(gidx, 1);
	p->z = GIDX_GET_MIN(gidx, 2) + GIDX_GET_MAX(gidx, 2);
}

/*
** BTree support function. Based on two geographies return true if
** they are "less than" and false otherwise.
*/
PG_FUNCTION_INFO_V1(geography_lt);
Datum geography_lt(PG_FUNCTION_ARGS)
{
	GSERIALIZED *g1 = PG_GETARG_GSERIALIZED_P(0);
	GSERIALIZED *g2 = PG_GETARG_GSERIALIZED_P(1);
	int cmp = gserialized_cmp(g1, g2);
	if (cmp < 0)
		PG_RETURN_BOOL(TRUE);
	else
		PG_RETURN_BOOL(FALSE);	
}

/*
** BTree support function. Based on two geographies return true if
** they are "less than or equal" and false otherwise.
*/
PG_FUNCTION_INFO_V1(geography_le);
Datum geography_le(PG_FUNCTION_ARGS)
{
	GSERIALIZED *g1 = PG_GETARG_GSERIALIZED_P(0);
	GSERIALIZED *g2 = PG_GETARG_GSERIALIZED_P(1);
	int cmp = gserialized_cmp(g1, g2);
	if (cmp <= 0)
		PG_RETURN_BOOL(TRUE);
	else
		PG_RETURN_BOOL(FALSE);	
}

/*
** BTree support function. Based on two geographies return true if
** they are "greater than" and false otherwise.
*/
PG_FUNCTION_INFO_V1(geography_gt);
Datum geography_gt(PG_FUNCTION_ARGS)
{
	GSERIALIZED *g1 = PG_GETARG_GSERIALIZED_P(0);
	GSERIALIZED *g2 = PG_GETARG_GSERIALIZED_P(1);
	int cmp = gserialized_cmp(g1, g2);
	if (cmp > 0)
		PG_RETURN_BOOL(TRUE);
	else
		PG_RETURN_BOOL(FALSE);	
}

/*
** BTree support function. Based on two geographies return true if
** they are "greater than or equal" and false otherwise.
*/
PG_FUNCTION_INFO_V1(geography_ge);
Datum geography_ge(PG_FUNCTION_ARGS)
{
	GSERIALIZED *g1 = PG_GETARG_GSERIALIZED_P(0);
	GSERIALIZED *g2 = PG_GETARG_GSERIALIZED_P(1);
	int cmp = gserialized_cmp(g1, g2);
	if (cmp >= 0)
		PG_RETURN_BOOL(TRUE);
	else
		PG_RETURN_BOOL(FALSE);	
}

/*
** BTree support function. Based on two geographies return true if
** they are "equal" and false otherwise.
*/
PG_FUNCTION_INFO_V1(geography_eq);
Datum geography_eq(PG_FUNCTION_ARGS)
{
	GSERIALIZED *g1 = PG_GETARG_GSERIALIZED_P(0);
	GSERIALIZED *g2 = PG_GETARG_GSERIALIZED_P(1);
	int cmp = gserialized_cmp(g1, g2);
	if (cmp == 0)
		PG_RETURN_BOOL(TRUE);
	else
		PG_RETURN_BOOL(FALSE);	
}

/*
** BTree support function. Based on two geographies return true if
** they are "equal" and false otherwise.
*/
PG_FUNCTION_INFO_V1(geography_cmp);
Datum geography_cmp(PG_FUNCTION_ARGS)
{
	GSERIALIZED *g1 = PG_GETARG_GSERIALIZED_P(0);
	GSERIALIZED *g2 = PG_GETARG_GSERIALIZED_P(1);
	PG_RETURN_INT32(gserialized_cmp(g1, g2));
}
