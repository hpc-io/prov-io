/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_SPARQL_PARSER_SPARQL_PARSER_TAB_H_INCLUDED
# define YY_SPARQL_PARSER_SPARQL_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int sparql_parser_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SELECT = 258,
    FROM = 259,
    WHERE = 260,
    OPTIONAL = 261,
    DESCRIBE = 262,
    CONSTRUCT = 263,
    ASK = 264,
    DISTINCT = 265,
    REDUCED = 266,
    LIMIT = 267,
    UNION = 268,
    PREFIX = 269,
    BASE = 270,
    BOUND = 271,
    GRAPH = 272,
    NAMED = 273,
    FILTER = 274,
    OFFSET = 275,
    ORDER = 276,
    BY = 277,
    REGEX = 278,
    ASC = 279,
    DESC = 280,
    LANGMATCHES = 281,
    A = 282,
    STRLANG = 283,
    STRDT = 284,
    STR = 285,
    IRI = 286,
    URI = 287,
    BNODE = 288,
    LANG = 289,
    DATATYPE = 290,
    ISURI = 291,
    ISBLANK = 292,
    ISLITERAL = 293,
    ISNUMERIC = 294,
    SAMETERM = 295,
    GROUP = 296,
    HAVING = 297,
    COUNT = 298,
    SUM = 299,
    AVG = 300,
    MIN = 301,
    MAX = 302,
    GROUP_CONCAT = 303,
    SAMPLE = 304,
    SEPARATOR = 305,
    DELETE = 306,
    INSERT = 307,
    WITH = 308,
    CLEAR = 309,
    CREATE = 310,
    SILENT = 311,
    DATA = 312,
    DROP = 313,
    LOAD = 314,
    INTO = 315,
    DEFAULT = 316,
    TO = 317,
    ADD = 318,
    MOVE = 319,
    COPY = 320,
    ALL = 321,
    COALESCE = 322,
    AS = 323,
    IF = 324,
    NOT = 325,
    IN = 326,
    BINDINGS = 327,
    UNDEF = 328,
    SERVICE = 329,
    MINUS = 330,
    YEAR = 331,
    MONTH = 332,
    DAY = 333,
    HOURS = 334,
    MINUTES = 335,
    SECONDS = 336,
    TIMEZONE = 337,
    TZ = 338,
    STRLEN = 339,
    SUBSTR = 340,
    UCASE = 341,
    LCASE = 342,
    STRSTARTS = 343,
    STRENDS = 344,
    CONTAINS = 345,
    ENCODE_FOR_URI = 346,
    CONCAT = 347,
    STRBEFORE = 348,
    STRAFTER = 349,
    REPLACE = 350,
    BIND = 351,
    ABS = 352,
    ROUND = 353,
    CEIL = 354,
    FLOOR = 355,
    RAND = 356,
    MD5 = 357,
    SHA1 = 358,
    SHA224 = 359,
    SHA256 = 360,
    SHA384 = 361,
    SHA512 = 362,
    UUID = 363,
    STRUUID = 364,
    VALUES = 365,
    EXPLAIN = 366,
    LET = 367,
    CURRENT_DATETIME = 368,
    NOW = 369,
    FROM_UNIXTIME = 370,
    TO_UNIXTIME = 371,
    HATHAT = 372,
    SC_OR = 373,
    SC_AND = 374,
    EQ = 375,
    NEQ = 376,
    LT = 377,
    GT = 378,
    LE = 379,
    GE = 380,
    ASSIGN = 381,
    STRING = 382,
    LANGTAG = 383,
    DOUBLE_LITERAL = 384,
    DOUBLE_POSITIVE_LITERAL = 385,
    DOUBLE_NEGATIVE_LITERAL = 386,
    INTEGER_LITERAL = 387,
    INTEGER_POSITIVE_LITERAL = 388,
    INTEGER_NEGATIVE_LITERAL = 389,
    DECIMAL_LITERAL = 390,
    DECIMAL_POSITIVE_LITERAL = 391,
    DECIMAL_NEGATIVE_LITERAL = 392,
    BOOLEAN_LITERAL = 393,
    URI_LITERAL = 394,
    URI_LITERAL_BRACE = 395,
    QNAME_LITERAL = 396,
    QNAME_LITERAL_BRACE = 397,
    BLANK_LITERAL = 398,
    IDENTIFIER = 399
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 196 "./sparql_parser.y" /* yacc.c:1909  */

  raptor_sequence *seq;
  rasqal_variable *variable;
  rasqal_literal *literal;
  rasqal_triple *triple;
  rasqal_expression *expr;
  rasqal_graph_pattern *graph_pattern;
  double floating;
  raptor_uri *uri;
  unsigned char *name;
  rasqal_formula *formula;
  rasqal_update_operation *update;
  unsigned int uinteger;
  rasqal_data_graph* data_graph;
  rasqal_row* row;
  rasqal_solution_modifier* modifier;
  int limit_offset[2];
  int integer;
  rasqal_projection* projection;
  rasqal_bindings* bindings;
  sparql_uri_applies* uri_applies;
  sparql_op_expr* op_expr;

#line 223 "sparql_parser.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int sparql_parser_parse (rasqal_query* rq, void* yyscanner);

#endif /* !YY_SPARQL_PARSER_SPARQL_PARSER_TAB_H_INCLUDED  */
