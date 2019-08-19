#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<stdbool.h>

struct block_end_h{
    int block_num;
    int s_time;
    double max_end_h1;
    double min_end_h1;
    double max_end_h2;
    double min_end_h2;
    double pass_max_h1_qin;
    double pass_min_h1_qin;
    double pass_max_h2_qin;
    double pass_min_h2_qin;
    double pass_fi;
};
typedef struct block_end_h  Block;

struct dp_table{
    int is_Switch;
    Block dp_block;
};
typedef struct dp_table Dp_table;

void build_output_table(int);
Block init_block(void);
Block cp_block(Block);
void print_block(Block);
void build_case_list(int);
Block system_cal_no_switch(double,double,double,int,int);
Block system_cal_switch(double,double,double,int,int);
bool tank_max_fi(double,int,int);
double get_min_fi(double,double);
void output_2tank_h(FILE *,Block [],int,int,int);
void simulat_system(void);
