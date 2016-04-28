/**
 * @file packing.h
 * @author Thunder Krakens
 * @date April 28th, 2016
 * @brief Packing classes
 * @version R6
 */

#ifdef __GNUC__ 
	#define PACKED(class_to_pack) class_to_pack __attribute__ ((packed))
#else
	#define PACKED(class_to_pack) __pragma(pack(push, 1)) class_to_pack __pragma(pack(pop))
#endif
