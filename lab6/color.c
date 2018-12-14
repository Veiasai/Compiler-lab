#include <stdio.h>
#include <string.h>

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
#include "table.h"

#define K 14

static G_nodeList simplifyWorklist;
static G_nodeList freezeWorklist;
static G_nodeList spillWorklist;

static G_nodeList spilledNodes;
static G_nodeList coalescedNodes;
static G_nodeList coloredNodes;
static G_nodeList selectStack;

static Live_moveList coalescedMoves;
static Live_moveList constrainedMoves;
static Live_moveList frozenMoves;
static Live_moveList worklistMoves;
static Live_moveList activeMoves;

static Temp_tempList notSpillTemps;

static G_table degreeTab;
static G_table colorTab;
static G_table aliasTab;

struct COL_result COL_color(G_graph ig, Temp_map initial, Temp_tempList regs, Live_moveList moves) {
	// initial
	simplifyWorklist = NULL;
	freezeWorklist = NULL;
	spillWorklist = NULL;

	spilledNodes = NULL;
	coalescedNodes = NULL;
	coloredNodes = NULL;
	selectStack = NULL;

	coalescedMoves = NULL;
	constrainedMoves = NULL;
	frozenMoves = NULL;
	worklistMoves = moves;
	activeMoves = NULL;

	degreeTab = G_empty();
	colorTab = G_empty();
	aliasTab = G_empty();

	Build(ig);

	MakeWorklist(ig);

	while (simplifyWorklist || worklistMoves || freezeWorklist || spillWorklist) {
		if (simplifyWorklist) {
			Simplify();
		} else if (worklistMoves) {
			Coalesce();
		} else if (freezeWorklist) {
			Freeze();
		} else if (spillWorklist) {
			SelectSpill();
		}
	}

	AssignColors();
	struct COL_result ret;
	return ret;
}


static void Build(G_graph ig) {
	for (G_nodeList nodes = G_nodes(ig); nodes; nodes = nodes->tail) {
		int *degree = checked_malloc(sizeof(int));
		*degree = 0;

		// i think adj may return duplicate nodes .  ???
		for (G_nodeList cur = G_succ(nodes->head); cur; cur = cur->tail)
			(*degree)++;
		
		G_enter(degreeTab, nodes->head, degree);

		/* intial color */
		int *color = checked_malloc(sizeof(int));
		Temp_temp temp = Live_gtemp(nodes->head);
		*color = locate_register(temp);

		G_enter(colorTab, nodes->head, color);

		/* initial alias */
		G_node *alias = checked_malloc(sizeof(G_node));
		*alias = nodes->head;
		G_enter(aliasTab, nodes->head, alias);
	}
}

static int locate_register(Temp_temp temp) {
	if (temp == F_RAX()) return 1;
	else if (temp == F_RBX()) return 2;
	else if (temp == F_RCX()) return 3;
	else if (temp == F_RDX()) return 4;
	else if (temp == F_RSI()) return 5;
	else if (temp == F_RDI()) return 6;
	else if (temp == F_RBP()) return 7;
	else if (temp == F_RSP()) return 8;
	else if (temp == F_R8()) return 9;
	else if (temp == F_R9()) return 10;
	else if (temp == F_R10()) return 11;
	else if (temp == F_R11()) return 12;
	else if (temp == F_R12()) return 13;
	else if (temp == F_R13()) return 14;
	else if (temp == F_R14()) return 15;
	else if (temp == F_R15()) return 16;

	return 0;
}


static void MakeWorklist(G_graph ig) {
	for (G_nodeList nodes = G_nodes(ig); nodes; nodes = nodes->tail) {
		int *degree = G_look(degreeTab, nodes->head);
		int *color = G_look(colorTab, nodes->head);

		if (*color != 0) {
			continue;
		}

		if (*degree >= K) { /* spill */
			spillWorklist = G_NodeList(nodes->head, spillWorklist);
		} else if (MoveRelated(nodes->head)) { /* moveRelated */
			freezeWorklist = G_NodeList(nodes->head, freezeWorklist);
		} else { /* simplify */
			simplifyWorklist = G_NodeList(nodes->head, simplifyWorklist);
		}
	}
}

static void Simplify() {
	G_node n = simplifyWorklist->head;
	simplifyWorklist = simplifyWorklist->tail;
	selectStack = G_NodeList(n, selectStack);
	for (G_nodeList nodes = G_adj(n); nodes; nodes = nodes->tail) {
		DecrementDegree(nodes->head);
	}
}

static void DecrementDegree(G_node m) {
	int *degree = G_look(degreeTab, m);
	int d = *degree;
	(*degree)--;

	int *color = G_look(colorTab, m);
	if (d == K && *color == 0) {
		EnableMoves(G_NodeList(m, G_adj(m)));
		spillWorklist = G_subNodeList(spillWorklist, G_NodeList(m, NULL));
		if (MoveRelated(m)) {
			freezeWorklist = G_NodeList(m, freezeWorklist);
		} else {
			simplifyWorklist = G_NodeList(m, simplifyWorklist);
		}
	}
}