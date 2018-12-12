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

	G_nodeList nodes = G_nodes(flow);

	// a good idea from Jzy, G_table acts as std::map 
	inTab = G_empty();
	outTab = G_empty();
	
	for (; nodes; nodes = nodes->tail) {
		G_enter(inTab, nodes->head, checked_malloc(sizeof(Temp_tempList)));
		G_enter(outTab, nodes->head, checked_malloc(sizeof(Temp_tempList)));
	}

	nodes = G_nodes(flow);
	while(dfs_live(nodes));
	
	
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

	*(Temp_tempList*)G_look(inTab, n) = in;
	*(Temp_tempList*)G_look(outTab, n) = out;

	return res || (!isEqual(in_old, in) || !isEqual(out_old, out));
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
