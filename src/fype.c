/*:*
 *: File: ./src/fype.c
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

#include "fype.h"

#include "argv.h"
#include "core/scanner.h"
#include "core/token.h"
#include "core/interpret.h"

PBSc*
fype_new() {
   PBSc *p_fype = malloc(sizeof(PBSc));

   p_fype->p_hash_syms = hash_new(512);
   p_fype->p_list_token = list_new();
   p_fype->p_tupel_argv = tupel_new();
   p_fype->c_basename = NULL;

   return (p_fype);
}

void
fype_delete(PBSc *p_fype) {
   argv_tupel_delete(p_fype->p_tupel_argv);

   hash_delete(p_fype->p_hash_syms);

   list_iterate(p_fype->p_list_token, token_delete_cb);
   list_delete(p_fype->p_list_token);

   if (p_fype->c_basename)
      free(p_fype->c_basename);
}

int
fype_run(int i_argc, char **pc_argv) {
   PBSc *p_fype = fype_new();

   // argv: Maintains command line options
   argv_run(p_fype, i_argc, pc_argv);

   // scanner: Fills the list of tokens
   scanner_run(p_fype);

   // interpret: Interprets the list of tokens
   interpret_run(p_fype);

   fype_delete(p_fype);

   return (0);
}
