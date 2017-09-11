#ifndef EDIT_H
#define EDIT_H

#include "types.h"

typedef struct _EditInfo
{
	char *src_dst_mp3_fname;
	FILE *fptr_src_dst_mp3;

	char *tag_artist;
	char artist[50];
	char *tag_album;
	char album[50];
	char *tag_year;
	char year[50];
	char *tag_title;
	char title[50];


} EditInfo;


/* Get File pointer for mp3 file */
Status func_open_mp3_file(EditInfo *edInfo);


/* Search tags and edit tag data */
Status func_search_edit_mp3_tags(EditInfo *edInfo);

/* Edit Tag data */
Status func_edit_tags_data(char *tag, char *tag_new_data, FILE *fptr);

/* Cleanup */
void func_cleanup_edit(void);


#endif
