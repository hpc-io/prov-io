/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * sv.h - Header for libsv
 *
 * Copyright (C) 2009-2013, David Beckett http://www.dajobe.org/
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


/**
 * sv_status_t:
 * @SV_STATUS_OK: OK
 * @SV_STATUS_FAILED: Failure
 * @SV_STATUS_NO_MEMORY: Out of memory
 * @SV_STATUS_LINE_FIELDS: Line had wrong number of fields
 *
 * Status / errors
*/
typedef enum {
  SV_STATUS_OK = 0,
  SV_STATUS_FAILED,
  SV_STATUS_NO_MEMORY,
  SV_STATUS_LINE_FIELDS
} sv_status_t;

typedef struct sv_s sv;

/**
 * @sv_fields_callback: 
 * @t: sv object
 * @user_data: user data
 * @fields: array of fields
 * @widths: array of field widths
 * @count: size of @fields and @widths
 *
 * Callback function for sv_new() header_callback and data_callback
 *
 * Return value: #SV_STATUS_OK or error code
 */
typedef sv_status_t (*sv_fields_callback)(sv *t, void *user_data, char** fields, size_t *widths, size_t count);

/**
 * @sv_line_callback: 
 * @t: sv object
 * @user_data: user data
 * @line: line buffer
 * @length: size of @line
 *
 * Callback function for lines set via sv_set_option() with #SV_OPTION_LINE_CALLBACK
 *
 * Return value: #SV_STATUS_OK or error code
 */
typedef sv_status_t (*sv_line_callback)(sv *t, void *user_data, const char* line, size_t length);


/**
 * sv_option_t:
 * 
 * @SV_OPTION_NONE: internal
 * @SV_OPTION_SAVE_HEADER: save header boolean; type long
 * @SV_OPTION_BAD_DATA_ERROR: bad dad is error boolean; type long
 * @SV_OPTION_QUOTED_FIELDS: fields are quoted boolean; type long
 * @SV_OPTION_STRIP_WHITESPACE: strip whitespace around fields boolean; type long
 * @SV_OPTION_QUOTE_CHAR: set field quote char; type int (char)
 * @SV_OPTION_LINE_CALLBACK: Set line callback of type #sv_line_callback
 *
 * Option type
 */
typedef enum {
  SV_OPTION_NONE = 0,
  SV_OPTION_SAVE_HEADER,
  SV_OPTION_BAD_DATA_ERROR,
  SV_OPTION_QUOTED_FIELDS,
  SV_OPTION_STRIP_WHITESPACE,
  SV_OPTION_QUOTE_CHAR,
  SV_OPTION_LINE_CALLBACK
} sv_option_t;

sv* sv_new(void *user_data, sv_fields_callback header_callback, sv_fields_callback data_callback, char field_sep);
void sv_free(sv *t);

sv_status_t sv_set_option(sv *t, sv_option_t option, ...);

int sv_get_line(sv *t);
const char* sv_get_header(sv *t, unsigned int i, size_t *width_p);

sv_status_t sv_parse_chunk(sv *t, char *buffer, size_t len);

