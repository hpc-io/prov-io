/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * sv.c - Parse separated-values (CSV, TSV) files
 *
 * Copyright (C) 2009-2014, David Beckett http://www.dajobe.org/
 * 
 * This package is Free Software
 * 
 * It is licensed under the following three licenses as alternatives:
 *   1. GNU Lesser General Public License (LGPL) V2.1 or any newer version
 *   2. GNU General Public License (GPL) V2 or any newer version
 *   3. Apache License, V2.0 or any newer version
 * 
 * You may not use this file except in compliance with at least one of
 * the above three licenses.
 * 
 * See LICENSE.txt at the top of this package for the
 * complete terms and further detail along with the license texts for
 * the licenses in COPYING.LIB, COPYING and LICENSE-2.0.txt respectively.
 * 
 */


#ifdef SV_CONFIG
#include <sv_config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <sv.h>

/* bit flags */
#define SV_FLAGS_SAVE_HEADER    (1<<0)
/* error out on bad data lines */
#define SV_FLAGS_BAD_DATA_ERROR (1<<1)
/* allow fields to be quoted */
#define SV_FLAGS_QUOTED_FIELDS  (1<<2)
/* strip (non-separator) whitespace around fields */
#define SV_FLAGS_STRIP_WHITESPACE  (1<<3)


struct sv_s {
  /* field separator: '\t' or ',' */
  char field_sep;

  int line;
  
  /* row callback */
  void *callback_user_data;
  sv_fields_callback header_callback;
  sv_fields_callback data_callback;

  /* current buffer */
  char *buffer;
  /* size allocated */
  size_t size;
  /* size used */
  size_t len;

  unsigned int fields_count;
  char **fields;
  size_t *fields_widths;

  /* memory buffer used for constructing fields for user;
   * array above 'fields' points into this 
   */
  char* fields_buffer;
  size_t fields_buffer_size;

  /* first row is saved as headers */
  char **headers;
  size_t *headers_widths;

  unsigned int flags;

  /* error state */
  sv_status_t status;

  int bad_records;

  char last_char;

  char quote_char;

  /* called with the line (before parsing) */
  sv_line_callback line_callback;
};


/**
 * sv_new:
 * @user_data: user data to use for callbacks
 * @header_callback: callback to receive headers (or NULL)
 * @data_callback: callback to receive data rows (or NULL)
 * @field_sep: field separator ',' or '\t'
 *
 * Constructor - create an SV object
 *
 * Return value: new SV object or NULL on failure.
 */
sv*
sv_new(void *user_data, sv_fields_callback header_callback,
       sv_fields_callback data_callback,
       char field_sep)
{
  sv *t;

  if(field_sep != '\t' && field_sep != ',')
    return NULL;
  
  t = (sv*)malloc(sizeof(*t));
  if(!t)
    return NULL;
  
  t->field_sep = field_sep;

  t->line = 1;
  
  t->callback_user_data = user_data;
  t->header_callback = header_callback;
  t->data_callback = data_callback;

  t->buffer = NULL;
  t->size = 0;
  t->len = 0;

  t->fields_count = 0;
  t->fields = NULL;
  t->fields_widths = NULL;

  t->fields_buffer = NULL;  
  t->fields_buffer_size = 0;
  
  t->headers = NULL;
  t->headers_widths = NULL;

  /* default flags */
  t->flags = SV_FLAGS_SAVE_HEADER | SV_FLAGS_QUOTED_FIELDS;

  t->status = SV_STATUS_OK;

  t->bad_records = 0;

  t->last_char = '\0';

  t->quote_char = '"';

  t->line_callback = NULL;

  return t;
}


static sv_status_t
sv_init_fields(sv *t) 
{
  t->fields = (char**)malloc(sizeof(char*) * (t->fields_count+1));
  if(!t->fields)
    goto failed;
    
  t->fields_widths = (size_t*)malloc(sizeof(size_t) * (t->fields_count+1));
  if(!t->fields_widths)
    goto failed;

  t->headers = (char**)malloc(sizeof(char*) * (t->fields_count+1));
  if(!t->headers)
    goto failed;
  
  t->headers_widths = (size_t*)malloc(sizeof(size_t) * (t->fields_count+1));
  if(!t->headers_widths)
    goto failed;

  return SV_STATUS_OK;
  

  failed:
  if(t->fields) {
    free(t->fields);
    t->fields = NULL;
  }

  if(t->fields_widths) {
    free(t->fields_widths);
    t->fields_widths = NULL;
  }
  
  if(t->headers) {
    free(t->headers);
    t->headers = NULL;
  }

  return SV_STATUS_NO_MEMORY;
}


/**
 * sv_free:
 * @t: SV object
 *
 * Destructor: destroy an SV object
 *
 */
void
sv_free(sv *t)
{
  if(!t)
    return;

  if(t->headers_widths)
    free(t->headers_widths);
  if(t->headers) {
    unsigned int i;
    
    for(i = 0; i < t->fields_count; i++)
      free(t->headers[i]);
    free(t->headers);
  }
  

  if(t->fields_buffer)
    free(t->fields_buffer);

  if(t->fields_widths)
    free(t->fields_widths);
  if(t->fields)
    free(t->fields);
  if(t->buffer)
    free(t->buffer);
  
  free(t);
}



/* Ensure fields buffer is big enough for len bytes total */
static sv_status_t
sv_ensure_fields_buffer_size(sv *t, size_t len)
{
  char *nbuffer;
  size_t nsize;
  
  if(len < t->fields_buffer_size)
    return SV_STATUS_OK;
  
  nsize = len + 8;

#if defined(SV_DEBUG) && SV_DEBUG > 1
  fprintf(stderr, "%d: Growing buffer from %d to %d bytes\n",
          t->line, (int)t->fields_buffer_size, (int)nsize);
#endif
  
  nbuffer = (char*)malloc(nsize + 1);
  if(!nbuffer)
    return SV_STATUS_NO_MEMORY;

  if(t->fields_buffer)
    free(t->fields_buffer);
  
  t->fields_buffer = nbuffer;
  t->fields_buffer_size = nsize;

  return SV_STATUS_OK;
}



/* Ensure internal buffer is big enough for len more bytes */
static sv_status_t
sv_ensure_line_buffer_size(sv *t, size_t len)
{
  char *nbuffer;
  size_t nsize;
  
  if(t->len + len < t->size)
    return SV_STATUS_OK;
  
  nsize = (len + t->len) << 1;
    
  nbuffer = (char*)malloc(nsize + 1);
  if(!nbuffer)
    return SV_STATUS_NO_MEMORY;

  if(t->len)
    memcpy(nbuffer, t->buffer, t->len);
  nbuffer[t->len] = '\0';
  
  if(t->buffer)
    free(t->buffer);
  
  t->buffer = nbuffer;
  t->size = nsize;

  return SV_STATUS_OK;
}


/**
 * sv_get_line:
 * @t: sv object
 *
 * Get current SV line number
 *
 * Return value: line number or <0 on failure
 */
int
sv_get_line(sv *t)
{
  if(!t)
    return -1;

  return t->line;
}


/**
 * sv_get_header:
 * @t: sv object
 * @i: header index 0
 * @width_p: pointer to store width (or NULL)
 *
 * Get an SV header with optional width
 *
 * Return value: shared pointer to header or NULL if out of range
 */
const char*
sv_get_header(sv *t, unsigned int i, size_t *width_p)
{
  if(!t || !t->headers || i > t->fields_count)
    return NULL;

  if(width_p)
    *width_p = t->headers_widths[i];
  
  return (const char*)t->headers[i];
}


#if defined(SV_DEBUG) && SV_DEBUG > 1
static void
sv_dump_buffer(FILE* fh, const char* label, const char* buffer, size_t len) 
{
  size_t mylen=len;
  
  fprintf(fh, "%s (%zu bytes) >>>", label, len);
  if(mylen > 100)
    mylen = 100;
  fwrite(buffer, 1, mylen, fh);
  if(mylen != len)
    fputs("...", fh);
  fputs("<<<\n", fh);
}
#endif


static sv_status_t
sv_parse_line(sv *t, char *line, size_t len,  unsigned int* field_count_p)
{
  unsigned int column;
  int field_width = 0;
  int field_offset = 0;
  char* current_field = NULL;
  char* p = NULL;
  char** fields = t->fields;
  size_t* fields_widths = t->fields_widths;
  sv_status_t status;
  int field_is_quoted = 0;

#if defined(SV_DEBUG) && SV_DEBUG > 1
  if(fields)
    sv_dump_buffer(stderr, "(sv_parse_line): Parsing line", line, len);
#endif
  
  status = sv_ensure_fields_buffer_size(t, len);
  if(status)
    return status;

  if(fields) {
    current_field = t->fields_buffer;
    p = current_field;

    if(!p)
      return SV_STATUS_OK;
  }

  for(column = 0; 1; column++) {
    int c = -1;
    int field_ended = 0;
    int expect_sep = 0;

    if(column == len) {
      field_ended = 1;
      goto do_last;
    }
    
    c = line[column];

    if(t->flags & SV_FLAGS_QUOTED_FIELDS) {
      if(c == t->quote_char) {
        if(!field_width && !field_is_quoted) {
          field_is_quoted = 1;
  #if defined(SV_DEBUG) && SV_DEBUG > 1
          fprintf(stderr, "Field is quoted\n");
  #endif
          continue;
        } else if(column < len && line[column+1] == t->quote_char) {
  #if defined(SV_DEBUG) && SV_DEBUG > 1
          fprintf(stderr, "Doubled quote %c absorbed\n", t->quote_char);
  #endif
          column++;
          /* skip repeated quote - so it just replaces ""... with " */
          goto skip;
        } else if(column == len-1 || line[column+1] == t->field_sep) {
  #if defined(SV_DEBUG) && SV_DEBUG > 1
          fprintf(stderr, "Field ended on quote + sep\n");
  #endif
          field_ended = 1;
          expect_sep = 1;
          goto do_last;
        }
      }
    }

    if(!field_is_quoted && c == t->field_sep) {
#if defined(SV_DEBUG) && SV_DEBUG > 1
      fprintf(stderr, "Field ended on sep\n");
#endif
      field_ended = 1;
    }

    do_last:
    if(field_ended) {
      if(p)
        *p++ = '\0';
      
      if(fields) {

        if(t->flags & SV_FLAGS_STRIP_WHITESPACE) {
          /* Remove whitespace around a field */
          while(field_width > 0 && isspace(current_field[0])) {
            current_field++;
            field_width--;
          }

          while(field_width > 0 && isspace(current_field[field_width - 1]))
            field_width--;

          current_field[field_width] = '\0';
        }

        if(expect_sep)
          column++;

      }

#if defined(SV_DEBUG) && SV_DEBUG > 1
      if(fields) {
        fprintf(stderr, "  Field %d: %s (%d)\n", (int)field_offset, current_field, (int)field_width);
      }
#endif
      if(fields)
        fields[field_offset] = current_field;
      if(fields_widths)
        fields_widths[field_offset] = field_width;

      /* end loop when out of columns */
      if(column == len)
        break;
      
      /* otherwise got a tab so reset for next field */
      field_width = 0;
      field_is_quoted = 0;

      field_offset++;
      current_field = p;

      continue;
    }

    skip:
    if(fields)
      *p++ = c;
    field_width++;
  }


  if(field_count_p)
    *field_count_p = field_offset + 1;

  return SV_STATUS_OK;
}


static sv_status_t
sv_parse_chunk_line(sv* t, size_t line_len, int has_nl)
{
  size_t move_len = line_len;
  sv_status_t status = SV_STATUS_OK;
  unsigned int fields_count = 0;

  if(!line_len)
    goto skip_line;

  if(t->line_callback) {
    char c = t->buffer[line_len];
      
    t->buffer[line_len] = '\0';
    status = t->line_callback(t, t->callback_user_data, t->buffer, line_len);
    t->buffer[line_len] = c;
    if(status != SV_STATUS_OK)
      return status;
  }

  if(!t->fields_count) {
    /* First line in the file - calculate number of fields */
    status = sv_parse_line(t, t->buffer, line_len, &t->fields_count);
    if(status)
      return status;

    /* initialise arrays of size t->fields_count */
    status = sv_init_fields(t);
    if(status)
      return status;
  }

  status = sv_parse_line(t, t->buffer, line_len, &fields_count);
  if(status)
    return status;

  if(fields_count != t->fields_count) {
    t->bad_records++;
    if(t->flags & SV_FLAGS_BAD_DATA_ERROR) {
#if defined(SV_DEBUG) && SV_DEBUG > 1
      fprintf(stderr, "Error in line %d: saw %d fields expected %d\n",
              t->line, fields_count, t->fields_count);
#endif
      status = SV_STATUS_LINE_FIELDS;
      return status;
    }
#if defined(SV_DEBUG) && SV_DEBUG > 1
    fprintf(stderr, "Ignoring line %d: saw %d fields expected %d\n",
            t->line, fields_count, t->fields_count);
#endif
    /* Otherwise skip the line */
    goto skip_line;
  }

  if(t->line == 1 && (t->flags & SV_FLAGS_SAVE_HEADER)) {
    /* first line and header: turn fields into headers */
    unsigned int i;
      
    for(i = 0; i < t->fields_count; i++) {
      char *s = (char*)malloc(t->fields_widths[i]+1);
      if(!s) {
        status = SV_STATUS_NO_MEMORY;
        break;
      }
      memcpy(s, t->fields[i], t->fields_widths[i]+1);
      t->headers[i] = s;
      t->headers_widths[i] = t->fields_widths[i];
    }

    if(status == SV_STATUS_OK && t->header_callback) {
      /* got header fields - return them to user */
      status = t->header_callback(t, t->callback_user_data, t->headers, 
                                  t->headers_widths, t->fields_count);
    }
  } else {
    /* data */

    if(t->data_callback) {
      /* got data fields - return them to user */
      status = t->data_callback(t, t->callback_user_data, t->fields, 
                                t->fields_widths, t->fields_count);
    }
  }

  skip_line:

  if(has_nl)
    move_len++;

  /* adjust buffer - remove 'line_len+1' bytes from start of buffer */
  t->len -= move_len;

  /* this is an overlapping move */
  memmove(t->buffer, &t->buffer[move_len], t->len);

  /* This is not needed: guaranteed above */
  /* t->buffer[t->len] = '\0' */

  t->line++;

  return status;
}


/**
 * sv_parse_chunk:
 * @t: sv object
 * @buffer: buffer to parse (or NULL)
 * @len: length of @buffer (or 0)
 *
 * Parse a chunk of data
 *
 * Parsing ends if either @buffer is NULL or @len is 0
 *
 * Return value: #SV_STATUS_OK on success
 */
sv_status_t
sv_parse_chunk(sv *t, char *buffer, size_t len)
{
  size_t offset = 0;
  sv_status_t status = SV_STATUS_OK;
  /* End of input if either of these is NULL */
  int is_end = (!buffer || !len);

  if(!is_end) {
    /* add new data to existing buffer */
    status = sv_ensure_line_buffer_size(t, len);
    if(status)
      return status;

    /* add new buffer */
    memcpy(t->buffer + t->len, buffer, len);

    /* always ensure it is NUL terminated even if input chunk was not */
    t->len += len;
    t->buffer[t->len] = '\0';
  }

  /* look for an end of line to do some work */
  for(offset = 0; offset < t->len; offset++) {
    char c = t->buffer[offset];

    /* skip \n when just seen \r - i.e. \r\n or CR LF */
    if(t->last_char == '\r' && c == '\n') {
#if defined(SV_DEBUG) && SV_DEBUG > 1
      fprintf(stderr, "Skipping a \\n after \\r\n");
#endif

      /* adjust buffer */
      t->len -= 1;

      /* this is an overlapping move */
      memmove(t->buffer, &t->buffer[1], t->len);

      t->last_char = '\0';
      continue;
    }

    if(c != '\r' && c != '\n')
      continue;

    t->last_char = c;

#if defined(SV_DEBUG) && SV_DEBUG > 1
    sv_dump_buffer(stderr, "Starting buffer", t->buffer, t->len);
#endif

    /* found a line */
    status = sv_parse_chunk_line(t, offset, 1);
    if(status != SV_STATUS_OK)
      break;

    offset = -1; /* so for loop starts at 0 */
  }

  if(is_end && status == SV_STATUS_OK) {
    /* If end of input and there is a non-empty buffer left, try to
     * parse it all as the last line.  It will NOT contain newlines.
     */
    if(t->len)
      status = sv_parse_chunk_line(t, t->len, 0);
  }

  return status;
}


static sv_status_t
sv_set_option_vararg(sv* t, sv_option_t option, va_list arg)
{
  sv_status_t status = SV_STATUS_OK;

  switch(option) {
    case SV_OPTION_SAVE_HEADER:
      t->flags &= ~SV_FLAGS_SAVE_HEADER;
      if(va_arg(arg, long))
        t->flags |= SV_FLAGS_SAVE_HEADER;
      break;

    case SV_OPTION_BAD_DATA_ERROR:
      t->flags &= ~SV_FLAGS_BAD_DATA_ERROR;
      if(va_arg(arg, long))
        t->flags |= SV_FLAGS_BAD_DATA_ERROR;
      break;

    case SV_OPTION_QUOTED_FIELDS:
      t->flags &= ~SV_FLAGS_QUOTED_FIELDS;
      if(va_arg(arg, long))
        t->flags |= SV_FLAGS_QUOTED_FIELDS;
      break;

    case SV_OPTION_STRIP_WHITESPACE:
      t->flags &= ~SV_FLAGS_STRIP_WHITESPACE;
      if(va_arg(arg, long))
        t->flags |= SV_FLAGS_STRIP_WHITESPACE;
      break;

    case SV_OPTION_QUOTE_CHAR:
      if(1) {
        int c = va_arg(arg, int);
        if(c != t->field_sep)
          t->quote_char = c;
      }
      break;

    case SV_OPTION_LINE_CALLBACK:
      if(1) {
        sv_line_callback cb = (sv_line_callback)va_arg(arg, void*);
        t->line_callback = cb;
      }

    default:
    case SV_OPTION_NONE:
      status = SV_STATUS_FAILED;
      break;

  }

  return status;
}
  

/**
 * sv_set_option:
 * @t: sv object
 * @option: option name
 *
 * Set an option value.  The value varies in type dependent on the @option
 *
 * Return value: #SV_STATUS_FAILED if failed
 */
sv_status_t
sv_set_option(sv *t, sv_option_t option, ...)
{
  sv_status_t status;
  va_list arg;

  va_start(arg, option);
  status = sv_set_option_vararg(t, option, arg);
  va_end(arg);

  return status;
}
