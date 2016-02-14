/**
 * @file errno.h
 * @author Thunder Krakens
 * @date February 7nd, 2016
 * @version R2
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

/** @brief Error when freeing memory*/
#define E_FREEMEM 4
/** @brief A NULL Pointer Error */
#define E_NULL_PTR 5
/** @brief Error removing pcb */
#define E_REMOVE_PCB 6
/** @brief Error inserting pcb */
#define E_INSERT_PCB_FAIL 7


/* @brief When this error happen, there is a special case that was not under program's consideration */
#define E_PROGERR 99


/**
 * @name error_t.
 *
 * @brief The datetype that holds the error code.
 *
 */
typedef unsigned int error_t;
