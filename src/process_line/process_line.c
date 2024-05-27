/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-jes <jode-jes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 11:19:59 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/27 15:35:01 by jode-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
CASOS DE USO:

O comando pode conter:
1 - o comando,
2 - um ou mais argumentos,
3 - o sinal de redirection < ou > (truncate) ou os dois no mesmo
	... comando (exemplo: echo main > file.txt ou echo main < file.txt ou
	... echo main < file.txt > file2.txt),
4 - ... ou repetidos (<< ou >> ... sinal de heredoc ou o operador de append >>
	... (exemplo: echo main >> file.txt),
5 - ... ou o pipe (|) (exemplo: echo main | wc -c).
6 - O comando poderá ser dividido em 3 partes (início, meio, fim) de forma a
	...facilitar o seu tratamento e análise.
7 - Não lidaremos com o operador lógico && ou || (exemplo: echo main
	&& echo main2), ou o operador
	... de bloco () (exemplo: (echo main)),
		ou o operador de background & (exemplo: echo main &).
	Esses fazem parte do bónus do projeto, o qual não será abordado aqui.
	Exemplo de comando global com todos os operadores,
		argumentos: echo main > file.txt < file.txt >> file2.txt | wc -c
Exemplo de outro comando global com todos os operadores,
	argumentos: < file.txt echo main > file.txt >> file2.txt | wc -c
//
//
//
Requisitos de uso:
//

Existem partes TRATADAS NESTE FICHEIRO e
... partes TRATADAS NO FICHEIRO EXECUTÁVEL
... que só podem/devem (estas últimas) ser
... verificadas no código do próprio comando.
... Caso contrário teríamos inúmeros/incontáveis
... casos de uso.
... Para a referida análise das combinações possíveis
... da linha de comando, e de forma a facilitar essa
... análise, dividirmos o comando em 3 partes: início, meio, fim,
... e analisamos a linha de comandos consoante esta divisão.


INÍCIO E FIM DA LINHA DE COMANDO

TRATADO NESTE FICHEIRO: 1.1/5.1, 2.1, 2.2, 4.1, 4.2, 7.1

TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL: 1.2, 1.3, 2.3, 3.1, 3.2, 3.3, 4.3,
	4.4, 4.5, 4.6, 4.7, 6.1, 6.2, 6.3, 6.4, 6.5

1 - Syntax do comando - Linha de comando e comando
***************** TRATADO NESTE FICHEIRO ***********************
	1.1 ---------> check_pipes <--------
		A linha de comando NÃO PODERÁ começar:
	1.1 - ... com Pipe (tratado no ponto 5.1), pq pipe requer
		... um argumento antes e depois, logo dará erro:
		..."-bash: syntax error near unexpected token `|'"
	TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
	1.2 - ou com argumento
			exemplo: joaosilvas-Air:src joaosilva$ main
			"-bash: main: command not found"
	TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
	1.3 - A linha de comando poderá começar com:
		1.3,1 - O comando (E DEVE)
		1.3,2 ... espaços
		1.3,3 - redirections stdin para file (<) ou
		1.3,4 - truncate (>) - stdout para file ou
		1.3,5 - heredoc (<<) ou
		1.3,6 - append (<<)
		1.3,7 - com o caminho ou sub-caminho (ex: /bin/ls)
		1.3,8 - com o comando (cat, echo, pwd, cd, etc)
		1.3,9 - com o comando com o argumento ou sub-comando (cat -e makefile,
			... echo joao, cd - ou cd .. , etc)
	... (exemplo: > file.txt echo main ou
	... << file.txt echo main ou >> file.txt echo main).
	... No ponto 1.2,1 aquando de começar com um comando,
	... ele será verificado quando o comando for ser executado.
	... Se o comando tem de existir no sistema ou
	... ser built-in, senão dá erro de comando não encontrado:
	... "-bash: echo Hello World: command not found" (exemplo: "echo Hello World"
	... é considerado apenas um argumento e por isso é valido, mas dá
	... erro porque o comando não é válido/não existe).

	2- Syntax do comando - espaços:
***************** TRATADO NESTE FICHEIRO ***********************
	2.1 ---------> função process_line através da strtrim para strtrim_line <--------
	2.1 - O comando pode conter um ou mais espaços no início e/ou fim.
		... (exemplo: "   echo main  "). Contudo estes devem ser tratados
		... antes de serem passados para o run_cmd (a função que o executa).
		... Por exemplo utilizando a função strtrim para "aparar" os
		... espaços nas pontas (início e fim).
		...	Retirar espaços no ínicio e fim da linha - strtrim.
		... Se esses espaços estiverem entre o comando e o argumento
		... e se comando existe é tratado no comando que o irá
		... executar (devido às inúmeras hipóteses existentes)



MEIO

O meio depende do comando a ser executado. Contudo, quer pipe,
		... quer <> ou << ou >> têm de poder ser lidos com espaços entre si
		... e os argumentos ou sem esse espaço (sendo necessário expandir
		... essa linha no tratamento do comando para que o comando seja
		... enviado de forma correta para o Parser, e de lá para o run_cmd).
		... Outro exemplo,
			o comando pipe tem de reconhecer o pipe sem aspas(exe:
		... echo main | "wc -c"). Com aspas não reconhece o pipe como pipe,
		...reconhece apenas como um argumento ...
		... Exemplos: echo main "|" wc -c  ou  cat file1.txt "|" grep int
		... No echo do exemplo acima assume td como o mm argumento, imprimindo:
		... main | wc -l.
		... No cat assume tb como argumento, dando seguinte resultado:
			... int mani
			... int main (xpto)
			... suca
			... cat: |: No such file or directory
			... cat: grep: No such file or directory
			... cat: int: No such file or directory


***************** TRATADO NESTE FICHEIRO ***********************
	2.2 ---------> insert_nullchar <--------
	2.2 - O pipe e <> e <<>> têm de conseguir ser lidas com espaços ou não:
		... exemplo: cat file2|wc -l tem de ser igual a cat file2 | wc -l
		...exemplo2: cat<<eof >>file1 tem de ser lido cat << eof >>file1.
		A função insert_nullchar inserirá um espaço ("expandindo" a linha)
		... e seguidamente o carater nulo, conjugando as funções expand_line
		...e trim_line do Luís Balsa.
	TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
	2.3 - O comando tem de estar separado do seu argumento através de
		... um espaço (exemplo: echo main). Se não tiver espaço
		... dá erro "-bash: echomain: command not found" (sem aspas).

	3 - Syntax do comando - Argumentos:
	TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		3.1 3.2 3.3 - Será tratado no comando que o irá executar. Aí é que
	... veremos se existe um argumento e se é válido.
	... exemplo: cat -e ou cat main. se não existir o ficheiro main
	... dá erro: "-bash: cat main: No such file or directory"
	O comando pode:
		3.1 - Correr isoladamente/sozinho (dentro ou fora de aspas), isto é,
		... NÃO TER um argumento (com algumas excepções como o grep, que indica
		... como o devemos usar numa msg lançada pela shell:
		... usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] [-C[num]]
		[-e pattern] [-f file] [--binary-files=value] [--color=when]
		[--context[=num]] [--directories=action] [--label] [--line-buffered]
		[--null] [pattern] [file ...])
		3.2 - ter um ou mais argumentos (exemplo: echo main main2 main3
			- 3 argumentos
		... ou echo main main 2 main3 - 4 argumentos.
		... O 2 separado do segundo main conta como 1).
		3.3 - O comando pode ter um sub-comando que complete o comando (-e) ou
		... ter apenas um argumento único (exemplo: cat ou cat -e em
		... cat -e Makefile, echo ou echo -n em echo -n Hello World,
			exit ou exit 42).

	4 - Syntax do comando - Aspas:
	Quer o comando quer o argumento do comando podem ter aspas:
***************** TRATADO NESTE FICHEIRO ***********************
		4.1 ---------> check_unmatched_quotes <--------
		4.1 - As aspas têm de ter correspondência.
		... As aspas têm de ser abertas e fechadas, ou seja,
		... tem de existir correspondência de aspas.
***************** TRATADO NESTE FICHEIRO ***********************
		4.2 ---------> insert_nullchar <--------
		4.2 - temos de verificar se os operadores (|<>) estão dentro
		... de aspas. Se estiverem não podemos colocar um espaço
		... entre estes e o comando a executar.
		... exemplo: cat  < file2"|"wc -l em vez de cat  < file2|wc -l
		... dando erro:
		... "-bash: file2|wc: No such file or directory"
		... Em algumas situações as aspas são ignoradas e o comando
		... é assumido como um todo exemplo: < eof"|wc" -l é assumido
		... como eof|wc, levando a um erro de:
		... "-bash: eof|wc: No such file or directory"
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		4.3 - O comando não pode estar entre aspas com o argumento
		... do comando (sub-comando).
		exemplo: echo main | "wc -c" tem de estar echo main | "wc" -c
		dá erro: "-bash: wc -c: command not found"
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		4.4 - em conjunto (exemplo: 'echo' 'main'
		... ou "echo" "main" ou 'echo' "main" ou "echo" 'main') ou
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		4.5 - de forma isolada (exemplo: echo "main" ou
		... "echo" main ou 'echo' main ou echo 'main').
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		4.6 - Pode ter diversas single ou double quotes
		... que lê na mesma o argumento e corre o comando.
		... exemplo: """"echo"""" main
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		4.7 - O importante é o espaço.
		... Não as pode ter é sem espaço (exemplo: echo"main"),
		... senão dá "-bash: echomain: command not found".
		... As primeiras ou últimas aspas NÃO PRECISAM de ter um
		... espaço antes ou dps.
		...
		... CONTUDO, casos como o | no final da linha de comando, ou aspas
		... isoladas, ou sinal \ não são tratados uma vez que não é
		... pedido no subject. Ao ser tratado deveria ficar à espera do
		... próximo input do utilizador. Ou seja, não dá erro como no minishell.
***************** TRATADO NESTE FICHEIRO ***********************
	5.1 ---------> check_pipes <--------
	5 - Syntax do comando - Pipes
		5.1 - os pipes necessitam de argumentos antes e dps do pipe.
		... Se não existirem dá erro de sintaxe:
		... "-bash: syntax error near unexpected token `|'"
		Por isso o comando não pode começar ou acabar com pipe.

	6 - Syntax do comando - Redirections
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		6.1 Resumindo, < ou << ou > ou >> precisam de argumentos a seguir.
		... Ou seja, depois do < ou << ou > ou >> tem de vir obrigatoriamente o
		... nome do ficheiro (e o comando a seguir a este), senão dá erro:
		... "-bash: syntax error near unexpected token `newline'"
		... exemplo: cat < file1 | grep main
		...	se o ficheiro não existir tb dá erro:
		... "-bash: file55: No such file or directory"
		... exemplo: < file55 cat ou cat < file55 onde file55 n existe.
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		6.2 - os redirection <:
			6.2,1 - antes pode não vir nada q não dá erro (cat < file1) ou
			- antes do < só podem vir os comandos, senão dá erro:
			... "-bash: sad: command not found"
			... < poderá ter o comando antes ou dps do 1º argumento
			... exemplo: cat > file1 ou >file1 cat ao invés de >file1
			6.2,2 - depois do < tem de vir obrigatoriamente o nome do ficheiro
			...(e o comando a seguir a este), senão dá erro:
			... "-bash: syntax error near unexpected token `newline'"
			... exemplo: cat < file1 | grep main
			...	se o ficheiro não existir tb dá erro:
			... "-bash: file55: No such file or directory"
			... exemplo: < file55 cat ou cat < file55 onde file55 n existe.
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		6.3 - os heredoc <<:
			6.3,1 - antes pode não vir nada q não dá erro (<< eof)
			- antes do << só podem vir os comandos com o respetivo
			... nome do ficheiro (cat << eof), senão
			... dá erro: "-bash: sad: command not found"
			6.3,2 - depois tem de vir obrigatoriamente o nome do limitador
			... (eof ou outro), senão dá erro:
			... "-bash: syntax error near unexpected token `newline'"
			6.3,3 - << poderá ter o comando antes ou dps
			...do 1º argumento. Exemplo: cat << eof ou <<eof cat
			O QUE ESTÁ DENTRO (PQ ESTÁ ENTRE < E >) E APÓS < OU <<
			... NÃO SERÁ AVALIADO NESTE FICHEIRO MAS
			... SIM NO COMANDO EXECUTÁVEL EM QUESTÃO.
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		6.4 - os redirection >:
			6.4,1 - antes pode não vir nada q não dá erro.
			... exemplo: > file.txt, cria o ficheiro txt, não dando erro.
			- antes do > só podem vir os comandos ou os sub-comandos
			... ou os nomes dos ficheiros, senão dá erro.
			... exemplo: deverá ser assim cat < eof | grep main > eof1, e não
			... assim: asd >eof1. Este dá erro de  "-bash: asd: command not found".
			... Contudo, e apesar de dar erro,
				reescreve no eof1 apagando o conteudo
			... que constava neste anteriormente. qd fazemos cat eof1 o ficheiro está
			... limpo, mesmo que dantes tivesse lá algum conteúdo.
			6.4,2 - depois tem de vir obrigatoriamente o nome do ficheiro,
			...senão dá erro:
			... "-bash: syntax error near unexpected token `newline'"
			... exemplo: echo main > ou echo main > file.txt
			6.4,3 - > poderá ter o comando antes ou dps.
			... poderá tb não ter o comando antes.
			exemplo: >file2
			com >> eof cat | wc -l dá um RESULTADO ESQUISITO
			O QUE ESTÁ DENTRO (PQ ESTÁ ENTRE < E >) E ANTES DE > OU >>
			... NÃO SERÁ AVALIADO NESTE FICHEIRO MAS SIM
			... NO COMANDO EXECUTÁVEL EM QUESTÃO.
		TRATADO NO FICHEIRO DO COMANDO EXECUTÁVEL
		6.5 - os redirection >>:
			6.5,1 - antes pode não vir nada q não dá erro.
			- antes do >> só podem vir os comandos, senão dá erro de:
			... "-bash: sad: command not found"
			6.5,2 - depois tem de vir obrigatoriamente o nome do ficheiro,
			... senão dá erro:
			... "-bash: syntax error near unexpected token `newline'"
			... exemplo: echo main >> ou echo main >> file.txt


	FIM DA LINHA DE COMANDO
		O comando PODE acabar
		7.1 - com pipe (|)
		... abre > para escrevermos o comando que pretendemos que execute, e
		... se inserirmos algo que não seja comando (exemplo: > asfsafa)
		... dá "-bash: asfsafa: command not found".
		... caso o comando esteja correto ele executa-o corretamente:
		... exemplo: cat file1.txt |
		... ABRIU: > grep int
		... RESULTADO: int mani int main (xpto)
		... exemplo acima de um file1.txt com seguinte conteúdo:
			... int main (xpto)
			... mainsuca
			... sucamain
			... spsadmain
			... mainasads
		... caso o segundo comando não faça nada, ele executa o primeiro
		... e manda-o pelo pipe: exemplo: echo main yz |
		... ABRIU: > grep main
		... RESULTADO: main yz
		... CONTUDO, casos como o | no final da linha de comando, ou aspas
		... isoladas, ou sinal \ não são tratados uma vez que não é
		... pedido no subject. Ao ser tratado deveria ficar à espera do
		... próximo input do utilizador. Ou seja, não dá erro como no minishell.



RESUMINDO:
Neste ficheiro trataremos do processamento da linha de comando
... inserida na prompt. Na realidade nós só podemos tratar
1- o início e o fim da linha de comando (alínea 2.1 através da
... função strtrim presente na função process_line)
Haverão algumas exceções como:
2 - o tratamento do operador pipe | ou redirection <> heredoc << ou append >>,
... que serão tratados mesmo estando no "meio" da linha de comando.
... Mesmo no meio aquando de não terem estes operadores têm de ter
... um espaço entre o operador e os seus argumentos (exemplo: cat <file1|grep main
... tem de ser igual a cat < file1 | grep main) (alínea 2.2 através
... da função insert_nullchar presente na função process_line)
3 - Além disso, têm de estar fora de aspas para que tal
... seja permitido (a expansão) (alínea 4.2 através da
... função insert_nullchar presente na função process_line)
4 - Em relação às aspas, as mesmas têm de ter correspondência (ser
...  abertas e fechadas) (alínea 4.1 através da
... função check_unmatched_quotes presente na função process_line)
5 - Erros (resumo):
	5.1 - Resumindo, "-bash: echomain: command not found" dá quando
	... o comando não é válido. E qd não é válido? qd é interpretado
	... e se verifica q não existe no sistema nem um um built-in.
	... O facto de não ter espaço entre ele e o argumento (dentro de aspas ou não).
	5.2
		- "-bash: syntax error near unexpected token `|'" acontece quando o pipe não
	...tem argumento antes ou dps. Por exemplo se pipe estiver no início do comando.
	...Acontece tb qd o pipe não tem comandos antes ou a seguir.
	...No final não dá esse erro. Abre a > e espera que o user
	...introduza um comando válido (caso contrário dá erro).
	5.3 - < ou > ou << ou >> precisam de argumentos a seguir:
	joaosilvas-Air:src joaosilva$ ls | wc >
	"-bash: syntax error near unexpected token `newline'"
	... Mas isso é só tratado no ficheiro do próprio comando.
6 - Expansão - isto será tratado no Expand
	o comando expande com '' ou "" ...
	...algumas excepções: echo '$?' não imprime o erro. Imprime o $?.
	... echo "$?" imprime o erro. Imprime o erro, assim como echo $?.
7 - Outros casos:
	echo ~test retorna ~test
	echo test~ retorna test~
	- reconhece e trata ~ como qualquer outro carater.

*/

#include "../../include/minishell.h"

static void	insert_nullchar(t_shell *shell)
{
	char	*tmp;
	int		quote;

	quote = 0;
	tmp = shell->line;
	while (*(++tmp))
	{
		if (ft_strchr("OPERATORS", *tmp) && !inside_quotes(shell->line, tmp))
			// Se o caractere for um operador e não estiver dentro de aspas
		{
			if (tmp != shell->line && !ft_strchr(" |><", *(tmp - 1)))
				// O catater não está no início da linha e o anterior não é um espaço ou um operador
			{
				if (expand_line(" ", tmp - shell->line, tmp - shell->line,
						&shell->line)) // Se a expansão for bem
					tmp = shell->line - 1;
			}
			else if (!ft_strchr(" |><", *(tmp + 1))) //
				if (expand_line(" ", tmp - shell->line + 1, tmp - shell->line
						+ 1, &shell->line))
					tmp = shell->line - 1;
			if (ft_strchr(SPACES, *tmp) && !quote)
				// Se o caractere for um espaço e não estiver dentro de aspas,
				então incluímos o caracter nulo.
				*tmp = '\0';
		}
	}
	shell->line_len = ft_strlen(shell->line);
}

// Função para alternar o status da citação e verificar se existem aspas não correspondidas
int	inside_quotes(char *line, char *current_position)
{
	int	quote;

	quote = 0;
	while (*line)
	{
		if (*line == '"' || *line == '\'')
		{
			if (quote == 0)
				quote = *line;
					// Se a quote não existir é definida aqui como existente
			else if (quote == *line)
				quote = 0; // Se a quote existir é definida aqui
		}
		if (&line == &current_position)
			break ;
		line++;
	}
	return (quote);
		// Retorna o status de citação atual se nenhuma das condições acima for verdadeira
}

// Função para verificar se existem pipes não correspondidos
static int	check_pipes(t_shell *shell)
{
	if (*shell->line == '|')
		return (print_error_syntax(shell, shell->line, 2));
	if (shell->line[strlen(shell->line) - 1] == '|')
		return (print_error(shell, "Open | not supported", NULL, 2));
	return (0);
}

// Função para verificar erros de sintaxe
static int	check_syntax_errors(t_shell *shell)
{
	if (!check_pipes(shell) && (!inside_quotes(shell->line, shell->line)))
		// Se não houver erros de pipe e não houver citações não correspondidas
		return (0);
	return (1);
}

int	process_line(t_shell *shell)
{
	char *tmp;

	// shell->status = CONTINUE; // Set the shell status to CONTINUE
	tmp = shell->line;                            
		// ????????????? Store the pointer to the original line ENTÃO CRIAS UM FICHEIRO temp E LIBERTA-LO A SEGUIR SEM NUNCA O TERES USADO PARA NADA!!!?
	shell->line = ft_strtrim(shell->line, SPACES);
		// ???????????? Trim leading and trailing spaces from the line. ESPACÇOS SÃO RETIRADOS NO INÍCIO E NO FIM DA STRING. E NO MEIO?
	free(tmp);                                    
		// Free the memory allocated for the original line
	if (shell->line[0] == '\0')                   
		// If the line is empty after trimming
		return (0);                               
			// ??????????? Return 0. PORQUE TEMOS DE RETORNAR ZERO AQUI,
			SE A STRTRIM JÁ RETORNA UMA CÓPIA VAZIA DA S1 NESSE CASO?
	add_history(shell->line);                     
		// É uma função da biblioteca readline que adiciona a linha ao histórico de comandos. Adds the line to the command history.
	if (check_syntax_errors(shell))               
		// Verifica se há erros de syntax nos pipes e nas aspas.
		return (0);
	insert_nullchar(shell);
	return (1); // Return 1
}