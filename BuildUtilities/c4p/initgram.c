/* A Bison parser, made by GNU Bison 1.875b.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     AND = 258,
     ARRAY = 259,
     BEGIn = 260,
     CASE = 261,
     CONST = 262,
     DIV = 263,
     DO = 264,
     DOWNTO = 265,
     ELSE = 266,
     END = 267,
     FILe = 268,
     FOR = 269,
     FORWARD = 270,
     FUNCTION = 271,
     GOTO = 272,
     IF = 273,
     IN = 274,
     LABEL = 275,
     MOD = 276,
     NIL = 277,
     NOT = 278,
     OF = 279,
     OR = 280,
     PACKED = 281,
     PROCEDURE = 282,
     PROGRAM = 283,
     RECORD = 284,
     REPEAT = 285,
     SET = 286,
     THEN = 287,
     TO = 288,
     TYPE = 289,
     UNTIL = 290,
     VAR = 291,
     WHILE = 292,
     WITH = 293,
     READ = 294,
     READLN = 295,
     OTHERS = 296,
     DOTDOT = 297,
     NEQ = 298,
     GEQ = 299,
     LEQ = 300,
     ASSIGN = 301,
     PASCAL_KEYWORD = 302,
     TYPE_IDENTIFIER = 303,
     FIELD_IDENTIFIER = 304,
     C_RESERVED = 305,
     CONSTANT_IDENTIFIER = 306,
     FUNCTION_IDENTIFIER = 307,
     PROCEDURE_IDENTIFIER = 308,
     UNDEFINED_IDENTIFIER = 309,
     VARIABLE_IDENTIFIER = 310,
     PARAMETER_IDENTIFIER = 311,
     WRITE = 312,
     WRITELN = 313,
     BUILD_IN_IDENTIFIER = 314,
     CHARACTER_STRING = 315,
     CHARACTER_CONSTANT = 316,
     UNSIGNED_NUMBER = 317,
     REAL_CONSTANT = 318,
     LONG_REAL_CONSTANT = 319,
     IDENTIFIER = 320
   };
#endif
#define AND 258
#define ARRAY 259
#define BEGIn 260
#define CASE 261
#define CONST 262
#define DIV 263
#define DO 264
#define DOWNTO 265
#define ELSE 266
#define END 267
#define FILe 268
#define FOR 269
#define FORWARD 270
#define FUNCTION 271
#define GOTO 272
#define IF 273
#define IN 274
#define LABEL 275
#define MOD 276
#define NIL 277
#define NOT 278
#define OF 279
#define OR 280
#define PACKED 281
#define PROCEDURE 282
#define PROGRAM 283
#define RECORD 284
#define REPEAT 285
#define SET 286
#define THEN 287
#define TO 288
#define TYPE 289
#define UNTIL 290
#define VAR 291
#define WHILE 292
#define WITH 293
#define READ 294
#define READLN 295
#define OTHERS 296
#define DOTDOT 297
#define NEQ 298
#define GEQ 299
#define LEQ 300
#define ASSIGN 301
#define PASCAL_KEYWORD 302
#define TYPE_IDENTIFIER 303
#define FIELD_IDENTIFIER 304
#define C_RESERVED 305
#define CONSTANT_IDENTIFIER 306
#define FUNCTION_IDENTIFIER 307
#define PROCEDURE_IDENTIFIER 308
#define UNDEFINED_IDENTIFIER 309
#define VARIABLE_IDENTIFIER 310
#define PARAMETER_IDENTIFIER 311
#define WRITE 312
#define WRITELN 313
#define BUILD_IN_IDENTIFIER 314
#define CHARACTER_STRING 315
#define CHARACTER_CONSTANT 316
#define UNSIGNED_NUMBER 317
#define REAL_CONSTANT 318
#define LONG_REAL_CONSTANT 319
#define IDENTIFIER 320




/* Copy the first part of user declarations.  */
#line 21 "gram.y"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <malloc.h>

#include "common.h"

unsigned block_level;
symbol_t * prog_symbol;

namespace {
  pascal_type last_type;
  void * last_type_ptr;
  value_t last_value;
  array_node * last_array_node;
  std::stack<array_node *> array_type_stack;
  std::stack<parameter_node *> parameter_node_stack;
  declarator_node * last_declarator;
  parameter_node * last_parameter;
  bool by_reference;
  record_section_node * last_record_section;
  variant_node * last_variant;
  std::string file_spec;
  bool file_specified;
  pascal_type file_type;
  size_t buf_mark;
  size_t buf_mark2;
  bool to_eoln;
  unsigned nglobalvars;
  unsigned routine_handle;
}

#define push_array_type(arr)			\
  array_type_stack.push (last_array_node);	\
  last_array_node = arr

#define pop_array_type()			\
  last_array_node = array_type_stack.top();	\
  array_type_stack.pop ()

#define push_parameter_node(par)		\
  parameter_node_stack.push (par)

#define pop_parameter_node(par)			\
  par = parameter_node_stack.top();		\
  parameter_node_stack.pop ()

#define get_array_offset(arr)  arr->lower_bound



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 74 "gram.y"
typedef union YYSTYPE {
  symbol_t * sym;
  long value;
  pascal_type type;
  double fvalue;
  void * type_ptr;
  unsigned buf_mark;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 268 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 280 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   323

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  82
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  186
/* YYNRULES -- Number of rules. */
#define YYNRULES  297
/* YYNRULES -- Number of states. */
#define YYNSTATES  444

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   320

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      68,    69,    81,    73,    70,    74,    67,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    71,    66,
      78,    72,    79,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    75,     2,    76,    77,     2,     2,     2,     2,     2,
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
      65
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     8,     9,    14,    15,    19,    20,    21,
      22,    23,    24,    36,    38,    42,    44,    45,    47,    49,
      52,    54,    56,    61,    62,    63,    76,    79,    85,    86,
      87,    92,    97,    98,    99,   112,   116,   118,   119,   124,
     125,   128,   129,   133,   137,   139,   140,   145,   147,   148,
     152,   154,   158,   159,   162,   164,   167,   168,   174,   176,
     179,   182,   184,   187,   190,   192,   195,   198,   200,   203,
     206,   208,   210,   212,   215,   218,   220,   223,   226,   228,
     229,   232,   234,   237,   238,   244,   246,   248,   250,   252,
     254,   256,   260,   262,   265,   267,   269,   271,   275,   276,
     284,   287,   289,   291,   292,   297,   301,   303,   307,   310,
     312,   314,   315,   320,   321,   325,   327,   328,   333,   335,
     336,   341,   343,   345,   347,   348,   353,   354,   355,   356,
     364,   367,   368,   371,   373,   376,   381,   383,   384,   389,
     391,   393,   394,   399,   401,   405,   406,   410,   413,   414,
     416,   419,   420,   422,   424,   426,   427,   431,   432,   436,
     437,   441,   442,   446,   447,   451,   452,   454,   455,   460,
     462,   463,   468,   470,   471,   473,   474,   479,   481,   482,
     487,   489,   490,   495,   497,   498,   503,   505,   507,   509,
     511,   513,   514,   515,   523,   524,   525,   529,   530,   531,
     540,   541,   543,   545,   547,   551,   552,   557,   559,   563,
     564,   567,   569,   571,   573,   575,   576,   577,   584,   585,
     586,   593,   594,   595,   596,   608,   610,   612,   614,   616,
     617,   618,   624,   627,   629,   631,   633,   635,   638,   640,
     642,   644,   646,   647,   653,   655,   659,   661,   663,   664,
     669,   671,   672,   677,   679,   681,   683,   685,   687,   689,
     691,   693,   694,   698,   701,   702,   707,   708,   713,   715,
     717,   719,   720,   725,   726,   731,   732,   737,   739,   741,
     743,   745,   747,   749,   751,   753,   755,   756,   762,   763,
     768,   769,   773,   774,   778,   780,   781,   786
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
      83,     0,    -1,    84,    66,    87,    67,    -1,    -1,    28,
      65,    85,    86,    -1,    -1,    68,    93,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    88,   117,    89,   119,    90,   125,
     161,    91,    95,    92,   167,    -1,    94,    -1,    93,    70,
      94,    -1,    65,    -1,    -1,    96,    -1,    97,    -1,    96,
      97,    -1,   104,    -1,    98,    -1,   101,    66,    15,    66,
      -1,    -1,    -1,   101,    66,    99,   117,   119,   125,   161,
       5,   100,   170,    12,    66,    -1,    16,    65,    -1,    16,
      65,   102,    71,    65,    -1,    -1,    -1,    68,   103,   108,
      69,    -1,   107,    66,    15,    66,    -1,    -1,    -1,   107,
      66,   105,   117,   119,   125,   161,     5,   106,   170,    12,
      66,    -1,    27,    65,   102,    -1,   110,    -1,    -1,   108,
      66,   109,   110,    -1,    -1,   111,   113,    -1,    -1,    36,
     112,   113,    -1,   114,    71,    65,    -1,   116,    -1,    -1,
     114,    70,   115,   116,    -1,    65,    -1,    -1,    20,   118,
      66,    -1,    62,    -1,   118,    70,    62,    -1,    -1,     7,
     120,    -1,   121,    -1,   120,   121,    -1,    -1,    65,    72,
     122,   123,    66,    -1,    63,    -1,    73,    63,    -1,    74,
      63,    -1,    64,    -1,    73,    64,    -1,    74,    63,    -1,
      62,    -1,    73,    62,    -1,    74,    62,    -1,    65,    -1,
      73,    65,    -1,    74,    65,    -1,    60,    -1,    61,    -1,
      62,    -1,    73,    62,    -1,    74,    62,    -1,    65,    -1,
      73,    65,    -1,    74,    65,    -1,    61,    -1,    -1,    34,
     126,    -1,   127,    -1,   126,   127,    -1,    -1,    65,    72,
     128,   129,    66,    -1,    65,    -1,   130,    -1,   131,    -1,
     133,    -1,   138,    -1,   132,    -1,   124,    42,   124,    -1,
     134,    -1,    26,   134,    -1,   136,    -1,   143,    -1,   135,
      -1,    13,    24,   139,    -1,    -1,     4,    75,   140,    76,
      24,   137,   139,    -1,    77,   139,    -1,   129,    -1,   142,
      -1,    -1,   140,    70,   141,   142,    -1,   124,    42,   124,
      -1,    65,    -1,    29,   144,    12,    -1,   145,   152,    -1,
     146,    -1,   148,    -1,    -1,   146,    66,   147,   148,    -1,
      -1,   149,    71,   129,    -1,   151,    -1,    -1,   149,    70,
     150,   151,    -1,    65,    -1,    -1,     6,   153,    24,   155,
      -1,   154,    -1,    65,    -1,   157,    -1,    -1,   155,    66,
     156,   157,    -1,    -1,    -1,    -1,   158,   212,    71,    68,
     159,   160,    69,    -1,   145,   152,    -1,    -1,    36,   162,
      -1,   163,    -1,   162,   163,    -1,   164,    71,   129,    66,
      -1,   166,    -1,    -1,   164,    70,   165,   166,    -1,    65,
      -1,   168,    -1,    -1,     5,   169,   170,    12,    -1,   171,
      -1,   170,    66,   171,    -1,    -1,   173,   172,   175,    -1,
     173,   197,    -1,    -1,   174,    -1,    62,    71,    -1,    -1,
     229,    -1,   176,    -1,   232,    -1,    -1,    39,   177,   183,
      -1,    -1,    40,   178,   182,    -1,    -1,    57,   179,   189,
      -1,    -1,    58,   180,   188,    -1,    -1,    65,   181,   264,
      -1,    -1,   183,    -1,    -1,    68,   184,   185,    69,    -1,
     187,    -1,    -1,   185,    70,   186,   187,    -1,   234,    -1,
      -1,   189,    -1,    -1,    68,   190,   191,    69,    -1,   193,
      -1,    -1,   191,    70,   192,   193,    -1,   246,    -1,    -1,
     246,    71,   194,   195,    -1,   246,    -1,    -1,   246,    71,
     196,   246,    -1,   168,    -1,   198,    -1,   215,    -1,   199,
      -1,   204,    -1,    -1,    -1,    18,   200,   248,    32,   201,
     171,   202,    -1,    -1,    -1,    11,   203,   171,    -1,    -1,
      -1,     6,   205,   208,    24,   206,   209,   207,    12,    -1,
      -1,    66,    -1,   246,    -1,   210,    -1,   209,    66,   210,
      -1,    -1,   212,    71,   211,   171,    -1,   213,    -1,   212,
      70,   213,    -1,    -1,   214,   123,    -1,    41,    -1,   219,
      -1,   216,    -1,   222,    -1,    -1,    -1,    37,   217,   246,
       9,   218,   171,    -1,    -1,    -1,    30,   220,   170,    35,
     221,   246,    -1,    -1,    -1,    -1,    14,    65,    46,   223,
     226,   228,   224,   227,     9,   225,   171,    -1,   246,    -1,
     246,    -1,    33,    -1,    10,    -1,    -1,    -1,   234,   230,
     233,   231,   246,    -1,    17,    62,    -1,    46,    -1,   236,
      -1,   238,    -1,   235,    -1,   234,    77,    -1,   237,    -1,
      65,    -1,   239,    -1,   242,    -1,    -1,   241,    75,   240,
     244,    76,    -1,   234,    -1,   243,    67,    65,    -1,   234,
      -1,   246,    -1,    -1,   244,    70,   245,   246,    -1,   250,
      -1,    -1,   246,   249,   247,   250,    -1,   246,    -1,    78,
      -1,    79,    -1,    45,    -1,    44,    -1,    72,    -1,    43,
      -1,   255,    -1,    -1,    74,   251,   255,    -1,    73,   255,
      -1,    -1,   250,   254,   252,   255,    -1,    -1,   250,    25,
     253,   255,    -1,    74,    -1,    73,    -1,   260,    -1,    -1,
     255,    80,   256,   260,    -1,    -1,   255,   259,   257,   260,
      -1,    -1,   255,     3,   258,   260,    -1,    81,    -1,     8,
      -1,    21,    -1,    62,    -1,    63,    -1,    64,    -1,    60,
      -1,    61,    -1,   234,    -1,    -1,    65,    68,   261,   265,
      69,    -1,    -1,    68,   262,   246,    69,    -1,    -1,    23,
     263,   260,    -1,    -1,    68,   265,    69,    -1,   267,    -1,
      -1,   265,    70,   266,   267,    -1,   246,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   123,   123,   127,   126,   137,   139,   143,   147,   152,
     158,   171,   143,   227,   228,   232,   237,   239,   243,   244,
     248,   249,   253,   257,   269,   256,   293,   299,   322,   326,
     325,   337,   341,   350,   340,   361,   380,   382,   381,   394,
     394,   402,   401,   416,   435,   437,   436,   449,   456,   458,
     463,   466,   469,   471,   476,   477,   482,   481,   500,   506,
     512,   518,   524,   530,   537,   543,   549,   555,   564,   573,
     583,   591,   602,   603,   607,   611,   618,   625,   632,   635,
     637,   641,   642,   647,   646,   658,   665,   669,   670,   671,
     675,   682,   690,   691,   698,   699,   700,   707,   716,   715,
     731,   739,   743,   745,   744,   759,   765,   814,   822,   829,
     833,   835,   834,   848,   851,   859,   861,   860,   873,   882,
     885,   892,   899,   905,   907,   906,   920,   924,   928,   924,
     939,   945,   947,   951,   952,   956,   980,   982,   981,   994,
    1002,  1007,  1006,  1032,  1033,  1038,  1037,  1042,  1045,  1047,
    1051,  1061,  1064,  1065,  1066,  1071,  1070,  1076,  1075,  1081,
    1080,  1086,  1085,  1091,  1090,  1114,  1117,  1122,  1121,  1145,
    1147,  1146,  1156,  1188,  1191,  1196,  1195,  1219,  1221,  1220,
    1230,  1260,  1259,  1283,  1287,  1286,  1297,  1298,  1299,  1306,
    1307,  1313,  1317,  1312,  1326,  1330,  1329,  1343,  1347,  1342,
    1358,  1360,  1364,  1370,  1371,  1376,  1375,  1387,  1388,  1392,
    1392,  1400,  1407,  1408,  1409,  1414,  1418,  1413,  1430,  1435,
    1429,  1448,  1461,  1465,  1447,  1486,  1492,  1498,  1502,  1510,
    1517,  1509,  1528,  1535,  1539,  1540,  1544,  1548,  1559,  1563,
    1602,  1603,  1608,  1607,  1633,  1637,  1647,  1651,  1659,  1658,
    1673,  1675,  1674,  1690,  1694,  1698,  1702,  1706,  1710,  1714,
    1721,  1723,  1722,  1730,  1735,  1734,  1748,  1747,  1755,  1759,
    1766,  1768,  1767,  1773,  1772,  1786,  1785,  1793,  1797,  1801,
    1808,  1813,  1818,  1823,  1830,  1837,  1854,  1853,  1891,  1890,
    1900,  1899,  1909,  1911,  1915,  1917,  1916,  1924
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "AND", "ARRAY", "BEGIn", "CASE", "CONST", 
  "DIV", "DO", "DOWNTO", "ELSE", "END", "FILe", "FOR", "FORWARD", 
  "FUNCTION", "GOTO", "IF", "IN", "LABEL", "MOD", "NIL", "NOT", "OF", 
  "OR", "PACKED", "PROCEDURE", "PROGRAM", "RECORD", "REPEAT", "SET", 
  "THEN", "TO", "TYPE", "UNTIL", "VAR", "WHILE", "WITH", "READ", "READLN", 
  "OTHERS", "DOTDOT", "NEQ", "GEQ", "LEQ", "ASSIGN", "PASCAL_KEYWORD", 
  "TYPE_IDENTIFIER", "FIELD_IDENTIFIER", "C_RESERVED", 
  "CONSTANT_IDENTIFIER", "FUNCTION_IDENTIFIER", "PROCEDURE_IDENTIFIER", 
  "UNDEFINED_IDENTIFIER", "VARIABLE_IDENTIFIER", "PARAMETER_IDENTIFIER", 
  "WRITE", "WRITELN", "BUILD_IN_IDENTIFIER", "CHARACTER_STRING", 
  "CHARACTER_CONSTANT", "UNSIGNED_NUMBER", "REAL_CONSTANT", 
  "LONG_REAL_CONSTANT", "IDENTIFIER", "';'", "'.'", "'('", "')'", "','", 
  "':'", "'='", "'+'", "'-'", "'['", "']'", "'^'", "'<'", "'>'", "'/'", 
  "'*'", "$accept", "program", "program_heading", "@1", 
  "optional_program_parameters", "program_block", "@2", "@3", "@4", "@5", 
  "@6", "program_parameter_list", "program_parameter", 
  "procedure_and_function_declaration_part", 
  "procedure_and_function_declarations", 
  "procedure_or_function_declaration", "function_declaration", "@7", "@8", 
  "function_head", "optional_formal_parameter_section_list", "@9", 
  "procedure_declaration", "@10", "@11", "procedure_head", 
  "formal_parameter_section_list", "@12", "formal_parameter_section", 
  "@13", "@14", "parameter_group", "parameter_declarator_list", "@15", 
  "parameter_declarator", "label_declaration_part", "label_list", 
  "constant_definition_part", "constant_definitions", 
  "constant_definition", "@16", "constant", "bound", 
  "type_definition_part", "type_definitions", "type_definition", "@17", 
  "type_denoter", "type", "ordinal_type", "subrange_type", 
  "structured_type", "unpacked_structured_type", "file_type", 
  "array_type", "@18", "pointer_type", "component_type", 
  "index_type_list", "@19", "index_type", "record_type", "field_list", 
  "fixed_part", "record_section_list", "@20", "record_section", 
  "field_declarator_list", "@21", "field_declarator", "variant_part", 
  "variant_selector", "tag_type", "variant_list", "@22", "variant", "@23", 
  "@24", "variant_field_list", "variable_declaration_part", 
  "variable_declarations", "variable_declaration", 
  "variable_declarator_list", "@25", "variable_declarator", 
  "statement_part", "compound_statement", "@26", "statement_sequence", 
  "statement", "@27", "optional_label", "label", "simple_statement", 
  "procedure_statement", "@28", "@29", "@30", "@31", "@32", 
  "readln_parameter_list", "read_parameter_list", "@33", 
  "read_parameter_list1", "@34", "read_parameter", 
  "writeln_parameter_list", "write_parameter_list", "@35", 
  "write_parameter_list1", "@36", "write_expression", "@37", 
  "output_width", "@38", "structured_statement", "conditional_statement", 
  "if_statement", "@39", "@40", "else_part", "@41", "case_statement", 
  "@42", "@43", "optional_semicolon", "case_index", 
  "case_list_element_sequence", "case_list_element", "@44", 
  "case_constant_list", "case_constant", "@45", "repetitive_statement", 
  "while_statement", "@46", "@47", "repeat_statement", "@48", "@49", 
  "for_statement", "@50", "@51", "@52", "initial_value", "final_value", 
  "to_or_downto", "assignment_statement", "@53", "@54", "goto_statement", 
  "assign", "variable_access", "buffer_variable", "entire_variable", 
  "variable_identifier", "component_variable", "indexed_variable", "@55", 
  "array_variable", "field_designator", "record_variable", 
  "index_expression_list", "@56", "expression", "@57", 
  "boolean_expression", "relational_operator", "simple_expression", "@58", 
  "@59", "@60", "adding_operator", "term", "@61", "@62", "@63", 
  "multiplying_operator", "factor", "@64", "@65", "@66", 
  "optional_actual_parameter_list", "actual_parameter_list", "@67", 
  "actual_parameter", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    59,    46,    40,    41,
      44,    58,    61,    43,    45,    91,    93,    94,    60,    62,
      47,    42
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,    82,    83,    85,    84,    86,    86,    88,    89,    90,
      91,    92,    87,    93,    93,    94,    95,    95,    96,    96,
      97,    97,    98,    99,   100,    98,   101,   101,   102,   103,
     102,   104,   105,   106,   104,   107,   108,   109,   108,   111,
     110,   112,   110,   113,   114,   115,   114,   116,   117,   117,
     118,   118,   119,   119,   120,   120,   122,   121,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   124,   124,   124,   124,   124,   124,   124,   125,
     125,   126,   126,   128,   127,   129,   129,   130,   130,   130,
     131,   132,   133,   133,   134,   134,   134,   135,   137,   136,
     138,   139,   140,   141,   140,   142,   142,   143,   144,   145,
     146,   147,   146,   148,   148,   149,   150,   149,   151,   152,
     152,   153,   154,   155,   156,   155,   157,   158,   159,   157,
     160,   161,   161,   162,   162,   163,   164,   165,   164,   166,
     167,   169,   168,   170,   170,   172,   171,   171,   173,   173,
     174,   175,   175,   175,   175,   177,   176,   178,   176,   179,
     176,   180,   176,   181,   176,   182,   182,   184,   183,   185,
     186,   185,   187,   188,   188,   190,   189,   191,   192,   191,
     193,   194,   193,   195,   196,   195,   197,   197,   197,   198,
     198,   200,   201,   199,   202,   203,   202,   205,   206,   204,
     207,   207,   208,   209,   209,   211,   210,   212,   212,   214,
     213,   213,   215,   215,   215,   217,   218,   216,   220,   221,
     219,   223,   224,   225,   222,   226,   227,   228,   228,   230,
     231,   229,   232,   233,   234,   234,   234,   235,   236,   237,
     238,   238,   240,   239,   241,   242,   243,   244,   245,   244,
     246,   247,   246,   248,   249,   249,   249,   249,   249,   249,
     250,   251,   250,   250,   252,   250,   253,   250,   254,   254,
     255,   256,   255,   257,   255,   258,   255,   259,   259,   259,
     260,   260,   260,   260,   260,   260,   261,   260,   262,   260,
     263,   260,   264,   264,   265,   266,   265,   267
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     4,     0,     4,     0,     3,     0,     0,     0,
       0,     0,    11,     1,     3,     1,     0,     1,     1,     2,
       1,     1,     4,     0,     0,    12,     2,     5,     0,     0,
       4,     4,     0,     0,    12,     3,     1,     0,     4,     0,
       2,     0,     3,     3,     1,     0,     4,     1,     0,     3,
       1,     3,     0,     2,     1,     2,     0,     5,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     1,     1,     2,     2,     1,     2,     2,     1,     0,
       2,     1,     2,     0,     5,     1,     1,     1,     1,     1,
       1,     3,     1,     2,     1,     1,     1,     3,     0,     7,
       2,     1,     1,     0,     4,     3,     1,     3,     2,     1,
       1,     0,     4,     0,     3,     1,     0,     4,     1,     0,
       4,     1,     1,     1,     0,     4,     0,     0,     0,     7,
       2,     0,     2,     1,     2,     4,     1,     0,     4,     1,
       1,     0,     4,     1,     3,     0,     3,     2,     0,     1,
       2,     0,     1,     1,     1,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     1,     0,     4,     1,
       0,     4,     1,     0,     1,     0,     4,     1,     0,     4,
       1,     0,     4,     1,     0,     4,     1,     1,     1,     1,
       1,     0,     0,     7,     0,     0,     3,     0,     0,     8,
       0,     1,     1,     1,     3,     0,     4,     1,     3,     0,
       2,     1,     1,     1,     1,     0,     0,     6,     0,     0,
       6,     0,     0,     0,    11,     1,     1,     1,     1,     0,
       0,     5,     2,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     0,     5,     1,     3,     1,     1,     0,     4,
       1,     0,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     2,     0,     4,     0,     4,     1,     1,
       1,     0,     4,     0,     4,     0,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     5,     0,     4,
       0,     3,     0,     3,     1,     0,     4,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     0,     0,     0,     3,     1,     7,     5,     0,    48,
       0,     4,     2,     0,     8,    15,     0,    13,    50,     0,
      52,     6,     0,    49,     0,     0,     9,    14,    51,     0,
      53,    54,    79,    56,    55,     0,   131,     0,     0,    80,
      81,     0,    10,    70,    71,    64,    58,    61,    67,     0,
       0,     0,    83,    82,   139,   132,   133,     0,   136,    16,
      65,    59,    62,    68,    66,    60,    69,    57,     0,   134,
     137,     0,     0,     0,    11,    17,    18,    21,     0,    20,
       0,     0,     0,     0,   113,    78,    72,    85,     0,     0,
       0,     0,     0,    86,    87,    90,    88,    92,    96,    94,
      89,    95,     0,     0,    26,    28,     0,    19,    23,    32,
       0,     0,    93,   118,     0,   119,   109,   110,     0,   115,
      73,    76,    74,    77,   101,   100,     0,    84,   138,   135,
      29,     0,    35,   141,    12,   140,     0,    48,     0,    48,
     106,     0,     0,   102,    97,   107,     0,   108,   111,   116,
       0,    75,    91,    39,     0,   148,    22,    52,    31,    52,
       0,   103,     0,   122,     0,   121,   113,     0,   114,    41,
       0,    36,     0,    27,     0,     0,   143,   145,   149,    79,
      79,   105,     0,    98,   127,   112,   117,     0,    37,    30,
      47,    40,     0,    44,   150,   142,   148,   197,     0,   191,
     218,   215,   186,   151,   147,   187,   189,   190,   188,   213,
     212,   214,   131,   131,   104,     0,   120,   123,   209,    42,
      39,    45,     0,   144,     0,     0,     0,   148,     0,     0,
     155,   157,   159,   161,   163,   146,   153,   152,   154,   229,
     236,   234,   238,   235,   240,     0,   241,     0,     0,     0,
      99,   124,   211,     0,   207,     0,    38,     0,    43,   290,
     283,   284,   280,   281,   282,   239,   288,     0,   261,     0,
     285,   202,   250,   260,   270,   221,   253,     0,     0,     0,
     232,     0,   165,     0,   173,   292,   237,     0,   242,     0,
      24,    33,   127,   209,     0,   210,    46,     0,   286,     0,
     263,     0,   198,   259,   257,   256,   258,   254,   255,   251,
     266,   269,   268,   264,   275,   278,   279,   271,   277,   273,
       0,   192,   219,   216,   167,   156,   158,   166,   175,   160,
     162,   174,     0,   164,   233,   230,     0,   245,   148,   148,
     125,   208,   128,   291,     0,     0,   262,   209,     0,     0,
       0,     0,     0,     0,     0,   225,   148,     0,   148,     0,
       0,   297,     0,   294,     0,     0,   247,     0,     0,   113,
       0,   289,   200,   203,     0,   252,   267,   265,   276,   272,
     274,   228,   227,   222,   194,   220,   217,   239,     0,   169,
     172,     0,   177,   180,   293,   295,   231,   248,   243,     0,
       0,   119,     0,   287,   209,     0,   205,     0,   195,   193,
     168,   170,   176,   178,   181,     0,     0,    25,    34,   130,
     129,   204,   199,   148,     0,   226,   148,     0,     0,     0,
     296,   249,   206,   223,   196,   171,   179,   182,   183,   148,
     184,   224,     0,   185
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     2,     3,     7,    11,     8,     9,    20,    32,    59,
     106,    16,    17,    74,    75,    76,    77,   137,   338,    78,
     131,   153,    79,   139,   339,    80,   170,   220,   171,   172,
     187,   191,   192,   257,   193,    14,    19,    26,    30,    31,
      37,    51,    91,    36,    39,    40,    68,   124,    93,    94,
      95,    96,    97,    98,    99,   215,   100,   125,   142,   182,
     143,   101,   114,   115,   116,   166,   117,   118,   167,   119,
     147,   164,   165,   216,   292,   217,   218,   369,   402,    42,
      55,    56,    57,   102,    58,   134,   135,   155,   175,   176,
     203,   177,   178,   235,   236,   281,   282,   283,   284,   285,
     326,   325,   359,   388,   427,   389,   330,   329,   360,   391,
     428,   392,   429,   437,   442,   204,   205,   206,   226,   356,
     409,   426,   207,   224,   347,   405,   269,   372,   373,   423,
     374,   254,   255,   208,   209,   228,   358,   210,   227,   357,
     211,   320,   407,   439,   354,   424,   383,   237,   287,   364,
     238,   335,   270,   240,   241,   242,   243,   244,   336,   245,
     246,   247,   365,   416,   361,   348,   277,   309,   272,   301,
     350,   349,   313,   273,   352,   353,   351,   319,   274,   344,
     299,   297,   333,   362,   415,   363
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -263
static const short yypact[] =
{
     -11,   -27,    43,   -16,  -263,  -263,  -263,    21,    70,    91,
       0,  -263,  -263,    88,  -263,  -263,   -40,  -263,  -263,    42,
     165,  -263,     0,  -263,   111,   130,  -263,  -263,  -263,   140,
     130,  -263,   185,  -263,  -263,   161,   197,   126,   162,   161,
    -263,   170,  -263,  -263,  -263,  -263,  -263,  -263,  -263,   119,
     114,   171,  -263,  -263,  -263,   170,  -263,    45,  -263,    17,
    -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,     5,  -263,
    -263,     5,   173,   174,  -263,    17,  -263,  -263,   175,  -263,
     176,   169,   212,    44,   180,  -263,  -263,   198,    90,   102,
       5,   204,   181,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,  -263,   170,   182,    -8,   183,   244,  -263,   235,   237,
     131,     5,  -263,  -263,   241,   248,   189,  -263,    56,  -263,
    -263,  -263,  -263,  -263,  -263,  -263,   136,  -263,  -263,  -263,
    -263,   186,  -263,  -263,  -263,  -263,   190,    91,   192,    91,
     198,   217,    23,  -263,  -263,  -263,   195,  -263,  -263,  -263,
       5,  -263,  -263,   225,   199,   200,  -263,   165,  -263,   165,
     136,  -263,   239,  -263,   242,  -263,   180,   180,  -263,  -263,
     142,  -263,   202,  -263,   194,    -2,  -263,    99,  -263,   185,
     185,  -263,   131,  -263,     3,  -263,  -263,   202,  -263,  -263,
    -263,  -263,   132,  -263,  -263,  -263,   200,  -263,   203,  -263,
    -263,  -263,  -263,    -4,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,  -263,   197,   197,  -263,     5,   205,  -263,   228,  -263,
     225,  -263,   207,  -263,    60,   224,    60,   200,    60,   211,
    -263,  -263,  -263,  -263,   103,  -263,  -263,  -263,  -263,    86,
    -263,  -263,  -263,  -263,  -263,   201,  -263,   208,   269,   272,
    -263,  -263,  -263,   143,  -263,   126,  -263,   202,  -263,  -263,
    -263,  -263,  -263,  -263,  -263,   210,  -263,    83,  -263,   255,
      86,    96,    12,    11,  -263,  -263,    96,   249,   -19,    31,
    -263,   214,   214,   215,   215,   216,  -263,   234,  -263,   220,
    -263,  -263,     3,   228,   218,  -263,  -263,    83,  -263,    60,
      11,    83,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
      60,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,  -263,    60,  -263,  -263,  -263,    60,  -263,   200,   200,
    -263,  -263,  -263,  -263,    60,   -23,    11,   228,    60,    83,
      83,    83,    83,    83,    35,    96,   200,    60,   200,   222,
      60,    96,   146,  -263,    60,    24,    96,    14,    15,   180,
     148,  -263,   223,  -263,   151,    12,    11,    11,  -263,  -263,
    -263,  -263,  -263,  -263,   277,    96,  -263,  -263,   154,  -263,
      86,   159,  -263,   -20,  -263,  -263,    96,  -263,  -263,   226,
     227,   248,   221,  -263,    30,   279,  -263,    60,  -263,  -263,
    -263,  -263,  -263,  -263,  -263,    60,    60,  -263,  -263,  -263,
    -263,  -263,  -263,   200,   285,    96,   200,   222,    60,    60,
    -263,    96,  -263,  -263,  -263,  -263,  -263,  -263,    87,   200,
    -263,  -263,    60,    96
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,  -263,   273,  -263,  -263,   229,  -263,  -263,  -263,  -263,
     191,  -263,  -263,  -263,  -263,  -263,  -263,  -263,    77,  -263,
    -263,   112,  -263,  -263,    41,    32,  -263,    -3,  -263,   270,
    -263,    46,   -98,   -78,  -263,   263,  -263,   -60,  -263,  -263,
    -263,  -263,   230,  -263,  -263,  -263,  -263,  -108,  -263,  -263,
     121,  -263,  -263,   -64,  -263,  -263,   141,  -263,  -263,   139,
     -93,  -263,  -263,  -263,  -263,    18,  -263,  -263,  -263,    19,
    -263,   254,  -263,  -263,   209,  -263,   135,  -263,  -220,  -196,
    -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,    33,  -263,  -263,  -263,  -113,  -263,    34,  -263,  -263,
    -263,  -112,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,   -87,  -263,
     101,    27,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,  -263,  -202,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,  -263,  -263,  -263,  -222,  -263,  -263,  -263,   -26,  -263,
    -263,  -263,  -263,  -262,  -263,  -263,  -263,  -263,  -256,  -263,
    -263,  -263,  -263,   -21,  -263,   -94
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -247
static const short yytable[] =
{
     223,   239,   271,   144,   276,   300,   279,   278,    92,    81,
     195,   103,   141,   229,   314,  -126,   322,     1,    82,   315,
     303,   304,   305,   303,   304,   305,   399,   400,   152,    21,
      22,    83,   316,    72,    84,   230,   231,   310,     4,   346,
     323,   343,  -201,     5,    73,   381,   371,   196,    81,   306,
       6,   414,   306,   232,   233,   307,   308,    82,   307,   308,
     130,   234,   181,   -28,   196,    15,    85,    86,   382,  -126,
      87,   252,  -126,    84,   303,   304,   305,   345,    88,    89,
     196,   196,    90,   259,   141,   311,   312,   376,   377,    10,
     168,   317,   318,   161,   397,   378,   379,   380,   355,   162,
     398,   212,   213,   306,   133,   197,   259,   250,    23,   307,
     308,    13,    24,   198,   366,    70,    71,   199,   367,   368,
     260,   261,   262,   263,   264,   265,   149,   150,   266,   200,
     303,   304,   305,   267,   268,   385,   201,    12,   393,   303,
     304,   305,   396,   260,   261,   262,   263,   264,   265,  -239,
      18,   266,   120,  -246,   179,   121,   180,   390,   440,   306,
     384,  -244,   386,   286,   122,   307,   308,   123,   306,   157,
    -239,   159,    25,    28,   307,   308,    64,    65,  -239,    66,
    -239,    60,    61,    62,    63,   425,    43,    44,    45,    46,
      47,    48,    85,    86,   431,    29,   140,    85,    86,    49,
      50,   151,   221,   222,    88,    89,   393,   438,   188,    88,
      89,   189,    33,   293,   294,   394,   395,   403,   395,    35,
     443,   293,   406,   410,   411,   390,    38,   432,   412,   413,
     434,   248,   249,    41,    52,    54,   111,    67,   104,   105,
     -75,   108,   109,   441,   110,   113,   126,   127,   129,   133,
     136,   130,   138,   145,   146,   148,   156,   154,   158,   160,
     163,   169,   174,   183,   173,   194,   184,   190,   225,   252,
     275,   251,   258,   280,   290,   289,   288,   291,   298,   302,
     334,   321,   324,   328,   332,   337,   342,   387,   408,   404,
     420,   422,   417,   418,   433,    27,   132,   256,   296,   219,
      34,   295,    53,   214,   107,   401,   186,   185,   419,    69,
     340,   128,   202,   112,   435,   327,   436,   421,   331,   253,
     341,   430,   375,   370
};

static const unsigned short yycheck[] =
{
     196,   203,   224,   111,   226,   267,   228,   227,    68,     4,
      12,    71,   110,    17,     3,    12,    35,    28,    13,     8,
      43,    44,    45,    43,    44,    45,    12,    12,   126,    69,
      70,    26,    21,    16,    29,    39,    40,    25,    65,   301,
       9,   297,    12,     0,    27,    10,    69,    66,     4,    72,
      66,    71,    72,    57,    58,    78,    79,    13,    78,    79,
      68,    65,   160,    71,    66,    65,    61,    62,    33,    66,
      65,    41,    69,    29,    43,    44,    45,   299,    73,    74,
      66,    66,    77,    23,   182,    73,    74,   349,   350,    68,
     150,    80,    81,    70,    70,   351,   352,   353,   320,    76,
      76,   179,   180,    72,     5,     6,    23,   215,    66,    78,
      79,    20,    70,    14,   336,    70,    71,    18,   338,   339,
      60,    61,    62,    63,    64,    65,    70,    71,    68,    30,
      43,    44,    45,    73,    74,   357,    37,    67,   360,    43,
      44,    45,   364,    60,    61,    62,    63,    64,    65,    46,
      62,    68,    62,    67,   157,    65,   159,   359,    71,    72,
     356,    75,   358,    77,    62,    78,    79,    65,    72,   137,
      67,   139,     7,    62,    78,    79,    62,    63,    75,    65,
      77,    62,    63,    64,    65,   407,    60,    61,    62,    63,
      64,    65,    61,    62,   416,    65,    65,    61,    62,    73,
      74,    65,    70,    71,    73,    74,   428,   429,    66,    73,
      74,    69,    72,    70,    71,    69,    70,    69,    70,    34,
     442,    70,    71,    69,    70,   427,    65,   423,    69,    70,
     426,   212,   213,    36,    72,    65,    24,    66,    65,    65,
      42,    66,    66,   439,    75,    65,    42,    66,    66,     5,
      15,    68,    15,    12,     6,    66,    66,    71,    66,    42,
      65,    36,    62,    24,    65,    71,    24,    65,    65,    41,
      46,    66,    65,    62,     5,    67,    75,     5,    68,    24,
      46,    32,    68,    68,    68,    65,    68,    65,    11,    66,
      69,    12,    66,    66,     9,    22,   105,   220,   257,   187,
      30,   255,    39,   182,    75,   369,   167,   166,   401,    55,
     292,   102,   177,    83,   427,   282,   428,   404,   284,   218,
     293,   415,   348,   344
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,    28,    83,    84,    65,     0,    66,    85,    87,    88,
      68,    86,    67,    20,   117,    65,    93,    94,    62,   118,
      89,    69,    70,    66,    70,     7,   119,    94,    62,    65,
     120,   121,    90,    72,   121,    34,   125,   122,    65,   126,
     127,    36,   161,    60,    61,    62,    63,    64,    65,    73,
      74,   123,    72,   127,    65,   162,   163,   164,   166,    91,
      62,    63,    64,    65,    62,    63,    65,    66,   128,   163,
      70,    71,    16,    27,    95,    96,    97,    98,   101,   104,
     107,     4,    13,    26,    29,    61,    62,    65,    73,    74,
      77,   124,   129,   130,   131,   132,   133,   134,   135,   136,
     138,   143,   165,   129,    65,    65,    92,    97,    66,    66,
      75,    24,   134,    65,   144,   145,   146,   148,   149,   151,
      62,    65,    62,    65,   129,   139,    42,    66,   166,    66,
      68,   102,   102,     5,   167,   168,    15,    99,    15,   105,
      65,   124,   140,   142,   139,    12,     6,   152,    66,    70,
      71,    65,   124,   103,    71,   169,    66,   117,    66,   117,
      42,    70,    76,    65,   153,   154,   147,   150,   129,    36,
     108,   110,   111,    65,    62,   170,   171,   173,   174,   119,
     119,   124,   141,    24,    24,   148,   151,   112,    66,    69,
      65,   113,   114,   116,    71,    12,    66,     6,    14,    18,
      30,    37,   168,   172,   197,   198,   199,   204,   215,   216,
     219,   222,   125,   125,   142,   137,   155,   157,   158,   113,
     109,    70,    71,   171,   205,    65,   200,   220,   217,    17,
      39,    40,    57,    58,    65,   175,   176,   229,   232,   234,
     235,   236,   237,   238,   239,   241,   242,   243,   161,   161,
     139,    66,    41,   212,   213,   214,   110,   115,    65,    23,
      60,    61,    62,    63,    64,    65,    68,    73,    74,   208,
     234,   246,   250,   255,   260,    46,   246,   248,   170,   246,
      62,   177,   178,   179,   180,   181,    77,   230,    75,    67,
       5,     5,   156,    70,    71,   123,   116,   263,    68,   262,
     255,   251,    24,    43,    44,    45,    72,    78,    79,   249,
      25,    73,    74,   254,     3,     8,    21,    80,    81,   259,
     223,    32,    35,     9,    68,   183,   182,   183,    68,   189,
     188,   189,    68,   264,    46,   233,   240,    65,   100,   106,
     157,   213,    68,   260,   261,   246,   255,   206,   247,   253,
     252,   258,   256,   257,   226,   246,   201,   221,   218,   184,
     190,   246,   265,   267,   231,   244,   246,   170,   170,   159,
     265,    69,   209,   210,   212,   250,   255,   255,   260,   260,
     260,    10,    33,   228,   171,   246,   171,    65,   185,   187,
     234,   191,   193,   246,    69,    70,   246,    70,    76,    12,
      12,   145,   160,    69,    66,   207,    71,   224,    11,   202,
      69,    70,    69,    70,    71,   266,   245,    66,    66,   152,
      69,   210,    12,   211,   227,   246,   203,   186,   192,   194,
     267,   246,   171,     9,   171,   187,   193,   195,   246,   225,
      71,   171,   196,   246
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 127 "gram.y"
    {
		  prog_symbol = yyvsp[0].sym;
		  open_name_file ();
                  open_header_file ();
		  if (def_filename.length() > 0)
		    open_def_file ();
		}
    break;

  case 7:
#line 143 "gram.y"
    {
		  redir_file (H_FILE_NUM);
		}
    break;

  case 8:
#line 147 "gram.y"
    {
		  if (def_filename.length() > 0)
		    redir_file (DEF_FILE_NUM);
		}
    break;

  case 9:
#line 152 "gram.y"
    {
		  if (def_filename.length() > 0)
		    redir_file (H_FILE_NUM);
		}
    break;

  case 10:
#line 158 "gram.y"
    {
		  if (nglobalvars > 0 && var_struct_name.length() > 0)
		    {
		      out_s ("};\n");
		      if (class_name.length() == 0)
			out_s ("C4PEXTERN ");
		      out_form ("struct %s %s;\n",
				var_struct_class_name.c_str(),
				var_struct_name.c_str());
		    }
		  redir_file (C_FILE_NUM);
		}
    break;

  case 11:
#line 171 "gram.y"
    {
                  if (! one_c_file)
		    begin_new_c_file (prog_symbol->s_repr, 1);
		  routine_handle = 0;
		  forget_fast_vars ();
		  if (class_name.length() == 0)
		    {
		      if (dll_flag)
			{
			  redir_file (H_FILE_NUM);
			  out_form ("\n#ifdef __cplusplus\n\
extern \"C++\" int __cdecl %s (int argc, const char * * argv);\n\
#endif\n",
				    entry_name.c_str());
			  redir_file (C_FILE_NUM);
			  out_form ("\
\nextern \"C++\" int __cdecl %s (int argc, const char * * argv)\n\n",
				    entry_name.c_str());
			}
		      else
			out_s ("\
\nvoid __cdecl int main (int argc, const char * * argv)\n\n");
		    }
		  else
		    {
		      redir_file (H_FILE_NUM);
		      out_form ("int Run (int argc, const char ** argv);\n");
		      redir_file (C_FILE_NUM);
		      out_form ("\nint %s::Run (int argc, const char ** argv)\n\n",
				class_name.c_str());
		    }
		}
    break;

  case 12:
#line 204 "gram.y"
    {
		  if (n_fast_vars)
		    declare_fast_var_macro (routine_handle);
		  close_file (C_FILE_NUM);
		  close_header_file ();
		  if (def_filename.length() > 0)
		    close_def_file ();
		  close_name_file ();
		}
    break;

  case 15:
#line 233 "gram.y"
    {
		}
    break;

  case 22:
#line 254 "gram.y"
    {
		}
    break;

  case 23:
#line 257 "gram.y"
    {
		  routine_handle += 1;
		  begin_routine (reinterpret_cast<prototype_node*>(yyvsp[-1].type_ptr), routine_handle);
		}
    break;

  case 24:
#line 269 "gram.y"
    {
		  symbol_t * result_type
		    = reinterpret_cast<prototype_node *>(yyvsp[-7].type_ptr)->result_type;
		  out_form ("\n%s c4p_result;\n\n",
			    (result_type->s_translated_type
			     ? result_type->s_translated_type
			     : result_type->s_repr));
		  out_form ("C4P_PROC_ENTRY (%u);\n", routine_handle);
		}
    break;

  case 25:
#line 279 "gram.y"
    {
		  out_s ("\n");
		  if (auto_exit_label_flag)
		    {
		      out_form ("c4p_L%ld:\n", auto_exit_label);
		      auto_exit_label_flag = false;
		    }
		  out_form ("C4P_PROC_EXIT (%u);\n", routine_handle);
		  out_s ("return (c4p_result);\n");
		  end_routine (routine_handle);
		}
    break;

  case 26:
#line 294 "gram.y"
    {
		  if (yyvsp[0].sym->s_kind != FUNCTION_IDENTIFIER)
		    c4p_error ("`%s' is not a function name", yyvsp[0].sym->s_repr);
		  yyval.type_ptr = yyvsp[0].sym->s_type_ptr;
		}
    break;

  case 27:
#line 301 "gram.y"
    {
		  if (yyvsp[-3].sym->s_kind == FUNCTION_IDENTIFIER)
		    { /* already declared (forward */
		      yyval.type_ptr = yyvsp[-3].sym->s_type_ptr;
		    }
		  else
		    {
		      if (yyvsp[-3].sym->s_kind != UNDEFINED_IDENTIFIER)
			c4p_error ("`%s' already defined", yyvsp[-3].sym->s_repr);
		      if (yyvsp[0].sym->s_kind != TYPE_IDENTIFIER)
			c4p_error ("`%s' is not a type identifier", yyvsp[0].sym->s_repr);
		      yyval.type_ptr = new_type_node(PROTOTYPE_NODE, yyvsp[-3].sym, yyvsp[-2].type_ptr, yyvsp[0].sym);
		      yyvsp[-3].sym->s_kind = FUNCTION_IDENTIFIER;
		      yyvsp[-3].sym->s_type = PROTOTYPE_NODE;
		      yyvsp[-3].sym->s_type_ptr = yyval.type_ptr;
		    }
		}
    break;

  case 28:
#line 322 "gram.y"
    {
		  yyval.type_ptr = 0;
		}
    break;

  case 29:
#line 326 "gram.y"
    {
		  mark_symbol_table ();
		}
    break;

  case 30:
#line 330 "gram.y"
    {
		  unmark_symbol_table ();
		  yyval.type_ptr = yyvsp[-1].type_ptr;
		}
    break;

  case 31:
#line 338 "gram.y"
    {
		}
    break;

  case 32:
#line 341 "gram.y"
    {
		  routine_handle += 1;
		  begin_routine (reinterpret_cast<prototype_node*>(yyvsp[-1].type_ptr), routine_handle);
		}
    break;

  case 33:
#line 350 "gram.y"
    {
		  out_form ("C4P_PROC_ENTRY (%u);\n", routine_handle);
		}
    break;

  case 34:
#line 354 "gram.y"
    {
		  out_form ("C4P_PROC_EXIT (%u);\n", routine_handle);
		  end_routine (routine_handle);
		}
    break;

  case 35:
#line 362 "gram.y"
    {
		  if (yyvsp[-1].sym->s_kind == PROCEDURE_IDENTIFIER)
		    { /* already declared (forward) */
		      yyval.type_ptr = yyvsp[-1].sym->s_type_ptr;
		    }
		  else
		    {
		      if (yyvsp[-1].sym->s_kind != UNDEFINED_IDENTIFIER)
		        c4p_error ("`%s' already declared", yyvsp[-1].sym->s_repr);
		     yyval.type_ptr = new_type_node(PROTOTYPE_NODE, yyvsp[-1].sym, yyvsp[0].type_ptr, 0);
		     yyvsp[-1].sym->s_kind = PROCEDURE_IDENTIFIER;
		     yyvsp[-1].sym->s_type = PROTOTYPE_NODE;
		     yyvsp[-1].sym->s_type_ptr = yyval.type_ptr;
		    }
		}
    break;

  case 37:
#line 382 "gram.y"
    {
		  yyval.type_ptr = last_parameter;
		}
    break;

  case 38:
#line 386 "gram.y"
    {
		  reinterpret_cast<parameter_node *>(yyvsp[-1].type_ptr)->next
		    = reinterpret_cast<parameter_node*>(yyvsp[0].type_ptr);
		  yyval.type_ptr = yyvsp[-3].type_ptr;
		}
    break;

  case 39:
#line 394 "gram.y"
    {
		  by_reference = false;
		}
    break;

  case 40:
#line 398 "gram.y"
    {
		  yyval.type_ptr = yyvsp[0].type_ptr;
		}
    break;

  case 41:
#line 402 "gram.y"
    {
		  by_reference = true;
		}
    break;

  case 42:
#line 406 "gram.y"
    {
		  yyval.type_ptr = yyvsp[0].type_ptr;
		}
    break;

  case 43:
#line 417 "gram.y"
    {
		  parameter_node * par = reinterpret_cast<parameter_node*>(yyvsp[-2].type_ptr);
		  if (yyvsp[0].sym->s_kind != TYPE_IDENTIFIER)
		    c4p_error ("`%s' is not a type identifier", yyvsp[0].sym->s_repr);
		  while (par != 0)
		    {
		      if (yyvsp[0].sym->s_type != ARRAY_NODE)
		        par->by_reference = by_reference;
		      else
		        par->by_reference = false;
		      par->type_symbol = yyvsp[0].sym;
		      par = par->next;
		    }
      		  yyval.type_ptr = yyvsp[-2].type_ptr;
		}
    break;

  case 45:
#line 437 "gram.y"
    {
		  yyval.type_ptr = last_parameter;
		}
    break;

  case 46:
#line 441 "gram.y"
    {
		  reinterpret_cast<parameter_node *>(yyvsp[-1].type_ptr)->next
		    = reinterpret_cast<parameter_node*>(yyvsp[0].type_ptr);
		  yyval.type_ptr = yyvsp[-3].type_ptr;
		}
    break;

  case 47:
#line 450 "gram.y"
    {
		  yyval.type_ptr = new_type_node(PARAMETER_NODE, yyvsp[0].sym->s_repr);
		  last_parameter = reinterpret_cast<parameter_node*>(yyval.type_ptr);
		}
    break;

  case 50:
#line 464 "gram.y"
    {
		}
    break;

  case 56:
#line 482 "gram.y"
    {
		  out_form ("#define %s ", yyvsp[-1].sym->s_repr);
		}
    break;

  case 57:
#line 486 "gram.y"
    {
		  define_symbol (yyvsp[-4].sym, CONSTANT_IDENTIFIER, block_level,
				 last_type, 0, &last_value);
		  out_s ("\n");
		}
    break;

  case 58:
#line 501 "gram.y"
    {
		  last_value.fvalue = yyvsp[0].fvalue;
		  last_type = REAL_TYPE;
		  out_form ("%lf", static_cast<double>(last_value.fvalue));
		}
    break;

  case 59:
#line 507 "gram.y"
    {
		  last_value.fvalue = yyvsp[0].fvalue;
		  last_type = REAL_TYPE;
		  out_form ("%lf", static_cast<double>(last_value.fvalue));
		}
    break;

  case 60:
#line 513 "gram.y"
    {
		  last_value.fvalue = - yyvsp[0].fvalue;
		  last_type = REAL_TYPE;
		  out_form ("%lf", static_cast<double>(last_value.fvalue));
		}
    break;

  case 61:
#line 519 "gram.y"
    {
		  last_value.fvalue = yyvsp[0].fvalue;
		  last_type = LONG_REAL_TYPE;
		  out_form ("%lf", static_cast<double>(last_value.fvalue));
		}
    break;

  case 62:
#line 525 "gram.y"
    {
		  last_value.fvalue = yyvsp[0].fvalue;
		  last_type = LONG_REAL_TYPE;
		  out_form ("%lf", static_cast<double>(last_value.fvalue));
		}
    break;

  case 63:
#line 531 "gram.y"
    {
		  last_value.fvalue = - yyvsp[0].fvalue;
		  last_type = LONG_REAL_TYPE;
		  out_form ("%lf", static_cast<double>(last_value.fvalue));
		}
    break;

  case 64:
#line 538 "gram.y"
    {
		  last_value.ivalue = yyvsp[0].value;
		  last_type = INTEGER_TYPE;
		  out_form ("%ldl", last_value.ivalue);
		}
    break;

  case 65:
#line 544 "gram.y"
    {
		  last_value.ivalue = yyvsp[0].value;
		  last_type = INTEGER_TYPE;
		  out_form ("%ldl", last_value.ivalue);
		}
    break;

  case 66:
#line 550 "gram.y"
    {
		  last_value.ivalue = - yyvsp[0].value;
		  last_type = INTEGER_TYPE;
		  out_form ("%ldl", last_value.ivalue);
		}
    break;

  case 67:
#line 556 "gram.y"
    {
		  if (yyvsp[0].sym->s_kind != CONSTANT_IDENTIFIER)
		    c4p_error ("`%s' is not a constant identifier",
			       yyvsp[0].sym->s_repr);
		  last_value = yyvsp[0].sym->s_value;
		  last_type = yyvsp[0].sym->s_type;
		  out_form ("%s", yyvsp[0].sym->s_repr);
		}
    break;

  case 68:
#line 565 "gram.y"
    {
		  if (yyvsp[0].sym->s_kind != CONSTANT_IDENTIFIER)
		    c4p_error ("`%s' is not a constant identifier",
			       yyvsp[0].sym->s_repr);
		  last_value = yyvsp[0].sym->s_value;
		  last_type = yyvsp[0].sym->s_type;
		  out_form ("%s", yyvsp[0].sym->s_repr);
		}
    break;

  case 69:
#line 574 "gram.y"
    {
		  if (yyvsp[0].sym->s_kind != CONSTANT_IDENTIFIER)
		    c4p_error ("`%s' is not a constant identifier",
			       yyvsp[0].sym->s_repr);
		  /* fixme: suspicious: */
		  last_value.ivalue = - yyvsp[0].sym->s_value.ivalue;
		  last_type = yyvsp[0].sym->s_type;
		  out_form ("(-%s)", yyvsp[0].sym->s_repr);
		}
    break;

  case 70:
#line 584 "gram.y"
    {
		  last_value.ivalue = yyvsp[0].value;
		  last_type = STRING_TYPE;
		  out_s ("\"");
		  translate_string (get_string ((STRING_PTR) yyvsp[0].value));
		  out_s ("\"");
		}
    break;

  case 71:
#line 592 "gram.y"
    {
		  last_value.ivalue = yyvsp[0].value;
		  last_type = CHARACTER_TYPE;
		  out_s ("\'");
		  translate_char (static_cast<int>(yyvsp[0].value));
		  out_s ("\'");
		}
    break;

  case 73:
#line 604 "gram.y"
    {
		  yyval.value = yyvsp[0].value;
		}
    break;

  case 74:
#line 608 "gram.y"
    {
		  yyval.value = - yyvsp[0].value;
		}
    break;

  case 75:
#line 612 "gram.y"
    {
		  if (yyvsp[0].sym->s_kind != CONSTANT_IDENTIFIER)
		    c4p_error ("`%s' is not a constant identifier",
			       yyvsp[0].sym->s_repr);
		  yyval.value = yyvsp[0].sym->s_value.ivalue;
		}
    break;

  case 76:
#line 619 "gram.y"
    {
		  if (yyvsp[0].sym->s_kind != CONSTANT_IDENTIFIER)
		    c4p_error ("`%s' is not a constant identifier",
			       yyvsp[0].sym->s_repr);
		  yyval.value = yyvsp[0].sym->s_value.ivalue;
		}
    break;

  case 77:
#line 626 "gram.y"
    {
		  if (yyvsp[0].sym->s_kind != CONSTANT_IDENTIFIER)
		    c4p_error ("`%s' is not a constant identifier",
			       yyvsp[0].sym->s_repr);
		  yyval.value = - yyvsp[0].sym->s_value.ivalue;
		}
    break;

  case 83:
#line 647 "gram.y"
    {
		  out_s ("typedef ");
		}
    break;

  case 84:
#line 651 "gram.y"
    {
		  define_type (yyvsp[-4].sym, block_level, last_type, yyvsp[-1].type_ptr);
		  out_s (";\n");
		}
    break;

  case 85:
#line 659 "gram.y"
    {
		  if (yyvsp[0].sym->s_kind != TYPE_IDENTIFIER)
		    c4p_error ("`%s' is not a type identifier", yyvsp[0].sym->s_repr);
		  last_type = NAMED_TYPE_NODE;
		  yyval.type_ptr = new_type_node(NAMED_TYPE_NODE, yyvsp[0].sym);
		}
    break;

  case 91:
#line 683 "gram.y"
    {
		  last_type = SUBRANGE_NODE;
		  yyval.type_ptr = new_type_node(SUBRANGE_NODE, yyvsp[-2].value, yyvsp[0].value);
		}
    break;

  case 93:
#line 692 "gram.y"
    {
		  yyval.type_ptr = yyvsp[0].type_ptr;
		}
    break;

  case 97:
#line 708 "gram.y"
    {
		  yyval.type_ptr = new_type_node(FILE_NODE, last_type, yyvsp[0].type_ptr);
		  last_type = FILE_NODE;
		}
    break;

  case 98:
#line 716 "gram.y"
    {
		  yyval.type_ptr = last_array_node;
		}
    break;

  case 99:
#line 720 "gram.y"
    {
		  reinterpret_cast<array_node *>(yyvsp[-1].type_ptr)->component_type
		    = last_type;
		  reinterpret_cast<array_node *>(yyvsp[-1].type_ptr)->component_type_ptr
		    = yyvsp[0].type_ptr;
		  last_type = ARRAY_NODE;
		  yyval.type_ptr = yyvsp[-4].type_ptr;
		}
    break;

  case 100:
#line 732 "gram.y"
    {
		  yyval.type_ptr = new_type_node(POINTER_NODE, last_type, yyvsp[0].type_ptr);
		  last_type = POINTER_NODE;
		}
    break;

  case 103:
#line 745 "gram.y"
    {
		  yyval.type_ptr = last_array_node;
		}
    break;

  case 104:
#line 749 "gram.y"
    {
		  reinterpret_cast<array_node *>(yyvsp[-1].type_ptr)->component_type
		    = ARRAY_NODE;
		  reinterpret_cast<array_node *>(yyvsp[-1].type_ptr)->component_type_ptr
		    = yyvsp[0].type_ptr;
		  yyval.type_ptr = yyvsp[-3].type_ptr;
		}
    break;

  case 105:
#line 760 "gram.y"
    {
		  last_array_node
		    = reinterpret_cast<array_node*>(new_type_node(ARRAY_NODE, yyvsp[-2].value, yyvsp[0].value));
		  yyval.type_ptr = reinterpret_cast<array_node*>(last_array_node);
		}
    break;

  case 106:
#line 766 "gram.y"
    {
		  long lbound;
		  long ubound;

		  if (yyvsp[0].sym->s_kind != TYPE_IDENTIFIER)
		    c4p_error ("`%s' is not a type identifier", yyvsp[0].sym->s_repr);
		  if (yyvsp[0].sym->s_type == CHARACTER_TYPE)
		    {
		      if (chars_are_unsigned)
			{
			  lbound = 0;
			  ubound = 255;
			}
		      else
			{
			  lbound = -128;
			  ubound = 127;
			}
		    }
		  else
		    if (yyvsp[0].sym->s_type == BOOLEAN_TYPE)
		      {
			lbound = 0;
			ubound = 1;
		      }
		    else
		      if (yyvsp[0].sym->s_type != SUBRANGE_NODE)
			c4p_error ("`%s' is not a subrange type identifier",
				   yyvsp[0].sym->s_repr);
		      else
			{
			  lbound
			    = (reinterpret_cast<subrange_node*>(yyvsp[0].sym->s_type_ptr)
			       ->lower_bound);
			  ubound
			    = (reinterpret_cast<subrange_node*>(yyvsp[0].sym->s_type_ptr)
			       ->upper_bound);
			}

		  last_array_node
		    = (reinterpret_cast<array_node*>
		       (new_type_node(ARRAY_NODE, lbound, ubound)));

		  yyval.type_ptr = last_array_node;
		}
    break;

  case 107:
#line 815 "gram.y"
    {
		  last_type = RECORD_NODE;
		  yyval.type_ptr = new_type_node(RECORD_NODE, yyvsp[-1].type_ptr);
		}
    break;

  case 108:
#line 823 "gram.y"
    {
		  yyval.type_ptr = new_type_node(FIELD_LIST_NODE, yyvsp[-1].type_ptr, yyvsp[0].type_ptr);
		}
    break;

  case 111:
#line 835 "gram.y"
    {
		  yyval.type_ptr = last_record_section;
		}
    break;

  case 112:
#line 839 "gram.y"
    {
		  reinterpret_cast<record_section_node *>(yyvsp[-1].type_ptr)->next
		    = reinterpret_cast<record_section_node*>(yyvsp[0].type_ptr);
		  yyval.type_ptr = yyvsp[-3].type_ptr;
		}
    break;

  case 113:
#line 848 "gram.y"
    {
		  yyval.type_ptr = 0;
		}
    break;

  case 114:
#line 852 "gram.y"
    {
		  yyval.type_ptr = new_type_node(RECORD_SECTION_NODE, yyvsp[-2].type_ptr, last_type, yyvsp[0].type_ptr);
		  last_record_section = reinterpret_cast<record_section_node*>(yyval.type_ptr);
		}
    break;

  case 116:
#line 861 "gram.y"
    {
		  yyval.type_ptr = last_declarator;
		}
    break;

  case 117:
#line 865 "gram.y"
    {
		  reinterpret_cast<declarator_node *>(yyvsp[-1].type_ptr)->next
		    = reinterpret_cast<declarator_node*>(yyvsp[0].type_ptr);
		  yyval.type_ptr = yyvsp[-3].type_ptr;
		}
    break;

  case 118:
#line 874 "gram.y"
    {
		  yyval.type_ptr = new_type_node(DECLARATOR_NODE, yyvsp[0].sym);
		  last_declarator = reinterpret_cast<declarator_node*>(yyval.type_ptr);
		}
    break;

  case 119:
#line 882 "gram.y"
    {
		  yyval.type_ptr = 0;
		}
    break;

  case 120:
#line 886 "gram.y"
    {
		  yyval.type_ptr = new_type_node(VARIANT_NODE, yyvsp[0].type_ptr);
		}
    break;

  case 122:
#line 900 "gram.y"
    {
		}
    break;

  case 124:
#line 907 "gram.y"
    {
		  yyval.type_ptr = last_variant;
		}
    break;

  case 125:
#line 911 "gram.y"
    {
		  reinterpret_cast<variant_field_list_node *>(yyvsp[-1].type_ptr)->next
		    = reinterpret_cast<variant_field_list_node*>(yyvsp[0].type_ptr);
		  yyval.type_ptr = yyvsp[-3].type_ptr;
		}
    break;

  case 126:
#line 920 "gram.y"
    {
		  yyval.type_ptr = last_variant = 0;
		}
    break;

  case 127:
#line 924 "gram.y"
    {
		  out_stop ();
		}
    break;

  case 128:
#line 928 "gram.y"
    {
		  out_resume ();
		}
    break;

  case 129:
#line 932 "gram.y"
    {
		  yyval.type_ptr = yyvsp[-1].type_ptr;
		  last_variant = reinterpret_cast<variant_node*>(yyval.type_ptr);
		}
    break;

  case 130:
#line 940 "gram.y"
    {
		  yyval.type_ptr = new_type_node(VARIANT_FIELD_LIST_NODE, yyvsp[-1].type_ptr, yyvsp[0].type_ptr);
		}
    break;

  case 135:
#line 957 "gram.y"
    {
		  if (block_level == 0)
		    {
		      if (class_name.length() == 0 && var_struct_name.length() == 0)
			out_s ("C4PEXTERN ");
		      if (nglobalvars == 0 && var_struct_name.length() > 0)
			{
			  out_form ("struct %s", var_struct_class_name.c_str());
			  if (var_struct_base_class_name.length() > 0)
			    out_form (" : public %s",
				      var_struct_base_class_name.c_str());
			  out_s (" {\n");
			}
		      nglobalvars += 1;
		    }
	          declare_var_list (reinterpret_cast<declarator_node*>(yyvsp[-3].type_ptr),
				    VARIABLE_IDENTIFIER, block_level,
				    last_type, yyvsp[-1].type_ptr);
		  out_s (";\n");
		}
    break;

  case 137:
#line 982 "gram.y"
    {
		  yyval.type_ptr = last_declarator;
		}
    break;

  case 138:
#line 986 "gram.y"
    {
		  reinterpret_cast<declarator_node *>(yyvsp[-1].type_ptr)->next
		    = reinterpret_cast<declarator_node*>(yyvsp[0].type_ptr);
		  yyval.type_ptr = yyvsp[-3].type_ptr;
		}
    break;

  case 139:
#line 995 "gram.y"
    {
		  yyval.type_ptr = new_type_node(DECLARATOR_NODE, yyvsp[0].sym);
		  last_declarator = reinterpret_cast<declarator_node*>(yyval.type_ptr);
		}
    break;

  case 141:
#line 1007 "gram.y"
    {
		  out_s ("{\n");
		  curly_brace_level += 1;
		  if (routine_handle == 0 && curly_brace_level == 1)
		    {
		      if (n_fast_vars)
			{
			  out_s ("C4P_FAST_VARS_0\n");
			}
		      out_form ("C4P_BEGIN_PROGRAM(\"%s\", argc, argv);\n",
				prog_symbol->s_repr);
		    }
		}
    break;

  case 142:
#line 1021 "gram.y"
    {
		  if (routine_handle == 0 && curly_brace_level == 1)
		    {
		      out_s ("C4P_END_PROGRAM();\n");
		    }
		  curly_brace_level -= 1;
		  out_s ("}\n");
		}
    break;

  case 145:
#line 1038 "gram.y"
    {
		  buf_mark2 = NO_MARK;
		}
    break;

  case 150:
#line 1052 "gram.y"
    {
		  out_form ("\nc4p_L%ld:\n", yyvsp[-1].value);
		  if (auto_exit_label >= 0 && yyvsp[-1].value == auto_exit_label)
		    auto_exit_label_flag = false;
		}
    break;

  case 151:
#line 1061 "gram.y"
    {
		  out_s(";\n");
		}
    break;

  case 155:
#line 1071 "gram.y"
    {
		  to_eoln = false;
		}
    break;

  case 157:
#line 1076 "gram.y"
    {
		  to_eoln = true;
		}
    break;

  case 159:
#line 1081 "gram.y"
    {
		  to_eoln = false;
		}
    break;

  case 161:
#line 1086 "gram.y"
    {
		  to_eoln = true;
		}
    break;

  case 163:
#line 1091 "gram.y"
    {
		  if (yyvsp[0].sym->s_kind != PROCEDURE_IDENTIFIER)
		    c4p_warning ("`%s' is not a procedure identifier", yyvsp[0].sym->s_repr);
		  out_form ("%s (", yyvsp[0].sym->s_repr);
		  push_parameter_node (last_parameter);
		  if (yyvsp[0].sym->s_kind == PROCEDURE_IDENTIFIER)
		    {
		      last_parameter
			= (reinterpret_cast<prototype_node*>(yyvsp[0].sym->s_type_ptr)
			   ->formal_parameter);
		    }
		  else
		    last_parameter = 0;
		}
    break;

  case 164:
#line 1106 "gram.y"
    {
		  pop_parameter_node (last_parameter);
		  out_s (");\n");
		}
    break;

  case 165:
#line 1114 "gram.y"
    {
		  out_s ("c4p_readln ();\n");
		}
    break;

  case 167:
#line 1122 "gram.y"
    {
	  	  if (to_eoln)
		    out_s ("C4P_READLN_BEGIN () ");
		  else
		    out_s ("C4P_READ_BEGIN () ");
		  out_s ("c4p_read_");
		  buf_mark = get_buf_mark();
		  out_s ("_ (");
		  file_specified = false;
		  file_spec = "input";
		}
    break;

  case 168:
#line 1134 "gram.y"
    {
	  	  if (to_eoln)
		    out_form (", %s); C4P_READLN_END (%s)\n",
			      file_spec.c_str(), file_spec.c_str());
		  else
		    out_form (", %s); C4P_READ_END (%s)\n",
			      file_spec.c_str(), file_spec.c_str());
		}
    break;

  case 170:
#line 1147 "gram.y"
    {
		  out_form (", %s); c4p_read_", file_spec.c_str());
		  buf_mark = get_buf_mark();
		  out_s ("_ (");
	        }
    break;

  case 172:
#line 1157 "gram.y"
    {
		  if (file_specified && file_type != CHARACTER_TYPE)
		    out_buf_over (buf_mark, "v", 1);
		  else
		    switch (last_type)
		      {
		      case FILE_NODE:
			out_buf_over (buf_mark, "f", 1);
      		        file_specified = true;
			get_buf_text (file_spec, buf_mark + 3, 0);
      		        file_type = reinterpret_cast<file_node *>(last_type_ptr)->type;
			break;
		      case REAL_TYPE:
		      case LONG_REAL_TYPE:
      		        out_buf_over (buf_mark, "r", 1); break;
		      case INTEGER_TYPE:
      		        out_buf_over (buf_mark, "i", 1); break;
		      case SUBRANGE_NODE:
      		        out_buf_over (buf_mark, "i", 1); break;
		      case CHARACTER_TYPE:
      		        out_buf_over (buf_mark, "c", 1); break;
		      case STRING_TYPE:
      		        out_buf_over (buf_mark, "s", 1); break;
		      default:
	                c4p_error ("internal error: unknown read type");
		      }
		}
    break;

  case 173:
#line 1188 "gram.y"
    {
		  out_s ("c4p_writeln ();\n");
		}
    break;

  case 175:
#line 1196 "gram.y"
    {
	  	  if (to_eoln)
		    out_s ("C4P_WRITELN_BEGIN () ");
		  else
		    out_s ("C4P_WRITE_BEGIN () ");
		  out_s ("c4p_write_");
		  buf_mark = get_buf_mark();
		  out_s ("__ (");
		  file_specified = false;
		  file_spec = "output";
		}
    break;

  case 176:
#line 1208 "gram.y"
    {
	  	  if (to_eoln)
		    out_form (", %s); C4P_WRITELN_END (%s)\n",
			      file_spec.c_str(), file_spec.c_str());
		  else
		    out_form (", %s); C4P_WRITE_END (%s)\n",
			      file_spec.c_str(), file_spec.c_str());
		}
    break;

  case 178:
#line 1221 "gram.y"
    {
		  out_form (", %s); c4p_write_", file_spec.c_str());
		  buf_mark = get_buf_mark();
		  out_s ("__ (");
	        }
    break;

  case 180:
#line 1231 "gram.y"
    {
		  if (file_specified && file_type != CHARACTER_TYPE)
		    out_buf_over (buf_mark, "v ", 2);
		  else
		    switch (last_type)
		      {
		      case FILE_NODE:
			out_buf_over (buf_mark, "f ", 2);
      		        file_specified = true;
			get_buf_text (file_spec, buf_mark + 4, 0);
      		        file_type = reinterpret_cast<file_node*>(last_type_ptr)->type;
			break;
		      case REAL_TYPE:
		      case LONG_REAL_TYPE:
      		        out_buf_over (buf_mark, "r ", 2); break;
		      case INTEGER_TYPE:
      		        out_buf_over (buf_mark, "i ", 2); break;
		      case SUBRANGE_NODE:
      		        out_buf_over (buf_mark, "i ", 2); break;
		      case CHARACTER_TYPE:
      		        out_buf_over (buf_mark, "c ", 2); break;
		      case ARRAY_NODE:
		      case STRING_TYPE:
      		        out_buf_over (buf_mark, "s ", 2); break;
		      default:
	                c4p_error ("internal error: unknown write type: %u", last_type);
		      }
		}
    break;

  case 181:
#line 1260 "gram.y"
    {
		  switch (last_type)
		    {
		    case REAL_TYPE:
		    case LONG_REAL_TYPE:
		      out_buf_over (buf_mark, "r1", 2); break;
		    case INTEGER_TYPE:
		      out_buf_over (buf_mark, "i1", 2); break;
		    case SUBRANGE_NODE:
		      out_buf_over (buf_mark, "i1", 2); break;
		    case CHARACTER_TYPE:
		      out_buf_over (buf_mark, "c1", 2); break;
		    case STRING_TYPE:
		      out_buf_over (buf_mark, "s1", 2); break;
		    default:
		      c4p_error ("internal error: unkown write type: %u", last_type);
		    }
		  out_s (", ");
		}
    break;

  case 183:
#line 1284 "gram.y"
    {
		}
    break;

  case 184:
#line 1287 "gram.y"
    {
		  out_s (", ");
		}
    break;

  case 185:
#line 1291 "gram.y"
    {
      		  out_buf_over (buf_mark + 1, "2", 1); break;
      		}
    break;

  case 191:
#line 1313 "gram.y"
    {
		  out_s ("if (");
		}
    break;

  case 192:
#line 1317 "gram.y"
    {
		  out_s (")\n");
		  extra_indent += 1;
		}
    break;

  case 194:
#line 1326 "gram.y"
    {
		  extra_indent -= 1;
		}
    break;

  case 195:
#line 1330 "gram.y"
    {
		  extra_indent -= 1;
		  out_s ("else\n");
		  extra_indent += 1;
		}
    break;

  case 196:
#line 1336 "gram.y"
    {
		  extra_indent -= 1;
		}
    break;

  case 197:
#line 1343 "gram.y"
    {
		  out_s ("switch (");
		}
    break;

  case 198:
#line 1347 "gram.y"
    {
		  out_s (") {\n");
		  curly_brace_level += 1;
		}
    break;

  case 199:
#line 1352 "gram.y"
    {
		  curly_brace_level -= 1;
		  out_s ("}\n");
		}
    break;

  case 202:
#line 1365 "gram.y"
    {
		}
    break;

  case 205:
#line 1376 "gram.y"
    {
		  extra_indent += 1;
		}
    break;

  case 206:
#line 1380 "gram.y"
    {
		  out_s ("break;\n");
		  extra_indent -= 1;
		}
    break;

  case 209:
#line 1392 "gram.y"
    {
		  out_s ("case ");
		}
    break;

  case 210:
#line 1396 "gram.y"
    {
		  out_s (":\n");
		}
    break;

  case 211:
#line 1401 "gram.y"
    {
		  out_s ("default:\n");
		}
    break;

  case 215:
#line 1414 "gram.y"
    {
		  out_s ("while (");
		}
    break;

  case 216:
#line 1418 "gram.y"
    {
		  out_s (")\n");
		  extra_indent += 1;
		}
    break;

  case 217:
#line 1423 "gram.y"
    {
		  extra_indent -= 1;
		}
    break;

  case 218:
#line 1430 "gram.y"
    {
		  out_s ("do {\n");
		  curly_brace_level += 1;
		}
    break;

  case 219:
#line 1435 "gram.y"
    {
		  curly_brace_level -= 1;
		  out_s ("} while (! (");
		}
    break;

  case 220:
#line 1440 "gram.y"
    {
		  out_s ("));\n");
		}
    break;

  case 221:
#line 1448 "gram.y"
    {
		  out_s ("C4P_FOR_BEGIN (");
		  if (yyvsp[-1].sym->s_block_level == 0
		      && yyvsp[-1].sym->s_kind == VARIABLE_IDENTIFIER
		      && ! (yyvsp[-1].sym->s_flags & S_PREDEFINED))
		    {
		      if (var_struct_name.length() > 0)
			out_form ("%s.", var_struct_name.c_str());
		      out_s (var_name_prefix.c_str());
		    }
		  out_form ("%s, ", yyvsp[-1].sym->s_repr);
		}
    break;

  case 222:
#line 1461 "gram.y"
    {
		  out_form (", %s, ", yyvsp[0].value > 0 ? "<=" : ">=");
		}
    break;

  case 223:
#line 1465 "gram.y"
    {
		  out_s (")\n");
		  extra_indent += 1;
		}
    break;

  case 224:
#line 1470 "gram.y"
    {
		  extra_indent -= 1;
		  out_s ("C4P_FOR_END (");
		  if (yyvsp[-9].sym->s_block_level == 0
		      && yyvsp[-9].sym->s_kind == VARIABLE_IDENTIFIER
		      && ! (yyvsp[-9].sym->s_flags & S_PREDEFINED))
		    {
		      if (var_struct_name.length() > 0)
			out_form ("%s.", var_struct_name.c_str());
		      out_s (var_name_prefix.c_str());
		    }
		  out_form ("%s, %s)\n", yyvsp[-9].sym->s_repr, (yyvsp[-5].value > 0 ? "++" : "--"));
		}
    break;

  case 225:
#line 1487 "gram.y"
    {
		}
    break;

  case 226:
#line 1493 "gram.y"
    {
		}
    break;

  case 227:
#line 1499 "gram.y"
    {
		  yyval.value = 1;
		}
    break;

  case 228:
#line 1503 "gram.y"
    {
		  yyval.value = -1;
		}
    break;

  case 229:
#line 1510 "gram.y"
    {
		  if (last_type == FUNCTION_TYPE)
		    out_s ("c4p_result = ");
		  else
		    out_s (" = ");
		}
    break;

  case 230:
#line 1517 "gram.y"
    {
		  if (buf_mark2 != NO_MARK)
		    out_buf_over (buf_mark2, "w", 1);
		}
    break;

  case 231:
#line 1522 "gram.y"
    {
		  out_s (";\n");
		}
    break;

  case 232:
#line 1529 "gram.y"
    {
		  out_form ("goto c4p_L%ld;\n", yyvsp[0].value);
		}
    break;

  case 237:
#line 1549 "gram.y"
    {
		  if (last_type != FILE_NODE)
		    c4p_error ("left side of `^' must be a file variable");
		  out_s (".bufref()");
		  last_type = reinterpret_cast<file_node*>(last_type_ptr)->type;
		  last_type_ptr = reinterpret_cast<file_node*>(last_type_ptr)->type_ptr;
		}
    break;

  case 239:
#line 1564 "gram.y"
    {
		  switch (yyvsp[0].sym->s_kind)
		    {
		    case VARIABLE_IDENTIFIER:
		    case PARAMETER_IDENTIFIER:
		    case CONSTANT_IDENTIFIER:
		      last_type = yyvsp[0].sym->s_type;
		      last_type_ptr = yyvsp[0].sym ->s_type_ptr;
		      if (yyvsp[0].sym->s_flags & S_FAST)
			{
			  out_form ("c4p_fast_%s_%u", yyvsp[0].sym->s_repr, routine_handle);
			  remember_fast_var (yyvsp[0].sym->s_repr);
			}
		      else
			{
			  if (yyvsp[0].sym->s_block_level == 0
			      && yyvsp[0].sym->s_kind == VARIABLE_IDENTIFIER
			      && ! (yyvsp[0].sym->s_flags & S_PREDEFINED))
			    {
			      if (var_struct_name.length() > 0)
				out_form ("%s.", var_struct_name.c_str());
			      out_s (var_name_prefix.c_str());
			    }
			  out_s (yyvsp[0].sym->s_repr);
			}
		      break;
		    case FUNCTION_IDENTIFIER:
		      last_type = FUNCTION_TYPE;
		      last_type_ptr = yyvsp[0].sym->s_type_ptr;
		      break;
		    default:
		      c4p_error ("`%s' isn't defined", yyvsp[0].sym->s_repr);
		      break;
		    }
		}
    break;

  case 242:
#line 1608 "gram.y"
    {
		  if (last_type != ARRAY_NODE && last_type != POINTER_NODE)
		    c4p_error ("left side of `[' must be an array variable");
		  out_s ("[ ");
		  yyval.value = 0;
		  if (last_type == POINTER_NODE)
		    {
		      push_array_type (reinterpret_cast<pointer_node*>(last_type_ptr)
				       ->array_node_ptr);
		    }
		  else
		    {
		      push_array_type (reinterpret_cast<array_node*>(last_type_ptr));
		    }
		}
    break;

  case 243:
#line 1624 "gram.y"
    {
		  last_type = last_array_node->component_type;
		  last_type_ptr = last_array_node->component_type_ptr;
		  pop_array_type ();
		  out_s (" ]");
		}
    break;

  case 245:
#line 1638 "gram.y"
    {
		  last_type
		    = translate_field_name(yyvsp[0].sym->s_repr,
					   reinterpret_cast<record_node*>(last_type_ptr),
					   &last_type_ptr);
		}
    break;

  case 247:
#line 1652 "gram.y"
    {
                  if (last_array_node->lower_bound != 0)
		    out_form (" - (%ld)", get_array_offset(last_array_node));
		  last_type = last_array_node->component_type;
		  last_type_ptr = last_array_node->component_type_ptr;
		}
    break;

  case 248:
#line 1659 "gram.y"
    {
		  out_s (" ][ ");
		}
    break;

  case 249:
#line 1663 "gram.y"
    {
		  last_array_node
		    = reinterpret_cast<array_node*>(last_array_node->component_type_ptr);
		  out_form (" - (%ld)",	get_array_offset(last_array_node));
		  last_type = last_array_node->component_type;
		  last_type_ptr = last_array_node->component_type_ptr;
		}
    break;

  case 251:
#line 1675 "gram.y"
    {
		  yyval.buf_mark = get_buf_mark();
		  out_s ("(C4P_integer)  ");
		}
    break;

  case 252:
#line 1680 "gram.y"
    {
		  yyval.type = coerce(yyvsp[-3].type, yyvsp[0].type);
		  if (yyval.type == REAL_TYPE)
		    out_buf_over (yyvsp[-1].buf_mark, "(C4P_real)     ", 15);
		  if (yyval.type == LONG_REAL_TYPE)
		    out_buf_over (yyvsp[-1].buf_mark, "(C4P_longreal) ", 15);
		}
    break;

  case 254:
#line 1695 "gram.y"
    {
		  out_s (" < ");
		}
    break;

  case 255:
#line 1699 "gram.y"
    {
		  out_s (" > ");
		}
    break;

  case 256:
#line 1703 "gram.y"
    {
		  out_s (" <= ");
		}
    break;

  case 257:
#line 1707 "gram.y"
    {
		  out_s (" >= ");
		}
    break;

  case 258:
#line 1711 "gram.y"
    {
		  out_s (" == ");
		}
    break;

  case 259:
#line 1715 "gram.y"
    {
		  out_s (" != ");
		}
    break;

  case 261:
#line 1723 "gram.y"
    {
		  out_s (" - (C4P_integer) ");
		}
    break;

  case 262:
#line 1727 "gram.y"
    {
		  yyval.type = yyvsp[0].type;
		}
    break;

  case 263:
#line 1731 "gram.y"
    {
		  yyval.type = yyvsp[0].type;
		}
    break;

  case 264:
#line 1735 "gram.y"
    {
		  yyval.buf_mark = get_buf_mark();
		  out_s ("(C4P_integer)  ");
		}
    break;

  case 265:
#line 1740 "gram.y"
    {
		  yyval.type = coerce(yyvsp[-3].type, yyvsp[0].type);
		  if (yyval.type == REAL_TYPE)
		    out_buf_over (yyvsp[-1].buf_mark, "(C4P_real)     ", 15);
		  if (yyval.type == LONG_REAL_TYPE)
		    out_buf_over (yyvsp[-1].buf_mark, "(C4P_longreal) ", 15);
		}
    break;

  case 266:
#line 1748 "gram.y"
    {
		  out_s (" || ");
		}
    break;

  case 268:
#line 1756 "gram.y"
    {
		  out_s (" - ");
		}
    break;

  case 269:
#line 1760 "gram.y"
    {
		  out_s (" + ");
		}
    break;

  case 271:
#line 1768 "gram.y"
    {
		  out_s (" / (C4P_real) ");
		}
    break;

  case 273:
#line 1773 "gram.y"
    {
		  yyval.buf_mark = get_buf_mark();
		  out_s ("(C4P_integer)  ");
		}
    break;

  case 274:
#line 1778 "gram.y"
    {
		  yyval.type = coerce(yyvsp[-3].type,yyvsp[0].type);
		  if (yyval.type == REAL_TYPE)
		    out_buf_over (yyvsp[-1].buf_mark, "(C4P_real)     ", 15);
		  if (yyval.type == LONG_REAL_TYPE)
		    out_buf_over (yyvsp[-1].buf_mark, "(C4P_longreal) ", 15);
		}
    break;

  case 275:
#line 1786 "gram.y"
    {
		  out_s (" && ");
		}
    break;

  case 277:
#line 1794 "gram.y"
    {
		  out_s (" * ");
		}
    break;

  case 278:
#line 1798 "gram.y"
    {
		  out_s (" / ");
		}
    break;

  case 279:
#line 1802 "gram.y"
    {
		  out_s (" % ");
		}
    break;

  case 280:
#line 1809 "gram.y"
    {
		  yyval.type = last_type = INTEGER_TYPE;
		  out_form ("%ldl", yyvsp[0].value);
		}
    break;

  case 281:
#line 1814 "gram.y"
    {
		  yyval.type = last_type = REAL_TYPE;
		  out_form ("%lf", static_cast<double>(yyvsp[0].fvalue));
		}
    break;

  case 282:
#line 1819 "gram.y"
    {
		  yyval.type = last_type = LONG_REAL_TYPE;
		  out_form ("%lf", static_cast<double>(yyvsp[0].fvalue));
		}
    break;

  case 283:
#line 1824 "gram.y"
    {
		  yyval.type = last_type = STRING_TYPE;
		  out_s ("\"");
		  translate_string (get_string ((STRING_PTR) yyvsp[0].value));
		  out_s ("\"");
		}
    break;

  case 284:
#line 1831 "gram.y"
    {
		  yyval.type = last_type = CHARACTER_TYPE;
		  out_s ("\'");
		  translate_char (static_cast<int>(yyvsp[0].value));
		  out_s ("\'");
		}
    break;

  case 285:
#line 1838 "gram.y"
    {
		  if (last_type == FUNCTION_TYPE)
		    {
		      prototype_node * proto
			= reinterpret_cast<prototype_node*>(last_type_ptr);
		      if (strcmp (proto->name->s_repr, "eoln") == 0)
			out_s ("eoln(input)");
		      else if (strcmp (proto->name->s_repr, "eof") == 0)
		        out_s ("eof(input)");
		      out_form ("%s ()", proto->name->s_repr);
		      last_type = proto->result_type->s_type;
		      last_type_ptr = proto->result_type->s_type_ptr;
		    }
		  yyval.type = last_type;
		}
    break;

  case 286:
#line 1854 "gram.y"
    {
		  if (yyvsp[-1].sym->s_kind != FUNCTION_IDENTIFIER)
		    c4p_warning ("`%s' is not a function identifier", yyvsp[-1].sym->s_repr);
		  else if (yyvsp[-1].sym->s_type != PROTOTYPE_NODE)
		    c4p_error ("internal error: `%1' has no type", yyvsp[-1].sym->s_repr);
		  out_form ("%s (", yyvsp[-1].sym->s_repr);
		  push_parameter_node(last_parameter);
		  if (yyvsp[-1].sym->s_kind == FUNCTION_IDENTIFIER)
		    {
		      last_parameter
			= (reinterpret_cast<prototype_node *>(yyvsp[-1].sym->s_type_ptr)
			   ->formal_parameter);
		    }
		  else
		    last_parameter = 0;
		}
    break;

  case 287:
#line 1871 "gram.y"
    {
		  prototype_node * proto
		    = reinterpret_cast<prototype_node*>(yyvsp[-4].sym->s_type_ptr);
		  out_s (")");
		  pop_parameter_node(last_parameter);
		  if (yyvsp[-4].sym->s_kind == FUNCTION_IDENTIFIER)
		    {
		      yyval.type = last_type = proto->result_type->s_type;
		      last_type_ptr = proto->result_type->s_type_ptr;
		    }
		  else
		    {
		      yyval.type = last_type = INTEGER_TYPE;
		      last_type_ptr = 0;
		    }
		}
    break;

  case 288:
#line 1891 "gram.y"
    {
		  out_s (" (");
		}
    break;

  case 289:
#line 1895 "gram.y"
    {
		  out_s (") ");
		  yyval.type = yyvsp[-1].type;
		}
    break;

  case 290:
#line 1900 "gram.y"
    {
		  out_s ("! ");
		}
    break;

  case 291:
#line 1904 "gram.y"
    {
		  yyval.type = yyvsp[0].type;
		}
    break;

  case 295:
#line 1917 "gram.y"
    {
		  out_s (", ");
		}
    break;

  case 297:
#line 1925 "gram.y"
    {
		  if (last_parameter != 0)
		     last_parameter = last_parameter->next;
		}
    break;


    }

/* Line 999 of yacc.c.  */
#line 3514 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}



