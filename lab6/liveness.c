#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "absyn.h"
#include "assem.h"
#include "frame.h"
#include "graph.h"
#include "flowgraph.h"
#include "liveness.h"
#include "table.h"


static bool dfs_live(G_nodeList nodes);
static Temp_tempList unionTempList(Temp_tempList a, Temp_tempList b);
static Temp_tempList subTempList(Temp_tempList a, Temp_tempList b);
static bool isEqual(Temp_tempList a, Temp_tempList b);
static bool inTempList(Temp_tempList a, Temp_temp t);
static bool inMoveList(Live_moveList a, G_node src, G_node dst);

static G_table inTab, outTab;

Live_moveList Live_MoveList(G_node src, G_node dst, Live_moveList tail) {
	Live_moveList lm = (Live_moveList) checked_malloc(sizeof(*lm));
	lm->src = src;
	lm->dst = dst;
	lm->tail = tail;
	return lm;
}


Temp_temp Live_gtemp(G_node n) {
	return G_nodeInfo(n);
}

struct Live_graph Live_liveness(G_graph flow) {
	struct Live_graph lg;

	// a good idea from Jzy, G_table acts as std::map 
	inTab = G_empty();
	outTab = G_empty();
	
	for (G_nodeList nodes = G_nodes(flow); nodes; nodes = nodes->tail) {
		G_enter(inTab, nodes->head, checked_malloc(sizeof(Temp_tempList)));
		G_enter(outTab, nodes->head, checked_malloc(sizeof(Temp_tempList)));
	}

	while(dfs_live(G_nodes(flow)));
	
	// add conflict edge
	lg.graph = G_graph();
	TAB_table temp_to_node = TAB_empty();

	for (G_nodeList nodes = G_nodes(flow); nodes; nodes = nodes->tail) {
		G_node n = nodes->head;

		if (!FG_isMove(n)) {
			Temp_tempList outhead = *(Temp_tempList *)G_look(outTab, n);
			for (Temp_tempList def = FG_def(n); def; def = def->tail) {
				G_node a = TAB_look(temp_to_node, def->head);
				if (a == NULL){
					a = G_Node(lg.graph, def->head);
					TAB_enter(temp_to_node, def->head, a);
				}

				for (Temp_tempList out = outhead; out; out = out->tail) {
					if (out->head == def->head)
						continue;
					
					G_node b = TAB_look(temp_to_node, out->head);
					if (b == NULL){
						b = G_Node(lg.graph, def->head);
						TAB_enter(temp_to_node, def->head, b);
					}

					if (!G_inNodeList(a, G_adj(b))) {
						G_addEdge(a, b);
						G_addEdge(b, a);
					}
				}
			}
		} else {
			Temp_tempList use = FG_use(n);
			Temp_tempList outhead = *(Temp_tempList *)G_look(outTab, n);

			for (Temp_tempList def = FG_def(n); def; def = def->tail) {
				G_node a = TAB_look(temp_to_node, def->head);
				if (a == NULL){
					a = G_Node(lg.graph, def->head);
					TAB_enter(temp_to_node, def->head, a);
				}

				for (Temp_tempList out = outhead; out; out = out->tail) {
					if (out->head == def->head) 
						continue;
					
					G_node b = TAB_look(temp_to_node, out->head);
					if (b == NULL){
						b = G_Node(lg.graph, def->head);
						TAB_enter(temp_to_node, def->head, b);
					}

					if (!G_inNodeList(a, G_adj(b)) && !inTempList(use, out->head)) {
						G_addEdge(a, b);
						G_addEdge(b, a);
					}
				}

				for (Temp_tempList out = use; out; out = out->tail) {
					if (out->head == def->head)
						continue;
					
					G_node b = TAB_look(temp_to_node, out->head);
					if (b == NULL){
						b = G_Node(lg.graph, def->head);
						TAB_enter(temp_to_node, def->head, b);
					}

					if (!inMoveList(lg.moves, b, a)) {
						lg.moves = Live_MoveList(b, a, lg.moves);
					}
				}
			}
		}
	}

	return lg;
}

static bool dfs_live(G_nodeList nodes){
	bool res;
	if (nodes->tail && nodes->tail->head)
		res = dfs_live(nodes->tail);
	else
		return false;	// no change
	
	n = nodes->head;
	Temp_tempList in_old = *(Temp_tempList *)G_look(inTab, n);
	Temp_tempList out_old = *(Temp_tempList *)G_look(outTab, n);

	Temp_tempList in = NULL, out = NULL;

	G_nodeList succs = G_succ(n);
	for (; succs; succs = succs->tail) {
		Temp_tempList in_succ = *(Temp_tempList *)G_look(inTab, succs->head);
		out = unionTempList(out, in_succ);
	}

	in = unionTempList(FG_use(n), subTempList(out, FG_def(n)));

	bool cur_res = !isEqual(in_old, in) || !isEqual(out_old, out);

	*(Temp_tempList*)G_look(inTab, n) = in;
	*(Temp_tempList*)G_look(outTab, n) = out;

	return res || cur_res;
}


static Temp_tempList unionTempList(Temp_tempList a, Temp_tempList b) {
	Temp_tempList res = a;
	for (; b; b = b->tail)
		if (!inTempList(a, b->head)) 
			res = Temp_TempList(b->head, res);
	return res;
}

static Temp_tempList subTempList(Temp_tempList a, Temp_tempList b) {
	Temp_tempList res = NULL;
	for (; a; a = a->tail) 
		if (!inTempList(b, a->head)) 
			res = Temp_TempList(a->head, res);
	return res;
}

static bool isEqual(Temp_tempList a, Temp_tempList b) {
	Temp_tempList p = a;
	for (; p; p = p->tail) 
		if (!inTempList(b, p->head)) {
			return FALSE;

	p = b;
	for (; p; p = p->tail) {
		if (!inTempList(a, p->head)) {
			return FALSE;

	return true;
}
