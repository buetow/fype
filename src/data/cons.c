/*:*
 *: File: ./src/data/cons.c
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

#include "cons.h"

Cons*
cons_new(void *p_val) {
   Cons *p_cons = malloc(sizeof(Cons));

   p_cons->p_val = p_val;
   p_cons->p_succ = NULL;

   return (p_cons);
}

void
cons_delete(Cons *p_cons) {
   free(p_cons);
}

void
cons_delete_cb(void *p_cons) {
   cons_delete(p_cons);
}

void
cons_iterate(Cons *p_cons, void (*func)(void *)) {
   if (p_cons != NULL && p_cons->p_val != NULL) {
      (*func) (p_cons->p_val);
      cons_iterate(p_cons->p_succ, func);
   }
}

void*
cons_car(Cons *p_cons) {
   return (p_cons->p_val);
}

Cons*
cons_cdr(Cons *p_cons) {
   return (p_cons->p_succ);
}

Cons*
cons_cons(Cons *p_cons, void *p_val) {
   if (p_cons->p_val == NULL) {
      p_cons->p_val = p_val;
      return (p_cons);

   } else if (p_val == NULL) {
      return (p_cons);
   }

   Cons *p_new = cons_new(p_val);
   p_new->p_succ = p_cons;
   return (p_new);
}
