#include "main.h"

/* File pointers to source and destination file */
FILE *fptr_src = NULL, *fptr_dst = NULL;

/* Function to get file pointers for source C file, and destination HTML file
 * Input  : Command line arguments
 * Output : File pointers
 * Return : Success or failure
 */

Status_t func_get_file_ptr(int argc, char *argv[])
{
	if(argc > 1)
	{
		if(NULL == (fptr_src = fopen(argv[1],"r")))
		{
			// Error handling
			fprintf(stderr, "Error: File %s failed to open in %s mode \n", argv[1], "r");
			return e_failure;
		}

		if(NULL == (fptr_dst = fopen(argv[2],"w")))
		{
			// Error handling
			fprintf(stderr, "Error: File %s failed to open in %s mode \n", argv[2], "w");
			return e_failure;
		}

		// If files open successfully, then return success
		return e_success;
	}

	else
	{
		// Error handling
		fprintf (stderr, "Error: Pass cmd line arg as <./a.out> <c_src_file> <html_dest_file>\n");
		return e_failure;
	}
}



/* Function to write inital section of destination html code
 * Input  : void
 * Output : header of html file
 * Return : Success or failure
 */

Status_t func_write_html_start(void)
{
	fprintf(fptr_dst, "<!DOCTYPE html>\n");
	fprintf(fptr_dst, "<html lang=\"en-US\">\n");
	fprintf(fptr_dst, "<head>\n");
	fprintf(fptr_dst, "<link rel=\"stylesheet\" href=\"styles.css\">\n");
	fprintf(fptr_dst, "</head>\n");
	fprintf(fptr_dst, "<body>\n");
	fprintf(fptr_dst, "<prev>\n");

	return e_success;
}



/* Function to write the end part (footer) of html file
 * Input  : void
 * Output : footer of html file
 * Return : void
 */

Status_t func_write_html_end(void)
{

	fprintf(fptr_dst, "</prev>\n");
	fprintf(fptr_dst, "</body>\n");
	fprintf(fptr_dst, "</html>\n");

	return e_success;
}


/* Functions to write html data with colour coding 
 * Input  : Tokens from C program using lexer
 * Output : HTML code for respective C token
 * Return : void
 */

// Encoding Comments in HTML
void func_comments(char *buffer)
{
	fprintf(fptr_dst, "<span class=\"comments\">%s</span>\n", buffer);
	fprintf(fptr_dst, "<span class=\"backspace\"></span>");
	return;
}

// Encoding Datatypes Keywords in HTML
void func_datatype(char *buffer)
{
	fprintf(fptr_dst, "<span class=\"keyword_datatypes\">%s</span>\n", buffer);
	fprintf(fptr_dst, "<span class=\"backspace\"></span>");
	return;
}

// Encoding Non-Datatypes Keywords in HTML
void func_keywords(char *buffer)
{
	fprintf(fptr_dst, "<span class=\"keyword_non_datatypes\">%s</span>\n", buffer);
	return;
}

// Encoding Preprocessor of type #define in HTML
void func_prepro(char *buffer)
{
	fprintf(fptr_dst, "<span class=\"preprocessor_dir\">%s</span>\n", buffer);
	return;
}

// Encoding Preprocessor of type #include in HTML
void func_header(char *buffer)
{
	//  &lt; and &gt; is used to represent '<' and '>' in HTML
	if (strchr(buffer, '<'))
	{
		char *hash_include = strtok (buffer, "<");
		fprintf(fptr_dst, "<span class=\"header_files\">%s</span>\n", hash_include);

		char *file_name = strtok (NULL, ">");
		fprintf(fptr_dst, "<span class=\"header_files\">&lt;%s&gt;</span>\n", file_name);
	}
	else
	{
		fprintf(fptr_dst, "<span class=\"header_files\">%s</span>\n", buffer);
	}

	return;
}

// Encoding Double quoted string in HTML
void func_string(char *buffer)
{
	fprintf(fptr_dst, "<span class=\"string\">%s</span>\n", buffer);
	return;
}

// Encoding Single quoted ASCII characters in HTML
void func_ascii (char *buffer)
{
	fprintf(fptr_dst, "<span class=\"ascii_char\">%s</span>\n", buffer);
	return;
}

// Encoding Numeric Constant in HTML
void func_num_const(char *buffer)
{
	fprintf(fptr_dst, "<span class=\"numeric_constant\">%s</span>\n", buffer);
	return;
}

// Encoding Space in HTML
void func_space()
{
	fprintf(fptr_dst, "<span>&ensp;</span>\n");
	return;
}

// Encoding Tab in HTML
void func_tab(void)
{
	fprintf(fptr_dst, "<span>&emsp;</span>\n");
	return;
}

// Encoding Non-AlphaNumeric characters (except newline) in HTML
void func_others(char *buffer)
{
	fprintf(fptr_dst, "<span>%s</span>\n", buffer);
	return;
}

// Encoding Newline in HTML
void func_newline(void)
{
	fprintf(fptr_dst, "<br>\n");
	return;
}

