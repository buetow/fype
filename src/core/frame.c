/*:*
 *: File: ./src/core/frame.c
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

#include "frame.h"
#include "lambda.h"
#include "variable.h"

unsigned _I_FRAME_ID_COUNT = 0;

Symbol*
symbol_new(SymbolType st, void *p_val) {
   Symbol *p_symbol = malloc(sizeof(Symbol));

   p_symbol->st = st;
   p_symbol->p_val = p_val;

   return (p_symbol);
}

void
symbol_delete(Symbol *p_symbol) {
   switch (p_symbol->st) {
   case ST_LAMBDA:
      lambda_delete(p_symbol->p_val);
      break;
   case ST_VARIABLE:
      variable_delete(p_symbol->p_val);
      break;
   }

   free(p_symbol);
}

void
symbol_delete_cb(void *p_symbol) {
   symbol_delete(p_symbol);
}

char*
symbol_get_type_name(Symbol *p_symbol) {
   switch (p_symbol->st) {
   case ST_LAMBDA:
      return ("ST_LAMBDA");
   case ST_VARIABLE:
      return ("ST_VARIABLE");
   }

   return ("ST_UNKNOWN");
}

Frame*
frame_new(Frame *p_parent_frame) {
   Frame *p_frame = malloc(sizeof(Frame));

   p_frame->p_parent_frame = p_parent_frame;
   p_frame->p_hash_symbols = hash_new(8);
   p_frame->i_frame_id = _I_FRAME_ID_COUNT++;

   return (p_frame);
}

void
frame_delete(Frame *p_frame) {
   Hash *p_hash_symbols = p_frame->p_hash_symbols;

   hash_iterate(p_hash_symbols, symbol_delete_cb);
   hash_delete(p_hash_symbols);
}

_Bool
frame_add_symbol(Frame *p_frame, char *c_name, SymbolType st, void *p_val) {
   Hash *p_hash_symbols = p_frame->p_hash_symbols;

   if (hash_key_exists(p_hash_symbols, c_name))
      return (false);


   Symbol *p_symbol = symbol_new(st, p_val);
   hash_insert(p_hash_symbols, c_name, p_symbol);

   return (true);
}

Symbol*
frame_get_symbol(Frame *p_frame, char *c_name) {
   void *p_val = hash_get(p_frame->p_hash_symbols, c_name);

   if (!p_val && p_frame->p_parent_frame)
      return (frame_get_symbol(p_frame->p_parent_frame, c_name));

   return (p_val);
}

void
_symbol_print_cb(void *p_val, char *c_name) {
   Symbol *p_symbol = p_val;
   switch (p_symbol->st) {
   case ST_LAMBDA:
      lambda_print(p_symbol->p_val);
      break;
   case ST_VARIABLE:
      variable_print(p_symbol->p_val);
      break;
   }
}

void
_frame_print(Frame *p_frame, int i_frame_nr) {
   printf("FRAME(id=%u) %d:\n", p_frame->i_frame_id, i_frame_nr);

   Hash *p_hash_symbols = p_frame->p_hash_symbols;
   hash_iterate_key(p_hash_symbols, _symbol_print_cb);

   if (p_frame->p_parent_frame)
      _frame_print(p_frame->p_parent_frame, i_frame_nr + 1);
}

void
frame_print(Frame *p_frame) {
   _frame_print(p_frame, 0);
}
