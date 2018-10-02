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
static unsigned char * rendered_frame = NULL;
unsigned char temp[3];
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    if (offset < 0){
        return processMoveDownReference(buffer_frame, width, height, offset * -1);
    }
    int top = width - offset;
    int l = 0;
    unsigned char* d = buffer_frame - width * 3;
    unsigned char* s = buffer_frame + offset * width * 3 - width * 3;
    while(l < top){
        d += width * 3;
        s += width * 3;
        memcpy(d,s,width * 3);
        l++;
    }
    int position_rendered_frame = top * 3 - 3;
    for (int row = top; row < height; row++) {
        for (int column = 0; column < width; column++) {
            position_rendered_frame += 3;
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
        return processMoveLeftReference(buffer_frame, width, height, offset * -1);
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
        return processMoveUpReference(buffer_frame, width, height, offset * -1);
    }

    int position_rendered_frame = height * width * 3 - 1;
    int position_buffer_frame = height * width * 3 - offset * width * 3 - 1;
    // store shifted pixels to temporary buffer
    for (int row = 0; row < height - offset; row++) {
        for (int column = 0; column < width; column++) {
            position_rendered_frame -= 3;
            position_buffer_frame -= 3;
            memcpy(buffer_frame + position_rendered_frame, buffer_frame + position_buffer_frame, 3);
        }
    }

    // fill left over pixels with white pixels
    position_rendered_frame = -3;
     for (int row = 0; row < offset; row++) {
        for (int column = 0; column < width; column++) {
            position_rendered_frame +=3;
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
 * @param offset - number of pixels to shift the object in bitmap image right
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
   // handle negative offsets
    if (offset < 0){
        return processMoveRightReference(buffer_frame, width, height, offset * -1);
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
unsigned char *processMirrorY(unsigned char *buffer_frame, unsigned width, unsigned height, int _unused) {
        // allocate memory for temporary image buffer
    if(rendered_frame == NULL){
        rendered_frame = allocateFrame(width, height);
    }

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * height * 3 + column * 3;
            int position_buffer_frame = row * height * 3 + (width - column - 1) * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    //deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;
}

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
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    int processed_frames = 0;
    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
//        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
//               sensor_values[sensorValueIdx].value);
        if (!strcmp(sensor_values[sensorValueIdx].key, "W")) {
            frame_buffer = processMoveUp(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "A")) {
            frame_buffer = processMoveLeft(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "S")) {
            frame_buffer = processMoveDown(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "D")) {
            frame_buffer = processMoveRight(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CW")) {
            frame_buffer = processRotateCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CCW")) {
            frame_buffer = processRotateCCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MX")) {
            frame_buffer = processMirrorX(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MY")) {
            frame_buffer = processMirrorY(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
//            printBMP(width, height, frame_buffer);
        }
        processed_frames += 1;
        if (processed_frames % 25 == 0) {
            verifyFrame(frame_buffer, width, height, grading_mode);
        }
    }
    return;
}
