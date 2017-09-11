/************************************
Project Title :	Image Steganography using LSB mechanism

Description	:
The art and science of hiding information by embedding messages withi other, seemingly harmless messages. Bits of unused data are replaced by bits of valuable information using LSB mechanism. Sender and reciever will have individual key / secret based on which they will be able to extract the actual data from the image.
This projecct also gives basic level understanding of image processing methodologies.

Technogies used:
* File I/O operations
* Bitwise operations

Date of Completion : 18/07/2017

*************************************/


#include <stdio.h>
#include <string.h>

#include "std_def.h"
#include "types.h"
#include "encode.h"
#include "decode.h"

// argv[1] : option ie "-e" for encode or "-d" for decode

// For Encoding
// argv[2] : encoding source file; here "secret.file_extn"
// argv[3] : bmp source file
// argv[4] : Stegano destination file
// argv[5] : Password

// For Decoding
// argv[2] : Stegano source file
// argv[3] : Destination file name without extension
// argv[4] : Magic Pattern
// argv[5] : Password

EncodeInfo encInfo;
DecodeInfo decInfo;

int main(int argc, char *argv[])
{

	if(func_check_operation_type(argv) == e_encode)
	{

		encInfo.secret_fname       = argv[2];			// 2nd command line argument is "secret filename"
		encInfo.source_image_fname = argv[3]; 			// 3rd command line argument is "image file" in bmp format
		encInfo.stegano_fname      = argv[4];			// 4th command line argument is "stegano image" in bmp format
		encInfo.password           = argv[5];			// 5th command line argument is "password"

		func_encoding(&encInfo);
		func_cleanup_encode();
	}

	else if(func_check_operation_type(argv) == e_decode)
	{
		decInfo.stegano_image_fname = argv[2];			// 2nd command line argument is "steganographed image name"
		decInfo.dest_fname          = argv[3];			// 3rd command line argument is "destination file name without extension"
		decInfo.magic_pattern       = argv[4];			// 4th command line argument is "magic pattern"
		decInfo.password            = argv[5];			// 5th command line argument is "password"

		func_decoding(&decInfo);
		func_cleanup_decode();
	}

	else if(func_check_operation_type(argv) == e_unsupported)
	{
		fprintf(stderr, "This Operation is unsupported. Use -e for encode or -d for decode\n");
		return 1;
	}


	return 0;
}
