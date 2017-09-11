#ifndef DECODE_H
#define DECODE_H

typedef struct _DecodeInfo
{
	/* Stegano Image Info */
	char *stegano_image_fname;
	FILE *fptr_stegano_image;

	/* Magic Pattern info */
	char *magic_pattern;

	/* Password info */
	char *password;

	/* Destination Secret file */
	char *dest_fname;
	char *dest_file_extn;
	char *dest_fname_with_extn;
	FILE *fptr_dest_file;
	long size_dest_file;

} DecodeInfo;


/* Decoding Function Prototypes */

/* Get File pointers for input files */
Status func_open_source_files(DecodeInfo *decInfo);

/* Get File pointers for output files */
Status func_open_dest_files(DecodeInfo *decInfo);

/* Decoding */
Status func_decoding(DecodeInfo *decInfo);

/* Skip Header of Stegano file */
Status func_skip_stegano_header(DecodeInfo *decInfo);

/* Decode Magic Pattern*/
char *func_decode_magic_pattern(DecodeInfo *decInfo); 

/* Decode Password */
char *func_decode_password(DecodeInfo *decInfo);

/* Decode Secret file extension */
Status func_decode_secret_file_extn(DecodeInfo *decInfo);

/* Append file extension to user given filename */
Status func_append_dest_file_extn(DecodeInfo *decInfo);

/* Decode Secret file size */
long func_decode_secret_file_size(DecodeInfo *decInfo);

/* Decode Secret file data and store in destination file */
Status func_decode_and_store_secret_file(DecodeInfo *decInfo);

/* Cleanup */
void func_cleanup_decode(void);

#endif
