/* 
 * File:   main.c
 * Author: AdrianMRivera
 *
 * Created on November 19, 2015, 4:12 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */

void man_wants_to_enter();
void woman_wants_to_enter();
void man_leaves();
void woman_leaves();
void enter_restroom();
void add(int*, int);
void check_queue();
void queue_to_string();
void print_status();

typedef int semaphore;    
    semaphore sig;    
    int queue_pos, rest_occ_pos;
    semaphore restroom_occupants; //analogous to shared memory location
    int queue[20]; // 0 == female, 1 == male
    
int main(int argc, char** argv) {
    
    sig = 0; // empty == 0, women present == 1, men present == 2
    queue_pos = 19;//beginning position for addition, akin to adding to an ADT queue
    rest_occ_pos = 0;
    restroom_occupants = 0;
    
    int i;
    for( i = 0 ; i < 20 ; i++ ){        
        queue[i] = -1;//same as above
    }   
    
    /* presence/absence of character constant identifies arrays */
    queue[0] = 'q';
    
    
    
    man_wants_to_enter(); // 1 enters
        print_status();
    
        woman_wants_to_enter(); // 1 woman in queue
            print_status();
        
    man_wants_to_enter(); // 2 in restroom
        print_status();
    
    man_leaves(); // 1 man in restroom
        print_status();
    
        woman_wants_to_enter();  // 2 in queue
            print_status();
        
    man_leaves(); // 0 men in restroom; 0 woman in queue, 2 woman in restroom
        print_status();
    
    man_wants_to_enter(); // 0 woman in queue, 1 man in queue
        print_status();
    
    //man_leaves(); // 0 men in restroom; 1 woman in queue, and 1 woman enters restroom
        woman_wants_to_enter(); // 3 women in restroom
            print_status();
        
        woman_wants_to_enter(); // 4 women in restroom
            print_status();
        
        woman_leaves(); // 3 women in restroom
            print_status();
        
    man_wants_to_enter(); // 0 women in queue, and 2 men in queue
        print_status();
    
        woman_leaves(); // 2 woman in restroom
            print_status();
        
        
    man_wants_to_enter(); // 0 women in queue, and 3 men in queue
        print_status();
    
    man_wants_to_enter(); // 4 men in queue
        print_status();
    
        woman_leaves(); // 1 woman in restroom
            print_status();
        
          woman_leaves(); // 0 women in restroom
            print_status();
//        
//        man_wants_to_enter();
//            print_status();
//        
//        woman_wants_to_enter();
//            print_status();
//        
    
    
    
    return (EXIT_SUCCESS);
}

void man_wants_to_enter(void){
    
    //check_queue();
    
    if( sig == 1 ){
        add(queue, 1);
    } else {
        enter_restroom();
        sig = 2;
        
    }
    
}

void woman_wants_to_enter(void){
    
    
    
    if( sig == 2 ){
        add(queue, 0); // add to queue
    } else {
        enter_restroom();        
        sig = 1;        
        
    }
    
    
}

void man_leaves(void){
    
    if(restroom_occupants > 0)
        restroom_occupants--;
    
    if( restroom_occupants == 0){
        sig = 0;
        check_queue();
        
    }
}

void woman_leaves(void){
    
    if(restroom_occupants > 0)
        restroom_occupants--;
    
    if( restroom_occupants == 0 ){
        sig = 0;
        check_queue();
    }
}

void add(int *array, int gender){
    
    if((array)[0] == 'q'){
        (array)[queue_pos] = gender; // 0 or 1

        if((queue_pos - 1) >= 0)
            queue_pos--;
    } else {
        (array)[rest_occ_pos] = gender;
    }
    
}

void enter_restroom(){
    
    restroom_occupants++;
}

void queue_to_string(void){
    
    int i;
    for(i = 0; i < 20; i++){
        if(i % 5 == 0)
            printf("\n");
        if(queue[i] == 0)
            printf("%s ", "female");
        else if( queue[i] == 1)
            printf("%s ", "male");
        else
            printf("%d ", i);
            
    }
        printf("\n");
}

void check_queue(void){
    int next;
    
    do{
    
        next = queue[19]; // next individual in queue

        if(sig == 0){
            next == 0 ? woman_wants_to_enter() : man_wants_to_enter();
        } else if(sig == 1){
            if(next == 0){
                woman_wants_to_enter();
            }
        } else {
            if(next == 1){
                man_wants_to_enter();
            }
        }

        /* update queue, simulating FIFO pop procedure*/
        int i;
        for(i = 19; i > 1; i--){
            queue[i] = queue[i - 1];

        }
        queue_pos++;
        
    }while( queue[19] == next);
}

void print_status(void){
    
    printf("--Status---------------------------------------------------\n");
    
    printf("%s", "Queue: ");
        queue_to_string();
    
    printf("Bathroom occupants: %d-", restroom_occupants);
    if(sig == 1)
        printf("Female(s)\n");
    else if(sig == 2)
        printf("Male(s)\n");
    
    printf("---------------------------------------------------------->|\n");
}