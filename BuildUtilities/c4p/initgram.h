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
/* Line 1252 of yacc.c.  */
#line 176 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



