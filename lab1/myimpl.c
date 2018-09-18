#include <printf.h>
#include "prog1.h"


int maxargs(A_stm stm);
int maxargsexp(A_exp exp);
int maxargsstm(A_stm stm);
void interpStm(A_stm);
int interpExp(A_exp);
void interplist(A_expList list);

typedef struct id_t_ *id_t;
struct id_t_ {
    string id;
    int value;
    id_t next;
};
int max;
id_t head;

void id_t_set(string id, int value){
    id_t n = checked_malloc(sizeof(struct id_t_));
    n->id = id;
    n->value = value;
    n->next = head;
    head = n;
}

int id_t_find(string id){
    id_t temp = head;
    while (temp){
        if (strcmp(temp->id, id) == 0){
            return temp->value;
        }
        temp = temp->next;
    }
    return 0;
}

int maxargslist(A_expList list){
	if (list == 0)
		return 0;
	if (list->kind == A_pairExpList){
		return maxargsexp(list->u.pair.head) + maxargslist(list->u.pair.tail) + 1;
	}else if (list->kind == A_lastExpList){
		return maxargsexp(list->u.last) + 1;
	}
    return 0;
}

int maxargsexp(A_exp exp){
	if (exp == 0)
		return 0;
	if (exp->kind == A_idExp){
		return 0;
	}else if (exp->kind == A_numExp){
		return 0;
	}else if (exp->kind == A_opExp){
		return maxargsexp(exp->u.op.left) + maxargsexp(exp->u.op.right);
	}else if (exp->kind == A_eseqExp){
		return maxargsstm(exp->u.eseq.stm) + maxargsexp(exp->u.eseq.exp);
	}
    return 0;
    
}

int maxargsstm(A_stm stm){
    if (stm == 0)
        return 0;
    if (stm->kind == A_compoundStm){
        return maxargsstm(stm->u.compound.stm1) + maxargsstm(stm->u.compound.stm2);
    }else if (stm->kind == A_assignStm){
        return maxargsexp(stm->u.assign.exp);
    }else if (stm->kind == A_printStm){
        int r = maxargslist(stm->u.print.exps);
        max = max > r ? max : r;
        return r;
    }
    return 0;
}

int maxargs(A_stm stm)
{
    max = 0;
    maxargsstm(stm);
    return max;
	//TODO: put your code here.
}

void interp(A_stm stm)
{
    head = 0;
    interpStm(stm);
	//TODO: put your code here.
}

void interpStm(A_stm stm){
    if (stm == 0)
        return;
    if (stm->kind == A_compoundStm){
        interpStm(stm->u.compound.stm1);
        interpStm(stm->u.compound.stm2);
    }else if (stm->kind == A_assignStm){
        id_t_set(stm->u.assign.id, interpExp(stm->u.assign.exp));
    }else if (stm->kind == A_printStm){
        interplist(stm->u.print.exps);
    }
}

int interpExp(A_exp exp){
    if (exp == 0)
        return 0;
    if (exp->kind == A_idExp){
        return id_t_find(exp->u.id);
    }else if (exp->kind == A_numExp){
        return exp->u.num;
    }else if (exp->kind == A_opExp){
        switch (exp->u.op.oper) {
            case A_plus:
                return interpExp(exp->u.op.left) + interpExp(exp->u.op.right);
                break;
            case A_div:
                return interpExp(exp->u.op.left) / interpExp(exp->u.op.right);
                break;
            case A_minus:
                return interpExp(exp->u.op.left) - interpExp(exp->u.op.right);
                break;
            case A_times:
                return interpExp(exp->u.op.left) * interpExp(exp->u.op.right);
                break;
            default:
                break;
        }
    }else if (exp->kind == A_eseqExp){
        interpStm(exp->u.eseq.stm);
        return interpExp(exp->u.eseq.exp);
    }
    return 0;
}

void interplist(A_expList list){
    if (list == 0)
        return;
    if (list->kind == A_pairExpList){
        printf("%d ", interpExp(list->u.pair.head));
        interplist(list->u.pair.tail);
    }else if (list->kind == A_lastExpList){
        printf("%d\n", interpExp(list->u.last));
    }
}






