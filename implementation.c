#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

//0             1            2
//invert x      invert y     swap flag
short getTranslatedCoords[8][3] = {
    {1, 1, 0},
    {-1, 1, 0},
    {1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 1},
    {-1, 1, 1},
    {1, 1, 1},
    {1, -1, 1}
};
int* colored_points;
unsigned char** dirty_points;
unsigned char* colors;

void print_team_info() {
    // Please modify this field with something interesting
    char team_name[] = "running in the 90s";

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

void copyAndDraw(unsigned char* buffer_frame, int orientation, int offset, int counter, int width, int translated_x, int translated_y) {
    int ref_x = 0, ref_y = 0, temp = 0, s_counter = 0;
    switch (orientation) {
        case 0:
            for (int i = 0; i < counter; i += 2) {
                ref_x = colored_points[i];
                ref_y = colored_points[i + 1];
                ref_x += translated_x;
                ref_y += translated_y;
                ref_x += offset;
                ref_y = -ref_y + offset;
                dirty_points[i/2] = buffer_frame + (unsigned int)(ref_y * width * 3 + ref_x * 3);
                memcpy( dirty_points[i/2], colors + (i / 2) * 3, 3);
            }
            break;
        case 1:
            for (int i = 0; i < counter; i += 2) {
                ref_x = colored_points[i];
                ref_y = colored_points[i + 1];
                ref_x = -ref_x - 1;
                ref_x += translated_x;
                ref_y += translated_y;
                ref_x += offset;
                ref_y = -ref_y + offset;
                dirty_points[i/2] = buffer_frame + (unsigned int)(ref_y * width * 3 + ref_x * 3);
                memcpy(dirty_points[i/2], colors + (i / 2) * 3, 3);
            }
            break;
        case 2:
            for (int i = 0; i < counter; i += 2) {

                ref_x = colored_points[i];
                ref_y = colored_points[i + 1];
                ref_y = -ref_y + 1;
                ref_x += translated_x;
                ref_y += translated_y;
                ref_x += offset;
                ref_y = -ref_y + offset;
                dirty_points[i/2] = buffer_frame + (unsigned int)(ref_y * width * 3 + ref_x * 3);
                memcpy(dirty_points[i/2], colors + (i / 2) * 3, 3);
            }

            break;
        case 3:
            for (int i = 0; i < counter; i += 2) {
                ref_x = colored_points[i];
                ref_y = colored_points[i + 1];
                ref_x = -ref_x - 1;
                ref_y = -ref_y + 1;
                ref_x += translated_x;
                ref_y += translated_y;
                ref_x += offset;
                ref_y = -ref_y + offset;
                dirty_points[i/2] = buffer_frame + (unsigned int)(ref_y * width * 3 + ref_x * 3);
                memcpy(dirty_points[i/2], colors + (i / 2) * 3, 3);
            }

            break;
        case 4:
            for (int i = 0; i < counter; i += 2) {
                ref_x = colored_points[i];
                ref_y = colored_points[i + 1];
                ref_x = -ref_x - 1;
                ref_y = -ref_y + 1;
                temp = ref_x;
                ref_x = ref_y;
                ref_y = temp;
                ref_x--;
                ref_y++;
                ref_x += translated_x;
                ref_y += translated_y;
                ref_x += offset;
                ref_y = -ref_y + offset;
                dirty_points[i/2] = buffer_frame + (unsigned int)(ref_y * width * 3 + ref_x * 3);
                memcpy(dirty_points[i/2], colors + (i / 2) * 3, 3);
            }

            break;
        case 5:
            for (int i = 0; i < counter; i += 2) {
                ref_x = colored_points[i];
                ref_y = colored_points[i + 1];
                ref_x = -ref_x - 1;
                temp = ref_x;
                ref_x = ref_y;
                ref_y = temp;
                ref_x--;
                ref_y++;
                ref_x += translated_x;
                ref_y += translated_y;
                ref_x += offset;
                ref_y = -ref_y + offset;
                dirty_points[i/2] = buffer_frame + (unsigned int)(ref_y * width * 3 + ref_x * 3);
                memcpy(dirty_points[i/2], colors + (i / 2) * 3, 3);
            }
            break;
        case 6:
            for (int i = 0; i < counter; i += 2) {
                ref_x = colored_points[i];
                ref_y = colored_points[i + 1];
                temp = ref_x;
                ref_x = ref_y;
                ref_y = temp;
                ref_x--;
                ref_y++;
                ref_x += translated_x;
                ref_y += translated_y;
                ref_x += offset;
                ref_y = -ref_y + offset;
                dirty_points[i/2] = buffer_frame + (unsigned int)(ref_y * width * 3 + ref_x * 3);
                memcpy(dirty_points[i/2], colors + (i / 2) * 3, 3);
            }
            break;
        case 7:
            for (int i = 0; i < counter; i += 2) {
                ref_x = colored_points[i];
                ref_y = colored_points[i + 1];
                ref_y = -ref_y + 1;
                temp = ref_x;
                ref_x = ref_y;
                ref_y = temp;
                ref_x--;
                ref_y++;
                ref_x += translated_x;
                ref_y += translated_y;
                ref_x += offset;
                ref_y = -ref_y + offset;
                dirty_points[i/2] = buffer_frame + (unsigned int)(ref_y * width * 3 + ref_x * 3);
                memcpy(dirty_points[i/2], colors + (i / 2) * 3, 3);
            }

            break;

    }
}

void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
        unsigned int width, unsigned int height, bool grading_mode) {

    //Orientations: 0 : 01    1: 10     2: 23       3: 32       4: 02    5: 20      6: 31       7: 13   
    //                  23       32        01          10          13       31         20          02
    //Actions: 0    1     2     3     4
    //         90   -90   180   MX    MY
    colored_points = (int *) malloc(height * width * sizeof (int) * 2);
    colors = (unsigned char*) malloc(height * width * 3);
    register unsigned int orientation = 0;
    unsigned char white = (unsigned char) 255;
    int counter = 0;
    int signed_width = (int) width/2;
    int signed_height = (int) height/2;
    unsigned int states[][5] = {
        {5, 7, 3, 2, 1},
        {6, 4, 2, 3, 0},
        {4, 6, 1, 0, 3},
        {7, 5, 0, 1, 2},
        {1, 2, 6, 7, 5},
        {3, 0, 7, 6, 4},
        {2, 1, 4, 5, 7},
        {0, 3, 5, 4, 6}
    };

    //X direction move multiply with 0,2
    //Y direction move multiply with 1,3
    //Actions: 0    1     2     3     4
    //         90   -90   180   MX    MY
    register int translated_x = 0;
    register int translated_y = 0;
    int x_f = -1, y_f = -1, processed_frames = 0;
    unsigned s_counter = 0, pos = 0;
    for (int j = signed_width; j > -signed_width; j--) {
        y_f++;
        x_f = -1;
        for (int i = -signed_height; i < signed_height; i++) {
            x_f++;
            pos = y_f * width * 3 + 3 * x_f;
            if (frame_buffer[pos] != white || frame_buffer[pos + 1] != white || frame_buffer[pos + 2] != white) {
                colored_points[counter] = i;
                colored_points[counter + 1] = j;
                memcpy(colors + s_counter * 3, frame_buffer + pos, 3);
                memset(frame_buffer + pos, 0xff, 3);
                s_counter++;
                counter += 2;
            }

        }
    }
    dirty_points = (unsigned char**) malloc(s_counter * sizeof(unsigned char*));
   
    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
        int value = sensor_values[sensorValueIdx].value;
        if (!strcmp(sensor_values[sensorValueIdx].key, "W")) {
            translated_y += value;
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "S")) {
            translated_y -= value;
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "D")) {
            translated_x += value;
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "A")) {
            translated_x -= value;
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MX")) {
            translated_y = -translated_y;
            orientation = states[orientation][3];
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MY")) {
            translated_x = -translated_x;
            orientation = states[orientation][4];
        } else {
            if (!strcmp(sensor_values[sensorValueIdx].key, "CCW")) value = -value;
            int abs_value = abs(value);
            abs_value = abs_value % 4;
            if (value < 0) abs_value = 4 - abs_value;
            if (abs_value == 1) {
                int temp = translated_x;
                translated_x = translated_y;
                translated_y = -temp;
                orientation = states[orientation][0];
            } else if (abs_value == 2) {
                translated_x = -translated_x;
                translated_y = -translated_y;
                orientation = states[orientation][2];
            } else if (abs_value == 3) {
                int temp = translated_x;
                translated_x = -translated_y;
                translated_y = temp;
                orientation = states[orientation][1];
            }
        }
        processed_frames += 1;
        if (processed_frames % 25 == 0) {
            copyAndDraw(frame_buffer, orientation, width / 2, counter, width, translated_x, translated_y);
            verifyFrame(frame_buffer, width, height, grading_mode);
            for(unsigned int i=0 ; i<s_counter; i++){
                memset(dirty_points[i],0xff,3);
            }       
        }
    }
    return;
}
