#include <sys/types.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <linux/videodev2.h>

int xioctl(int fd, int request, void *arg)
{
    for (int i = 0; i < 100; i++)
    {
        int r = ioctl(fd, request, arg);
        if (r != -1 || errno != EINTR)
            return r;
    }
    return -1;
}

const char* pixelfmts[4] = {"YUYV", "UYVY", "YVYU", "VYUY"};

unsigned int pixelformats[4] = {V4L2_PIX_FMT_YUYV,V4L2_PIX_FMT_UYVY,V4L2_PIX_FMT_YVYU,V4L2_PIX_FMT_VYUY};

int main(int argc, char **argv)
{
	int ret;
	int fd;
	char *device;
	device = strdup(argv[1]);
    unsigned int pixelformat;

    fd = open(argv[1], O_RDWR | O_NONBLOCK, 0);
    if (fd == -1)
    {
        printf("open %s failed\n", argv[1]);
        return -1;
    }
    printf("open %s Successed, fd = %d\n", argv[1], fd);


    //Try to set picture format, drvier can modify params
    struct v4l2_format format;
    memset(&format, 0, sizeof format);
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
    format.fmt.pix.field = V4L2_FIELD_ANY;

	//Get picture format
	if(xioctl(fd, VIDIOC_G_FMT, &format) == -1)
		printf("VIDIOC_G_FMT falied\n");
	printf("{ pixelformat = '%c%c%c%c'}\n",
           format.fmt.pix.pixelformat & 0xFF, (format.fmt.pix.pixelformat >> 8) & 0xFF,
           (format.fmt.pix.pixelformat >> 16) & 0xFF, (format.fmt.pix.pixelformat >> 24) & 0xFF);
    if(argc == 3)
    {
        for(int i=0; i<4; i++)
        {
            if(0 == strcmp(argv[2],pixelfmts[i]))
            {
                pixelformat = pixelformats[i];
                printf("%s\n", argv[2]);
                break;
            }
        }
         //Set picture format
        format.fmt.pix.pixelformat = pixelformat;
        if (xioctl(fd, VIDIOC_S_FMT, &format) == -1)
            printf("VIDIOC_S_FMT falied\n");

        //Get picture format
        if(xioctl(fd, VIDIOC_G_FMT, &format) == -1)
            printf("VIDIOC_G_FMT falied\n");

        printf("{ pixelformat = '%c%c%c%c'}\n",
            format.fmt.pix.pixelformat & 0xFF, (format.fmt.pix.pixelformat >> 8) & 0xFF,
            (format.fmt.pix.pixelformat >> 16) & 0xFF, (format.fmt.pix.pixelformat >> 24) & 0xFF);
    }
   	
}