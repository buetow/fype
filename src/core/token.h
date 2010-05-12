/*:*
 *: File: ./src/core/token.h
 *: A simple Fype interpreter
 *: 
 *: WWW: http://fype.buetow.org
 *: AUTHOR: http://paul.buetow.org
 *: E-Mail: fype at dev.buetow.org
 *: 
 *: The Fype Language; (c) 2005 - 2010 Paul Buetow 
 *: 
 *: Redistribution and use in source and binary forms, with or without modi-
 *: fication, are permitted provided that the following conditions are met:
 *:  * Redistributions of source code must retain the above copyright
 *:    notice, this list of conditions and the following disclaimer.
 *:  * Redistributions in binary form must reproduce the above copyright
 *:    notice, this list of conditions and the following disclaimer in the
 *:    documentation and/or other materials provided with the distribution.
 *:  * Neither the name of buetow.org nor the names of its contributors may 
 *:    be used to endorse or promote products derived from this software 
 *:    without specific prior written permission.
 *: 
 *: THIS SOFTWARE IS PROVIDED BY PAUL C. BUETOW AS IS'' AND ANY EXPRESS OR 
 *: IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 *: WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *: DISCLAIMED. IN NO EVENT SHALL PAUL C. BUETOW BE LIABLE FOR ANY DIRECT, 
 *: INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 *: (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 *:  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 *: HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *: STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 *: IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *: POSSIBILITY OF SUCH DAMAGE.
 *:*/

#ifndef TOKEN_H
#define TOKEN_H

#include "../defines.h"
#include "../data/array.h"

typedef enum {
   TT_IDENT,
   TT_PARANT_L,
   TT_PARANT_R,
   TT_INTEGER,
   TT_DOUBLE,
   TT_STRING,
   TT_SQUOTE,
   TT_DOT
} TokenType;

typedef struct {
   TokenType tt_cur;
   char *c_val;
   int i_line_nr;
   int i_pos_nr;
   char *c_filename;
   unsigned int u_token_id;
} Token;

Token* token_new(char *c_val, TokenType tt_cur, int i_line_nr,
                 int i_pos_nr, char *c_filename);
Token* token_new_dummy();
void token_delete(Token *p_token);
void token_delete_cb(void *p_token);
void* token_copy_cb(void *p_token);
char* tt_get_name(TokenType tt_cur);
void token_print_cb(void *p_void);
void token_print(Token *p_token);
void token_print_ln(Token *p_token);
void token_print_val(Token *p_token);
_Bool token_is(Token *p_token, char *c_str);
TokenType get_tt(char *c_token);

#endif
