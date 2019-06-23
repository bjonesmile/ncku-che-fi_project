#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<stdbool.h>

//limit+ fi=0.463

struct block_end_h{
    int block_num;
    double max_end_h;
    double min_end_h;
    double pass_max_qout;
    double pass_min_qout;
    double pass_fi;
};
typedef struct block_end_h  Block;
//func list
void print_block(Block);


Block cal_h(double init_h,double fi,int dir)
{
    printf("input fi=%lf\n",fi);
    int time,i;
    int block_num1,block_num2,block_num3;
    double Ar=5;
    int t1;
    double h1,h2,h3,h4;
    double max_h=-1000,min_h=1000;
    double qout1,qout2,qout3,qout4;
    double max_last_h=9.999,min_last_h=1.001;
    double theta_plus,theta_minus,theta;
    double min_h1=1000,max_h1=-1000;
    double max=-1000,min=1000;
    int qout_long=200;
    int max_t1,min_t1;
    double sub_value=9;
    
    double case1_min_h1[200]={0};
    double case1_max_h1[200]={0};
    double pass_max_qout[200]={0};
    double pass_min_qout[200]={0};
    
    Block block_return;
    block_return.block_num=-1;
    block_return.max_end_h=-1;
    block_return.min_end_h=-1;
    block_return.pass_max_qout=-1;
    block_return.pass_min_qout=-1;
    block_return.pass_fi=-1;
    
    theta_plus=0.5+fi*0.5;
    theta_minus=0.5-fi*0.5;
    
    for(time=0;time<200;time++)
    if(dir>0)
    {
        t1=time;
        theta=theta_plus;
        min_h1=1000;
        max_h1=-1000;
            for(qout1=0;qout1<=0.7;qout1+=0.0001)
            {
                theta=theta_plus;
                h1=init_h+(theta-qout1)*t1/Ar+(0.5-qout1)/Ar;
                if(h1<1||h1>10)
                    continue;
                theta=theta_minus;
                h1=h1+(theta-qout1)*(199-t1)/Ar;
                
                if(!(h1>=1&&h1<=10))
                    continue;
                else{
                    if(h1<min_h1){
                        min_h1=h1;
                        case1_min_h1[t1]=min_h1;
                        pass_min_qout[t1]=qout1; }
                    
                    if(h1>max_h1){
                        max_h1=h1; 
                        case1_max_h1[t1]=max_h1;
                        pass_max_qout[t1]=qout1; }
                }
            }
            if(min_h1>=10||min_h1<=1||max_h1>=10||max_h1<=1)
                {printf("fi=%lf cant pass at t1=%d for init h=%lf\n",fi,t1,init_h); return block_return;}

    }else{
        t1=time;
        theta=theta_minus;
        min_h1=1000;
        max_h1=-1000;
            for(qout1=0;qout1<=0.7;qout1+=0.0001)
            {
                theta=theta_minus;
                h1=init_h+(theta-qout1)*t1/Ar+(0.5-qout1)/Ar;
                if(h1<1||h1>10)
                    continue;
                theta=theta_plus;
                h1=h1+(theta-qout1)*(199-t1)/Ar;
                
                if(!(h1>=1&&h1<=10))
                    continue;
                else{
                    if(h1<min_h1){
                        min_h1=h1;
                        case1_min_h1[t1]=min_h1;
                        pass_min_qout[t1]=qout1; }
                    
                    if(h1>max_h1){
                        max_h1=h1; 
                        case1_max_h1[t1]=max_h1;
                        pass_max_qout[t1]=qout1; }
                }
            }
            if(min_h1>=10||min_h1<=1||max_h1>=10||max_h1<=1)
                {printf("fi=%lf cant pass at t1=%d for init h=%lf\n",fi,t1,init_h); return block_return;}
    }
    
    for(i=0;i<200;i++){
        printf("t1:%d\n",i);
        printf("min h1=%lf\n",case1_min_h1[i]);
        printf("max h1=%lf\n",case1_max_h1[i]);
        if((case1_max_h1[i]-case1_min_h1[i])<=sub_value)
        {
           sub_value=case1_max_h1[i]-case1_min_h1[i];
           max_t1=i; max=case1_max_h1[i];
           min_t1=i; min=case1_min_h1[i];
        }
        /*if(case1_max_h1[i]>max)
            {max_t1=i; max=case1_max_h1[i];}
        if(case1_min_h1[i]<min)
            {min_t1=i; min=case1_min_h1[i];}*/
    }
    printf("t1=%d max:%lf\nt1=%d min:%lf\n",max_t1,max,min_t1,min);
    
    block_return.block_num=5;
    block_return.max_end_h=max;
    block_return.min_end_h=min;
    block_return.pass_max_qout=pass_max_qout[max_t1];
    block_return.pass_min_qout=pass_min_qout[min_t1];
    block_return.pass_fi=fi;
    
    /*print_block(block_return);
    getchar();*/
    
    return block_return;
}

Block cal_h_no_swift(double init_h,double fi,int dir)
{
    printf("input fi=%lf\n",fi);
    int time,i;
    int block_num1,block_num2,block_num3;
    double Ar=5;
    double h1,h2,h3,h4;
    double max_h=-1000,min_h=1000;
    double qout1,qout2,qout3,qout4;
    double max_last_h=9.999,min_last_h=1.001;
    double theta_plus,theta_minus,theta;
    double min_h1=1000,max_h1=-1000;
    double max=-1000,min=1000;
    int qout_long=200;
    int max_t1,min_t1;
    
    double case1_min_h1[200]={0};
    double case1_max_h1[200]={0};
    double pass_qout[200]={0};
    double pass_qout_max,pass_qout_min;
    
    Block block_return;
    block_return.block_num=-1;
    block_return.max_end_h=-1;
    block_return.min_end_h=-1;
    block_return.pass_max_qout=-1;
    block_return.pass_min_qout=-1;
    block_return.pass_fi=-1;
    
    theta_plus=0.5+fi*0.5;
    theta_minus=0.5-fi*0.5;
    
    if(dir>0)
    {
        theta=theta_plus;
        min_h1=1000;
        max_h1=-1000;
            for(qout1=0;qout1<=0.7;qout1+=0.0001)
            {
                h1=init_h+(theta-qout1)*200/Ar;
                
                if(!(h1>=1&&h1<=10))
                    continue;
                else{
                    //printf("qout=%lf h1=%lf\n",qout1,h1);
                    if(h1<min_h1){
                        min_h1=h1;
                        pass_qout_min=qout1;
                        }
                    
                    if(h1>max_h1){
                        max_h1=h1;
                        pass_qout_max=qout1;
                        }
                }
            }
            if(min_h1>=10||min_h1<=1||max_h1>=10||max_h1<=1)
                {printf("fi=%lf cant pass at init h=%lf\n",fi,init_h); return block_return;}
    }else{
        theta=theta_minus;
        min_h1=1000;
        max_h1=-1000;
            for(qout1=0;qout1<=0.7;qout1+=0.0001)
            {
                h1=init_h+(theta-qout1)*200/Ar;               
                
                if(!(h1>=1&&h1<=10))
                    continue;
                else{
                    //printf("qout=%lf h1=%lf\n",qout1,h1);
                    if(h1<min_h1){
                        min_h1=h1;
                        pass_qout_min=qout1;
                        }
                    
                    if(h1>max_h1){
                        max_h1=h1;
                        pass_qout_max=qout1;
                        }
                }
            }
            if(min_h1>=10||min_h1<=1||max_h1>=10||max_h1<=1)
                {printf("fi=%lf cant pass at init h=%lf\n",fi,init_h); return block_return;}
    }
    printf("min h1=%lf ",min_h1);
    printf("min qout=%lf\n",pass_qout_min);
    printf("max h1=%lf ",max_h1);
    printf("max qout=%lf\n",pass_qout_max);
    //printf("max:%lf\nmin:%lf\n",max_h1,min_h1);
    
    block_return.block_num=6;
    block_return.max_end_h=max_h1;
    block_return.min_end_h=min_h1;
    block_return.pass_max_qout=pass_qout_max;
    block_return.pass_min_qout=pass_qout_min;
    block_return.pass_fi=fi;
    //for debug
    /*print_block(block_return);
    getchar();*/
    
    return block_return;
}

void print_block(Block input)
{
    printf("Block \n");
    printf("block_num: %d\n",input.block_num);
    printf("max_end_h: %lf\n",input.max_end_h);
    printf("min_end_h: %lf\n",input.min_end_h);
    printf("pass_max_qout: %lf\n",input.pass_max_qout);
    printf("pass_min_qout: %lf\n",input.pass_min_qout);
    printf("pass_fi: %lf\n",input.pass_fi);
}

bool tank_max_fi(double input_fi,int init_dir)
{
    int i,j;
    int redo_input_fi=0;
    int dir=init_dir;
    
    double fi=input_fi;
    double init_h=5,last_h;
    double last_max_h,last_min_h;
    double block1_init_h_max,block1_init_h_min,block1_init_h;
    
    bool all_case[4][4]={{1,0,0,0},
                         {0,1,0,0},
                         {0,0,1,0},
                         {0,0,0,1}};
    
    Block all_block[4];
    for(i=0;i<4;i++)
    {
        all_block[i].block_num=-1;
        all_block[i].max_end_h=-1;
        all_block[i].min_end_h=-1;
        all_block[i].pass_max_qout=-1;
        all_block[i].pass_min_qout=-1;
        all_block[i].pass_fi=-1;
    }  
    
    for(i=0;i<4;i++)
    {
        dir=init_dir;
        for(j=0;j<4;j++)
        {
            printf("case:%d block:%d switch:%d\n",i,j,all_case[i][j]);
            printf("dir=%d\n",dir);
            if(all_case[i][j]==1&&j==0)
            {
                all_block[j]=cal_h(init_h,fi,dir);
                dir=-dir;
            }
            if(all_case[i][j]==0&&j==0)
            {
                all_block[j]=cal_h_no_swift(init_h,fi,dir);
                printf("Case:%d Block:%d pass_fi=%lf\n",i,j,all_block[j].pass_fi);
                //for gebug
                //getchar();
            }
            if(all_case[i][j]==1&&j!=0)
            {
                if(dir>0)
                {
                    for(last_h=last_min_h;last_h<=last_max_h;last_h+=0.001)
                    {
                        all_block[j]=cal_h(last_h,fi,dir);
                        
                        if(all_block[j].pass_fi==-1)
                            continue;
                        else
                            break;
                    }
                }else{
                    for(last_h=last_max_h;last_h>=last_min_h;last_h-=0.001)
                    {
                        all_block[j]=cal_h(last_h,fi,dir);
                        
                        if(all_block[j].pass_fi==-1)
                            continue;
                        else
                            break;
                    }
                }                
                dir=-dir;
            }
            if(all_case[i][j]==0&&j!=0)
            {
                if(dir>0)
                {
                    for(last_h=last_min_h;last_h<=last_max_h;last_h+=0.001)
                    {
                        all_block[j]=cal_h_no_swift(last_h,fi,dir);
                        
                        if(all_block[j].pass_fi==-1)
                            continue;
                        else
                            break;
                    }
                }else{
                    for(last_h=last_min_h;last_h<=last_max_h;last_h+=0.001)
                    {
                        all_block[j]=cal_h_no_swift(last_h,fi,dir);
                        
                        if(all_block[j].pass_fi==-1)
                            continue;
                        else
                            break;
                    }
                }
            }
            
            if(all_block[j].pass_fi!=-1)
            {
                all_block[j].block_num=j;
                printf("Case:%d Block:%d pass_fi=%lf\n",i,j,all_block[j].pass_fi);
                last_max_h=all_block[j].max_end_h;
                last_min_h=all_block[j].min_end_h;
            }else{
                printf("error fi=%lf\n",fi);
                printf("at Case:%d Block:%d fi=%lf\n",i,j,fi);
                return 0;
            }
        }
        
        printf("pass Case:%d\n",i);
        for(j=0;j<4;j++)
        {
            print_block(all_block[j]);
        }
        
        for(j=0;j<4;j++)
        {
            all_block[j].block_num=-1;
            all_block[j].max_end_h=-1;
            all_block[j].min_end_h=-1;
            all_block[j].pass_max_qout=-1;
            all_block[j].pass_min_qout=-1;
            all_block[j].pass_fi=-1;
        }
        
    }
    
    return 1;
    //case0 block 1 0 0 0
}

double get_min_fi(double x1,double x2)
{
    if(x1>=x2){
        return x2;}
    else{
        return x1;}
}

int main(){
    bool is_inputSuc=0;

    int redo_input_fi=0;
    int loop_level=4;
    
    double f=1,reduce_f;
    double dir0_min_f,dir1_min_f,final_fi;
    double init_h,input_fi;
    double block1_init_h_max,block1_init_h_min,block1_init_h;
    
    Block output[4];
    clock_t start,end;
    
    printf("plz input start f for dir+\n");
    scanf(" %lf",&f);
    
    start=clock();
    
    while(loop_level>0){
    
        switch(loop_level){
            case 4:
                reduce_f=0.1;
                break;
            case 3:
                reduce_f=0.01;
                break;
            case 2:
                reduce_f=0.001;
                break;
            case 1:
                reduce_f=0.0001;
                break;
            default:
                printf("error loop  level\n");
        }
    
    while(is_inputSuc==0)
    {
        
        is_inputSuc=tank_max_fi(f,1);
        if(is_inputSuc)
            break;
        else
            f-=reduce_f;
    }
        printf("pass fi:%lf\n",f);
        if(loop_level!=0){
            f+=reduce_f;
            is_inputSuc=0;
            loop_level--;
        }else{
            break;
        }
    }
    
    dir1_min_f=f-0.0001;
    printf("dir+ min pass fi:%lf\n",dir1_min_f);
    
    //printf("dir+ pass fi:%lf\n",f);
    //printf("plz input start f for dir-\n");
    //scanf(" %lf",&f);
        
    loop_level=4;
    while(loop_level>0){
    
        switch(loop_level){
            case 4:
                reduce_f=0.1;
                break;
            case 3:
                reduce_f=0.01;
                break;
            case 2:
                reduce_f=0.001;
                break;
            case 1:
                reduce_f=0.0001;
                break;
            default:
                printf("error loop  level\n");
        }
    
    while(is_inputSuc==0)
    {
        
        is_inputSuc=tank_max_fi(f,-1);
        if(is_inputSuc)
            break;
        else
            f-=reduce_f;
    }
        printf("pass fi:%lf\n",f);
        if(loop_level!=0){
            f+=reduce_f;
            is_inputSuc=0;
            loop_level--;
        }else{
            break;
        }
    }
    
    dir0_min_f=f-0.0001;
    end=clock();
    
    printf("dir+ min pass fi:%lf\n",dir1_min_f);
    printf("dir- min pass fi:%lf\n",dir0_min_f);
    
    final_fi=get_min_fi(dir0_min_f,dir1_min_f);
    printf("final min fi:%lf\n",final_fi);
    printf("process time=%lf\n",(end-start)/(double)CLOCKS_PER_SEC);
    /*
    is_inputSuc=0;
    while(!is_inputSuc)
    {
        printf("plz guess input fi:\n");
        scanf(" %lf",&input_fi);
        is_inputSuc=tank_max_fi(input_fi,1);
        
    }*/
    

    return 0;
}
