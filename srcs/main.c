/*
** main.c for ok in /home/essian_b/rendu/nm_objdump
**
** Made by Essiane Abel
** Login   <essian_b@epitech.net>
**
** Started on  Wed Feb  3 04:12:13 2016 Essiane Abel
** Last update Tue Aug 02 15:05:25 2016 ESSIANE Abel
*/

#include <elf.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include "objdump.h"
#include "utils.h"
#include "main.h"

static int	get_file_infos(char const *name, t_file *file)
{
  if ((file->fd = open(name, O_RDONLY)) == -1)
    return (quit("objdump: '%s': %m\n", name));
  else if ((file->size = lseek(file->fd, 0, SEEK_END)) == (off_t)-1 ||
	   !(file->start = (Elf64_Ehdr *)mmap(NULL, file->size, PROT_READ, MAP_SHARED, file->fd, 0)))
    {
      close(file->fd);
      return (quit("objdump: %s: %m\n", name));
    }
  close(file->fd);
  file->end = add_pointers(1, file->start, file->size);
  file->name = name;
  return (get_elf_header(file));
}

static int      process_file(char const *name)
{
  t_file	file;

  memset(&file, 0, sizeof(file));
  if (get_file_infos(name, &file))
    return (1);
  print_objdump(&file);
  return (0);
}

int		main(int ac, char **av)
{
  int		ret;
  int		i;

  i = 0;
  ret = 0;
  if (ac == 1)
    return (process_file(DEFAULT_EXECUTABLE));
  while (++i < ac)
    if (process_file(av[i]))
      ret = 1;
  return (ret);
}
