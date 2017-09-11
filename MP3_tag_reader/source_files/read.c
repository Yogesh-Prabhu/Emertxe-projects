#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "std_def.h"
#include "types.h"
#include "read.h"

/* User defined function definitions */


/* Check operation type
 * Input : Command line argument vector
 * Output : Operation decoded
 * Return : e_read or e_edit or e_unsupported
 */
OperationType func_check_operation_type(char *argv[])
{
	if(argv[1][0] == '-' && argv[1][1] == 'r')      // read
	{   
		return e_read;
	}   
	else if(argv[1][0] == '-' && argv[1][1] == 'e')      // edit
	{   
		return e_edit;
	}   

	else
		// Unsupported operation
		return e_unsupported;
}



/* Get file pointer for source mp3 song file
 * Input : Source mp3 filename
 * Output : File pointer to this file 
 * Return Value : e_success or e_failure
 */
Status func_open_src_file_read(ReadInfo *rdInfo)
{
	// Open Source files
	rdInfo->fptr_src_mp3 = fopen(rdInfo->src_mp3_fname, "r");
	if(NULL == rdInfo->fptr_src_mp3)
	{
		fprintf(stderr, "File %s failed to open\n", rdInfo->src_mp3_fname);
		return e_failure;
	}
	
	// If no failure, then return e_success
	return e_success;
}



/* Get file pointer for destination jpg file
 * Input : Destination pic filename
 * Output : File pointer to this file
 * Return Value : e_success or e_failure
 */
Status func_open_dst_file_read(ReadInfo *rdInfo)
{
	// Open Destination files
	rdInfo->fptr_dst_jpg = fopen(rdInfo->dst_jpg_fname, "w");
	if(NULL == rdInfo->fptr_dst_jpg)
	{
		fprintf(stderr, "File %s failed to open\n", rdInfo->dst_jpg_fname);
		return e_failure;
	}
	
	// If no failure, then return e_success
	return e_success;
}



/* Reading tags
 * Input : All tags
 * Output : All tags data
 * Return Value : e_success or e_failure
 */
Status func_read_mp3_tags(ReadInfo *rdInfo)
{
	// Open Source file
	if(func_open_src_file_read(rdInfo) == e_failure)
	// Error handling	
	{
		fprintf(stderr, "Function %s failed\n", "func_open_src_file_read");
		return e_failure;
	}

	if(func_matching_tags(rdInfo->tag_album, &(rdInfo->album), rdInfo->fptr_src_mp3) == e_failure)
	// Error handling	
	{
		fprintf(stderr, "Function %s failed for Tag %s\n", "func_matching_tags", rdInfo->tag_album);
		return e_failure;
	}

	if(func_matching_tags(rdInfo->tag_title, &(rdInfo->title), rdInfo->fptr_src_mp3) == e_failure)
	// Error handling	
	{
		fprintf(stderr, "Function %s failed for Tag %s\n", "func_matching_tags", rdInfo->tag_title);
		return e_failure;
	}
	;

	if(func_matching_tags(rdInfo->tag_year, &(rdInfo->year), rdInfo->fptr_src_mp3) == e_failure)
	// Error handling	
	{
		fprintf(stderr, "Function %s failed for Tag %s\n", "func_matching_tags", rdInfo->tag_year);
		return e_failure;
	}
	;
/*
	if(func_matching_tags(rdInfo->tag_artist, &(rdInfo->artist), rdInfo->fptr_src_mp3) == e_failure)
	// Error handling	
	{
		fprintf(stderr, "Function %s failed for Tag %s\n", "func_matching_tags", rdInfo->tag_artist);
		return e_failure;
	}
*/
	

  	printf("------------------------------------------------------------\n");
	printf("Album		: %s\n", rdInfo->album);
	printf("Title		: %s\n", rdInfo->title);
	printf("Artist		: %s\n", rdInfo->artist);
	printf("Year		: %s\n", rdInfo->year);
    printf("------------------------------------------------------------\n");

	return e_success;
}


/* Matching Tags
 * Input : Tag name, file pointer to source mp3 file
 * Output : Tag data
 * Return Value : e_success or e_failure
 */
Status func_matching_tags(char *tag, char **tag_data, FILE *fptr)
{
	u_int idx = 0;
	u_int sz_tag_data = 0;	

	int ch;
	char temp[LEN_TAG + 1];
	char temp_data[LEN_TAG + 1];

	char tag_data_buff[50];
	*tag_data = malloc(50 * sizeof(char));

	fseek(fptr, 0L, SEEK_SET);

	while(feof(fptr) == 0)
	{
		// Search for tag
		fscanf(fptr, "%4[^\n]s", temp);			// Read 4 bytes at a time

		if(strcmp(temp, tag) == 0)				// tag found
		{
			// to read length of tag data			
			for(idx = 0; idx < LEN_SIZE_TAG_DATA; idx++)
			{
				ch=fgetc(fptr);

				sz_tag_data <<= sizeof(char);	// left shift by sizeof a byte = 8

				sz_tag_data |= ch;
			}
				
			fseek(fptr, 2L, SEEK_CUR);			// Skip 2 bytes of flag as per the format, to now point to tag data


			ch = fgetc(fptr);		// skip first non-printable byte from data
			sz_tag_data -= 1;		// since data size is reduced by 1 

			// now file pointer points to actual tag data 
			for(idx = 0; idx < sz_tag_data; idx++)	
			{
				ch = fgetc(fptr);
				tag_data_buff[idx] = ch;
			}

			tag_data_buff[idx] = '\0';

			break;	// break from the while loop
		}

			// Since 4 bytes were read at a time, and those bytes were not matched, so go to the next immediate pointer
			fseek(fptr, -3L, SEEK_CUR);		// fptr had moved by 4 bytes, so go back by 3 bytes
	}

	strcpy(*tag_data, tag_data_buff); 

	return e_success;
}



/* Get jpg image
 * Input : Destination Image name
 * Output : Destination image with proper data
 * Return Value : e_success or e_failure
 */
Status func_get_jpg_image(ReadInfo *rdInfo)
{
	unsigned char ch1, ch2;

	FILE *fptr_mp3 = rdInfo->fptr_src_mp3;

	// Open Destination file
	if(func_open_dst_file_read(rdInfo) == e_failure)
	// Error handling	
	{
		fprintf(stderr, "Function %s failed\n", "func_open_dst_file_read");
		return e_failure;
	}


	FILE *fptr_jpg = rdInfo->fptr_dst_jpg;

	fseek(fptr_mp3, 0L, SEEK_SET);

	do		// Check for "ff d8" data bytes
	{
		fscanf(fptr_mp3, "%c%c", &ch1, &ch2);

		// If 0xffd8 is found in the data, it indicates start of jpg image data
		if(0xff == ch1 && 0xd8 == ch2) 		// if "ff d8" found, copy them into new jpg file
		{
			fputc(ch1, fptr_jpg);
			fputc(ch2, fptr_jpg);
			
			break;
		}

		// return e_failure if jpg image data is not found untill end of mp3 file
		if(feof(fptr_mp3) != 0 )
		{
			return e_failure;
		}
	}while(1);

	do	// copy bytes till "ff d9" including them also 
	{
		fscanf(fptr_mp3, "%c%c", &ch1, &ch2);

		fputc(ch1, fptr_jpg);
		fputc(ch2, fptr_jpg);

		// If 0xffd9 is found in the data, it indicates end of jpg image data
		if(0xff == ch1 && 0xd9 == ch2)
		{
		//	fseek(fptr_jpg, -1L, SEEK_CUR);
			break;
		}

		// return e_failure if jpg image data is not found untill end of mp3 file
		if(feof(fptr_mp3) != 0 )
		{
			return e_failure;
		}

	}while(1);



	return e_success;
}


/*	Cleanup 
 * Input : void
 * Output : Print Error if cleanup failed 
 * Return Value : void
 */
void func_cleanup_read(void)
{
	if(fcloseall() == EOF)
	{
		printf("Function %s failed to close opened files of read module\n", "func_cleanup_read");
	}

	return;
}
