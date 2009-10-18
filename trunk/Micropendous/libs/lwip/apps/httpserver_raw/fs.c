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
#include "lwip/def.h"
#include "http-strings.h"
#include "fs.h"

#define HTTPD_FS_NUMFILES 2

typedef struct _httpd_fsdata_file {
  PGM_P mime;
  PGM_P name;
  PGM_P data;
  const int len;
} httpd_fsdata;


static prog_char data_404_html[] = {
	/* /404.html */
	0x2f, 0x34, 0x30, 0x34, 0x2e, 0x68, 0x74, 0x6d, 0x6c, 0,
	0x3c, 0x68, 0x74, 0x6d, 0x6c, 0x3e, 0x3c, 0x62, 0x6f, 0x64, 
	0x79, 0x3e, 0x3c, 0x63, 0x65, 0x6e, 0x74, 0x65, 0x72, 0x3e, 
	0x34, 0x30, 0x34, 0x20, 0x2d, 0x20, 0x66, 0x69, 0x6c, 0x65, 
	0x20, 0x6e, 0x6f, 0x74, 0x20, 0x66, 0x6f, 0x75, 0x6e, 0x64, 
	0x3c, 0x2f, 0x63, 0x65, 0x6e, 0x74, 0x65, 0x72, 0x3e, 0x3c, 
	0x2f, 0x62, 0x6f, 0x64, 0x79, 0x3e, 0x3c, 0x2f, 0x68, 0x74, 
	0x6d, 0x6c, 0x3e, 0xa, 0};

static prog_char data_index_html[] = {
	/* /index.html */
	0x2f, 0x69, 0x6e, 0x64, 0x65, 0x78, 0x2e, 0x68, 0x74, 0x6d, 0x6c, 0,
	0x3c, 0x68, 0x74, 0x6d, 0x6c, 0x3e, 0xa, 0x3c, 0x68, 0x65, 
	0x61, 0x64, 0x65, 0x72, 0x3e, 0xa, 0x6c, 0x77, 0x48, 0x54, 
	0x54, 0x50, 0x44, 0x20, 0xa, 0x3c, 0x2f, 0x68, 0x65, 0x61, 
	0x64, 0x65, 0x72, 0x3e, 0xa, 0x3c, 0x48, 0x31, 0x3e, 0x6c, 
	0x77, 0x48, 0x54, 0x54, 0x50, 0x44, 0x3c, 0x2f, 0x48, 0x31, 
	0x3e, 0xa, 0x3c, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0x3e, 0};

const httpd_fsdata httpd_fsdata_file[2]={
{http_content_type_html,data_404_html, data_404_html + 10, sizeof(data_404_html) - 10},
{http_content_type_html,data_index_html, data_index_html + 12, sizeof(data_index_html) - 12},
};
/*-----------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/
int
fs_open(const char *name, struct fs_file *file)
{
	int i;
  /*struct httpd_fsdata_file *f;

  for(f = HTTPD_FS_ROOT;
      f != NULL;
      f = f->next) {
    if (!strcmp_P(name, f->name)) {
      file->data = f->data;
      file->len =  f->len;
      return 1;
    }
  }*/

	for(i=0;i<HTTPD_FS_NUMFILES;i++)
	{
		if (!strcmp_P(name, httpd_fsdata_file[i].name)) {
			file->data =  httpd_fsdata_file[i].data;
			file->len  =  httpd_fsdata_file[i].len;
			file->mime =	httpd_fsdata_file[i].mime;
			return 1;
		}
	}

	return 0;
}
