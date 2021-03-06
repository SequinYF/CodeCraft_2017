#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <sys/timeb.h>

#define INF                 0x3f3f3f3f
#define DATA_INF            100009
#define NODE_MAXNUM         1009            //网络节点数量最大值
#define EDGE_MAXNUM         50009           //最大链路数量
#define COST_MAXNUM         509             //消费节点数量最大值

using namespace std;

struct CostEdge
{
    int cost_id;        //消费节点id
    int node_id;        //供应节点id
    int flow_need;      //流量需求
};

struct EDGE
{
    int cost, cap, v;
    int next;
    int pre_cap;
};

void deploy_server(char * graph[MAX_EDGE_NUM], int edge_num, char * filename);

void parse_file(char* topo[MAX_EDGE_NUM], int line_num);

void add_edge(int u, int v, int cap, int cost);

int get_time();

void out_badResult();

void out_goodResult(bool* video_list);

void init_ITEM();

void solve();

struct ZKW{
    void init(EDGE* edge, int edge_num, int node_num, int* head, int src, int des, bool* video_list){

        this->ans_cost = this->ans_flow = 0;

        memcpy(ZKW::edge, edge, sizeof(ZKW::edge));
        memcpy(ZKW::head, head, sizeof(ZKW::head));
        memset(dis, 0, sizeof(dis));
        this->edge_num = edge_num;
        this->node_num = node_num;
        this->src = src;
        this->des = des;

        //设置超级源
        for(int i=0; i<ZKW::node_num-2; ++i)
        {
            if(video_list[i]) {
                add_edge(ZKW::src, i, INF, 0);
            }
        }
    }

    int dfs(int now_node, int max_f)
    {
        if(now_node == ZKW::des || max_f == 0) {
            ans_flow += max_f;
            return max_f;
        }
        int used = 0;
        vis[now_node] = true;
        for(int i=head[now_node]; i != -1; i=edge[i].next){
            if(edge[i].cap && dis[edge[i].v]+edge[i].cost==dis[now_node] && !vis[edge[i].v]){

                int f = dfs(edge[i].v, min(edge[i].cap, max_f-used));

                ans_cost += f*edge[i].cost;
                edge[i].cap -= f;
                edge[i^1].cap += f;
                used += f;
                if(used == max_f)
                    break;
            }
        }
        return used;
    }

    void add_edge(int u, int v, int cap, int cost)
    {
        edge[edge_num].v = v;
        edge[edge_num].cap = cap;
        edge[edge_num].pre_cap = cap;
        edge[edge_num].cost = cost;
        edge[edge_num].next = head[u];
        head[u] = edge_num++;

        edge[edge_num].v = u;
        edge[edge_num].cap = 0;
        edge[edge_num].cost = -cost;
        edge[edge_num].next = head[v];
        head[v] = edge_num++;
    }

    bool change()
    {
        int delta = INF;
        for(int i=0; i<node_num; i++){
            if(vis[i])
                for(int j=head[i]; j!=-1; j=edge[j].next){
                    if(!vis[edge[j].v] && edge[j].cap)
                        delta = min(delta, dis[edge[j].v]+edge[j].cost-dis[i]);
                }
        }

        if(delta == INF)
            return false;
        for(int i=0; i<node_num; ++i) {
            if(vis[i])
                dis[i] += delta;
        }
        return true;
    }

    void costflow()
    {
        do{
            do{
                memset(vis, 0, sizeof(vis));
            }while(dfs(src, INF));
        }while(change());
    }

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

    static ZKW zkw;
};

//个体
struct ITEM{

    void init() {
        for(int i=0; i<ITEM::node_num; i++){
            this->video_list[i] = rand()%2;
        }
    }

    void assess(){

        ZKW::zkw.init(ITEM::edge, ITEM::edge_num, ITEM::node_num+2, ITEM::head, ITEM::node_num, ITEM::node_num+1, this->video_list);
        ZKW::zkw.costflow();

        int num = 0;
        for(int i=0; i<node_num; i++)
            if(video_list[i])
                ++num;
        this->cost = ZKW::zkw.ans_cost + num*video_cost;
        this->flow = ZKW::zkw.ans_flow;

        if(this->flow != ITEM::sum_flow || ZKW::zkw.ans_cost == 0)
            this->cost = INF;
    }

    void variation(int f) {
        int num = rand() % 100;
        if (num > f)
            return;

        num = rand() % ITEM::node_num;
        this->video_list[num] ^= 1;
    }

    bool video_list[NODE_MAXNUM];
    int cost;
    int flow;

    static EDGE* edge;
    static int* head;
    static int node_num;
    static int edge_num;
    static int video_cost;
    static int sum_flow;
};

//遗传算法
struct GA
{
    GA(){
        GA_ITERATIONS = 3000;   //迭代次数
        GA_ITEM_NUM = 300;       //最大个体数
        GA_VATRIATION = 100;         //变异概率???
        flag = true;
    }

    //种群初始化
    void init(){
        for(int i=0; i<GA_ITEM_NUM; i++){
            group[flag][i].init();
        }
    }

    //评估
    void assess()
    {
        for(int i=0; i<GA_ITEM_NUM; i++)
            group[flag][i].assess();
    }

    void cross_item(ITEM& boy, ITEM& girl, ITEM& father, ITEM& mother){
        int left = rand()%ITEM::node_num;
        int right = rand()%ITEM::node_num;
        if(left > right)
            swap(left, right);

        for(int i=0; i<left; i++){

            boy.video_list[i] = father.video_list[i];
            girl.video_list[i] = mother.video_list[i];
        }
        for(int i=left; i<=right; i++){
            boy.video_list[i] = mother.video_list[i];
            girl.video_list[i] = father.video_list[i];
        }
        for(int i=right+1; i<ITEM::node_num; i++){
            boy.video_list[i] = father.video_list[i];
            girl.video_list[i] = mother.video_list[i];
        }
        //提前判定？？？
        boy.cost = girl.cost = INF;
    }

    //交叉
    void cross(){
        bool new_flag = flag^1;
        int len = GA_ITEM_NUM/2;

        for(int i=0; i<len; i++){
            if(group[flag][i+1].cost == INF) {
                for(int j=i*2; j<GA_ITEM_NUM; j++){
                    group[new_flag][j].init();
                }
                break;
            }
            cross_item(group[new_flag][i*2], group[new_flag][i*2+1], group[flag][i], group[flag][i+1]);
        }
        this->flag = new_flag;
    }

    //变异
    void variation(){
        for (int i = 0; i < GA_ITEM_NUM; ++i) {
            group[flag][i].variation(100);
        }
    }

    void sort(){
        for (int i = 0; i < GA_ITEM_NUM; ++i) {
            for (int j = i + 1; j < GA_ITEM_NUM; ++j) {
                if (group[flag][i].cost > group[flag][j].cost)
                    swap(group[flag][i], group[flag][j]);
            }
        }
    }

    //执行进化
    void work(){

        init();
        best_item.cost = INF;
        best_iter = 0;

        for(int i=0; i<GA_ITERATIONS; ++i){

            assess();
            sort();

            if(best_item.cost > group[flag][0].cost){
                best_item = group[flag][0];
                best_iter = i;
            }

            if(i > best_iter+150) {
//                printf("提前停止\n");
                break;
            }

            if(get_time()-GA::start_time > 80)
                break;

//            if(i > best_iter+100) {
//                GA_VATRIATION = 200;
//                GA_VATRIATION += 100;
//                GA_VATRIATION = min(GA_VATRIATION, 200);
//                printf("增加变异概率\n");
//                break;
//            }

            cross();
            variation();

//            printf("第%d代： cost = %d  flow = %d\n", i+1, best_item.cost, best_item.flow);
        }
    }

    bool flag;              //辅助变量
    ITEM group[2][1000];
    ITEM best_item;
    int best_iter;
    int GA_ITERATIONS;
    int GA_ITEM_NUM;
    int GA_VATRIATION;
    static int start_time;
};

#endif
