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

void translateCoords(int translated_x, int translated_y, int* refs, int offset, unsigned int orientation){
    int ref_x = 0, ref_y = 0;
 
    ref_x = refs[0] * getTranslatedCoords[orientation][0];
    ref_y = refs[1] * getTranslatedCoords[orientation][1];
    
    if(getTranslatedCoords[orientation][0] < 0){
        ref_x--;
    }
    if(getTranslatedCoords[orientation][1] < 0){
        ref_y++;
    }
    
    if(getTranslatedCoords[orientation][2] == 1){
        int temp = ref_x;
        ref_x = ref_y;
        ref_y = temp;
        ref_x--;
        ref_y++;
    }
    
    ref_x += translated_x;
    ref_y += translated_y;
    
    ref_x +=  offset;
    ref_y = -ref_y + offset;
    
    refs[0] = ref_x;
    refs[1] = ref_y;
    
}

void copyAndDraw(int x_start, int x_end, int y_start, int y_end, int step_x, int step_y, unsigned char* rendered_frame, unsigned char* frame_buffer,int c_x, int c_y, int ref_x, int ref_y,unsigned int width,int orientation){
    int T = width < 160 ? 1: 160;
    int tile_offset_y = (y_end - y_start) % T;
    int tile_offset_x = (x_end - x_start) % T;
    int fitting_width = y_end - tile_offset_y;
    int fitting_height = x_end - tile_offset_x;
    int triple_width = width * 3;
    if(itr_dir[orientation][2] == 1){
        int y_width_val = 0;
        int b_f_width_val = 0;
        for (int i = y_start; i < fitting_width; i +=T)
           for (int j = x_start; j < fitting_height; j += T)
             for (int i1 = i; i1 < i + T; i1++) {
                    c_y = ref_y + i1 * step_y;
                    y_width_val = c_y * triple_width;
                    b_f_width_val = i1 * triple_width;
                    for (int j1 = j; j1 < j + T; j1++) {
                        c_x = ref_x + j1 * step_x;            
                            int pos_f_b =  b_f_width_val + j1 * 3;
                            int pos_r_f = y_width_val + c_x * 3;
                            memcpy( rendered_frame +pos_r_f, frame_buffer + pos_f_b, 3);                  
                    }
                }
         /* The rest should all fit into the cache */
        for (int i = y_end - tile_offset_y; i < y_end; i++){
            c_y = ref_y + i * step_y;
            y_width_val = c_y * triple_width;
            b_f_width_val = i * triple_width;
            for(int j = x_end - tile_offset_x; j < x_end; j++){
                c_x = ref_x + j * step_x;
                    int pos_f_b =  b_f_width_val + j * 3;
                    int pos_r_f = y_width_val + c_x * 3;
                    memcpy(rendered_frame + pos_r_f, frame_buffer + pos_f_b, 3);
            }
        }


    }else{
        int x_val = 0;
        int b_f_width_val = 0;
        for(int i = y_start; i < fitting_width; i+=T)
            for (int j = x_start; j < fitting_height; j+= T)
                for (int i1 = i; i1 < i + T; i1 ++) {
                    c_x = ref_x + i1 * step_x;
                    x_val = c_x * 3;
                    b_f_width_val = i1 * triple_width;
                    for(int j1 = j; j1 < j + T; j1++){
                        c_y = ref_y + j1 * step_y;
                            int pos_f_b =  b_f_width_val + j1 * 3;
                            int pos_r_f = x_val + c_y * triple_width;
                            memcpy(rendered_frame + pos_r_f, frame_buffer + pos_f_b, 3);
                }
         }

        for (int i = y_end - tile_offset_y; i < y_end; i ++) {
            c_x = ref_x + i * step_x;
            x_val = c_x * 3;
            b_f_width_val = i * triple_width;
            for(int j = x_end - tile_offset_x; j < x_end; j++){
                c_y = ref_y + j * step_y;
                    int pos_f_b =  b_f_width_val + j * 3;
                    int pos_r_f = x_val + c_y * triple_width;
                    memcpy(rendered_frame + pos_r_f, frame_buffer + pos_f_b, 3);
            }
        }
    }
}

void doTranslation(unsigned char *frame_buffer, unsigned char *rendered_frame, unsigned width, unsigned height, unsigned int orientation, int ref_x, int ref_y){
    int signed_width = (int)width, signed_height = (int)height;
    int c_x = 0, c_y = 0, step_x = 0, step_y = 0;
    int x_start = 0, y_start = 0, x_end = width, y_end = height; 
    
    step_x = itr_dir[orientation][0];
    step_y = itr_dir[orientation][1];
    
    c_x = ref_x;
    c_y = ref_y;
    switch(orientation){
        case 0:
            if(c_x < 0){
                x_start -= c_x;
                c_x = 0;
            }else{
                x_end -= c_x;
            }
            if(c_y < 0){
                y_start -= c_y;
                c_y = 0;
            }else{
                y_end -= c_y;
            }
            break;
        case 1:    
            if(c_x >= signed_width){
                x_start += (c_x - signed_width + 1); 
                c_x = signed_width -1;
            }else{
                x_end -= (signed_width - 1 - c_x);
            }
            if(c_y < 0){
                y_start -= c_y;
                c_y = 0;
            }else{
                y_end -= c_y;
            }          
            break;
        case 2:
            if(c_x < 0){
                x_start -= c_x;
                c_x = 0;
            }else{
                x_end -= c_x;
            }
            if(c_y >= signed_height){
                y_start += (c_y - signed_height + 1);
                c_y = signed_height - 1;
            }else{
                y_end -= (signed_height - 1 - c_y);
            }  
            break;    
        case 3:
            if(c_x >= signed_width){
                x_start += (c_x - signed_width + 1); 
                c_x = signed_width -1;
            }else{
                x_end -= (signed_width - 1 - c_x);
            }
            if(c_y >= signed_height){
                y_start += (c_y - signed_height + 1);
                c_y = signed_height - 1;
            }else{
                y_end -= (signed_height - 1 - c_y);
            }  
            break;
        case 4:
            if(c_y < 0){
                x_start -= c_y;
                c_y = 0;
            }else{
                x_end -= c_y; 
            }
            if(c_x < 0){
                y_start -= c_x;
                c_x = 0;
            }else{
                y_end -= c_x;
            }
            break;
        case 5:
            if(c_y < 0){
                x_start -= c_y;
                c_y = 0;
            }else{
                x_end -= c_y; 
            }
            if(c_x >= signed_width){
                y_start += (c_x - signed_width + 1);
                c_x = signed_width - 1;
            }else{
                y_end -= (signed_width - 1 - c_x);
            }
            break;
        case 6:
            if(c_y >= signed_height){
                x_start += (c_y - signed_height + 1);
                c_y = signed_height - 1;
            }else{
                x_end -= (signed_height - 1 - c_y);
            }
            if(c_x >= signed_width){
                y_start += (c_x - signed_width + 1);
                c_x = signed_width - 1;
            }else{
                y_end -= (signed_width - 1 - c_x);
            }
            break;
        case 7:
             if(c_y >= signed_height){
                x_start += (c_y - signed_height + 1);
                c_y = signed_height - 1;
            }else{
                x_end -= (signed_height - 1 - c_y);
            }
            if(c_x < 0){
                y_start -= c_x;
                c_x = 0;
            }else{
                y_end -= c_x;
            }
            break;    
    }       
    copyAndDraw(x_start,x_end,y_start,y_end,step_x,step_y,rendered_frame,frame_buffer,c_x,c_y,ref_x,ref_y,width,orientation);
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
    int refs[2];
    int x_d = width/2, y_d = height/2;
    int x_l = 0, x_h = 0, y_l = 0, y_h = 0;
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
    unsigned char* orientation_pointers[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; 
    unsigned char* canvas_pointers[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; 
    
    memset(rendered_frame,0xff,width * height * 3);
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
        if(orientation_pointers[orientation] == NULL){
            canvas_pointers[orientation] = allocateFrame(height * 3, width * 3);
            orientation_pointers[orientation] = canvas_pointers[orientation] + height * width * 3;
            createOrientation(orientation,orientation_pointers);
        }
        processed_frames += 1;
        if (processed_frames % 25 == 0) {
            refs[0] = left_x;
            refs[1] = top_y;
            translateCoords(translated_x,translated_y,refs,top_y,orientation);
            doTranslation(frame_buffer, rendered_frame, height, width, orientation,refs[0],refs[1]);
            verifyFrame(rendered_frame, width, height, grading_mode);    
            memset(rendered_frame,0xff,width * height * 3);
            //erase(rendered_frame,x_l,x_h,y_l,y_h,width);
        }
    }
    return;
}
