/*
** getters.c for ok in /home/essian_b/rendu/PSU_2015_nmobjdump/objdump
**
** Made by Essiane Abel
** Login   <essian_b@epitech.net>
**
** Started on  Sat Feb 27 19:24:15 2016 Essiane Abel
** Last update Sat Feb 27 19:28:07 2016 Essiane Abel
*/

#include <elf.h>
#include <sys/types.h>
#include "objdump.h"
#include "utils.h"

uint32_t	get_true_shstrndx(t_file *file)
{
  void		*sect;

  if (file->header.e_shstrndx != SHN_XINDEX)
    return (file->header.e_shstrndx);
  sect = add_pointers(2, file->start, file->header.e_shoff);
  return (file->arch == BIT32 ? ((Elf32_Shdr *)sect)->sh_link :
	  ((Elf64_Shdr *)sect)->sh_link);
}

unsigned	get_offset(t_file const *file, void const *sc, enum e_get type)
{
  unsigned	offset;

  if (file->arch == BIT32)
    offset = ((Elf32_Shdr const *)sc)->sh_offset +
      ((type != MAX || ((Elf32_Shdr const *)sc)->sh_type == SHT_NOBITS) ? 0 :
       ((Elf32_Shdr const *)sc)->sh_size);
  else
    offset = ((Elf64_Shdr const *)sc)->sh_offset +
      ((type != MAX || ((Elf64_Shdr const *)sc)->sh_type == SHT_NOBITS) ? 0 :
       ((Elf64_Shdr const *)sc)->sh_size);
  return (offset);
}

unsigned long	get_addr(t_file const *file, void const *sc)
{
  return (file->arch == BIT32 ? ((Elf32_Shdr const *)sc)->sh_addr :
	  ((Elf64_Shdr const *)sc)->sh_addr);
}
