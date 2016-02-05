/**
 * @file errno.h
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @brief Type of Erros
 *
 * 
 * This file contains the type of errors the the user may input. The error can be from invalid paramter passed to a function, or invalid input format.
 */
 
#define E_NOERROR 0   /* No errors */
#define E_INVPARA 1   /* Invalid parameters had been passed in */
#define E_INVSTRF 2   /* Invalid [Input] string format */

/** @brief  Name: error_t.
 *
 * Description: This file contains the type of errors the the user may input.
 *
 */

typedef unsigned int error_t;
