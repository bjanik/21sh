#include "parser.h"
#include "exec.h"

void		append_wordlist(t_exec *exec, t_stack *stack)
{
	t_list	*word;
	
	word = ft_lstnew(stack->next->sym.value, ft_strlen(stack->next->sym.value) + 1);
	if (exec->word_list == NULL)
	{
		exec->word_list = word;
		exec->last_word = word;
	}
	else
	{
		exec->last_word->next = word;
		exec->last_word = word;
	}
	exec->word_count++;
}

void		set_here_end(t_exec *exec, t_stack *stack)
{
	t_redir	*redir;

	if (!(redir = init_redir(exec, stack)))
		return ;
	redir->here_end = ft_strdup(stack->next->sym.value);
	redir->type = DLESS;
	if (exec->redir_list == NULL)
	{
		exec->redir_list = redir;
		exec->last_redir = redir;
	}
	else
	{
		exec->last_redir->next = redir;
		exec->last_redir = redir;
	}
}

void		set_dest_file(t_exec *exec, t_stack *stack)
{
	t_redir	*redir;

	if (!((redir = init_redir(exec, stack))))
		return ;
	redir->dest_file = ft_strdup(stack->next->sym.value);
	redir->type = stack->next->next->next->sym.type;
	if (exec->redir_list == NULL)
	{
		exec->redir_list = redir;
		exec->last_redir = redir;
	}
	else
	{
		exec->last_redir->next = redir;
		exec->last_redir = redir;
	}
}

inline void	set_io_number(t_exec *exec, t_stack *stack)
{
	exec->last_redir->fd = ft_atoi(stack->next->next->next->sym.value);
}
