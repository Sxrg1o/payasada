#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libudev.h>
#include <sys/select.h>

int main()
{
    struct udev *udev;
    struct udev_monitor *mon;
    int fd;

    udev = udev_new();
    if (!udev)
    {
        fprintf(stderr, "Error: No se pudo crear el contexto udev.\n");
        exit(EXIT_FAILURE);
    }

    mon = udev_monitor_new_from_netlink(udev, "kernel");
    if (!mon)
    {
        fprintf(stderr, "Error: No se pudo crear el monitor udev.\n");
        udev_unref(udev);
        exit(EXIT_FAILURE);
    }

    udev_monitor_filter_add_match_subsystem_devtype(mon, "typec", NULL);
    udev_monitor_enable_receiving(mon);

    fd = udev_monitor_get_fd(mon);

    while (1)
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        int ret = select(fd + 1, &fds, NULL, NULL, NULL);

        if (ret > 0 && FD_ISSET(fd, &fds))
        {
            struct udev_device *dev = udev_monitor_receive_device(mon);
            if (dev)
            {
                const char *action = udev_device_get_action(dev);
                const char *sysname = udev_device_get_sysname(dev);

                if (action && strstr(sysname, "port1-partner"))
                {
                    if (strcmp(action, "add") == 0)
                    {
                        printf("CONECTADO %s\n", sysname);
                        if (fork() == 0)
                        {
                            execlp("paplay", "paplay", "HENTAI.wav", NULL);
                            exit(0);
                        }
                        sleep(1);
                    }
                    else if (strcmp(action, "remove") == 0)
                    {
                        printf("DESCONECTADO %s\n", sysname);
                        if (fork() == 0)
                        {
                            execlp("paplay", "paplay", "HENTAI.wav", NULL);
                            exit(0);
                        }
                        sleep(1);
                    }
                }
                udev_device_unref(dev);
            }
        }
    }

    udev_monitor_unref(mon);
    udev_unref(udev);
    return 0;
}