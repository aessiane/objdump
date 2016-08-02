/*
** utils.h for ok in /home/essian_b/rendu/PSU_2015_nmobjdump/objdump
**
** Made by Essiane Abel
** Login   <essian_b@epitech.net>
**
** Started on  Sat Feb 27 05:57:59 2016 Essiane Abel
** Last update Sun Feb 28 21:00:16 2016 Essiane Abel
*/

#ifndef UTILS_H_
# define UTILS_H_

union		u_rev
{
  unsigned	a;
  char		tab[4];
};

uint32_t	get_true_shstrndx(t_file *file);
void		*add_pointers(unsigned args, void *first, ...);
void		*move(void *dest, t_file *file);
int		quit(char const *format, char const *name);
unsigned	get_offset(t_file const *file, void const *sc, enum e_get type);
unsigned long	get_addr(t_file const *file, void const *sc);
unsigned	revert_data(unsigned *data);
void		print_dots(unsigned char const *pos, unsigned i, unsigned j);

#endif /* !UTILS_H_ */
