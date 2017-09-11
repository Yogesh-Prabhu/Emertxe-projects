#ifndef READ_H
#define READ_H

#include "types.h"
/* Structure to store infomation regarding Source 
 * mp3 file and destination embedded image file */

typedef struct _ReadInfo
{
	/* Source mp3 song file info */
	char *src_mp3_fname;
	FILE *fptr_src_mp3;
	
	char *tag_artist;
	char *artist;
	char *tag_album;
	char *album;
	char *tag_year;
	char *year;
	char *tag_title;
	char *title;


	/* Destination jpeg file info */
	char *dst_jpg_fname;
	FILE *fptr_dst_jpg;
	

}ReadInfo;


/* Function Prototypes for reading source mp3 file */

/* Check Operation type */
OperationType func_check_operation_type(char *argv[]);

/* Get file pointer for source mp3 song file */
Status func_open_src_file_read(ReadInfo *rdInfo);

/* Get file pointer for destination picture file */
Status func_open_dst_file_read(ReadInfo *rdInfo);

/* Reading all tags */
Status func_read_mp3_tags(ReadInfo *rdInfo);

/* Fetching tag data */
Status func_matching_tags(char *tag, char **tag_data, FILE *fptr);

/* Extract image from source mp3 file */
Status func_get_jpg_image(ReadInfo *rdInfo);

/* Cleanup function */
void func_cleanup_read(void);


#endif
