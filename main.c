        /* 
         * File:   main.c
         * Author: AdrianMRivera
         *
         * Created on November 19, 2015, 4:12 PM
         */

        #include <stdio.h>
        #include <stdlib.h>
        #include <time.h>
        #include <pthread.h>
        #include <stdbool.h>
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
        void *guest_loader(void *ptr);
        void *guest_extractor(void *ptr);

            typedef int semaphore;
            pthread_mutex_t mutex;
            pthread_cond_t condl, conde;    
            semaphore sig;    
            int queue_pos, rest_occ_pos;
            semaphore restroom_occupants; //analogous to shared memory location
            int queue[20]; // 0 == female, 1 == male
            int cycle;
            bool complete;
        int main(int argc, char** argv) {
            void* s_return;
            void* t_return;
            srand48((long)time(NULL));
            sig = 0; // empty == 0, women present == 1, men present == 2
            queue_pos = 19;//beginning position for addition, akin to adding to an ADT queue
            rest_occ_pos = 0;
            restroom_occupants = 0;
            queue[0] = 'q'; 
            cycle = 0;
            complete = false;
            
            int i;
            for( i = 1 ; i < 20 ; i++ ){ 
                
                queue[i] = -1;//same as above
            }   
            
            pthread_t loader, extractor;
            pthread_mutex_init(&mutex, NULL);
            pthread_cond_init(&condl, NULL);
            pthread_cond_init(&conde, NULL);
            int succ = pthread_create(&extractor, NULL, guest_extractor, NULL);
            if(succ != 0)
                printf("There was a problem creating your thread\n");
            int succ2 = pthread_create(&loader, NULL, guest_loader, NULL);    
                if(succ2 != 0)
                printf("There was a problem creating your thread\n");
            pthread_join(extractor, (void *)s_return);
            pthread_join(loader, (void *)t_return);
            pthread_cond_destroy(&condl);
            pthread_cond_destroy(&conde);
            pthread_mutex_destroy(&mutex);
            
            pthread_exit(NULL);
//            
//            
//            
//            
//            
//            man_wants_to_enter(); // 1 enters
//                print_status();
//            
//                woman_wants_to_enter(); // 1 woman in queue
//                    print_status();
//                
//            man_wants_to_enter(); // 2 in restroom
//                print_status();
//            
//            man_leaves(); // 1 man in restroom
//                print_status();
//            
//                woman_wants_to_enter();  // 2 in queue
//                    print_status();
//                
//            man_leaves(); // 0 men in restroom; 0 woman in queue, 2 woman in restroom
//                print_status();
//            
//            man_wants_to_enter(); // 0 woman in queue, 1 man in queue
//                print_status();
//            
//            //man_leaves(); // 0 men in restroom; 1 woman in queue, and 1 woman enters restroom
//                woman_wants_to_enter(); // 3 women in restroom
//                    print_status();
//                
//                woman_wants_to_enter(); // 4 women in restroom
//                    print_status();
//                
//                woman_leaves(); // 3 women in restroom
//                    print_status();
//                
//            man_wants_to_enter(); // 0 women in queue, and 2 men in queue
//                print_status();
//            
//                woman_leaves(); // 2 woman in restroom
//                    print_status();
//                
//                
//            man_wants_to_enter(); // 0 women in queue, and 3 men in queue
//                print_status();
//            
//            man_wants_to_enter(); // 4 men in queue
//                print_status();
//            
//                woman_leaves(); // 1 woman in restroom
//                    print_status();
//                
//                  woman_leaves(); // 0 women in restroom
//                    print_status();
//                
//                man_wants_to_enter();
//                    print_status();
//                
//                woman_wants_to_enter();
//                    print_status();
                



            return (EXIT_SUCCESS);
        }

        void man_wants_to_enter(void){
            
            cycle++;
            //check_queue();
            print_status();
            if( sig == 1 ){
                add(queue, 1);
            } else {
                enter_restroom();
                sig = 2;

            }
            
                

        }

        void woman_wants_to_enter(void){

            cycle++;
            print_status();
            if( sig == 2 ){
                add(queue, 0); // add to queue
            } else {
                enter_restroom();        
                sig = 1;        

            }
            
               

        }

        void man_leaves(void){
            cycle++;
            print_status();
            
            if(restroom_occupants > 0)
                restroom_occupants--;
            
            if( restroom_occupants == 0){
                sig = 0;
                check_queue();
            }
            
                
        }

        void woman_leaves(void){
            cycle++;
            print_status();
            
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

                if((queue_pos - 1) > 1)
                    queue_pos--;
            } else {
                if((queue_pos - 1) > 1)
                    (array)[queue_pos--] = gender;
                
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
                cycle++;
                next = queue[19]; // next individual in queue
                
                if(sig == 0){
                    if(!complete){                   
                        next == 0 ? woman_wants_to_enter() : man_wants_to_enter();
                      
                    }
                    
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
                for(i = 19; i > 0; i--){
                    queue[i] = queue[i - 1];

                }
                queue_pos++;
                
            }while( queue[19] == next );
            printf("sig %d", sig);
        }

        void print_status(void){

            printf("--Status---------------------------------------------------\n");

            printf("%s", "Queue: ");
                queue_to_string();

            printf("Restroom occupants: %d ", restroom_occupants);
            if(sig == 1)
                printf("Female(s)\n");
            else if(sig == 2)
                printf("Male(s)\n");
            else
                printf("\n");
            printf("Cycle: %d ", cycle);
           
            //guest_loader(NULL);
            printf("---------------------------------------------------------->|\n\n");
            
        }

        void *guest_loader(void *ptr){
       
            int i;
                
                for(i = 0; i < 20; i++){
                    
                    
                    int gender = (rand() % 100) + 1;
                    
                    pthread_mutex_lock(&mutex);
                    
                    while(restroom_occupants == 20){
                        pthread_cond_wait(&condl, &mutex);                
                    }
                    
                    if(gender < 50){                        
                        man_wants_to_enter();
                        printf("Man wants to enter\n");
                    }else{                        
                        woman_wants_to_enter();
                        printf("Woman wants to enter\n");
                    }
                    pthread_cond_signal(&conde);
                                       
                    pthread_mutex_unlock(&mutex);
                }


            
            pthread_exit(0);

            return NULL;

        }

        void *guest_extractor(void *ptr){

            int j;
            
                for(j = 0; j < 20; j++){
                    
                    
                    pthread_mutex_lock(&mutex);
                    //sleep(10);
                    if(sig == 0){                          
                        pthread_cond_wait(&conde, &mutex);
                    }else if (sig == 1){                          
                        complete = j != 19 ? false : true;                          
                        woman_leaves();
                        printf("**Woman left**\n");
                        
                    }else{                                      
                        complete = j != 19 ? false : true;                        
                        man_leaves();
                        printf("**Man left**\n");
                         
                    }
                        
                    pthread_cond_signal(&condl);
                    
                    pthread_mutex_unlock(&mutex);
                }

            
            
            pthread_exit(0);

            return NULL;
        }