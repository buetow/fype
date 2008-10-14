/*:*
 *: File: ./src/core/garbage.c
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

#include "garbage.h"

#define _GARBAGE_ERROR(m) \
	 ERROR("%s: Garbage error", m);

List *LIST_GARBAGE = NULL;

typedef struct {
   void (*p_func)(void*);
   int *p_ref_count;
   void *p_2free;
   GarbageType type;
} _Garbage;

void
garbage_init() {
   LIST_GARBAGE = list_new();
}

void
_garbage_print(_Garbage *p_garbage) {
   switch (p_garbage->type) {
   case GC_TOKEN:
   {
      Token *p_token = p_garbage->p_2free;
      token_print(p_token);
      printf("\n");
   }
   break;
   }
}

void
_garbage_free(_Garbage *p_garbage) {
   (*p_garbage->p_func) (p_garbage->p_2free);
   free(p_garbage);
}

void
garbage_destroy() {
   garbage_collect();

   if (!list_empty(LIST_GARBAGE)) {
      EPRINTF("The garbage collector still has %d registered items which don't have"
              " a zero ref count!\n", list_size(LIST_GARBAGE));

      ListIterator *p_iter = listiterator_new(LIST_GARBAGE);
      while (listiterator_has_next(p_iter)) {
         _Garbage *p_garbage = listiterator_next(p_iter);
         _garbage_print(p_garbage);
      }
      listiterator_delete(p_iter);

      _GARBAGE_ERROR("Garbage left");
   }

   list_delete(LIST_GARBAGE);
}

int
garbage_collect() {
   //printf("GARBAGE_COLLECT\n");
   ListIterator *p_iter = listiterator_new(LIST_GARBAGE);
   List *p_list_garbage_new = list_new();
   int i_count = 0;
   //printf("size %d\n", list_size(LIST_GARBAGE));

   while (listiterator_has_next(p_iter)) {
      _Garbage *p_garbage = listiterator_next(p_iter);

      if (p_garbage->p_ref_count == NULL || *p_garbage->p_ref_count <= 0) {
         //_garbage_print(p_garbage);
         _garbage_free(p_garbage);
         ++i_count;

      } else {
         list_add_back(p_list_garbage_new, p_garbage);
      }
   }

   listiterator_delete(p_iter);
   list_delete(LIST_GARBAGE);
   LIST_GARBAGE = p_list_garbage_new;
   //printf("GARBAGE_COLLECT_END\n");

   return (i_count);
}

void
garbage_add(void *p, GarbageType type) {
   garbage_add2(p, free, NULL, type);
}

void
garbage_add2(void *p, void (*p_func)(void*), int *p_ref_count, GarbageType type) {
   _Garbage *p_garbage = malloc(sizeof(_Garbage));

   p_garbage->p_2free = p;
   p_garbage->p_func = p_func;
   p_garbage->p_ref_count = p_ref_count;
   p_garbage->type = type;

   list_add_back(LIST_GARBAGE, p_garbage);
}

void
garbage_add_token(Token *p_token) {
   garbage_add2(p_token, token_delete_cb, &p_token->i_ref_count, GC_TOKEN);
}
