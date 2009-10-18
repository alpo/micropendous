/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include <avr/pgmspace.h>
#include <string.h>

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "http-strings.h"

#define NUM_HDR_STRINGS 3

struct http_state {
  PGM_P file;

  u16_t left;
  u8_t retries;
  
  PGM_P header[NUM_HDR_STRINGS]; /* HTTP headers to be sent. */
  u16_t header_pos;     /* The position of the first unsent header byte in the
                           current string */
  u16_t header_index;   /* The index of the hdr string currently being sent. */
};

/*-----------------------------------------------------------------------------------*/
static void
conn_err(void *arg, err_t err)
{
  struct http_state *hs;

  LWIP_UNUSED_ARG(err);

  hs = arg;
  mem_free(hs);
}
/*-----------------------------------------------------------------------------------*/
static void
close_conn(struct tcp_pcb *pcb, struct http_state *hs)
{
  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
  tcp_recv(pcb, NULL);
  mem_free(hs);
  tcp_close(pcb);
}
/*-----------------------------------------------------------------------------------*/
static void
send_data(struct tcp_pcb *pcb, struct http_state *hs)
{
  err_t err;
  u16_t len;
	u8_t data_to_send = 0;
  u16_t hdrlen, sendlen;

  /* Assume no error until we find otherwise */
  err = ERR_OK;

  /* Do we have any more header data to send for this file? */
  if(hs->header_index < NUM_HDR_STRINGS)
  {
      /* How much data can we send? */
      len = tcp_sndbuf(pcb);
      sendlen = len;

      while(len && (hs->header_index < NUM_HDR_STRINGS) && sendlen)
      {
          /* How much do we have to send from the current header? */
          hdrlen = strlen_P(hs->header[hs->header_index]);

          /* How much of this can we send? */
          sendlen = (len < (hdrlen - hs->header_pos)) ?
                          len : (hdrlen - hs->header_pos);

          /* Send this amount of data or as much as we can given memory
           * constraints. */
          do {
            err = tcp_write(pcb, (const void *)(hs->header[hs->header_index] +
                            hs->header_pos), sendlen, TCP_WRITE_FLAG_PGM_SPACE);
            if (err == ERR_MEM) {
              sendlen /= 2;
            }
            else if (err == ERR_OK) {
              /* Remember that we added some more data to be transmitted. */
              data_to_send = 1;
            }
          } while ((err == ERR_MEM) && sendlen);

          /* Fix up the header position for the next time round. */
          hs->header_pos += sendlen;
          len -= sendlen;

          /* Have we finished sending this string? */
          if(hs->header_pos == hdrlen) {
              /* Yes - move on to the next one */
              hs->header_index++;
              hs->header_pos = 0;
          }
      }

      /* If we get here and there are still header bytes to send, we send
       * the header information we just wrote immediately.  If there are no
       * more headers to send, but we do have file data to send, drop through
       * to try to send some file data too.
       */
      if((hs->header_index < NUM_HDR_STRINGS) || !hs->file) {
        tcp_output(pcb);
        return;
      }
  }

  /* We cannot send more data than space available in the send
     buffer. */     
  if (tcp_sndbuf(pcb) < hs->left) {
    len = tcp_sndbuf(pcb);
  } else {
    len = hs->left;
  }

  do {
    err = tcp_write(pcb, hs->file, len, TCP_WRITE_FLAG_PGM_SPACE);
    if (err == ERR_MEM) {
      len /= 2;
    }
  } while (err == ERR_MEM && len > 1);
  
  if (err == ERR_OK) {
    hs->file += len;
    hs->left -= len;
    /*  } else {
    printf("send_data: error %s len %d %d\n", lwip_strerr(err), len, tcp_sndbuf(pcb));*/
  }
}

/*-----------------------------------------------------------------------------------*/
static err_t
http_poll(void *arg, struct tcp_pcb *pcb)
{
  struct http_state *hs;

  hs = arg;
  
  /*  printf("Polll\n");*/
  if (hs == NULL) {
    /*    printf("Null, close\n");*/
    tcp_abort(pcb);
    return ERR_ABRT;
  } else {
    ++hs->retries;
    if (hs->retries == 8) {
      tcp_abort(pcb);
      return ERR_ABRT;
    }
    if(hs->file) send_data(pcb, hs);
  }

  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
static err_t
http_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  struct http_state *hs;

  LWIP_UNUSED_ARG(len);

  hs = arg;

  hs->retries = 0;
  
  if (hs->left > 0) {    
    send_data(pcb, hs);
  } else {
    close_conn(pcb, hs);
  }

  return ERR_OK;
}


/*-----------------------------------------------------------------------------------*/
static err_t
http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  int i;
  char *data,*uri,*_work,*file_name;
  u16_t uri_len;
  struct fs_file file;
  struct http_state *hs;

  hs = arg;
	
  if (err == ERR_OK && p != NULL) {
		

    if (strncmp_P(p->payload, PSTR("GET "), 4)) //wait for get 
			return ERR_OK;

    /* Inform TCP that we have taken the data. */
    tcp_recved(pcb, p->tot_len);
    
    if (hs->file == NULL) {
      data = p->payload;
      data[p->tot_len]=0;
      
      if (strncmp_P(data, PSTR("GET "), 4) == 0) {
        for(i = 0; i < 40; i++) {
          if (((char *)data + 4)[i] == ' ' ||
             ((char *)data + 4)[i] == '\r' ||
             ((char *)data + 4)[i] == '\n') {
             ((char *)data + 4)[i] = 0;
          }
        }
        
        uri=(char *)data + 4;
        uri_len=p->tot_len -4;
        
        if((_work=strchr(uri,' '))) //TODO: parse parameters?
        {
          *_work=0;//stop parsing for now
        }
        
        if((_work=strchr(uri,'?'))) //TODO: parse parameters?
        {
          *_work=0;//stop parsing for now
        }
				file_name=uri;

				hs->header[0]=http_header_200;

        if (file_name[0] == '/' &&
            file_name[1] == 0) {
          if(!fs_open(file_name="/index.html", &file))
						fs_open(file_name="/index.shtml", &file);
        } else if (!fs_open(file_name, &file)) {
          fs_open("/404.html", &file);
					hs->header[0]=http_header_404;
          file_name=NULL;
				}
        hs->header[1]=http_server;
        hs->file = file.data;
        hs->left = file.len;
				hs->header[2]=file.mime;

				hs->header_index=0;
				hs->header_pos=0;	

        pbuf_free(p);
        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      } else {
        pbuf_free(p);
        close_conn(pcb, hs);
      }
    } else {
      pbuf_free(p);
    }
  }

  if (err == ERR_OK && p == NULL) {
    close_conn(pcb, hs);
  }
  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
static err_t
http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  struct http_state *hs;

  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  tcp_setprio(pcb, TCP_PRIO_MIN);
  
  /* Allocate memory for the structure that holds the state of the
     connection. */
  hs = (struct http_state *)mem_malloc(sizeof(struct http_state));

  if (hs == NULL) {
    //printf("http_accept: Out of memory\n");
    return ERR_MEM;
  }
  
  /* Initialize the structure. */
  hs->file = NULL;
  hs->left = 0;
  hs->retries = 0;
	hs->header_index=0;
	hs->header_pos=0;	
  
  /* Tell TCP that this is the structure we wish to be passed for our
     callbacks. */
  tcp_arg(pcb, hs);

  /* Tell TCP that we wish to be informed of incoming data by a call
     to the http_recv() function. */
  tcp_recv(pcb, http_recv);

  tcp_err(pcb, conn_err);
  
  tcp_poll(pcb, http_poll, 4);
  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
void
httpd_init(void)
{
  struct tcp_pcb *pcb;

  pcb = tcp_new();
  tcp_bind(pcb, IP_ADDR_ANY, 80);
  pcb = tcp_listen(pcb);
  tcp_accept(pcb, http_accept);
}
/*-----------------------------------------------------------------------------------*/

