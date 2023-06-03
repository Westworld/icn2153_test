#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"


uint16_t ColorCoefs[256] =
{
0, 8, 16, 24, 32, 40, 48, 56,
64, 72, 80, 88, 96, 104, 112, 120,
128, 134, 142, 150, 158, 166, 174, 182,
190, 198, 206, 214, 222, 230, 238, 246,
254, 262, 278, 294, 310, 326, 342, 358,
374, 388, 420, 444, 476, 500, 532, 572,
604, 636, 674, 714, 754, 794, 842, 882,
928, 976, 1023, 1079, 1127, 1181, 1237, 1293,
1349, 1411, 1475, 1539, 1603, 1665, 1737, 1809,
1881, 1951, 2031, 2111, 2189, 2269, 2357, 2435,
2531, 2619, 2705, 2801, 2897, 2999, 3094, 3196,
3300, 3404, 3514, 3626, 3736, 3856, 3974, 4094,
4212, 4340, 4466, 4594, 4720, 4856, 4990, 5133,
5267, 5419, 5561, 5713, 5863, 6013, 6173, 6331,
6490, 6649, 6815, 6990, 7156, 7330, 7513, 7688,
7870, 8060, 8244, 8434, 8632, 8822, 9021, 9227,
9433, 9639, 9853, 10060, 10283, 10497, 10727, 10949,
11178, 11408, 11646, 11884, 12122, 12368, 12615, 12861,
13115, 13376, 13630, 13899, 14161, 14431, 14709, 14979,
15264, 15542, 15828, 16122, 16415, 16708, 17010, 17311,
17621, 17930, 18239, 18557, 18883, 19208, 19532, 19866,
20199, 20540, 20882, 21223, 21571, 21929, 22286, 22643,
23008, 23380, 23753, 24126, 24508, 24888, 25277, 25665,
26062, 26459, 26864, 27269, 27680, 28093, 28514, 28935,
29363, 29791, 30219, 30664, 31101, 31552, 32004, 32457,
32917, 33378, 33845, 34313, 34790, 35274, 35757, 36241,
36733, 37233, 37732, 38240, 38748, 39264, 39779, 40303,
40827, 41359, 41898, 42438, 42985, 43533, 44087, 44643,
45207, 45777, 46349, 46920, 47508, 48086, 48681, 49277,
49871, 50482, 51086, 51704, 52323, 52942, 53569, 54203,
54838, 55481, 56131, 56781, 57441, 58098, 58766, 59440,
60113, 60797, 61487, 62176, 62866, 63574, 64279, 64985
};

/*
uint16_t ColorCoefs[256] =
{
0x0000, 0x01FF, 0x02FF, 0x03FF, 0x04FF, 0x05FF, 0x06FF, 0x07FF,
0x08FF, 0x09FF, 0x0AFF, 0x0BFF, 0x0CFF, 0x0DFF, 0x0EFF, 0x0FFF,
0x10FF, 0x11FF, 0x12FF, 0x13FF, 0x14FF, 0x15FF, 0x16FF, 0x17FF,
0x18FF, 0x19FF, 0x1AFF, 0x1BFF, 0x1CFF, 0x1DFF, 0x1EFF, 0x1FFF,
0x20FF, 0x21FF, 0x22FF, 0x23FF, 0x24FF, 0x25FF, 0x26FF, 0x27FF,
0x28FF, 0x29FF, 0x2AFF, 0x2BFF, 0x2CFF, 0x2DFF, 0x2EFF, 0x2FFF,
0x30FF, 0x31FF, 0x32FF, 0x33FF, 0x34FF, 0x35FF, 0x36FF, 0x37FF,
0x38FF, 0x39FF, 0x3AFF, 0x3BFF, 0x3CFF, 0x3DFF, 0x3EFF, 0x3FFF,
0x40FF, 0x41FF, 0x42FF, 0x43FF, 0x44FF, 0x45FF, 0x46FF, 0x47FF,
0x48FF, 0x49FF, 0x4AFF, 0x4BFF, 0x4CFF, 0x4DFF, 0x4EFF, 0x4FFF,
0x50FF, 0x51FF, 0x52FF, 0x53FF, 0x54FF, 0x55FF, 0x56FF, 0x57FF,
0x58FF, 0x59FF, 0x5AFF, 0x5BFF, 0x5CFF, 0x5DFF, 0x5EFF, 0x5FFF,
0x60FF, 0x61FF, 0x62FF, 0x63FF, 0x64FF, 0x65FF, 0x66FF, 0x67FF,
0x68FF, 0x69FF, 0x6AFF, 0x6BFF, 0x6CFF, 0x6DFF, 0x6EFF, 0x6FFF,
0x70FF, 0x71FF, 0x72FF, 0x73FF, 0x74FF, 0x75FF, 0x76FF, 0x77FF,
0x78FF, 0x79FF, 0x7AFF, 0x7BFF, 0x7CFF, 0x7DFF, 0x7EFF, 0x7FFF,
0x80FF, 0x81FF, 0x82FF, 0x83FF, 0x84FF, 0x85FF, 0x86FF, 0x87FF,
0x88FF, 0x89FF, 0x8AFF, 0x8BFF, 0x8CFF, 0x8DFF, 0x8EFF, 0x8FFF,
0x90FF, 0x91FF, 0x92FF, 0x93FF, 0x94FF, 0x95FF, 0x96FF, 0x97FF,
0x98FF, 0x99FF, 0x9AFF, 0x9BFF, 0x9CFF, 0x9DFF, 0x9EFF, 0x9FFF,
0xA0FF, 0xA1FF, 0xA2FF, 0xA3FF, 0xA4FF, 0xA5FF, 0xA6FF, 0xA7FF,
0xA8FF, 0xA9FF, 0xAAFF, 0xABFF, 0xACFF, 0xADFF, 0xAEFF, 0xAFFF,
0xB0FF, 0xB1FF, 0xB2FF, 0xB3FF, 0xB4FF, 0xB5FF, 0xB6FF, 0xB7FF,
0xB8FF, 0xB9FF, 0xBAFF, 0xBBFF, 0xBCFF, 0xBDFF, 0xBEFF, 0xBFFF,
0xC0FF, 0xC1FF, 0xC2FF, 0xC3FF, 0xC4FF, 0xC5FF, 0xC6FF, 0xC7FF,
0xC8FF, 0xC9FF, 0xCAFF, 0xCBFF, 0xCCFF, 0xCDFF, 0xCEFF, 0xCFFF,
0xD0FF, 0xD1FF, 0xD2FF, 0xD3FF, 0xD4FF, 0xD5FF, 0xD6FF, 0xD7FF,
0xD8FF, 0xD9FF, 0xDAFF, 0xDBFF, 0xDCFF, 0xDDFF, 0xDEFF, 0xDFFF,
0xE0FF, 0xE1FF, 0xE2FF, 0xE3FF, 0xE4FF, 0xE5FF, 0xE6FF, 0xE7FF,
0xE8FF, 0xE9FF, 0xEAFF, 0xEBFF, 0xECFF, 0xEDFF, 0xEEFF, 0xEFFF,
0xF0FF, 0xF1FF, 0xF2FF, 0xF3FF, 0xF4FF, 0xF5FF, 0xF6FF, 0xF7FF,
0xF8FF, 0xF9FF, 0xFAFF, 0xFBFF, 0xFCFF, 0xFDFF, 0xFEFF, 0xFFFF
};
*/