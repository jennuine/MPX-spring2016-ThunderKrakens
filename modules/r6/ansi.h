
#if defined(__linux__) || defined(linux) || defined(__linux)

// Text
#define T_RESET     "\x1B[0m" //reset all attrs

#define T_BOLD      "\x1B[1m" //bold
#define T_BOLD_OFF  "\x1B[22m" //bold off
#define T_ITCS      "\x1B[7m" //inverted
#define T_ITCS_OFF  "\x1B[27m" //inverted off


// Text Colors
#define T_NRM       "\x1B[39m" //normal/default
#define T_RED       "\x1B[31m" //red
#define T_CYAN      "\x1B[36m" //cyan
#define T_WHT       "\x1B[37m" //white

// Background Colors
#define B_NRM       "\x1B[49m" //normal/default
#define B_CYAN      "\x1b[46m" //cyan


#define T_DIR       T_BOLD T_CYAN //color for directory
#define T_DIR_OFF   T_NRM T_BOLD_OFF

// keys
#define K_LEFT 		"\x1B[D"

#else
	
	// Text
#define T_RESET     "" //reset all attrs

#define T_BOLD      "" //bold
#define T_BOLD_OFF  "" //bold off
#define T_ITCS      "" //italics
#define T_ITCS_OFF  "" //italics off


	// Text Colors
#define T_NRM       "" //normal/default
#define T_RED       "" //red
#define T_CYAN      "" //cyan
#define T_WHT       "" //white

	// Background Colors
#define B_NRM       "" //normal/default
#define B_CYAN      "" //cyan



#define T_DIR       "" //color for directory
#define T_DIR_OFF   ""

#endif