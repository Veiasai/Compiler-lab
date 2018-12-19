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
	char * fp_fix[100];
	int fp_fix_num = 0;
	int fp_fix_off[100];

	Temp_map F_tempMap;
	G_graph flow_graph;
	struct Live_graph live_graph;
	struct COL_result color;
	while(1){
		flow_graph = FG_AssemFlowGraph(il, f);
		live_graph = Live_liveness(flow_graph);
		color = COL_color(live_graph.graph, F_tempMap, NULL, live_graph.moves);;
		
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

						char *inst = checked_malloc(MAXLINE*(sizeof(char)));
						fp_fix[fp_fix_num] = inst;
						fp_fix_off[fp_fix_num++] = f->local_count;
						sprintf(inst, "movq `s0, %s(%%rsp)", Temp_labelstring(f->label));
						AS_instr store = AS_Oper(String(inst), NULL, Temp_TempList(temp, NULL), NULL);

						instrs->tail = AS_InstrList(store, instrs->tail);
					} else if (hasTemp(instr->u.OPER.src, spill)) {  
						Temp_temp temp = Temp_newtemp();
						replaceTemp(instr->u.OPER.src, spill, temp);  
						char *inst = checked_malloc(MAXLINE*(sizeof(char)));
						fp_fix[fp_fix_num] = inst;
						fp_fix_off[fp_fix_num++] = - f->local_count;
						sprintf(inst, "movq %s(%%rsp), `d0", Temp_labelstring(f->label));
						AS_instr fetch = AS_Oper(String(inst), Temp_TempList(temp, NULL), NULL, NULL);

						instrs->head = fetch;
						instrs->tail = AS_InstrList(instr, instrs->tail);
					}
				}
			}
		}
	}

	int framesize = f->local_count * F_wordSize;
	for (int i=0;i<fp_fix_num;i++){
		if (fp_fix_off[i] > 0)
			sprintf(fp_fix[i], "movq `s0, %d(%%rsp)", (- (fp_fix_off[i]-1) * F_wordSize) + framesize);
		else
			sprintf(fp_fix[i], "movq %d(%%rsp), `d0", ((fp_fix_off[i]+1) * F_wordSize) + framesize);
	}
	struct RA_result ret;
	ret.coloring = color.coloring;
	ret.il = il;
	return ret;
}
