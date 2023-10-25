/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   colors.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/10 17:59:01 by ohengelm      #+#    #+#                 */
/*   Updated: 2022/12/10 17:59:10 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_HPP
# define COLORS_HPP
/* ====================================||==================================== *\
||																			  ||
||							   Colors and Styles							  ||
||																			  ||
||																			  ||
||																			  ||
||		https://chrisyeh96.github.io/2020/03/28/terminal-colors.html		  ||
||				 https://www.google.com/search?q=color+chooser				  ||
||				  https://en.wikipedia.org/wiki/Tertiary_color				  ||
\* ===============colors===============||==============©Othello============== */

/* ====================================||==================================== *\
||																			  ||
||									 Reset									  ||
||																			  ||
\* ===============colors===============||==============©Othello============== */
# define C_RESET	"\x1b[0m"
/* ====================================||==================================== *\
||																			  ||
||									 Styles									  ||
||																			  ||
\* ===============colors===============||==============©Othello============== */
# define C_BOLD	"\x1b[1m"
# define C_WEAK	"\x1b[2m"
# define C_CURS	"\x1b[3m"
# define C_UNDL	"\x1b[4m"
# define C_BLNK	"\x1b[5m"
# define C_REV	"\x1b[7m"
# define C_HIDDEN	"\x1b[8m"
/* ====================================||==================================== *\
||																			  ||
||									 Colors									  ||
||																			  ||
|| ====================================||==================================== ||
||								   Graytones								  ||
\* ===============colors===============||==============©Othello============== */
# define C_WHITE	"\x1b[38;2;255;255;255m"
# define C_LGRAY	"\x1b[38;2;192;192;192m"
# define C_GRAY		"\x1b[38;2;128;128;128m"
# define C_DGRAY	"\x1b[38;2;64;64;64m"
# define C_BLACK	"\x1b[38;2;0;0;0m"
/* ====================================||==================================== *\
||									Rainbow									  ||
\* ===============colors===============||==============©Othello============== */
# define C_LRED		"\x1b[38;2;255;128;128m"
# define C_RED		"\x1b[38;2;255;0;0m"
# define C_DRED		"\x1b[38;2;128;0;0m"
# define C_LORANGE		"\x1b[38;2;255;192;128m"
# define C_ORANGE		"\x1b[38;2;255;128;0m"
# define C_DORANGE		"\x1b[38;2;128;64;0m"
# define C_LYELLOW	"\x1b[38;2;255;255;128m"
# define C_YELLOW	"\x1b[38;2;255;255;0m"
# define C_DYELLOW	"\x1b[38;2;128;128;0m"
# define C_LCHRT		"\x1b[38;2;192;255;128m"
# define C_CHRT			"\x1b[38;2;128;255;0m"	//chartreuse
# define C_DCHRT		"\x1b[38;2;64;128;0m"
# define C_LGREEN	"\x1b[38;2;128;255;128m"
# define C_GREEN	"\x1b[38;2;0;255;0m"
# define C_DGREEN	"\x1b[38;2;0;128;0m"
# define C_LSPRGR		"\x1b[38;2;128;255;192m"
# define C_SPRGR		"\x1b[38;2;0;255;128m"	//spring green
# define C_DSPRGR		"\x1b[38;2;0;128;64m"
# define C_LCYAN	"\x1b[38;2;128;255;255m"
# define C_CYAN		"\x1b[38;2;0;255;255m"
# define C_DCYAN	"\x1b[38;2;0;128;128m"
# define C_LAZURE		"\x1b[38;2;0;192;255m"
# define C_AZURE			"\x1b[38;2;0;128;255m"
# define C_DAZURE		"\x1b[38;2;0;64;128m"
# define C_LBLUE	"\x1b[38;2;128;128;255m"
# define C_BLUE		"\x1b[38;2;0;0;255m"
# define C_DBLUE	"\x1b[38;2;0;0;128m"
# define C_LVIOLET		"\x1b[38;2;192;0;255m"
# define C_VIOLET		"\x1b[38;2;128;0;255m"
# define C_DVIOLET		"\x1b[38;2;64;0;255m"
# define C_LMGNT	"\x1b[38;2;255;128;255m"
# define C_MGNT		"\x1b[38;2;255;0;255m"	//magenta
# define C_DMGNT	"\x1b[38;2;128;0;128m"
# define C_LROSE		"\x1b[38;2;255;128;192m"
# define C_ROSE			"\x1b[38;2;255;0;128m"
# define C_DROSE		"\x1b[38;2;128;0;64m"
/* ====================================||==================================== *\
||								 Common Colors								  ||
\* ===============colors===============||==============©Othello============== */
# define C_LBROWN	"\x1b[38;2;192;144;96m"
# define C_BROWN	"\x1b[38;2;128;64;0m"	//hue 30
# define C_DBROWN	"\x1b[38;2;64;32;0m"
# define C_LPURPLE	"\x1b[38;2;192;96;192m"
# define C_PURPLE	"\x1b[38;2;128;0;128m"	//hue 300
# define C_DPURPLE	"\x1b[38;2;64;0;64m"
# define C_LPINK		"\x1b[38;2;255;224;229m"
# define C_PINK			"\x1b[38;2;255;192;203m"	//hue 350
# define C_DPINK		"\x1b[38;2;128;48;62m"
/* ====================================||==================================== *\
||								  Prized Metal								  ||
\* ===============colors===============||==============©Othello============== */
# define C_BRONZE	"\x1b[38;2;205;127;50m"
# define C_SILVER	"\x1b[38;2;192;192;192m"
# define C_GOLD		"\x1b[38;2;255;215;0m"
/* ====================================||==================================== *\
||																			  ||
||								  Backgrounds								  ||
||																			  ||
|| ====================================||==================================== ||
||								   Graytones								  ||
\* ===============colors===============||==============©Othello============== */
# define CB_WHITE	"\x1b[48;2;255;255;255m"
# define CB_LGRAY	"\x1b[48;2;192;192;192m"
# define CB_GRAY	"\x1b[48;2;128;128;128m"
# define CB_DGRAY	"\x1b[48;2;64;64;64m"
# define CB_BLACK	"\x1b[48;2;0;0;0m"
/* ====================================||==================================== *\
||									Rainbow									  ||
\* ===============colors===============||==============©Othello============== */
# define CB_LRED	"\x1b[48;2;255;128;128m"
# define CB_RED		"\x1b[48;2;255;0;0m"
# define CB_DRED	"\x1b[48;2;128;0;0m"
# define CB_LORANGE		"\x1b[48;2;255;192;128m"
# define CB_ORANGE		"\x1b[48;2;255;128;0m"
# define CB_DORANGE		"\x1b[48;2;128;64;0m"
# define CB_LYELLOW	"\x1b[48;2;255;255;128m"
# define CB_YELLOW	"\x1b[48;2;255;255;0m"
# define CB_DYELLOW	"\x1b[48;2;128;128;0m"
# define CB_LCHRT		"\x1b[48;2;192;255;128m"
# define CB_CHRT		"\x1b[48;2;128;255;0m"	//chartreuse
# define CB_DCHRT		"\x1b[48;2;64;128;0m"
# define CB_LGREEN	"\x1b[48;2;128;255;128m"
# define CB_GREEN	"\x1b[48;2;0;255;0m"
# define CB_DGREEN	"\x1b[48;2;0;128;0m"
# define CB_LSPRGR		"\x1b[48;2;128;255;192m"
# define CB_SPRGR		"\x1b[48;2;0;255;128m"	//spring green
# define CB_DSPRGR		"\x1b[48;2;0;128;64m"
# define CB_LCYAN	"\x1b[48;2;128;255;255m"
# define CB_CYAN	"\x1b[48;2;0;255;255m"
# define CB_DCYAN	"\x1b[48;2;0;128;128m"
# define CB_LAZURE		"\x1b[48;2;0;192;255m"
# define CB_AZURE		"\x1b[48;2;0;128;255m"
# define CB_DAZURE		"\x1b[48;2;0;64;128m"
# define CB_LBLUE	"\x1b[48;2;128;128;255m"
# define CB_BLUE	"\x1b[48;2;0;0;255m"
# define CB_DBLUE	"\x1b[48;2;0;0;128m"
# define CB_LVIOLET		"\x1b[48;2;192;0;255m"
# define CB_VIOLET		"\x1b[48;2;128;0;255m"
# define CB_DVIOLET		"\x1b[48;2;64;0;255m"
# define CB_LMGNT	"\x1b[48;2;255;128;255m"
# define CB_MGNT	"\x1b[48;2;255;0;255m"	//magenta
# define CB_DMGNT	"\x1b[48;2;128;0;128m"
# define CB_LROSE		"\x1b[48;2;255;128;192m"
# define CB_ROSE		"\x1b[48;2;255;0;128m"
# define CB_DROSE		"\x1b[48;2;128;0;64m"
/* ====================================||==================================== *\
||								 Common Colors								  ||
\* ===============colors===============||==============©Othello============== */
# define CB_LBROWN	"\x1b[48;2;192;144;96m"
# define CB_BROWN	"\x1b[48;2;128;64;0m"	//hue 30
# define CB_DBROWN	"\x1b[48;2;64;32;0m"
# define CB_LPURPLE	"\x1b[48;2;192;96;192m"
# define CB_PURPLE	"\x1b[48;2;128;0;128m"	//hue 300
# define CB_DPURPLE	"\x1b[48;2;64;0;64m"
# define CB_LPINK		"\x1b[48;2;255;224;229m"
# define CB_PINK		"\x1b[48;2;255;192;203m"	//hue 350
# define CB_DPINK		"\x1b[48;2;128;48;62m"
/* ====================================||==================================== *\
||								  Prized Metal								  ||
\* ===============colors===============||==============©Othello============== */
# define CB_BRONZE	"\x1b[48;2;205;127;50m"
# define CB_SILVER	"\x1b[48;2;192;192;192m"
# define CB_GOLD	"\x1b[48;2;255;215;0m"
/* ====================================||==================================== *\
||																			  ||
||								 Miscellaneous								  ||
||																			  ||
\* ===============colors===============||==============©Othello============== */
# define C_HEADER	"\x1b[48;2;85;85;85m \x1b[48;2;139;139;139m \
\x1b[48;2;192;192;192m \x1b[48;2;255;128;0m \x1b[1m\x1b[38;2;0;0;0m"
# define C_SUBHEAD	"\x1b[48;2;85;85;85m \x1b[48;2;139;139;139m \
\x1b[48;2;192;192;192m \x1b[1m\x1b[38;2;0;0;0m"
# define C_OK		"\x1b[38;2;16;223;16m"
# define CC_CLEAR	"\x1b[H\x1b[0J"
# define CC_1UP		"\033[F\x1b[0J"
# define CC_LINE	"\r\x1b[0J"

#endif