#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define PATH_X "/sys/bus/iio/devices/iio:device2/in_accel_x_raw"
#define PATH_Y "/sys/bus/iio/devices/iio:device2/in_accel_y_raw"
#define PATH_Z "/sys/bus/iio/devices/iio:device2/in_accel_z_raw"

int read_raw(const char *path)
{
    int val = 0;
    FILE *fp = fopen(path, "r");
    if (fp)
    {
        fscanf(fp, "%d", &val);
        fclose(fp);
    }
    else
    {
        fprintf(stderr, "Error: No se pudo abrir %s\n", path);
        exit(EXIT_FAILURE);
    }
    return val;
}

int main()
{
    int x, y, z;
    int prev_x, prev_y, prev_z;
    int threshold = 80000;

    prev_x = read_raw(PATH_X);
    prev_y = read_raw(PATH_Y);
    prev_z = read_raw(PATH_Z);

    while (1)
    {
        x = read_raw(PATH_X);
        y = read_raw(PATH_Y);
        z = read_raw(PATH_Z);

        int diff_x = abs(x - prev_x);
        int diff_y = abs(y - prev_y);
        int diff_z = abs(z - prev_z);

        printf("Raw -> X:%d Y:%d Z:%d | Diff -> X:%d Y:%d Z:%d\n", x, y, z, diff_x, diff_y, diff_z);

        if (diff_x > threshold || diff_y > threshold || diff_z > threshold)
        {
            printf("Impact -> Diff X:%d Y:%d Z:%d\n", diff_x, diff_y, diff_z);

            system("mpg123 -q HENTAI.mp3 &");

            sleep(2);

            prev_x = read_raw(PATH_X);
            prev_y = read_raw(PATH_Y);
            prev_z = read_raw(PATH_Z);
            continue;
        }

        prev_x = x;
        prev_y = y;
        prev_z = z;

        usleep(100000);
    }

    return 0;
}