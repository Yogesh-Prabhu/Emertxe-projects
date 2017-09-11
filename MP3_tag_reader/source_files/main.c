/***********************************************************
 Project Title : MP3 Tag Reader

Description :
MP3 tag reader is a software which will read and display MP3 (ID3) tag information from MP3 files.

Technologies used :
* File I/O operations
* String and char operations

Date of Completion : 20/07/2017

***********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdio_ext.h>
#include "read.h"
#include "edit.h"
//#include "types.h"

ReadInfo rdInfo;
EditInfo edInfo;

int main(int argc, char *argv[])
{
	// Read tags option selected
	if(func_check_operation_type(argv) == e_read)
	{
		rdInfo.src_mp3_fname = argv[2];				// mp3 song file name
		rdInfo.tag_artist	 = "TPE1";	
		rdInfo.tag_album     = "TALB";
		rdInfo.tag_year      = "TYER";
		rdInfo.tag_title     = "TIT2";

		// Read tags and tag data
		if(func_read_mp3_tags(&rdInfo) == e_failure)
		// Error handling	
		{
			fprintf(stderr, "Function %s failed\n", "func_read_mp3_tags");

			// Cleanup
			func_cleanup_read();

			return 1;
		}

		rdInfo.dst_jpg_fname = "pic.jpg";		// Image name

		// Extract image
		if(func_get_jpg_image(&rdInfo) == e_failure)
		// Error handling	
		{
			fprintf(stderr, "Function %s failed\n", "func_get_jpg_image");

			// Cleanup
			func_cleanup_read();

			return 1;
		}


	}

	// Edit tags option selected
	if(func_check_operation_type(argv) == e_edit)
	{
		
		edInfo.src_dst_mp3_fname = argv[2];			// mp3 song file name

		edInfo.tag_album  = "TALB";
		edInfo.tag_year   = "TYER";
		edInfo.tag_title  = "TIT2";
		edInfo.tag_artist = "TPE1";


		printf("\n\n To Edit tags...\n");
		printf("Enter new Album name\n");
		__fpurge(stdin);
		scanf("%[^\n]", edInfo.album);

		printf("Enter new Year\n");
		__fpurge(stdin);
		scanf("%[^\n]", edInfo.year);

		printf("Enter new Title\n");
		__fpurge(stdin);
		scanf("%[^\n]", edInfo.title);

		printf("Enter new Artist\n");
		__fpurge(stdin);
		scanf("%[^\n]", edInfo.artist);

		__fpurge(stdin);		// If this __fpurge() is not added, then program waits infinitely at this point

		// Search mp3 tags and edit their corresponding data
		if(func_search_edit_mp3_tags(&edInfo) == e_failure)
		// Error handling
		{
			fprintf(stderr, "Function %s failed\n", "func_search_edit_mp3_tags");

			// Cleanup
			func_cleanup_edit();

			return 1;
		}	
		
	}
	return 0;
}
