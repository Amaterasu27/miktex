%%---------------------------------------------------
%%-- RC Thesis 
%%---------------------------------------------------
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{rcthesis}

%%---------------------------------------------------
%%-- Load the base class and modify current macros.
%%---------------------------------------------------

\newif\if@usecppsyntax \@usecppsyntaxfalse

\DeclareOption{cpp}{\@usecppsyntaxtrue}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{scrreprt}}
\ProcessOptions
%\PassOptionsToClass{12pt}{scrreprt}

\LoadClass{scrreprt}

%%--Set the section number depth.
\setcounter{secnumdepth}{3}
\setcounter{tocdepth}{4}

%\setcounter{page}{2}

%%--Include some required packages.

%%--Must change the margin above the Chapter heading to 2 inches
\RequirePackage{rcchapter}


%%--Create a vertical skip for the distance between the title and
%%--the first line of text on the prolog pages.
\newskip\thesisvskip
\thesisvskip 36pt

%%--Set the margins. %\headheight6pt \headsep12pt
\marginparwidth 1in
\marginparsep 6pt
\oddsidemargin 38pt %36pt
\evensidemargin 38pt %36pt
\topmargin  -1pt
\textwidth  6in
\textheight 8.5 in
\footskip   24pt  %%check this

%%--Change the marginpar.
\reversemarginpar
\let\old@marginpar=\marginpar
\def\marginpar#1{
	\old@marginpar{\def\baselinestretch{1}\em\small #1}}

%%--Parameters that can be set by the user.
\def\thesis@listtables{y}
\def\thesis@listfigures{y}

\newcommand{\notablelist}{\gdef\thesis@listtables{n}}
\newcommand{\nofigurelist}{\gdef\thesis@listfigures{n}}




%%--------------------------------------------
%%-- Bibliography related commands.
%%--------------------------------------------


%%--Set the bib style
\def\thesisBibStyle{rcbib}

%%--Define a single command for making the bibliography.
\newcommand{\makeThesisBib}[1]{
   \bibliographystyle{\thesisBibStyle}
   \bibliography{#1}}

%%--Redefine the thebibliography environment
\let\wm@thebibliography\thebibliography
\let\wm@endthebibliography\endthebibliography

\def\thebibliography#1{\wm@thebibliography{#1}
	\addcontentsline{toc}{chapter}{\numberline{}Bibliography}}
\def\endthebibliography{\wm@endthebibliography}

\newcommand{\makeThesisVita}[1]{
	  \newpage
	  \pagestyle{empty}
	  \begin{center}
	    \Large VITA
       \vskip\thesisvskip
       \thesis@authorx
       \vskip\thesisvskip
	  \end{center}
	  \input{#1}
   
}

\newcommand{\makeUMIAbstract}[1]{
	  \newpage
	  \pagestyle{empty}
	  \begin{center}
      \Large \thesis@title \\
	    ABSTRACT \vskip\thesisvskip
	 \end{center}
	  \input{#1}
      \vfill
      \begin{center}
      \large \thesis@authorx \\
      Department of Computer Science \\
      The College of William and Mary in Virginia \\
      Advisor: \ThesisAdvisor  \\
      \thesis@title
      \end{center}
}

%%--------------------------------------------
%%-- Definition and Quotation commands.
%%--------------------------------------------

\newtheorem{dbdefine}{Definition}[chapter]
\newtheorem{dbtheorem}{Theorem}[chapter]
\newtheorem{dblemma}{Lemma}[chapter]
\newtheorem{dbnotate}{Notation}[chapter]
\newtheorem{dbaxiom}{Axiom}[chapter]
\newenvironment{dbproof}[1][Proof]
{\noindent {\bf #1: }}{\hfill $\blacksquare$}

%%-----------------------------------------------
%%-- Float related environments and commands.
%%-----------------------------------------------

%%--Modify behavior of the figure and table environment.
\def\fps@figure{hbt}
\def\fnum@figure{{\bf \figurename~\thefigure}}
\def\fps@table{htb}
\def\fnum@table{{\bf \tablename~\thetable}}

\def\mathfigure{\thesisfiguretrue\figure}
\def\endmathfigure{\endfigure\thesisfigurefalse}



%%-----------------------------------------------
%%-- Modify the displayed math environments.
%%-----------------------------------------------

\newif\ifthesisfigure
\thesisfigurefalse



%%--------------------------------------------
%%-- Miscellaneous commands.
%%--------------------------------------------

%%--Commands to simplify font changing.
\newcommand{\sfup}{\sffamily\mdseries\upshape}
\newcommand{\sfsc}{\ttfamily\mdseries\scshape}
\newcommand{\sfit}{\sffamily\mdseries\itshape}
\newcommand{\rmup}{\rmfamily\mdseries\upshape}
\newcommand{\rmbf}{\rmfamily\bfseries\upshape}
\newcommand{\rmit}{\rmfamily\mdseries\itshape}
\newcommand{\ttup}{\ttfamily\mdseries\upshape}


\endinput

%%--The \tableofcontents.
\renewcommand\tableofcontents{%
   \renewcommand{\contentsname}{Table of Contents}
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \oldchapter*{\contentsname
        \@mkboth{\uppercase{\contentsname}}{\uppercase{\contentsname}}}%
    \@starttoc{toc}%
    \if@restonecol\twocolumn\fi}

%%--The \listoffigures command.
\renewcommand\listoffigures{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \oldchapter*{\listfigurename
      \@mkboth{\uppercase{\listfigurename}}{\uppercase{\listfigurename}}}%
    \@starttoc{lof}%
    \if@restonecol\twocolumn\fi}

%%--The \listoftables command.
\renewcommand\listoftables{%
    \if@twocolumn%
      \@restonecoltrue\onecolumn%
    \else%
      \@restonecolfalse%
    \fi%
    \oldchapter*{\listtablename%
      \@mkboth{\uppercase{\listtablename}}{\uppercase{\listtablename}}}%
    \@starttoc{lot}%
    \if@restonecol\twocolumn\fi}

\endinput
