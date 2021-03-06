/**
 * @file errno.h
 * @author Thunder Krakens
 * @date February 7nd, 2016
 * @version R2
 *
 * @brief This file contains the type of errors. 
 * 
 * The error can be from invalid paramter passed to a function, or invalid input format.
 */

/* @brief No errors */
#define E_NOERROR 0
/* @brief Invalid parameters had been passed in */
#define E_INVPARA 1
/* @brief Invalid [Input] string format */
#define E_INVSTRF 2
/* @brief Invalid User Input */
#define E_INVUSRI 3
/** @brief Error we cannot actually free the memory space
  *  since the student_free had not been implemented before R5
  */
#define E_FREEMEM 4


/** @brief A NULL Pointer Error */
#define E_NULL_PTR 5

/** @brief The pcb queue is empty */
#define E_EMPTPCB 6

#define E_PCB_SYS 7

/** @brief The file was not found */
#define E_FILE_NF 8

/** @brief The specific file name is already exsit. */
#define E_NAMEDUP 9

/** @brief The specific file name contains invalid character. */
#define E_NAMEINV 10

/** @brief Not enough space to store the file. */
#define E_NOSPACE 11

/** @brief The file's attributes are invalid */
#define E_INVATTRS 12

/** @brief The specific directory is full. */
#define E_FOLDFUL 13

/* @brief When this error happen, there is a special case that was not under program's consideration */
#define E_PROGERR 99

/**
 * @name error_t.
 *
 * @brief The datetype that holds the error code.
 *
 */
typedef unsigned int error_t;
