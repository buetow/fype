/*:*
 *: File: ./src/core/scanner.h
 *: A simple interpreter
 *: 
 *: WWW		: http://fype.buetow.org
 *: E-Mail	: fype@dev.buetow.org
 *: 
 *: Copyright (c) 2005 2006 2007 2008, Dipl.-Inf. (FH) Paul C. Buetow 
 *: All rights reserved.
 *: 
 *: Redistribution and use in source and binary forms, with or without modi-
 *: fication, are permitted provided that the following conditions are met:
 *:  * Redistributions of source code must retain the above copyright
 *:    notice, this list of conditions and the following disclaimer.
 *:  * Redistributions in binary form must reproduce the above copyright
 *:    notice, this list of conditions and the following disclaimer in the
 *:    documentation and/or other materials provided with the distribution.
 *:  * Neither the name of P. B. Labs nor the names of its contributors may 
 *:    be used to endorse or promote products derived from this software 
 *:    without specific prior written permission.
 *: 
 *: THIS SOFTWARE IS PROVIDED BY Paul Buetow AS IS'' AND ANY EXPRESS OR 
 *: IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 *: WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *: DISCLAIMED. IN NO EVENT SHALL Paul Buetow BE LIABLE FOR ANY DIRECT, 
 *: INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 *: (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 *:  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 *: HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *: STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 *: IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *: POSSIBILITY OF SUCH DAMAGE.
 *:*/

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <ctype.h>

#include "token.h"
#include "../fype.h"
#include "../data/dat.h"

#define scanner_get_list_token(s) s->p_list_token
#define scanner_get_fp(s) s->fp
#define scanner_get_filename(s) s->c_filename
#define scanner_get_codestring(s) s->c_codestring

typedef struct {
   int i_current_line_nr;
   int i_current_pos_nr;
   int i_num_tokenends2;
   int i_num_tokenends;
   char *c_filename;
   char *c_codestring;
   FILE *fp;
   List *p_list_token;
} Scanner;

Scanner *scanner_new(List *p_list_token, Tupel *p_tupel_argv);
void scanner_post_task(Scanner *p_scanner);
void scanner_delete(Scanner *p_scanner);
void scanner_run(Fype *p_fype);
void scanner_add_token(Scanner *p_scanner, char **cc_token, int *p_token_len,
                       TokenType tt_cur);
TokenType scanner_get_tt_cur(char *c_token);
void scanner_cleanup_list_token_cb(void *p_void);

#endif
