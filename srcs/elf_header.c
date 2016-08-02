/*
** elf_header.c for ok in /home/essian_b/rendu/PSU_2015_nmobjdump/objdump
**
** Made by Essiane Abel
** Login   <essian_b@epitech.net>
**
** Started on  Sat Feb 27 06:09:27 2016 Essiane Abel
** Last update Sun Feb 28 03:29:23 2016 Essiane Abel
*/

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <elf.h>
#include "objdump.h"
#include "utils.h"

static void		copy_for_32bit_compatibility(t_file *file)
{
  Elf32_Ehdr		*header;

  header = (Elf32_Ehdr *)file->start;
  file->header.e_entry = header->e_entry;
  file->header.e_phoff = header->e_phoff;
  file->header.e_shoff = header->e_shoff;
  file->header.e_flags = header->e_flags;
  file->header.e_ehsize = header->e_ehsize;
  file->header.e_phentsize = header->e_phentsize;
  file->header.e_phnum = header->e_phnum;
  file->header.e_shentsize = header->e_shentsize;
  file->header.e_shnum = header->e_shnum;
  file->header.e_shstrndx = header->e_shstrndx;
}

static int		first_check_and_copy(t_file *file)
{
  if (file->header.e_ident[EI_MAG0] != ELFMAG0 ||
      file->header.e_ident[EI_MAG1] != ELFMAG1 ||
      file->header.e_ident[EI_MAG2] != ELFMAG2 ||
      file->header.e_ident[EI_MAG3] != ELFMAG3 ||
      file->header.e_ident[EI_DATA] != ELFDATA2LSB ||
      (file->header.e_ident[EI_CLASS] != ELFCLASS64 &&
       file->header.e_ident[EI_CLASS] != ELFCLASS32) ||
      (file->header.e_ident[EI_DATA] != ELFDATA2MSB &&
       file->header.e_ident[EI_DATA] != ELFDATA2LSB) ||
      file->header.e_ident[EI_VERSION] != EV_CURRENT)
    return (quit("objdump: %s: File format not recognized\n",
		 file->name));
  file->arch = (file->header.e_ident[EI_CLASS] == ELFCLASS64 ? BIT64 : BIT32);
  if (file->header.e_type == ET_REL)
    file->flags |= 1 << HAS_RELOC;
  else if (file->header.e_type == ET_EXEC)
    file->flags |= 1 << EXEC_P;
  else if (file->header.e_type == ET_DYN)
    file->flags |= 1 << DYNAMIC;
  file->machine = (file->header.e_machine == EM_X86_64 ? BIT64 :
		   (file->header.e_machine == EM_386 ? BIT32 : UNKNOWN));
  if (file->arch == BIT32)
    copy_for_32bit_compatibility(file);
  file->dump_file_header = 1;
  return (0);
}

static int	check_shstrtab(t_file *file)
{
  Elf64_Shdr	*shstr;

  if (file->header.e_shstrndx == SHN_UNDEF)
    return (1);
  if (file->header.e_shstrndx >= file->header.e_shnum)
    {
      fprintf(stderr, "%s%s has a corrupt string table index - ignoring\n",
	      "objdump: warning: ", file->name);
      return (1);
    }
  file->shstrndx = get_true_shstrndx(file);
  shstr = add_pointers(2, file->start, file->header.e_shoff,
		       (file->header.e_shentsize * file->shstrndx));
  file->shstr_s = shstr;
  if (shstr->sh_type != SHT_STRTAB)
    {
      fprintf(stderr, "objdump: %s: attempt to load strings %s (number %d)\n",
	      file->name, "from a non-string section", file->shstrndx);
      return (1);
    }
  if (!(file->shstr = move(
    add_pointers(1, file->start, file->arch == BIT32 ?
		 ((Elf32_Shdr *)shstr)->sh_offset : shstr->sh_offset), file)))
    return (quit("objdump: %s: File truncated\n", file->name) + 1);
  return (0);
}

static int	check_sections(t_file *file)
{
  unsigned	i;
  int		ret;
  Elf64_Shdr    *sc;

  if ((move(add_pointers(2, file->start,
			 (file->header.e_shnum * file->header.e_shentsize),
			 file->header.e_shoff), file)) == NULL)
    return (quit("objdump: %s: File truncated\n", file->name));
  else if ((ret = check_shstrtab(file)))
    return (ret - 1);
  file->dump_sections = 1;
  i = -1;
  while (++i < file->header.e_shnum)
    {
      sc = add_pointers(2, file->start, file->header.e_shoff,
			(i * file->header.e_shentsize));
      if (sc->sh_type == SHT_SYMTAB || sc->sh_type == SHT_DYNSYM)
	file->flags |= (1 << HAS_SYMS);
      else if (sc->sh_type == SHT_DYNAMIC)
	file->flags |= (1 << D_PAGED);
      if (sc->sh_type && sc->sh_type != SHT_NOBITS && sc != file->shstr_s &&
	  !move(add_pointers(1, file->start, get_offset(file, sc, MAX)), file))
	return (quit("objdump: %s: File truncated\n", file->name));
    }
  return (0);
}

int		get_elf_header(t_file *file)
{
  if (file->size < (off_t)sizeof(Elf32_Ehdr))
    return (quit("objdump: %s: File truncated\n", file->name));
  memcpy(&file->header, file->start, sizeof(Elf64_Ehdr));
  return (first_check_and_copy(file) || check_sections(file));
}
