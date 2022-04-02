/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         sparql_parser_parse
#define yylex           sparql_parser_lex
#define yyerror         sparql_parser_error
#define yydebug         sparql_parser_debug
#define yynerrs         sparql_parser_nerrs


/* Copy the first part of user declarations.  */
#line 28 "./sparql_parser.y" /* yacc.c:339  */

#ifdef HAVE_CONFIG_H
#include <rasqal_config.h>
#endif

#ifdef WIN32
#include <win32_rasqal_config.h>
#endif

#include <stdio.h>
#include <stdarg.h>

#include <rasqal.h>
#include <rasqal_internal.h>

#include <sparql_parser.h>

#define YY_NO_UNISTD_H 1
#include <sparql_lexer.h>

#include <sparql_common.h>


/* Set RASQAL_DEBUG to 3 for super verbose parsing - watching the shift/reduces */
#if 0
#undef RASQAL_DEBUG
#define RASQAL_DEBUG 3
#endif


#define DEBUG_FH stderr

/* Make verbose error messages for syntax errors */
#define YYERROR_VERBOSE 1

/* Fail with an debug error message if RASQAL_DEBUG > 1 */
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1
#define YYERROR_MSG(msg) do { fputs("** YYERROR ", DEBUG_FH); fputs(msg, DEBUG_FH); fputc('\n', DEBUG_FH); YYERROR; } while(0)
#else
#define YYERROR_MSG(ignore) YYERROR
#endif
#define YYERR_MSG_GOTO(label,msg) do { errmsg = msg; goto label; } while(0)

/* Slow down the grammar operation and watch it work */
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 2
#undef YYDEBUG
#define YYDEBUG 1
#endif

/* the lexer does not seem to track this */
#undef RASQAL_SPARQL_USE_ERROR_COLUMNS

/* Prototypes */ 
int sparql_parser_error(rasqal_query* rq, void* scanner, const char *msg);

/* Make lex/yacc interface as small as possible */
#undef yylex
#define yylex sparql_lexer_lex

/* Make the yyerror below use the rdf_parser */
#undef yyerror
#define yyerror(rq, scanner, message) sparql_query_error(rq, message)

/* Prototypes for local functions */
static int sparql_parse(rasqal_query* rq);
static void sparql_query_error(rasqal_query* rq, const char *message);
static void sparql_query_error_full(rasqal_query *rq, const char *message, ...) RASQAL_PRINTF_FORMAT(2, 3);


static sparql_uri_applies*
new_uri_applies(raptor_uri* uri, rasqal_update_graph_applies applies) 
{
  sparql_uri_applies* ua;

  ua = RASQAL_MALLOC(sparql_uri_applies*, sizeof(*ua));
  if(!ua)
    return NULL;
  
  ua->uri = uri;
  ua->applies = applies;

  return ua;
}


static void
free_uri_applies(sparql_uri_applies* ua)
{
  if(ua->uri)
    raptor_free_uri(ua->uri);
  RASQAL_FREE(sparql_uri_applies*, ua);
}



static sparql_op_expr*
new_op_expr(rasqal_op op, rasqal_expression *expr)
{
  sparql_op_expr* oe;

  oe = RASQAL_MALLOC(sparql_op_expr*, sizeof(*oe));
  if(!oe)
    return NULL;
  
  oe->op = op;
  oe->expr = expr;

  return oe;
}


static void
free_op_expr(sparql_op_expr* oe)
{
  if(oe->expr)
    rasqal_free_expression(oe->expr);
  RASQAL_FREE(sparql_op_expr*, oe);
}

static void
print_op_expr(sparql_op_expr* oe, FILE* fh)
{
  fputs("<op ", fh);
  fputs(rasqal_expression_op_label(oe->op), fh);
  fputs(", ", fh);
  if(oe->expr)
    rasqal_expression_print(oe->expr, fh);
  else
    fputs("NULL", fh);
  fputs(">", fh);
}




#line 208 "sparql_parser.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "sparql_parser.tab.h".  */
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
#line 196 "./sparql_parser.y" /* yacc.c:355  */

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

#line 417 "sparql_parser.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int sparql_parser_parse (rasqal_query* rq, void* yyscanner);

#endif /* !YY_SPARQL_PARSER_SPARQL_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 431 "sparql_parser.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2245

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  161
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  164
/* YYNRULES -- Number of rules.  */
#define YYNRULES  405
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  827

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   399

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   160,     2,     2,   125,     2,     2,     2,
     118,   119,   137,   135,   117,   136,   159,   138,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   158,
       2,     2,     2,   124,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   120,     2,   121,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   122,     2,   123,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   564,   564,   565,   570,   579,   593,   600,   620,   625,
     630,   638,   642,   643,   644,   648,   652,   656,   660,   664,
     668,   672,   676,   680,   684,   692,   700,   706,   713,   743,
     750,   768,   783,   788,   793,   803,   807,   817,   826,   835,
     853,   857,   899,   903,   907,   911,   915,   919,   923,   930,
     944,   950,   954,   962,   983,  1004,  1025,  1046,  1067,  1074,
    1083,  1092,  1108,  1136,  1158,  1181,  1226,  1250,  1267,  1276,
    1285,  1301,  1322,  1326,  1334,  1342,  1362,  1394,  1414,  1458,
    1475,  1508,  1523,  1527,  1535,  1550,  1559,  1579,  1609,  1629,
    1673,  1715,  1757,  1781,  1785,  1789,  1793,  1797,  1810,  1842,
    1877,  1882,  1889,  1916,  1951,  1987,  1996,  2015,  2019,  2026,
    2030,  2038,  2050,  2077,  2123,  2154,  2185,  2216,  2222,  2231,
    2236,  2243,  2267,  2290,  2298,  2302,  2310,  2315,  2322,  2336,
    2346,  2366,  2371,  2378,  2382,  2386,  2413,  2427,  2441,  2448,
    2455,  2465,  2485,  2499,  2506,  2511,  2516,  2521,  2527,  2535,
    2540,  2547,  2557,  2577,  2584,  2591,  2599,  2616,  2624,  2636,
    2650,  2663,  2668,  2675,  2680,  2686,  2695,  2714,  2719,  2728,
    2740,  2762,  2790,  2797,  2811,  2834,  2840,  2846,  2854,  2872,
    2876,  2880,  2884,  2888,  2900,  2904,  2915,  2973,  2987,  2999,
    3036,  3041,  3053,  3101,  3114,  3142,  3143,  3148,  3179,  3187,
    3191,  3195,  3199,  3203,  3207,  3211,  3215,  3223,  3262,  3312,
    3329,  3350,  3357,  3361,  3369,  3378,  3383,  3398,  3414,  3424,
    3434,  3449,  3456,  3466,  3491,  3512,  3520,  3524,  3528,  3535,
    3540,  3547,  3586,  3620,  3648,  3655,  3665,  3682,  3690,  3698,
    3703,  3714,  3744,  3759,  3803,  3851,  3949,  3954,  3961,  3966,
    3973,  4051,  4056,  4063,  4071,  4081,  4109,  4113,  4121,  4188,
    4307,  4360,  4397,  4401,  4409,  4421,  4434,  4440,  4448,  4452,
    4459,  4471,  4475,  4479,  4489,  4493,  4497,  4501,  4505,  4509,
    4519,  4527,  4534,  4542,  4550,  4559,  4566,  4573,  4580,  4587,
    4594,  4601,  4606,  4611,  4653,  4679,  4686,  4700,  4706,  4725,
    4744,  4765,  4788,  4802,  4807,  4825,  4848,  4854,  4882,  4905,
    4911,  4922,  4938,  4944,  4955,  4962,  4966,  4973,  4987,  4991,
    4995,  5004,  5010,  5020,  5028,  5036,  5043,  5050,  5057,  5064,
    5079,  5086,  5093,  5100,  5107,  5114,  5121,  5128,  5135,  5142,
    5149,  5156,  5163,  5170,  5177,  5184,  5191,  5198,  5202,  5206,
    5213,  5220,  5227,  5234,  5241,  5248,  5255,  5262,  5266,  5270,
    5277,  5284,  5291,  5298,  5305,  5312,  5319,  5326,  5333,  5340,
    5347,  5354,  5361,  5368,  5379,  5386,  5397,  5404,  5411,  5418,
    5425,  5432,  5439,  5446,  5457,  5474,  5492,  5510,  5535,  5541,
    5561,  5565,  5569,  5576,  5580,  5584,  5592,  5596,  5600,  5608,
    5612,  5616,  5628,  5634,  5654,  5660
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SELECT", "FROM", "WHERE", "OPTIONAL",
  "DESCRIBE", "CONSTRUCT", "ASK", "DISTINCT", "REDUCED", "LIMIT", "UNION",
  "PREFIX", "BASE", "BOUND", "GRAPH", "NAMED", "FILTER", "OFFSET", "ORDER",
  "BY", "REGEX", "ASC", "DESC", "LANGMATCHES", "\"a\"", "\"strlang\"",
  "\"strdt\"", "\"str\"", "\"iri\"", "\"uri\"", "\"bnode\"", "\"lang\"",
  "\"datatype\"", "\"isUri\"", "\"isBlank\"", "\"isLiteral\"",
  "\"isNumeric\"", "\"sameTerm\"", "GROUP", "HAVING", "COUNT", "SUM",
  "AVG", "MIN", "MAX", "GROUP_CONCAT", "SAMPLE", "SEPARATOR", "DELETE",
  "INSERT", "WITH", "CLEAR", "CREATE", "SILENT", "DATA", "DROP", "LOAD",
  "INTO", "DEFAULT", "TO", "ADD", "MOVE", "COPY", "ALL", "COALESCE", "AS",
  "IF", "NOT", "IN", "BINDINGS", "UNDEF", "SERVICE", "MINUS", "YEAR",
  "MONTH", "DAY", "HOURS", "MINUTES", "SECONDS", "TIMEZONE", "TZ",
  "STRLEN", "SUBSTR", "UCASE", "LCASE", "STRSTARTS", "STRENDS", "CONTAINS",
  "ENCODE_FOR_URI", "CONCAT", "STRBEFORE", "STRAFTER", "REPLACE", "BIND",
  "ABS", "ROUND", "CEIL", "FLOOR", "RAND", "MD5", "SHA1", "SHA224",
  "SHA256", "SHA384", "SHA512", "UUID", "STRUUID", "VALUES", "EXPLAIN",
  "LET", "CURRENT_DATETIME", "NOW", "FROM_UNIXTIME", "TO_UNIXTIME", "','",
  "'('", "')'", "'['", "']'", "'{'", "'}'", "'?'", "'$'", "\"^^\"",
  "SC_OR", "SC_AND", "EQ", "NEQ", "LT", "GT", "LE", "GE", "'+'", "'-'",
  "'*'", "'/'", "\":=\"", "\"string\"", "\"langtag\"",
  "\"double literal\"", "\"double positive literal\"",
  "\"double negative literal\"", "\"integer literal\"",
  "\"integer positive literal\"", "\"integer negative literal\"",
  "\"decimal literal\"", "\"decimal positive literal\"",
  "\"decimal negative literal\"", "\"boolean literal\"", "\"URI literal\"",
  "\"URI literal (\"", "\"QName literal\"", "\"QName literal (\"",
  "\"blank node literal\"", "\"identifier\"", "';'", "'.'", "'!'",
  "$accept", "Sparql", "Query", "ExplainOpt", "ReportFormat", "Update",
  "UpdateTailOpt", "UpdateOperation", "Prologue", "BaseDeclOpt",
  "PrefixDeclListOpt", "SelectQuery", "SubSelect", "SelectClause",
  "SelectExpressionList", "SelectExpressionListTail", "SelectTerm",
  "AggregateExpression", "DistinctOpt", "ExpressionOrStar",
  "CountAggregateExpression", "SumAggregateExpression",
  "AvgAggregateExpression", "MinAggregateExpression",
  "MaxAggregateExpression", "SeparatorOpt", "ExpressionList",
  "GroupConcatAggregateExpression", "SampleAggregateExpression",
  "ConstructQuery", "DescribeQuery", "VarOrIRIrefList", "AskQuery",
  "DatasetClause", "GraphRef", "DeleteQuery", "GraphTriples",
  "GraphTemplate", "ModifyTemplate", "ModifyTemplateList", "InsertQuery",
  "UpdateQuery", "GraphRefAll", "ClearQuery", "SilentOpt", "CreateQuery",
  "DropQuery", "IriRefList", "GraphOrDefault", "OldGraphRef", "LoadQuery",
  "AddQuery", "MoveQuery", "CopyQuery", "DatasetClauseList",
  "DatasetClauseListOpt", "DefaultGraphClause", "NamedGraphClause",
  "SourceSelector", "WhereClause", "WhereClauseOpt", "SolutionModifier",
  "GroupConditionList", "AsVarOpt", "GroupCondition", "GroupClauseOpt",
  "HavingCondition", "HavingConditionList", "HavingClauseOpt",
  "LimitOffsetClausesOpt", "OrderClauseOpt", "OrderConditionList",
  "OrderCondition", "LimitClause", "OffsetClause", "ValuesClauseOpt",
  "VarListOpt", "VarList", "DataBlockRowListOpt", "DataBlockRowList",
  "DataBlockRow", "DataBlockValueList", "RDFLiteral", "DataBlockValue",
  "GroupGraphPattern", "GroupGraphPatternSub", "TriplesBlockOpt",
  "GraphPatternListOpt", "GraphPatternList", "GraphPatternListFilter",
  "DotOptional", "TriplesBlock", "GraphPatternNotTriples",
  "OptionalGraphPattern", "GraphGraphPattern", "ServiceGraphPattern",
  "Bind", "InlineData", "DataBlock", "InlineDataOneVar",
  "DataBlockValueListOpt", "InlineDataFull", "InlineDataGraphPattern",
  "MinusGraphPattern", "GroupOrUnionGraphPattern",
  "GroupOrUnionGraphPatternList", "LetGraphPattern", "Filter",
  "Constraint", "ParamsOpt", "FunctionCall", "CoalesceExpression",
  "ArgList", "ArgListNoBraces", "ConstructTemplate", "ConstructTriplesOpt",
  "ConstructTriples", "TriplesSameSubject", "PropertyListNotEmpty",
  "PropertyListTailOpt", "PropertyList", "ObjectList", "ObjectTail",
  "Object", "Verb", "TriplesNode", "BlankNodePropertyList", "Collection",
  "GraphNodeListNotEmpty", "GraphNode", "VarOrTerm", "VarOrIRIref", "Var",
  "VarName", "VarOrBadVarName", "GraphTerm", "Expression",
  "ConditionalOrExpression", "ConditionalAndExpression",
  "RelationalExpression", "AdditiveExpression",
  "AdExOpExpressionListOuter", "AdExOpExpressionListInner",
  "AdExOpUnaryExpressionListOpt", "AdExOpUnaryExpressionList",
  "AdExOpUnaryExpression", "MultiplicativeExpression",
  "MuExOpUnaryExpressionList", "MuExOpUnaryExpression", "UnaryExpression",
  "PrimaryExpression", "BrackettedExpression", "BuiltInCall",
  "StringExpression", "RegexExpression", "DatetimeBuiltinAccessors",
  "DatetimeExtensions", "IRIrefBrace", "NumericLiteral",
  "NumericLiteralUnsigned", "NumericLiteralPositive",
  "NumericLiteralNegative", "IRIref", "BlankNode", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,    44,    40,    41,
      91,    93,   123,   125,    63,    36,   372,   373,   374,   375,
     376,   377,   378,   379,   380,    43,    45,    42,    47,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,    59,    46,
      33
};
# endif

#define YYPACT_NINF -595

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-595)))

#define YYTABLE_NINF -113

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     115,   -17,   157,  -595,  -595,   831,  -595,  -595,  -595,    74,
      66,    16,    25,   121,   121,   121,   121,   121,   121,  -595,
     131,     3,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,    75,    51,    82,    85,  1941,  -595,   128,    91,
    1941,   128,    92,  -595,   133,    54,   133,    64,   -23,   -23,
     -23,   222,   132,    43,   215,   116,  -595,   215,  -595,  -595,
    -595,   105,  -595,   103,   -40,  -595,  -595,  -595,  -595,  -595,
    -595,    97,  -595,  1976,   137,  2015,   201,   107,   107,   -20,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,   389,  -595,  -595,    95,    84,  -595,  -595,
      84,  -595,  -595,   141,  -595,  -595,  -595,  -595,  -595,    82,
    -595,  -595,  -595,  -595,  1976,   675,  -595,   165,     6,    -6,
    -595,  -595,  -595,  -595,  -595,   123,  -595,  -595,  -595,    33,
      -4,  -595,  -595,   216,   236,   253,   233,   233,  1374,  -595,
    -595,   156,  -595,  -595,   215,    73,  -595,  -595,  -595,  2089,
     215,   316,   215,    89,   238,  -595,    89,  -595,   723,   176,
    -595,   209,    89,   218,   472,  -595,   180,   192,   229,  -595,
     242,  -595,  -595,  2054,  -595,  -595,  -595,  -595,   239,  2089,
    -595,  -595,  -595,  -595,   -40,  -595,    89,  -595,  2089,  -595,
    -595,  -595,   -40,  -595,   246,    89,  1941,   255,  1941,  -595,
    -595,    29,  -595,   -23,   -23,   -23,  -595,  -595,   261,   268,
     274,   278,   280,   282,   284,   285,   287,   289,   290,   291,
     292,   294,   295,   296,   298,   310,   312,   314,   315,   317,
     318,   319,   320,   322,   324,   330,   332,   333,   336,   337,
     338,   341,   344,   345,   350,   352,   355,   365,   367,   368,
     369,   370,   372,   374,   376,   377,   379,   388,   390,   398,
     399,   400,   402,   404,   406,   408,   409,   410,   424,   426,
     951,   427,  1515,  1515,  -595,  -595,  1515,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,   481,
     423,   436,  -595,   619,   452,   181,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  1374,   434,   243,  -595,    89,   137,    89,
    -595,   443,  -595,   445,    89,  -595,   199,  -595,  -595,  -595,
     447,   529,  -595,  -595,   529,  -595,    82,   137,  1862,   121,
      82,   453,   238,   454,   561,   472,  -595,   414,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  2089,
     455,  -595,  2089,  -595,  -595,  -595,   417,   459,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,   712,  1976,   785,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,   199,  1374,  1374,  1374,  1374,
    1374,  1374,  1374,  1092,  1374,  1374,  1374,  1374,  1374,  1374,
    1374,   568,   568,   568,   568,   568,   568,   568,  1374,  -595,
    1374,  1374,  1374,  1374,  1374,  1374,  1374,  1374,  1374,  1374,
    1374,  1374,  1374,  1374,  1374,  1374,  1374,  1374,  1374,  1374,
    1374,  1374,  1374,  1374,  1374,   462,  1374,  1374,  1374,  1374,
    1374,  1374,   464,   470,   471,   473,  1374,  1374,   474,  -595,
    -595,  -595,   -58,  1374,  1374,   520,   319,  1374,  1374,  1374,
    1374,  1374,  1374,  1374,  1374,   452,  -595,   235,   235,  1374,
    1374,   181,  -595,    -1,  -595,   568,  -595,   529,  -595,   529,
    -595,  2089,   529,   478,   199,  -595,   560,   581,  -595,   562,
     116,  -595,    82,  1374,  -595,  -595,  -595,  -595,   434,   137,
    -595,  1374,  -595,   199,    82,  -595,  -595,  2089,  -595,  2089,
     482,    84,  -595,  2089,  -595,    40,   484,    89,   490,   493,
     494,   496,   497,   512,   513,   515,  -595,   516,   517,   518,
     519,   546,   553,   554,   498,  -595,  1233,  1374,  1374,  1374,
    1374,  1374,  1374,     7,   557,   556,   563,   564,   566,   567,
     569,   572,   575,   576,   570,   577,   578,   596,   598,   599,
     579,   146,  -595,   600,   601,   603,   602,   605,   606,   608,
    -595,   609,   611,   614,   615,   617,   618,  -595,  -595,  -595,
    -595,   620,   621,  -595,   107,   107,  -595,   622,   436,  -595,
     319,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  1374,  1374,  -595,   235,  -595,  -595,  -595,  -595,  -595,
    1374,  -595,  1374,  -595,  -595,   558,  -595,   616,  -595,  -595,
    -595,   560,  -595,  -595,   623,   141,  -595,  1759,  1862,   659,
    -595,  -595,    82,   654,   624,  -595,   729,  -595,   631,  -595,
    -595,  -595,   634,  -595,  -595,  -595,  -595,  1374,  1374,  1374,
    1374,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  1374,  -595,   625,  -595,   626,   638,   639,   640,   -33,
     641,  -595,  1374,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  1374,  -595,  -595,  1374,  1374,  1374,  -595,  1374,
    -595,  1374,  1374,  1374,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,   -31,   529,   643,  -595,  -595,  1374,
    1759,  -595,  -595,  -595,  -595,  -595,  1862,  -595,   742,    60,
    -595,   199,  1374,    82,  -595,  1941,   153,   660,   664,   665,
     666,  -595,  -595,  -595,  -595,  -595,   740,   672,  -595,   656,
     166,   673,   677,   678,  -595,   682,   684,   687,  -595,   686,
    -595,   275,   671,   643,  -595,   738,  -595,  -595,  1656,   662,
     663,  -595,   789,   798,   692,   694,  -595,  1886,  1374,  -595,
    -595,  -595,  -595,  -595,   685,  -595,  1374,  1374,  -595,  -595,
    -595,  -595,  -595,  -595,  1374,  -595,  -595,   411,  -595,  -595,
     199,   709,   715,   715,  1656,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,    89,   719,   699,   722,
     724,   175,  -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,
    -595,  -595,  -595,  1374,  -595,   725,  -595
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      27,     0,     0,     2,     3,     6,    29,    26,     1,     0,
       0,     0,   101,   101,   101,   101,   101,   101,   101,     5,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,     0,     0,     0,     0,   118,   127,     0,
       0,   127,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   120,   120,   162,     7,   120,     8,     9,
      10,    27,    11,     0,     0,   402,   403,    72,    73,   121,
     123,   188,    78,     0,     0,     0,     0,     0,     0,   175,
     395,   398,   401,   393,   396,   399,   394,   397,   400,   277,
     404,    83,    85,     0,   275,    82,   242,   249,   257,   256,
       0,   264,   265,   276,   390,   391,   392,   274,   278,     0,
     117,   126,    75,   125,     0,     0,    86,     0,     0,     0,
      95,    94,    96,    93,    98,     0,   102,   103,   104,   106,
       0,   107,   108,     0,     0,     0,     0,     0,     0,    36,
      34,    35,    39,    40,   120,   120,    70,   266,   267,   240,
     119,     0,   120,     0,     0,     4,     0,    12,     0,     0,
     122,     0,     0,     0,   191,   187,   198,     0,     0,    79,
       0,   279,   263,     0,   261,   262,   255,   405,     0,     0,
     254,   270,   268,   269,     0,   176,     0,    84,   240,   248,
     244,   243,     0,   124,     0,   127,     0,     0,     0,    97,
      74,     0,   105,     0,     0,     0,    32,    33,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   388,   389,     0,   323,    42,    43,
      44,    45,    46,    47,    48,   320,   348,   322,   321,     0,
     280,   282,   284,   293,   295,   309,   317,   318,   319,   347,
     357,   358,   359,   237,   274,     0,    37,   127,     0,   127,
      68,     0,   239,     0,     0,    71,   164,   161,   212,   213,
       0,   138,    28,   184,   138,   185,     0,     0,     0,   101,
       0,     0,     0,     0,   221,   186,   190,   196,   193,   200,
     202,   203,   205,   218,   206,   201,   199,   204,   194,   188,
       0,    77,     0,   259,   260,   258,   247,   252,   253,   177,
      76,   241,   178,    88,    87,     0,     0,     0,   111,   110,
     109,   113,   114,   115,   116,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    50,    50,    50,    50,    50,    50,   237,   233,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
     316,   314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   294,   297,   303,   303,     0,
       0,   308,   311,     0,   236,    50,    38,   138,    69,   138,
     238,     0,   138,     0,   163,   166,   216,     0,    30,   143,
     162,   207,     0,     0,   225,   228,   226,   227,     0,     0,
     219,     0,   211,     0,     0,   189,   195,   188,   197,     0,
       0,   249,   245,     0,   250,   127,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,   333,     0,     0,     0,
       0,     0,     0,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
     334,     0,     0,     0,     0,     0,     0,   345,   346,   384,
     385,     0,     0,   324,     0,     0,   273,     0,   281,   283,
       0,   291,   285,   286,   287,   288,   289,   290,   298,   299,
     296,     0,     0,   300,   302,   305,   301,   312,   313,   310,
       0,   232,   237,    67,    66,     0,    64,     0,   165,   183,
     182,   215,   180,   174,     0,   181,   179,     0,     0,   150,
      31,   208,     0,     0,     0,   223,   220,   192,     0,    81,
     246,   251,     0,    90,    92,    91,   329,     0,     0,     0,
       0,   325,   330,   331,   332,   326,   328,   353,   354,   355,
     356,     0,    52,     0,    51,     0,     0,     0,     0,    59,
       0,   234,     0,   376,   377,   378,   379,   380,   381,   382,
     383,   360,     0,   363,   364,     0,     0,     0,   368,     0,
     369,     0,     0,     0,   335,   338,   336,   337,   339,   340,
     341,   342,   343,   344,   386,   387,   271,   272,    41,   292,
     306,   307,   304,   235,   230,   138,   168,   173,   214,     0,
     137,   130,   134,   136,   133,   141,   142,   139,     0,   148,
     209,     0,     0,     0,    80,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,    57,     0,     0,    63,     0,
       0,     0,     0,     0,    60,     0,     0,     0,   229,     0,
      65,     0,     0,   167,   170,   132,   129,   140,     0,     0,
       0,   128,   146,   147,     0,     0,   222,     0,     0,   374,
     327,   350,   351,   352,     0,    62,     0,     0,   361,   365,
     366,   367,   370,   371,     0,   231,   172,     0,   217,   169,
       0,     0,     0,     0,   149,   152,   155,   156,   157,   158,
     159,   160,   144,   145,   210,   224,   127,     0,     0,     0,
       0,     0,   171,   131,   135,   153,   154,   151,    89,   375,
      58,   349,   362,     0,   372,     0,   373
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -595,  -595,  -595,  -595,  -595,   781,  -595,  -595,   784,  -595,
    -595,  -595,  -595,   775,   200,  -595,  -119,  -595,   -12,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,   334,  -595,  -595,  -595,
    -595,  -595,  -595,    50,    -9,  -595,  -107,  -595,   -88,   -26,
    -595,  -595,   801,  -595,    36,  -595,  -595,  -595,    26,  -595,
    -595,  -595,  -595,  -595,   366,   198,  -595,  -595,   786,  -127,
     -41,  -294,  -595,  -595,   138,  -595,   135,  -595,  -595,  -595,
    -595,  -595,    55,    90,   108,   387,  -595,  -595,  -595,  -595,
     118,   122,  -458,  -594,   -22,  -595,  -318,  -595,   534,  -595,
    -595,   -54,  -595,  -595,  -595,  -595,  -595,  -595,   540,  -595,
    -595,  -595,  -595,  -595,  -595,  -595,  -595,  -595,   547,  -595,
    -317,  -595,  -406,  -388,  -595,   693,  -140,   -50,     9,  -595,
     373,   384,  -595,  -595,  -595,   -72,  -595,  -595,  -595,   -18,
     -69,   -37,    22,   -76,  -595,   109,   240,  -595,   449,   444,
      53,  -595,   438,   439,  -595,   304,   -63,  -595,   440,  -357,
      38,  -312,  -315,  -595,  -595,  -595,  -595,  -595,  -442,  -595,
    -270,  -266,   -32,  -595
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    20,    55,     4,    62,    21,     5,     6,
      32,    56,   161,    57,   140,   141,   142,   277,   526,   653,
     278,   279,   280,   281,   282,   737,   551,   283,   284,    58,
      59,   145,    60,    37,   123,    22,   168,    91,    92,    93,
      23,    24,   124,    25,    44,    26,    27,   130,   133,   371,
      28,    29,    30,    31,   150,   151,    67,    68,    69,   111,
     112,   478,   710,   791,   711,   479,   715,   716,   619,   761,
     719,   794,   795,   762,   763,   155,   473,   474,   752,   753,
     754,   611,    94,   613,   113,   163,   164,   335,   336,   337,
     497,   165,   338,   339,   340,   341,   342,   343,   317,   318,
     614,   319,   344,   345,   346,   626,   347,   348,   717,   749,
     285,   286,   399,   463,   152,   311,    95,    96,   189,   502,
     190,   356,   504,   357,   179,    97,    98,    99,   173,   358,
     100,   180,   287,   182,   577,   288,   464,   290,   291,   292,
     293,   455,   456,   593,   594,   595,   294,   461,   462,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   103,   104,
     105,   106,   304,   108
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     116,    70,   183,   172,   107,   187,   175,   194,   107,   312,
     533,   485,    72,   487,   115,   146,   486,   707,   612,   169,
     148,   166,   306,   166,   457,   -99,   315,   187,   458,   321,
     480,   498,    70,  -112,   615,   324,   127,   170,   131,   107,
     581,   107,   148,   107,   148,   109,   125,    33,   312,    45,
      46,    47,    48,    49,    50,   199,   201,   174,   101,   360,
     169,   107,   101,   197,   166,   148,   574,   575,   148,    64,
      33,   125,   759,   143,   147,   134,   135,    33,    33,    34,
     760,    43,   107,   107,   679,   178,   600,   193,   110,    63,
     368,   110,   632,   101,   109,   101,   147,   101,   147,   181,
      51,   172,   597,   598,   175,   -13,   184,   172,   310,   191,
     175,   176,    65,   148,    66,   101,   600,   107,   601,   147,
       1,   185,   147,    39,   600,   736,   661,   748,   198,   132,
       1,    35,    33,   109,    51,     7,   101,   101,    52,    53,
      54,   107,   334,   117,   118,   102,   200,   107,   202,   102,
     119,   120,   359,   612,   364,   354,   107,     8,   143,   143,
     362,    61,    71,   143,   107,   149,   107,   147,    42,   615,
     365,   101,   367,   603,   699,   604,   320,    43,   606,   627,
     102,   369,   102,   -99,   102,   457,   466,   472,    40,   458,
     308,  -112,   370,   707,   121,   101,    36,    77,    78,   122,
     110,   101,   102,    65,    71,    66,   126,    73,    77,    78,
     101,    71,   500,   114,   704,    75,   129,    76,   101,    33,
     101,    77,    78,   102,   102,    65,   154,    66,   176,   372,
     373,   374,   136,   137,   700,   701,    65,    79,    66,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    65,
      71,    66,   153,    90,   188,   156,    77,    78,   102,   506,
     159,    77,    78,   679,   181,   680,   467,   192,   469,   144,
     768,   468,   769,   305,   138,   200,   148,   187,   203,   187,
      77,    78,   102,   777,    65,   778,    66,   196,   102,    65,
     482,    66,   823,   612,   824,   148,   488,   102,   204,   166,
     712,   485,   714,   487,   481,   102,   486,   102,   490,   615,
     439,   440,   169,   334,   441,   205,   166,   107,   459,   460,
     107,   313,   177,    77,    78,    77,    78,   143,   322,   612,
     147,   605,   323,   107,   107,   107,   206,   207,   475,   349,
     138,   325,   307,   309,   350,   615,    77,    78,   609,   147,
     314,   138,   351,    65,   320,    66,   316,    77,    78,   139,
     355,   138,    77,    78,   352,   489,   576,    77,    78,   363,
     139,   101,   591,   592,   101,    38,    41,   366,   289,   375,
     527,   528,   529,   530,   531,   532,   376,   101,   101,   101,
     588,   589,   377,   712,   786,   714,   378,   508,   379,   485,
     380,   487,   381,   382,   486,   383,    74,   384,   385,   386,
     387,   750,   388,   389,   390,    79,   391,    80,    81,    82,
      83,    84,    85,    86,    87,    88,   610,    65,   392,    66,
     393,   172,   394,   395,   175,   396,   397,   398,   400,   107,
     401,   796,   402,   799,   616,   628,   798,   166,   403,   166,
     404,   405,   622,   602,   406,   407,   408,   148,   102,   409,
     621,   102,   410,   411,   633,   107,   635,   107,   412,   148,
     413,   107,   625,   414,   102,   102,   102,   796,   326,   799,
     815,   816,   798,   415,   609,   416,   417,   418,   419,   327,
     420,   328,   421,   101,   422,   423,   608,   424,   696,   697,
     582,   583,   584,   585,   586,   587,   425,    75,   426,    76,
     438,   147,   186,    77,    78,   624,   427,   428,   429,   101,
     430,   101,   431,   147,   432,   101,   433,   434,   435,    79,
     812,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    65,   436,    66,   437,    90,   329,   330,   177,   442,
     443,    79,   465,    80,    81,    82,    83,    84,    85,    86,
      87,    88,   610,    65,   444,    66,   470,   471,   331,   476,
     477,   491,   493,   496,   494,   501,   503,   499,   525,   616,
     102,   560,   332,   567,   333,   488,   488,   453,   454,   568,
     569,   580,   570,   573,    71,    81,    82,   607,    84,    85,
     720,    87,    88,   617,   618,   629,   102,   634,   102,   636,
     637,   638,   102,   639,   640,   651,   509,   510,   511,   512,
     513,   514,   515,   517,   518,   519,   520,   521,   522,   523,
     524,   641,   642,   609,   643,   644,   645,   646,   647,   713,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   552,   553,   554,
     555,   556,   557,   558,   559,   648,   561,   562,   563,   564,
     565,   566,   649,   650,   662,   663,   571,   572,   488,   187,
     718,   705,   664,   665,   488,   666,   667,   672,   668,   445,
     446,   669,    74,   107,   670,   671,   673,   674,   678,   767,
      79,   766,    80,    81,    82,    83,    84,    85,    86,    87,
      88,   610,    65,   675,    66,   676,   677,   681,   682,   616,
     683,   684,   721,   438,   685,   686,   488,   687,   688,    74,
     689,   623,   713,   690,   691,   107,   692,   693,   706,   694,
     695,   698,   723,   764,   731,   732,   708,   101,   447,   448,
     449,   450,   451,   452,   724,   616,   725,   733,   734,   735,
     738,   751,   488,   722,   758,   818,   654,   655,   656,   657,
     658,   552,   660,   776,     9,    10,    11,    12,    13,   770,
     797,    14,    15,   771,   772,   773,    16,    17,    18,   101,
     774,   775,   779,    75,   788,    76,   780,   781,   195,    77,
      78,   782,    74,   783,   784,   785,   790,   800,   801,   760,
     759,   804,   813,   805,   808,    79,   797,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    65,   814,    66,
      75,    90,    76,   483,   102,   505,    77,    78,   819,   820,
     703,   821,   157,   822,   826,   158,   162,   128,   756,   817,
     160,   757,    79,   803,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    65,   659,    66,   620,    90,   495,
     802,   789,   492,   787,   630,   484,   102,   726,   727,   728,
     729,   361,     9,    10,    11,    12,    13,   631,   579,    14,
      15,   730,   578,   590,    16,    17,    18,   596,   702,     0,
       0,   599,   739,    75,     0,    76,     0,     0,   507,    77,
      78,     0,   740,     0,     0,   741,   742,   743,     0,   744,
       0,   745,   746,   747,     0,    79,     0,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    65,     0,    66,
       0,    90,    19,     0,     0,     0,     0,     0,     0,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   765,     0,     0,     0,     0,   208,     0,     0,
       0,     0,     0,     0,   209,     0,     0,   210,     0,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,     0,     0,   224,   225,   226,   227,   228,   229,
     230,     0,     0,     0,     0,     0,     0,     0,   807,     0,
       0,     0,     0,     0,     0,     0,   809,   810,   231,     0,
     232,     0,     0,     0,   811,     0,     0,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,     0,     0,   825,   266,   267,   268,   269,     0,   270,
     171,   271,     0,     0,     0,    77,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   272,   273,     0,     0,
       0,    79,     0,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    65,   274,    66,   275,    90,   208,     0,
       0,   276,     0,     0,     0,   209,     0,     0,   210,     0,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,     0,     0,   224,   225,   226,   227,   228,
     229,   230,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
       0,   232,     0,     0,     0,     0,     0,     0,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,     0,     0,     0,   266,   267,   268,   269,     0,
     270,   516,   271,     0,     0,     0,    77,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   272,   273,     0,
       0,     0,    79,     0,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    65,   274,    66,   275,    90,   208,
       0,     0,   276,     0,     0,     0,   209,     0,     0,   210,
       0,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,     0,     0,   224,   225,   226,   227,
     228,   229,   230,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     231,     0,   232,     0,     0,     0,     0,     0,     0,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,     0,     0,     0,   266,   267,   268,   269,
       0,   270,     0,   271,     0,     0,     0,    77,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   272,   273,
     652,     0,     0,    79,     0,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    65,   274,    66,   275,    90,
     208,     0,     0,   276,     0,     0,     0,   209,     0,     0,
     210,     0,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,     0,     0,   224,   225,   226,
     227,   228,   229,   230,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   231,     0,   232,     0,     0,     0,     0,     0,     0,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,     0,     0,     0,   266,   267,   268,
     269,     0,   270,     0,   271,     0,     0,     0,    77,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   272,
     273,     0,     0,     0,    79,     0,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    65,   274,    66,   275,
      90,   208,     0,     0,   276,     0,     0,     0,   209,     0,
       0,   210,     0,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,     0,     0,   224,   225,
     226,   227,   228,   229,   230,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   231,     0,   232,     0,     0,     0,     0,     0,
       0,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     0,     0,   266,   267,
     268,   269,     0,   270,     0,   271,     0,     0,     0,    77,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,     0,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    65,   274,    66,
     275,    90,   208,     0,     0,     0,     0,     0,     0,   209,
     792,   793,   210,     0,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   231,     0,   232,     0,     0,     0,     0,
       0,     0,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,     0,     0,     0,   266,
     267,   268,   269,     0,   483,   208,     0,     0,     0,     0,
      77,    78,   209,     0,     0,   210,     0,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,     0,     0,     0,    65,   274,
      66,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   231,     0,   232,     0,
       0,     0,     0,     0,     0,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,     0,
       0,     0,   266,   267,   268,   269,     0,   709,   208,     0,
       0,     0,     0,    77,    78,   209,     0,     0,   210,     0,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,    74,     0,     0,     0,     0,     0,     0,
       0,    65,   274,    66,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
       0,   232,     0,     0,     0,     0,     0,     0,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,    74,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,     0,     0,     0,   266,   267,   268,   269,     0,
     483,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,     0,    76,     0,     0,   806,
      77,    78,     0,     0,    65,   274,    66,   275,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    65,     0,
      66,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
       0,    76,     0,     0,     0,    77,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,     0,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    65,    75,    66,    76,    90,     0,     0,
      77,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    65,     0,
      66,     0,    90,    75,   171,    76,     0,     0,     0,    77,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,     0,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    65,     0,    66,
       0,    90,    75,   353,    76,     0,     0,     0,    77,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,     0,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    65,    75,    66,    76,
      90,     0,     0,    77,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
       0,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    65,     0,    66,     0,    90
};

static const yytype_int16 yycheck[] =
{
      41,    33,    78,    75,    36,    93,    75,   114,    40,   149,
     398,   328,    34,   328,    40,    52,   328,   611,   476,    73,
      52,    71,   141,    73,   294,     0,   153,   115,   294,   156,
     324,   349,    64,     0,   476,   162,    45,    74,    61,    71,
     446,    73,    74,    75,    76,     5,    17,     4,   188,    13,
      14,    15,    16,    17,    18,    61,    60,    75,    36,   186,
     114,    93,    40,    57,   114,    97,   124,   125,   100,    18,
       4,    17,    12,    51,    52,    49,    50,     4,     4,     5,
      20,    56,   114,   115,   117,    76,   117,   109,    38,    14,
      61,    41,    52,    71,     5,    73,    74,    75,    76,   157,
       3,   173,   459,   460,   173,     0,   126,   179,   145,   100,
     179,    27,   152,   145,   154,    93,   117,   149,   119,    97,
      15,   141,   100,    57,   117,   158,   119,   158,   122,   152,
      15,    57,     4,     5,     3,   152,   114,   115,     7,     8,
       9,   173,   164,    51,    52,    36,   152,   179,   152,    40,
      17,    18,   184,   611,   195,   173,   188,     0,   136,   137,
     192,   158,   122,   141,   196,   122,   198,   145,   152,   611,
     196,   149,   198,   467,   580,   469,   154,    56,   472,   497,
      71,   152,    73,   158,    75,   455,   305,   314,   122,   455,
     117,   158,   201,   787,    61,   173,   122,   124,   125,    66,
     150,   179,    93,   152,   122,   154,   152,   122,   124,   125,
     188,   122,   352,   122,   602,   118,   152,   120,   196,     4,
     198,   124,   125,   114,   115,   152,   110,   154,    27,   203,
     204,   205,    10,    11,   591,   592,   152,   140,   154,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     122,   154,    54,   156,   159,    57,   124,   125,   149,   366,
     157,   124,   125,   117,   157,   119,   307,   126,   309,   137,
     117,   308,   119,   117,   118,   152,   308,   365,    62,   367,
     124,   125,   173,   117,   152,   119,   154,   122,   179,   152,
     327,   154,   117,   751,   119,   327,   328,   188,    62,   349,
     617,   618,   617,   618,   326,   196,   618,   198,   330,   751,
     272,   273,   366,   335,   276,    62,   366,   349,   137,   138,
     352,     5,   121,   124,   125,   124,   125,   305,   152,   787,
     308,   471,   123,   365,   366,   367,   136,   137,   316,   159,
     118,   123,   144,   145,   152,   787,   124,   125,    73,   327,
     152,   118,   123,   152,   332,   154,   118,   124,   125,   137,
     121,   118,   124,   125,   122,   329,   442,   124,   125,   123,
     137,   349,   137,   138,   352,     9,    10,   122,   138,   118,
     392,   393,   394,   395,   396,   397,   118,   365,   366,   367,
     453,   454,   118,   710,   119,   710,   118,   375,   118,   716,
     118,   716,   118,   118,   716,   118,    17,   118,   118,   118,
     118,   705,   118,   118,   118,   140,   118,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   118,   154,
     118,   503,   118,   118,   503,   118,   118,   118,   118,   471,
     118,   758,   118,   758,   476,   499,   758,   497,   118,   499,
     118,   118,   489,   465,   118,   118,   118,   489,   349,   118,
     482,   352,   118,   118,   505,   497,   507,   499,   118,   501,
     118,   503,   494,   118,   365,   366,   367,   794,     6,   794,
     792,   793,   794,   118,    73,   118,   118,   118,   118,    17,
     118,    19,   118,   471,   118,   118,   474,   118,   574,   575,
     447,   448,   449,   450,   451,   452,   118,   118,   118,   120,
     270,   489,   123,   124,   125,   493,   118,   118,   118,   497,
     118,   499,   118,   501,   118,   503,   118,   118,   118,   140,
     119,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   118,   154,   118,   156,    74,    75,   121,    68,
     127,   140,   118,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   128,   154,   123,   122,    96,   122,
      41,   118,   118,   159,    13,   158,   117,   122,    10,   611,
     471,   119,   110,   119,   112,   617,   618,   135,   136,   119,
     119,    71,   119,   119,   122,   143,   144,   119,   146,   147,
     622,   149,   150,    22,    42,   123,   497,   123,   499,   119,
     117,   117,   503,   117,   117,   117,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   119,   119,    73,   119,   119,   119,   119,   119,   617,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   119,   426,   427,   428,   429,
     430,   431,   119,   119,   117,   119,   436,   437,   710,   767,
      21,   123,   119,   119,   716,   119,   119,   117,   119,    70,
      71,   119,    17,   725,   119,   119,   119,   119,   119,   725,
     140,   723,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   117,   154,   117,   117,   117,   117,   751,
     117,   119,    68,   483,   119,   119,   758,   119,   119,    17,
     119,   491,   710,   119,   119,   767,   119,   119,   122,   119,
     119,   119,    13,   721,   119,   119,   123,   725,   129,   130,
     131,   132,   133,   134,   123,   787,   122,   119,   119,   119,
     119,   118,   794,   139,    22,   806,   526,   527,   528,   529,
     530,   531,   532,   117,    51,    52,    53,    54,    55,   119,
     758,    58,    59,   119,   119,   119,    63,    64,    65,   767,
      50,   119,   119,   118,   123,   120,   119,   119,   123,   124,
     125,   119,    17,   119,   117,   119,    68,   145,   145,    20,
      12,   119,   790,   119,   129,   140,   794,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   119,   154,
     118,   156,   120,   118,   725,   123,   124,   125,   119,   140,
     600,   119,    61,   119,   119,    61,    71,    46,   710,   794,
      64,   716,   140,   763,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   531,   154,   480,   156,   335,
     762,   753,   332,   751,   501,   328,   767,   637,   638,   639,
     640,   188,    51,    52,    53,    54,    55,   503,   444,    58,
      59,   651,   443,   455,    63,    64,    65,   458,   594,    -1,
      -1,   461,   662,   118,    -1,   120,    -1,    -1,   123,   124,
     125,    -1,   672,    -1,    -1,   675,   676,   677,    -1,   679,
      -1,   681,   682,   683,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   111,    -1,    -1,    -1,    -1,    -1,    -1,   709,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   722,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    44,    45,    46,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   768,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   776,   777,    67,    -1,
      69,    -1,    -1,    -1,   784,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    -1,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,    -1,    -1,   823,   113,   114,   115,   116,    -1,   118,
     119,   120,    -1,    -1,    -1,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,   136,    -1,    -1,
      -1,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,    16,    -1,
      -1,   160,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    44,    45,    46,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,    -1,    -1,    -1,   113,   114,   115,   116,    -1,
     118,   119,   120,    -1,    -1,    -1,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,    -1,
      -1,    -1,   140,    -1,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,    16,
      -1,    -1,   160,    -1,    -1,    -1,    23,    -1,    -1,    26,
      -1,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    44,    45,    46,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    -1,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,    -1,    -1,    -1,   113,   114,   115,   116,
      -1,   118,    -1,   120,    -1,    -1,    -1,   124,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,
     137,    -1,    -1,   140,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
      16,    -1,    -1,   160,    -1,    -1,    -1,    23,    -1,    -1,
      26,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    44,    45,
      46,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    -1,    -1,    -1,   113,   114,   115,
     116,    -1,   118,    -1,   120,    -1,    -1,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,
     136,    -1,    -1,    -1,   140,    -1,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,    16,    -1,    -1,   160,    -1,    -1,    -1,    23,    -1,
      -1,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    44,
      45,    46,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    -1,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,    -1,    -1,    -1,   113,   114,
     115,   116,    -1,   118,    -1,   120,    -1,    -1,    -1,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    -1,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    -1,    -1,    -1,   113,
     114,   115,   116,    -1,   118,    16,    -1,    -1,    -1,    -1,
     124,   125,    23,    -1,    -1,    26,    -1,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,
     154,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    -1,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,    -1,
      -1,    -1,   113,   114,   115,   116,    -1,   118,    16,    -1,
      -1,    -1,    -1,   124,   125,    23,    -1,    -1,    26,    -1,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,   153,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    17,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,    -1,    -1,    -1,   113,   114,   115,   116,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
     124,   125,    -1,    -1,   152,   153,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
     154,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,   120,    -1,    -1,    -1,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   118,   154,   120,   156,    -1,    -1,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
     154,    -1,   156,   118,   119,   120,    -1,    -1,    -1,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,   154,
      -1,   156,   118,   119,   120,    -1,    -1,    -1,   124,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   118,   154,   120,
     156,    -1,    -1,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,    -1,   156
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,   162,   163,   166,   169,   170,   152,     0,    51,
      52,    53,    54,    55,    58,    59,    63,    64,    65,   111,
     164,   168,   196,   201,   202,   204,   206,   207,   211,   212,
     213,   214,   171,     4,     5,    57,   122,   194,   215,    57,
     122,   215,   152,    56,   205,   205,   205,   205,   205,   205,
     205,     3,     7,     8,     9,   165,   172,   174,   190,   191,
     193,   158,   167,    14,    18,   152,   154,   217,   218,   219,
     323,   122,   245,   122,    17,   118,   120,   124,   125,   140,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     156,   198,   199,   200,   243,   277,   278,   286,   287,   288,
     291,   293,   296,   319,   320,   321,   322,   323,   324,     5,
     194,   220,   221,   245,   122,   200,   221,    51,    52,    17,
      18,    61,    66,   195,   203,    17,   152,   195,   203,   152,
     208,    61,   152,   209,   209,   209,    10,    11,   118,   137,
     175,   176,   177,   293,   137,   192,   292,   293,   323,   122,
     215,   216,   275,   216,   110,   236,   216,   166,   169,   157,
     219,   173,   174,   246,   247,   252,   278,    17,   197,   252,
     292,   119,   286,   289,   290,   291,    27,   121,   279,   285,
     292,   157,   294,   294,   126,   141,   123,   199,   159,   279,
     281,   279,   126,   245,   197,   123,   122,    57,   122,    61,
     152,    60,   152,    62,    62,    62,   175,   175,    16,    23,
      26,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    43,    44,    45,    46,    47,    48,
      49,    67,    69,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   113,   114,   115,   116,
     118,   120,   135,   136,   153,   155,   160,   178,   181,   182,
     183,   184,   185,   188,   189,   271,   272,   293,   296,   297,
     298,   299,   300,   301,   307,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   323,   117,   177,   216,   117,   216,
     292,   276,   277,     5,   216,   220,   118,   259,   260,   262,
     293,   220,   152,   123,   220,   123,     6,    17,    19,    74,
      75,    96,   110,   112,   245,   248,   249,   250,   253,   254,
     255,   256,   257,   258,   263,   264,   265,   267,   268,   159,
     152,   123,   122,   119,   290,   121,   282,   284,   290,   323,
     220,   276,   323,   123,   221,   200,   122,   200,    61,   152,
     195,   210,   209,   209,   209,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   273,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   297,   311,
     311,   311,    68,   127,   128,    70,    71,   129,   130,   131,
     132,   133,   134,   135,   136,   302,   303,   321,   322,   137,
     138,   308,   309,   274,   297,   118,   177,   221,   292,   221,
     123,   122,   220,   237,   238,   293,   122,    41,   222,   226,
     222,   245,   292,   118,   269,   271,   312,   313,   323,   205,
     245,   118,   259,   118,    13,   249,   159,   251,   247,   122,
     277,   158,   280,   117,   283,   123,   197,   123,   293,   297,
     297,   297,   297,   297,   297,   297,   119,   297,   297,   297,
     297,   297,   297,   297,   297,    10,   179,   179,   179,   179,
     179,   179,   179,   274,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   187,   297,   297,   297,   297,   297,   297,   297,   297,
     119,   297,   297,   297,   297,   297,   297,   119,   119,   119,
     119,   297,   297,   119,   124,   125,   294,   295,   299,   300,
      71,   273,   301,   301,   301,   301,   301,   301,   307,   307,
     303,   137,   138,   304,   305,   306,   304,   310,   310,   309,
     117,   119,   179,   222,   222,   277,   222,   119,   293,    73,
     151,   242,   243,   244,   261,   319,   323,    22,    42,   229,
     236,   245,   292,   297,   293,   245,   266,   247,   252,   123,
     281,   282,    52,   221,   123,   221,   119,   117,   117,   117,
     117,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   117,   137,   180,   297,   297,   297,   297,   297,   187,
     297,   119,   117,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   117,   119,   119,   117,   117,   117,   119,   117,
     119,   117,   117,   117,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   294,   294,   119,   273,
     310,   310,   306,   297,   274,   123,   122,   244,   123,   118,
     223,   225,   271,   293,   313,   227,   228,   269,    21,   231,
     245,    68,   139,    13,   123,   122,   297,   297,   297,   297,
     297,   119,   119,   119,   119,   119,   158,   186,   119,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   158,   270,
     222,   118,   239,   240,   241,   297,   225,   227,    22,    12,
      20,   230,   234,   235,   293,   297,   245,   200,   117,   119,
     119,   119,   119,   119,    50,   119,   117,   117,   119,   119,
     119,   119,   119,   119,   117,   119,   119,   242,   123,   241,
      68,   224,    24,    25,   232,   233,   271,   293,   312,   313,
     145,   145,   235,   234,   119,   119,   123,   297,   129,   297,
     297,   297,   119,   293,   119,   312,   312,   233,   221,   119,
     140,   119,   119,   117,   119,   297,   119
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   161,   162,   162,   163,   164,   164,   165,   165,   165,
     165,   166,   167,   167,   167,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   169,   170,   170,   171,   171,
     172,   173,   174,   174,   174,   175,   175,   176,   176,   176,
     177,   177,   178,   178,   178,   178,   178,   178,   178,   179,
     179,   180,   180,   181,   182,   183,   184,   185,   186,   186,
     187,   187,   188,   189,   190,   190,   191,   191,   192,   192,
     192,   193,   194,   194,   195,   196,   196,   196,   196,   197,
     197,   198,   199,   199,   200,   200,   201,   201,   201,   202,
     202,   202,   202,   203,   203,   203,   203,   203,   204,   204,
     205,   205,   206,   206,   207,   208,   208,   209,   209,   210,
     210,   210,   211,   211,   212,   213,   214,   215,   215,   216,
     216,   217,   218,   219,   220,   220,   221,   221,   222,   223,
     223,   224,   224,   225,   225,   225,   225,   226,   226,   227,
     228,   228,   229,   229,   230,   230,   230,   230,   230,   231,
     231,   232,   232,   233,   233,   233,   233,   233,   233,   234,
     235,   236,   236,   237,   237,   238,   238,   239,   239,   240,
     240,   241,   241,   242,   242,   243,   243,   243,   243,   244,
     244,   244,   244,   244,   245,   245,   246,   247,   247,   248,
     248,   248,   249,   250,   250,   251,   251,   252,   252,   253,
     253,   253,   253,   253,   253,   253,   253,   254,   255,   256,
     257,   258,   259,   259,   260,   261,   261,   262,   263,   264,
     265,   265,   266,   266,   267,   268,   269,   269,   269,   270,
     270,   271,   271,   272,   273,   274,   274,   274,   275,   276,
     276,   277,   277,   278,   278,   279,   280,   280,   281,   281,
     282,   283,   283,   284,   285,   285,   286,   286,   287,   288,
     289,   289,   290,   290,   291,   291,   292,   292,   293,   293,
     294,   295,   295,   295,   296,   296,   296,   296,   296,   296,
     297,   298,   298,   299,   299,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   301,   301,   302,   302,   303,   303,
     303,   303,   304,   304,   305,   305,   306,   306,   307,   307,
     308,   308,   309,   309,   310,   310,   310,   310,   311,   311,
     311,   311,   311,   311,   312,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   315,   315,   316,   316,   316,   316,
     316,   316,   316,   316,   317,   317,   317,   317,   318,   318,
     319,   319,   319,   320,   320,   320,   321,   321,   321,   322,
     322,   322,   323,   323,   324,   324
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     4,     1,     0,     1,     1,     1,
       1,     3,     2,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     0,     4,     0,
       4,     4,     3,     3,     2,     1,     1,     2,     3,     1,
       1,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     5,     5,     5,     5,     5,     4,     0,
       3,     1,     6,     5,     5,     7,     5,     5,     2,     3,
       1,     3,     2,     2,     2,     3,     5,     5,     3,     1,
       5,     5,     1,     1,     2,     1,     3,     5,     5,    11,
       7,     7,     7,     1,     1,     1,     1,     2,     3,     1,
       1,     0,     3,     3,     3,     2,     1,     1,     1,     1,
       1,     1,     3,     5,     5,     5,     5,     2,     1,     1,
       0,     1,     2,     1,     2,     1,     1,     0,     4,     2,
       1,     2,     0,     1,     1,     4,     1,     3,     0,     1,
       2,     1,     2,     0,     2,     2,     1,     1,     0,     3,
       0,     2,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     2,     0,     1,     0,     2,     1,     1,     0,     2,
       1,     3,     2,     2,     1,     1,     2,     3,     3,     1,
       1,     1,     1,     1,     3,     3,     2,     1,     0,     2,
       1,     0,     3,     1,     1,     1,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     4,
       6,     2,     1,     1,     4,     1,     0,     6,     1,     2,
       3,     1,     3,     1,     6,     2,     1,     1,     1,     1,
       0,     6,     3,     2,     3,     3,     1,     0,     3,     1,
       0,     3,     1,     2,     2,     3,     2,     0,     1,     0,
       2,     2,     0,     1,     1,     1,     1,     1,     3,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       1,     3,     1,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     4,     1,     2,     1,     2,     1,     2,     2,
       2,     2,     1,     0,     2,     1,     2,     2,     2,     1,
       2,     1,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     4,     6,     4,     4,
       4,     4,     4,     3,     3,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     3,     3,     1,     1,     8,
       6,     6,     6,     4,     4,     4,     4,     1,     1,     1,
       4,     6,     8,     4,     4,     6,     6,     6,     4,     4,
       6,     6,     8,    10,     6,     8,     4,     4,     4,     4,
       4,     4,     4,     4,     3,     3,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (rq, yyscanner, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, rq, yyscanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, rasqal_query* rq, void* yyscanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (rq);
  YYUSE (yyscanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, rasqal_query* rq, void* yyscanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, rq, yyscanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, rasqal_query* rq, void* yyscanner)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              , rq, yyscanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, rq, yyscanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
      default: yyformat = YY_("syntax error");
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, rasqal_query* rq, void* yyscanner)
{
  YYUSE (yyvaluep);
  YYUSE (rq);
  YYUSE (yyscanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 140: /* "string"  */
#line 429 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).name))
    RASQAL_FREE(char*, ((*yyvaluep).name));
}
#line 2199 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 141: /* "langtag"  */
#line 429 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).name))
    RASQAL_FREE(char*, ((*yyvaluep).name));
}
#line 2208 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 142: /* "double literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2217 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 143: /* "double positive literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2226 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 144: /* "double negative literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2235 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 145: /* "integer literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2244 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 146: /* "integer positive literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2253 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 147: /* "integer negative literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2262 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 148: /* "decimal literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2271 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 149: /* "decimal positive literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2280 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 150: /* "decimal negative literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2289 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 151: /* "boolean literal"  */
#line 408 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2298 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 152: /* "URI literal"  */
#line 418 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).uri))
    raptor_free_uri(((*yyvaluep).uri));
}
#line 2307 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 153: /* "URI literal ("  */
#line 418 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).uri))
    raptor_free_uri(((*yyvaluep).uri));
}
#line 2316 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 154: /* "QName literal"  */
#line 429 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).name))
    RASQAL_FREE(char*, ((*yyvaluep).name));
}
#line 2325 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 155: /* "QName literal ("  */
#line 429 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).name))
    RASQAL_FREE(char*, ((*yyvaluep).name));
}
#line 2334 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 156: /* "blank node literal"  */
#line 429 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).name))
    RASQAL_FREE(char*, ((*yyvaluep).name));
}
#line 2343 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 157: /* "identifier"  */
#line 429 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).name))
    RASQAL_FREE(char*, ((*yyvaluep).name));
}
#line 2352 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 172: /* SelectQuery  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2361 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 173: /* SubSelect  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2370 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 174: /* SelectClause  */
#line 535 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).projection))
    rasqal_free_projection(((*yyvaluep).projection));
}
#line 2379 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 175: /* SelectExpressionList  */
#line 535 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).projection))
    rasqal_free_projection(((*yyvaluep).projection));
}
#line 2388 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 176: /* SelectExpressionListTail  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2397 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 177: /* SelectTerm  */
#line 511 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).variable))
    rasqal_free_variable(((*yyvaluep).variable));
}
#line 2406 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 178: /* AggregateExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2415 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 180: /* ExpressionOrStar  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2424 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 181: /* CountAggregateExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2433 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 182: /* SumAggregateExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2442 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 183: /* AvgAggregateExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2451 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 184: /* MinAggregateExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2460 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 185: /* MaxAggregateExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2469 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 186: /* SeparatorOpt  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2478 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 188: /* GroupConcatAggregateExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2487 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 189: /* SampleAggregateExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2496 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 190: /* ConstructQuery  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2505 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 191: /* DescribeQuery  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2514 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 192: /* VarOrIRIrefList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2523 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 194: /* DatasetClause  */
#line 517 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).data_graph))
    rasqal_free_data_graph(((*yyvaluep).data_graph));
}
#line 2532 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 197: /* GraphTriples  */
#line 453 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).update))
    rasqal_free_update_operation(((*yyvaluep).update));
}
#line 2541 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 198: /* GraphTemplate  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2550 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 199: /* ModifyTemplate  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2559 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 200: /* ModifyTemplateList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2568 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 203: /* GraphRefAll  */
#line 424 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).uri_applies))
    free_uri_applies(((*yyvaluep).uri_applies));
}
#line 2577 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 208: /* IriRefList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2586 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 215: /* DatasetClauseList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2595 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 216: /* DatasetClauseListOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2604 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 217: /* DefaultGraphClause  */
#line 517 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).data_graph))
    rasqal_free_data_graph(((*yyvaluep).data_graph));
}
#line 2613 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 218: /* NamedGraphClause  */
#line 517 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).data_graph))
    rasqal_free_data_graph(((*yyvaluep).data_graph));
}
#line 2622 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 219: /* SourceSelector  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2631 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 222: /* SolutionModifier  */
#line 529 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).modifier))
    rasqal_free_solution_modifier(((*yyvaluep).modifier));
}
#line 2640 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 223: /* GroupConditionList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2649 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 224: /* AsVarOpt  */
#line 511 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).variable))
    rasqal_free_variable(((*yyvaluep).variable));
}
#line 2658 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 225: /* GroupCondition  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2667 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 226: /* GroupClauseOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2676 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 227: /* HavingCondition  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2685 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 228: /* HavingConditionList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2694 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 229: /* HavingClauseOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2703 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 231: /* OrderClauseOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2712 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 232: /* OrderConditionList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2721 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 233: /* OrderCondition  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 2730 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 236: /* ValuesClauseOpt  */
#line 542 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).bindings))
    rasqal_free_bindings(((*yyvaluep).bindings));
}
#line 2739 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 237: /* VarListOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2748 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 238: /* VarList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2757 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 239: /* DataBlockRowListOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2766 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 240: /* DataBlockRowList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2775 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 241: /* DataBlockRow  */
#line 523 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).row))
    rasqal_free_row(((*yyvaluep).row));
}
#line 2784 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 242: /* DataBlockValueList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2793 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 243: /* RDFLiteral  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2802 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 244: /* DataBlockValue  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 2811 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 245: /* GroupGraphPattern  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2820 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 246: /* GroupGraphPatternSub  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2829 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 247: /* TriplesBlockOpt  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 2838 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 248: /* GraphPatternListOpt  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2847 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 249: /* GraphPatternList  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2856 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 250: /* GraphPatternListFilter  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2865 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 252: /* TriplesBlock  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 2874 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 253: /* GraphPatternNotTriples  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2883 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 254: /* OptionalGraphPattern  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2892 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 255: /* GraphGraphPattern  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2901 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 256: /* ServiceGraphPattern  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2910 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 257: /* Bind  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2919 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 258: /* InlineData  */
#line 542 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).bindings))
    rasqal_free_bindings(((*yyvaluep).bindings));
}
#line 2928 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 259: /* DataBlock  */
#line 542 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).bindings))
    rasqal_free_bindings(((*yyvaluep).bindings));
}
#line 2937 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 260: /* InlineDataOneVar  */
#line 542 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).bindings))
    rasqal_free_bindings(((*yyvaluep).bindings));
}
#line 2946 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 261: /* DataBlockValueListOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 2955 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 262: /* InlineDataFull  */
#line 542 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).bindings))
    rasqal_free_bindings(((*yyvaluep).bindings));
}
#line 2964 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 263: /* InlineDataGraphPattern  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2973 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 264: /* MinusGraphPattern  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2982 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 265: /* GroupOrUnionGraphPattern  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 2991 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 266: /* GroupOrUnionGraphPatternList  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 3000 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 267: /* LetGraphPattern  */
#line 470 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).graph_pattern))
    rasqal_free_graph_pattern(((*yyvaluep).graph_pattern));
}
#line 3009 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 268: /* Filter  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3018 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 269: /* Constraint  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3027 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 270: /* ParamsOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3036 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 271: /* FunctionCall  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3045 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 272: /* CoalesceExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3054 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 273: /* ArgList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3063 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 274: /* ArgListNoBraces  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3072 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 275: /* ConstructTemplate  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3081 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 276: /* ConstructTriplesOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3090 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 277: /* ConstructTriples  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3099 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 278: /* TriplesSameSubject  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3108 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 279: /* PropertyListNotEmpty  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3117 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 280: /* PropertyListTailOpt  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3126 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 281: /* PropertyList  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3135 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 282: /* ObjectList  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3144 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 283: /* ObjectTail  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3153 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 284: /* Object  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3162 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 285: /* Verb  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3171 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 286: /* TriplesNode  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3180 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 287: /* BlankNodePropertyList  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3189 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 288: /* Collection  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3198 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 289: /* GraphNodeListNotEmpty  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3207 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 290: /* GraphNode  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3216 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 291: /* VarOrTerm  */
#line 459 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).formula))
    rasqal_free_formula(((*yyvaluep).formula));
}
#line 3225 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 292: /* VarOrIRIref  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 3234 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 293: /* Var  */
#line 511 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).variable))
    rasqal_free_variable(((*yyvaluep).variable));
}
#line 3243 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 294: /* VarName  */
#line 511 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).variable))
    rasqal_free_variable(((*yyvaluep).variable));
}
#line 3252 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 296: /* GraphTerm  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 3261 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 297: /* Expression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3270 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 298: /* ConditionalOrExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3279 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 299: /* ConditionalAndExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3288 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 300: /* RelationalExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3297 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 301: /* AdditiveExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3306 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 302: /* AdExOpExpressionListOuter  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3315 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 303: /* AdExOpExpressionListInner  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3324 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 304: /* AdExOpUnaryExpressionListOpt  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3333 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 305: /* AdExOpUnaryExpressionList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3342 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 306: /* AdExOpUnaryExpression  */
#line 548 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).op_expr))
    free_op_expr(((*yyvaluep).op_expr));
}
#line 3351 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 307: /* MultiplicativeExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3360 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 308: /* MuExOpUnaryExpressionList  */
#line 435 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).seq))
    raptor_free_sequence(((*yyvaluep).seq));
}
#line 3369 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 309: /* MuExOpUnaryExpression  */
#line 548 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).op_expr))
    free_op_expr(((*yyvaluep).op_expr));
}
#line 3378 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 310: /* UnaryExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3387 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 311: /* PrimaryExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3396 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 312: /* BrackettedExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3405 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 313: /* BuiltInCall  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3414 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 315: /* RegexExpression  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3423 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 316: /* DatetimeBuiltinAccessors  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3432 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 317: /* DatetimeExtensions  */
#line 483 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).expr))
    rasqal_free_expression(((*yyvaluep).expr));
}
#line 3441 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 318: /* IRIrefBrace  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 3450 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 319: /* NumericLiteral  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 3459 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 320: /* NumericLiteralUnsigned  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 3468 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 321: /* NumericLiteralPositive  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 3477 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 322: /* NumericLiteralNegative  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 3486 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 323: /* IRIref  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 3495 "sparql_parser.c" /* yacc.c:1257  */
        break;

    case 324: /* BlankNode  */
#line 500 "./sparql_parser.y" /* yacc.c:1257  */
      {
  if(((*yyvaluep).literal))
    rasqal_free_literal(((*yyvaluep).literal));
}
#line 3504 "sparql_parser.c" /* yacc.c:1257  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (rasqal_query* rq, void* yyscanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, yyscanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 571 "./sparql_parser.y" /* yacc.c:1646  */
    {
  if((yyvsp[0].bindings))
    rq->bindings = (yyvsp[0].bindings);
}
#line 3773 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 580 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(sparql->experimental)
    rq->explain = 1;
  else {
    sparql_syntax_error(rq,
                        "EXPLAIN can only used with LAQRS");
    YYERROR;
  }
}
#line 3790 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 593 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* nothing to do */
}
#line 3798 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 601 "./sparql_parser.y" /* yacc.c:1646  */
    {
  raptor_sequence* seq;
  rasqal_graph_pattern* where_gp;

  /* Query graph pattern is first GP inside sequence of sub-GPs */
  seq = rasqal_graph_pattern_get_sub_graph_pattern_sequence((yyvsp[0].graph_pattern));
  where_gp = (rasqal_graph_pattern*)raptor_sequence_delete_at(seq, 0);

  rasqal_query_store_select_query(rq,
                                  (yyvsp[0].graph_pattern)->projection,
                                  (yyvsp[0].graph_pattern)->data_graphs,
                                  where_gp,
                                  (yyvsp[0].graph_pattern)->modifier);
  (yyvsp[0].graph_pattern)->projection = NULL;
  (yyvsp[0].graph_pattern)->data_graphs = NULL;
  (yyvsp[0].graph_pattern)->modifier = NULL;

  rasqal_free_graph_pattern((yyvsp[0].graph_pattern));
}
#line 3822 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 621 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->constructs = (yyvsp[0].seq);
  rq->verb = RASQAL_QUERY_VERB_CONSTRUCT;
}
#line 3831 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 626 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->describes = (yyvsp[0].seq);
  rq->verb = RASQAL_QUERY_VERB_DESCRIBE;
}
#line 3840 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 631 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_ASK;
}
#line 3848 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 649 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_DELETE;
}
#line 3856 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 653 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_INSERT;
}
#line 3864 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 657 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_UPDATE;
}
#line 3872 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 661 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_UPDATE;
}
#line 3880 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 665 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_UPDATE;
}
#line 3888 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 669 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_UPDATE;
}
#line 3896 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 673 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_UPDATE;
}
#line 3904 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 677 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_UPDATE;
}
#line 3912 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 681 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_UPDATE;
}
#line 3920 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 685 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rq->verb = RASQAL_QUERY_VERB_UPDATE;
}
#line 3928 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 693 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* nothing to do */
}
#line 3936 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 701 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_query_set_base_uri(rq, (yyvsp[0].uri));
  rasqal_evaluation_context_set_base_uri(rq->eval_context, (yyvsp[0].uri));
}
#line 3945 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 706 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* nothing to do */
}
#line 3953 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 714 "./sparql_parser.y" /* yacc.c:1646  */
    {
  raptor_sequence *seq = rq->prefixes;
  unsigned const char* prefix_string = (yyvsp[-1].name);
  size_t prefix_length = 0;

  if(prefix_string)
    prefix_length = strlen(RASQAL_GOOD_CAST(const char*, prefix_string));
  
  if(raptor_namespaces_find_namespace(rq->namespaces,
                                      prefix_string, RASQAL_BAD_CAST(int, prefix_length))) {
    /* A prefix may be defined only once */
    sparql_syntax_warning(rq,
                          "PREFIX %s can be defined only once.",
                          prefix_string ? RASQAL_GOOD_CAST(const char*, prefix_string) : ":");
    RASQAL_FREE(char*, prefix_string);
    raptor_free_uri((yyvsp[0].uri));
  } else {
    rasqal_prefix *p;
    p = rasqal_new_prefix(rq->world, prefix_string, (yyvsp[0].uri));
    if(!p)
      YYERROR_MSG("PrefixDeclOpt: failed to create new prefix");
    if(raptor_sequence_push(seq, p))
      YYERROR_MSG("PrefixDeclOpt: cannot push prefix to seq");
    if(rasqal_query_declare_prefix(rq, p)) {
      YYERROR_MSG("PrefixDeclOpt: cannot declare prefix");
    }
  }
}
#line 3986 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 743 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* nothing to do, rq->prefixes already initialised */
}
#line 3994 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 751 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.graph_pattern) = NULL;
  if(!sparql->sparql_scda) {
    sparql_syntax_error(rq,
                        "SELECT can only be used with a SPARQL query");
    YYERROR;
  } else {
    (yyval.graph_pattern) = rasqal_new_select_graph_pattern(rq,
                                         (yyvsp[-3].projection), (yyvsp[-2].seq), (yyvsp[-1].graph_pattern), (yyvsp[0].modifier), NULL);
  }
}
#line 4013 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 769 "./sparql_parser.y" /* yacc.c:1646  */
    {
  if((yyvsp[-3].projection) && (yyvsp[-2].graph_pattern) && (yyvsp[-1].modifier)) {
    (yyval.graph_pattern) = rasqal_new_select_graph_pattern(rq,
                                         (yyvsp[-3].projection),
                                         /* data graphs */ NULL,
                                         (yyvsp[-2].graph_pattern),
                                         (yyvsp[-1].modifier),
                                         (yyvsp[0].bindings));
  } else
    (yyval.graph_pattern) = NULL;
}
#line 4029 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 784 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.projection) = (yyvsp[0].projection);
  (yyval.projection)->distinct = 1;
}
#line 4038 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 789 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.projection) = (yyvsp[0].projection);
  (yyval.projection)->distinct = 2;
}
#line 4047 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 794 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.projection) = (yyvsp[0].projection);
}
#line 4055 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 804 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.projection) = rasqal_new_projection(rq, (yyvsp[0].seq), 0, 0);
}
#line 4063 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 808 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.projection) = rasqal_new_projection(rq, NULL, /* wildcard */ 1, 0);
}
#line 4071 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 818 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].variable))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("SelectExpressionListTail 1: sequence push failed");
  }
}
#line 4084 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 827 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-2].seq);
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].variable))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("SelectExpressionListTail 2: sequence push failed");
  }
}
#line 4097 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 836 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_variable,
                           (raptor_data_print_handler)rasqal_variable_print);
  if(!(yyval.seq))
    YYERROR_MSG("SelectExpressionListTail 3: failed to create sequence");
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].variable))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("SelectExpressionListTail 3: sequence push failed");
  }
}
#line 4113 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 854 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.variable) = (yyvsp[0].variable);
}
#line 4121 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 858 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.variable) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "SELECT ( expression ) AS Variable can only be used with SPARQL 1.1");
    YYERROR;
  } else if((yyvsp[-3].expr) && (yyvsp[-1].variable)) {
    if(rasqal_expression_mentions_variable((yyvsp[-3].expr), (yyvsp[-1].variable))) {
      sparql_query_error_full(rq,
                              "Expression in SELECT ( expression ) AS %s contains the variable name '%s'",
                              (yyvsp[-1].variable)->name, (yyvsp[-1].variable)->name);
      YYERROR;
    } else {
      (yyval.variable) = (yyvsp[-1].variable);
      (yyval.variable)->expression = (yyvsp[-3].expr);
    }

  }
}
#line 4148 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 900 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 4156 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 904 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 4164 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 908 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 4172 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 912 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 4180 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 46:
#line 916 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 4188 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 47:
#line 920 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 4196 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 48:
#line 924 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 4204 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 931 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "functions with DISTINCT can only be used with SPARQL 1.1");
    YYERROR;
  }
  
  (yyval.uinteger) = RASQAL_EXPR_FLAG_DISTINCT;
}
#line 4221 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 944 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.uinteger) = 0;
}
#line 4229 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 951 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 4237 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 955 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_0op_expression(rq->world,
                                 RASQAL_EXPR_VARSTAR);
}
#line 4246 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 963 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "COUNT() can only be used with SPARQL 1.1");
    YYERROR;
  } else {
    (yyval.expr) = rasqal_new_aggregate_function_expression(rq->world,
                                                  RASQAL_EXPR_COUNT, (yyvsp[-1].expr),
                                                  NULL /* params */, (yyvsp[-2].uinteger));
    if(!(yyval.expr))
      YYERROR_MSG("CountAggregateExpression: cannot create expr");
  }
}
#line 4268 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 984 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "SUM() can only be used with SPARQL 1.1");
    YYERROR;
  } else {
    (yyval.expr) = rasqal_new_aggregate_function_expression(rq->world,
                                                  RASQAL_EXPR_SUM, (yyvsp[-1].expr),
                                                  NULL /* params */, (yyvsp[-2].uinteger));
    if(!(yyval.expr))
      YYERROR_MSG("SumAggregateExpression: cannot create expr");
  }
}
#line 4290 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1005 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "AVG() can only be used with SPARQL 1.1");
    YYERROR;
  } else {
    (yyval.expr) = rasqal_new_aggregate_function_expression(rq->world,
                                                  RASQAL_EXPR_AVG, (yyvsp[-1].expr),
                                                  NULL /* params */, (yyvsp[-2].uinteger));
    if(!(yyval.expr))
      YYERROR_MSG("AvgAggregateExpression: cannot create expr");
  }
}
#line 4312 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1026 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "MIN() can only be used with SPARQL 1.1");
    YYERROR;
  } else {
    (yyval.expr) = rasqal_new_aggregate_function_expression(rq->world,
                                                  RASQAL_EXPR_MIN, (yyvsp[-1].expr),
                                                  NULL /* params */, (yyvsp[-2].uinteger));
    if(!(yyval.expr))
      YYERROR_MSG("MinAggregateExpression: cannot create expr");
  }
}
#line 4334 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1047 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "MAX() can only be used with SPARQL 1.1");
    YYERROR;
  } else {
    (yyval.expr) = rasqal_new_aggregate_function_expression(rq->world,
                                                  RASQAL_EXPR_MAX, (yyvsp[-1].expr),
                                                  NULL /* params */, (yyvsp[-2].uinteger));
    if(!(yyval.expr))
      YYERROR_MSG("MaxAggregateExpression: cannot create expr");
  }
}
#line 4356 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1068 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_string_literal(rq->world, (yyvsp[0].name), 
	                         NULL /* language */,
                                 NULL /* dt uri */, NULL /* dt_qname */);
}
#line 4366 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1074 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = NULL;
}
#line 4374 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1084 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-2].seq);
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("ExpressionList 1: sequence push failed");
  }
}
#line 4387 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1093 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_expression,
                           (raptor_data_print_handler)rasqal_expression_print);
  if(!(yyval.seq))
    YYERROR_MSG("ExpressionList 2: failed to create sequence");

  if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("ExpressionList 2: sequence push failed");
  }
}
#line 4404 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1109 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "GROUP_CONCAT() can only be used with SPARQL 1.1");
    YYERROR;
  } else {
    int flags = 0;
    
    if((yyvsp[-3].uinteger))
      flags |= RASQAL_EXPR_FLAG_DISTINCT;

    (yyval.expr) = rasqal_new_group_concat_expression(rq->world,
                                            flags /* flags */,
                                            (yyvsp[-2].seq) /* args */,
                                            (yyvsp[-1].literal) /* separator */);
    if(!(yyval.expr))
      YYERROR_MSG("GroupConcatAggregateExpression: cannot create expr");
  }
}
#line 4433 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1137 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "SAMPLE() can only be used with SPARQL 1.1");
    YYERROR;
  } else {
    (yyval.expr) = rasqal_new_aggregate_function_expression(rq->world,
                                                  RASQAL_EXPR_SAMPLE, (yyvsp[-1].expr),
                                                  NULL /* params */, (yyvsp[-2].uinteger));
    if(!(yyval.expr))
      YYERROR_MSG("SampleAggregateExpression: cannot create expr");
  }
}
#line 4455 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1160 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.seq) = NULL;
  if(!sparql->sparql_scda) {
    sparql_syntax_error(rq,
                        "CONSTRUCT can only be used with a SPARQL query");
    YYERROR;
  }
  
  (yyval.seq) = (yyvsp[-3].seq);

  if((yyvsp[-2].seq))
    rasqal_query_add_data_graphs(rq, (yyvsp[-2].seq));
  rq->query_graph_pattern = (yyvsp[-1].graph_pattern);

  if((yyvsp[0].modifier))
    rq->modifier = (yyvsp[0].modifier);
}
#line 4481 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1182 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_graph_pattern* where_gp;
  raptor_sequence* seq = NULL;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql_scda) {
    sparql_syntax_error(rq,
                        "CONSTRUCT can only be used with a SPARQL query");
    YYERROR;
  }

  if((yyvsp[-2].seq)) {
    int i;
    int size = raptor_sequence_size((yyvsp[-2].seq));
    
    seq = raptor_new_sequence((raptor_data_free_handler)rasqal_free_triple,
                              (raptor_data_print_handler)rasqal_triple_print);
    for(i = 0; i < size; i++) {
      rasqal_triple* t = (rasqal_triple*)raptor_sequence_get_at((yyvsp[-2].seq), i);
      t = rasqal_new_triple_from_triple(t);
      raptor_sequence_push(seq, t);
    }
  }
  
  where_gp = rasqal_new_basic_graph_pattern_from_triples(rq, seq);
  seq = NULL;
  if(!where_gp)
    YYERROR_MSG("ConstructQuery: cannot create graph pattern");

  (yyval.seq) = (yyvsp[-2].seq);

  if((yyvsp[-5].seq))
    rasqal_query_add_data_graphs(rq, (yyvsp[-5].seq));
  rq->query_graph_pattern = where_gp;

  if((yyvsp[0].modifier))
    rq->modifier = (yyvsp[0].modifier);
}
#line 4526 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1228 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.seq) = NULL;
  if(!sparql->sparql_scda) {
    sparql_syntax_error(rq,
                        "DESCRIBE can only be used with a SPARQL query");
    YYERROR;
  }
  
  (yyval.seq) = (yyvsp[-3].seq);

  if((yyvsp[-2].seq))
    rasqal_query_add_data_graphs(rq, (yyvsp[-2].seq));

  rq->query_graph_pattern = (yyvsp[-1].graph_pattern);

  if((yyvsp[0].modifier))
    rq->modifier = (yyvsp[0].modifier);
}
#line 4553 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1252 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;

  if((yyvsp[-2].seq))
    rasqal_query_add_data_graphs(rq, (yyvsp[-2].seq));

  rq->query_graph_pattern = (yyvsp[-1].graph_pattern);

  if((yyvsp[0].modifier))
    rq->modifier = (yyvsp[0].modifier);
}
#line 4569 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1268 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].literal))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("VarOrIRIrefList 1: sequence push failed");
  }
}
#line 4582 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1277 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-2].seq);
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].literal))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("VarOrIRIrefList 2: sequence push failed");
  }
}
#line 4595 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1286 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_literal,
                           (raptor_data_print_handler)rasqal_literal_print);
  if(!(yyval.seq))
    YYERROR_MSG("VarOrIRIrefList 3: cannot create seq");
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].literal))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("VarOrIRIrefList 3: sequence push failed");
  }
}
#line 4611 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1303 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql_scda) {
    sparql_syntax_error(rq,
                        "ASK can only be used with a SPARQL query");
    YYERROR;
  }
  
  if((yyvsp[-1].seq))
    rasqal_query_add_data_graphs(rq, (yyvsp[-1].seq));

  rq->query_graph_pattern = (yyvsp[0].graph_pattern);
}
#line 4631 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1323 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.data_graph) = (yyvsp[0].data_graph);
}
#line 4639 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1327 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.data_graph) = (yyvsp[0].data_graph);
}
#line 4647 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1335 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.uri) = (yyvsp[0].uri);
}
#line 4655 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1343 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "DELETE can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }
  
  /* LAQRS: experimental syntax */
  sparql_syntax_warning(rq,
                        "DELETE FROM <uri> ... WHERE ... is deprecated LAQRS syntax.");

  if((yyvsp[-1].seq))
    rasqal_query_add_data_graphs(rq, (yyvsp[-1].seq));

  rq->query_graph_pattern = (yyvsp[0].graph_pattern);
}
#line 4679 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1363 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "DELETE can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  /* SPARQL 1.1 (Draft) update:
   * deleting via template + query - not inline atomic triples 
   */

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_UPDATE,
                                       NULL /* graph_uri */,
                                       NULL /* document_uri */,
                                       NULL /* insert templates */,
                                       (yyvsp[-2].seq) /* delete templates */,
                                       (yyvsp[0].graph_pattern) /* where */,
                                       0 /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("DeleteQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("DeleteQuery: rasqal_query_add_update_operation failed");
  }
}
#line 4715 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1395 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "DELETE can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }
  
  /* SPARQL 1.1 (Draft) update:
   * deleting inline triples - not inserting from graph URIs 
   */
  (yyvsp[-1].update)->type = RASQAL_UPDATE_TYPE_UPDATE;
  (yyvsp[-1].update)->delete_templates = (yyvsp[-1].update)->insert_templates; (yyvsp[-1].update)->insert_templates = NULL;
  (yyvsp[-1].update)->flags |= RASQAL_UPDATE_FLAGS_DATA;
  
  rasqal_query_add_update_operation(rq, (yyvsp[-1].update));
}
#line 4739 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1415 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;
  raptor_sequence* delete_templates = NULL;
  
  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "DELETE WHERE { } can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  /* SPARQL 1.1 (Draft) update:
   * deleting via template - not inline atomic triples 
   */

  /* Turn GP into flattened triples */
  if((yyvsp[0].graph_pattern)) {
    delete_templates = rasqal_graph_pattern_get_flattened_triples(rq, (yyvsp[0].graph_pattern));
    rasqal_free_graph_pattern((yyvsp[0].graph_pattern));
    (yyvsp[0].graph_pattern) = NULL;
  }

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_UPDATE,
                                       NULL /* graph_uri */,
                                       NULL /* document_uri */,
                                       NULL /* insert templates */,
                                       delete_templates /* delete templates */,
                                       NULL /* where */,
                                       0 /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("DeleteQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("DeleteQuery: rasqal_query_add_update_operation failed");
  }
}
#line 4783 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1459 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.update) = NULL;
 
  if((yyvsp[0].formula)) {
    (yyval.update) = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_UNKNOWN,
                                     NULL /* graph_uri */,
                                     NULL /* document_uri */,
                                     (yyvsp[0].formula)->triples /* insert templates */, 
                                     NULL /* delete templates */,
                                     NULL /* where */,
                                     0 /* flags */,
                                     RASQAL_UPDATE_GRAPH_ONE /* applies */);
    (yyvsp[0].formula)->triples = NULL;
    rasqal_free_formula((yyvsp[0].formula));
  }
}
#line 4804 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1476 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.update) = NULL;

  if((yyvsp[-1].formula)) {
    raptor_sequence* seq;
    seq = (yyvsp[-1].formula)->triples;

    if((yyvsp[-3].uri)) {
      rasqal_literal* origin_literal;
      
      origin_literal = rasqal_new_uri_literal(rq->world, (yyvsp[-3].uri));
      (yyvsp[-3].uri) = NULL;

      rasqal_triples_sequence_set_origin(/* dest */ NULL, seq, origin_literal);
      rasqal_free_literal(origin_literal);
    }
    (yyval.update) = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_UNKNOWN,
                                     NULL /* graph uri */,
                                     NULL /* document uri */,
                                     seq /* insert templates */,
                                     NULL /* delete templates */,
                                     NULL /* where */,
                                     0 /* flags */,
                                     RASQAL_UPDATE_GRAPH_ONE /* applies */);
    (yyvsp[-1].formula)->triples = NULL;
    rasqal_free_formula((yyvsp[-1].formula));
  }
}
#line 4837 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1509 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);

  if((yyvsp[-3].literal)) {
    rasqal_triples_sequence_set_origin(NULL, (yyval.seq), (yyvsp[-3].literal));
    rasqal_free_literal((yyvsp[-3].literal));
    (yyvsp[-3].literal) = NULL;
  }
}
#line 4851 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1524 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 4859 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1528 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 4867 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1536 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);

  if((yyvsp[0].seq)) {
    if(raptor_sequence_join((yyval.seq), (yyvsp[0].seq))) {
      raptor_free_sequence((yyvsp[0].seq));
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("ModifyTemplateList: sequence join failed");
    }
    raptor_free_sequence((yyvsp[0].seq));
  }

}
#line 4886 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1551 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 4894 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1560 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql  = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "INSERT can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  /* LAQRS: experimental syntax */
  sparql_syntax_warning(rq,
                        "INSERT FROM <uri> ... WHERE ... is deprecated LAQRS syntax.");

  if((yyvsp[-1].seq))
    rasqal_query_add_data_graphs(rq, (yyvsp[-1].seq));

  rq->query_graph_pattern = (yyvsp[0].graph_pattern);
}
#line 4918 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1580 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "INSERT can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }
  
  /* inserting via template + query - not inline atomic triples */

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_UPDATE,
                                       NULL /* graph_uri */,
                                       NULL /* document_uri */,
                                       (yyvsp[-2].seq) /* insert templates */,
                                       NULL /* delete templates */,
                                       (yyvsp[0].graph_pattern) /* where */,
                                       0 /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("InsertQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("InsertQuery: rasqal_query_add_update_operation failed");
  }
}
#line 4952 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1610 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "INSERT DATA can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }
  
  /* inserting inline atomic triples (no variables) - not via template */
  (yyvsp[-1].update)->type = RASQAL_UPDATE_TYPE_UPDATE;
  (yyvsp[-1].update)->flags |= RASQAL_UPDATE_FLAGS_DATA;

  rasqal_query_add_update_operation(rq, (yyvsp[-1].update));
}
#line 4973 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1633 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "WITH can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }
  
  if((yyvsp[-9].uri)) {
    rasqal_literal* origin_literal;

    origin_literal = rasqal_new_uri_literal(rq->world, (yyvsp[-9].uri));
    (yyvsp[-9].uri) = NULL;

    rasqal_triples_sequence_set_origin(/* dest */ NULL, (yyvsp[-2].seq), origin_literal);
    rasqal_triples_sequence_set_origin(/* dest */ NULL, (yyvsp[-6].seq), origin_literal);

    rasqal_free_literal(origin_literal);
  }

  /* after this $5, $9 and $12 are owned by update */
  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_UPDATE,
                                       NULL /* graph uri */, 
                                       NULL /* document uri */,
                                       (yyvsp[-2].seq) /* insert templates */,
                                       (yyvsp[-6].seq) /* delete templates */,
                                       (yyvsp[0].graph_pattern) /* where */,
                                       0 /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("UpdateQuery 1: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("UpdateQuery 1: rasqal_query_add_update_operation failed");
  }
}
#line 5018 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1676 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "WITH can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }
  
  if((yyvsp[-5].uri)) {
    rasqal_literal* origin_literal;
    
    origin_literal = rasqal_new_uri_literal(rq->world, (yyvsp[-5].uri));
    (yyvsp[-5].uri) = NULL;

    rasqal_triples_sequence_set_origin(/* dest */ NULL, (yyvsp[-2].seq), origin_literal);

    rasqal_free_literal(origin_literal);
  }
  
  /* after this $5 and $7 are owned by update */
  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_UPDATE,
                                       NULL /* graph uri */, 
                                       NULL /* document uri */,
                                       NULL /* insert templates */,
                                       (yyvsp[-2].seq) /* delete templates */,
                                       (yyvsp[0].graph_pattern) /* where */,
                                       0 /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("UpdateQuery 2: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("UpdateQuery 2: rasqal_query_add_update_operation failed");
  }
}
#line 5062 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1718 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "WITH can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  if((yyvsp[-5].uri)) {
    rasqal_literal* origin_literal;
    
    origin_literal = rasqal_new_uri_literal(rq->world, (yyvsp[-5].uri));
    (yyvsp[-5].uri) = NULL;

    rasqal_triples_sequence_set_origin(/* dest */ NULL, (yyvsp[-2].seq), origin_literal);

    rasqal_free_literal(origin_literal);
  }

  /* after this $5 and $7 are owned by update */
  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_UPDATE,
                                       NULL /* graph uri */, 
                                       NULL /* document uri */,
                                       (yyvsp[-2].seq) /* insert templates */,
                                       NULL /* delete templates */,
                                       (yyvsp[0].graph_pattern) /* where */,
                                       0 /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("UpdateQuery 3: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("UpdateQuery 3: rasqal_query_add_update_operation failed");
  }
}
#line 5106 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1759 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "WITH can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  /* inserting inline atomic triples (no variables) - not via template */
  (yyvsp[-1].update)->graph_uri = (yyvsp[-5].uri); /* graph uri */
  (yyvsp[-1].update)->type = RASQAL_UPDATE_TYPE_UPDATE;
  (yyvsp[-1].update)->flags |= RASQAL_UPDATE_FLAGS_DATA;

  rasqal_query_add_update_operation(rq, (yyvsp[-1].update));
}
#line 5129 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1782 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.uri_applies) = new_uri_applies((yyvsp[0].uri), RASQAL_UPDATE_GRAPH_ONE);
}
#line 5137 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1786 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.uri_applies) = new_uri_applies(NULL, RASQAL_UPDATE_GRAPH_DEFAULT);
}
#line 5145 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1790 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.uri_applies) = new_uri_applies(NULL, RASQAL_UPDATE_GRAPH_NAMED);
}
#line 5153 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1794 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.uri_applies) = new_uri_applies(NULL, RASQAL_UPDATE_GRAPH_ALL);
}
#line 5161 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1798 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* Early draft syntax - deprecated */
  sparql_syntax_warning(rq,
                        "CLEAR GRAPH DEFAULT is replaced by CLEAR DEFAULT in later SPARQL 1.1 drafts");


  (yyval.uri_applies) = new_uri_applies(NULL, RASQAL_UPDATE_GRAPH_DEFAULT);
}
#line 5174 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1811 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "CLEAR (SILENT) DEFAULT | NAMED | ALL can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  if((yyvsp[0].uri_applies)) {
    update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_CLEAR,
                                         (yyvsp[0].uri_applies)->uri ? raptor_uri_copy((yyvsp[0].uri_applies)->uri) : NULL /* graph uri or NULL */,
                                         NULL /* document uri */,
                                         NULL, NULL,
                                         NULL /*where */,
                                         (yyvsp[-1].integer) /* flags */,
                                         (yyvsp[0].uri_applies)->applies /* applies */);
    free_uri_applies((yyvsp[0].uri_applies));
    (yyvsp[0].uri_applies) = NULL;

    if(!update) {
      YYERROR_MSG("ClearQuery: rasqal_new_update_operation failed");
    } else {
      if(rasqal_query_add_update_operation(rq, update))
        YYERROR_MSG("ClearQuery: rasqal_query_add_update_operation failed");
    }
  }
}
#line 5210 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1843 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "CLEAR can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  /* Early draft syntax - deprecated */
  sparql_syntax_warning(rq,
                        "CLEAR is replaced by CLEAR DEFAULT in later SPARQL 1.1 drafts");

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_CLEAR,
                                       NULL /* graph uri */, 
                                       NULL /* document uri */,
                                       NULL, NULL,
                                       NULL /* where */,
                                       0 /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("ClearQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("ClearQuery: rasqal_query_add_update_operation failed");
  }
}
#line 5245 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1878 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.integer) = RASQAL_UPDATE_FLAGS_SILENT;
}
#line 5253 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1882 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.integer) = 0;
}
#line 5261 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1890 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "CREATE (SILENT) <uri> can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_CREATE,
                                       (yyvsp[0].uri) /* graph uri */, 
                                       NULL /* document uri */,
                                       NULL, NULL,
                                       NULL /*where */,
                                       (yyvsp[-1].integer) /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("CreateQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("CreateQuery: rasqal_query_add_update_operation failed");
  }
}
#line 5292 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1917 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "CREATE (SILENT) GRAPH <uri> can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  /* Early draft syntax - deprecated */
  sparql_syntax_warning(rq,
                        "CREATE (SILENT) GRAPH <uri> is replaced by CREATE (SILENT) <uri> in later SPARQL 1.1 drafts");

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_CREATE,
                                       (yyvsp[0].uri) /* graph uri */, 
                                       NULL /* document uri */,
                                       NULL, NULL,
                                       NULL /*where */,
                                       RASQAL_UPDATE_FLAGS_SILENT /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("CreateQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("CreateQuery: rasqal_query_add_update_operation failed");
  }
}
#line 5327 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1952 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;
  
  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "DROP (SILENT) DEFAULT | NAMED | ALL can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  if((yyvsp[0].uri_applies)) {
    update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_DROP,
                                         (yyvsp[0].uri_applies)->uri ? raptor_uri_copy((yyvsp[0].uri_applies)->uri) : NULL /* graph uri or NULL */,
                                         NULL /* document uri */,
                                         NULL, NULL,
                                         NULL /*where */,
                                         (yyvsp[-1].integer) /* flags */,
                                         (yyvsp[0].uri_applies)->applies /* applies */);
    free_uri_applies((yyvsp[0].uri_applies));
    (yyvsp[0].uri_applies) = NULL;

    if(!update) {
      YYERROR_MSG("DropQuery: rasqal_new_update_operation failed");
    } else {
      if(rasqal_query_add_update_operation(rq, update))
        YYERROR_MSG("DropQuery: rasqal_query_add_update_operation failed");
    }
  }
}
#line 5363 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1988 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].uri))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("IriRefList 1: sequence push failed");
  }
}
#line 5376 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1997 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)raptor_free_uri,
                           (raptor_data_print_handler)raptor_uri_print);
  if(!(yyval.seq)) {
    if((yyvsp[0].uri))
      raptor_free_uri((yyvsp[0].uri));
    YYERROR_MSG("IriRefList 2: cannot create sequence");
  }
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].uri))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("IriRefList 2: sequence push failed");
  }
}
#line 5395 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 2016 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.uri) = NULL;
}
#line 5403 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 2020 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.uri) = (yyvsp[0].uri);
}
#line 5411 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 2027 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.uri) = (yyvsp[0].uri);
}
#line 5419 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 2031 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* Early draft syntax allowed a list of URIs - deprecated */
  sparql_syntax_warning(rq,
                        "LOAD <document uri list> INTO <graph uri> is replaced by LOAD <document uri> INTO GRAPH <graph uri> in later SPARQL 1.1 drafts");

  (yyval.uri) = (yyvsp[0].uri);
}
#line 5431 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 2039 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* Early draft syntax allowed a list of URIs - deprecated */
  sparql_syntax_warning(rq,
                        "LOAD <document uri list> INTO DEFAULT is replaced by LOAD <document uri> in later SPARQL 1.1 drafts");

  (yyval.uri) = NULL;
}
#line 5443 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 2051 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;
  
  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "LOAD <uri> can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_LOAD,
                                       NULL /* graph uri */, 
                                       (yyvsp[0].uri) /* document uri */,
                                       NULL, NULL,
                                       NULL /* where */,
                                       (yyvsp[-1].integer) /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("LoadQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("LoadQuery: rasqal_query_add_update_operation failed");
  }
}
#line 5474 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 2078 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  int i;
  raptor_uri* doc_uri;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "LOAD <document uri> INTO GRAPH <graph URI> / DEFAULT can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  for(i = 0; (doc_uri = (raptor_uri*)raptor_sequence_get_at((yyvsp[-2].seq), i)); i++) {
    rasqal_update_operation* update;
    update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_LOAD,
                                         (yyvsp[0].uri) ? raptor_uri_copy((yyvsp[0].uri)) : NULL /* graph uri */,
                                         raptor_uri_copy(doc_uri) /* document uri */,
                                         NULL, NULL,
                                         NULL /*where */,
                                         (yyvsp[-3].integer) /* flags */,
                                         RASQAL_UPDATE_GRAPH_ONE /* applies */);
    if(!update) {
      YYERROR_MSG("LoadQuery: rasqal_new_update_operation failed");
    } else {
      if(rasqal_query_add_update_operation(rq, update))
        YYERROR_MSG("LoadQuery: rasqal_query_add_update_operation failed");
    }

    if(i == 1)
      /* Early draft syntax allowed a list of URIs - deprecated */
      sparql_syntax_warning(rq,
                            "LOAD <document uri list> INTO <graph uri> / DEFAULT is replaced by LOAD <document uri> INTO GRAPH <graph uri> or LOAD <document uri> in later SPARQL 1.1 drafts");
    

  }

  raptor_free_sequence((yyvsp[-2].seq));
  if((yyvsp[0].uri))
    raptor_free_uri((yyvsp[0].uri));
}
#line 5520 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 2124 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "ADD (SILENT) <uri> TO <uri> can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_ADD,
                                       (yyvsp[-2].uri) /* graph uri or NULL */, 
                                       (yyvsp[0].uri) /* document uri */,
                                       NULL, NULL,
                                       NULL /*where */,
                                       (yyvsp[-3].integer) /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("AddQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("AddQuery: rasqal_query_add_update_operation failed");
  }
}
#line 5551 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 2155 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "MOVE (SILENT) <uri> TO <uri> can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_MOVE,
                                       (yyvsp[-2].uri) /* graph uri or NULL */, 
                                       (yyvsp[0].uri) /* document uri */,
                                       NULL, NULL,
                                       NULL /*where */,
                                       (yyvsp[-3].integer) /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("MoveQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("MoveQuery: rasqal_query_add_update_operation failed");
  }
}
#line 5582 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 2186 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  rasqal_update_operation* update;

  sparql = (rasqal_sparql_query_language*)(rq->context);

  if(!sparql->sparql11_update) {
    sparql_syntax_error(rq,
                        "COPY (SILENT) <uri> TO <uri> can only be used with a SPARQL 1.1 Update");
    YYERROR;
  }

  update = rasqal_new_update_operation(RASQAL_UPDATE_TYPE_COPY,
                                       (yyvsp[-2].uri) /* graph uri or NULL */, 
                                       (yyvsp[0].uri) /* document uri */,
                                       NULL, NULL,
                                       NULL /*where */,
                                       (yyvsp[-3].integer) /* flags */,
                                       RASQAL_UPDATE_GRAPH_ONE /* applies */);
  if(!update) {
    YYERROR_MSG("CopyQuery: rasqal_new_update_operation failed");
  } else {
    if(rasqal_query_add_update_operation(rq, update))
      YYERROR_MSG("CopyQuery: rasqal_query_add_update_operation failed");
  }
}
#line 5613 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 2217 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if((yyvsp[-1].seq) && (yyvsp[0].data_graph))
    raptor_sequence_push((yyvsp[-1].seq), (yyvsp[0].data_graph));
}
#line 5623 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 2223 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_data_graph, (raptor_data_print_handler)rasqal_data_graph_print);
  if((yyval.seq) && (yyvsp[0].data_graph))
    raptor_sequence_push((yyval.seq), (yyvsp[0].data_graph));
}
#line 5633 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 2232 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 5641 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 2236 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 5649 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 2244 "./sparql_parser.y" /* yacc.c:1646  */
    {
  if((yyvsp[0].literal)) {
    raptor_uri* uri = rasqal_literal_as_uri((yyvsp[0].literal));
    rasqal_data_graph* dg;

    dg = rasqal_new_data_graph_from_uri(rq->world, uri,
                                        NULL, RASQAL_DATA_GRAPH_BACKGROUND,
                                        NULL, NULL, NULL);

    if(!dg) {
      rasqal_free_literal((yyvsp[0].literal));
      YYERROR_MSG("DefaultGraphClause: rasqal_query_new_data_graph_from_uri() failed");
    }
    rasqal_free_literal((yyvsp[0].literal));

    (yyval.data_graph) = dg;
  } else
    (yyval.data_graph) = NULL;
}
#line 5673 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 2268 "./sparql_parser.y" /* yacc.c:1646  */
    {
  if((yyvsp[0].literal)) {
    raptor_uri* uri = rasqal_literal_as_uri((yyvsp[0].literal));
    rasqal_data_graph* dg;

    dg = rasqal_new_data_graph_from_uri(rq->world, uri,
                                        uri, RASQAL_DATA_GRAPH_NAMED,
                                        NULL, NULL, NULL);
    
    if(!dg) {
      rasqal_free_literal((yyvsp[0].literal));
      YYERROR_MSG("NamedGraphClause: rasqal_query_new_data_graph_from_uri() failed");
    }
    rasqal_free_literal((yyvsp[0].literal));
    (yyval.data_graph) = dg;
  } else
    (yyval.data_graph) = NULL;
}
#line 5696 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 2291 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 5704 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 2299 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 5712 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 2303 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 5720 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 2311 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 5728 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 2315 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = NULL;
}
#line 5736 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 2323 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.modifier) = rasqal_new_solution_modifier(rq,
                                    /* order_conditions */ (yyvsp[-1].seq),
                                    /* group_conditions */ (yyvsp[-3].seq),
                                    /* having_conditions */ (yyvsp[-2].seq),
                                    /* limit */ (yyvsp[0].limit_offset)[0],
                                    /* offset */ (yyvsp[0].limit_offset)[1]);
  
}
#line 5750 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 2337 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if((yyvsp[0].expr))
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("GroupConditionList 1: sequence push failed");
    }
}
#line 5764 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 2347 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_expression,
                           (raptor_data_print_handler)rasqal_expression_print);
  if(!(yyval.seq)) {
    if((yyvsp[0].expr))
      rasqal_free_expression((yyvsp[0].expr));
    YYERROR_MSG("GroupConditionList 2: cannot create sequence");
  }
  if((yyvsp[0].expr))
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("GroupConditionList 2: sequence push failed");
    }
}
#line 5784 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 2367 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.variable) = (yyvsp[0].variable);
}
#line 5792 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 2371 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.variable) = NULL;
}
#line 5800 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 2379 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 5808 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 2383 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 5816 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 2387 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_literal* l;

  (yyval.expr) = (yyvsp[-2].expr);
  if((yyvsp[-1].variable)) {
    if(rasqal_expression_mentions_variable((yyval.expr), (yyvsp[-1].variable))) {
      sparql_query_error_full(rq,
                              "Expression in GROUP BY ( expression ) AS %s contains the variable name '%s'",
                              (yyvsp[-1].variable)->name, (yyvsp[-1].variable)->name);
    } else {
      /* Expression AS Variable */
      (yyvsp[-1].variable)->expression = (yyval.expr);
      (yyval.expr) = NULL;
      
      l = rasqal_new_variable_literal(rq->world, (yyvsp[-1].variable));
      if(!l)
        YYERROR_MSG("GroupCondition 4: cannot create variable literal");
      (yyvsp[-1].variable) = NULL;

      (yyval.expr) = rasqal_new_literal_expression(rq->world, l);
      if(!(yyval.expr))
        YYERROR_MSG("GroupCondition 4: cannot create variable literal expression");
    }
  }
  
}
#line 5847 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 2414 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_literal* l;
  l = rasqal_new_variable_literal(rq->world, (yyvsp[0].variable));
  if(!l)
    YYERROR_MSG("GroupCondition 5: cannot create lit");
  (yyval.expr) = rasqal_new_literal_expression(rq->world, l);
  if(!(yyval.expr))
    YYERROR_MSG("GroupCondition 5: cannot create lit expr");
}
#line 5861 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 2428 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.seq) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "GROUP BY can only be used with SPARQL 1.1");
    YYERROR;
  } else
    (yyval.seq) = (yyvsp[0].seq);
}
#line 5878 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 2441 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 5886 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 2449 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 5894 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 2456 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if((yyvsp[0].expr))
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("HavingConditionList 1: sequence push failed");
    }
}
#line 5908 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 2466 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_expression,
                           (raptor_data_print_handler)rasqal_expression_print);
  if(!(yyval.seq)) {
    if((yyvsp[0].expr))
      rasqal_free_expression((yyvsp[0].expr));
    YYERROR_MSG("HavingConditionList 2: cannot create sequence");
  }
  if((yyvsp[0].expr))
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("HavingConditionList 2: sequence push failed");
    }
}
#line 5928 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 2486 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.seq) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "HAVING can only be used with SPARQL 1.1");
    YYERROR;
  } else 
    (yyval.seq) = (yyvsp[0].seq);
}
#line 5945 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 2499 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 5953 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 2507 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.limit_offset)[0] = (yyvsp[-1].integer);
  (yyval.limit_offset)[1] = (yyvsp[0].integer);
}
#line 5962 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 2512 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.limit_offset)[0] = (yyvsp[0].integer);
  (yyval.limit_offset)[1] = (yyvsp[-1].integer);
}
#line 5971 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 2517 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.limit_offset)[0] = (yyvsp[0].integer);
  (yyval.limit_offset)[1] = -1;
}
#line 5980 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 2522 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.limit_offset)[0] = -1;
  (yyval.limit_offset)[1] = (yyvsp[0].integer);
}
#line 5989 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 2527 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.limit_offset)[0] = -1;
  (yyval.limit_offset)[1] = -1;
}
#line 5998 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 2536 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 6006 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 2540 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 6014 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 2548 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if((yyvsp[0].expr))
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("OrderConditionList 1: sequence push failed");
    }
}
#line 6028 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 2558 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_expression,
                           (raptor_data_print_handler)rasqal_expression_print);
  if(!(yyval.seq)) {
    if((yyvsp[0].expr))
      rasqal_free_expression((yyvsp[0].expr));
    YYERROR_MSG("OrderConditionList 2: cannot create sequence");
  }
  if((yyvsp[0].expr))
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("OrderConditionList 2: sequence push failed");
    }
}
#line 6048 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 2578 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ORDER_COND_ASC, (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("OrderCondition 1: cannot create expr");
}
#line 6059 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 2585 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ORDER_COND_DESC, (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("OrderCondition 2: cannot create expr");
}
#line 6070 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 2592 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* The direction of ordering is ascending by default */
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ORDER_COND_ASC, (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("OrderCondition 3: cannot create expr");
}
#line 6082 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 2600 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_literal* l;
  rasqal_expression *e;
  l = rasqal_new_variable_literal(rq->world, (yyvsp[0].variable));
  if(!l)
    YYERROR_MSG("OrderCondition 4: cannot create lit");
  e = rasqal_new_literal_expression(rq->world, l);
  if(!e)
    YYERROR_MSG("OrderCondition 4: cannot create lit expr");

  /* The direction of ordering is ascending by default */
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ORDER_COND_ASC, e);
  if(!(yyval.expr))
    YYERROR_MSG("OrderCondition 1: cannot create expr");
}
#line 6103 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 2617 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* The direction of ordering is ascending by default */
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ORDER_COND_ASC, (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("OrderCondition 5: cannot create expr");
}
#line 6115 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 2625 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* The direction of ordering is ascending by default */
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ORDER_COND_ASC, (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("OrderCondition 6: cannot create expr");
}
#line 6127 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 2637 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.integer) = -1;

  if((yyvsp[0].literal) != NULL) {
    (yyval.integer) = (yyvsp[0].literal)->value.integer;
    rasqal_free_literal((yyvsp[0].literal));
  }
  
}
#line 6141 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 2651 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.integer) = -1;

  if((yyvsp[0].literal) != NULL) {
    (yyval.integer) = (yyvsp[0].literal)->value.integer;
    rasqal_free_literal((yyvsp[0].literal));
  }
}
#line 6154 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 2664 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.bindings) = (yyvsp[0].bindings);
}
#line 6162 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 2668 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.bindings) = NULL;
}
#line 6170 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 2676 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 6178 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 2680 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 6186 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 2687 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].variable))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("VarList 1: sequence push failed");
  }
}
#line 6199 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 2696 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_variable,
                           (raptor_data_print_handler)rasqal_variable_print);
  if(!(yyval.seq))
    YYERROR_MSG("VarList 2: cannot create seq");

  if(raptor_sequence_push((yyval.seq), (yyvsp[0].variable))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("VarList 3: sequence push failed");
  }
}
#line 6216 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 2715 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 6224 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 2719 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 6232 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 2729 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].row))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("DataBlockRowList 1: sequence push failed");
  } else {
    int size = raptor_sequence_size((yyval.seq));
    (yyvsp[0].row)->offset = size-1;
  }
}
#line 6248 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 2741 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_row,
                           (raptor_data_print_handler)rasqal_row_print);
  if(!(yyval.seq)) {
    if((yyvsp[0].row))
      rasqal_free_row((yyvsp[0].row));

    YYERROR_MSG("DataBlockRowList 2: cannot create sequence");
  }
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].row))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("DataBlockRowList 2: sequence push failed");
  }
}
#line 6268 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 2763 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.row) = NULL;
  if((yyvsp[-1].seq)) {
    int size;
    rasqal_row* row;
    int i;
    
    size = raptor_sequence_size((yyvsp[-1].seq));

    row = rasqal_new_row_for_size(rq->world, size);
    if(!row) {
      YYERROR_MSG("DataBlockRow: cannot create row");
    } else {
      for(i = 0; i < size; i++) {
        rasqal_literal* value = (rasqal_literal*)raptor_sequence_get_at((yyvsp[-1].seq), i);
        rasqal_row_set_value_at(row, i, value);
      }
    }
    raptor_free_sequence((yyvsp[-1].seq));
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1
    RASQAL_DEBUG1("DataBlockRow returned: ");
    rasqal_row_print(row, stderr);
    fputc('\n', stderr);
#endif
    (yyval.row) = row;
  }
}
#line 6300 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 2791 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.row) = NULL;
}
#line 6308 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 2798 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  RASQAL_DEBUG1("DataBlockValue 1 value: ");
  rasqal_literal_print((yyvsp[0].literal), stderr);
  fputc('\n', stderr);
#endif
  (yyval.seq) = (yyvsp[-1].seq);
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].literal))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("IriRefList 1: sequence push failed");
  }
}
#line 6326 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 2812 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  RASQAL_DEBUG1("DataBlockValue 2 value: ");
  rasqal_literal_print((yyvsp[0].literal), stderr);
  fputc('\n', stderr);
#endif
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_literal,
                           (raptor_data_print_handler)rasqal_literal_print);
  if(!(yyval.seq)) {
    if((yyvsp[0].literal))
      rasqal_free_literal((yyvsp[0].literal));
    YYERROR_MSG("IriRefList 2: cannot create sequence");
  }
  if(raptor_sequence_push((yyval.seq), (yyvsp[0].literal))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("IriRefList 2: sequence push failed");
  }
}
#line 6350 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 2835 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_string_literal(rq->world, (yyvsp[0].name), 
	                         NULL /* language */,
                                 NULL /* dt uri */, NULL /* dt_qname */);
}
#line 6360 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 2841 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_string_literal(rq->world, (yyvsp[-1].name), 
	                         RASQAL_GOOD_CAST(const char*, (yyvsp[0].name)),
                                 NULL /* dt uri */, NULL /* dt_qname */);
}
#line 6370 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 2847 "./sparql_parser.y" /* yacc.c:1646  */
    {
  raptor_uri* dt_uri = raptor_uri_copy(rasqal_literal_as_uri((yyvsp[0].literal)));
  (yyval.literal) = rasqal_new_string_literal(rq->world, (yyvsp[-2].name), 
	                         NULL /* language */,
                                 dt_uri, NULL /* dt_qname */);
  rasqal_free_literal((yyvsp[0].literal));
}
#line 6382 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 2855 "./sparql_parser.y" /* yacc.c:1646  */
    {
  if((yyvsp[-2].literal)) {
    raptor_uri* dt_uri = raptor_uri_copy(rasqal_literal_as_uri((yyvsp[0].literal)));
    const unsigned char *str = (yyvsp[-2].literal)->string;
    (yyvsp[-2].literal)->string = NULL;

    (yyval.literal) = rasqal_new_string_literal(rq->world, str,
                                   NULL /* language */,
                                   dt_uri, NULL /* dt_qname */);
  }
  rasqal_free_literal((yyvsp[0].literal));
  rasqal_free_literal((yyvsp[-2].literal));
}
#line 6400 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 2873 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 6408 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 2877 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 6416 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 2881 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 6424 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 2885 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 6432 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 2889 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = NULL;
}
#line 6440 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 2901 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[-1].graph_pattern);
}
#line 6448 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 2905 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[-1].graph_pattern);
}
#line 6456 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 2916 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_graph_pattern *formula_gp = NULL;

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "GroupGraphPattern\n  TriplesBlockOpt=");
  if((yyvsp[0].graph_pattern))
    rasqal_formula_print((yyvsp[-1].formula), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, ", GraphpatternListOpt=");
  if((yyvsp[0].graph_pattern))
    rasqal_graph_pattern_print((yyvsp[0].graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputs("\n", DEBUG_FH);
#endif


  if(!(yyvsp[-1].formula) && !(yyvsp[0].graph_pattern)) {
    (yyval.graph_pattern) = rasqal_new_2_group_graph_pattern(rq, NULL, NULL);
    if(!(yyval.graph_pattern))
      YYERROR_MSG("GroupGraphPattern: cannot create group gp");
  } else {
    if((yyvsp[-1].formula)) {
      formula_gp = rasqal_new_basic_graph_pattern_from_formula(rq,
                                                               (yyvsp[-1].formula));
      if(!formula_gp) {
        if((yyvsp[0].graph_pattern))
          rasqal_free_graph_pattern((yyvsp[0].graph_pattern));
        YYERROR_MSG("GroupGraphPattern: cannot create formula_gp");
      }
    }

    if((yyvsp[0].graph_pattern)) {
      (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
      if(formula_gp && raptor_sequence_shift((yyval.graph_pattern)->graph_patterns, formula_gp)) {
        rasqal_free_graph_pattern((yyval.graph_pattern));
        (yyval.graph_pattern) = NULL;
        YYERROR_MSG("GroupGraphPattern: sequence push failed");
      }
    } else
      (yyval.graph_pattern) = formula_gp;
  }
  
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "  after graph pattern=");
  if((yyval.graph_pattern))
    rasqal_graph_pattern_print((yyval.graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif
}
#line 6514 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 2974 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "TriplesBlockOpt 1\n  TriplesBlock=");
  if((yyvsp[0].formula))
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputs("\n\n", DEBUG_FH);
#endif

  (yyval.formula) = (yyvsp[0].formula);
}
#line 6531 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 2987 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = NULL;
}
#line 6539 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 3000 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "GraphPatternListOpt\n  GraphPatternListOpt=");
  if((yyvsp[-1].graph_pattern))
    rasqal_graph_pattern_print((yyvsp[-1].graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, ", GraphPatternList=");
  if((yyvsp[0].graph_pattern))
    rasqal_graph_pattern_print((yyvsp[0].graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputs("\n", DEBUG_FH);
#endif

  (yyval.graph_pattern) =  ((yyvsp[-1].graph_pattern) ? (yyvsp[-1].graph_pattern) : (yyvsp[0].graph_pattern));
  if((yyvsp[-1].graph_pattern) && (yyvsp[0].graph_pattern)) {
    (yyval.graph_pattern) = (yyvsp[-1].graph_pattern);
    if(rasqal_graph_patterns_join((yyval.graph_pattern), (yyvsp[0].graph_pattern))) {
      rasqal_free_graph_pattern((yyval.graph_pattern));
      rasqal_free_graph_pattern((yyvsp[0].graph_pattern));
      (yyval.graph_pattern) = NULL;
      YYERROR_MSG("GraphPatternListOpt: sequence join failed");
    }
    rasqal_free_graph_pattern((yyvsp[0].graph_pattern));
  }
  
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "  after grouping graph pattern=");
  if((yyval.graph_pattern))
    rasqal_graph_pattern_print((yyval.graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif
}
#line 6580 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 3037 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6588 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 3041 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = NULL;
}
#line 6596 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 3054 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_graph_pattern *formula_gp = NULL;

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "GraphPatternList\n  GraphPatternListFilter=");
  if((yyvsp[-2].graph_pattern))
    rasqal_graph_pattern_print((yyvsp[-2].graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, ", TriplesBlockOpt=");
  if((yyvsp[0].formula))
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputs("\n", DEBUG_FH);
#endif

  if((yyvsp[0].formula)) {
    formula_gp = rasqal_new_basic_graph_pattern_from_formula(rq,
                                                             (yyvsp[0].formula));
    if(!formula_gp) {
      if((yyvsp[-2].graph_pattern))
        rasqal_free_graph_pattern((yyvsp[-2].graph_pattern));
      YYERROR_MSG("GraphPatternList: cannot create formula_gp");
    }
  }
  (yyval.graph_pattern) = rasqal_new_2_group_graph_pattern(rq, (yyvsp[-2].graph_pattern), formula_gp);
  if(!(yyval.graph_pattern))
    YYERROR_MSG("GraphPatternList: cannot create sequence");

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "  after graph pattern=");
  if((yyval.graph_pattern))
    rasqal_graph_pattern_print((yyval.graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif
}
#line 6640 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 3102 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "GraphPatternListFilter 1\n  GraphPatternNotTriples=");
  if((yyvsp[0].graph_pattern))
    rasqal_graph_pattern_print((yyvsp[0].graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputs("\n\n", DEBUG_FH);
#endif

  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6657 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 3115 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "GraphPatternListFilter 2\n  Filter=");
  if((yyvsp[0].expr))
    rasqal_expression_print((yyvsp[0].expr), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputs("\n", DEBUG_FH);
#endif

  (yyval.graph_pattern) = rasqal_new_filter_graph_pattern(rq, (yyvsp[0].expr));
  if(!(yyval.graph_pattern))
    YYERROR_MSG("GraphPatternListFilter 2: cannot create graph pattern");

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "  after graph pattern=");
  if((yyval.graph_pattern))
    rasqal_graph_pattern_print((yyval.graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif
}
#line 6685 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 3149 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "TriplesBlock\n  TriplesSameSubject=");
  if((yyvsp[-2].formula))
    rasqal_formula_print((yyvsp[-2].formula), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, ", TriplesBlockOpt=");
  if((yyvsp[0].formula))
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputs("\n", DEBUG_FH);
#endif


  (yyval.formula) =  ((yyvsp[-2].formula) ? (yyvsp[-2].formula) : (yyvsp[0].formula));
  if((yyvsp[-2].formula) && (yyvsp[0].formula)) {
    /* $1 and $3 are freed as necessary */
    (yyval.formula) = rasqal_formula_join((yyvsp[-2].formula), (yyvsp[0].formula));
    if(!(yyvsp[-2].formula))
      YYERROR_MSG("TriplesBlock: formula join failed");
  }

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "  after joining formula=");
  rasqal_formula_print((yyval.formula), DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif
}
#line 6720 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 3180 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = (yyvsp[0].formula);
}
#line 6728 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 3188 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6736 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 3192 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6744 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 3196 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6752 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 3200 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6760 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 3204 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6768 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 3208 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6776 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 3212 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6784 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 3216 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 6792 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 3224 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "PatternElementForms 4\n  graphpattern=");
  if((yyvsp[0].graph_pattern))
    rasqal_graph_pattern_print((yyvsp[0].graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputs("\n\n", DEBUG_FH);
#endif

  (yyval.graph_pattern) = NULL;

  if((yyvsp[0].graph_pattern)) {
    raptor_sequence *seq;

    seq = raptor_new_sequence((raptor_data_free_handler)rasqal_free_graph_pattern,
                              (raptor_data_print_handler)rasqal_graph_pattern_print);
    if(!seq) {
      rasqal_free_graph_pattern((yyvsp[0].graph_pattern));
      YYERROR_MSG("OptionalGraphPattern 1: cannot create sequence");
    } else {
      if(raptor_sequence_push(seq, (yyvsp[0].graph_pattern))) {
        raptor_free_sequence(seq);
        YYERROR_MSG("OptionalGraphPattern 2: sequence push failed");
      } else {
        (yyval.graph_pattern) = rasqal_new_graph_pattern_from_sequence(rq,
                                                    seq,
                                                    RASQAL_GRAPH_PATTERN_OPERATOR_OPTIONAL);
        if(!(yyval.graph_pattern))
          YYERROR_MSG("OptionalGraphPattern: cannot create graph pattern");
      }
    }
  }
}
#line 6831 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 3263 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "GraphGraphPattern 2\n  varoruri=");
  rasqal_literal_print((yyvsp[-1].literal), DEBUG_FH);
  fprintf(DEBUG_FH, ", graphpattern=");
  if((yyvsp[0].graph_pattern))
    rasqal_graph_pattern_print((yyvsp[0].graph_pattern), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputs("\n\n", DEBUG_FH);
#endif

  if((yyvsp[0].graph_pattern)) {
    raptor_sequence *seq;

    seq = raptor_new_sequence((raptor_data_free_handler)rasqal_free_graph_pattern,
                              (raptor_data_print_handler)rasqal_graph_pattern_print);
    if(!seq) {
      rasqal_free_graph_pattern((yyvsp[0].graph_pattern));
      YYERROR_MSG("GraphGraphPattern 1: cannot create sequence");
    } else {
      if(raptor_sequence_push(seq, (yyvsp[0].graph_pattern))) {
        raptor_free_sequence(seq);
        YYERROR_MSG("GraphGraphPattern 2: sequence push failed");
      } else {
        (yyval.graph_pattern) = rasqal_new_graph_pattern_from_sequence(rq,
                                                    seq,
                                                    RASQAL_GRAPH_PATTERN_OPERATOR_GRAPH);
        if(!(yyval.graph_pattern))
          YYERROR_MSG("GraphGraphPattern: cannot create graph pattern");
        else
          rasqal_graph_pattern_set_origin((yyval.graph_pattern), (yyvsp[-1].literal));
      }
    }
  }


#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "GraphGraphPattern\n  graphpattern=");
  rasqal_graph_pattern_print((yyval.graph_pattern), DEBUG_FH);
  fputs("\n\n", DEBUG_FH);
#endif

  rasqal_free_literal((yyvsp[-1].literal));
}
#line 6881 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 3313 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = rasqal_new_single_graph_pattern(rq,
                                       RASQAL_GRAPH_PATTERN_OPERATOR_SERVICE,
                                       (yyvsp[0].graph_pattern));
  if((yyval.graph_pattern)) {
    (yyval.graph_pattern)->silent = ((yyvsp[-2].integer) & RASQAL_UPDATE_FLAGS_SILENT) ? 1 : 0;

    (yyval.graph_pattern)->origin = (yyvsp[-1].literal);
    (yyvsp[-1].literal) = NULL;
  } else if((yyvsp[-1].literal))
    rasqal_free_literal((yyvsp[-1].literal));
}
#line 6898 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 3330 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.graph_pattern) = NULL;
  if((yyvsp[-3].expr) && (yyvsp[-1].variable)) {
    if(!sparql->sparql11_query) {
      sparql_syntax_error(rq,
                          "BIND can only be used with SPARQL 1.1");
      YYERROR;
    } else {
      (yyval.graph_pattern) = rasqal_new_let_graph_pattern(rq, (yyvsp[-1].variable), (yyvsp[-3].expr));
    }
  } else
    (yyval.graph_pattern) = NULL;
}
#line 6919 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 3351 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.bindings) = (yyvsp[0].bindings);
}
#line 6927 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 3358 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.bindings) = (yyvsp[0].bindings);
}
#line 6935 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 3362 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.bindings) = (yyvsp[0].bindings);
}
#line 6943 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 3370 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.bindings) = rasqal_new_bindings_from_var_values(rq, (yyvsp[-3].variable), (yyvsp[-1].seq));
}
#line 6951 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 3379 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 6959 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 3383 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 6967 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 3399 "./sparql_parser.y" /* yacc.c:1646  */
    {
  if((yyvsp[-4].seq)) {
    (yyval.bindings) = rasqal_new_bindings(rq, (yyvsp[-4].seq), (yyvsp[-1].seq));
    if(!(yyval.bindings))
      YYERROR_MSG("InlineDataFull: cannot create bindings");
  } else {
    if((yyvsp[-1].seq))
      raptor_free_sequence((yyvsp[-1].seq));

    (yyval.bindings) = NULL;
  }
}
#line 6984 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 3415 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = rasqal_new_values_graph_pattern(rq, (yyvsp[0].bindings));
  if(!(yyval.graph_pattern))
    YYERROR_MSG("InlineDataGraphPattern: cannot create gp");
}
#line 6994 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 219:
#line 3425 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = rasqal_new_single_graph_pattern(rq,
                                       RASQAL_GRAPH_PATTERN_OPERATOR_MINUS,
                                       (yyvsp[0].graph_pattern));
}
#line 7004 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 220:
#line 3435 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
  if(raptor_sequence_shift((yyval.graph_pattern)->graph_patterns, (yyvsp[-2].graph_pattern))) {
    rasqal_free_graph_pattern((yyval.graph_pattern));
    (yyval.graph_pattern) = NULL;
    YYERROR_MSG("GroupOrUnionGraphPattern: sequence push failed");
  }

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "UnionGraphPattern\n  graphpattern=");
  rasqal_graph_pattern_print((yyval.graph_pattern), DEBUG_FH);
  fputs("\n\n", DEBUG_FH);
#endif
}
#line 7023 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 221:
#line 3450 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[0].graph_pattern);
}
#line 7031 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 222:
#line 3457 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.graph_pattern) = (yyvsp[-2].graph_pattern);
  if((yyvsp[0].graph_pattern))
    if(raptor_sequence_push((yyval.graph_pattern)->graph_patterns, (yyvsp[0].graph_pattern))) {
      rasqal_free_graph_pattern((yyval.graph_pattern));
      (yyval.graph_pattern) = NULL;
      YYERROR_MSG("GroupOrUnionGraphPatternList 1: sequence push failed");
    }
}
#line 7045 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 223:
#line 3467 "./sparql_parser.y" /* yacc.c:1646  */
    {
  raptor_sequence *seq;
  seq = raptor_new_sequence((raptor_data_free_handler)rasqal_free_graph_pattern,
                            (raptor_data_print_handler)rasqal_graph_pattern_print);
  if(!seq) {
    if((yyvsp[0].graph_pattern))
      rasqal_free_graph_pattern((yyvsp[0].graph_pattern));
    YYERROR_MSG("GroupOrUnionGraphPatternList 2: cannot create sequence");
  }
  if((yyvsp[0].graph_pattern))
    if(raptor_sequence_push(seq, (yyvsp[0].graph_pattern))) {
      raptor_free_sequence(seq);
      YYERROR_MSG("GroupOrUnionGraphPatternList 2: sequence push failed");
    }
  (yyval.graph_pattern) = rasqal_new_graph_pattern_from_sequence(rq,
                                              seq,
                                              RASQAL_GRAPH_PATTERN_OPERATOR_UNION);
  if(!(yyval.graph_pattern))
    YYERROR_MSG("GroupOrUnionGraphPatternList 1: cannot create gp");
}
#line 7070 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 224:
#line 3492 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.graph_pattern) = NULL;
  if((yyvsp[-3].variable) && (yyvsp[-1].expr)) {
    if(sparql->experimental)
      (yyval.graph_pattern) = rasqal_new_let_graph_pattern(rq, (yyvsp[-3].variable), (yyvsp[-1].expr));
    else {
      sparql_syntax_error(rq,
                          "LET can only be used with LAQRS");
      YYERROR;
    }
  } else
    (yyval.graph_pattern) = NULL;
}
#line 7091 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 225:
#line 3513 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 7099 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 226:
#line 3521 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 7107 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 227:
#line 3525 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 7115 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 228:
#line 3529 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 7123 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 229:
#line 3536 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 7131 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 230:
#line 3540 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 7139 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 231:
#line 3548 "./sparql_parser.y" /* yacc.c:1646  */
    {
  raptor_uri* uri = rasqal_literal_as_uri((yyvsp[-5].literal));
  
  if(!(yyvsp[-2].seq)) {
    (yyvsp[-2].seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_expression,
                             (raptor_data_print_handler)rasqal_expression_print);
    if(!(yyvsp[-2].seq)) {
      rasqal_free_literal((yyvsp[-5].literal));
      YYERROR_MSG("FunctionCall: cannot create sequence");
    }
  }

  uri = raptor_uri_copy(uri);

  if(raptor_sequence_size((yyvsp[-2].seq)) == 1 &&
     rasqal_xsd_is_datatype_uri(rq->world, uri)) {
    rasqal_expression* e = (rasqal_expression*)raptor_sequence_pop((yyvsp[-2].seq));
    (yyval.expr) = rasqal_new_cast_expression(rq->world, uri, e);
    if((yyval.expr))
      (yyval.expr)->flags |= (yyvsp[-3].uinteger);
    raptor_free_sequence((yyvsp[-2].seq));
  } else {
    unsigned int flags = 0;
    if((yyvsp[-3].uinteger))
      flags |= 1;
    
    (yyval.expr) = rasqal_new_function_expression(rq->world, 
                                        uri, (yyvsp[-2].seq), (yyvsp[-1].seq) /* params */,
                                        flags);
    if((yyval.expr))
      (yyval.expr)->flags |= (yyvsp[-3].uinteger);
  }
  rasqal_free_literal((yyvsp[-5].literal));

  if(!(yyval.expr))
    YYERROR_MSG("FunctionCall: cannot create expr");
}
#line 7181 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 232:
#line 3587 "./sparql_parser.y" /* yacc.c:1646  */
    {
  raptor_uri* uri = rasqal_literal_as_uri((yyvsp[-2].literal));
  
  if(!(yyvsp[-1].seq)) {
    (yyvsp[-1].seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_expression,
                             (raptor_data_print_handler)rasqal_expression_print);
    if(!(yyvsp[-1].seq)) {
      rasqal_free_literal((yyvsp[-2].literal));
      YYERROR_MSG("FunctionCall: cannot create sequence");
    }
  }

  uri = raptor_uri_copy(uri);

  if(raptor_sequence_size((yyvsp[-1].seq)) == 1 &&
     rasqal_xsd_is_datatype_uri(rq->world, uri)) {
    rasqal_expression* e = (rasqal_expression*)raptor_sequence_pop((yyvsp[-1].seq));
    (yyval.expr) = rasqal_new_cast_expression(rq->world, uri, e);
    raptor_free_sequence((yyvsp[-1].seq));
  } else {
    (yyval.expr) = rasqal_new_function_expression(rq->world,
                                        uri, (yyvsp[-1].seq), NULL /* params */,
                                        0 /* flags */);
  }
  rasqal_free_literal((yyvsp[-2].literal));

  if(!(yyval.expr))
    YYERROR_MSG("FunctionCall: cannot create expr");
}
#line 7215 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 233:
#line 3621 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "COALESCE can only be used with SPARQL 1.1");
    YYERROR;
  }
  
  if(!(yyvsp[0].seq)) {
    (yyvsp[0].seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_expression,
                             (raptor_data_print_handler)rasqal_expression_print);
    if(!(yyvsp[0].seq))
      YYERROR_MSG("FunctionCall: cannot create sequence");
  }

  (yyval.expr) = rasqal_new_expr_seq_expression(rq->world, 
                                      RASQAL_EXPR_COALESCE, (yyvsp[0].seq));
  if(!(yyval.expr))
    YYERROR_MSG("Coalesce: cannot create expr");
}
#line 7243 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 234:
#line 3649 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
}
#line 7251 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 235:
#line 3656 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-2].seq);
  if((yyvsp[0].expr))
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("ArgListNoBraces 1: sequence push failed");
    }
}
#line 7265 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 236:
#line 3666 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_expression,
                           (raptor_data_print_handler)rasqal_expression_print);
  if(!(yyval.seq)) {
    if((yyvsp[0].expr))
      rasqal_free_expression((yyvsp[0].expr));
    YYERROR_MSG("ArgListNoBraces 2: cannot create sequence");
  }
  if((yyvsp[0].expr))
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("ArgListNoBraces 2: sequence push failed");
    }
}
#line 7285 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 237:
#line 3682 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_expression,
                           (raptor_data_print_handler)rasqal_expression_print);
}
#line 7294 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 238:
#line 3691 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
}
#line 7302 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 239:
#line 3699 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 7310 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 240:
#line 3703 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_triple,
                           (raptor_data_print_handler)rasqal_triple_print);
  if(!(yyval.seq)) {
    YYERROR_MSG("ConstructTriplesOpt: cannot create sequence");
  }
}
#line 7322 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 241:
#line 3715 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
 
  if((yyvsp[-2].formula)) {
    (yyval.seq) = (yyvsp[-2].formula)->triples;
    (yyvsp[-2].formula)->triples = NULL;
    rasqal_free_formula((yyvsp[-2].formula));
  }
  
  if((yyvsp[0].seq)) {
    if(!(yyval.seq)) {
      (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_triple,
                               (raptor_data_print_handler)rasqal_triple_print);
      if(!(yyval.seq)) {
        raptor_free_sequence((yyvsp[0].seq));
        YYERROR_MSG("ConstructTriples: cannot create sequence");
      }
    }

    if(raptor_sequence_join((yyval.seq), (yyvsp[0].seq))) {
      raptor_free_sequence((yyvsp[0].seq));
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("ConstructTriples: sequence join failed");
    }
    raptor_free_sequence((yyvsp[0].seq));
  }

 }
#line 7356 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 242:
#line 3745 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
  
  if((yyvsp[0].formula)) {
    (yyval.seq) = (yyvsp[0].formula)->triples;
    (yyvsp[0].formula)->triples = NULL;
    rasqal_free_formula((yyvsp[0].formula));
  }
  
}
#line 7371 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 243:
#line 3760 "./sparql_parser.y" /* yacc.c:1646  */
    {
  int i;

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "TriplesSameSubject 1\n  subject=");
  rasqal_formula_print((yyvsp[-1].formula), DEBUG_FH);
  if((yyvsp[0].formula)) {
    fprintf(DEBUG_FH, "\n  propertyList=");
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
  } else     
    fprintf(DEBUG_FH, "\n  and empty propertyList\n");
#endif

  if((yyvsp[0].formula)) {
    raptor_sequence *seq = (yyvsp[0].formula)->triples;
    rasqal_literal *subject = (yyvsp[-1].formula)->value;
    int size = raptor_sequence_size(seq);
    
    /* non-empty property list, handle it  */
    for(i = 0; i < size; i++) {
      rasqal_triple* t2 = (rasqal_triple*)raptor_sequence_get_at(seq, i);
      if(t2->subject)
        continue;
      t2->subject = rasqal_new_literal_from_literal(subject);
    }
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
    fprintf(DEBUG_FH, "  after substitution propertyList=");
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
#endif
  }

  (yyval.formula) = rasqal_formula_join((yyvsp[-1].formula), (yyvsp[0].formula));
  if(!(yyval.formula))
    YYERROR_MSG("TriplesSameSubject 1: formula join failed");

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "  after joining formula=");
  rasqal_formula_print((yyval.formula), DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif
}
#line 7419 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 244:
#line 3804 "./sparql_parser.y" /* yacc.c:1646  */
    {
  int i;

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "TriplesSameSubject 2\n  TriplesNode=");
  rasqal_formula_print((yyvsp[-1].formula), DEBUG_FH);
  if((yyvsp[0].formula)) {
    fprintf(DEBUG_FH, "\n  propertyList=");
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
  } else     
    fprintf(DEBUG_FH, "\n  and empty propertyList\n");
#endif

  if((yyvsp[0].formula)) {
    raptor_sequence *seq = (yyvsp[0].formula)->triples;
    rasqal_literal *subject = (yyvsp[-1].formula)->value;
    int size = raptor_sequence_size(seq);
    
    /* non-empty property list, handle it  */
    for(i = 0; i < size; i++) {
      rasqal_triple* t2 = (rasqal_triple*)raptor_sequence_get_at(seq, i);
      if(t2->subject)
        continue;
      t2->subject = rasqal_new_literal_from_literal(subject);
    }
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
    fprintf(DEBUG_FH, "  after substitution propertyList=");
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
#endif
  }

  (yyval.formula) = rasqal_formula_join((yyvsp[-1].formula), (yyvsp[0].formula));
  if(!(yyval.formula))
    YYERROR_MSG("TriplesSameSubject 2: formula join failed");

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "  after joining formula=");
  rasqal_formula_print((yyval.formula), DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif
}
#line 7467 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 245:
#line 3852 "./sparql_parser.y" /* yacc.c:1646  */
    {
  int i;
  
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "PropertyList 1\n  Verb=");
  rasqal_formula_print((yyvsp[-2].formula), DEBUG_FH);
  fprintf(DEBUG_FH, "\n  ObjectList=");
  rasqal_formula_print((yyvsp[-1].formula), DEBUG_FH);
  fprintf(DEBUG_FH, "\n  PropertyListTail=");
  if((yyvsp[0].formula) != NULL)
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, "\n");
#endif
  
  if((yyvsp[-1].formula) == NULL) {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
    fprintf(DEBUG_FH, " empty ObjectList not processed\n");
#endif
  } else if((yyvsp[-2].formula) && (yyvsp[-1].formula)) {
    raptor_sequence *seq = (yyvsp[-1].formula)->triples;
    rasqal_literal *predicate = (yyvsp[-2].formula)->value;
    rasqal_formula *formula;
    rasqal_triple *t2;
    int size;
    
    formula = rasqal_new_formula(rq->world);
    if(!formula) {
      rasqal_free_formula((yyvsp[-2].formula));
      rasqal_free_formula((yyvsp[-1].formula));
      if((yyvsp[0].formula))
        rasqal_free_formula((yyvsp[0].formula));
      YYERROR_MSG("PropertyList 1: cannot create formula");
    }
    formula->triples = raptor_new_sequence((raptor_data_free_handler)rasqal_free_triple,
                                           (raptor_data_print_handler)rasqal_triple_print);
    if(!formula->triples) {
      rasqal_free_formula(formula);
      rasqal_free_formula((yyvsp[-2].formula));
      rasqal_free_formula((yyvsp[-1].formula));
      if((yyvsp[0].formula))
        rasqal_free_formula((yyvsp[0].formula));
      YYERROR_MSG("PropertyList 1: cannot create sequence");
    }

    /* non-empty property list, handle it  */
    size = raptor_sequence_size(seq);
    for(i = 0; i < size; i++) {
      t2 = (rasqal_triple*)raptor_sequence_get_at(seq, i);
      if(!t2->predicate)
        t2->predicate = (rasqal_literal*)rasqal_new_literal_from_literal(predicate);
    }
  
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
    fprintf(DEBUG_FH, "  after substitution ObjectList=");
    raptor_sequence_print(seq, DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
#endif

    while(raptor_sequence_size(seq)) {
      t2 = (rasqal_triple*)raptor_sequence_unshift(seq);
      if(raptor_sequence_push(formula->triples, t2)) {
        rasqal_free_formula(formula);
        rasqal_free_formula((yyvsp[-2].formula));
        rasqal_free_formula((yyvsp[-1].formula));
        if((yyvsp[0].formula))
          rasqal_free_formula((yyvsp[0].formula));
        YYERROR_MSG("PropertyList 1: sequence push failed");
      }
    }

    (yyvsp[0].formula) = rasqal_formula_join(formula, (yyvsp[0].formula));
    if(!(yyvsp[0].formula)) {
      rasqal_free_formula((yyvsp[-2].formula));
      rasqal_free_formula((yyvsp[-1].formula));
      YYERROR_MSG("PropertyList 1: formula join failed");
    }

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
    fprintf(DEBUG_FH, "  after appending ObjectList=");
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n\n");
#endif

    rasqal_free_formula((yyvsp[-1].formula));
  }

  if((yyvsp[-2].formula))
    rasqal_free_formula((yyvsp[-2].formula));

  (yyval.formula) = (yyvsp[0].formula);
}
#line 7565 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 246:
#line 3950 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = (yyvsp[0].formula);
}
#line 7573 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 247:
#line 3954 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = NULL;
}
#line 7581 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 248:
#line 3962 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = (yyvsp[0].formula);
}
#line 7589 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 249:
#line 3966 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = NULL;
}
#line 7597 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 250:
#line 3974 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_formula *formula;
  rasqal_triple *triple;

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "ObjectList 1\n");
  fprintf(DEBUG_FH, "  Object=\n");
  rasqal_formula_print((yyvsp[-1].formula), DEBUG_FH);
  fprintf(DEBUG_FH, "\n");
  if((yyvsp[0].formula)) {
    fprintf(DEBUG_FH, "  ObjectTail=");
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
  } else
    fprintf(DEBUG_FH, "  and empty ObjectTail\n");
#endif

  formula = rasqal_new_formula(rq->world);
  if(!formula) {
    rasqal_free_formula((yyvsp[-1].formula));
    if((yyvsp[0].formula))
      rasqal_free_formula((yyvsp[0].formula));
    YYERROR_MSG("ObjectList: cannot create formula");
  }
  
  formula->triples = raptor_new_sequence((raptor_data_free_handler)rasqal_free_triple,
                                         (raptor_data_print_handler)rasqal_triple_print);
  if(!formula->triples) {
    rasqal_free_formula(formula);
    rasqal_free_formula((yyvsp[-1].formula));
    if((yyvsp[0].formula))
      rasqal_free_formula((yyvsp[0].formula));
    YYERROR_MSG("ObjectList: cannot create sequence");
  }

  triple = rasqal_new_triple(NULL, NULL, (yyvsp[-1].formula)->value);
  (yyvsp[-1].formula)->value = NULL; /* value now owned by triple */
  if(!triple) {
    rasqal_free_formula(formula);
    rasqal_free_formula((yyvsp[-1].formula));
    if((yyvsp[0].formula))
      rasqal_free_formula((yyvsp[0].formula));
    YYERROR_MSG("ObjectList: cannot create triple");
  }

  if(raptor_sequence_push(formula->triples, triple)) {
    rasqal_free_formula(formula);
    rasqal_free_formula((yyvsp[-1].formula));
    if((yyvsp[0].formula))
      rasqal_free_formula((yyvsp[0].formula));
    YYERROR_MSG("ObjectList: sequence push failed");
  }

  (yyval.formula) = rasqal_formula_join(formula, (yyvsp[-1].formula));
  if(!(yyval.formula)) {
    if((yyvsp[0].formula))
      rasqal_free_formula((yyvsp[0].formula));
    YYERROR_MSG("ObjectList: formula join $1 failed");
  }

  (yyval.formula) = rasqal_formula_join((yyval.formula), (yyvsp[0].formula));
  if(!(yyval.formula))
    YYERROR_MSG("ObjectList: formula join $2 failed");

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "  objectList is now ");
  if((yyval.formula))
    raptor_sequence_print((yyval.formula)->triples, DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif
}
#line 7675 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 251:
#line 4052 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = (yyvsp[0].formula);
}
#line 7683 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 252:
#line 4056 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = NULL;
}
#line 7691 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 253:
#line 4064 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = (yyvsp[0].formula);
}
#line 7699 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 254:
#line 4072 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = rasqal_new_formula(rq->world);
  if(!(yyval.formula)) {
    if((yyvsp[0].literal))
      rasqal_free_literal((yyvsp[0].literal));
    YYERROR_MSG("Verb 1: cannot create formula");
  }
  (yyval.formula)->value = (yyvsp[0].literal);
}
#line 7713 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 255:
#line 4082 "./sparql_parser.y" /* yacc.c:1646  */
    {
  raptor_uri *uri;

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "verb Verb=rdf:type (a)\n");
#endif

  uri = raptor_new_uri_for_rdf_concept(rq->world->raptor_world_ptr,
                                       RASQAL_GOOD_CAST(const unsigned char*, "type"));
  if(!uri)
    YYERROR_MSG("Verb 2: uri for rdf concept type failed");
  (yyval.formula) = rasqal_new_formula(rq->world);
  if(!(yyval.formula)) {
    raptor_free_uri(uri);
    YYERROR_MSG("Verb 2: cannot create formula");
  }
  (yyval.formula)->value = rasqal_new_uri_literal(rq->world, uri);
  if(!(yyval.formula)->value) {
    rasqal_free_formula((yyval.formula));
    (yyval.formula) = NULL;
    YYERROR_MSG("Verb 2: cannot create uri literal");
  }
}
#line 7741 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 256:
#line 4110 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = (yyvsp[0].formula);
}
#line 7749 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 257:
#line 4114 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = (yyvsp[0].formula);
}
#line 7757 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 258:
#line 4122 "./sparql_parser.y" /* yacc.c:1646  */
    {
  int i;
  const unsigned char *id;

  if((yyvsp[-1].formula) == NULL) {
    (yyval.formula) = rasqal_new_formula(rq->world);
    if(!(yyval.formula))
      YYERROR_MSG("BlankNodePropertyList: cannot create formula");
  } else {
    (yyval.formula) = (yyvsp[-1].formula);
    if((yyval.formula)->value) {
      rasqal_free_literal((yyval.formula)->value);
      (yyval.formula)->value = NULL;
    }
  }
  
  id = rasqal_query_generate_bnodeid(rq, NULL);
  if(!id) {
    rasqal_free_formula((yyval.formula));
    (yyval.formula) = NULL;
    YYERROR_MSG("BlankNodeProperyList: cannot create bnodeid");
  }

  (yyval.formula)->value = rasqal_new_simple_literal(rq->world,
                                        RASQAL_LITERAL_BLANK, id);
  if(!(yyval.formula)->value) {
    rasqal_free_formula((yyval.formula));
    (yyval.formula) = NULL;
    YYERROR_MSG("BlankNodePropertyList: cannot create literal");
  }

  if((yyvsp[-1].formula) == NULL) {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
    fprintf(DEBUG_FH, "TriplesNode\n  PropertyList=");
    rasqal_formula_print((yyval.formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
#endif
  } else {
    raptor_sequence *seq = (yyvsp[-1].formula)->triples;

    /* non-empty property list, handle it  */
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
    fprintf(DEBUG_FH, "TriplesNode\n  PropertyList=");
    raptor_sequence_print(seq, DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
#endif

    for(i = 0; i<raptor_sequence_size(seq); i++) {
      rasqal_triple* t2 = (rasqal_triple*)raptor_sequence_get_at(seq, i);
      if(t2->subject)
        continue;
      
      t2->subject = (rasqal_literal*)rasqal_new_literal_from_literal((yyval.formula)->value);
    }

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1
    fprintf(DEBUG_FH, "  after substitution formula=");
    rasqal_formula_print((yyval.formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n\n");
#endif
  }
}
#line 7824 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 259:
#line 4189 "./sparql_parser.y" /* yacc.c:1646  */
    {
  int i;
  rasqal_literal* first_identifier = NULL;
  rasqal_literal* rest_identifier = NULL;
  rasqal_literal* object = NULL;
  rasqal_literal* blank = NULL;
  char const *errmsg = NULL;

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "Collection\n  GraphNodeListNotEmpty=");
  raptor_sequence_print((yyvsp[-1].seq), DEBUG_FH);
  fprintf(DEBUG_FH, "\n");
#endif

  (yyval.formula) = rasqal_new_formula(rq->world);
  if(!(yyval.formula))
    YYERR_MSG_GOTO(err_Collection, "Collection: cannot create formula");

  (yyval.formula)->triples = raptor_new_sequence((raptor_data_free_handler)rasqal_free_triple,
                                    (raptor_data_print_handler)rasqal_triple_print);
  if(!(yyval.formula)->triples)
    YYERR_MSG_GOTO(err_Collection, "Collection: cannot create sequence");

  first_identifier = rasqal_new_uri_literal(rq->world,
                                            raptor_uri_copy(rq->world->rdf_first_uri));
  if(!first_identifier)
    YYERR_MSG_GOTO(err_Collection, "Collection: cannot first_identifier");
  
  rest_identifier = rasqal_new_uri_literal(rq->world,
                                           raptor_uri_copy(rq->world->rdf_rest_uri));
  if(!rest_identifier)
    YYERR_MSG_GOTO(err_Collection, "Collection: cannot create rest_identifier");
  
  object = rasqal_new_uri_literal(rq->world,
                                  raptor_uri_copy(rq->world->rdf_nil_uri));
  if(!object)
    YYERR_MSG_GOTO(err_Collection, "Collection: cannot create nil object");

  for(i = raptor_sequence_size((yyvsp[-1].seq))-1; i >= 0; i--) {
    rasqal_formula* f = (rasqal_formula*)raptor_sequence_get_at((yyvsp[-1].seq), i);
    rasqal_triple *t2;
    const unsigned char *blank_id = NULL;

    blank_id = rasqal_query_generate_bnodeid(rq, NULL);
    if(!blank_id)
      YYERR_MSG_GOTO(err_Collection, "Collection: cannot create bnodeid");

    blank = rasqal_new_simple_literal(rq->world, RASQAL_LITERAL_BLANK, blank_id);
    if(!blank)
      YYERR_MSG_GOTO(err_Collection, "Collection: cannot create bnode");

    /* Move existing formula triples */
    if(f->triples)
      if(raptor_sequence_join((yyval.formula)->triples, f->triples))
        YYERR_MSG_GOTO(err_Collection, "Collection: sequence join failed");

    /* add new triples we needed */
    t2 = rasqal_new_triple(rasqal_new_literal_from_literal(blank),
                           rasqal_new_literal_from_literal(first_identifier),
                           rasqal_new_literal_from_literal(f->value));
    if(!t2)
      YYERR_MSG_GOTO(err_Collection, "Collection: cannot create triple");

    if(raptor_sequence_push((yyval.formula)->triples, t2))
      YYERR_MSG_GOTO(err_Collection, "Collection: cannot create triple");

    t2 = rasqal_new_triple(rasqal_new_literal_from_literal(blank),
                           rasqal_new_literal_from_literal(rest_identifier),
                           rasqal_new_literal_from_literal(object));
    if(!t2)
      YYERR_MSG_GOTO(err_Collection, "Collection: cannot create triple 2");

    if(raptor_sequence_push((yyval.formula)->triples, t2))
      YYERR_MSG_GOTO(err_Collection, "Collection: sequence push 2 failed");

    rasqal_free_literal(object);
    object=blank;
    blank = NULL;
  }

  /* free sequence of formulas just processed */
  raptor_free_sequence((yyvsp[-1].seq));
  
  (yyval.formula)->value=object;
  
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1
  fprintf(DEBUG_FH, "  after substitution collection=");
  rasqal_formula_print((yyval.formula), DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif

  rasqal_free_literal(first_identifier);
  rasqal_free_literal(rest_identifier);

  err_Collection:
  if(errmsg) {
    if(blank)
      rasqal_free_literal(blank);
    if(object)
      rasqal_free_literal(object);
    if(rest_identifier)
      rasqal_free_literal(rest_identifier);
    if(first_identifier)
      rasqal_free_literal(first_identifier);
    if((yyvsp[-1].seq))
      raptor_free_sequence((yyvsp[-1].seq));
    if((yyval.formula)) {
      rasqal_free_formula((yyval.formula));
      (yyval.formula) = NULL;
    }
    YYERROR_MSG(errmsg);
  }
}
#line 7942 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 260:
#line 4308 "./sparql_parser.y" /* yacc.c:1646  */
    {
  char const *errmsg = NULL;
  
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "GraphNodeListNotEmpty 1\n");
  if((yyvsp[0].formula)) {
    fprintf(DEBUG_FH, "  GraphNode=");
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
  } else  
    fprintf(DEBUG_FH, "  and empty GraphNode\n");
  if((yyvsp[-1].seq)) {
    fprintf(DEBUG_FH, "  GraphNodeListNotEmpty=");
    raptor_sequence_print((yyvsp[-1].seq), DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
  } else
    fprintf(DEBUG_FH, "  and empty GraphNodeListNotEmpty\n");
#endif

  (yyval.seq) = (yyvsp[-1].seq);
  if(!(yyval.seq)) {
    (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_formula,
                             (raptor_data_print_handler)rasqal_formula_print);
    if(!(yyval.seq))
      YYERR_MSG_GOTO(err_GraphNodeListNotEmpty,
                     "GraphNodeListNotEmpty: cannot create formula");
  }
  
  if((yyvsp[0].formula)) {
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].formula))) {
      YYERR_MSG_GOTO(err_GraphNodeListNotEmpty,
                     "GraphNodeListNotEmpty 1: sequence push failed");
    }
    (yyvsp[0].formula) = NULL;
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
    fprintf(DEBUG_FH, "  itemList is now ");
    raptor_sequence_print((yyval.seq), DEBUG_FH);
    fprintf(DEBUG_FH, "\n\n");
#endif
  }

  err_GraphNodeListNotEmpty:
  if(errmsg) {
    if((yyvsp[0].formula))
      rasqal_free_formula((yyvsp[0].formula));
    if((yyval.seq)) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
    }
    YYERROR_MSG(errmsg);
  }
}
#line 7999 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 261:
#line 4361 "./sparql_parser.y" /* yacc.c:1646  */
    {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "GraphNodeListNotEmpty 2\n");
  if((yyvsp[0].formula)) {
    fprintf(DEBUG_FH, "  GraphNode=");
    rasqal_formula_print((yyvsp[0].formula), DEBUG_FH);
    fprintf(DEBUG_FH, "\n");
  } else  
    fprintf(DEBUG_FH, "  and empty GraphNode\n");
#endif

  if(!(yyvsp[0].formula))
    (yyval.seq) = NULL;
  else {
    (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)rasqal_free_formula,
                             (raptor_data_print_handler)rasqal_formula_print);
    if(!(yyval.seq)) {
      rasqal_free_formula((yyvsp[0].formula));
      YYERROR_MSG("GraphNodeListNotEmpty 2: cannot create sequence");
    }
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].formula))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("GraphNodeListNotEmpty 2: sequence push failed");
    }
  }
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1  
  fprintf(DEBUG_FH, "  GraphNodeListNotEmpty is now ");
  raptor_sequence_print((yyval.seq), DEBUG_FH);
  fprintf(DEBUG_FH, "\n\n");
#endif
}
#line 8036 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 262:
#line 4398 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = (yyvsp[0].formula);
}
#line 8044 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 263:
#line 4402 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = (yyvsp[0].formula);
}
#line 8052 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 264:
#line 4410 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = rasqal_new_formula(rq->world);
  if(!(yyval.formula))
    YYERROR_MSG("VarOrTerm 1: cannot create formula");
  (yyval.formula)->value = rasqal_new_variable_literal(rq->world, (yyvsp[0].variable));
  if(!(yyval.formula)->value) {
    rasqal_free_formula((yyval.formula));
    (yyval.formula) = NULL;
    YYERROR_MSG("VarOrTerm 1: cannot create literal");
  }
}
#line 8068 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 265:
#line 4422 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.formula) = rasqal_new_formula(rq->world);
  if(!(yyval.formula)) {
    if((yyvsp[0].literal))
      rasqal_free_literal((yyvsp[0].literal));
    YYERROR_MSG("VarOrTerm 2: cannot create formula");
  }
  (yyval.formula)->value = (yyvsp[0].literal);
}
#line 8082 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 266:
#line 4435 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_variable_literal(rq->world, (yyvsp[0].variable));
  if(!(yyval.literal))
    YYERROR_MSG("VarOrIRIref: cannot create literal");
}
#line 8092 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 267:
#line 4441 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 8100 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 268:
#line 4449 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.variable) = (yyvsp[0].variable);
}
#line 8108 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 269:
#line 4453 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.variable) = (yyvsp[0].variable);
}
#line 8116 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 270:
#line 4460 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.variable) = rasqal_variables_table_add2(rq->vars_table,
                                   RASQAL_VARIABLE_TYPE_NORMAL, (yyvsp[0].name), 0, NULL);
  if(!(yyval.variable))
    YYERROR_MSG("VarName: cannot create var");
  RASQAL_FREE(char*, (yyvsp[0].name));
}
#line 8128 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 271:
#line 4472 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.variable) = (yyvsp[0].variable);
}
#line 8136 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 272:
#line 4476 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.variable) = (yyvsp[0].variable);
}
#line 8144 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 273:
#line 4480 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.variable) = (yyvsp[0].variable);
  sparql_syntax_warning(rq,
                        "... AS varname is deprecated LAQRS syntax, use ... AS ?varname");
}
#line 8154 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 274:
#line 4490 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 8162 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 275:
#line 4494 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 8170 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 276:
#line 4498 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 8178 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 277:
#line 4502 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 8186 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 278:
#line 4506 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 8194 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 279:
#line 4510 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_uri_literal(rq->world, 
                              raptor_uri_copy(rq->world->rdf_nil_uri));
  if(!(yyval.literal))
    YYERROR_MSG("GraphTerm: cannot create literal");
}
#line 8205 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 280:
#line 4520 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8213 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 281:
#line 4528 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_OR, (yyvsp[-2].expr), (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("ConditionalOrExpression: cannot create expr");
}
#line 8224 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 282:
#line 4535 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8232 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 283:
#line 4543 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_AND, (yyvsp[-2].expr), (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("ConditionalAndExpression: cannot create expr");
;
}
#line 8244 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 284:
#line 4551 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8252 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 285:
#line 4560 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_EQ, (yyvsp[-2].expr), (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("RelationalExpression 1: cannot create expr");
}
#line 8263 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 286:
#line 4567 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_NEQ, (yyvsp[-2].expr), (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("RelationalExpression 2: cannot create expr");
}
#line 8274 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 287:
#line 4574 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_LT, (yyvsp[-2].expr), (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("RelationalExpression 3: cannot create expr");
}
#line 8285 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 288:
#line 4581 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_GT, (yyvsp[-2].expr), (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("RelationalExpression 4: cannot create expr");
}
#line 8296 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 289:
#line 4588 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_LE, (yyvsp[-2].expr), (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("RelationalExpression 5: cannot create expr");
}
#line 8307 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 290:
#line 4595 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_GE, (yyvsp[-2].expr), (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("RelationalExpression 6: cannot create expr");
}
#line 8318 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 291:
#line 4602 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_set_expression(rq->world,
                                 RASQAL_EXPR_IN, (yyvsp[-2].expr), (yyvsp[0].seq));
}
#line 8327 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 292:
#line 4607 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_set_expression(rq->world,
                                 RASQAL_EXPR_NOT_IN, (yyvsp[-3].expr), (yyvsp[0].seq));
}
#line 8336 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 293:
#line 4612 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8344 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 294:
#line 4654 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[-1].expr);

  if((yyvsp[0].seq)) {
    int i;
    int size = raptor_sequence_size((yyvsp[0].seq));

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1
    RASQAL_DEBUG1("AdExOpExpressionListOuter sequence: ");
    if((yyvsp[0].seq))
      raptor_sequence_print((yyvsp[0].seq), DEBUG_FH);
    else
      fputs("NULL", DEBUG_FH);
    fputc('\n', DEBUG_FH);
#endif

    /* Walk sequence forming tree of exprs in $$ */
    for(i = 0; i < size; i++) {
      sparql_op_expr* op_expr = (sparql_op_expr*)raptor_sequence_get_at((yyvsp[0].seq), i);
      (yyval.expr) = rasqal_new_2op_expression(rq->world, op_expr->op, (yyval.expr), op_expr->expr);
      op_expr->expr = NULL;
    }
    raptor_free_sequence((yyvsp[0].seq));
  }
}
#line 8374 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 295:
#line 4680 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8382 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 296:
#line 4687 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);

  if((yyvsp[0].seq)) {
    if(raptor_sequence_join((yyval.seq), (yyvsp[0].seq))) {
      raptor_free_sequence((yyvsp[0].seq));
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("AdExOpExpressionListOuter: sequence join failed");
    }
    raptor_free_sequence((yyvsp[0].seq));
  }
}
#line 8400 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 297:
#line 4701 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);
}
#line 8408 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 298:
#line 4707 "./sparql_parser.y" /* yacc.c:1646  */
    {
  sparql_op_expr* oe;

  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)free_op_expr,
                           (raptor_data_print_handler)print_op_expr);
  if(!(yyval.seq))
    YYERROR_MSG("AdExOpExpressionListInner 1: failed to create sequence");

  oe = new_op_expr(RASQAL_EXPR_PLUS, (yyvsp[0].expr));
  if(!oe)
    YYERROR_MSG("AdExOpExpressionListInner 1: cannot create plus expr");

  if(raptor_sequence_push((yyval.seq), oe)) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("AdExOpExpressionListInner 1: sequence push failed");
  }
}
#line 8431 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 299:
#line 4726 "./sparql_parser.y" /* yacc.c:1646  */
    {
  sparql_op_expr* oe;

  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)free_op_expr,
                           (raptor_data_print_handler)print_op_expr);
  if(!(yyval.seq))
    YYERROR_MSG("AdExOpExpressionListInner 2: failed to create sequence");

  oe = new_op_expr(RASQAL_EXPR_MINUS, (yyvsp[0].expr));
  if(!oe)
    YYERROR_MSG("AdExOpExpressionListInner 2: cannot create minus expr");

  if(raptor_sequence_push((yyval.seq), oe)) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("AdExOpExpressionListInner 2: sequence push failed");
  }
}
#line 8454 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 300:
#line 4745 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_expression *e;
  sparql_op_expr* oe;

  (yyval.seq) = (yyvsp[0].seq);
  if(!(yyval.seq)) {
    (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)free_op_expr,
                             (raptor_data_print_handler)print_op_expr);
    if(!(yyval.seq))
      YYERROR_MSG("AdExOpExpressionListInner 2: failed to create sequence");
  }

  e = rasqal_new_literal_expression(rq->world, (yyvsp[-1].literal));
  if(!e)
    YYERROR_MSG("AdExOpExpressionListInner 2: cannot create NumericLiteralPositive literal expression");
  oe = new_op_expr(RASQAL_EXPR_PLUS, e);
  if(!oe)
    YYERROR_MSG("AdExOpExpressionListInner 2: cannot create plus expr");
  raptor_sequence_shift((yyval.seq), oe);
}
#line 8479 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 301:
#line 4766 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_expression *e;
  sparql_op_expr* oe;

  (yyval.seq) = (yyvsp[0].seq);
  if(!(yyval.seq)) {
    (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)free_op_expr,
                             (raptor_data_print_handler)print_op_expr);
    if(!(yyval.seq))
      YYERROR_MSG("AdExOpExpressionListInner 3: failed to create sequence");
  }

  e = rasqal_new_literal_expression(rq->world, (yyvsp[-1].literal));
  if(!e)
    YYERROR_MSG("AdExOpExpressionListInner 3: cannot create NumericLiteralNegative literal expression");
  oe = new_op_expr(RASQAL_EXPR_MINUS, e);
  if(!oe)
    YYERROR_MSG("AdExOpExpressionListInner 3: cannot create minus expr");
  raptor_sequence_shift((yyval.seq), oe);
}
#line 8504 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 302:
#line 4789 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[0].seq);

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1
  RASQAL_DEBUG1("AEListOpt sequence: ");
  if((yyval.seq))
    raptor_sequence_print((yyval.seq), DEBUG_FH);
  else
    fputs("NULL", DEBUG_FH);
  fputc('\n', DEBUG_FH);
#endif
}
#line 8521 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 303:
#line 4802 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = NULL;
}
#line 8529 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 304:
#line 4808 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);

  if((yyval.seq) && (yyvsp[0].op_expr)) {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1
    RASQAL_DEBUG1("AdExOpUnaryExpressionList adding AdExOpUnaryExpression: ");
    print_op_expr((yyvsp[0].op_expr), DEBUG_FH);
    fputc('\n', DEBUG_FH);
#endif

    if(raptor_sequence_push((yyval.seq), (yyvsp[0].op_expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("AdExOpUnaryExpressionListOpt 1: sequence push failed");
    }
  }
}
#line 8551 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 305:
#line 4826 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)free_op_expr,
                           (raptor_data_print_handler)print_op_expr);
  if(!(yyval.seq))
    YYERROR_MSG("AdExOpUnaryExpressionListOpt 2: failed to create sequence");

  if((yyvsp[0].op_expr)) {
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1
    RASQAL_DEBUG1("AdExOpUnaryExpressionList adding AdExOpUnaryExpression: ");
    print_op_expr((yyvsp[0].op_expr), DEBUG_FH);
    fputc('\n', DEBUG_FH);
#endif

    if(raptor_sequence_push((yyval.seq), (yyvsp[0].op_expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("AdExOpUnaryExpressionListOpt 2: sequence push failed");
    }
  }
}
#line 8576 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 306:
#line 4849 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.op_expr) = new_op_expr(RASQAL_EXPR_STAR, (yyvsp[0].expr));
  if(!(yyval.op_expr))
    YYERROR_MSG("AdExOpUnaryExpression 1: cannot create star expr");
}
#line 8586 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 307:
#line 4855 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.op_expr) = new_op_expr(RASQAL_EXPR_SLASH, (yyvsp[0].expr));
  if(!(yyval.op_expr))
    YYERROR_MSG("AdExOpUnaryExpression 2: cannot create slash expr");
}
#line 8596 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 308:
#line 4883 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[-1].expr);

  if((yyvsp[0].seq)) {
    int i;
    int size = raptor_sequence_size((yyvsp[0].seq));

#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 1
    RASQAL_DEBUG1("MuExOpUnaryExpressionList sequence: ");
    raptor_sequence_print((yyvsp[0].seq), DEBUG_FH);
    fputc('\n', DEBUG_FH);
#endif

    /* Walk sequence forming tree of exprs in $$ */
    for(i = 0; i < size; i++) {
      sparql_op_expr* op_expr = (sparql_op_expr*)raptor_sequence_get_at((yyvsp[0].seq), i);
      (yyval.expr) = rasqal_new_2op_expression(rq->world, op_expr->op, (yyval.expr), op_expr->expr);
      op_expr->expr = NULL;
    }
    raptor_free_sequence((yyvsp[0].seq));
  }
}
#line 8623 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 309:
#line 4906 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8631 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 310:
#line 4912 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = (yyvsp[-1].seq);
  if((yyval.seq) && (yyvsp[0].op_expr)) {
    if(raptor_sequence_push((yyval.seq), (yyvsp[0].op_expr))) {
      raptor_free_sequence((yyval.seq));
      (yyval.seq) = NULL;
      YYERROR_MSG("MuExOpUnaryExpressionListOpt 1: sequence push failed");
    }
  }
}
#line 8646 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 311:
#line 4923 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.seq) = raptor_new_sequence((raptor_data_free_handler)free_op_expr,
                           (raptor_data_print_handler)print_op_expr);
  if(!(yyval.seq))
    YYERROR_MSG("MuExOpUnaryExpressionListOpt 2: failed to create sequence");

  if(raptor_sequence_push((yyval.seq), (yyvsp[0].op_expr))) {
    raptor_free_sequence((yyval.seq));
    (yyval.seq) = NULL;
    YYERROR_MSG("MuExOpUnaryExpressionListOpt 2: sequence push failed");
  }
}
#line 8663 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 312:
#line 4939 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.op_expr) = new_op_expr(RASQAL_EXPR_STAR, (yyvsp[0].expr));
  if(!(yyval.op_expr))
    YYERROR_MSG("MuExOpUnaryExpression 1: cannot create star expr");
}
#line 8673 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 313:
#line 4945 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.op_expr) = new_op_expr(RASQAL_EXPR_SLASH, (yyvsp[0].expr));
  if(!(yyval.op_expr))
    YYERROR_MSG("MuExOpUnaryExpression 2: cannot create slash expr");
}
#line 8683 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 314:
#line 4956 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_BANG, (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("UnaryExpression 1: cannot create expr");
}
#line 8694 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 315:
#line 4963 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8702 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 316:
#line 4967 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_UMINUS, (yyvsp[0].expr));
  if(!(yyval.expr))
    YYERROR_MSG("UnaryExpression 3: cannot create expr");
}
#line 8713 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 317:
#line 4974 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8721 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 318:
#line 4988 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8729 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 319:
#line 4992 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8737 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 320:
#line 4996 "./sparql_parser.y" /* yacc.c:1646  */
    {
  /* Grammar has IRIrefOrFunction here which is "IRIref ArgList?"
   * and essentially shorthand for FunctionCall | IRIref.  The Rasqal
   * SPARQL lexer distinguishes these for us with IRIrefBrace.
   * IRIref is covered below by GraphTerm.
   */
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8750 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 321:
#line 5005 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_literal_expression(rq->world, (yyvsp[0].literal));
  if(!(yyval.expr))
    YYERROR_MSG("PrimaryExpression 4: cannot create expr");
}
#line 8760 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 322:
#line 5011 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_literal *l;
  l = rasqal_new_variable_literal(rq->world, (yyvsp[0].variable));
  if(!l)
    YYERROR_MSG("PrimaryExpression 5: cannot create literal");
  (yyval.expr) = rasqal_new_literal_expression(rq->world, l);
  if(!(yyval.expr))
    YYERROR_MSG("PrimaryExpression 5: cannot create expr");
}
#line 8774 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 323:
#line 5021 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 8782 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 324:
#line 5029 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[-1].expr);
}
#line 8790 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 325:
#line 5037 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_STR, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 1: cannot create expr");
}
#line 8801 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 326:
#line 5044 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_LANG, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 2: cannot create expr");
}
#line 8812 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 327:
#line 5051 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_LANGMATCHES, (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 3: cannot create expr");
}
#line 8823 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 328:
#line 5058 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_DATATYPE, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 4: cannot create expr");
}
#line 8834 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 329:
#line 5065 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_literal *l;
  rasqal_expression *e;
  l = rasqal_new_variable_literal(rq->world, (yyvsp[-1].variable));
  if(!l)
    YYERROR_MSG("BuiltInCall 5: cannot create literal");
  e = rasqal_new_literal_expression(rq->world, l);
  if(!e)
    YYERROR_MSG("BuiltInCall 6: cannot create literal expr");
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_BOUND, e);
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7: cannot create expr");
}
#line 8853 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 330:
#line 5080 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_IRI, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7a: cannot create expr");
}
#line 8864 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 331:
#line 5087 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_IRI, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7b: cannot create expr");
}
#line 8875 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 332:
#line 5094 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_BNODE, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7c: cannot create expr");
}
#line 8886 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 333:
#line 5101 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_BNODE, NULL);
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7d: cannot create expr");
}
#line 8897 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 334:
#line 5108 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_0op_expression(rq->world, 
                                 RASQAL_EXPR_RAND);
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7e: cannot create expr");
}
#line 8908 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 335:
#line 5115 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_ABS, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7f: cannot create expr");
}
#line 8919 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 336:
#line 5122 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_CEIL, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7g: cannot create expr");
}
#line 8930 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 337:
#line 5129 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_FLOOR, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7h: cannot create expr");
}
#line 8941 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 338:
#line 5136 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_ROUND, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7i: cannot create expr");
}
#line 8952 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 339:
#line 5143 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_MD5, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7j: cannot create expr");
}
#line 8963 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 340:
#line 5150 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_SHA1, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7k: cannot create expr");
}
#line 8974 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 341:
#line 5157 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_SHA224, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7l: cannot create expr");
}
#line 8985 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 342:
#line 5164 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_SHA256, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7m: cannot create expr");
}
#line 8996 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 343:
#line 5171 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_SHA384, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7n: cannot create expr");
}
#line 9007 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 344:
#line 5178 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world, 
                                 RASQAL_EXPR_SHA512, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7o: cannot create expr");
}
#line 9018 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 345:
#line 5185 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_0op_expression(rq->world, 
                                 RASQAL_EXPR_UUID);
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7p: cannot create expr");
}
#line 9029 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 346:
#line 5192 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_0op_expression(rq->world, 
                                 RASQAL_EXPR_STRUUID);
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7q: cannot create expr");
}
#line 9040 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 347:
#line 5199 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 9048 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 348:
#line 5203 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 9056 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 349:
#line 5207 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_3op_expression(rq->world,
                                 RASQAL_EXPR_IF, (yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7e: cannot create expr");
}
#line 9067 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 350:
#line 5214 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_STRLANG, (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7f: cannot create expr");
}
#line 9078 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 351:
#line 5221 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_STRDT, (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 7g: cannot create expr");
}
#line 9089 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 352:
#line 5228 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_SAMETERM, (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 8: cannot create expr");
}
#line 9100 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 353:
#line 5235 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ISURI, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 9: cannot create expr");
}
#line 9111 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 354:
#line 5242 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ISBLANK, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 10: cannot create expr");
}
#line 9122 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 355:
#line 5249 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ISLITERAL, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 11: cannot create expr");
}
#line 9133 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 356:
#line 5256 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ISNUMERIC, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("BuiltInCall 12: cannot create expr");
}
#line 9144 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 357:
#line 5263 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 9152 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 358:
#line 5267 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 9160 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 359:
#line 5271 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 9168 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 360:
#line 5278 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_STRLEN, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create STRLEN() expr");
}
#line 9179 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 361:
#line 5285 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_3op_expression(rq->world,
                                 RASQAL_EXPR_SUBSTR, (yyvsp[-3].expr), (yyvsp[-1].expr), NULL);
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create SUBSTR() expr");
}
#line 9190 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 362:
#line 5292 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_3op_expression(rq->world,
                                 RASQAL_EXPR_SUBSTR, (yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create SUBSTR() expr");
}
#line 9201 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 363:
#line 5299 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_UCASE, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create UCASE() expr");
}
#line 9212 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 364:
#line 5306 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_LCASE, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create LCASE() expr");
}
#line 9223 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 365:
#line 5313 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_STRSTARTS, (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create STRSTARTS() expr");
}
#line 9234 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 366:
#line 5320 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_STRENDS, (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create STRENDS() expr");
}
#line 9245 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 367:
#line 5327 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_CONTAINS, (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create YEAR expr");
}
#line 9256 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 368:
#line 5334 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_ENCODE_FOR_URI, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create ENCODE_FOR_URI() expr");
}
#line 9267 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 369:
#line 5341 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_expr_seq_expression(rq->world, 
                                      RASQAL_EXPR_CONCAT, (yyvsp[-1].seq));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create CONCAT() expr");
}
#line 9278 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 370:
#line 5348 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_STRBEFORE, (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create STRBEFORE() expr");
}
#line 9289 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 371:
#line 5355 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_2op_expression(rq->world,
                                 RASQAL_EXPR_STRAFTER, (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create STRAFTER() expr");
}
#line 9300 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 372:
#line 5362 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_3op_expression(rq->world,
                                 RASQAL_EXPR_REPLACE, (yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create REPLACE() expr");
}
#line 9311 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 373:
#line 5369 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_4op_expression(rq->world,
                                 RASQAL_EXPR_REPLACE, (yyvsp[-7].expr), (yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("StringExpression: cannot create REPLACE() expr");
}
#line 9322 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 374:
#line 5380 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_3op_expression(rq->world,
                                 RASQAL_EXPR_REGEX, (yyvsp[-3].expr), (yyvsp[-1].expr), NULL);
  if(!(yyval.expr))
    YYERROR_MSG("RegexExpression 1: cannot create expr");
}
#line 9333 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 375:
#line 5387 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_3op_expression(rq->world,
                                 RASQAL_EXPR_REGEX, (yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("RegexExpression 2: cannot create expr");
}
#line 9344 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 376:
#line 5398 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_YEAR, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("DatetimeBuiltinAccessors: cannot create YEAR expr");
}
#line 9355 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 377:
#line 5405 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_MONTH, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("DatetimeBuiltinAccessors: cannot create MONTH expr");
}
#line 9366 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 378:
#line 5412 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_DAY, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("DatetimeBuiltinAccessors: cannot create DAY expr");
}
#line 9377 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 379:
#line 5419 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_HOURS, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("DatetimeBuiltinAccessors: cannot create HOURS expr");
}
#line 9388 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 380:
#line 5426 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_MINUTES, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("DatetimeBuiltinAccessors: cannot create MINUTES expr");
}
#line 9399 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 381:
#line 5433 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_SECONDS, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("DatetimeBuiltinAccessors: cannot create SECONDS expr");
}
#line 9410 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 382:
#line 5440 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_TIMEZONE, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("DatetimeBuiltinAccessors: cannot create TIMEZONE expr");
}
#line 9421 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 383:
#line 5447 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                 RASQAL_EXPR_TZ, (yyvsp[-1].expr));
  if(!(yyval.expr))
    YYERROR_MSG("DatetimeBuiltinAccessors: cannot create TZ expr");
}
#line 9432 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 384:
#line 5458 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(sparql->experimental) {
    (yyval.expr) = rasqal_new_0op_expression(rq->world,
                                   RASQAL_EXPR_CURRENT_DATETIME);
    if(!(yyval.expr))
      YYERROR_MSG("DatetimeExtensions: cannot create CURRENT_DATETIME() expr");
  } else {
    sparql_syntax_error(rq,
                        "CURRENT_DATETIME() can only used with LAQRS");
    YYERROR;
  }
}
#line 9453 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 385:
#line 5475 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);
  
  (yyval.expr) = NULL;
  if(!sparql->sparql11_query) {
    sparql_syntax_error(rq,
                        "NOW() can only be used with SPARQL 1.1");
    YYERROR;
  }
  
  (yyval.expr) = rasqal_new_0op_expression(rq->world,
                                   RASQAL_EXPR_NOW);
  if(!(yyval.expr))
    YYERROR_MSG("DatetimeExtensions: cannot create NOW()");

}
#line 9475 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 386:
#line 5493 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(sparql->experimental) {
    (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                   RASQAL_EXPR_FROM_UNIXTIME, (yyvsp[-1].expr));
    if(!(yyval.expr))
      YYERROR_MSG("DatetimeExtensions: cannot create FROM_UNIXTIME() expr");
  } else {
    sparql_syntax_error(rq,
                        "FROM_UNIXTIME() can only used with LAQRS");
    YYERROR;
  }
  
}
#line 9497 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 387:
#line 5511 "./sparql_parser.y" /* yacc.c:1646  */
    {
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)(rq->context);

  (yyval.expr) = NULL;
  if(sparql->experimental) {
    (yyval.expr) = rasqal_new_1op_expression(rq->world,
                                   RASQAL_EXPR_TO_UNIXTIME, (yyvsp[-1].expr));
    if(!(yyval.expr))
      YYERROR_MSG("DatetimeExtensions: cannot create TO_UNIXTIME() expr");
  } else {
    sparql_syntax_error(rq,
                        "TO_UNIXTIME() can only used with LAQRS");
    YYERROR;
  }
  
}
#line 9519 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 388:
#line 5536 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_uri_literal(rq->world, (yyvsp[0].uri));
  if(!(yyval.literal))
    YYERROR_MSG("IRIrefBrace 1: cannot create literal");
}
#line 9529 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 389:
#line 5542 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_simple_literal(rq->world,
                                 RASQAL_LITERAL_QNAME, (yyvsp[0].name));
  if(!(yyval.literal))
    YYERROR_MSG("IRIrefBrace 2: cannot create literal");
  if(rasqal_literal_expand_qname(rq, (yyval.literal))) {
    sparql_query_error_full(rq,
                            "QName %s cannot be expanded", (yyvsp[0].name));
    rasqal_free_literal((yyval.literal));
    (yyval.literal) = NULL;
    YYERROR_MSG("IRIrefBrace 2: cannot expand qname");
  }
}
#line 9547 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 390:
#line 5562 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9555 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 391:
#line 5566 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9563 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 392:
#line 5570 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9571 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 393:
#line 5577 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9579 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 394:
#line 5581 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9587 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 395:
#line 5585 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9595 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 396:
#line 5593 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9603 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 397:
#line 5597 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9611 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 398:
#line 5601 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9619 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 399:
#line 5609 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9627 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 400:
#line 5613 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9635 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 401:
#line 5617 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = (yyvsp[0].literal);
}
#line 9643 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 402:
#line 5629 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_uri_literal(rq->world, (yyvsp[0].uri));
  if(!(yyval.literal))
    YYERROR_MSG("IRIref 1: cannot create literal");
}
#line 9653 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 403:
#line 5635 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_simple_literal(rq->world,
                                 RASQAL_LITERAL_QNAME, (yyvsp[0].name));
  if(!(yyval.literal))
    YYERROR_MSG("IRIref 2: cannot create literal");
  if(rasqal_literal_expand_qname(rq, (yyval.literal))) {
    sparql_query_error_full(rq,
                            "QName %s cannot be expanded", (yyvsp[0].name));
    rasqal_free_literal((yyval.literal));
    (yyval.literal) = NULL;
    YYERROR_MSG("IRIrefBrace 2: cannot expand qname");
  }
}
#line 9671 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 404:
#line 5655 "./sparql_parser.y" /* yacc.c:1646  */
    {
  (yyval.literal) = rasqal_new_simple_literal(rq->world,
                                 RASQAL_LITERAL_BLANK, (yyvsp[0].name));
  if(!(yyval.literal))
    YYERROR_MSG("BlankNode 1: cannot create literal");
}
#line 9682 "sparql_parser.c" /* yacc.c:1646  */
    break;

  case 405:
#line 5661 "./sparql_parser.y" /* yacc.c:1646  */
    {
  const unsigned char *id;
  id = rasqal_query_generate_bnodeid(rq, NULL);
  if(!id)
    YYERROR_MSG("BlankNode 2: cannot create bnodeid");
  (yyval.literal) = rasqal_new_simple_literal(rq->world,
                                 RASQAL_LITERAL_BLANK, id);
  if(!(yyval.literal))
    YYERROR_MSG("BlankNode 2: cannot create literal");
}
#line 9697 "sparql_parser.c" /* yacc.c:1646  */
    break;


#line 9701 "sparql_parser.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (rq, yyscanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
if(yytoken < 0) yytoken = YYUNDEFTOK;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
if(yytoken < 0) yytoken = YYUNDEFTOK;
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (rq, yyscanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, rq, yyscanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, rq, yyscanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (rq, yyscanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, rq, yyscanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, rq, yyscanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 5680 "./sparql_parser.y" /* yacc.c:1906  */



/* Support functions */


/* This is declared in sparql_lexer.h but never used, so we always get
 * a warning unless this dummy code is here.  Used once below in an error case.
 */
static int yy_init_globals (yyscan_t yyscanner ) { return 0; }


/*
 * rasqal_sparql_query_language_init:
 * @rdf_query: query
 * @name: language name (or NULL)
 *
 * Internal: Initialise the SPARQL query language parser
 *
 * Return value: non 0 on failure
 **/
static int
rasqal_sparql_query_language_init(rasqal_query* rdf_query, const char *name)
{
  rasqal_sparql_query_language* rqe;

  rqe = (rasqal_sparql_query_language*)rdf_query->context;

  rdf_query->compare_flags = RASQAL_COMPARE_XQUERY;

  /* All the sparql query families support this */
  rqe->sparql_scda = 1; /* SELECT CONSTRUCT DESCRIBE ASK */

  /* SPARQL 1.1 Query + Update is the default */
  rqe->sparql_scda = 1; /* SELECT CONSTRUCT DESCRIBE ASK */
  rqe->sparql11_query = 1;
  rqe->sparql11_property_paths = 1;
  rqe->sparql11_update = 1;

  if(name) {
    /* SPARQL 1.0 disables SPARQL 1.1 features */
    if(!strncmp(name, "sparql10", 8)) {
      rqe->sparql11_query = 0;
      rqe->sparql11_property_paths = 0;
      rqe->sparql11_update = 0;
    }

    if(!strcmp(name, "sparql11-query")) {
      /* No update if SPARQL 1.1 query */
      rqe->sparql11_update = 0;
    }

    if(!strcmp(name, "sparql11-update")) {
      /* No query (SELECT, CONSTRUCT, DESCRIBE, ASK) if SPARQL 1.1 update */
      rqe->sparql_scda = 0;
    }

    /* LAQRS for experiments */
    if(!strcmp(name, "laqrs"))
      rqe->experimental = 1;
  }

  return 0;
}


/**
 * rasqal_sparql_query_language_terminate - Free the SPARQL query language parser
 *
 * Return value: non 0 on failure
 **/
static void
rasqal_sparql_query_language_terminate(rasqal_query* rdf_query)
{
  rasqal_sparql_query_language* sparql;
  sparql = (rasqal_sparql_query_language*)rdf_query->context;

  if(sparql && sparql->scanner_set) {
    sparql_lexer_lex_destroy(sparql->scanner);
    sparql->scanner_set = 0;
  }

}


static int
rasqal_sparql_query_language_prepare(rasqal_query* rdf_query)
{
  /* rasqal_sparql_query_language* sparql = (rasqal_sparql_query_language*)rdf_query->context; */
  int rc;
  
  if(!rdf_query->query_string)
    return 1;

  rc = rasqal_query_reset_select_query(rdf_query);
  if(rc)
    return 1;

  rc = sparql_parse(rdf_query);
  if(rc)
    return rc;

  /* FIXME - should check remaining query parts  */
  if(rasqal_sequence_has_qname(rdf_query->triples) ||
     rasqal_sequence_has_qname(rdf_query->constructs) ||
     rasqal_query_constraints_has_qname(rdf_query)) {
    sparql_query_error(rdf_query, "SPARQL query has unexpanded QNames");
    return 1;
  }

  /* SPARQL: Turn [] into anonymous variables */
  if(rasqal_query_build_anonymous_variables(rdf_query))
    return 1;
  
  /* SPARQL: Expand 'SELECT *' */
  if(rasqal_query_expand_wildcards(rdf_query,
                                   rasqal_query_get_projection(rdf_query)))
    return 1;

  return 0;
}


static int
sparql_parse(rasqal_query* rq)
{
  rasqal_sparql_query_language* rqe;
  raptor_locator *locator=&rq->locator;

  rqe = (rasqal_sparql_query_language*)rq->context;

  if(!rq->query_string)
    return yy_init_globals(NULL); /* 0 but a way to use yy_init_globals */

  locator->line = 1;
  locator->column = -1; /* No column info */
  locator->byte = -1; /* No bytes info */

  rqe->lineno = 1;

  if(sparql_lexer_lex_init(&rqe->scanner))
    return 1;
  rqe->scanner_set = 1;

 #if defined(YYDEBUG) && YYDEBUG > 0
   sparql_lexer_set_debug(1 ,&rqe->scanner);
   sparql_parser_debug = 1;
 #endif

  sparql_lexer_set_extra(rq, rqe->scanner);

  (void)sparql_lexer__scan_buffer(RASQAL_GOOD_CAST(char*, rq->query_string),
                                  rq->query_string_length, rqe->scanner);

  rqe->error_count = 0;

  sparql_parser_parse(rq, rqe->scanner);

  sparql_lexer_lex_destroy(rqe->scanner);
  rqe->scanner_set = 0;

  /* Parsing failed */
  if(rq->failed)
    return 1;
  
  return 0;
}


static void
sparql_query_error(rasqal_query *rq, const char *msg)
{
  rasqal_sparql_query_language* rqe;

  rqe = (rasqal_sparql_query_language*)rq->context;

  if(rqe->error_count++)
    return;

  rq->locator.line = rqe->lineno;
#ifdef RASQAL_SPARQL_USE_ERROR_COLUMNS
  /*  rq->locator.column = sparql_lexer_get_column(yyscanner);*/
#endif

  rq->failed = 1;
  rasqal_log_error_simple(((rasqal_query*)rq)->world, RAPTOR_LOG_LEVEL_ERROR,
                          &rq->locator, "%s", msg);
}


static void
sparql_query_error_full(rasqal_query *rq, const char *message, ...)
{
  va_list arguments;
  rasqal_sparql_query_language* rqe;

  rqe = (rasqal_sparql_query_language*)rq->context;

  if(rqe->error_count++)
    return;

  rq->locator.line = rqe->lineno;
#ifdef RASQAL_SPARQL_USE_ERROR_COLUMNS
  /*  rq->locator.column = sparql_lexer_get_column(yyscanner);*/
#endif

  va_start(arguments, message);

  rq->failed = 1;
  rasqal_log_error_varargs(((rasqal_query*)rq)->world, RAPTOR_LOG_LEVEL_ERROR,
                           &rq->locator, message, arguments);

  va_end(arguments);
}


int
sparql_syntax_error(rasqal_query *rq, const char *message, ...)
{
  rasqal_sparql_query_language *rqe;
  va_list arguments;

  rqe = (rasqal_sparql_query_language*)rq->context;

  if(rqe->error_count++)
    return 0;

  rq->locator.line=rqe->lineno;
#ifdef RASQAL_SPARQL_USE_ERROR_COLUMNS
  /*  rp->locator.column=sparql_lexer_get_column(yyscanner);*/
#endif

  va_start(arguments, message);
  rq->failed = 1;
  rasqal_log_error_varargs(((rasqal_query*)rq)->world, RAPTOR_LOG_LEVEL_ERROR,
                           &rq->locator, message, arguments);
  va_end(arguments);

  return 0;
}


int
sparql_syntax_warning(rasqal_query *rq, const char *message, ...)
{
  rasqal_sparql_query_language *rqe;
  va_list arguments;

  if(RASQAL_WARNING_LEVEL_QUERY_SYNTAX < rq->world->warning_level)
    return 0;
  
  rqe = (rasqal_sparql_query_language*)rq->context;

  rq->locator.line = rqe->lineno;
#ifdef RASQAL_SPARQL_USE_ERROR_COLUMNS
  /*  rq->locator.column=sparql_lexer_get_column(yyscanner);*/
#endif

  va_start(arguments, message);
  rasqal_log_error_varargs(((rasqal_query*)rq)->world, RAPTOR_LOG_LEVEL_WARN,
                           &rq->locator, message, arguments);
  va_end(arguments);

  return 0;
}


static int
rasqal_sparql_query_language_iostream_write_escaped_counted_string(rasqal_query* query,
                                                                   raptor_iostream* iostr,
                                                                   const unsigned char* string,
                                                                   size_t len)
{
  const char delim = '"';
  
  raptor_iostream_write_byte(delim, iostr);
  if(raptor_string_ntriples_write(string, len, delim, iostr))
    return 1;
  
  raptor_iostream_write_byte(delim, iostr);

  return 0;
}


static const char* const sparql_names[] = { "sparql10", NULL};

static const raptor_type_q sparql_types[] = {
  { NULL, 0, 0}
};


static int
rasqal_sparql_query_language_register_factory(rasqal_query_language_factory *factory)
{
  int rc = 0;

  factory->desc.names = sparql_names;

  factory->desc.mime_types = sparql_types;

  factory->desc.label = "SPARQL 1.0 W3C RDF Query Language";

  factory->desc.uri_strings = NULL;

  factory->context_length = sizeof(rasqal_sparql_query_language);

  factory->init      = rasqal_sparql_query_language_init;
  factory->terminate = rasqal_sparql_query_language_terminate;
  factory->prepare   = rasqal_sparql_query_language_prepare;
  factory->iostream_write_escaped_counted_string = rasqal_sparql_query_language_iostream_write_escaped_counted_string;

  return rc;
}


int
rasqal_init_query_language_sparql(rasqal_world* world)
{
  return !rasqal_query_language_register_factory(world,
                                                 &rasqal_sparql_query_language_register_factory);
}


static const char* const sparql11_names[] = { "sparql", "sparql11", NULL };


static const char* const sparql11_uri_strings[] = {
  "http://www.w3.org/TR/rdf-sparql-query/",
  NULL
};

static const raptor_type_q sparql11_types[] = {
  { "application/sparql", 18, 10}, 
  { NULL, 0, 0}
};


static int
rasqal_sparql11_language_register_factory(rasqal_query_language_factory *factory)
{
  int rc = 0;

  factory->desc.names = sparql11_names;

  factory->desc.mime_types = sparql11_types;

  factory->desc.label = "SPARQL 1.1 (DRAFT) Query and Update Languages";

  /* What URI describes Query and Update languages? */
  factory->desc.uri_strings = sparql11_uri_strings;

  factory->context_length = sizeof(rasqal_sparql_query_language);

  factory->init      = rasqal_sparql_query_language_init;
  factory->terminate = rasqal_sparql_query_language_terminate;
  factory->prepare   = rasqal_sparql_query_language_prepare;
  factory->iostream_write_escaped_counted_string = rasqal_sparql_query_language_iostream_write_escaped_counted_string;

  return rc;
}


static const char* const sparql11_query_names[] = { "sparql11-query", NULL };

static const char* const sparql11_query_uri_strings[] = {
  "http://www.w3.org/TR/2010/WD-sparql11-query-20101014/",
  NULL
};

static const raptor_type_q sparql11_query_types[] = {
  { NULL, 0, 0}
};


static int
rasqal_sparql11_query_language_register_factory(rasqal_query_language_factory *factory)
{
  int rc = 0;

  factory->desc.names = sparql11_query_names;

  factory->desc.mime_types = sparql11_query_types;

  factory->desc.label = "SPARQL 1.1 (DRAFT) Query Language";

  factory->desc.uri_strings = sparql11_query_uri_strings;

  factory->context_length = sizeof(rasqal_sparql_query_language);

  factory->init      = rasqal_sparql_query_language_init;
  factory->terminate = rasqal_sparql_query_language_terminate;
  factory->prepare   = rasqal_sparql_query_language_prepare;
  factory->iostream_write_escaped_counted_string = rasqal_sparql_query_language_iostream_write_escaped_counted_string;

  return rc;
}


static const char* const sparql11_update_names[] = { "sparql11-update", NULL };

static const char* const sparql11_update_uri_strings[] = {
  "http://www.w3.org/TR/2010/WD-sparql11-update-20101014/",
  NULL
};

static const raptor_type_q sparql11_update_types[] = {
  { NULL, 0, 0}
};


static int
rasqal_sparql11_update_language_register_factory(rasqal_query_language_factory *factory)
{
  int rc = 0;

  factory->desc.names = sparql11_update_names;

  factory->desc.mime_types = sparql11_update_types;

  factory->desc.label = "SPARQL 1.1 (DRAFT) Update Language";

  factory->desc.uri_strings = sparql11_update_uri_strings;

  factory->context_length = sizeof(rasqal_sparql_query_language);

  factory->init      = rasqal_sparql_query_language_init;
  factory->terminate = rasqal_sparql_query_language_terminate;
  factory->prepare   = rasqal_sparql_query_language_prepare;
  factory->iostream_write_escaped_counted_string = rasqal_sparql_query_language_iostream_write_escaped_counted_string;

  return rc;
}


int
rasqal_init_query_language_sparql11(rasqal_world* world)
{
  if(!rasqal_query_language_register_factory(world,
                                             &rasqal_sparql11_language_register_factory))
    return 1;
  
  if(!rasqal_query_language_register_factory(world,
                                             &rasqal_sparql11_query_language_register_factory))
    return 1;
  
  if(!rasqal_query_language_register_factory(world,
                                             &rasqal_sparql11_update_language_register_factory))
    return 1;
  
  return 0;
}


static const char* const laqrs_names[] = { "laqrs", NULL};

static const raptor_type_q laqrs_types[] = {
  { NULL, 0, 0}
};


static int
rasqal_laqrs_query_language_register_factory(rasqal_query_language_factory *factory)
{
  int rc = 0;

  factory->desc.names = laqrs_names;

  factory->desc.mime_types = laqrs_types;

  factory->desc.label = "LAQRS adds to Querying RDF in SPARQL";

  factory->desc.uri_strings = NULL;

  factory->context_length = sizeof(rasqal_sparql_query_language);

  factory->init      = rasqal_sparql_query_language_init;
  factory->terminate = rasqal_sparql_query_language_terminate;
  factory->prepare   = rasqal_sparql_query_language_prepare;
  factory->iostream_write_escaped_counted_string = rasqal_sparql_query_language_iostream_write_escaped_counted_string;

  return rc;
}


int
rasqal_init_query_language_laqrs(rasqal_world* world)
{
  return !rasqal_query_language_register_factory(world,
                                                 &rasqal_laqrs_query_language_register_factory);
}


#ifdef STANDALONE
#include <stdio.h>
#include <locale.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifndef HAVE_GETOPT
#include <rasqal_getopt.h>
#endif

#ifdef NEED_OPTIND_DECLARATION
extern int optind;
extern char *optarg;
#endif

#define GETOPT_STRING "di:"

#define SPARQL_FILE_BUF_SIZE 4096

static char query_string[SPARQL_FILE_BUF_SIZE];

int
main(int argc, char *argv[]) 
{
  const char *program = rasqal_basename(argv[0]);
  rasqal_query *query = NULL;
  FILE *fh;
  int rc;
  const char *filename = NULL;
  raptor_uri* base_uri = NULL;
  unsigned char *uri_string = NULL;
  const char* query_language = "sparql";
  int usage = 0;
  rasqal_world *world;
  size_t read_len;

  world = rasqal_new_world();
  if(!world || rasqal_world_open(world))
    exit(1);

  filename = getenv("SPARQL_QUERY_FILE");
    
  while(!usage) {
    int c = getopt (argc, argv, GETOPT_STRING);

    if (c == -1)
      break;

    switch (c) {
      case 0:
      case '?': /* getopt() - unknown option */
        usage = 1;
        break;
        
      case 'd':
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 2
        sparql_parser_debug = 1;
#endif
        break;
  
      case 'i':
        if(optarg) {
          if(rasqal_language_name_check(world, optarg)) {
            query_language = optarg;
          } else {
            fprintf(stderr, "%s: Unknown query language '%s'\n",
                    program, optarg);
            usage = 1;
          }
        }
        break;
    }
  }

  if(!filename) {
    if((argc-optind) != 1) {
      fprintf(stderr, "%s: Too many arguments.\n", program);
      usage = 1;
    } else
      filename = argv[optind];
  }
  
  if(usage) {
    fprintf(stderr, "SPARQL/LAQRS parser test for Rasqal %s\n", 
            rasqal_version_string);
    fprintf(stderr, "USAGE: %s [OPTIONS] SPARQL-QUERY-FILE\n", program);
    fprintf(stderr, "OPTIONS:\n");
#if defined(RASQAL_DEBUG) && RASQAL_DEBUG > 2
    fprintf(stderr, " -d           Bison parser debugging\n");
#endif
    fprintf(stderr, " -i LANGUAGE  Set query language\n");
    rc = 1;
    goto tidy;
  }


 fh = fopen(filename, "r");
 if(!fh) {
   fprintf(stderr, "%s: Cannot open file %s - %s\n", program, filename,
           strerror(errno));
   rc = 1;
   goto tidy;
 }
 
  memset(query_string, 0, SPARQL_FILE_BUF_SIZE);
  read_len = fread(query_string, SPARQL_FILE_BUF_SIZE, 1, fh);
  if(read_len < SPARQL_FILE_BUF_SIZE) {
    if(ferror(fh)) {
      fprintf(stderr, "%s: file '%s' read failed - %s\n",
              program, filename, strerror(errno));
      fclose(fh);
      rc = 1;
      goto tidy;
    }
  }
  
  fclose(fh);

  query = rasqal_new_query(world, query_language, NULL);
  rc = 1;
  if(query) {
    uri_string = raptor_uri_filename_to_uri_string(filename);

    if(uri_string) {
      base_uri = raptor_new_uri(world->raptor_world_ptr, uri_string);

      if(base_uri) {
        rc = rasqal_query_prepare(query,
                                  RASQAL_GOOD_CAST(const unsigned char*, query_string),
                                  base_uri);

        if(!rc)
          rasqal_query_print(query, stdout);
      }
    }
  }

  tidy:
  if(query)
    rasqal_free_query(query);

  if(base_uri)
    raptor_free_uri(base_uri);

  if(uri_string)
    raptor_free_memory(uri_string);

  if(world)
    rasqal_free_world(world);

  return rc;
}

#endif /* STANDALONE */
