#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
    //0             1            2
    //invert x      invert y     swap flag
    int getTranslatedCoords[8][3] = {
        {1,1,0},
        {-1,1,0},
        {1,-1,0},
        {-1,-1,0},
        {-1,-1,1},
        {-1,1,1},
        {1,1,1},
        {1,-1,1}
    };
    int itr_dir[8][3] = {
        {1,1,1},
        {-1,1,1},
        {1,-1,1},
        {-1,-1,1},
        {1,1,0},
        {-1,1,0},
        {-1,-1,0},
        {1,-1,0}
    };
/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "default-name";

    // Please fill in your information
    char student1_first_name[] = "Ruixin";
    char student1_last_name[] = "Huang";
    char student1_student_number[] = "1001781565";

    // Please fill in your partner's information
    // If yon't have partner, do not modify this
    char student2_first_name[] = "joe";
    char student2_last_name[] = "doe";
    char student2_student_number[] = "0000000001";

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

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          You can modify anything else in this file
 ***********************************************************************************************************************
 * @param sensor_values - structure stores parsed key value pairs of program instructions
 * @param sensor_values_count - number of valid sensor values parsed from sensor log file or commandline console
 * @param frame_buffer - pointer pointing to a buffer storing the imported  24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param grading_mode - turns off verification and turn on instrumentation
 ***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void doTranslation(unsigned char *frame_buffer, unsigned char *rendered_frame, unsigned width, unsigned height, unsigned int orientation,
int translated_x, int translated_y){
    int signed_width = (int)width, signed_height = (int)height;
    int ref_x = -(signed_width/2), ref_y = (signed_height/2), c_x = 0, c_y = 0, step_x = 0, step_y = 0;
    int x_min = signed_width, x_max = 0, y_min = signed_height, y_max = 0;
    ref_x = ref_x * getTranslatedCoords[orientation][0];
    ref_y = ref_y * getTranslatedCoords[orientation][1];

    ref_x = (getTranslatedCoords[orientation][0] < 0)?(ref_x - 1):(ref_x);
    ref_y = (getTranslatedCoords[orientation][1] < 0)?(ref_y + 1):(ref_y);

    if(getTranslatedCoords[orientation][2] == 1){
        int temp = ref_x;
        ref_x = ref_y;
        ref_y = temp;
        ref_x--;
        ref_y++;
    }
    
    ref_x += translated_x;
    ref_y += translated_y;
    
    ref_x +=  (signed_width/2);
    ref_y = -ref_y + (signed_height/2);
    
    step_x = itr_dir[orientation][0];
    step_y = itr_dir[orientation][1];
    
    c_x = ref_x;
    c_y = ref_y;
    
            
    
    if(itr_dir[orientation][2] == 1){
        for(int i = 0; i < width; i++){
            c_y = ref_y + i * step_y;
            for(int j =0; j < height; j++){
                c_x = ref_x + j * step_x;
                if(c_x >= 0 && c_x < width &&c_y >= 0 && c_y < height){
                    memcpy(rendered_frame + c_y * width * 3 + c_x * 3, frame_buffer + i * width * 3 + j * 3 ,3);
                }
            }
        }
    }else{
        for(int i = 0; i < width; i++){
            c_x = ref_x + i * step_x;
            for(int j =0; j < height; j++){
                c_y = ref_y + j * step_y;
                if(c_x >= 0 && c_x < width &&c_y >= 0 && c_y < height){
                    memcpy(rendered_frame + c_y * width * 3 + c_x * 3, frame_buffer + i * width * 3 + j * 3 ,3);
                }
            }
        }
        
    }
}
void erase(unsigned char *rendered_frame, int x_min, int x_max, int y_min, int y_max, int width){
    unsigned int size = (y_min - y_max) * (x_max - x_min + 1) * 3;
    memset(rendered_frame + y_min * width * 3 + x_min * 3, 255, size);
}
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {

    //Orientations: 0 : 01    1: 10     2: 23       3: 32       4: 02    5: 20      6: 31       7: 13
//                  23       32        01          10          13       31         20          02
//Actions: 0    1     2     3     4
//         90   -90   180   MX    MY
    int x_d = width/2, y_d = height/2;
    int x_l = 0, x_h = 0, y_l = 0, y_h = 0;
    unsigned int orientation = 0;
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
    unsigned char* rendered_frame = allocateFrame(width, height);
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
        processed_frames += 1;
        if (processed_frames % 25 == 0) {
            doTranslation(frame_buffer, rendered_frame, height, width, orientation, translated_x, translated_y);
            verifyFrame(rendered_frame, width, height, grading_mode);
            memset(rendered_frame,0xff,width * height * 3);
            //erase(rendered_frame,x_l,x_h,y_l,y_h,width);
        }
    }
    return;
}
