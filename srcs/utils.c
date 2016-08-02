/*
** utils.c for ok in /home/essian_b/rendu/PSU_2015_nmobjdump/objdump
**
** Made by Essiane Abel
** Login   <essian_b@epitech.net>
**
** Started on  Sat Feb 27 05:52:56 2016 Essiane Abel
** Last update Sun Feb 28 20:58:09 2016 Essiane Abel
*/

#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <elf.h>
#include <stdarg.h>
#include "objdump.h"
#include "utils.h"

void		*add_pointers(unsigned args, void *first, ...)
{
  void		*result;
  va_list	ap;
  unsigned long	next;
  unsigned	i;

  i = 0;
  va_start(ap, first);
  result = first;
  while (i < args)
    {
      next = va_arg(ap, unsigned long);
      result = (void *)((unsigned long)result + next);
      ++i;
    }
  va_end(ap);
  return (result);
}

void		*move(void *dest, t_file *file)
{
  return (dest > file->end ? 0 : dest);
}

int		quit(char const *format, char const *name)
{
  fprintf(stderr, format, name);
  return (1);
}

void		print_dots(unsigned char const *pos, unsigned save,
			   unsigned i)
{
  unsigned	spaces;

  if ((i - save) == 16)
    spaces = 1;
  else
    {
      spaces = (16 - (i - save));
      spaces = (spaces * 2) + (spaces / 4) + sizeof(" ") - 1;
    }
  while (spaces--)
    printf(" ");
  spaces = save;
  while (spaces < i)
    {
      printf("%c", (isprint(pos[spaces]) ? pos[spaces] : '.'));
      ++spaces;
    }
  spaces = (i - save);
  while (spaces++ < 16)
    printf(" ");
  printf("\n");
}
