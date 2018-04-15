/*
 * cook.h
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */
#ifndef SRC_APPLICATION_TOKENIZE_INTERNAL_H_
#define SRC_APPLICATION_TOKENIZE_INTERNAL_H_

#include "ag-char-map.h"
#include "ao_string_tokenize.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define NL  '\n'
#define NUL  0
typedef unsigned int uint_t;

extern unsigned int
ao_string_cook_escape_char(char const * pzIn, char * pRes, uint_t nl);

#endif /* SRC_APPLICATION_TOKENIZE_INTERNAL_H_ */
