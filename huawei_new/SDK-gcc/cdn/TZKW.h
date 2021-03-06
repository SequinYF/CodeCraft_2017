//
// Created by Sequin on 2017/4/3.
//

#ifndef CDN_TZKW_H
#define CDN_TZKW_H


#include "Global.h"

struct TZKW{
    int ans_cost;
    int ans_flow;
    int edge_num;
    int node_num;
    int src;
    int des;
    EDGE edge[EDGE_MAXNUM];
    int head[NODE_MAXNUM];   //别忘记初始化
    int dis[NODE_MAXNUM];
    bool vis[NODE_MAXNUM];

    static TZKW* get(){
        static TZKW* instance = new TZKW;
        return instance;
    }

    void init(EDGE* edge, int edge_num, int node_num, int* head, int src, int des);

    int dfs(int now_node, int max_f);

    void add_edge(int u, int v, int cap, int cost);

    void work(int* video_list);

    bool change();

    void costflow();

};


#endif //CDN_TZKW_H
