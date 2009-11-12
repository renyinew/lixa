/*
 * Copyright (c) 2009, Christian Ferrari
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <config.h>



#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#endif



#include <lixa_errors.h>
#include <lixa_trace.h>
#include <lixa_xml_msg.h>



/* set module trace flag */
#ifdef LIXA_TRACE_MODULE
# undef LIXA_TRACE_MODULE
#endif /* LIXA_TRACE_MODULE */
#define LIXA_TRACE_MODULE   LIXA_TRACE_MOD_COMMON_XML_MSG



const xmlChar *LIXA_XML_MSG_PROP_LEVEL =   (xmlChar *)"level";
const xmlChar *LIXA_XML_MSG_PROP_NAME =    (xmlChar *)"name";
const xmlChar *LIXA_XML_MSG_PROP_PROFILE = (xmlChar *)"profile";
const xmlChar *LIXA_XML_MSG_PROP_RMID =    (xmlChar *)"rmid";
const xmlChar *LIXA_XML_MSG_PROP_STEP =    (xmlChar *)"step";
const xmlChar *LIXA_XML_MSG_PROP_SYNC =    (xmlChar *)"sync";
const xmlChar *LIXA_XML_MSG_PROP_VERB =    (xmlChar *)"verb";
const xmlChar *LIXA_XML_MSG_PROP_WAIT =    (xmlChar *)"wait";
const xmlChar *LIXA_XML_MSG_TAG_CLIENT =   (xmlChar *)"client";
const xmlChar *LIXA_XML_MSG_TAG_MSG =      (xmlChar *)"msg";
const xmlChar *LIXA_XML_MSG_TAG_RSRMGR =   (xmlChar *)"rsrmgr";
const xmlChar *LIXA_XML_MSG_TAG_RSRMGRS =  (xmlChar *)"rsrmgrs";



int lixa_msg_serialize(const struct lixa_msg_s *msg,
                       char *buffer, size_t buffer_len,
                       int *msg_len)
{
    enum Exception { BUFFER_TOO_SHORT1
                     , BUFFER_TOO_SHORT2
                     , BUFFER_TOO_SHORT3
                     , BUFFER_TOO_SHORT4
                     , BUFFER_TOO_SHORT5
                     , BUFFER_TOO_SHORT6
                     , INVALID_OPEN_STEP
                     , INVALID_VERB
                     , BUFFER_TOO_SHORT999
                     , NONE } excp;
    int ret_cod = LIXA_RC_INTERNAL_ERROR;

    int used_chars = 0, offset = 0;
    size_t free_chars = buffer_len;
    
    LIXA_TRACE(("lixa_msg_serialize\n"));
    TRY {
        guint i;
        used_chars = snprintf(buffer, free_chars,
                              "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>");
        if (used_chars >= free_chars)
            THROW(BUFFER_TOO_SHORT1);
        /* <msg ... > */
        free_chars -= used_chars;
        offset += used_chars;
        used_chars = snprintf(buffer + offset, free_chars,
                              "<%s %s=\"%d\" %s=\"%d\" %s=\"%d\" %s=\"%d\" "
                              "%s=\"%d\">",
                              LIXA_XML_MSG_TAG_MSG,
                              LIXA_XML_MSG_PROP_LEVEL,
                              msg->header.level,
                              LIXA_XML_MSG_PROP_VERB,
                              msg->header.verb,
                              LIXA_XML_MSG_PROP_STEP,
                              msg->header.step,
                              LIXA_XML_MSG_PROP_WAIT,
                              msg->header.wait,
                              LIXA_XML_MSG_PROP_SYNC,
                              msg->header.sync);
        if (used_chars >= free_chars)
            THROW(BUFFER_TOO_SHORT2);
        free_chars -= used_chars;
        offset += used_chars;

        switch (msg->header.verb) {
            case LIXA_MSG_VERB_OPEN:
                switch (msg->header.step) {
                    case 1:
                        /* <client> */
                        used_chars = snprintf(buffer + offset, free_chars,
                                              "<%s %s=\"%s\"/>",
                                              LIXA_XML_MSG_TAG_CLIENT,
                                              LIXA_XML_MSG_PROP_PROFILE,
                                              msg->body.open_1.client.profile);
                        if (used_chars >= free_chars)
                            THROW(BUFFER_TOO_SHORT3);
                        free_chars -= used_chars;
                        offset += used_chars;
                        /* <rsrmgrs> */
                        used_chars = snprintf(buffer + offset, free_chars,
                                              "<%s>",
                                              LIXA_XML_MSG_TAG_RSRMGRS);
                        if (used_chars >= free_chars)
                            THROW(BUFFER_TOO_SHORT4);
                        free_chars -= used_chars;
                        offset += used_chars;
                        /* <rsrmgr> */
                        for (i=0; i<msg->body.open_1.rsrmgrs->len; ++i) {
                            struct lixa_msg_body_open_1_rsrmgr_s *rsrmgr;
                            rsrmgr = &g_array_index(
                                msg->body.open_1.rsrmgrs,
                                struct lixa_msg_body_open_1_rsrmgr_s, i);
                            used_chars = snprintf(buffer + offset, free_chars,
                                                  "<%s %s=\"%d\" %s=\"%s\"/>",
                                                  LIXA_XML_MSG_TAG_RSRMGR,
                                                  LIXA_XML_MSG_PROP_RMID,
                                                  rsrmgr->rmid,
                                                  LIXA_XML_MSG_PROP_NAME,
                                                  rsrmgr->name);
                            if (used_chars >= free_chars)
                                THROW(BUFFER_TOO_SHORT5);
                            free_chars -= used_chars;
                            offset += used_chars;
                        }
                        /* </rsrmgrs> */
                        used_chars = snprintf(buffer + offset, free_chars,
                                              "</%s>",
                                              LIXA_XML_MSG_TAG_RSRMGRS);
                        if (used_chars >= free_chars)
                            THROW(BUFFER_TOO_SHORT6);
                        free_chars -= used_chars;
                        offset += used_chars;
                        break;
                    default:
                        THROW(INVALID_OPEN_STEP);
                }
                break;
            default:
                THROW(INVALID_VERB);
        }

        /* @@@ */

        /* </msg> */
        used_chars = snprintf(buffer + offset, free_chars,
                              "</%s>", LIXA_XML_MSG_TAG_MSG);
        if (used_chars >= free_chars)
            THROW(BUFFER_TOO_SHORT999);
        free_chars -= used_chars;
        offset += used_chars;

        *msg_len = offset;
        THROW(NONE);
    } CATCH {
        switch (excp) {
            case BUFFER_TOO_SHORT1:
            case BUFFER_TOO_SHORT2:
            case BUFFER_TOO_SHORT3:
            case BUFFER_TOO_SHORT4:
            case BUFFER_TOO_SHORT5:
            case BUFFER_TOO_SHORT6:
                ret_cod = LIXA_RC_CONTAINER_FULL;
                break;
            case INVALID_OPEN_STEP:
            case INVALID_VERB:
                ret_cod = LIXA_RC_INTERNAL_ERROR;
                break;
            case BUFFER_TOO_SHORT999:
                ret_cod = LIXA_RC_CONTAINER_FULL;
                break;
            case NONE:
                ret_cod = LIXA_RC_OK;
                break;
            default:
                ret_cod = LIXA_RC_INTERNAL_ERROR;
        } /* switch (excp) */
    } /* TRY-CATCH */
    LIXA_TRACE(("lixa_msg_serialize/excp=%d/"
                "ret_cod=%d/errno=%d\n", excp, ret_cod, errno));
    return ret_cod;
}



int lixa_msg_deserialize(const char *buffer, size_t buffer_len,
                         struct lixa_msg_s *msg)
{
    enum Exception { XML_READ_MEMORY_ERROR
                     , NONE } excp;
    int ret_cod = LIXA_RC_INTERNAL_ERROR;

    xmlDocPtr doc = NULL;
    
    LIXA_TRACE(("lixa_msg_deserialize\n"));
    TRY {
        if (NULL == (doc = xmlReadMemory(buffer, (int)buffer_len, "buffer.xml",
                                         NULL, 0)))
            THROW(XML_READ_MEMORY_ERROR);

        /* @@@ perform deserialization logic (tree navigation) */

        THROW(NONE);
    } CATCH {
        switch (excp) {
            case XML_READ_MEMORY_ERROR:
                ret_cod = LIXA_RC_XML_READ_MEMORY_ERROR;
                break;
            case NONE:
                ret_cod = LIXA_RC_OK;
                break;
            default:
                ret_cod = LIXA_RC_INTERNAL_ERROR;
        } /* switch (excp) */
        /* recover resources */
        if (NULL != doc) {
            /* free parsed document */
            xmlFreeDoc(doc);
            /* release libxml2 stuff */
            xmlCleanupParser();
        }
    } /* TRY-CATCH */
    LIXA_TRACE(("lixa_msg_deserialize/excp=%d/"
                "ret_cod=%d/errno=%d\n", excp, ret_cod, errno));
    return ret_cod;
}

