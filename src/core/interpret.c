/*:*
 *: File: ./src/core/interpret.c
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

#include "interpret.h"
#include "promise.h"
#include "variable.h"
#include "tools.h"

void _eval(Interpret *p_inter);

Interpret*
interpret_new(List *p_list_token) {
   Interpret *p_inter = malloc(sizeof(Interpret));

   p_inter->p_frame = frame_new(NULL);

   p_inter->p_list_token = p_list_token;
   p_inter->p_token = NULL;

   p_inter->i_pcount = 0;

   p_inter->b_is_lambda_interpretation = false;
   p_inter->p_lambda = NULL;

   return (p_inter);
}

Interpret*
interpret_new_lambda(Interpret *p_inter, Lambda *p_lambda) {
   Interpret *p_inter_up = malloc(sizeof(Interpret));
   p_inter_up->p_frame = frame_new(p_lambda->p_frame);
   p_inter_up->p_list_token = NULL;
   p_inter_up->p_token = NULL;
   p_inter_up->i_pcount = 0;

   p_inter_up->b_is_lambda_interpretation = true;
   p_inter_up->p_lambda = p_lambda;

   return (p_inter_up);
}

void
interpret_delete(Interpret *p_inter) {
   frame_delete(p_inter->p_frame);
   free(p_inter);
}

void
interpret_run(PBSc *p_fype) {
   Interpret *p_inter =
      interpret_new(p_fype->p_list_token);

   _eval(p_inter);

   interpret_delete(p_inter);
}

void
_def(Interpret *p_inter, Token *p_token, ListIterator *p_iter) {
   Frame *p_frame = p_inter->p_frame;
   _Bool b_success;

   if (!listiterator_has_next(p_iter))
      ERROR_EOB;

   p_token = listiterator_next(p_iter);
   char *c_name;

   if (p_token->tt_cur == TT_IDENT) {
      c_name = p_token->c_val;

      ListElem *p_listelem = listiterator_current_elem(p_iter);

      if (!listiterator_has_next(p_iter))
         ERROR_EOB;

      p_token = listiterator_next(p_iter);

      switch (p_token->tt_cur) {
      case TT_PARANT_L: {
         tool_skip_block(p_iter, 0);
         ListElem *p_listelem_end = listiterator_current_elem(p_iter);
         Lambda *p_lambda = lambda_new(
                               c_name,
                               NULL,
                               p_listelem,
                               p_listelem_end,
                               p_inter->p_frame);
         //printf("::1\n");
         b_success = frame_add_symbol(p_frame, c_name, ST_LAMBDA, p_lambda);
      }
      break;
      case TT_IDENT: {
         Variable *p_variable = variable_new(
                                   c_name,
                                   p_token,
                                   p_inter->p_frame);
         b_success = frame_add_symbol(p_frame,
                                      c_name, ST_VARIABLE, p_variable);
         if (!listiterator_has_next(p_iter))
            ERROR_EOB;
         Token *p_token2 = listiterator_next(p_iter);
         if (p_token2->tt_cur != TT_PARANT_R)
            ERROR_INTERPRET("Expected ) or (", p_token);
      }
      break;
      default:
         ERROR_INTERPRET("Expected ( or identifier", p_token);
      }

   } else if (p_token->tt_cur == TT_PARANT_L) {
      List *p_list_args = list_new();

      if (!listiterator_has_next(p_iter))
         ERROR_INTERPRET("Expected identifier", p_token);

      p_token = listiterator_next(p_iter);
      if (p_token->tt_cur != TT_IDENT)
         ERROR_INTERPRET("Expected identifier", p_token);

      c_name = p_token->c_val;
      while (listiterator_has_next(p_iter)) {
         p_token = listiterator_next(p_iter);

         if (p_token->tt_cur != TT_IDENT) {
            if (p_token->tt_cur != TT_PARANT_R)
               ERROR_INTERPRET("Expected identifier or )", p_token);
            break;

         } else {
            list_add_back(p_list_args, p_token->c_val);
         }
      }

      ListElem *p_listelem = listiterator_next_elem(p_iter);
      //printf("::2\n");
      tool_skip_block(p_iter, 2);
      ListElem *p_listelem_end = listiterator_current_elem(p_iter);

      Lambda *p_lambda = lambda_new(
                            c_name,
                            p_list_args,
                            p_listelem,
                            p_listelem_end,
                            p_inter->p_frame);
      b_success = frame_add_symbol(p_frame, c_name, ST_LAMBDA, p_lambda);
   } else {
      ERROR_INTERPRET("Expected identifier or (", p_token);
   }

   if (!b_success)
      ERROR("Forbidden to redef symbol \"%s\" @ current frame", c_name);
}

void
_say(Interpret *p_inter, Token *p_token, ListIterator *p_iter) {
   while (listiterator_has_next(p_iter)) {
      Token *p_token = listiterator_next(p_iter);
      switch (p_token->tt_cur) {
      case TT_IDENT:
      case TT_INTEGER:
      case TT_STRING:
         printf("%s\n", p_token->c_val);
         break;
      case TT_PARANT_L:
         ERROR("Not yet implemented");
         break;
      case TT_PARANT_R:
         return;
         NO_DEFAULT;
      }
   }
}

void
_eval_lambda(Interpret *p_inter, Lambda *p_lambda, ListIterator *p_iter) {
   Interpret *p_inter_local = interpret_new_lambda(p_inter, p_lambda);
   Frame *p_frame_local = p_inter_local->p_frame;
   ListIterator *p_iter_args = NULL;


   if (p_lambda->p_list_args)
      p_iter_args = listiterator_new(p_lambda->p_list_args);

   Token *p_token = listiterator_current(p_iter);

   if (p_iter_args) {
      while (listiterator_has_next(p_iter_args)) {
         char *c_name = listiterator_next(p_iter_args);

         if (!listiterator_has_next(p_iter))
            ERROR_EOB;
         ListElem *p_listelem = listiterator_current_elem(p_iter);
         p_token = listiterator_next(p_iter);

         switch (p_token->tt_cur) {
         case TT_PARANT_L: {
            //printf("::3\n");
            tool_skip_block(p_iter, 1 );
            ListElem *p_listelem_end = listiterator_current_elem(p_iter);
            Lambda *p_lambda_ = lambda_new(
                                   c_name,
                                   NULL,
                                   p_listelem,
                                   p_listelem_end,
                                   p_frame_local);
            if (!frame_add_symbol(p_frame_local,
                                  c_name,
                                  ST_LAMBDA,
                                  p_lambda_)) {
               printf("Illegal reuse of parameter '%s' @ function '%s'",
                      c_name, p_lambda->c_name);
               ERROR_INTERPRET(". Error binding local lambda", p_token);
            }
         }
         break;
         case TT_PARANT_R:
            ERROR_INTERPRET("Didn't expect ) here", p_token);
            break;
         default: {
            Variable *p_variable = variable_new(c_name,
                                                p_token,
                                                p_frame_local);
            if (!frame_add_symbol(p_frame_local,
                                  c_name,
                                  ST_VARIABLE,
                                  p_variable)) {
               printf("Illegal reuse of parameter '%s' @ function '%s'\n",
                      c_name, p_lambda->c_name);
               frame_print(p_frame_local);
               ERROR_INTERPRET("Fatal", p_token);
            }
         }
         break;
         }
      }

      listiterator_delete(p_iter_args);
   }

   if (!listiterator_has_next(p_iter))
      ERROR_EOB;

   _eval(p_inter_local);
   interpret_delete(p_inter_local);
}

void
_eval_symbol(Interpret *p_inter, Symbol *p_symbol, ListIterator *p_iter) {
   switch (p_symbol->st) {
   case ST_LAMBDA: {
      _eval_lambda(p_inter, p_symbol->p_val, p_iter);
      //printf("::EVAL_LAMBDA_END\n");
   }
   break;
   case ST_VARIABLE:
      break;
   }
}

Token*
_parant(Interpret *p_inter, Token *p_token) {
   //printf("::PARANT<%d>: %s\n", p_inter->i_pcount, p_token->c_val);
   if (p_token->tt_cur == TT_PARANT_L) {
      ++p_inter->i_pcount;

   } else if (p_token->tt_cur == TT_PARANT_R) {
      if (--p_inter->i_pcount == 0)
         return (NULL);

      else if (p_inter->i_pcount < 0)
         ERROR_INTERPRET("Too many closing )'s",
                         p_token);
   }

   return (p_token);
}

void
_run_func(Interpret *p_inter, Token *p_token, ListIterator *p_iter) {
   Frame *p_frame = p_inter->p_frame;
   Symbol *p_symbol = NULL;

   if (listiterator_has_next(p_iter)) {
      if (! (p_token = _parant(p_inter, listiterator_next(p_iter))) )
         return;

      //printf("::Interpreting: %s\n", p_token->c_val);

      if (token_is(p_token, "def")) {

         _def(p_inter, p_token, p_iter);

      } else if (token_is(p_token, "say")) {
         _say(p_inter, p_token, p_iter);

      } else if (token_is(p_token, "noop")) {

      } else if ( (p_symbol = frame_get_symbol(p_frame, p_token->c_val)) != NULL ) {
         _eval_symbol(p_inter, p_symbol, p_iter);

      } else if (token_is(p_token, "show-frames")) {
         frame_print(p_inter->p_frame);

      } else if (token_is(p_token, "beep")) {
         printf("BEEP\n");

      } else {
         printf("No symbol '%s' defined @ any frame:\n", p_token->c_val);
         frame_print(p_frame);
         ERROR_INTERPRET("Error.", p_token);
      }
   }
}

void
_eval(Interpret *p_inter) {
   ListIterator *p_iter = NULL;
   ListElem *p_listelem_end = NULL;

   if (!p_inter->b_is_lambda_interpretation) {
      p_iter = listiterator_new(p_inter->p_list_token);
   } else {
      p_iter = listiterator_new_from_elem(p_inter->p_lambda->p_listelem);
      p_listelem_end = p_inter->p_lambda->p_listelem_end;
   }

   while (listiterator_has_next(p_iter)) {
      Token *p_token = _parant(p_inter, listiterator_next(p_iter));
      if (!p_token)
         break;

      if (p_listelem_end && listiterator_current_elem_equals(
               p_iter,
               p_listelem_end))
         break;

      switch (p_token->tt_cur) {
      case TT_PARANT_L:
         _run_func(p_inter, p_token, p_iter);
         break;
      case TT_PARANT_R:
         break;
         NO_DEFAULT;
      }
   }
   listiterator_delete(p_iter);
}
