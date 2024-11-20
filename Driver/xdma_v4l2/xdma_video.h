#include <linux/mutex.h>
#include <linux/pci.h>
#include <linux/timer.h>
#include <linux/videodev2.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/videobuf2-v4l2.h>
#include <sound/pcm.h>

#define MAGIC_VIDEO     	0xAAAAAAAAUL
#define TYPE_MAX_CHANNELS	0x0f

#if defined(RHEL_RELEASE_CODE)
#	define PCI_VFL_TYPECHANGE (RHEL_RELEASE_CODE >= RHEL_RELEASE_VERSION(8, 3))
#else
#	define PCI_VFL_TYPECHANGE (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 7, 0))
#endif

struct xdma_video_format {
	char *name;
	unsigned int fourcc;
	unsigned int depth;
};

struct xdma_video_v4l2_buf {
	struct vb2_v4l2_buffer vb;
	struct list_head list;
	
	struct user_frame_request * req[3];
};

struct video_kthread {
	spinlock_t lock;
	struct task_struct *task;
	volatile unsigned int streaming_started;
	volatile unsigned int streaming_running;
	volatile unsigned int dummy_frame;
	volatile unsigned int trigger_started;
	wait_queue_head_t stopped_wq;
};

struct xdma_video_channel {
	struct xdma_video_dev *dev;
	struct vb2_queue vidq;
	struct list_head vidq_queued;
	struct video_device *device;
	struct video_kthread kthread;

	struct v4l2_ctrl_handler ctrl_handler;
	const struct xdma_video_format *format;
	struct mutex vb_mutex;
	spinlock_t qlock;
	v4l2_std_id video_standard;
	unsigned int width, height;
	unsigned int ch;
	unsigned int num;
	unsigned int fps;
	unsigned int input;
	unsigned int sequence;
	
	int irq_count;
	int events_irq;			
	spinlock_t events_lock;		
	wait_queue_head_t events_wq;		
	unsigned int frame_loss;
	unsigned int frame_sync_loss;
	unsigned int trig_count;
	unsigned int frame_count;
	u64 trig_timestamp1;
	u64 trig_timestamp2;
	u64 timestamp_xdma_xfr_userirq;
	u64 timestamp_xdma_xfr_grab_one;
	u64 timestamp_xdma_xfr_channelirq;
	u64 timestamp_xdma_xfr_grab_done;
};

struct xdma_video_dev {
	unsigned long magic;		/* structure ID for sanity checks */
	struct xdma_pci_dev *xpdev;
	struct xdma_dev *xdev;
	struct pci_dev *pci_dev;
	struct v4l2_device v4l2_dev;
	struct xdma_video_channel *video_channels;
	unsigned int channels;
	char name[32];
	enum v4l2_field field;
	spinlock_t lock;
	struct transfer_monitor * engine_monitor;
};

struct transfer_monitor {
	struct xdma_engine *engine;
	struct task_struct *task;
	spinlock_t lock;
	struct list_head transfer_list;
	wait_queue_head_t wq;
	unsigned int num_element;
};

struct user_frame_request {
	//struct xdma_video_channel *vc;
	//struct xdma_video_v4l2_buf *buf;

	struct list_head entry;
	unsigned int transfer_finished;
	wait_queue_head_t finish_wq;
	spinlock_t lock;	

	struct sg_table *sgt;
	u64 ep_addr;
	
};

static inline unsigned int max_channels(struct xdma_video_dev *dev)
{
	return dev->channels;
}

int xdma_video_add(struct xdma_pci_dev *xpdev);
int xdma_video_remove(struct xdma_pci_dev *xpdev);
void xdma_video_buf_done(struct xdma_video_channel *vc, struct xdma_video_v4l2_buf *buf);

