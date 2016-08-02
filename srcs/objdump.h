/*
** objdump.h for ok in /home/essian_b/rendu/PSU_2015_nmobjdump/objdump
**
** Made by Essiane Abel
** Login   <essian_b@epitech.net>
**
** Started on  Sat Feb 27 05:39:22 2016 Essiane Abel
** Last update Sat Feb 27 19:12:56 2016 Essiane Abel
*/

#ifndef OBJDUMP_H_
# define OBJDUMP_H_

enum		e_arch
  {
    BIT32,
    BIT64,
    UNKNOWN
  };

enum		e_bfd
  {
    BFD_NO_FLAGS = 0,
    HAS_RELOC,
    EXEC_P,
    HAS_LINENO,
    HAS_DEBUG,
    HAS_SYMS,
    HAS_LOCALS,
    DYNAMIC,
    WP_TEXT,
    D_PAGED,
    END
  };

typedef struct	s_file
{
  Elf64_Ehdr    *start;
  void		*end;
  char const    *name;
  int		fd;
  Elf64_Ehdr	header;
  off_t		size;
  enum e_arch	arch;
  enum e_arch	machine;
  unsigned	flags;
  int		dump_sections;
  int		dump_file_header;
  unsigned      shstrndx;
  char		*shstr;
  void		*shstr_s;
}		t_file;

enum		e_get
  {
    MAX,
    NORMAL
  };

int		get_elf_header(t_file *);
void		print_objdump(t_file const *);

#endif /* !OBJDUMP_H_ */
