/*
** file_headers.c for ok in /home/essian_b/rendu/PSU_2015_nmobjdump/objdump
**
** Made by Essiane Abel
** Login   <essian_b@epitech.net>
**
** Started on  Sat Feb 27 05:19:17 2016 Essiane Abel
** Last update Sun Feb 28 21:03:33 2016 Essiane Abel
*/

#include <string.h>
#include <stdio.h>
#include <elf.h>
#include "objdump.h"
#include "utils.h"

static void		print_bfd_flags(unsigned flags)
{
  unsigned		i;
  unsigned		printed;
  static const char	* const strs[] = {
    "BFD_NO_FLAGS",
    "HAS_RELOC",
    "EXEC_P",
    "HAS_LINENO",
    "HAS_DEBUG",
    "HAS_SYMS",
    "HAS_LOCALS",
    "DYNAMIC",
    "WP_TEXT",
    "D_PAGED",
  };

  i = 1;
  printed = 0;
  while (i < END)
    {
      if (flags & (1 << i))
	printf("%s%s", (printed++ ? ", " : ""), strs[i]);
      ++i;
    }
  printf("\n");
}

static void     print_file_header(t_file const *file)
{
  char		*machine;
  char		*adress_format;

  if (file->machine == UNKNOWN)
    machine = "UNKNOWN!";
  else
    machine = (file->machine == BIT64 ? "i386:x86-64" : "i386");
  printf("architecture: %s, flags 0x%08x:\n", machine, file->flags >> 1);
  print_bfd_flags(file->flags);
  adress_format = (file->arch == BIT32
		   ? "start address 0x%08x\n" : "start address 0x%016llx\n");
  printf(adress_format, file->header.e_entry);
  printf("\n");
}

static void	dump_section(t_file const *file, void *sc)
{
  unsigned	save;
  unsigned	i;
  unsigned	j;
  unsigned	k;
  unsigned	max;
  unsigned	addr;

  i = get_offset(file, sc, NORMAL);
  max = get_offset(file, sc, MAX);
  addr = get_addr(file, sc);
  while (i < max)
    {
      printf(" %04x ", addr);
      j = -1;
      save = i;
      while (++j < 4 && i < max)
	{
	  k = -1;
	  while (++k < 4 && i < max)
	    printf("%02x", ((unsigned char *)file->start)[i++]);
	  printf(" ");
	}
      addr += 16;
      print_dots((unsigned char *)file->start, save, i);
    }
}

static void	print_sections(t_file const *file)
{
  unsigned	i;
  Elf64_Shdr    *sc;

  i = 0;
  while (i < file->header.e_shnum)
    {
      sc = add_pointers(2, file->start, file->header.e_shoff,
			(i * file->header.e_shentsize));
      if ((sc->sh_type && sc->sh_type != SHT_NOBITS &&
	   sc->sh_type != SHT_SYMTAB) && i != file->shstrndx &&
	  strcmp(&file->shstr[sc->sh_name], ".strtab"))
	{
	  printf("Contents of section %s:\n",
		 (char *)add_pointers(1, file->shstr, sc->sh_name));
	  dump_section(file, sc);
	}
      ++i;
    }
}

void		print_objdump(t_file const *file)
{
  char		*arch;

  arch = (file->arch == BIT32 ? "elf32-i386" : "elf64-x86-64");
  printf("\n");
  printf("%s:     file format %s\n", file->name, arch);
  if (file->dump_file_header)
    print_file_header(file);
  if (file->dump_sections)
    print_sections(file);
}
