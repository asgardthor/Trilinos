/*====================================================================
 * ------------------------
 * | CVS File Information |
 * ------------------------
 *
 * $RCSfile$
 *
 * $Author$
 *
 * $Date$
 *
 * $Revision$
 *
 *====================================================================*/

#include "lb_const.h"
#include "all_allo_const.h"


void LB_Free_Params(
LB *lb)				/* load balance structure */
{
/*
 * Free the list of new parameter values.
 */
    LB_PARAM *ptr, *ptr2;	/* loops through parameter list */


    ptr = lb->Params;
    while (ptr != NULL) {
	ptr2 = ptr->next;
	LB_FREE(&(ptr->name));
	LB_FREE(&(ptr->new_val));
	LB_FREE(&ptr);
	ptr = ptr2;
    }

    lb->Params = NULL;
}
