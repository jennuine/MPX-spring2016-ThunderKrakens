/**
 * @file errno.h
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @version R1
 *
 * @brief This file contains the type of errors. The error can be from invalid paramter passed to a function, or invalid input format.
 */

/* @brief No errors */
#define E_NOERROR 0
/* @brief Invalid parameters had been passed in */
#define E_INVPARA 1
/* @brief Invalid [Input] string format */
#define E_INVSTRF 2
/* @brief Invalid User Input */
#define E_INVUSRI 3
/* @brief PCB was removed */
#define E_REMOVE_PCB
/* @brief PCB was failed to be removed */
#define E_REMOVE_PCB_FAIL
/* @brief PCB was inserted */
#define E_INSERT_PCB
/* @brief PCB was failed to be inserted */
#define E_INSERT_PCB_FAIL
/** @brief Error item was not found */
#define E_NOT_FOUND 5


/**
 * @name error_t.
 *
 * @brief The datetype that holds the error code.
 *
 */
typedef unsigned int error_t;
