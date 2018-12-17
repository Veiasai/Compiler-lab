#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "temp.h"
#include "errormsg.h"
#include "tree.h"
#include "assem.h"
#include "frame.h"
#include "codegen.h"
#include "table.h"


#define MAXLINE 100
#define TL Temp_TempList
#define AIL AS_InstrList
#define AT AS_Targets
#define specialregs TL(F_RAX(), TL(F_RSP(), NULL))
#define argregs TL(F_RDI(), TL(F_RSI(), TL(F_RDX(), TL(F_RCX(), TL(F_R8(),TL(F_R9(), NULL))))))
#define caller_save TL(F_RAX(), TL(F_R10(), TL(F_R11(), NULL)))
#define callee_save TL(F_RBX(), TL(F_RBP(), TL(F_R12(), TL(F_R13(), TL(F_R14(),TL(F_R15(), NULL))))))

static AS_instrList instr_list, cur;
static F_frame frame;
static void munchStm(T_stm stm);
static Temp_temp munchExp(T_exp exp);
static void munchArgs(T_expList l, bool reg);
static void emit(AS_instr itr);     // add itr to instr_list tail

static const int F_keep = 6;	//number of parameters kept in regs;

//Lab 6: put your code here
AS_instrList F_codegen(F_frame f, T_stmList stmList) {
    instr_list = cur = NULL;
    frame = f;

    // callee save reg
    Temp_tempList csr = callee_save;
    Temp_tempList csr_bak, csr_cur; 
    csr_bak = csr_cur = TL(Temp_newtemp(), NULL);
    for(;csr;csr_cur = csr_cur->tail = TL(Temp_newtemp(), NULL), csr=csr->tail)
        emit(AS_Move("movq `s0 `d0", TL(csr_cur->head, NULL), TL(csr->head, NULL)));
    
    while (stmList){
        munchStm(stmList->head);
        stmList = stmList->tail;
    }

    csr = callee_save;
    csr_cur = csr_bak;
    for(;csr;csr_cur = csr_cur->tail = TL(Temp_newtemp(), NULL), csr=csr->tail)
        emit(AS_Move("movq `s0 `d0", TL(csr->head, NULL), TL(csr_cur->head, NULL)));
    
    return instr_list;
}

static void emit(AS_instr itr){
    instr_list = AIL(itr, cur);
    cur = instr_list;
}

static void  munchStm(T_stm stm){
    switch(stm->kind){
        case T_SEQ:
            munchStm(stm->u.SEQ.left);
            munchStm(stm->u.SEQ.right);
            break;
        case T_LABEL:
            emit(AS_Label(Temp_labelstring(stm->u.LABEL),stm->u.LABEL));
            break;
        case T_JUMP:
            assert(stm->u.JUMP.exp->kind == T_NAME);
            Temp_label target = stm->u.JUMP.exp->u.NAME;
            emit(AS_Oper("jmp `j0", NULL, NULL, AT(stm->u.JUMP.jumps)));
            break;
        case T_CJUMP:
            emit(AS_Oper("cmp `s1 `s0", NULL, TL(munchExp(stm->u.CJUMP.left), TL(munchExp(stm->u.CJUMP.right), NULL)), AT(NULL)));
            string inst;
            switch(stm->u.CJUMP.op){
                case T_eq:
                    inst = "je `j0";
                    break;
                case T_ne:
                    inst = "jne `j0";
                    break;
                case T_lt:
                    inst = "jl `j0";
                    break;
                case T_le:
                    inst = "jle `j0";
                    break;
                case T_gt:
                    inst = "jg `j0";
                    break;
                case T_ge:
                    inst = "jge `j0";
                    break;
                case T_ult:
                    inst = "jb `j0";
                    break;
                case T_ule:
                    inst = "jbe `j0";
                    break;
                case T_ugt:
                    inst = "ja `j0";
                    break;
                case T_uge:
                    inst = "jae `j0";
                    break;
            }
            emit(AS_Oper(inst, NULL, NULL, AT(TL(stm->u.CJUMP.true, NULL))));
            break;
        case T_MOVE:{
            T_exp dst = stm->u.MOVE.dst, src = stm->u.MOVE.src;
            if (dst->kind == T_MEM){
                if (dst->u.MEM->kind == T_BINOP){
                    string inst = checked_malloc(MAXLINE * sizeof(char));
                    if (dst->u.MEM->u.BINOP.right->kind == T_CONST){
                        T_exp e1 = dst->u.MEM->u.BINOP.left, e2 = src;
                        sprintf(inst, "movq `s0 %d(`d0)", dst->u.MEM->u.BINOP.right->u.CONST);
                        emit(AS_Oper(inst, TL(munchExp(e1), NULL), 
                                                TL(munchExp(e2), NULL), AT(NULL)));
                        break;
                    }else if (dst->u.MEM->u.BINOP.left->kind == T_CONST){
                        T_exp e1 = dst->u.MEM->u.BINOP.right, e2 = src;
                        sprintf(inst, "movq `s0 %d(`d0)", dst->u.MEM->u.BINOP.left->u.CONST);
                        emit(AS_Oper(inst, TL(munchExp(e1), NULL), 
                                                TL(munchExp(e2), NULL), AT(NULL)));
                        break;
                    }
                    assert(0);
                }else if (src->kind == T_MEM){
                    assert(0);
                }else{
                    emit(AS_Oper("movq `s0, (`d0)", TL(munchExp(dst), NULL), 
                                                TL(munchExp(src), NULL), AT(NULL)));
                    break;
                }
            }else if (dst->kind == T_TEMP){
			    emit(AS_Move("movq `s0, `d0", TL(munchExp(dst), NULL), 
                                                TL(munchExp(src), NULL)));
                break;
            }else{
                assert(0);
            }
        }
        case T_EXP:
            munchExp(stm->u.EXP);
            break;
    }
}

static Temp_temp munchExp(T_exp e){
    Temp_temp d = Temp_newtemp();
    switch(e->kind){
        case T_BINOP:{
			Temp_temp left = munchExp(e->u.BINOP.left);
			Temp_temp right = munchExp(e->u.BINOP.right);
            string opinst;
            switch(e->u.BINOP.op){ 
                case T_plus: opinst = "addq `s0, `d0"; break;
                case T_minus: opinst = "subq `s0, `d0"; break;
                case T_mul: opinst = "imulq `s0, `d0"; break;
                // TODO:
                case T_div: 
                //tiger exclude, TODO:
                    assert(0);
                case T_and: 
                case T_or: 
                case T_lshift: 
                case T_rshift: 
                case T_arshift: 
                case T_xor:
                    assert(0);
            }
            emit(AS_Move("movq `s0, `d0", TL(d, NULL), TL(left, NULL)));
			emit(AS_Oper(opinst, TL(d, NULL), TL(right, TL(d, NULL)), AT(NULL)));
			break;
            }
        case T_MEM:
		    emit(AS_Oper("movq (`s0), `d0", TL(d, NULL), 
                                                TL(munchExp(e->u.MEM), NULL), 
                                                    AT(NULL)));
		    break;
        case T_TEMP:
            return e->u.TEMP;
        case T_ESEQ:
            // standard tree
            assert(0);
        case T_NAME:{
            char *inst = checked_malloc(MAXLINE * sizeof(char));
		    sprintf(inst, "movq $%s, `d0", Temp_labelstring(e->u.NAME));
		    emit(AS_Oper(inst, TL(d, NULL), NULL, AT(NULL)));
            break;
            }
        case T_CONST:{
            char *inst = checked_malloc(MAXLINE * sizeof(char));
		    sprintf(inst, "movq $%d, `d0", e->u.CONST);
		    emit(AS_Oper(inst, TL(Temp_newtemp(), NULL), NULL, AT(NULL)));
            break;
        }
        case T_CALL:{
            Temp_label func = e->u.CALL.fun->u.NAME;
	        char *inst = checked_malloc(MAXLINE * sizeof(char));

	        munchArgs(e->u.CALL.args, TRUE);

	        sprintf(inst, "call %s", Temp_labelstring(func));
	        emit(AS_Oper(inst, caller_save, NULL, AT(NULL)));
	        // emit(AS_Move("movq `s0, `d0", TL(d, NULL), TL(F_RAX(), NULL)));
            break;
            }
    }
    return d;
}

// WTF？？
static void munchArgs(T_expList l, bool reg){
    if (reg){
        Temp_tempList regs = argregs;
        for (int i=0;i<F_keep && l;i++){
            emit(AS_Move("movq `s0 `d0", TL(regs->head, NULL), TL(munchExp(l->head), NULL)));
            l = l->tail; regs = regs->tail;
        }
        munchArgs(l, FALSE);
    }else{
        if (l){
            munchArgs(l->tail, FALSE);
            emit(AS_Oper("pushq `s0", NULL, TL(munchExp(l->head), NULL), AT(NULL)));
        }
    }
    
}