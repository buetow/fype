/*:*
 *: File: ./src/core/scanner.c
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

#include "scanner.h"

#include <ctype.h>
#include <string.h>

const char _TOKEN_ENDS[] = "()";
int _CODESTR_INDEX = 0;

Scanner*
scanner_new(List *p_list_token, Tupel *p_tupel_argv) {
   Scanner *p_scanner = malloc(sizeof(Scanner));

   Dat *p_dat_string = p_tupel_argv->a;

   if (dat_empty(p_dat_string))
      ERROR("No source given");

   if (argv_checkopts("e", p_tupel_argv)) {
      p_scanner->c_codestring = dat_pop(p_dat_string);
      p_scanner->c_filename = NULL;
      p_scanner->fp = NULL;

   } else {
      p_scanner->c_codestring = NULL;
      p_scanner->c_filename = dat_pop(p_dat_string);
      p_scanner->fp = fopen(p_scanner->c_filename, "r");

      if (!p_scanner->fp)
         ERROR("Could not open '%s' for reading!", p_scanner->c_filename);
   }

   p_scanner->p_list_token = p_list_token;

   p_scanner->i_current_line_nr = 1;
   p_scanner->i_current_pos_nr = 0;

   p_scanner->i_num_tokenends = strlen(_TOKEN_ENDS);
   p_scanner->tt_last = TT_IDENT;

   return p_scanner;
}

void
scanner_delete(Scanner *p_scanner) {
   if (p_scanner->fp)
      fclose(p_scanner->fp);
   free(p_scanner);
}

_Bool
_scanner_has_next_char(Scanner *p_scanner) {
   if (p_scanner->fp)
      return !feof(p_scanner->fp);

   return p_scanner->c_codestring[_CODESTR_INDEX] != 0;
}

char
_scanner_get_next_char(Scanner *p_scanner) {
   if (p_scanner->fp)
      return fgetc(p_scanner->fp);

   return (p_scanner->c_codestring[_CODESTR_INDEX++]);
}

void
scanner_run(PBSc *p_fype) {
   Scanner *p_scanner = scanner_new(p_fype->p_list_token,
                                    p_fype->p_tupel_argv);

   int i_token_len = 0;
   char *c_token = malloc(sizeof(char));

   c_token[0] = 0;

   while ( _scanner_has_next_char(p_scanner) ) {
      char c = _scanner_get_next_char(p_scanner);
      ++p_scanner->i_current_pos_nr;

      switch (c) {
      case '#':
      {
         c = _scanner_get_next_char(p_scanner);
         ++p_scanner->i_current_pos_nr;
         _Bool b_multi_comment = c == '*';

         do {
            c = _scanner_get_next_char(p_scanner);
            ++p_scanner->i_current_pos_nr;
            if (c == '\n') {
               ++p_scanner->i_current_line_nr;
               p_scanner->i_current_pos_nr = 0;

               if (!b_multi_comment)
                  break;

            } else if (b_multi_comment && c == '*' &&
                       _scanner_has_next_char(p_scanner)) {
               if ( (c = _scanner_get_next_char(p_scanner)) == '#')
                  break;

               else if (c == '\n')
                  ++p_scanner->i_current_line_nr;
            }

         } while ( _scanner_has_next_char(p_scanner) );
      }

      break;

      case '"':
         if (i_token_len) {
            TokenType tt_cur = scanner_get_tt_cur(c_token);
            scanner_add_token(p_scanner, &c_token, &i_token_len, tt_cur);
         }
         {
            int i_num_nl = 0;
            do {
               c = _scanner_get_next_char(p_scanner);
               if ( c == '\n' ) {
                  ++i_num_nl;
                  p_scanner->i_current_pos_nr = 0;

                  i_token_len += 2;
                  c_token = realloc(c_token, sizeof(char) * (i_token_len + 1));
                  c_token[i_token_len-2] = '\\';
                  c_token[i_token_len-1] = 'n';
                  c_token[i_token_len] = 0;

               } else if (c == '"') {
                  if (i_token_len && c_token[i_token_len-1] == '\\') {
                     c_token[i_token_len-1] = '"';

                  } else {
                     break;
                  }

               } else {
                  ++i_token_len;
                  c_token = realloc(c_token, sizeof(char) * (i_token_len + 1));
                  c_token[i_token_len-1] = c;
                  c_token[i_token_len] = 0;
               }

            } while ( _scanner_has_next_char(p_scanner) );

            scanner_add_token(p_scanner, &c_token, &i_token_len, TT_STRING);

            if (i_num_nl)
               p_scanner->i_current_line_nr += i_num_nl;
         }

         break;

      case '\n':
      case '\t':
      case ' ':
         if (i_token_len) {
            TokenType tt_cur = scanner_get_tt_cur(c_token);
            scanner_add_token(p_scanner, &c_token, &i_token_len, tt_cur);
         }

         if (c == '\n') {
            ++p_scanner->i_current_line_nr;
            p_scanner->i_current_pos_nr = 0;
         }

         break;

      case '(':
      case '.':
      {
         if (i_token_len) {
            TokenType tt_cur = scanner_get_tt_cur(c_token);
            scanner_add_token(p_scanner, &c_token, &i_token_len, tt_cur);
         }
         ++i_token_len;
         c_token = realloc(c_token, sizeof(char) * i_token_len + 1);
         c_token[i_token_len-1] = c;
         c_token[i_token_len] = 0;
         TokenType tt_cur = scanner_get_tt_cur(c_token);
         scanner_add_token(p_scanner, &c_token, &i_token_len, tt_cur);
      }
      break;

      default:
         if (i_token_len) {
            TokenType tt_cur = scanner_get_tt_cur(c_token);

            for (int i = 0; i < p_scanner->i_num_tokenends; ++i) {
               if (_TOKEN_ENDS[i] == c) {
                  scanner_add_token(p_scanner, &c_token,
                                    &i_token_len, tt_cur);
                  break;
               }
            }
         }

         ++i_token_len;
         c_token = realloc(c_token, sizeof(char) * i_token_len + 1);
         c_token[i_token_len-1] = c;
         c_token[i_token_len] = 0;
      }
   }

   if (argv_checkopts("e", p_fype->p_tupel_argv) && i_token_len) {
      TokenType tt_cur = scanner_get_tt_cur(c_token);
      scanner_add_token(p_scanner, &c_token, &i_token_len, tt_cur);
   }

   char *c_filename = scanner_get_filename(p_scanner);
   scanner_delete(p_scanner);

   if (argv_checkopts("TV", p_fype->p_tupel_argv))
      list_iterate(p_fype->p_list_token, token_print_cb);

   char *c_basename = NULL;
   if (c_filename) {
      int i_len = strlen(c_filename) - 3;
      c_basename = calloc(i_len+1, sizeof(char));
      strncpy(c_basename, c_filename, i_len);
      c_basename[i_len] = 0;

   } else {
      char *c_basename = calloc(1, sizeof(char));
      c_basename[0] = 0;
   }

   p_fype->c_basename = c_basename;
}

void
scanner_add_token(Scanner *p_scanner, char **cc_token, int *p_token_len,
                  TokenType tt_cur) {

   List *p_list_token = scanner_get_list_token(p_scanner);
   Token *p_token = token_new(*cc_token, tt_cur,
                              p_scanner->i_current_line_nr,
                              p_scanner->i_current_pos_nr,
                              p_scanner->c_filename);

   list_add_back(p_list_token, p_token);

   *cc_token = malloc(sizeof(char));
   (*cc_token)[0] = 0;
   *p_token_len = 0;

   p_scanner->tt_last = tt_cur;
}

TokenType
scanner_get_tt_cur(char *c_token) {
   if (isdigit(c_token[0]))
      return TT_INTEGER;

   return (get_tt(c_token));
}

void
scanner_cleanup_list_token_cb(void *p_void) {
   Token *p_token = p_void;
   token_delete(p_token);
}
