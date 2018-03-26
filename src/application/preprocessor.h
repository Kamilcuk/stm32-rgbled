/*
 * preprocessor.h
 *
 *  Created on: 5 mar 2018
 *      Author: kamil
 */

#ifndef SRC_PREPROCESSOR_H_
#define SRC_PREPROCESSOR_H_

/**
 * Execute preExpressions on entering the statement block,
 * and execute postExpressions when exiting the statement block.
 */
#define BLOCK_prepost(preExpressions, postExpressions) \
	for(int _bLoCk_ToDo = ( pre, 1); _bLoCk_ToDo; _bLoCk_ToDo = (postExpressions, 0))

/**
 * Somehow simulates:
 * if( declaration epxression_opt ; expression )
 * Examples:
 * IF_var(int ret = sleep(1), ret2 = sleep(2), ret3; (ret3 = sleep(3), ret < 0))
 *    printf("sleep(1) returned %x\n", ret);
 */
#define IF(...) \
	for(int _ToDo = 1; _ToDo; _ToDo = 0) \
		for(__VA_ARGS__ && _ToDo; _ToDo = 0 )

#ifdef __GNUC__

#define ON_STACK(x)  (__typeof__(x)){x}

#endif //__GNUC__


#endif /* SRC_PREPROCESSOR_H_ */
