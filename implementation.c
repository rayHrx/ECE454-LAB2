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

//unsigned int r_matrix[8] = {
//    {1,0,0,1},
//    {-1,0,0,1},
//    {1,0,0,-1},
//    {-1,0,0,-1},
//    {0,-1,-1,0},
//    {0,-1,1,0},
//    {0,1,1,0},
//    {0,1,-1,0}
//}
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
    void print(unsigned char* matrix, int width, int height)
{
    int i, j;
    unsigned char* start;
    for (i = 0; i < height; ++i)
    {
        start = matrix + i*10*3;
        for (j = 0; j < width; ++j){
            start = matrix + i*width*3 + j*3;
            printf("%d ",*start);
            printf("%d ",*(start+1));
            printf("%d ",*(start+2));
        }
        printf("\n");
    }
}
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char temp[3];
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    if (offset < 0){
        return processMoveDown(buffer_frame, width, height, offset * -1);
    }
    int top = width - offset, l = 0, step = width * 3;
    unsigned char* d = buffer_frame - step;
    unsigned char* src = buffer_frame + offset * step - step;
    while(l < top){
        d += step;
        src += step;
        memcpy(d,src,step);
        l++;
    }
//    l = height - offset + 1;
//    top = height;
//    src = buffer_frame + l * 3;
//    while(l < top){
//        for(int i = 0; i < step; i++) *(src + i) = 225;
//        l++;
//        src += step; 
//    }
    
    for (int row = (height - offset); row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            buffer_frame[position_rendered_frame] = 255;
            buffer_frame[position_rendered_frame + 1] = 255;
            buffer_frame[position_rendered_frame + 2] = 255;
        }
    }
    
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image left
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
        // handle negative offsets
    if (offset < 0){
        return processMoveLeft(buffer_frame, width, height, offset * -1);
    }

    int position_rendered_frame = (height * width) * 3 + offset * 3 - 1;
    int position_buffer_frame = (height * width) * 3 - 1;
    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        position_rendered_frame -= offset * 3;
        position_buffer_frame -= offset * 3;
        for (int column = 0; column < width - offset; column++) {
            position_rendered_frame -= 3;
            position_buffer_frame -= 3;
            memcpy(buffer_frame + position_rendered_frame, buffer_frame + position_buffer_frame, 3);
        }
    }

   for (int row = 0; row < height; row++) {
        for (int column = 0; column < offset; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            buffer_frame[position_rendered_frame] = 255;
            buffer_frame[position_rendered_frame + 1] = 255;
            buffer_frame[position_rendered_frame + 2] = 255;
        }
    }
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
       // handle negative offsets
    if (offset < 0){
        return processMoveUp(buffer_frame, width, height, offset * -1);
    }
    
    unsigned char* src = buffer_frame + width * height * 3 - offset * width * 3;
    unsigned char* dest = buffer_frame + width * height * 3;
    int step = width * 3, l = 0, top = height - offset;
    while(l < top){
        dest -= step;
        src -= step;
        memcpy(dest,src,step);
        l++;
    }
    int position_rendered_frame = 0;
    for (int row = 0; row < offset; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            buffer_frame[position_rendered_frame] = 255;
            buffer_frame[position_rendered_frame + 1] = 255;
            buffer_frame[position_rendered_frame + 2] = 255;
        }
    }
    
//    l = 0;
//    top = offset;
//    src = buffer_frame;
//    while(l < top){
//        for(int i = 0; i < step; i++) *(src + i) = 225;
//        l++;
//        src += step; 
//    }

    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image right
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
   // handle negative offsets
    if (offset < 0){
        return processMoveRight(buffer_frame, width, height, offset * -1);
    }

    int top = width - offset;
    int position_rendered_frame = -offset * 3 -3;
    int position_buffer_frame = -3;
    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        position_rendered_frame += offset * 3;
        position_buffer_frame += offset * 3;
        for (int column = 0; column < top; column++) {
            position_rendered_frame += 3;
            position_buffer_frame += 3;
            memcpy(buffer_frame + position_rendered_frame, buffer_frame + position_buffer_frame, 3);
        }
    }
    for (int row = 0; row < height; row++) {
        for (int column = width - offset; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            buffer_frame[position_rendered_frame] = 255;
            buffer_frame[position_rendered_frame + 1] = 255;
            buffer_frame[position_rendered_frame + 2] = 255;
        }
    }
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                               int rotate_iteration) {
    int c = 0; 
     // handle negative offsets
    if (rotate_iteration < 0){
        c = 4 - abs(rotate_iteration)%4;
    }else{
        c = rotate_iteration % 4;
    }

    int top = 0;
    int right = 0;
    int bottom = 0;
    int left = 0;
    int x = 0;
    int y = 0;
    
   
    if(c == 1){
         for (int x = 0; x < width / 2; x++){ 
            for (int y = x; y < height -x -1; y++){             
                //temp is top//
                top = x * width * 3 + y * 3;
                right = y * width * 3 + (width - x - 1) * 3;
                bottom = (width -1 -x ) * width * 3 + (width - y - 1) * 3;
                left = (width - 1 -y) * width * 3 + x * 3;
                memcpy(temp, buffer_frame + top, 3);
                memcpy(buffer_frame + top, buffer_frame + left, 3);
                memcpy(buffer_frame + left, buffer_frame + bottom, 3);
                memcpy(buffer_frame + bottom, buffer_frame + right, 3);
                memcpy(buffer_frame + right, temp, 3);
            } 
        }        
    }else if(c == 2 ){
        for (int x = 0; x < width / 2; x++){ 
            for (int y = x; y < height -x -1; y++){             
                //temp is top//
                top = x * width * 3 + y * 3;
                right = y * width * 3 + (width - x - 1) * 3;
                bottom = (width -1 -x ) * width * 3 + (width - y - 1) * 3;
                left = (width - 1 -y) * width * 3 + x * 3;
                memcpy(temp, buffer_frame + top, 3);
                memcpy(buffer_frame + top, buffer_frame + bottom, 3);
                memcpy(buffer_frame + bottom, temp, 3);
                memcpy(temp, buffer_frame + right, 3);
                memcpy(buffer_frame + right, buffer_frame + left, 3);
                memcpy(buffer_frame + left, temp, 3);                      
            } 
        }  
    }else if(c == 3){
        for (int x = 0; x < width / 2; x++){ 
            for (int y = x; y < height -x -1; y++){             
                //temp is top//
                top = x * width * 3 + y * 3;
                right = y * width * 3 + (width - x - 1) * 3;
                bottom = (width -1 -x ) * width * 3 + (width - y - 1) * 3;
                left = (width - 1 -y) * width * 3 + x * 3;
                memcpy(temp, buffer_frame + top, 3);
                memcpy(buffer_frame + top, buffer_frame + right, 3);
                memcpy(buffer_frame + right, buffer_frame + bottom, 3);
                memcpy(buffer_frame + bottom, buffer_frame + left, 3);
                memcpy(buffer_frame + left, temp, 3);
            } 
        }  
    }
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer counter clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                                int rotate_iteration) {
    int c = 0;
    if (rotate_iteration < 0){
        c = abs(rotate_iteration)%4;
        buffer_frame = processRotateCW(buffer_frame, width, height, c);
    } else {
        c = 4 - abs(rotate_iteration)%4;
        buffer_frame = processRotateCW(buffer_frame, width, height, c);
    }
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorX(unsigned char *buffer_frame, unsigned int width, unsigned int height, int _unused) {

    int start = 0;
    int end = height - 1;
    unsigned char* low;
    unsigned char* high;
    unsigned char t[width * 3];
    while(start < end){
        low = buffer_frame + start * width * 3;
        high = buffer_frame + end * width * 3;
        memcpy(t, low, width * 3);
        memcpy(low, high, width * 3);
        memcpy(high, t, width * 3);
        start ++;
        end --;
    }

    return buffer_frame;
}

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
void doTranslation(unsigned char *frame_buffer, unsigned char *rendered_frame, unsigned width, unsigned height, unsigned int orientation, int translated_x, int translated_y){
    int signed_width = (int)width, signed_height = (int)height, x_start = -(signed_width/2);
    int frame_t_x = x_start, frame_t_y = (signed_height/2), x_bound = (signed_width/2), y_bound = -(signed_height/2);
    int _x = 0, _y = 0, ori_y = 0,temp = 0 , x_c = 0, y_c =0, buffer_x = 0, buffer_y = 0;
    bool swap = false;
    x_c = getTranslatedCoords[orientation][0];
    y_c = getTranslatedCoords[orientation][1];
    swap = (getTranslatedCoords[orientation][2] == 1)?true:false;
    for(; frame_t_y > y_bound; --frame_t_y){
        for(frame_t_x = x_start; frame_t_x < x_bound; ++frame_t_x){
            //Change sign first
            _x = x_c * frame_t_x;
            _y = y_c * frame_t_y;
            //Need to plus 1 or minus one
            //Because x has one less positive number
            //And y has one less negative number
            _x = (x_c < 0)?(_x - 1):_x;
            _y = (y_c < 0)?(_y + 1):_y;
            //Need to minus x by one and plus y by one for the similar reason
            if(swap){
                temp = _x;
                _x = _y;
                _y = temp;
                _x--;
                _y++;
            }
            //Add the translate amount//
            _x += translated_x;
            _y += translated_y;
            //Translate to array coordinate
            _y = -_y + (int)height/2;
            _x += x_bound;
            
            buffer_x = frame_t_x + x_bound;
            buffer_y = -frame_t_y + (int)height/2;
            if(_x >= 0 && _x < width && _y >= 0 && _y < height){
                memcpy(rendered_frame + _y * width * 3 + _x * 3, frame_buffer + buffer_y * width * 3 + buffer_x * 3 ,3);
            }
        }
    }
    
}
void erase(unsigned char *rendered_frame, unsigned width, unsigned height, unsigned int orientation, int translated_x, int translated_y){
 int signed_width = (int)width, signed_height = (int)height;
    int frame_t_x = -(int)((int)signed_width/(int)2), frame_t_y = (int)((int)signed_height/(int)2), x_bound = (int)((int)signed_width/(int)2), y_bound = -(int)((int)signed_height/(int)2);
    int _x = 0, _y = 0, ori_y = 0,temp = 0 , x_c = 0, y_c =0, buffer_x = 0, buffer_y = 0;
    bool swap = false;
    x_c = getTranslatedCoords[orientation][0];
    y_c = getTranslatedCoords[orientation][1];
    swap = (getTranslatedCoords[orientation][2] == 1)?true:false;
    for(; frame_t_y > y_bound; --frame_t_y){
        for(frame_t_x = -(int)((int)signed_width/(int)2); frame_t_x < x_bound; ++frame_t_x){
            //Change sign first
            _x = x_c * frame_t_x;
            _y = y_c * frame_t_y;
            //Need to plus 1 or minus one
            //Because x has one less positive number
            //And y has one less negative number
            _x = (x_c < 0)?(_x - 1):_x;
            _y = (y_c < 0)?(_y + 1):_y;
            //Need to minus x by one and plus y by one for the similar reason
            if(swap){
                temp = _x;
                _x = _y;
                _y = temp;
                _x--;
                _y++;
            }
            //Add the translate amount//
            _x += translated_x;
            _y += translated_y;
            //Translate to array coordinate
            _y = -_y + (int)height/2;
            _x += x_bound;
            
            buffer_x = frame_t_x + x_bound;
            buffer_y = -frame_t_y + (int)height/2;
            if(_x >= 0 && _x < width && _y >= 0 && _y < height){
                memset(rendered_frame + _y * width * 3 + _x * 3,255,3);
            }
        }
    }
}
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    
    //Orientations: 0 : 01    1: 10     2: 23       3: 32       4: 02    5: 20      6: 31       7: 13   
//                  23       32        01          10          13       31         20          02
//Actions: 0    1     2     3     4
//         90   -90   180   MX    MY
    int x_d = width/2, y_d = height/2;
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
        }
    }
    return;
}

//    int processed_frames = 0;
//    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
////        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
////               sensor_values[sensorValueIdx].value);
//        if (!strcmp(sensor_values[sensorValueIdx].key, "W")) {
//            frame_buffer = processMoveUp(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
////            printBMP(width, height, frame_buffer);
//        } else if (!strcmp(sensor_values[sensorValueIdx].key, "A")) {
//            frame_buffer = processMoveLeft(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
////            printBMP(width, height, frame_buffer);
//        } else if (!strcmp(sensor_values[sensorValueIdx].key, "S")) {
//            frame_buffer = processMoveDown(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
////            printBMP(width, height, frame_buffer);
//        } else if (!strcmp(sensor_values[sensorValueIdx].key, "D")) {
//            frame_buffer = processMoveRight(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
////            printBMP(width, height, frame_buffer);
//        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CW")) {
//            frame_buffer = processRotateCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
////            printBMP(width, height, frame_buffer);
//        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CCW")) {
//            frame_buffer = processRotateCCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
////            printBMP(width, height, frame_buffer);
//        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MX")) {
//            frame_buffer = processMirrorX(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
////            printBMP(width, height, frame_buffer);
//        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MY")) {
//            frame_buffer = processMirrorY(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
////            printBMP(width, height, frame_buffer);
//        }
//        processed_frames += 1;
//        if (processed_frames % 25 == 0) {
//            verifyFrame(frame_buffer, width, height, grading_mode);
//        }
//    }
