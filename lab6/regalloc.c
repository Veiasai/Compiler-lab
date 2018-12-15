#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "absyn.h"
#include "assem.h"
#include "frame.h"
#include "graph.h"
#include "liveness.h"
#include "color.h"
#include "regalloc.h"
#include "table.h"
#include "flowgraph.h"


#define MAXLINE 100

static char * postfix = "size";

static bool hasTemp(Temp_tempList list, Temp_temp temp) {
    for (; list; list = list->tail)
        if (list->head == temp)
            return 1;
    return 0;
}

static void replaceTemp(Temp_tempList list, Temp_temp old, Temp_temp new_) {
    for (; list; list = list->tail)
        if (list->head == old)
            list->head = new_;
}

struct RA_result RA_regAlloc(F_frame f, AS_instrList il) {
	//your code here
	Temp_map F_tempMap;
	while(1){
		G_graph flow_graph = FG_AssemFlowGraph(il, f);
		struct Live_graph live_graph = Live_liveness(flow_graph);
		struct COL_result color = COL_color(live_graph.graph, F_tempMap, NULL, live_graph.moves);;
		
		if (color.spills == NULL)
			break;
	
		for (Temp_tempList spills = color.spills; spills; spills = spills->tail) {
			Temp_temp spill = spills->head;
			f->local_count += 1;
			
			AS_instrList instrs = il;
			for (; instrs; instrs = instrs->tail) {
				AS_instr instr = instrs->head;

				if (instr->kind == I_OPER || instr->kind == I_MOVE) {
					if (hasTemp(instr->u.OPER.dst, spill)) {  
						Temp_temp temp = Temp_newtemp();
						replaceTemp(instr->u.OPER.dst, spill, temp);  

						char inst = checked_malloc(MAXLINE*(sizeof(char)));
						sprintf(inst, "movl `s0, %s%s(%%rsp)", f->label, postfix);
						AS_instr store = AS_Oper(String(inst), NULL, Temp_TempList(temp, NULL), NULL);

						instrs->tail = AS_InstrList(store, instrs->tail);
					} else if (hasTemp(instr->u.OPER.src, spill)) {  
						Temp_temp temp = Temp_newtemp();
						replaceTemp(instr->u.OPER.src, spill, temp);  
						char inst = checked_malloc(MAXLINE*(sizeof(char)));
						sprintf(inst, "movl %s%s(%%rsp), `d0", f->label, postfix);
						AS_instr fetch = AS_Oper(String(inst), Temp_TempList(temp, NULL), NULL, NULL);

						instrs->head = fetch;
						instrs->tail = AS_InstrList(instr, instrs->tail);
					}
				}
			}
		}
	}
	struct RA_result ret;
	return ret;
}
