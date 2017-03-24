/*
 * Copyright (c) 2009-2017, Christian Ferrari <tiian@users.sourceforge.net>
 * All rights reserved.
 *
 * This file is part of LIXA.
 *
 * LIXA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * LIXA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LIXA.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <config.h>



/* system includes */
#ifdef HAVE_GLIB_H
# include <glib.h>
#endif
/* LIXA includes */
#include "lixa_errors.h"
#include "lixa_trace.h"
/* XTA includes */
#include "xta_transaction.h"



/* set module trace flag */
#ifdef LIXA_TRACE_MODULE
# undef LIXA_TRACE_MODULE
#endif /* LIXA_TRACE_MODULE */
#define LIXA_TRACE_MODULE   LIXA_TRACE_MOD_XTA



xta_transaction_t *xta_transaction_new(void)
{
    enum Exception { G_TRY_MALLOC_ERROR
                     , NONE } excp;
    int ret_cod = LIXA_RC_INTERNAL_ERROR;
    xta_transaction_t *t = NULL;
    
    LIXA_TRACE(("xta_transaction_t\n"));
    TRY {
        /* allocate the object */
        if (NULL == (t = (xta_transaction_t *)
                     g_try_malloc0(sizeof(xta_transaction_t))))
            THROW(G_TRY_MALLOC_ERROR);
        /* @@@ initialize the object */
        THROW(NONE);
    } CATCH {
        switch (excp) {
            case G_TRY_MALLOC_ERROR:
                ret_cod = LIXA_RC_G_TRY_MALLOC_ERROR;
                break;
            case NONE:
                ret_cod = LIXA_RC_OK;
                break;
            default:
                ret_cod = LIXA_RC_INTERNAL_ERROR;
        } /* switch (excp) */
    } /* TRY-CATCH */
    LIXA_TRACE(("xta_transaction_t/excp=%d/"
                "ret_cod=%d/errno=%d\n", excp, ret_cod, errno));
    XTA_LAST_OPERATION_SET(t, excp, ret_cod);
    return t;
}



void xta_transaction_delete(xta_transaction_t *t)
{
    enum Exception { NONE } excp;
    int ret_cod = LIXA_RC_INTERNAL_ERROR;
    
    LIXA_TRACE(("xta_transaction_delete\n"));
    TRY {
        /* @@@ destroy the object content if necessary */

        g_free(t);
        
        THROW(NONE);
    } CATCH {
        switch (excp) {
            case NONE:
                ret_cod = LIXA_RC_OK;
                break;
            default:
                ret_cod = LIXA_RC_INTERNAL_ERROR;
        } /* switch (excp) */
    } /* TRY-CATCH */
    LIXA_TRACE(("xta_transaction_delete/excp=%d/"
                "ret_cod=%d/errno=%d\n", excp, ret_cod, errno));
    return;
}



int xta_transaction_delist_resource(xta_transaction_t *transaction,
                                    const xta_resource_t *resource,
                                    long flag)
{
    enum Exception { NONE } excp;
    int ret_cod = LIXA_RC_INTERNAL_ERROR;
    
    LIXA_TRACE(("xta_transaction_delist_resource\n"));
    TRY {
        /* @@@ */
        
        THROW(NONE);
    } CATCH {
        switch (excp) {
            case NONE:
                ret_cod = LIXA_RC_OK;
                break;
            default:
                ret_cod = LIXA_RC_INTERNAL_ERROR;
        } /* switch (excp) */
    } /* TRY-CATCH */
    LIXA_TRACE(("xta_transaction_delist_resource/excp=%d/"
                "ret_cod=%d/errno=%d\n", excp, ret_cod, errno));
    return ret_cod;
}



int xta_transaction_enlist_resource(xta_transaction_t *transaction,
                                    const xta_resource_t *resource)
{
    enum Exception { NONE } excp;
    int ret_cod = LIXA_RC_INTERNAL_ERROR;
    
    LIXA_TRACE(("xta_transaction_enlist_resource\n"));
    TRY {
        /* @@@ */
        
        THROW(NONE);
    } CATCH {
        switch (excp) {
            case NONE:
                ret_cod = LIXA_RC_OK;
                break;
            default:
                ret_cod = LIXA_RC_INTERNAL_ERROR;
        } /* switch (excp) */
    } /* TRY-CATCH */
    LIXA_TRACE(("xta_transaction_enlist_resource/excp=%d/"
                "ret_cod=%d/errno=%d\n", excp, ret_cod, errno));
    return ret_cod;
}

