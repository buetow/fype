/*:*
 *: File: ./src/core/frame.h
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

#ifndef FRAME_H
#define FRAME_H

#include "../defines.h"
#include "../data/hash.h"

typedef enum {
   ST_LAMBDA,
   ST_VARIABLE,
} SymbolType;

typedef struct {
   SymbolType st;
   void *p_val;
} Symbol;

typedef struct _Frame {
   struct _Frame *p_parent_frame;
   Hash *p_hash_symbols;
   unsigned i_frame_id;
} Frame;

Symbol* symbol_new(SymbolType st, void *p_val);
void symbol_delete(Symbol *p_symbol);
void symbol_delete_cb(void *p_symbol);
char* symbol_get_type_name(Symbol *p_symbol);

Frame* frame_new(Frame *p_parent_frame);
void frame_delete(Frame *p_frame);
_Bool frame_add_symbol(Frame *p_frame, char *c_name, SymbolType st, void *p_val);
Symbol *frame_get_symbol(Frame *p_frame, char *c_name);
void frame_print(Frame *p_frame);

#endif
