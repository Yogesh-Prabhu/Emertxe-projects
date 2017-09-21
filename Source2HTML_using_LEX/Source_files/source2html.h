#ifndef SOURCE2HTML_H
#define SOURCE2HTML_H


/* Get file pointers for source c and destination html file*/
Status_t func_get_file_ptr(int argc, char *argv[]);

/* Insert initial header in HTML file */
Status_t func_write_html_start(void);

/* Insert end footer in HTML file*/
Status_t func_write_html_end(void);

#endif
