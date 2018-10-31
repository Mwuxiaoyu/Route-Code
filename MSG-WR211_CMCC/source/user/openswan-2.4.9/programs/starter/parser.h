/* FreeS/WAN config file parser (parser.h)
 * Copyright (C) 2001-2002 Mathieu Lafon - Arkoon Network Security
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * RCSID $Id: parser.h,v 1.1.1.1 2011/08/17 11:10:42 brwang Exp $
 */

#ifndef _IPSEC_PARSER_H_
#define _IPSEC_PARSER_H_

struct config_parsed *parser_load_conf (const char *file, char **perr);
void parser_free_conf (struct config_parsed *cfg);

extern int warningsarefatal;

extern char *parser_cur_filename(void);
extern int   parser_cur_lineno(void);
extern void parser_y_error(char *b, int size, const char *s);
extern void parser_y_init (const char *f);
extern void parser_y_fini (void);
extern int  parser_y_include (const char *filename);
extern char rootdir[PATH_MAX];       /* when evaluating paths, prefix this to them */



#define THIS_IPSEC_CONF_VERSION 2

#endif /* _IPSEC_PARSER_H_ */

/*
 * Local Variables:
 * c-basic-offset:4
 * c-style: pluto
 * End:
 */
