/*:*
 *: File: ./src/core/lambda.c
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

#include "lambda.h"
#include "token.h"

Lambda*
lambda_new(char *c_name, List *p_list_args, ListElem *p_listelem, ListElem *p_listelem_end, Frame *p_frame) {
   Lambda *p_lambda = malloc(sizeof(Lambda));

   p_lambda->c_name = c_name;
   p_lambda->p_list_args = p_list_args;
   p_lambda->p_listelem = p_listelem;
   p_lambda->p_listelem_end = p_listelem_end;;
   p_lambda->p_frame = p_frame;

   return (p_lambda);
}

void
lambda_delete(Lambda *p_lambda) {
   if (p_lambda->p_list_args)
      list_delete(p_lambda->p_list_args);
   free(p_lambda);
}

void
lambda_print(Lambda *p_lambda) {
   printf("+ST_LAMBDA(name=%s;args=", p_lambda->c_name);

   if (p_lambda->p_list_args) {
      unsigned i_count = p_lambda->p_list_args->i_size;
      ListIterator *p_iter = listiterator_new(p_lambda->p_list_args);

      while (listiterator_has_next(p_iter)) {
         char *c_name = listiterator_next(p_iter);
         if (--i_count == 0)
            printf("%s", c_name);
         else
            printf("%s ", c_name);
      }

      listiterator_delete(p_iter);
   }
   printf(")\n( ");

   ListIterator *p_iter = listiterator_new_from_elem(p_lambda->p_listelem);
   Token *p_token = listiterator_current(p_iter);
   ListElem *p_listelem_end = p_lambda->p_listelem_end;


   while (listiterator_has_next(p_iter)) {
      p_token = listiterator_next(p_iter);
      printf("%s ", p_token->c_val);

      if (listiterator_current_elem_equals(p_iter, p_listelem_end))
         goto LAMBDA_PRINT_END;
   }

   ERROR_EOB;

LAMBDA_PRINT_END:

   printf("\n");
   listiterator_delete(p_iter);
}

