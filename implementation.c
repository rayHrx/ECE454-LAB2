#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

    //0             1            2
    //invert x      invert y     swap flag
    short getTranslatedCoords[8][3] = {
        {1,1,0},
        {-1,1,0},
        {1,-1,0},
        {-1,-1,0},
        {-1,-1,1},
        {-1,1,1},
        {1,1,1},
        {1,-1,1}
    };
    short itr_dir[8][3] = {
        {1,1,1},
        {-1,1,1},
        {1,-1,1},
        {-1,-1,1},
        {1,1,0},
        {-1,1,0},
        {-1,-1,0},
        {1,-1,0}
    };

struct direct_info {
    int outer_step;
    int inner_step;
    unsigned char* start;
};

void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "BELL388";

    // Please fill in your information
    char student1_first_name[] = "Ruixin";
    char student1_last_name[] = "Huang";
    char student1_student_number[] = "1001781565";

    // Please fill in your partner's information
    // If yon't have partner, do not modify this
    char student2_first_name[] = "Ruixin";
    char student2_last_name[] = "Bao";
    char student2_student_number[] = "1001294501";

    // Printing out team information
    printf("*******************************************************************************************************\n");
    printf("Team Information:\n");
    printf("\tteam_name: %s\n", team_name);
    printf("\tstudent1_first_name: %s\n", student1_first_name);
    printf("\tstudent1_last_name: %s\n", student1_last_name);
    printf("\tstudent1_student_number: %s\n", student1_student_number);
    printf("\tstudent2_first_name: %s\n", student2_first_name);
    printf("\tstudent2_last_name: %s\n", student2_last_name);
    printf("\tstudent2_student_number: %s\n", student2_student_number);
}

void createOrientation(unsigned char *frame_buffer, unsigned char *rendered_frame, unsigned width, unsigned height, unsigned int orientation, struct direct_info directs[]) {

    int outer_step = 0, inner_step = 0, outer = 0, inner = 0;
    unsigned char* start, current;
    start = directs[orientation].start;
    outer_step = directs[orientation].outer_step;
    inner_step = directs[orientation].inner_step;
    for (int i = 0; i < height; i++) {
        unsigned char* current = start + i * outer_step;
        for (int j = 0; j < width; j++) {
            unsigned char* pos = current + j * inner_step;
            memcpy(pos, frame_buffer + i * width * 3 + j * 3, 3);
        }
    }
    return;
}

unsigned char* translatePtr(int translate_x, int translate_y, unsigned height, unsigned width, unsigned char * start_ptr){
    int sign_width = (int)width;
    start_ptr -= sign_width * translate_y;
    start_ptr -= 3 * translate_x;
    return  start_ptr;
}

//void erase(unsigned char *rendered_frame, int x_min, int x_max, int y_min, int y_max, int width){
//    for(int i = y_min; i < y_max; i++){
//            for(int j = x_min; j < x_max; j++){
//                memcpy(rendered_frame + i * width * 3 + j * 3,0xff,3);
//            }
//    }
//}
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    
    //Orientations: 0 : 01    1: 10     2: 23       3: 32       4: 02    5: 20      6: 31       7: 13   
//                  23       32        01          10          13       31         20          02
//Actions: 0    1     2     3     4
//         90   -90   180   MX    MY
    register unsigned int orientation = 0;
    unsigned int states[8][5] = {
        {5,7,3,2,1},
        {6,4,2,3,0},
        {4,6,1,0,3},
        {7,5,0,1,2},
        {1,2,6,7,5},
        {3,0,7,6,4},
        {2,1,4,5,7},
        {0,3,5,4,6}
        };
//X direction move multiply with 0,2
//Y direction move multiply with 1,3
//Actions: 0    1     2     3     4
//         90   -90   180   MX    MY
    register int translated_x = 0;
    register int translated_y = 0;
    int top_y = (int)height/2, left_x = -(int)width/2;
    
    unsigned char* orientation_pointers[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; 
    unsigned char* canvas_pointers[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; 
    unsigned char* test_frame = NULL;
    
    for(int i = 0; i < 8; i++){
          canvas_pointers[i] = allocateFrame(height * 3, width * 3);
          orientation_pointers[i] = canvas_pointers[i] + height * width * 3;
          memset(canvas_pointers[i],0xff,height * 3 * width * 3 * 3);
    } 
    
    struct direct_info directions[8];
    directions[0].outer_step = width * 3;
    directions[0].inner_step = 3;
    directions[0].start = orientation_pointers[0];
    directions[1].outer_step = width * 3;
    directions[1].inner_step = -3;
    directions[1].start = orientation_pointers[1] + width * 3 - 3;
    directions[2].outer_step = -width * 3;
    directions[2].inner_step = 3;
    directions[2].start = orientation_pointers[2] + (height - 1) * width * 3;
    directions[3].outer_step = -width * 3;
    directions[3].inner_step = -3;
    directions[3].start = orientation_pointers[3] + (height * width * 3) - 3;
    directions[4].outer_step = 3;
    directions[4].inner_step = width * 3;
    directions[4].start = orientation_pointers[4];
    directions[5].outer_step = -3;
    directions[5].inner_step = width * 3;
    directions[5].start = orientation_pointers[5] + width * 3 - 3;
    directions[6].outer_step = -3;
    directions[6].inner_step = -width * 3;
    directions[6].start = orientation_pointers[6] + (height * width * 3) - 3;
    directions[7].outer_step = 3;
    directions[7].inner_step = -width * 3;
    directions[7].start = orientation_pointers[7] + (height - 1) * width * 3;
    
    for(int i = 0; i < 8; i++){
        createOrientation(frame_buffer,orientation_pointers[i],width,height,i,directions);
    }
    
    int processed_frames = 0;
//    printf("%d\n",sensor_values_count);
        for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
             int value = sensor_values[sensorValueIdx].value;
            if(!strcmp(sensor_values[sensorValueIdx].key, "W")){
                translated_y += value;
            }else if(!strcmp(sensor_values[sensorValueIdx].key, "S")){
                translated_y -= value;
            }else if(!strcmp(sensor_values[sensorValueIdx].key, "D")){
                translated_x += value;
            }else if(!strcmp(sensor_values[sensorValueIdx].key, "A")){
                translated_x -= value;
            }else if(!strcmp(sensor_values[sensorValueIdx].key, "MX")){
                translated_y = -translated_y;
                orientation = states[orientation][3];
            }else if(!strcmp(sensor_values[sensorValueIdx].key, "MY")){
                translated_x = -translated_x;
                orientation = states[orientation][4];
            }else{
                if(!strcmp(sensor_values[sensorValueIdx].key, "CCW")) value = -value;
                int abs_value = abs(value);
                    abs_value = abs_value%4;
                    if(value < 0) abs_value = 4 - abs_value;
                    if(abs_value == 1){
                        int temp = translated_x;
                        translated_x = translated_y;
                        translated_y = -temp;
                        orientation = states[orientation][0];
                    }else if(abs_value == 2){
                        translated_x = -translated_x;
                        translated_y = -translated_y;
                        orientation = states[orientation][2];
                    }else if(abs_value == 3){
                        int temp = translated_x;
                        translated_x = -translated_y;
                        translated_y = temp;
                        orientation = states[orientation][1];
                    }
            }
            
        processed_frames += 1;
        if (processed_frames % 25 == 0) { 
            test_frame = translatePtr(translated_x,translated_y,height,width,orientation_pointers[orientation]);   
            verifyFrame(test_frame, width, height, grading_mode);    
        }
    }
    return;
}
