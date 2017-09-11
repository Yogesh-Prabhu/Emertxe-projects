#ifndef ENCODE_H
#define ENCODE_H

#define MAGIC_PATTERN	"*#+"		// Set Magic Pattern


/* Structure to store information required for
 * encoding secret file to source Image.
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _EncodeInfo
{
	/* Source Image info */
	char *source_image_fname;
	FILE *fptr_source_image;
	u_int image_capacity;

	/* Password info */
	char *password;
	u_int len_password;

	/* Secret File info */
	char *secret_fname;
	FILE *fptr_secret_file;
	char extn_secret_file[MAX_FILE_SUFFIX];
	long size_secret_file;

	/* Stegano File info*/
	char *stegano_fname;
	FILE *fptr_stegano_image;

}EncodeInfo;

/* Encoding Function Prototypes */

/* Check Operation Type */
OperationType func_check_operation_type(char *argv[]);

/* Get file pointers for source files*/
Status func_open_source_files_encode(EncodeInfo *encInfo);

/* Get file pointers for destination files*/
Status func_open_dest_files_encode(EncodeInfo *encInfo);

/* Get data holding capacity of given bmp image */
u_int func_get_image_capacity_for_bmp(EncodeInfo *encInfo); 

/* Perform Encoding */
Status func_encoding(EncodeInfo *encInfo);

/* Copy BMP image header */
Status func_copy_bmp_header(EncodeInfo *encInfo);

/* Encode Magic Patter along with its length */
Status func_encode_magic_pattern_with_length(EncodeInfo *encInfo);

/* Encode Password Length and Password */
Status func_encode_password_length_and_password(EncodeInfo *encInfo);

/* Encode Secret file extension */
Status func_encode_secret_file_extn_size_and_extn(EncodeInfo *encInfo);

/* Encode Secret file size */
Status func_encode_secret_file_size(long sz_file, EncodeInfo *encInfo);

/* Get secret file size */
long func_get_file_size(EncodeInfo *encInfo);

/* Encode secret file data */
Status func_encode_secret_file_data(EncodeInfo *encInfo);

/* Copy remaining image data */
Status func_copy_remaining_image_data(FILE *fptr_src, FILE *fptr_dest);

/* Cleanup */
void func_cleanup_encode(void);


#endif
