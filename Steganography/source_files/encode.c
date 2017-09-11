#include <stdio.h>
#include <string.h>

#include "std_def.h"
#include "types.h"
#include "encode.h"

#define _GNU_SOURCE			// for fcloseall

/* Function Definitions */


/* Check operation type
 * Input : Command line argument vector
 * Output : Operation decoded
 * Return : e_encode or e_decode or e_unsupported
 */
OperationType func_check_operation_type(char *argv[])
{
	if(argv[1][0] == '-' && argv[1][1] == 'e')      // Encode
	{
		return e_encode;
	}
	else if(argv[1][0] == '-' && argv[1][1] == 'd')      // Decode
	{
		return e_decode;
	}

	else
		// Unsupported operation
		return e_unsupported;
}



/* Open files related to encoding
 * Input : Source Image file, Secret file and Stegano Image file
 * Output : FILE pointer for above files 
 * Return Value : e_success or e_failure 
 */

Status func_open_source_files_encode(EncodeInfo *encInfo)
{

	// Src Image file
	encInfo->fptr_source_image = fopen(encInfo->source_image_fname, "r");
	// Error handling
	if(NULL == encInfo->fptr_source_image)
	{
		perror("fopen");
		fprintf(stderr, "Error: Unable to open file %s\n", encInfo->source_image_fname);
		return e_failure;
	}

	// Secret file
	encInfo->fptr_secret_file = fopen(encInfo->secret_fname, "r");
	// Error handling
	if(NULL == encInfo->fptr_secret_file)
	{
		perror("fopen");
		fprintf(stderr, "Error: Unable to open file %s\n", encInfo->secret_fname);
		return e_failure;
	}

	// If no failure, then return e_success
	return e_success;
}



/* Open files related to decoding
 * Input : Destination Steganographed image name
 * Output : File poiinter to destination file
 * Return Value : e_success or e_failure
 */  
Status func_open_dest_files_encode(EncodeInfo *encInfo)
{
	// Stegano Image file
	encInfo->fptr_stegano_image = fopen(encInfo->stegano_fname, "w");
	// Error handling
	if(NULL == encInfo->fptr_stegano_image)
	{
		perror("fopen");
		fprintf(stderr, "Error: Unable to open file %s\n", encInfo->stegano_fname);
		return e_failure;
	}

	// If no failure, then return e_success
	return e_success;
}



/* Get Image Capacity
 * Input : File pointer to source image
 * Output : Data holding capacity of source bmp file
 * Return Value : Image data holding Capacity
 */
u_int func_get_image_capacity_for_bmp(EncodeInfo *encInfo)
{
	u_int capacity = 0;

	fseek(encInfo->fptr_source_image, 0L, SEEK_END);				// File pointer pointing to end of file

	capacity = ftell(encInfo->fptr_source_image) - BMP_HEADER_SIZE;	// Get location of end of file; subtract bmp header to get bmp file capacity

	fseek(encInfo->fptr_source_image, 0L, SEEK_SET);				// Reset file pointer to start of file
	return capacity;
}




/* Perform Encoding
 * Input : Source Image file, Secret file 
 * Output : Stegano Image file
 * Return Value : e_success or e_failure
 */
Status func_encoding(EncodeInfo *encInfo)
{
	u_int capacity_image;
	u_int sz_secret;

	// Open Encoding Source files
	if((func_open_source_files_encode(encInfo)) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_open_source_files_encode");
		return e_failure;
	}


	// Get Image capacity for holding encoded data
	capacity_image = func_get_image_capacity_for_bmp(encInfo);
	sz_secret = func_get_file_size(encInfo); 

	// Check if given bmp image can hold encoded secret file
	if(capacity_image < (8 * sz_secret))		// each bit of data is encoded into a LSB of each bmp byte ; so 8 *
	{
		fprintf(stderr, "%s image cannot hold encoded %s file\n", encInfo->source_image_fname, encInfo->secret_fname);
		return e_failure;
	}

	// Open Encoding Dest files
	if((func_open_dest_files_encode(encInfo)) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_open_dest_files_encode");
		return e_failure;
	}


	// Copy Header of Source Image file (here .bmp file) to Stegano file
	if(func_copy_bmp_header(encInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_copy_bmp_header");
		return e_failure;
	}


	// Encode Magic Pattern
	if(func_encode_magic_pattern_with_length(encInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_encode_magic_pattern");
		return e_failure;
	}


	// Encode Password length and Password into stegano Image file
	if(func_encode_password_length_and_password(encInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_encode_password_length_and_password");
		return e_failure;
	}


	// Encode length of extension and extension of Secret file name
	char file_extn[MAX_FILE_SUFFIX];
	if(func_encode_secret_file_extn_size_and_extn(encInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_encode_secret_file_extn_size_and_extn");
		return e_failure;
	}


	// Get Size of Secret file
	long sz_file = func_get_file_size(encInfo);
	 
	// Encode Secret file size into Stegano Image file
	if(func_encode_secret_file_size(sz_file, encInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_encode_secret_file_size");
		return e_failure;
	}


	// Encode Secret file Data into Stegano Image file
	if(func_encode_secret_file_data(encInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_encode_secret_file_data");
		return e_failure;
	}


	// Copy remaining Source Image bytes to Stegano file as it is
	if(func_copy_remaining_image_data(encInfo->fptr_source_image, encInfo->fptr_stegano_image) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_copy_remaining_image_data");
		return e_failure;
	}

	// If no errors then return success
	return e_success;
}




/* Copy header of bmp file to stegano file
 * Input: Source Image file, Secret file and Stegano Image file 
 * Output: Header of bmp file copied to stegano file as its header
 * Return Value: e_success or e_failure
 */
Status func_copy_bmp_header(EncodeInfo *encInfo)
{
	int idx;
	int ch;

	for(idx = 0; idx < BMP_HEADER_SIZE; idx++)      // Copy header of image file to new image file
	{
		ch = fgetc(encInfo->fptr_source_image );
		fputc(ch, encInfo->fptr_stegano_image);
	}

	return e_success;
}



/* Encode Magic Pattern
 * Input : Magic Pattern as given in header file
 * Output : Magic pattern encoded 
 * Return value : e_success or e_failure
 */
Status func_encode_magic_pattern_with_length(EncodeInfo *encInfo)
{
	int idx, cdx;
	int ch, byte_stegano;
	int mask = ~1;				// ie 0xffff fffe ie only LSB as zero  

	char *magic_pattern = MAGIC_PATTERN;
	u_int len_magic = strlen(magic_pattern);

	// Each bit of a byte of secret  data is encoded as LSB of a stegano byte 

	// Encode length of Magic Pattern
	for(idx = 0; idx < (8 * sizeof(len_magic)); idx++)
	{
		byte_stegano = ((len_magic & (1 << idx)) >> idx) | (fgetc(encInfo->fptr_source_image) & mask);
		fputc(byte_stegano, encInfo->fptr_stegano_image);
	}

	// Encode Magic Pattern
	for(idx = 0; idx < len_magic; idx++)
	{   
		for(cdx = 0; cdx < (8 * sizeof(char)); cdx++)				// cdx stands for character index 
		{
			byte_stegano = ((magic_pattern[idx] & (1 << cdx)) >> cdx) | (fgetc(encInfo->fptr_source_image) & mask);     

			fputc(byte_stegano, encInfo->fptr_stegano_image);
		}       
	}

	return e_success;
}



/* Encode Password Length and Password into Stegano image  
 * Input : Password
 * Output : Password length and Password encoded 
 * Return value : e_success or e_failure
 */
Status func_encode_password_length_and_password(EncodeInfo *encInfo)
{
	int idx, cdx;
	int ch, byte_stegano;
	int mask = ~1;				// ie 0xffff fffe ie only LSB as zero  

	u_int pswd_len = strlen(encInfo->password);
	encInfo->len_password = pswd_len;

	// Each bit of a byte of secret  data is encoded as LSB of a stegano byte 

	// Encode Password length
	for(idx = 0; idx < (8 * sizeof(pswd_len)); idx++)
	{
		byte_stegano = ((pswd_len & (1 << idx)) >> idx) | (fgetc(encInfo->fptr_source_image) & mask);
		fputc(byte_stegano, encInfo->fptr_stegano_image);
	}

	// Encode Password
	for(idx = 0; idx < pswd_len; idx++)
	{   
		for(cdx = 0; cdx < (8 * sizeof(char)); cdx++)				// cdx stands for character index 
		{
			byte_stegano = (((encInfo->password)[idx] & (1 << cdx)) >> cdx) | (fgetc(encInfo->fptr_source_image) & mask);     

			fputc(byte_stegano, encInfo->fptr_stegano_image);
		}       

	}

	return e_success;
}



/* Encode Secret file extension
 * Input : Secret file name
 * Output : Secret file name extension
 * Return value : e_success or e_failure
 */
Status func_encode_secret_file_extn_size_and_extn(EncodeInfo *encInfo)
{

	int idx, cdx;
	int ch, byte_stegano;
	int mask = ~1;				// ie 0xffff fffe ie only LSB as zero  

	u_int sz_extn;
	const char *file_extn;

	file_extn = strchr(encInfo->secret_fname, '.') + sizeof(char);	// Extra sizeof(char) added to the pointer to skip '.' in extension

	// Each bit of a byte of secret  data is encoded as LSB of a stegano byte 

	sz_extn = strlen(file_extn);

	// Encode file extension size
	for(idx = 0; idx < (8 * sizeof(sz_extn)); idx++)
	{
		byte_stegano = ((sz_extn & (1 << idx)) >> idx) | (fgetc(encInfo->fptr_source_image) & mask);
		fputc(byte_stegano, encInfo->fptr_stegano_image);
	}

	// Encode file extenstion
	for(idx = 0; idx < sz_extn; idx++)
	{   
		for(cdx = 0; cdx < (8 * sizeof(char)); cdx++)				// cdx stands for character index 
		{
			byte_stegano = ((file_extn[idx] & (1 << cdx)) >> cdx) | (fgetc(encInfo->fptr_source_image) & mask);     

			fputc(byte_stegano, encInfo->fptr_stegano_image);
		}       

	}

	return e_success;
}



/* Encode Secret file size into the Stegano file
 * Input: file size, Structure Pointer to Encoding Info
 * Output: Secret file size encoded into Stegano file
 * Return Value: e_success or e_failure
 */
Status func_encode_secret_file_size(long sz_file, EncodeInfo *encInfo)
{
	int mask = ~1;                      // Mask (0xfffffffe )for image byte 
	int idx;
	int byte_stegano;

	// Each bit of a byte of secret  data is encoded as LSB of a stegano byte 
	for(idx = 0; idx < (8 * sizeof(sz_file)); idx++)
	{
		byte_stegano = ((sz_file & (1 << idx)) >> idx) | (fgetc(encInfo->fptr_source_image) & mask);
		fputc(byte_stegano, encInfo->fptr_stegano_image);
	}

	return e_success;
}


/* Get size of secret file file 
 * Input: Struct pointer to Encode Info 
 * Output: Size of secret file file
 * Return Value: Size of secret file file
 */
long func_get_file_size(EncodeInfo *encInfo)
{
	long sz_file;
	fseek(encInfo->fptr_secret_file, 0L, SEEK_END);					// Take the file pointer to end of file
	sz_file = ftell(encInfo->fptr_secret_file);						// Get location of file pointer
	encInfo->size_secret_file = sz_file;							// Store it at approriate location

	fseek(encInfo->fptr_secret_file, 0L, SEEK_SET);                  // To reset the file pointer to start

	return sz_file;
}



/* Encode Secret file data
 * Input : Source Image file, Source Text file  
 * Output: Stegano Image file
 * Return Value: e_success or e_failure
 */
Status func_encode_secret_file_data(EncodeInfo *encInfo)
{
	int idx, cdx;
	int ch, byte_stegano;
	int mask = ~1;

	// Each bit of a byte of secret  data is encoded as LSB of a stegano byte 

	for(idx = 0; idx < encInfo->size_secret_file; idx++)
	{   
		ch = fgetc(encInfo->fptr_secret_file);                   // get each char of secret file file     

		for(cdx = 0; cdx < (8 * sizeof(char)); cdx++)       // cdx stands for character index 
		{
			byte_stegano = ((ch & (1 << cdx)) >> cdx) | (fgetc(encInfo->fptr_source_image) & mask);     

			fputc(byte_stegano, encInfo->fptr_stegano_image);
		}       

	}

	return e_success;
}


/* Copy remaining Source Image Data bytes to Stegano Image fileafter encoding
 * Input : FILE pointer to Source Image file and Destination Stegano file 
 * Output: Stegano file complete
 * Return Value: e_success or e_failure
 */
Status func_copy_remaining_image_data(FILE *fptr_src, FILE *fptr_dest)
{
	int ch;

	while((ch = fgetc(fptr_src)) != EOF)		// Fetch data one byte at a time
	{   
		fputc(ch, fptr_dest);					// Copy it to destination
	}   

	return e_success;
}


/* Cleanup operations 
 * Input : Void
 * Output: All encoding related open file pointers closed
 * Return Value : Void
 */
void func_cleanup_encode(void)
{
	/* Close all open file pointers that were opened for encoding */

	printf("Encoding Successful\n");
	if(fcloseall() == EOF)
	
		fprintf(stderr, "ERROR in closing all open file pointers related to encoding\n");

	return;
}
