/****************************************************************************
* DESCRIPTION
* This file contains 3 functions for evaluating stored expressions.
*
* --------------------
* GetVariableList()
*
* Purpose: Return a linked list of variable names, the values for which
*	must be filled in before this list is passed to CalcExpression()
*
* Input:
*	char * expression (as retrieved from hdb table hdb_unit_conversion)
*
* Return:
*	expression_var_list * 
*
*--------------------
* FreeVariableList()
*
* Purpose: Free the variable list once done with it.
*
* Input:
*	expression_var_list * var_list
*
* Return:
* 	void
*
*--------------------
* CalcExpression()
*
* Purpose: evaluate an expression based on its variable list
* 
* Input:
*	double * result
*	char * expression
*	expression_var_list * var_list
*
* Return:
*       int (1 if successful evaluation, 0 if failure)
*
*--------------------
* Example:
*
*main ()
*{
*    char expression[64];
*    double source_value = 100;
*    int number_of_days = 2;
*    int success = 0;
*    double result;
*    expression_var_list * var_list;
*    expression_var_list * current;
*
*    strcpy(expression, "Vdeg_c 9 * 5 / 32 + Pn_days *          ");  rather, get the expression from the db 
*
*    var_list = GetVariableList(expression);
*
*    if (!var_list)
*        printf("No var list returned for expression\n");
*
*    current = var_list;
*    while (current) {
*        switch (current->var_type) {
*            case (SOURCE_VALUE):
*                current->value = source_value;
*                break;
*            case (N_DAYS):
*                current->value = number_of_days;       rather, get number of days btwn dates 
*                break;
*            default:
*                printf("bad variable type returned\n");
*                current->value = 0.0;
*                break;
*        }
*        current = current->nextItem;
*    }
*
*    success = CalcExpression(&result, expression, var_list);
*
*    if (success) {
*        execute success response here
*    }
*    else {
*        execute failure response here
*    }
*
*    FreeVariableList(var_list);
*}
*
* PTM
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include <defines.h> 
#include <typedefs.h> 
#include <utils.h>

#define DS(s,t,i)

/*
 * #define DS(s,t,i) { printf("["); for (i = 0; i < t; i++) printf("%5.1f,",s[i]); printf("]");}
 */

/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetParamType

<P> <STRONG>Function signature:</STRONG> int GetParamType(char *)

<P> <STRONG>Function description:</STRONG> Determines the type of the parameter passed in (if known).

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *var_name: The variable whose type must be determined.
</DL>

<P> <STRONG>Function return value:</STRONG> The type of the variable, e.g., N_DAYS.

</HTML>
*/

int GetParamType(char * var_name)
{
    if (!strcmp(var_name, "n_days"))
        return N_DAYS;

 /* use this template to add new variable types 
  * NOTE: add the new type to the enum!
  *
  * else if (!strcmp(var_name, "the_new_type"))
  *     return THE_NEW_TYPE;
  */

    else
        return ILLEGAL;
}

/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetVariableList

<P> <STRONG>Function signature:</STRONG>  expression_var_list * GetVariableList(char *)

<P> <STRONG>Function description:</STRONG> Return a linked list of variable names, the values for which must be filled in before this list is passed to CalcExpression().

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> char *expression: Expression to be parsed.
</DL>

<P> <STRONG>Function return value:</STRONG> Linked list of expression_var_list

</HTML>
*/
expression_var_list * GetVariableList(char * expression)
{
    char copy[MAX_EXPRESSION_LENGTH];
    char *cp, *newcp, *token, *step;
    expression_var_list * head;
    expression_var_list * current;
    int first = 1;
  
    if (!expression)
        return(NULL);

    strcpy(copy, expression);
    cp = copy;

    for (;;) {
        if ((!cp) || (!*cp)) break;

        NEXT_TOKEN(cp,newcp,token);
        if (!*token) break;

        if ((token[0] == 'V') || (token[0] == 'P')) {
            if (first) {
                first = 0;
                head = (expression_var_list *) malloc(sizeof(expression_var_list));
                head->nextItem = NULL;
                current = head;
            }
            else {
                current->nextItem = (expression_var_list *) malloc(sizeof(expression_var_list));
                current = current->nextItem;
                current->nextItem = NULL;
            }

            if (token[0] == 'V') {
                strcpy(current->var_name, "V"); /* V indicates the source value, the one to be converted */
                current->var_type = SOURCE_VALUE;
            }

            else if (token[0] == 'P') {
                /* walk forward past the 'P' and copy out the var name */
                step = token;
                step++;
                token = step;
                strcpy(current->var_name, token);
                current->var_type = GetParamType(current->var_name);
            }
            else
                return(NULL); /* logical failure of algorithm */
        }
    }

    return(head);
}

/*
<HTML>

<P> <STRONG>Function name:</STRONG> FreeVariableList

<P> <STRONG>Function signature:</STRONG> void FreeVariableList(expression_var_list *)

<P> <STRONG>Function description:</STRONG> Free the variable list once done with it.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> expression_var_list *var_list: Linked list to be free.
</DL>

<P> <STRONG>Function return value:</STRONG>

</HTML>
*/
void FreeVariableList(expression_var_list * var_list)
{
    expression_var_list * current;
    expression_var_list * next;

    if (!var_list)
        return;

    current = var_list;

    while (current) {
        next = current->nextItem;
        free(current);
        current->nextItem = NULL;
        current = next;
    }
}

/*
<HTML>

<P> <STRONG>Function name:</STRONG> CalcExpression

<P> <STRONG>Function signature:</STRONG> int CalcExpression(double *, char *, expression_var_list *)

<P> <STRONG>Function description:</STRONG> Evaluate an expression based on its variable list; used primarily by ConvertValue library routine.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> double *result:                 Result of the calculation.
<BR><DD> char *expression:               The expression to be calculated.
<BR><DD> exrpression_var_list *var_list: The variable list for the expression.
</DL>

<P> <STRONG>Function return value:</STRONG>

</HTML>
*/

int CalcExpression(double * result, 
                   char * expression, 
                   expression_var_list * var_list)
{
    double v = 0.0;
    char copy[MAX_EXPRESSION_LENGTH];
    char *cp, *newcp, *token;
    double stack[MAX_STACK_DEPTH];
    expression_var_list * current;
    int   top = 0;

    double v1, v2;

    if ((!expression) || (!var_list)) {
      PrintError("NULL expression or var_list in CalcExpression");

      return 0;
    }

    current = var_list;

    strcpy(copy, expression);
    cp = copy;

    for (;;) {
        if ((!cp) || (!*cp)) break;

        NEXT_TOKEN(cp,newcp,token);
        if (!*token) break;

        if ((token[0] >= '0') && (token[0] <= '9')) {
            v1 = atof(token);
            PUSH(stack,top,v1);
        }
        else if (token[0] == '+') {
            if (top < 2) {
	      PrintError("less than two in stack when operator found in CalcExpression");
	      return 0;
            }
            v1 = POP(stack,top);
            v2 = POP(stack,top);
            v1 += v2;
            PUSH(stack,top,v1);
        }
        else if (token[0] == '-') {
            if (top < 2) {
	      PrintError("less than two in stack when operator found in CalcExpression");
	      return 0;
            }
            v2 = POP(stack,top);
            v1 = POP(stack,top);
            v1 -= v2;
            PUSH(stack,top,v1);
        }
        else if (token[0] == '*') {
            if (top < 2) {
	      PrintError("less than two in stack when operator found in CalcExpression");
	      return 0;
            }
            v1 = POP(stack,top);
            v2 = POP(stack,top);
            v1 *= v2;
            PUSH(stack,top,v1);
        }
        else if (token[0] == '/') {
            if (top < 2) {
	      PrintError("less than two in stack when operator found in CalcExpression");
	      return 0;
            }
            v2 = POP(stack,top);
            v1 = POP(stack,top);
            if (v2 == 0.0) {
	      PrintError("attempted division by zero in CalcExpression");
	      PUSH(stack,top,0.0);
            }
            else {
                v1 /= v2;
                PUSH(stack,top,v1);
            }
        } /* must be a passed in param */
        else if ((token[0] == 'V') || (token[0] == 'P')) {
            if (!current) {
	      PrintError("no var list for required input in CalcExpression");
	      return 0;
            }
            v1 = current->value;
            PUSH(stack,top,v1);
            current = current->nextItem;
        }
        else {
	  PrintError("illegal expression component found in CalcExpression");
	  return 0;
            /* PUSH(stack,top,0.0); */
        }

        DS(stack,top,i);
    }

    if (top > 1) {
      PrintError("items left in stack upon completion of CalcExpression");
      v = stack[top-1];
    }
    else if (top == 1)
        v = stack[top-1];
    else {
      PrintError("nothing left in stack upon completion of CalcExpression");
      return 0;
    }

    *result = v;
           
    return 1;
}
