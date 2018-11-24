#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "table.h"
#include "tree.h"
#include "frame.h"

/*Lab5: Your implementation here.*/

//varibales
struct F_access_ {
	enum {inFrame, inReg} kind;
	union {
		int offset; //inFrame
		Temp_temp reg; //inReg
	} u;
};

struct F_frame_ {
	F_accessList formals, locals;
	int local_count;
	Temp_label label;
};

static F_access InFrame(int offset);
static F_access InReg(Temp_temp reg);

const int F_wordSize = 8;		//x86: 64bit
static const int F_keep = 6;	//number of parameters kept in regs;


/* function implementation */
F_accessList F_AccessList(F_access head, F_accessList tail) {
	F_accessList l = checked_malloc(sizeof(*l));
	l->head = head;
	l->tail = tail;
	return l;
}

// miss
Temp_temp F_FP() {
	static Temp_temp t = NULL;
	if (!t)
		t = Temp_newtemp();
	return t;
}

// miss
Temp_temp F_RV() {
	static Temp_temp t = NULL;
	if (!t)
		t = Temp_newtemp();
	return t;
}

F_frame F_newFrame(Temp_label name, U_boolList formals) {
	F_frame fr = checked_malloc(sizeof(*fr));
	fr->label = name;		
	fr->formals = NULL;
	fr->locals = NULL;
	fr->local_count = 0;

	F_accessList head = NULL, tail = NULL;

	// register count
	int rn = 0, fn = 0;
	U_boolList ptr;
	for (ptr = formals; ptr; ptr = ptr->tail) {
		F_access ac = NULL;
		if (rn < F_keep && !(ptr->head)) {
			ac = InReg(Temp_newtemp());	
			rn++;
		} else {
			fn++;
			ac = InFrame((fn+1)*F_wordSize);	//one word for return address
		}

		if (head) {
			tail->tail = F_AccessList(ac, NULL);
			tail = tail->tail;
		} else {
			head = F_AccessList(ac, NULL);
			tail = head;
		}
	}
	fr->formals = head;

	return fr;
}

Temp_label F_name(F_frame f) {
	return f->label;
}

F_accessList F_formals(F_frame f) {
	return f->formals;
}

F_access F_allocLocal(F_frame f, bool escape) {
	f->local_count++;
	if (escape) 
		return InFrame(-F_wordSize * (f->local_count));
	else 
		return InReg(Temp_newtemp());
}

// when the acc is from an inner-nested function, it's calculated by static link
T_exp F_Exp(F_access acc, T_exp framePtr) {
	if (acc->kind == inReg)
		return T_Temp(acc->u.reg);
	else
		return T_Mem(T_Binop(T_plus, T_Const(acc->u.offset), framePtr));
}

T_exp F_externalCall(string s, T_expList args) {
	return T_Call(T_Name(Temp_namedlabel(s)), args);
}

static F_access InFrame(int offset) {
	F_access ac = checked_malloc(sizeof(*ac));
	ac->kind = inFrame;
	ac->u.offset = offset;
	return ac;
}

static F_access InReg(Temp_temp reg) {
	F_access ac = checked_malloc(sizeof(*ac));
	ac->kind = inReg;
	ac->u.reg = reg;
	return ac;
}

/* fragment */

F_frag F_StringFrag(Temp_label label, string str) {
	F_frag frag = checked_malloc(sizeof(*frag));
	frag->kind = F_stringFrag;
	frag->u.stringg.label = label;
	frag->u.stringg.str = str;
	return frag;
}

F_frag F_ProcFrag(T_stm body, F_frame frame) {
	F_frag frag = checked_malloc(sizeof(*frag));
	frag->kind = F_procFrag;
	frag->u.proc.body = body;
	frag->u.proc.frame = frame;
	return frag;
}

F_fragList F_FragList(F_frag head, F_fragList tail) {
	F_fragList fl = checked_malloc(sizeof(*fl));
	fl->head = head;
	fl->tail = tail;
	return fl;
}                                             

