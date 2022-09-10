% vim: syntax=tex
%------------------------------------------------------------------------------
% scrartcl-based class for homeworks on C++
%------------------------------------------------------------------------------
\NeedsTeXFormat{LaTeX2e}
\ProvidesPackage{cpp-hmwk}[2020/07/10 Class for homeworks on C++]

\PassOptionsToPackage{10pt,fleqn}{scrartcl}

% geometry for ISO A4, following suggestions by Bringhurst
\PassOptionsToPackage{a4paper}{geometry}
\PassOptionsToClass{a4paper}{scrartcl}
\PassOptionsToPackage{
   hdivide={0.111111\paperwidth,*,0.222222\paperwidth},
   vdivide={0.111111\paperwidth,*,0.222222\paperwidth},
}{geometry}

% language selection
\DeclareOption{german}{%
   \AtEndOfClass{%
      \setmainlanguage{german}
   }
}
\DeclareOption{english}{%
   \AtEndOfClass{%
      \setmainlanguage{english}
   }
}

% pass all other options to the scrartcl class
\DeclareOption*{%
   \PassOptionsToClass{\CurrentOption}{scrartcl}%
}
% any options for this class must be processed before the \ProcesOptions:
\ProcessOptions\relax

% now we load article and all the packages that we need
\LoadClass{scrartcl}

\RequirePackage{geometry}

% language
\RequirePackage{polyglossia}

% math stuff
\RequirePackage{amsfonts}
\RequirePackage{amssymb}
\RequirePackage{amsmath}
\RequirePackage{unicode-math}% must be loaded after amsmath

% fonts
\RequirePackage{fontspec}
\setmainfont[Ligatures=TeX]{TeX Gyre Pagella}
\setmathfont{TeX Gyre Pagella Math}
\setmonofont{Source Code Pro}[Scale=MatchLowercase]
\RequirePackage{sectsty}
\allsectionsfont{\normalfont\sffamily\bfseries}

% graphics
\RequirePackage{graphicx}
\DeclareGraphicsExtensions{.pdf,.png,.jpg,.mps}

% colors
\RequirePackage{framed}
\RequirePackage{xcolor}
\colorlet{shadecolor}{gray!25}

% listings
\RequirePackage{listings}
\lstloadlanguages{C,C++}
\lstset{language=[11]C++}
\lstset{morekeywords={concept}}
\lstset{breaklines=true,numbersep=10pt,stepnumber=1,captionpos=t}
\lstset{basewidth={0.60em,0.48em},columns=[l]fullflexible,flexiblecolumns=true}
\lstset{frame=tb,numbers=left,numberstyle=\tiny,escapechar=`,extendedchars=true}
\lstset{basicstyle={},%
   keywordstyle=\bfseries,
   identifierstyle=\itshape,
   commentstyle=\color{gray}\itshape,
   stringstyle=}
% http://tex.stackexchange.com/questions/181307/use-textit-in-listing
\lstset{escapeinside={(*@}{@*)}}

% macros that should conform to the listings configuration
\newcommand{\ident}[1]{\textit{#1}}
\newcommand{\qualident}[2]{\textit{#1}::\textit{#2}}
\newcommand{\std}[1]{\qualident{std}{#1}}
\newcommand{\reftype}[1]{#1\ampersandSY}
\newcommand{\header}[1]{\nonterminal{#1}}

% tabularx
\RequirePackage{tabularx}
% taken from LaTeX Companion, 2nd ed, p. 249, 251
\newcolumntype{Y}{>{\raggedright\arraybackslash}X}

% support of MetaPost
\RequirePackage{luamplib}
\preto\mplibcode{\center\leavevmode}
\appto\endmplibcode{\endcenter}

% biblatex
\RequirePackage[backend=biber,style=ieee]{biblatex}
\addbibresource{references.bib}

% misc stuff
\RequirePackage[autostyle]{csquotes}
\RequirePackage[pdfencoding=auto]{hyperref}
\RequirePackage{hhline}
\RequirePackage{fullpage}
\RequirePackage{tabularx}
\RequirePackage{algorithm2e}
\RequirePackage{varwidth}   %  \fbox{}
\RequirePackage{enumitem}
\RequirePackage{varwidth}
\RequirePackage{array, multirow}
\RequirePackage{calc}
\RequirePackage{caption}
\RequirePackage{epigraph}
\RequirePackage{xcolor}
\RequirePackage[group-separator={,}]{siunitx}
\RequirePackage{ifthen}

\RequirePackage[protrusion=true,expansion=false]{microtype}
\pdfcompresslevel=9
\frenchspacing

\DeclareCaptionType{equ}[][]
%------------------------------------------------------------------------------
% environment for grammars
%------------------------------------------------------------------------------
\newlength{\grammarwidth}%
\setlength{\grammarwidth}{\textwidth}%
\addtolength{\grammarwidth}{-2\fboxsep}%
\newenvironment{grammar}{%
   \begingroup
   \allowdisplaybreaks
   \smallskip%
   \noindent%
   \begin{eqnarray*}
}{%
   \end{eqnarray*}
   \smallskip%
   \endgroup
}
\newcommand{\produces}{& \longrightarrow &~}
\newcommand{\nonterminal}[1]{%
   \relax%
   \ifmmode\langle\mbox{#1}\rangle~%
   \else$\langle\mbox{#1}\rangle$%
   \fi%
}
\newcommand{\lextoken}[1]{%
   \relax%
   \ifmmode\mbox{\enquote{#1}}~%
   \else\enquote{#1}%
   \fi%
}
\newcommand{\keyword}[1]{%
   \relax%
   \ifmmode\mbox{\textbf{#1}}~%
   \else{\textbf{#1}}%
   \fi%
}
\newcommand{\optional}[1]{\left[~{#1}\right]~}
\newcommand{\multiple}[1]{\left\{~{#1}\right\}~}
\newcommand{\nextline}{\\ & &~}
\newcommand{\alternative}{\mid~}
%------------------------------------------------------------------------------
% typographical correct \n, \t ... (don't use $\backslash$ outside math)
% Application: \bs{n}, \bs{t}, ...
%------------------------------------------------------------------------------
\newcommand{\bs}[1]{{\char'134\kern0pt#1}}
\newcommand{\bsbs}{\char'134\char'134}
\newcommand{\tildeSY}{\char'176}
\newcommand{\barSY}{\char'174}
\newcommand{\caretSY}{\char'136}
\newcommand{\backslashSY}{\char'134}
\newcommand{\leftbracketSY}{\char'133}
\newcommand{\rightbracketSY}{\char'135}
\newcommand{\leftparenSY}{\char'050}
\newcommand{\rightparenSY}{\char'051}
\newcommand{\leftbraceSY}{\{}
\newcommand{\rightbraceSY}{\}}
\newcommand{\leftangleSY}{%
   \relax%
   \ifmmode<%
   \else$<$%
   \fi%
}
\newcommand{\rightangleSY}{%
   \relax%
   \ifmmode>%
   \else$>$%
   \fi%
}
\newcommand{\ampersandSY}{\char '046}
\newcommand{\dquoteSY}{\char '042}
\newcommand{\greaterSY}{\textgreater}
\newcommand{\lessSY}{\textless}
\newcommand{\shiftleftSY}{\lessSY\kern0pt\lessSY}
\newcommand{\shiftrightSY}{\greaterSY\kern0pt\greaterSY}
\newcommand{\minusSY}{%
   \relax%
   \ifmmode-%
   \else$-$%
   \fi%
}
\newcommand{\plusSY}{%
   \relax%
   \ifmmode+%
   \else$+$%
   \fi%
}
\newcommand{\asteriskSY}{\ast}
\newcommand{\slashSY}{\slash}
\newcommand{\percentSY}{\%}
%------------------------------------------------------------------------------
\newcommand{\Homework}[2]{%
   \begin{minipage}[t]{\textwidth}%
      \begin{center}%
	 {%
	    \usekomafont{title}\huge%
	    #1\par
	 }%

	 \bigskip
	 {%
	    \usekomafont{author}%
	    #2\par
	 }%

	 {%
	    \usekomafont{date}14. August 2020\par
	 }%
      \end{center}%
   \end{minipage}
   \vspace*{2cm}
}
