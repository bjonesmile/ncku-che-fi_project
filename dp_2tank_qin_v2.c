#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<stdbool.h>
#include "dp_2tank_qin_v1.h"

bool is_debug;

const int Ar1=5;
const int Ar2=5;
const double R1=10;
const double R2=20;
const int z_limit=10;
int last_ErrorCase=-1;
int error_case_list[800]={0};

bool **case_list;
Block **output_table;

Block init_block(void)
{
    Block input;    
    
    input.block_num=-1;
    input.s_time=-1;
    input.max_end_h1=-1;
    input.min_end_h1=-1;
    input.max_end_h2=-1;
    input.min_end_h2=-1;
    input.pass_max_h1_qin=-1;
    input.pass_min_h1_qin=-1;
    input.pass_max_h2_qin=-1;
    input.pass_min_h2_qin=-1;
    input.pass_fi=-1;
    
    return input;
}

void build_output_table(int block_num)
{
    int i,j;
    int case_num=pow(2,block_num);
    
    output_table=(Block**)malloc(block_num*(sizeof(Block*)));
    for(i=0;i<block_num;i++)
    {
        output_table[i]=(Block*)malloc(block_num*(sizeof(Block)));
    }
    
    for(i=0;i<block_num;i++)
    {
        for(j=0;j<block_num;j++)
            output_table[i][j]=init_block();
    }
    
}

Block cp_block(Block input)
{
    Block output;    
    
    output.block_num=input.block_num;
    output.s_time=input.s_time;
    output.max_end_h1=input.max_end_h1;
    output.min_end_h1=input.min_end_h1;
    output.max_end_h2=input.max_end_h2;
    output.min_end_h2=input.min_end_h2;
    output.pass_max_h1_qin=input.pass_max_h1_qin;
    output.pass_min_h1_qin=input.pass_min_h1_qin;
    output.pass_max_h2_qin=input.pass_max_h2_qin;
    output.pass_min_h2_qin=input.pass_min_h2_qin;
    output.pass_fi=input.pass_fi;
    
    return output;
}

void print_block(Block input)
{
    printf("block_num:%d\n",input.block_num);
    printf("switch time:%d\n",input.s_time);
    printf("max_end_h1:%lf\n",input.max_end_h1);
    printf("min_end_h1:%lf\n",input.min_end_h1);
    printf("max_end_h2:%lf\n",input.max_end_h2);
    printf("min_end_h2:%lf\n",input.min_end_h2);
    printf("pass_max_h1_qin:%lf\n",input.pass_max_h1_qin);
    printf("pass_min_h1_qin:%lf\n",input.pass_min_h1_qin);
    printf("pass_max_h2_qin:%lf\n",input.pass_max_h2_qin);
    printf("pass_min_h2_qin:%lf\n",input.pass_min_h2_qin);
    printf("pass_fi:%lf\n",input.pass_fi);
}

void build_case_list(int block_num)
{
    int i,j,k;
    
    case_list=(bool**)malloc(block_num*(sizeof(bool*)));
    for(i=0;i<block_num;i++)
    {
        case_list[i]=(bool*)malloc(block_num*(sizeof(bool)));
    }
    
    for(k=0;k<block_num;k++)
    {
        for(j=0;j<block_num;j++)
        {
            if(j==k)
                case_list[k][j]=true;
            else
                case_list[k][j]=false;
        }
    }
    
    for(k=0;k<block_num;k++)
    {
        for(j=0;j<block_num;j++)
            printf("%d ",case_list[k][j]);
        printf("\n");
    }
    getchar();
}

Block system_cal_no_switch(double init_h1,double init_h2,double fi,int len,int dir)
{
    int i,t;
    double h1,h2,current_h1,current_h2;
    double theta,theta_plus,theta_minus;
    theta_plus=0.5+0.5*fi;
    theta_minus=0.5-0.5*fi;
    double qin,q1,q2;
    double max_h1_qin;
    double min_h1_qin;
    double max_h2_qin;
    double min_h2_qin;
    double max_h1=-1000,min_h1=1000,max_h2=-1000,min_h2=1000;
    
    Block block=init_block();

    if(dir>0)
        theta=theta_plus;
    else
        theta=theta_minus;

    for(qin=0;qin<=0.4;qin+=0.0001)
    {
        current_h1=init_h1;
        current_h2=init_h2;
    
        for(t=0;t<len;t++)
        {
            q1=current_h1/R1;
            q2=(current_h2-current_h1)/R2;
            h1=current_h1+(qin+q2-q1)/Ar1;
            h2=current_h2+(theta-q2-0.4)/Ar2;
            
            if(h1<1||h1>10||h2<1||h2>10)
            {
                current_h1=init_h1;
                current_h2=init_h2;

                break;
            }else{
                current_h1=h1;
                current_h2=h2;
            }
        }
        if(t==len)
        {         
            if(h1>=max_h1)
                {max_h1=h1; max_h1_qin=qin;}
            if(h1<=min_h1)
                {min_h1=h1; min_h1_qin=qin;}
            if(h2>=max_h2)
                {max_h2=h2; max_h2_qin=qin;}
            if(h2<=min_h2)
                {min_h2=h2; min_h2_qin=qin;}
        }
    }
    if(!(max_h2<=10&&max_h2>=1)||!(max_h1<=10&&max_h1>=1))
        return block;
    
    block.block_num=1;
    block.max_end_h1=max_h1;
    block.min_end_h1=min_h1;
    block.max_end_h2=max_h2;
    block.min_end_h2=min_h2;
    block.pass_max_h1_qin=max_h1_qin;
    block.pass_min_h1_qin=min_h1_qin;
    block.pass_max_h2_qin=max_h2_qin;
    block.pass_min_h2_qin=min_h2_qin;
    block.pass_fi=fi;
    
    return block;
}

Block system_cal_switch(double init_h1,double init_h2,double fi,int len,int dir)
{
    int i,t;
    int is_Suc=-1;
    int change_time;
    bool in_range=false;
    double h1,h2,current_h1,current_h2;
    double theta,theta_plus,theta_minus,theta_switch;
    theta_plus=0.5+0.5*fi;
    theta_minus=0.5-0.5*fi;
    double qin,q1,q2;
    double max_h1_qin;
    double min_h1_qin;
    double max_h2_qin;
    double min_h2_qin;
    double max_h1=-1000,min_h1=1000,max_h2=-1000,min_h2=1000;
    
    Block block=init_block();
    double *pass_end_max_h1=(double*)malloc(len*(sizeof(double)));
    double *pass_end_min_h1=(double*)malloc(len*(sizeof(double)));
    double *pass_h1_max_qin=(double*)malloc(len*(sizeof(double)));
    double *pass_h1_min_qin=(double*)malloc(len*(sizeof(double)));
    double *pass_end_max_h2=(double*)malloc(len*(sizeof(double)));
    double *pass_end_min_h2=(double*)malloc(len*(sizeof(double)));
    double *pass_h2_max_qin=(double*)malloc(len*(sizeof(double)));
    double *pass_h2_min_qin=(double*)malloc(len*(sizeof(double)));
    
    current_h1=init_h1;
    current_h2=init_h2;
    if(dir>0)
        {theta=theta_plus; theta_switch=theta_minus;}
    else
        {theta=theta_minus; theta_switch=theta_plus;}
    
    for(change_time=0;change_time<len;change_time++)
    {
        is_Suc=-1;
        in_range=false;
        max_h1=-1000;
        min_h1=1000;
        max_h2=-1000;
        min_h2=1000;
                  
        for(qin=0;qin<=0.4;qin+=0.0001)
        {
            current_h1=init_h1;
            current_h2=init_h2;    
            
            if(dir>0)
                theta=theta_plus;
            else
                theta=theta_minus;
        for(t=0;t<len;t++)
        {
            if(t==change_time){
                q1=current_h1/R1;
                q2=(current_h2-current_h1)/R2;
                h1=current_h1+(qin+q2-q1)/Ar1;
                h2=current_h2+(0.5-q2-0.4)/Ar2;
                
                theta=theta_switch;
            }else{
                q1=current_h1/R1;
                q2=(current_h2-current_h1)/R2;
                h1=current_h1+(qin+q2-q1)/Ar1;
                h2=current_h2+(theta-q2-0.4)/Ar2;
            }
            
            if(h1<1||h1>10||h2<1||h2>10)
            {
                current_h1=init_h1;
                current_h2=init_h2;
                
                break;
            }else{
                current_h1=h1;
                current_h2=h2;
            }
        }//t loop end
        if(in_range==true&&t!=(len))
            break;
        
        if(t==len)
        {
            if(in_range==false)
                in_range=true;
            is_Suc=1;
            
            if(h1>=max_h1)
                {max_h1=h1;
                 pass_end_max_h1[change_time]=h1;
                 pass_h1_max_qin[change_time]=qin;}
            if(h1<=min_h1)
                {min_h1=h1;
                 pass_end_min_h1[change_time]=h1;
                 pass_h1_min_qin[change_time]=qin;}
            if(h2>=max_h2)
                {max_h2=h2;
                 pass_end_max_h2[change_time]=h2;
                 pass_h2_max_qin[change_time]=qin;}
            if(h2<=min_h2)
                {min_h2=h2;
                 pass_end_min_h2[change_time]=h2;
                 pass_h2_min_qin[change_time]=qin;}
            //getchar();
        }
        if(qin>0.4)
        {
            getchar();
        }
        
        }//qin loop end
        if(is_Suc!=1){
            printf("fi=%lf cant pass at change time=%d\n",fi,change_time);
            free(pass_end_max_h1);
            free(pass_end_min_h1);
            free(pass_h1_max_qin);
            free(pass_h1_min_qin);
            free(pass_end_max_h2);
            free(pass_end_min_h2);
            free(pass_h2_max_qin);
            free(pass_h2_min_qin);
            return block;
        }
    }//change time loop end
    if(!(max_h2<=10&&max_h2>=1)||!(max_h1<=10&&max_h1>=1))
    {
        printf("error at change time=%d\n",change_time);
        free(pass_end_max_h1);
        free(pass_end_min_h1);
        free(pass_h1_max_qin);
        free(pass_h1_min_qin);
        free(pass_end_max_h2);
        free(pass_end_min_h2);
        free(pass_h2_max_qin);
        free(pass_h2_min_qin);
        return block;
    }
    
    double total_sub_value=1000;
    double min_sub_h1_value;
    double min_sub_h2_value;
    double sum;
    int min_time;
    for(i=0;i<len;i++)
    {
    	if(pass_end_max_h1[i]>10||pass_end_max_h1[i]<1||pass_end_min_h1[i]>10||pass_end_min_h1[i]<1)
    	{
    		printf("h1 pass error\n");
    		getchar();
    		free(pass_end_max_h1);
        	free(pass_end_min_h1);
        	free(pass_h1_max_qin);
        	free(pass_h1_min_qin);
        	free(pass_end_max_h2);
        	free(pass_end_min_h2);
        	free(pass_h2_max_qin);
        	free(pass_h2_min_qin);
        	return block;
    	}else if(pass_end_max_h2[i]>10||pass_end_max_h2[i]<1||pass_end_min_h2[i]>10||pass_end_min_h2[i]<1){
    		printf("h2 pass error\n");
    		getchar();
    		free(pass_end_max_h1);
        	free(pass_end_min_h1);
        	free(pass_h1_max_qin);
        	free(pass_h1_min_qin);
        	free(pass_end_max_h2);
        	free(pass_end_min_h2);
        	free(pass_h2_max_qin);
        	free(pass_h2_min_qin);
        	return block;
    	}
        min_sub_h1_value=pass_end_max_h1[i]-pass_end_min_h1[i];
        min_sub_h2_value=pass_end_max_h2[i]-pass_end_min_h2[i];
        sum=min_sub_h1_value+min_sub_h2_value;
        if(sum<total_sub_value&&(total_sub_value-sum)>=0.0001)
        {
            min_time=i;
            total_sub_value=sum;
        }
    }
    
    if(is_debug)
    if(last_ErrorCase!=0)
    {
        for(i=0;i<len;i++)
        {
            printf("t:%d ",i);
            printf("max h1=%lf min h1=%lf\n",pass_end_max_h1[i],pass_end_min_h1[i]);
            printf("max h2=%lf min h2=%lf\n",pass_end_max_h2[i],pass_end_min_h2[i]);
            min_sub_h1_value=pass_end_max_h1[i]-pass_end_min_h1[i];
            min_sub_h2_value=pass_end_max_h2[i]-pass_end_min_h2[i];
            sum=min_sub_h1_value+min_sub_h2_value;
            printf("switch time:%d\nsum=%lf\n",i,sum);
        }
        printf("last_ErrorCase=%d\n",last_ErrorCase);
        printf("\nmin_time=%d\n",min_time);
        last_ErrorCase=0;
        getchar();
    }
    
    printf("min t:%d\nmax h1=%lf min h1=%lf\nmax h2=%lf min h2=%lf\n",min_time,pass_end_max_h1[min_time],pass_end_min_h1[min_time],pass_end_max_h2[min_time],pass_end_min_h2[min_time]);
    //set value for return block
    block.block_num=1;
    block.s_time=min_time;
    block.max_end_h1=pass_end_max_h1[min_time];
    block.min_end_h1=pass_end_min_h1[min_time];
    block.max_end_h2=pass_end_max_h2[min_time];
    block.min_end_h2=pass_end_min_h2[min_time];
    block.pass_max_h1_qin=pass_h1_max_qin[min_time];
    block.pass_min_h1_qin=pass_h1_min_qin[min_time];
    block.pass_max_h2_qin=pass_h2_max_qin[min_time];
    block.pass_min_h2_qin=pass_h2_min_qin[min_time];
    block.pass_fi=fi;
    //free dinamic array
    free(pass_end_max_h1);
    free(pass_end_min_h1);
    free(pass_h1_max_qin);
    free(pass_h1_min_qin);
    free(pass_end_max_h2);
    free(pass_end_min_h2);
    free(pass_h2_max_qin);
    free(pass_h2_min_qin);
    
    return block;
}

bool tank_max_fi(double input_fi,int init_dir,int block_num)
{
    int i,j;
    int dir=init_dir;
    int case_num=pow(2,block_num);
    double fi=input_fi;
    double init_h1=3,init_h2=5,last_h1,last_h2;
    double last_max_h1,last_min_h1,last_max_h2,last_min_h2;
    double block1_init_h_max,block1_init_h_min,block1_init_h;
    int len;
    int block_long;
    int block_long_else;
    
    block_long=ceil(800/block_num);
    block_long_else=800%block_long+block_long;
    //printf("block_long=%d\n",block_long);
    //printf("block_long_else=%d\n",block_long_else);
    //getchar();
    
    Dp_table *last_case=(Dp_table*)malloc(block_num*(sizeof(Dp_table)));
    for(i=0;i<block_num;i++)//dp table init 
    {
        last_case[i].is_Switch=-1;
        last_case[i].dp_block=init_block();
    }
    
    Block *all_block=(Block*)malloc(block_num*(sizeof(Block)));
    for(i=0;i<block_num;i++)//all case block init 
    {
        all_block[i]=init_block();
    }
    
    for(i=0;i<block_num;i++)
    {
        dir=init_dir;
        if(i!=0)
        {
        	for(j=0;j<block_num;j++)
        	{
        		if(case_list[i][j]!=last_case[j].is_Switch)
        		{
        			printf("dp start block:%d\n",j);
        			if(is_debug)
        				getchar();
        			break;
        		}else{
        			all_block[j]=cp_block(last_case[j].dp_block);
        			last_max_h1=all_block[j].max_end_h1;
            	    last_min_h1=all_block[j].min_end_h1;
            	    last_max_h2=all_block[j].max_end_h2;
            	    last_min_h2=all_block[j].min_end_h2;
            	    if(is_debug)
            	    	print_block(all_block[j]);
        		}
        	}
        }else{
        	j=0;
        }
        
        printf("case:%d\nstart block:%d\n",i,j);
        if(is_debug)
        	getchar();
        
        for(;j<block_num;j++)
        {
            if(j==block_num-1)
                len=block_long_else;
            else
                len=block_long;
            
            printf("case:%d block:%d switch:%d\n",i,j,case_list[i][j]);
            printf("dir=%d\n",dir);
                                
            if(case_list[i][j]==1&&j==0)
            {
                all_block[j]=system_cal_switch(init_h1,init_h2,fi,len,dir);
                dir=-dir;
            }else if(case_list[i][j]==0&&j==0){
                all_block[j]=system_cal_no_switch(init_h1,init_h2,fi,len,dir);
            }else if(case_list[i][j]==1&&j!=0){
                int z=0;
                Block max_case_block=init_block();
                Block min_case_block=init_block();
                printf("no switch dir-:\n");
                printf("max case:\nlast_h1=%lf last_h2=%lf\n",last_max_h1,last_max_h2);
                max_case_block=system_cal_switch(last_max_h1,last_max_h2,fi,len,dir);
                printf("no switch dir-:\n");
                printf("min case:\nlast_h1=%lf last_h2=%lf\n",last_min_h1,last_min_h2);
                min_case_block=system_cal_switch(last_min_h1,last_min_h2,fi,len,dir);
                if(max_case_block.pass_fi==-1&&min_case_block.pass_fi==-1)
                {
                    free(last_case);
                    free(all_block);
                    last_ErrorCase=i;
                    return false;
                }else{
                	if(max_case_block.pass_fi!=1&&min_case_block.pass_fi!=-1){
               			double max_case_range=0;
               			double min_case_range=0;
               			max_case_range+=max_case_block.max_end_h1-max_case_block.min_end_h1;
               			max_case_range+=max_case_block.max_end_h2-max_case_block.min_end_h2;
                  		min_case_range+=min_case_block.max_end_h1-min_case_block.min_end_h1;
                 		min_case_range+=min_case_block.max_end_h2-min_case_block.min_end_h2;
                   			
               			if(max_case_range>=min_case_range)
               			{
                   			all_block[j]=cp_block(max_case_block);
                   		}else{
                   			all_block[j]=cp_block(min_case_block);
               			}
               		}else if(max_case_block.pass_fi!=-1&&min_case_block.pass_fi==-1){
               			all_block[j]=cp_block(max_case_block);
               		}else if(max_case_block.pass_fi==-1&&min_case_block.pass_fi!=-1){
               			all_block[j]=cp_block(min_case_block);
               		}
               	}
                                               
                dir=-dir;
            }else if(case_list[i][j]==0&&j!=0){
                
                Block max_case_block=init_block();
                Block min_case_block=init_block();
                printf("no switch dir-:\n");
                printf("max case:\nlast_h1=%lf last_h2=%lf\n",last_max_h1,last_max_h2);
                max_case_block=system_cal_no_switch(last_max_h1,last_max_h2,fi,len,dir);
                printf("no switch dir-:\n");
                printf("min case:\nlast_h1=%lf last_h2=%lf\n",last_min_h1,last_min_h2);
                min_case_block=system_cal_no_switch(last_min_h1,last_min_h2,fi,len,dir);
                if(max_case_block.pass_fi==-1&&min_case_block.pass_fi==-1)
                {
                    free(last_case);
                    free(all_block);
                    last_ErrorCase=i;
                    return false;
                }else{
               		if(max_case_block.pass_fi!=1&&min_case_block.pass_fi!=-1){
               			double max_case_range=0;
               			double min_case_range=0;
               			max_case_range+=max_case_block.max_end_h1-max_case_block.min_end_h1;
               			max_case_range+=max_case_block.max_end_h2-max_case_block.min_end_h2;
                  		min_case_range+=min_case_block.max_end_h1-min_case_block.min_end_h1;
                 		min_case_range+=min_case_block.max_end_h2-min_case_block.min_end_h2;
                   			
               			if(max_case_range>=min_case_range)
               			{
                   			all_block[j]=cp_block(max_case_block);
                   		}else{
                   			all_block[j]=cp_block(min_case_block);
               			}
               		}else if(max_case_block.pass_fi!=-1&&min_case_block.pass_fi==-1){
               			all_block[j]=cp_block(max_case_block);
               		}else if(max_case_block.pass_fi==-1&&min_case_block.pass_fi!=-1){
               			all_block[j]=cp_block(min_case_block);
               		}
               	}
           	}//if-else case list end
            
           	if(all_block[j].pass_fi!=-1)//here to dtermine next block h1&h2 possible range
            {
           	    all_block[j].block_num=j;
           	    printf("Case:%d Block:%d pass_fi=%lf\n",i,j,all_block[j].pass_fi);
           	    last_max_h1=all_block[j].max_end_h1;
           	    last_min_h1=all_block[j].min_end_h1;
           	    last_max_h2=all_block[j].max_end_h2;
           	    last_min_h2=all_block[j].min_end_h2;
           	}else{ 
                printf("error fi=%lf\n",fi);
               	printf("at Case:%d Block:%d fi=%lf\n",i,j,fi);
               	last_ErrorCase=i;
               	error_case_list[i]++;
               	free(last_case);
               	free(all_block);                
               	return false;
           	}                       
        }//block loop end
        
        printf("pass Case:%d\n",i);        
		//for output table
        for(j=0;j<block_num;j++)
            output_table[i][j]=cp_block(all_block[j]);
        
        printf("dp case\n");
        for(j=0;j<block_num;j++)
        {
            last_case[j].is_Switch=case_list[i][j];
            last_case[j].dp_block=cp_block(all_block[j]);
            if(is_debug)
            {
            	printf("block:%d\n",last_case[j].is_Switch);
            	print_block(last_case[j].dp_block);
        	}
        }
        if(is_debug)
        	getchar();
    }//all case finish   
    
    free(all_block);
    free(last_case);
    //printf("last_case free suc\n");
    return true;
}

double get_min_fi(double x1,double x2)
{
    if(x1>=x2){
        return x2;}
    else{
        return x1;}
}

void output_2tank_h(FILE *fp,Block input[],int init_dir,int block_num,int Case)
{
    int i,t;
    int dir=init_dir;
    double fi;
    double init_h1=3,init_h2=5;
    double h1,h2,current_h1,current_h2;
    double qin,q1,q2;
    double theta,theta_plus,theta_minus,theta_switch;
    int block_long,block_long_else,len;
    
    block_long=ceil(800/block_num);
    block_long_else=800%block_long+block_long;
    
    for(i=0;i<block_num;i++)
    {
        printf("dir=%d\n",dir);
        if(input[i].pass_fi==-1)
        {
            printf("error input\n");
            return;
        }else{
            fi=input[i].pass_fi;
            theta_plus=0.5+0.5*fi;
            theta_minus=0.5-0.5*fi;
        
            if(dir>0)
            {
                theta=theta_plus;
                theta_switch=theta_minus;
            }else{
                theta=theta_minus;
                theta_switch=theta_plus;
            }
        
            if(case_list[Case][i]==1)//switch dir
            {
                if(dir>0)
                    {qin=input[i].pass_max_h1_qin;}
                else
                    {qin=input[i].pass_min_h1_qin;}
            }else{//no switch dir
                if(dir>0)
                    {qin=input[i].pass_min_h1_qin;}
                else
                    {qin=input[i].pass_max_h1_qin;}
            }
        }
        if(i==0)
        {
            current_h1=init_h1;
            current_h2=init_h2;
            fprintf(fp,"%.5lf,",current_h1);
            fprintf(fp,"%.5lf\n",current_h2);
        }
        
        if(i==block_num-1)
            len=block_long_else;
        else
            len=block_long;
        
        if(input[i].s_time==-1)
        {
            printf("block:%d qin=%lf\n",i,qin);
            
            for(t=0;t<len;t++)
            {
                q1=current_h1/R1;
                q2=(current_h2-current_h1)/R2;
                h1=current_h1+(qin+q2-q1)/Ar1;
                h2=current_h2+(theta-q2-0.4)/Ar2;    
                current_h1=h1;
                current_h2=h2;
                fprintf(fp,"%.5lf,",current_h1);
                fprintf(fp,"%.5lf\n",current_h2);
            }
        }else{
            printf("block:%d qin=%lf\n",i,qin);
            
            for(t=0;t<len;t++)
            {
                if(t==input[i].s_time){
                    q1=current_h1/R1;
                    q2=(current_h2-current_h1)/R2;
                    h1=current_h1+(qin+q2-q1)/Ar1;
                    h2=current_h2+(0.5-q2-0.4)/Ar2;
                    
                    theta=theta_switch;
                    dir=-dir;
                }else{
                    q1=current_h1/R1;
                    q2=(current_h2-current_h1)/R2;
                    h1=current_h1+(qin+q2-q1)/Ar1;
                    h2=current_h2+(theta-q2-0.4)/Ar2;
                }
            
                current_h1=h1;
                current_h2=h2;
                fprintf(fp,"%.5lf,",current_h1);
                fprintf(fp,"%.5lf\n",current_h2);
            }
        }
    }
}

void simulat_system(void)
{
    bool isSuc=false;
    FILE *fp;
    clock_t start,end;
    int s_time;
    double guess_qin;
    double guess_f,reduce_f;
    double f=1,dir1_min_f,dir0_min_f,final_f;
    char control[2];
    control[0]='Y';
    control[1]='\0';
    int i,j;
    int loop_level=4;
    int dir0_error_case=-1,dir1_error_case=-1;
    int block_num,case_num;
    
    printf("plz input block number(1~800):\n");
    scanf("%d",&block_num);
    printf("do debug(1/0)?\n");
    scanf("%d",&is_debug);
    printf("debug:%d\n",is_debug);
    build_case_list(block_num);
    build_output_table(block_num);
    getchar();
    
    Block *dir0_list=(Block*)malloc(block_num*(sizeof(Block)));
    Block *dir1_list=(Block*)malloc(block_num*(sizeof(Block)));
    for(i=0;i<block_num;i++)
    {
        dir0_list[i]=init_block();
        dir1_list[i]=init_block();
    }
        
    start=clock();
    while(loop_level>0)
    {
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
                exit(1);
        }
        if(loop_level!=4)
        	f-=reduce_f;
        
        while(!isSuc){
            isSuc=tank_max_fi(f,1,block_num);
            if(isSuc)
                break;
            else
                f-=reduce_f;
        }
        isSuc=0;
        dir1_min_f=f;
        printf("guess loop pass fi:%lf\n",f);
        if(is_debug)
        	getchar();
        if(loop_level)
        {
            f+=reduce_f;
            loop_level--;
        }else{
            break;
        }
    }
    printf("dir+ min pass fi:%lf\n",dir1_min_f);
    printf("dir+ finish\n");
    printf("error_case_list\n");
    int error_num=0;
    int most_error_case=-1;
    for(i=0;i<block_num;i++)
    {
        printf("case %d: ",i);
        printf("%d\n",error_case_list[i]);
        if(error_case_list[i]>error_num)
        {
            error_num=error_case_list[i];
            most_error_case=i;
        }
    }
    printf("last_ErrorCase:%d\n",last_ErrorCase);
    printf("most_error_case:%d\n",most_error_case);
    if(is_debug)
    	getchar();
    dir1_error_case=last_ErrorCase;
    for(i=0;i<block_num;i++)
    {
        dir1_list[i]=cp_block(output_table[dir1_error_case][i]);
        if(is_debug)
        	print_block(dir1_list[i]);
    }
    if(is_debug)
    	getchar();
    //init
    f=1;
    loop_level=4;
    isSuc=false;
    last_ErrorCase=-1;
    for(i=0;i<16;i++){
        error_case_list[i]=0;
    }
    
    while(loop_level>0)
    {
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
                exit(1);
        }
        if(loop_level!=4)
        	f-=reduce_f;
        while(!isSuc){
            isSuc=tank_max_fi(f,-1,block_num);
            if(isSuc)
                break;
            else
                f-=reduce_f;
        }
        isSuc=0;
		dir0_min_f=f;
        if(is_debug)
        	getchar();
        if(loop_level)
        {
            f+=reduce_f;
            loop_level--;
        }else{
            break;
        }
    }
    final_f=get_min_fi(dir0_min_f,dir1_min_f);
    end=clock();   
    dir0_error_case=last_ErrorCase;
    printf("dir- min pass fi:%lf\n",dir0_min_f);
    printf("dir- finish\n");
    printf("error_case_list\n");
    error_num=0;
    most_error_case=-1;
    for(i=0;i<block_num;i++)
    {
        printf("case %d: ",i);
        printf("%d\n",error_case_list[i]);
        if(error_case_list[i]>error_num)
        {
            error_num=error_case_list[i];
            most_error_case=i;
        }
    }
    printf("last_ErrorCase:%d\n",last_ErrorCase);
    printf("most_error_case:%d\n",most_error_case);
    if(is_debug)
    	getchar();
    for(i=0;i<block_num;i++)
    {
        dir0_list[i]=cp_block(output_table[dir0_error_case][i]);
        if(is_debug)
	        print_block(dir0_list[i]);
    }
    if(is_debug)
		getchar();  
    printf("final min fi:%lf\n",final_f);
    printf("process time:%lf\n",(end-start)/(double)CLOCKS_PER_SEC);
    printf("dir-to+ min pass fi:%lf\n",dir0_min_f);
    printf("dir+to- min pass fi:%lf\n",dir1_min_f);
        
    printf("dir-to+ last error case:Case%d\n",dir0_error_case);
    printf("dir+to- last error case:Case%d\n",dir1_error_case);
    getchar();
    
    if(dir0_min_f<dir1_min_f){
        for(i=0;i<block_num;i++)
            print_block(dir0_list[i]);
    }else{
        for(i=0;i<block_num;i++)
            print_block(dir1_list[i]);
    }
    
    printf("output tank h?(Y/N)\n");
    scanf("%s",control);
   
    int print_case;
    int print_dir; 
    if(control[0]=='Y')
    {
    	if(NULL==(fp=fopen("dp_2tank_qin.csv","w"))){
        	printf("fail to open the file\n");
        	exit(EXIT_FAILURE);
    	}
    	
    	if(dir0_min_f<dir1_min_f)
        	{print_dir=-1; print_case=dir0_error_case;}
    	else
        	{print_dir=1; print_case=dir1_error_case;}
      	printf("print_dir=%d\n",print_dir);
        if(print_dir>0)
            output_2tank_h(fp,dir1_list,print_dir,block_num,print_case);
        else
            output_2tank_h(fp,dir0_list,print_dir,block_num,print_case);
            
        printf("output end\n");
        fclose(fp);
    	printf("fp close\n");
    }
    
    for(i=0;i<block_num;i++)
    {
        free(output_table[i]);
    }
    free(output_table);
    printf("output_table suc\n");
    
    for(i=0;i<block_num;i++)
    {
        free(case_list[i]);
    }
    free(case_list);
    printf("case_list suc\n");
    
}
