#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "std_def.h"
#include "types.h"
#include "edit.h"

/* User defined function definitions */


/* Get file pointer for mp3 song file
 * Input : Mp3 file name which is both source and destination
 * Output : File pointer to this file
 * Return Value : e_success or e_failure
 */
Status func_open_mp3_file(EditInfo *edInfo)
{
	// Open mp3 file
	edInfo->fptr_src_dst_mp3 = fopen(edInfo->src_dst_mp3_fname, "r+");
	if(NULL == edInfo->fptr_src_dst_mp3)
	{
		fprintf(stderr, "File %s failed to open\n", edInfo->src_dst_mp3_fname);
		return e_failure;
	}

	// If no failure, then return e_success
	return e_success;
}




/* Reading tags
 * Input : ALL Tag names
 * Output : All Tag data
 * Return Value : e_success or e_failure
 */
Status func_search_edit_mp3_tags(EditInfo *edInfo)
{
	// Open Source file
	if(func_open_mp3_file(edInfo) == e_failure)
	// Error handling   
	{
		fprintf(stderr, "Function %s failed\n", "func_open_mp3_file");
		return e_failure;
	}

	func_edit_tags_data(edInfo->tag_album, edInfo->album, edInfo->fptr_src_dst_mp3);

	func_edit_tags_data(edInfo->tag_title, edInfo->title, edInfo->fptr_src_dst_mp3);

	func_edit_tags_data(edInfo->tag_year, edInfo->year, edInfo->fptr_src_dst_mp3);

	//  func_matching_tags(edInfo->tag_artist, edInfo->artist, edInfo->fptr_src_dst_mp3);

	printf("After Editing\n");
	printf("------------------------------------------------------------\n");
	printf("Album       : %s\n", edInfo->album);
	printf("Title       : %s\n", edInfo->title);
	// printf("Artist      : %s\n", edInfo->artist);
	printf("Year        : %s\n", edInfo->year);
	printf("------------------------------------------------------------\n");


	return e_success;
}



/* Matching tags
 * Input : Tag names and file pointer to source mp3 file
 * Output : Matched Tag data
 * Return Value : e_success or e_failure
 */
Status func_edit_tags_data(char *tag, char *tag_new_data, FILE *fptr)
{
	int idx = 0;
	u_int sz_tag_data = 0;

	int ch_num;
	char temp[LEN_TAG + 1];
	char temp_data[LEN_TAG + 1];

	fseek(fptr, 0L, SEEK_SET);

	while(feof(fptr) == 0)
	{
		// Search for tag
		fscanf(fptr, "%4[^\n]s", temp);

		if(strcmp(temp, tag) == 0)          // tag found
		{
			// to change length of tag data
			sz_tag_data = strlen(tag_new_data) + 1;	// 1 extra as first char is empty
			//sz_tag_data = strlen(tag_new_data) ;
			u_int mask = 0xff;
			for(idx = LEN_SIZE_TAG_DATA - 1; idx >= 0; idx--)
			{
				ch_num = (sz_tag_data & (mask << (8 * idx) )) >> idx;
				fputc(ch_num, fptr);
			}

			fseek(fptr, 2L, SEEK_CUR);          // Skip 2 bytes of flag as per the format, to now point to tag data

			fseek(fptr, 1L, SEEK_CUR);       // skip first non-printable byte from data
			sz_tag_data--;
			// now file pointer points to begining of actual tag data
			// Store the new data
			for(idx = 0; idx < sz_tag_data; idx++)
			{
				fputc(tag_new_data[idx], fptr);
			}


			break;  // break from the while loop
		}// if ends

		fseek(fptr, -3L, SEEK_CUR);		// To goto next byte, instead of 4th byte
	}// while loop ends

	return e_success;
}

/* Cleanup function
 * Input : void
 * Output : Close all opened File pointers
 * Return value : void
 */
void func_cleanup_edit(void)
{
	if(fcloseall() == EOF )
	{
		printf("Function %s failed at closing all open function pointers\n", "func_cleanup_edit");
	}
	return;
}

