/*:*
 *: File: ./src/core/token.c
 *: A simple Fype interpreter
 *:
 *: WWW: http://fype.buetow.org
 *: AUTHOR: http://paul.buetow.org
 *: E-Mail: fype at dev.buetow.org
 *:
 *: The Fype Language; (c) 2005 - 2010 - Dipl.-Inform. (FH) Paul C. Buetow
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

#include "token.h"

#define CHECK(...) if (!strcmp(c_token, __VA_ARGS__)) return
#define CASE(t,r) case t: return r;

long TOKEN_ID_COUNTER = 0;

TokenType
get_tt(char *c_token) {
   CHECK("(") TT_PARANT_L;
   CHECK(")") TT_PARANT_R;
   CHECK("'") TT_SQUOTE;
   CHECK(".") TT_DOT;

   return TT_IDENT;
}

char*
tt_get_name(TokenType tt_cur) {
   switch (tt_cur) {
      CASE(TT_PARANT_L,"TT_PARANT_L")
      CASE(TT_PARANT_R,"TT_PARANT_R")
      CASE(TT_INTEGER,"TT_INTEGER")
      CASE(TT_DOUBLE,"TT_DOUBLE")
      CASE(TT_STRING,"TT_STRING")
      CASE(TT_DOT,"TT_DOT")
      CASE(TT_SQUOTE,"TT_SQUOTE")
      CASE(TT_IDENT,"TT_IDENT")
   }

   return "TT_IDENT";
}

Token*
token_new(char *c_val, TokenType tt_cur, int i_line_nr,
          int i_pos_nr, char *c_filename) {
   Token *p_token = token_new_dummy();

   p_token->c_val = c_val;
   p_token->tt_cur = tt_cur;
   p_token->i_line_nr = i_line_nr;
   p_token->i_pos_nr = i_pos_nr;
   p_token->c_filename = c_filename;

   return (p_token);
}

Token*
token_new_dummy() {
   Token *p_token = malloc(sizeof(Token));

   p_token->c_val = NULL;
   p_token->tt_cur = TT_IDENT;
   p_token->i_line_nr = -1;
   p_token->i_pos_nr = -1;
   p_token->c_filename = NULL;
   p_token->u_token_id = TOKEN_ID_COUNTER++;

   return (p_token);
}

void
token_delete_cb(void *p_void) {
   token_delete(p_void);
}

void
token_delete(Token *p_token) {
}

_Bool
token_is(Token *p_token, char *c_str) {
   if (p_token->tt_cur == TT_IDENT && strcmp(p_token->c_val, c_str) == 0)
      return true;

   return false;
}

void
token_print(Token *p_token) {
   printf("(id=%05u, line=%05d, pos=%04d, type=%s, val=%s)",
          p_token->u_token_id,
          p_token->i_line_nr,
          p_token->i_pos_nr,
          tt_get_name(p_token->tt_cur),
          p_token->c_val);
}

void
token_print_ln(Token *p_token) {
   token_print(p_token);
   printf("\n");
}

void
token_print_cb(void *p_void) {
   Token *p_token = p_void;
   printf("Token ");
   token_print(p_token);
   printf("\n");
}
