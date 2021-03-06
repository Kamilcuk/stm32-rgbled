/*
 * ao_string_tokenize.h
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */
/*   -*- buffer-read-only: t -*- vi: set ro:
 *
 *  DO NOT EDIT THIS FILE   (options.h)
 *
 *  It has been AutoGen-ed
 *  From the definitions    funcs.def
 *  and the template file   options_h
 *
 *  This file defines all the global structures and special values
 *  used in the automated option processing library.
 *
 *  Automated Options Copyright (C) 1992-2014 by Bruce Korb
 *
 *  This file is part of AutoOpts, a companion to AutoGen.
 *  AutoOpts is free software.
 *  AutoOpts is Copyright (C) 1992-2014 by Bruce Korb - all rights reserved
 *
 *  AutoOpts is available under any one of two licenses.  The license
 *  in use must be one of these two and the choice is under the control
 *  of the user of the license.
 *
 *   The GNU Lesser General Public License, version 3 or later
 *      See the files "COPYING.lgplv3" and "COPYING.gplv3"
 *
 *   The Modified Berkeley Software Distribution License
 *      See the file "COPYING.mbsd"
 *
 *  These files have the following sha256 sums:
 *
 *  8584710e9b04216a394078dc156b781d0b47e1729104d666658aecef8ee32e95  COPYING.gplv3
 *  4379e7444a0e2ce2b12dd6f5a52a27a4d02d39d247901d3285c88cf0d37f477b  COPYING.lgplv3
 *  13aa749a5b0a454917a944ed8fffc530b784f5ead522b1aacaf4ec8aa55a6239  COPYING.mbsd
 */
#ifndef SRC_APPLICATION_AO_STRING_TOKENIZE_H_
#define SRC_APPLICATION_AO_STRING_TOKENIZE_H_

/**
 *  "token list" structure returned by "string_tokenize()"
 */
typedef struct {
    unsigned long   tkn_ct;      ///< number of tokens found
    unsigned char*  tkn_list[1]; ///< array of pointers to tokens
} token_list_t;

/**
 * ao_string_tokenize - tokenize an input string
 *
 *  This function will convert one input string into a list of strings.
 *  The list of strings is derived by separating the input based on
 *  white space separation.  However, if the input contains either single
 *  or double quote characters, then the text after that character up to
 *  a matching quote will become the string in the list.
 *
 *  The returned pointer should be deallocated with @code{free(3C)} when
 *  are done using the data.  The data are placed in a single block of
 *  allocated memory.  Do not deallocate individual token/strings.
 *
 *  The structure pointed to will contain at least these two fields:
 *  @table @samp
 *  @item tkn_ct
 *  The number of tokens found in the input string.
 *  @item tok_list
 *  An array of @code{tkn_ct + 1} pointers to substring tokens, with
 *  the last pointer set to NULL.
 *  @end table
 *
 *  There are two types of quoted strings: single quoted (@code{'}) and
 *  double quoted (@code{"}).  Singly quoted strings are fairly raw in that
 *  escape characters (@code{\\}) are simply another character, except when
 *  preceding the following characters:
 *  @example
 *  @code{\\}  double backslashes reduce to one
 *  @code{'}   incorporates the single quote into the string
 *  @code{\n}  suppresses both the backslash and newline character
 *  @end example
 *
 *  Double quote strings are formed according to the rules of string
 *  constants in ANSI-C programs.
 *
 * @param string       string to be tokenized
 *
 * @return token_list_t* - pointer to a structure that lists each token
 */
extern token_list_t* ao_string_tokenize(char const*);

#endif /* SRC_APPLICATION_AO_STRING_TOKENIZE_H_ */
