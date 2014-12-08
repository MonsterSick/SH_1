/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiouf <mdiouf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/10/06 17:17:43 by mdiouf            #+#    #+#             */
/*   Updated: 2014/12/08 23:05:42 by mdiouf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell1.h"
#include "get_next_line.h"

void		init_main(t_main *vars, int argc, char **argv)
{
	vars->fd = 0;
	vars->args_nbr = 0;
	vars->i = 0;
	vars->env = NULL;
	vars->temp_env = NULL;
	vars->line = NULL;
	vars->line2 = NULL;
	vars->tmp = NULL;
	vars->tmp2 = NULL;
	vars->cmd_list = NULL;
	vars->cmd_list2 = NULL;
	vars->command = NULL;
	vars->test_path = NULL;
	vars->paths = NULL;
	vars->split_args = NULL;
	vars->split_args2 = NULL;
	argc = argc;
	argv = argv;
	ft_putstr("$> ");
}

void		while_funcs(t_main **vars, t_paths **var)
{
	if (ft_strcmp((*vars)->command, "exit") == 0)
		exit(0);
	else if (ft_strcmp((*vars)->command, "cd") == 0)
		execute(vars, var);
	else if (ft_strcmp((*vars)->command, "setenv") == 0)
		execute(vars, var);
	else if (ft_strcmp((*vars)->command, "unsetenv") == 0)
		execute(vars, var);
	else if (ft_strcmp((*vars)->command, "env") == 0)
		execute(vars, var);
	else
		ft_fork(vars, var);
}

void		handle_and1(t_main **vars, t_paths **var, t_tree **temp)
{
	*temp = (*temp)->left->left_two;
	if ((*vars)->line != NULL)
	{
		free((*vars)->line);
		(*vars)->line = NULL;
		(*vars)->line = (*temp)->cmd;
	}
	if ((*vars)->split_args != NULL)
	{
		free((*vars)->split_args);
		(*vars)->split_args = NULL;
	}
	ft_split_args(vars);
	(*vars)->cmd_list = str_split_pipes((*temp)->cmd);
	while_funcs(vars, var);
}

void		handle_and2(t_main **vars, t_paths **var, t_tree **temp)
{
	*temp = (*vars)->var.root;
	if ((*temp)->left->right_two != NULL)
	{
		*temp = (*vars)->var.root->left->right_two;
		if ((*vars)->line != NULL)
		{
			free((*vars)->line);
			(*vars)->line = NULL;
			(*vars)->line = (*temp)->cmd;
		}
		if ((*vars)->split_args != NULL)
		{
			free((*vars)->split_args);
			(*vars)->split_args = NULL;
		}
		ft_split_args(vars);
		(*vars)->cmd_list = str_split_pipes((*temp)->cmd);
		while_funcs(vars, var);
	}
}

void		handle_and(t_main **vars, t_paths **var, t_tree **temp)
{
	(*vars)->type = 1;
	if ((*temp)->left->left_two != NULL)
	{
		handle_and1(vars, var, temp);
		handle_and2(vars, var, temp);
	}
}

void		handle_pipe2(t_main **vars, t_paths **var, t_tree **temp)
{
	*temp = (*vars)->var.root;
	if ((*temp)->left->right_two != NULL)
	{
		*temp = (*vars)->var.root->left->right_two;
		if ((*vars)->line2 != NULL)
		{
			free((*vars)->line2);
			(*vars)->line2 = NULL;
			(*vars)->line2 = (*temp)->cmd;
		}
		else
			(*vars)->line2 = (*temp)->cmd;
		ft_split_args2(vars);
		(*vars)->cmd_list2 = str_split_pipes((*temp)->cmd);
		while_funcs(vars, var);
	}
}

void		handle_pipe1(t_main **vars, t_tree **temp)
{
	*temp = (*temp)->left->left_two;
	if ((*vars)->line != NULL)
	{
		free((*vars)->line);
		(*vars)->line = NULL;
		(*vars)->line = (*temp)->cmd;
	}
	if ((*vars)->split_args != NULL)
	{
		free((*vars)->split_args);
		(*vars)->split_args = NULL;
	}
	ft_split_args(vars);
	(*vars)->cmd_list = str_split_pipes((*temp)->cmd);
//	while_funcs(vars, var);
}

void		handle_pipe(t_main **vars, t_paths **var, t_tree **temp)
{
	(*vars)->type = 2;
	if ((*temp)->left->left_two != NULL)
	{
		handle_pipe1(vars, temp);
		handle_pipe2(vars, var, temp);
	}
}

void		print_command_lists(t_main **vars, int *i)
{
	*i = 0;
	if ((*vars)->cmd_list != NULL)
	{
		while ((*vars)->cmd_list[*i] != NULL)
		{
			printf("(*vars)->cmd_list[i] %s\n", (*vars)->cmd_list[*i]);
			(*i)++;
		}
	}
	if ((*vars)->cmd_list2 != NULL)
	{
		*i = 0;
		while ((*vars)->cmd_list2[*i] != NULL)
		{
			printf("(*vars)->cmd_list2[i] %s\n", (*vars)->cmd_list2[*i]);
			(*i)++;
		}
	}
}

void		init_while_tree(t_main **vars, t_tree **temp, int *i)
{
	*i = 0;
	(*vars)->pipe_fd[0] = -1;
	(*vars)->pipe_fd[1] = -1;
	*temp = (*vars)->var.root; // initialise to first command
	(*vars)->type = 0;
}

void		while_tree(t_main **vars, t_paths **var)
{
	t_tree	*temp;
	int		i;

	init_while_tree(vars, &temp, &i);
	if (ft_strcmp(temp->left->cmd, ";") == 0) // ; handling
		handle_and(vars, var, &temp);
	else if (ft_strcmp(temp->left->cmd, "|") == 0)
		handle_pipe(vars, var, &temp);
	else if (temp->left != NULL && (ft_strcmp(temp->left->cmd, "|") == 0 ||
			ft_strcmp(temp->left->cmd, ";") == 0))
	{
		if (temp->left->left_two != NULL)
			temp = temp->left->left_two;
	}
	else if (temp->left != NULL)
	{
		temp = temp->left;
		(*vars)->cmd_list = str_split_pipes(temp->cmd);
		while_funcs(vars, var);
	}
	print_command_lists(vars, &i);
}

// leftovers from above function
//	while (temp != NULL)
//	{
//		while_funcs(vars, var);
//	}
//	return (0);
//	while (ft_command(vars))
//	{
//		while_funcs(vars, var);
//	}

void		main_body(t_main *vars)
{
	if (vars->split_args != NULL)
	{
		free(vars->split_args);
		vars->split_args = NULL;
	}
	if (vars->command != NULL)
	{
		free(vars->command);
		vars->command = NULL;
	}
/*	if (vars->split_args2 != NULL)
	{
		free(vars->split_args2);
		vars->split_args2 = NULL;
	}
	if (vars->command2 != NULL)
	{
		free(vars->command2);
		vars->command2 = NULL;
	}*/
}

int			init_main1(t_mn *vr, char **envp)
{
	vr->temp = &(vr->var);
	vr->temp2 = &(vr->vars);
	if (envp[0] == NULL)
	{
		ft_putstr_fd("Program cannot run without env\n", 2);
		return (-1);
	}
	return (0);
}

int			main(int argc, char **argv, char **envp)
{
	t_mn	vr;

	if (init_main1(&vr, envp) == -1)
		return (0);
	init_main(&(vr.vars), argc, argv);
	cpy_env(envp, &(vr.vars));
	vr.vars.paths = ft_split_path(envp, &(vr.vars.env));
	while (get_next_line(vr.vars.fd, &(vr.vars.line)))
	{
		vr.vars.zero_backup = dup(0);
		vr.vars.one_backup = dup(1);
		if (vr.vars.line != NULL)
		{
			ft_split_args(&(vr.temp2));
			vr.vars.list = split_pipe_and(vr.vars.line);
			create_tree(&(vr.vars.list), &(vr.vars.var));
			while_tree(&(vr.temp2), &(vr.temp));
//			while_funcs(&(vr.temp2), &(vr.temp));
			main_body(&(vr.vars));
		}
		ft_putstr("$> ");
	}
	return (0);
}
