#include <stdio.h>
#include <string.h>

#include "std_def.h"
#include "types.h"
#include "decode.h"

/* Function Definitions */

/* Get file pointers to input files related to decoding 
 * Input : Stegnographed source file name
 * Output : File pointers to source files
 * Return Value : e_success or e_failure
 */
Status func_open_source_files_decode(DecodeInfo *decInfo)
{

	// Src Image file
	decInfo->fptr_stegano_image = fopen(decInfo->stegano_image_fname, "r");
	// Error handling
	if(NULL == decInfo->fptr_stegano_image)
	{
		perror("fopen");
		fprintf(stderr, "Error: Unable to open file %s\n", decInfo->stegano_image_fname);
		return e_failure;
	}

	// If no failure, then return e_success
	return e_success;
}



/* Get file pointers to output files related to decoding
 * Input : Destination or result file name with extension
 * Output : File pointers to destination files
 * Return Value : e_success or e_failure
 */
Status func_open_dest_files_decode(DecodeInfo *decInfo)
{
	decInfo->fptr_dest_file = fopen(decInfo->dest_fname_with_extn, "w");
	// Error handling
	if(NULL == decInfo->fptr_dest_file)
	{
		perror("fopen");
		fprintf(stderr, "Error: Unable to open file %s\n", decInfo->dest_fname);
		return e_failure;
	}

	// If no failure, then return e_success
	return e_success;
}



/* Perform Decoding
 * Input : Steganographed source file, user given Magic pattern and password
 * Output : Decoded resultant file with extension
 * Return value : e_success or e_failure
 */	
Status func_decoding(DecodeInfo *decInfo)
{
	u_int sz_image;

	// Open Source files for Decoding
	if((func_open_source_files_decode(decInfo)) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_open_source_files_decode");
		return e_failure;
	}

	// Skip Header section of Source Steano image file 
	if(func_skip_stegano_header(decInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_skip_header");
		return e_failure;
	}

	// Magic Pattern Matching
	char *decoded_magic_pattern = func_decode_magic_pattern(decInfo);
	if(strcmp(decoded_magic_pattern, decInfo->magic_pattern) != 0)		// If Magic Pattern doesn't match, then exit program 
	{	
		fprintf(stderr, "Magic Pattern didn't match\n");
		return e_failure;
	}

	// Password Matching
	char *decoded_password = func_decode_password(decInfo);
	if(strcmp(decoded_password, decInfo->password) != 0)				// If Password doesn't match, then exit program 
	{	
		fprintf(stderr, "Password didn't match\n");
		return e_failure;
	}

	// Recover Secret file extension extension 
	if(func_decode_secret_file_extn(decInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_decode_secret_file_extn");
		return e_failure;
	}

	// Append secret file extension to user given destination filename
	if(func_append_dest_file_extn(decInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_append_dest_file_extn");
		return e_failure;
	}

	// Open Destination files for Decoding
	if((func_open_dest_files_decode(decInfo)) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_open_dest_files_decode");
		return e_failure;
	}


	// Recover Secret file size
	long sz_file = 0;
	sz_file = func_decode_secret_file_size(decInfo);


	// Recover Secret file data and store it in destination
	if(func_decode_and_store_secret_file(decInfo) == e_failure)
	{
		fprintf(stderr, "Function %s failed\n", "func_decode_store_secret_file");
		return e_failure;
	}


	// Else return Success
	return e_success;
}



/* Skip Header of Source Stegano image file
 * Input : Steganographed Source file
 * Output : Source File pointer points to data byte right after bmp header 
 * Return Value : e_success or e_failure
 */
Status func_skip_stegano_header(DecodeInfo *decInfo)
{
	if(fseek(decInfo->fptr_stegano_image, BMP_HEADER_SIZE, SEEK_SET) != 0)
	{
		perror("fseek");
		fprintf(stderr, "Function %s failed\n", "func_skip_stegano_header");
		return e_failure;
	}

	// Else return success
	return e_success;
}



/* Decode Magic pattern
 * Input : Source file pointer	
 * Output : Decoded Magic Pattern
 * Return value : Decoded Magic Pattern from source file
 */
char *func_decode_magic_pattern(DecodeInfo *decInfo)
{
	int ch;
	int idx, cdx;
	
	long len_magic = 0;
	static char magic_pattern[MAX_MAGIC_PATTERN_SIZE];  

	// 8 encoded bytes give one data byte 

	// Decode magic pattern size 
	for(idx = 0; idx < 8 * sizeof(len_magic); idx++)
	{

		ch = fgetc(decInfo->fptr_stegano_image);

		len_magic |= (ch & 1) << idx;

	}

	// Decode magic pattern 
	int char_decoded;
	for(idx = 0; idx < len_magic; idx++)
	{
		char_decoded = 0;

		// 8 encoded bytes give one data byte 
		for(cdx = 0; cdx < 8 * sizeof(char); cdx++)
		{
			ch = fgetc(decInfo->fptr_stegano_image);

			char_decoded |= (ch & 1) << cdx;
		}
		magic_pattern[idx] = char_decoded;
	}

	magic_pattern[idx] = '\0'; 		// Append NULL character to make it string

	return magic_pattern;
}




/* Decode Password
 * Input : Source file pointer
 * Output : Decoded password from source file
 * Return Value : Decoded password
 */
char *func_decode_password(DecodeInfo *decInfo)
{
	int ch;
	int idx, cdx;
	
	long len_pswd = 0;
	static char password[MAX_PSWD_SIZE];  

	// 8 encoded bytes give one data byte 

	// Decode password size 
	for(idx = 0; idx < 8 * sizeof(len_pswd); idx++)
	{

		ch = fgetc(decInfo->fptr_stegano_image);

		len_pswd |= (ch & 1) << idx;

	}

	// Decode password
	int char_decoded;
	for(idx = 0; idx < len_pswd; idx++)
	{
		char_decoded = 0;
		
		// 8 encoded bytes give one data byte 
		for(cdx = 0; cdx < 8 * sizeof(char); cdx++)
		{
			ch = fgetc(decInfo->fptr_stegano_image);

			char_decoded |= (ch & 1) << cdx;
		}
		password[idx] = char_decoded;
	}

	password[idx] = '\0'; 		// Append NULL character to make it string

	return password;

}



/* Decode Secret file extension
 * Input : Source file pointer
 * Output : Decoded file extension of secret file from source Steganographed file
 * Return Value : e_success or e_failure
 */
Status func_decode_secret_file_extn(DecodeInfo *decInfo)
{
	int ch;
	int idx, cdx;
	
	// Decode extension size 
	long sz_extn = 0;
	static char file_extn[MAX_FILE_SUFFIX];  

	// 8 encoded bytes give one data byte 
	for(idx = 0; idx < 8 * sizeof(sz_extn); idx++)
	{

		ch = fgetc(decInfo->fptr_stegano_image);

		sz_extn |= (ch & 1) << idx;

	}

	// Decode extension
	int char_decoded;
	for(idx = 0; idx < sz_extn; idx++)
	{
		char_decoded = 0;

		// 8 encoded bytes give one data byte 
		for(cdx = 0; cdx < 8 * sizeof(char); cdx++)
		{
			ch = fgetc(decInfo->fptr_stegano_image);

			char_decoded |= (ch & 1) << cdx;
		}
		file_extn[idx] = char_decoded;
	}

	file_extn[idx] = '\0'; 		// Append NULL character to make it string

	decInfo->dest_file_extn = file_extn;

	return e_success;
}



/* Append destination file extension
 * Input : User given destination file name  
 * Output : Destination file name with extension
 * Return Value : e_success or e_failure
 */
Status func_append_dest_file_extn(DecodeInfo *decInfo)
{
	// Take file name, a dot '.' and the  extension, and concatenate them.   
	static char full_name[50];
	strcpy(full_name, decInfo->dest_fname);
	strcat(full_name, ".");
	strcat(full_name, decInfo->dest_file_extn);

	decInfo->dest_fname_with_extn = full_name;

	return e_success;
}




/* Decode size of Secret file
 * Input : Steganographed source file
 * Output : Size of secret file 
 * Return value : Size of secret file
 */	
long func_decode_secret_file_size(DecodeInfo *decInfo)
{
	int ch;
	int idx;

	long sz_file = 0;
	
	// 8 encoded bytes give one data byte 
	for(idx = 0; idx < 8 * sizeof(sz_file); idx++)
	{

		ch = fgetc(decInfo->fptr_stegano_image);

		sz_file |= (ch & 1) << idx;

	}

	decInfo->size_dest_file = sz_file;
	
	return sz_file;
}



/* Decode Secret file data and store in destination file 
 * Input : Source Steganographed file
 * Output : Decoded Secret destination file
 * Return value : e_success or e_failure
 */	
Status func_decode_and_store_secret_file(DecodeInfo *decInfo)
{
	int idx, cdx; 
	int ch, char_decoded;
	for(idx = 0; idx < decInfo->size_dest_file; idx++)
	{
		char_decoded = 0;
		
		// 8 encoded bytes give one data byte 
		for(cdx = 0; cdx < 8 * sizeof(char); cdx++)
		{
			ch = fgetc(decInfo->fptr_stegano_image);

			char_decoded |= (ch & 1) << cdx;
		}
		fputc(char_decoded, decInfo->fptr_dest_file);
	}
	
	return e_success;	
}



/* Cleanup operations 
 * Input: void 
 * Output: all opened file pointers related to decoing closed
 * Return Value: void
 */
void func_cleanup_decode(void)
{
	/* Close all open file pointers that were opened for decoding */

	printf("Decoding Successful\n");
	if(fcloseall() == EOF)

		fprintf(stderr, "ERROR in closing all open file pointers related to decoding\n");

	return;
}
