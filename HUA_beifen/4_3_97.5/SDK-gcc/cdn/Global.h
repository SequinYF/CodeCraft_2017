//
// Created by shiyi on 2017/3/31.
//

#ifndef CDN_GLOBAL_H
#define CDN_GLOBAL_H

#define INF                 999999
#define MAX_COST            1000
#define NODE_MAXNUM         1009            //网络节点数量最大值
#define EDGE_MAXNUM         50009           //最大链路数量
#define COST_MAXNUM         509             //消费节点数量最大值

#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib_io.h"

struct CostEdge
{
    int cost_id;        //消费节点id
    int node_id;        //供应节点id
    int flow_need;      //流量需求
};

struct EDGE
{
    int cost;
    int cap, u, v;
    int next;
    int pre_cap;
};

struct Global
{
    int start_time;

    EDGE tedge[EDGE_MAXNUM];					//网络流初始邻接表
    int tedgeNum;							//初始邻接表边总个数
    int thead[NODE_MAXNUM];					//网络流邻接表头指针

    //输入数据
    int netNodeNum;       					//网络节点数量
    int netEdgeNum;       					//网络链路数量
    int costNodeNum;      					//消费节点数量
    int sumFlow;                            //总消费流量大小
    EDGE edge[EDGE_MAXNUM];					//网络流初始邻接表
    int edgeNum;							//初始邻接表边总个数
    int head[NODE_MAXNUM];					//网络流邻接表头指针
    CostEdge costEdge[COST_MAXNUM];			//保存消费节点信息
    int videoCost;							//视频服务器部署成本
    int hashNode[NODE_MAXNUM];					//hash消费节点虚拟id
    int capSum[NODE_MAXNUM];                //每个点最大流量

//输出数据
    int now_flow;
    int dfs_tmp[NODE_MAXNUM];
    bool dfs_vis[NODE_MAXNUM];
    char resultStr[NODE_MAXNUM*COST_MAXNUM];  //保存最终结果
    int resultLen;
    std::vector<int> routeList[NODE_MAXNUM*22];
    int routeListNum;

    void init();

    void input_file(char* topo[MAX_EDGE_NUM], int line_num);

    int get_time();

    void add_edge(int u, int v, int cap, int cost)
    {
        edge[edgeNum].u = u;
        edge[edgeNum].v = v;
        edge[edgeNum].cap = cap;
        edge[edgeNum].pre_cap = cap;
        edge[edgeNum].cost = cost;
        edge[edgeNum].next = head[u];
        head[u] = edgeNum++;

        edge[edgeNum].u = v;
        edge[edgeNum].v = u;
        edge[edgeNum].cap = 0;
        edge[edgeNum].cost = -cost;
        edge[edgeNum].next = head[v];
        head[v] = edgeNum++;
    }

    int dfs_getPath(int from, int depth, int cap);

    void out_goodResult(bool* video_list);

    void out_badResult();




    static Global* get(){
        static Global instance;
        return &instance;
    }

};


#endif //CDN_GLOBAL_H
