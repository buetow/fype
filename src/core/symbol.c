/*:*
 *: File: ./src/core/symbol.c
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

#include "symbol.h"

#include "../data/list.h"

Symbol*
symbol_new(SymbolType sym, void *p_val) {
   Symbol *p_symbol = malloc(sizeof(Symbol));

   p_symbol->sym = sym;
   p_symbol->p_val = p_val;
   p_symbol->i_refs = 1;

   return p_symbol;
}

void
symbol_delete(Symbol *p_symbol) {
	if (--p_symbol->i_refs == 0) {
   switch (symbol_get_sym(p_symbol)) {
   case SYM_PROCEDURE:
   {
      List *p_list_token = symbol_get_val(p_symbol);
      list_delete(p_list_token);
   }
   break;
   NO_DEFAULT;
   }
   free(p_symbol);
	}
}

void
symbol_cleanup_hash_syms_cb(void *p_void) {
   symbol_delete(p_void);
}
