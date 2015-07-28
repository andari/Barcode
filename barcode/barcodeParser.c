#include "barcodeParser.h"

const char NewLineBreakTag[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char StartStopTag[] = { 0x00, 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char SkipTag[] =  { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const barcodeMapping CharacterMap[] = {
	{ 'A', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00} },
	{ 'B', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00} },
	{ 'C', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00} },
	{ 'D', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00} },
	{ 'E', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	 	    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00} },
	{ 'F', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
 		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00} },
	{ 'G', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00, 
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00} },
	{ 'H', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5a,0x00,0x00,0x00,0x00,0x00} },
	{ 'I', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5b,0x00,0x00,0x00,0x00,0x00} },
	{ 'J', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00} },
	{ 'K', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00} },
	{ 'L', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00} },
	{ 'M', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00} },
	{ 'N', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00} },
	{ 'O', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00} },
	{ 'P', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00} },
	{ 'Q', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00} },
	{ 'R', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5a,0x00,0x00,0x00,0x00,0x00} },
	{ 'S', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5b,0x00,0x00,0x00,0x00,0x00} },
	{ 'T', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00, 
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00} },
	{ 'U', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00} },
	{ 'V', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00} },
	{ 'W', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00} },
	{ 'X', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00, 
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00} },
	{ 'Y', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00} },
	{ 'Z', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00} },
	{ '0', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00} },
	{ '1', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00} },
	{ '2', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00} },
	{ '3', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00} },
	{ '4', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00, 
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5a,0x00,0x00,0x00,0x00,0x00} },
	{ '5', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5b,0x00,0x00,0x00,0x00,0x00} },
	{ '6', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00, 
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00} },
	{ '7', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00} },
	{ '8', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00} },
	{ '9', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00} },
	{ '-', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00} },
	{ ':', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00} },
	{ ';', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00} },
	{ '!', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5b,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5b,0x00,0x00,0x00,0x00,0x00} },
	{ '-', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00} },
	{ '<', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00} },
	{ '.', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00} },
	{ '/', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00} },
	{ 'a', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00} },
	{ 'b', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00} },
	{ 'c', 24, {0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00} },
	{ 'd', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00} },
	{ 'e', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00} },
	{ 'f', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5a,0x00,0x00,0x00,0x00,0x00} },
	{ 'g', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5b,0x00,0x00,0x00,0x00,0x00} },
	{ 'h', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00} },
	{ 'i', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00} },
	{ 'j', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00} },
	{ 'k', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00} },
	{ 'l', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00} },
	{ 'm', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00} },
	{ 'n', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00} },
	{ 'o', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00} },
	{ 'p', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5a,0x00,0x00,0x00,0x00,0x00} },
	{ 'q', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5b,0x00,0x00,0x00,0x00,0x00} },
	{ 'r', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5c,0x00,0x00,0x00,0x00,0x00} },
	{ 's', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5d,0x00,0x00,0x00,0x00,0x00} },
	{ 't', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5e,0x00,0x00,0x00,0x00,0x00} },
	{ 'u', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5f,0x00,0x00,0x00,0x00,0x00} },
	{ 'v', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00} },
	{ 'w', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x61,0x00,0x00,0x00,0x00,0x00} },
	{ 'x', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5a,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x62,0x00,0x00,0x00,0x00,0x00} },
	{ 'y', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5a,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00} },
	{ 'z', 24, {0x04,0x00,0x59,0x00,0x00,0x00,0x00,0x00,
		    0x04,0x00,0x5a,0x00,0x00,0x00,0x00,0x00,
	            0x04,0x00,0x5a,0x00,0x00,0x00,0x00,0x00} },

};

barcodeContext initializeBarcodeContext(){
	barcodeContext context;
	bzero(&context, sizeof(context));
	return context;
}

int addInputLineToContext(barcodeContext* context, barcodeInput input){
	if (context == NULL){
		return FAILURE;
	}
	
	if (context->noInputLines < MAX_BARCODE_INPUTS){
		context->inputLines[context->noInputLines] = input;
		//input = context->inputLines[context->noInputLines];
		context->noInputLines++;
		return SUCCESS;
	}
	
	return FAILURE;
}

int validString(char* buffer, size_t size){
	if (buffer != NULL && size == INPUT_LINE_LENGTH){
		return SUCCESS;
	}
	
	return FAILURE;
}

int parseBarcodeContext(barcodeContext context, barcodeOutput* output){
	unsigned int 
		iter = 0, 
		newLine = 0, 
		startStop = 0, 
		bufferSize = 0,
		characterCounter = 0;
	char buffer[MAX_BARCODE_OUTPUT] = {0};
	
	if (output == NULL){
		return FAILURE;
	}
	
	bzero(output, sizeof(barcodeOutput));
	
	for (iter = 0 ; iter < context.noInputLines && startStop < 2; iter++){
        	if (newLine == 1){
			char ch = 0x00;
			if (output->size < MAX_BARCODE_OUTPUT && mapInputToCharacter(buffer, &ch, bufferSize) == SUCCESS){
				output->line[output->size] = ch;
				output->size += 1;

				// DEBUG
				//printf("CHARACTER=%c\n",ch);
			}
            		newLine = 0;
            		bufferSize = 0;
            		bzero(buffer, sizeof(buffer));
        	}

		if (isNewBlockSeparator(context.inputLines[iter]) == SUCCESS){
			newLine = 1;
		}
		else if (isBufferStartEndTag(context.inputLines[iter]) == SUCCESS){
			startStop++;
			//printf("START STOP TAG\n");
		}
		else if (isCharacterTag(context.inputLines[iter]) == SUCCESS){
			if (isSkipTag(context.inputLines[iter]) == FAILURE){
				memcpy(buffer + bufferSize, context.inputLines[iter].line, INPUT_LINE_LENGTH);
				bufferSize += INPUT_LINE_LENGTH;
			}
		}
		/*
		int iterBytes = 0;
		for (iterBytes = 0 ; iterBytes < INPUT_LINE_LENGTH ; iterBytes++){
			printf("0x%x", context.inputLines[iter].line[iterBytes]);
		}
		printf("\n");
		*/
	}
	
	if (startStop == 2){
		return SUCCESS;
	}
	
	return FAILURE;
}

int isNewBlockSeparator(barcodeInput input){
	if (memcmp((void *)input.line, (void *)NewLineBreakTag, INPUT_LINE_LENGTH) == 0){
		return SUCCESS;
	}
	return FAILURE;
}

int isBufferStartEndTag(barcodeInput input){
	if (memcmp((void *)input.line, (void *)StartStopTag, INPUT_LINE_LENGTH) == 0){
		return SUCCESS;
	}
	return FAILURE;
}

int isSkipTag(barcodeInput input){
	if (memcmp((void *)input.line, (void *)SkipTag, INPUT_LINE_LENGTH) == 0){
		return SUCCESS;
	}
	return FAILURE;
}

int isCharacterTag(barcodeInput input){
	if (input.line[0] == 0x04){
		return SUCCESS;
	}
	return FAILURE;
}

int mapInputToCharacter(char* input, char* output, size_t size){
	int found = FAILURE;
	unsigned int iter = 0;

	for (iter = 0 ; iter < sizeof(CharacterMap) / sizeof(barcodeMapping) && found == FAILURE; iter++){
		if (size == CharacterMap[iter].size){
			if (memcmp(CharacterMap[iter].pattern, input, size) == 0){
				found = SUCCESS;
				*output = CharacterMap[iter].character;
			}
		}
	}
	
	if (found == SUCCESS){
		return SUCCESS;
	}
	
	return FAILURE;
}

barcodeInput convertStringToLineInput(char* buffer, size_t size){
	barcodeInput barcodeLine = {{0}};
	
	if (buffer != NULL && size == INPUT_LINE_LENGTH){
		memcpy((void *)&barcodeLine.line, (void *)buffer, INPUT_LINE_LENGTH);
	}
	
	return barcodeLine;
}




