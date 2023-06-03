# icn2153_test

for 128x64 pixel matrix display based using icn2153
board detailed described here:
http://kuku.eu.org/?projects/hub75e/index

based on
https://bitbucket.org/nadyrshin_ryu/icn2053_esp32_demo/src/master/

using that code the matrix shows only some fragments, but changing:

void IRAM_ATTR icn2053_scan_next()
{
	icn2053_SetScan(scan++);
	scan &= 31;
}

to
void IRAM_ATTR icn2053_scan_next()
{
    if(scan == 0) {
        icn2053_SetScan(4);
        icn2053_SetScan(5);
        icn2053_SetScan(4);
        icn2053_SetScan(0);

    } else {
		icn2053_SetScan(1);
        icn2053_SetScan(0);
    }
    scan = (scan+1)&31;
	
}

makes the board fully working!

Problem: it runs only with 7fps. Good for slower updated data, useless for movies...


Searching for faster library...