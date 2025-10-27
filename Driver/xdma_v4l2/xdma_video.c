#define pr_fmt(fmt) KBUILD_MODNAME ":%s: " fmt, __func__
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <media/v4l2-common.h>
#include <media/v4l2-event.h>
#include <media/videobuf2-dma-contig.h>
#include <media/videobuf2-dma-sg.h>
#include <media/videobuf2-vmalloc.h>
#include "xdma_mod.h"
#include "libxdma.h"
#include "libxdma_api.h"
#include "regs_user.h"

static const struct xdma_video_format formats[] = {
	{
		.fourcc = V4L2_PIX_FMT_YUYV,
		.depth = 16,
	},
	{
		.fourcc = V4L2_PIX_FMT_YVYU,
		.depth = 16,
	},
	{
		.fourcc = V4L2_PIX_FMT_UYVY,
		.depth = 16,
	},
	{
		.fourcc = V4L2_PIX_FMT_VYUY,
		.depth = 16,
	}};

/// Add by Liyunfeng 220111
static ssize_t xdma_channel_status_show0(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct xdma_pci_dev *xpdev = (struct xdma_pci_dev *)dev_get_drvdata(dev);
	struct xdma_video_channel *vc = &xpdev->video_dev.video_channels[0];

	return snprintf(buf, PAGE_SIZE, "irq_count=%d\nsequence=%d\nframe_count=%d\ntrig_count=%d\nframe_sync_loss=%d\nframe_loss=%d\n",
					vc->irq_count, vc->sequence, vc->frame_count, vc->trig_count, vc->frame_sync_loss, vc->frame_loss);
}

static ssize_t xdma_channel_status_show1(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct xdma_pci_dev *xpdev = (struct xdma_pci_dev *)dev_get_drvdata(dev);
	struct xdma_video_channel *vc = &xpdev->video_dev.video_channels[1];

	return snprintf(buf, PAGE_SIZE, "irq_count=%d\nsequence=%d\nframe_count=%d\ntrig_count=%d\nframe_sync_loss=%d\nframe_loss=%d\n",
					vc->irq_count, vc->sequence, vc->frame_count, vc->trig_count, vc->frame_sync_loss, vc->frame_loss);
}

static ssize_t xdma_channel_status_show2(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct xdma_pci_dev *xpdev = (struct xdma_pci_dev *)dev_get_drvdata(dev);
	struct xdma_video_channel *vc = &xpdev->video_dev.video_channels[2];

	return snprintf(buf, PAGE_SIZE, "irq_count=%d\nsequence=%d\nframe_count=%d\ntrig_count=%d\nframe_sync_loss=%d\nframe_loss=%d\n",
					vc->irq_count, vc->sequence, vc->frame_count, vc->trig_count, vc->frame_sync_loss, vc->frame_loss);
}

static ssize_t xdma_channel_status_show3(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct xdma_pci_dev *xpdev = (struct xdma_pci_dev *)dev_get_drvdata(dev);
	struct xdma_video_channel *vc = &xpdev->video_dev.video_channels[3];

	return snprintf(buf, PAGE_SIZE, "irq_count=%d\nsequence=%d\nframe_count=%d\ntrig_count=%d\nframe_sync_loss=%d\nframe_loss=%d\n",
					vc->irq_count, vc->sequence, vc->frame_count, vc->trig_count, vc->frame_sync_loss, vc->frame_loss);
}

static ssize_t xdma_channel_status_show4(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct xdma_pci_dev *xpdev = (struct xdma_pci_dev *)dev_get_drvdata(dev);
	struct xdma_video_channel *vc = &xpdev->video_dev.video_channels[4];

	return snprintf(buf, PAGE_SIZE, "irq_count=%d\nsequence=%d\nframe_count=%d\ntrig_count=%d\nframe_sync_loss=%d\nframe_loss=%d\n",
					vc->irq_count, vc->sequence, vc->frame_count, vc->trig_count, vc->frame_sync_loss, vc->frame_loss);
}

static ssize_t xdma_channel_status_show5(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct xdma_pci_dev *xpdev = (struct xdma_pci_dev *)dev_get_drvdata(dev);
	struct xdma_video_channel *vc = &xpdev->video_dev.video_channels[5];

	return snprintf(buf, PAGE_SIZE, "irq_count=%d\nsequence=%d\nframe_count=%d\ntrig_count=%d\nframe_sync_loss=%d\nframe_loss=%d\n",
					vc->irq_count, vc->sequence, vc->frame_count, vc->trig_count, vc->frame_sync_loss, vc->frame_loss);
}

static ssize_t xdma_channel_status_show6(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct xdma_pci_dev *xpdev = (struct xdma_pci_dev *)dev_get_drvdata(dev);
	struct xdma_video_channel *vc = &xpdev->video_dev.video_channels[6];

	return snprintf(buf, PAGE_SIZE, "irq_count=%d\nsequence=%d\nframe_count=%d\ntrig_count=%d\nframe_sync_loss=%d\nframe_loss=%d\n",
					vc->irq_count, vc->sequence, vc->frame_count, vc->trig_count, vc->frame_sync_loss, vc->frame_loss);
}

static ssize_t xdma_channel_status_show7(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct xdma_pci_dev *xpdev = (struct xdma_pci_dev *)dev_get_drvdata(dev);
	struct xdma_video_channel *vc = &xpdev->video_dev.video_channels[7];

	return snprintf(buf, PAGE_SIZE, "irq_count=%d\nsequence=%d\nframe_count=%d\ntrig_count=%d\nframe_sync_loss=%d\nframe_loss=%d\n",
					vc->irq_count, vc->sequence, vc->frame_count, vc->trig_count, vc->frame_sync_loss, vc->frame_loss);
}

static struct device_attribute dev_attr_channel[] = {
	{
		.attr = {
			.name = "ch_status0",
			.mode = S_IRUGO,

		},
		.show = xdma_channel_status_show0,
		.store = NULL,
	},
	{
		.attr = {
			.name = "ch_status1",
			.mode = S_IRUGO,

		},
		.show = xdma_channel_status_show1,
		.store = NULL,
	},
	{
		.attr = {
			.name = "ch_status2",
			.mode = S_IRUGO,

		},
		.show = xdma_channel_status_show2,
		.store = NULL,
	},
	{
		.attr = {
			.name = "ch_status3",
			.mode = S_IRUGO,

		},
		.show = xdma_channel_status_show3,
		.store = NULL,
	},
	{
		.attr = {
			.name = "ch_status4",
			.mode = S_IRUGO,

		},
		.show = xdma_channel_status_show4,
		.store = NULL,
	},
	{
		.attr = {
			.name = "ch_status5",
			.mode = S_IRUGO,

		},
		.show = xdma_channel_status_show5,
		.store = NULL,
	},
	{
		.attr = {
			.name = "ch_status6",
			.mode = S_IRUGO,

		},
		.show = xdma_channel_status_show6,
		.store = NULL,
	},
	{
		.attr = {
			.name = "ch_status7",
			.mode = S_IRUGO,

		},
		.show = xdma_channel_status_show7,
		.store = NULL,
	},
};

static int xdma_video_attr_create(struct xdma_video_dev *dev)
{
	unsigned int ch;
	int rv = 0;

	for (ch = 0; ch < max_channels(dev); ch++)
	{
		rv = device_create_file(&dev->pci_dev->dev, &dev_attr_channel[ch]);
		if (rv)
		{
			pr_err("Failed to create attr device%d file\n", ch);
		}
	}

	return 0;
}

static int xdma_video_attr_remove(struct xdma_video_dev *dev)
{
	unsigned int ch;

	for (ch = 0; ch < max_channels(dev); ch++)
	{
		device_remove_file(&dev->pci_dev->dev, &dev_attr_channel[ch]);
	}

	return 0;
}
/// End Add by Liyunfeng 220111

static void xdma_video_set_framerate(struct xdma_video_channel *vc,
									 unsigned int fps)
{
	vc->fps = fps;
}

static const struct xdma_video_format *format_by_fourcc(unsigned int fourcc)
{
	unsigned int cnt;

	for (cnt = 0; cnt < ARRAY_SIZE(formats); cnt++)
		if (formats[cnt].fourcc == fourcc)
			return &formats[cnt];
	return NULL;
}

static void xdma_video_clear_queue(struct xdma_video_channel *vc,
								   enum vb2_buffer_state state)
{
	dbg_video("video%d: \n", vc->num);
	while (!list_empty(&vc->vidq_queued))
	{
		struct xdma_video_v4l2_buf *buf;

		buf = list_first_entry(&vc->vidq_queued,
							   struct xdma_video_v4l2_buf, list);
		list_del(&buf->list);
		vb2_buffer_done(&buf->vb.vb2_buf, state);
	}
}

void xdma_video_get_timestamp(struct xdma_video_channel *vc, struct xdma_video_dev *dev)
{
	u64 timestamp_s, timestamp_ns;
	void *user_intc_reg;
	struct xdma_dev *xdev = dev->xdev;

	user_intc_reg = (xdev->bar[xdev->user_bar_idx] + REGS_USER_OFS_INT_V2);
	timestamp_s = read_register(user_intc_reg + 0x170+ 0x8 * vc->ch); 
	timestamp_ns = read_register(user_intc_reg + 0x174 + 0x8 * vc->ch);
	vc->timestamp_xdma_xfr_userirq = (timestamp_s*1000000000 + timestamp_ns);
}

void xdma_video_buf_done(struct xdma_video_channel *vc,
						 struct xdma_video_v4l2_buf *buf)
{
	struct xdma_video_dev *dev = vc->dev;
	struct xdma_dev *xdev = dev->xdev;
	struct vb2_v4l2_buffer *vb = &buf->vb;
	struct vb2_buffer *vb2_buf;
	u32 *reg_trigger;
	u32 trig_mode;
	// struct sg_table *sgt = vb2_dma_sg_plane_desc(&buf->vb.vb2_buf, 0);

	reg_trigger = (u32 *)(xdev->bar[xdev->user_bar_idx] +
						  REGS_USER_OFS_TRIGER + 0x7C);

	trig_mode = 0; // ioread32(reg_trigger);
	// dbg_video("trig_mode = 0x%x\n", trig_mode);
	trig_mode = (trig_mode >> (vc->ch * 4)) & 0xf;

	// dbg_video("video%d: sgt(0x%p)\n", vc->num, sgt);
	vb->field = dev->field;
	// vb->sequence = vc->sequence++;
	vb->sequence = vc->sequence;
	vb2_buf = &vb->vb2_buf;
#if 0
	if (trig_mode == 0)
	{
		vb2_buf->timestamp = ktime_get_real_ns();
	}
	else if (trig_mode == 1)
	{
		vb2_buf->timestamp = vc->trig_timestamp1;
	}
	else if (trig_mode == 2)
	{
		vb2_buf->timestamp = vc->trig_timestamp2;
	}
#else
#endif
	vb2_buf->timestamp = vc->timestamp_xdma_xfr_userirq;	
	//pr_inf("video%d: vb2_buf->timestamp = %llu\n", vc->num, vb2_buf->timestamp/1000);
	vb2_buffer_done(vb2_buf, VB2_BUF_STATE_DONE);
}

// static void xdma_request_submit(struct xdma_video_dev *dev, struct user_frame_request *req);

static irqreturn_t frame_trigger_isr(int irq, void *dev_id)
{
	struct xdma_video_channel *vc = (struct xdma_video_channel *)dev_id;

	if (vc->kthread.trigger_started)
	{
		vc->trig_count++;
		vc->trig_timestamp2 = vc->trig_timestamp1;
		vc->trig_timestamp1 = ktime_get_real_ns();
	}

	return IRQ_HANDLED;
}

static int g_count = 0;
static irqreturn_t frame_sync_isr(int irq, void *dev_id)
{
	struct xdma_video_channel *vc = (struct xdma_video_channel *)dev_id;
	unsigned int started;
	unsigned int running;
	unsigned int dummy_frame;
	unsigned long flags;
	//u32 time_stamp_l;
	u64 timestamp;

	// dbg_video("channel = 0x%08x in \n", vc->ch);

	spin_lock_irqsave(&vc->kthread.lock, flags);
	started = vc->kthread.streaming_started;
	running = vc->kthread.streaming_running;
	dummy_frame = vc->kthread.dummy_frame;
	spin_unlock_irqrestore(&vc->kthread.lock, flags);
	timestamp =  ktime_get_real_ns();
	xdma_video_get_timestamp(vc, vc->dev);
	g_count++;
	dbg_video("video%d: count:%d frame:%09d fpga_timestamp = %llu.%09llu(s)\n", vc->num,g_count,vc->sequence, vc->timestamp_xdma_xfr_userirq/1000000000,vc->timestamp_xdma_xfr_userirq%1000000000);
	dbg_video("video%d: count:%d frame:%09d recv_timestamp = %llu.%09llu(s)\n",vc->num,g_count,vc->sequence,  timestamp/1000000000,timestamp%1000000000);
	//dbg_video("video%d: frame:%09d (rece - fpga )        = %llu.%09llu(s)\n",
	//vc->num,vc->sequence,  (timestamp-vc->timestamp_xdma_xfr_userirq)/1000000000, (timestamp-vc->timestamp_xdma_xfr_userirq)%1000000000);
	// time_stamp_l = PCI_DMA_L(vc->timestamp_xdma_xfr_userirq);
	// dbg_video("started:%d running:%d vc->events_irq:%d\n", started, running, vc->events_irq);

	if (started && (dummy_frame < 6))
	{
		if (dummy_frame > 3)
		{
			vc->kthread.streaming_running = 1;
		}
		else if (dummy_frame > 1)
		{
			vc->kthread.trigger_started = 1;
		}
		vc->kthread.dummy_frame++;
		vc->kthread.streaming_running = 1;
	}
	running = vc->kthread.streaming_running;
	if (started && running)
	{
		spin_lock_irqsave(&(vc->events_lock), flags);
		if (!vc->events_irq)
		{
			vc->sequence++;
			vc->events_irq = 1;
			wake_up_interruptible(&(vc->events_wq));
		}
		else
		{
			vc->frame_sync_loss++;
		}
		spin_unlock_irqrestore(&(vc->events_lock), flags);
	}

	vc->irq_count++;

	if (0 == vc->irq_count % 30)
	{
		// pr_info("video channel:%d irq_count:%d sequence:%d\n",vc->ch, vc->irq_count, vc->sequence);
	}
#ifdef __TIME_STAMAP__
	dbg_timestamp("video:%d receive the (%d) user irq timestamp: %d:%d:%d:%d(s:ms:us:ns)\n",
				  vc->ch, vc->irq_count, time_stamp_l / 1000000000, time_stamp_l % 1000000000 / 1000000, time_stamp_l % 1000000 / 1000, time_stamp_l % 1000);
#endif
	return IRQ_HANDLED;
}

///////Added by Liyunfeng 210729
// static struct transfer_monitor * engine_monitor;
static void xdma_request_queue(struct transfer_monitor *monitor, struct user_frame_request *req)
{
	unsigned long flags;

	spin_lock_irqsave(&monitor->lock, flags);
	list_add_tail(&req->entry, &monitor->transfer_list);
	monitor->num_element++;
	if (monitor->num_element < 4)
		wake_up_interruptible(&(monitor->wq));
	spin_unlock_irqrestore(&monitor->lock, flags);

	wait_event_interruptible(req->finish_wq, req->transfer_finished != 0);

	spin_lock_irqsave(&req->lock, flags);
	req->transfer_finished = 0;
	spin_unlock_irqrestore(&req->lock, flags);
}

static void xdma_request_submit(struct xdma_video_dev *dev, struct user_frame_request *req)
{
	int i;
	unsigned long flags;
	unsigned int num_element[4];
	struct transfer_monitor *monitor;
	struct xdma_dev *xdev = dev->xdev;

	for (i = 0; i < xdev->c2h_channel_max; i++)
	{
		monitor = &dev->engine_monitor[i];
		spin_lock_irqsave(&monitor->lock, flags);
		num_element[i] = monitor->num_element;
		spin_unlock_irqrestore(&monitor->lock, flags);
	}

	// if(num_element[0] <= num_element[1])
	monitor = &dev->engine_monitor[0];
	// else
	//	monitor = &dev->engine_monitor[1];

	xdma_request_queue(monitor, req);
}

static void xdma_request_done(struct user_frame_request *req)
{
	unsigned long flags;

	spin_lock_irqsave(&req->lock, flags);
	req->transfer_finished = 1;
	wake_up_interruptible(&(req->finish_wq));
	spin_unlock_irqrestore(&req->lock, flags);
}

static int xthread_transfer_process(void *data)
{
	struct transfer_monitor *monitor = (struct transfer_monitor *)data;
	struct xdma_engine *engine = monitor->engine;
	struct xdma_dev *xdev = engine->xdev;
	struct user_frame_request *req;
	unsigned long flags;
	ssize_t res = 0;
	int rv;

	disallow_signal(SIGPIPE);
	while (!kthread_should_stop())
	{
		spin_lock_irqsave(&monitor->lock, flags);
		if (!list_empty(&monitor->transfer_list))
		{
			dbg_video("list not empty\n");
			req = list_first_entry(&monitor->transfer_list,
								   struct user_frame_request, entry);
			spin_unlock_irqrestore(&monitor->lock, flags);

			//res = my_launch_transfer(engine, req);
			//res = xdma_xfer_submit(xdev,0);
			spin_lock_irqsave(&monitor->lock, flags);
			list_del(&req->entry);
			monitor->num_element--;
			spin_unlock_irqrestore(&monitor->lock, flags);

			xdma_request_done(req);
		}
		else
		{
			spin_unlock_irqrestore(&monitor->lock, flags);

			rv = wait_event_interruptible(monitor->wq,
										  monitor->num_element != 0);
			if (rv)
				dbg_video("wait_event_interruptible=%d\n", rv);
		}
	}

	return 0;
}

static void transfer_monitor_init(struct xdma_video_dev *dev)
{
	int i;
	struct xdma_dev *xdev = dev->xdev;
	struct transfer_monitor *monitor;

	dev->engine_monitor = kcalloc(xdev->c2h_channel_max, sizeof(*dev->engine_monitor), GFP_KERNEL);
	if (!dev->engine_monitor)
	{
		dbg_video("engine_monitor create failed!\n");
	}

	for (i = 0; i < xdev->c2h_channel_max; i++)
	{
		monitor = &dev->engine_monitor[i];

		monitor->engine = &xdev->engine_c2h[i];
		spin_lock_init(&monitor->lock);
		INIT_LIST_HEAD(&monitor->transfer_list);
		init_waitqueue_head(&monitor->wq);
		monitor->num_element = 0;
		monitor->task = kthread_run(xthread_transfer_process,
									(void *)monitor, "trans_xdma_%d_%d", xdev->idx, i);
		if (IS_ERR(monitor->task))
		{
			dbg_video("trans_xdma_%d_%d create failed!\n", xdev->idx, i);
			monitor->task = NULL;
		}
	}
}

static void transfer_monitor_free(struct xdma_video_dev *dev)
{
	int i;
	unsigned long flags;
	struct xdma_dev *xdev = dev->xdev;
	struct transfer_monitor *monitor;

	for (i = 0; i < xdev->c2h_channel_max; i++)
	{
		monitor = &dev->engine_monitor[i];
		spin_lock_irqsave(&monitor->lock, flags);
		monitor->num_element = 1;
		spin_unlock_irqrestore(&monitor->lock, flags);
		kthread_stop(monitor->task);
	}

	kfree(dev->engine_monitor);
}

////End Added by Liyunfeng 210729

static ssize_t grab_one_frame(struct xdma_video_channel *vc, struct xdma_video_v4l2_buf *buf)
{
	ssize_t res = 0;
	struct xdma_video_dev *dev = vc->dev;
	struct xdma_dev *xdev = dev->xdev;
	struct user_frame_request *req;
	// struct axi_video_regs *status_regs;
	void *user_intc_reg;
	u32 status;
	struct transfer_monitor *monitor;

	// status_regs = (struct axi_video_regs *)(xdev->bar[xdev->user_bar_idx] +
	//		     REGS_USER_OFS_VIDEO);

	user_intc_reg = (xdev->bar[xdev->user_bar_idx] + REGS_USER_OFS_INT_V2);

	status = read_register(user_intc_reg + 0x0c);

	dbg_video("status = 0x%08x\n", status);

	// pr_info("video:%d frame buf:%x is ready\n",vc->ch, status);
	status = ((status >> (vc->ch * 4)) & 0xf);

	dbg_video("status = 0x%08x\n", status);
	// pr_info("video:%d frame buf:%d is ready\n",vc->ch, status);

	if (0 == status)
	{
		req = buf->req[0];
	}
	else if (1 == status)
	{
		req = buf->req[1];
	}
	else if (2 == status)
	{
		req = buf->req[2];
	}
	else
	{
		req = buf->req[0];
	}

	//monitor = &dev->engine_monitor[vc->ch / 2];
	//xdma_request_queue(monitor, req);
	// xdma_request_submit(dev, req);
	xdma_xfer_submit(xdev,vc->ch/2,0, req->ep_addr, req->sgt,true,100);
	//  grab one frame done
	iowrite32(1 << vc->ch, user_intc_reg + 0xb4);
	return res;
}

static int xthread_video(void *data)
{
	struct xdma_video_channel *vc = (struct xdma_video_channel *)data;
	struct xdma_video_v4l2_buf *buf;
	unsigned int running;
	unsigned long flags;
	ssize_t res = 0;
	int rv;
	u32 time_stamp_l;

	dbg_video("xthread_video create\n");

	disallow_signal(SIGPIPE);
	while (!kthread_should_stop())
	{
		dbg_video("wait_event_interruptible \n");
		rv = wait_event_interruptible(vc->events_wq,
									  vc->events_irq != 0);

		if (rv)
			dbg_sg("wait_event_interruptible=%d\n", rv);
		dbg_video("wait interrupt result= %d\n", rv);

		spin_lock_irqsave(&vc->kthread.lock, flags);
		running = vc->kthread.streaming_running;
		spin_unlock_irqrestore(&vc->kthread.lock, flags);

		if (!list_empty(&vc->vidq_queued) && running)
		{
			buf = list_first_entry(&vc->vidq_queued,
								   struct xdma_video_v4l2_buf, list);
			buf->vb.vb2_buf.state = VB2_BUF_STATE_ACTIVE;
#ifdef __TIME_STAMAP__
			vc->timestamp_xdma_xfr_grab_one = ktime_get_real_ns();

			time_stamp_l = PCI_DMA_L(vc->timestamp_xdma_xfr_grab_one);
			dbg_timestamp("video:%d execute grab the(%d) frame dma start time_stamp:%d:%d:%d:%d(s:ms:us:ns)\n", vc->ch, vc->frame_count,
						  time_stamp_l / 1000000000, time_stamp_l % 1000000000 / 1000000, time_stamp_l % 1000000 / 1000, time_stamp_l % 1000);
#endif
			res = grab_one_frame(vc, buf);

			spin_lock_irqsave(&vc->qlock, flags);
			list_del(&buf->list);
			spin_unlock_irqrestore(&vc->qlock, flags);

			xdma_video_buf_done(vc, buf);
#ifdef __TIME_STAMAP__
			vc->timestamp_xdma_xfr_grab_done = ktime_get_real_ns();

			time_stamp_l = PCI_DMA_L(vc->timestamp_xdma_xfr_grab_done);
			dbg_timestamp("video:%d execute grab the(%d) frame dma done time_stamp:%d:%d:%d:%d(s:ms:us:ns)\n", vc->ch, vc->frame_count,
						  time_stamp_l / 1000000000, time_stamp_l % 1000000000 / 1000000, time_stamp_l % 1000000 / 1000, time_stamp_l % 1000);
#endif
			vc->frame_count++;
			if (0 == vc->frame_count % 30)
			{
				// pr_info("video:%d get %d frame",vc->ch, vc->frame_count);
			}
		}
		else if (list_empty(&vc->vidq_queued) && running)
		{
			vc->frame_loss++;
			// if(0 == vc->frame_loss % 30)
			{
				// pr_info("video:%d lost %d frame",vc->ch, vc->frame_loss);
			}
		}

		spin_lock_irqsave(&vc->events_lock, flags);
		vc->events_irq = 0;
		spin_unlock_irqrestore(&vc->events_lock, flags);
	}

	return 0;
}

static int xdma_video_start_streaming(struct vb2_queue *vq, unsigned int count)
{
	struct xdma_video_channel *vc = vb2_get_drv_priv(vq);
	struct xdma_video_dev *dev = vc->dev;
	struct xdma_dev *xdev = dev->xdev;
	struct axi_intc_regs *intc_regs;
	u32 *reg_trigger;
	// u32 reg_mer;
	u32 mask;
	unsigned long flags;
	// u32 i;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
		dbg_video("video%d: vq->max_num_buffers  = %d\n", vc->num, vq->max_num_buffers );
	#else
		dbg_video("video%d: vq->num_buffers = %d\n", vc->num, vq->num_buffers);
	#endif
	
	dbg_video("video%d: count = %d\n", vc->num, count);

	intc_regs = (struct axi_intc_regs *)(xdev->bar[xdev->user_bar_idx] +
										 REGS_USER_OFS_INT);

	reg_trigger = (u32 *)(xdev->bar[xdev->user_bar_idx] +
						  REGS_USER_OFS_TRIGER + 0x7C);

	// dbg_video("video%d: \n", vc->num);
	spin_lock_irqsave(&vc->events_lock, flags);
	vc->events_irq = 0;
	spin_unlock_irqrestore(&vc->events_lock, flags);

	spin_lock_irqsave(&vc->kthread.lock, flags);
	vc->kthread.streaming_started = 1;
	vc->kthread.streaming_running = 0;
	vc->kthread.trigger_started = 0;
	vc->kthread.dummy_frame = 0;
	spin_unlock_irqrestore(&vc->kthread.lock, flags);
	vc->sequence = 0;
	// vc->irq_count = 0;
	vc->frame_sync_loss = 0;
	vc->frame_loss = 0;
	vc->trig_count = 0;
	vc->frame_count = 0;

	udelay(10);
	mask = 0x1 << vc->ch;
	// reg_mer = ioread32(&intc_regs->mer);
	// if(!(reg_mer & 0x1))
	//	iowrite32(0x3, &intc_regs->mer);
	// iowrite32(mask, &intc_regs->iar);
	// iowrite32(mask, &intc_regs->sie);

	// if((ioread32(reg_trigger)>>(vc->ch*4))&0xf) {
	// iowrite32(mask<<8, &intc_regs->iar);
	// iowrite32(mask<<8, &intc_regs->sie);
	//}

	return 0;
}

static void xdma_video_stop_streaming(struct vb2_queue *vq)
{
	struct xdma_video_channel *vc = vb2_get_drv_priv(vq);
	struct xdma_video_dev *dev = vc->dev;
	struct xdma_dev *xdev = dev->xdev;
	struct axi_intc_regs *intc_regs;
	u32 *reg_trigger;
	u32 mask;
	unsigned long flags;

	intc_regs = (struct axi_intc_regs *)(xdev->bar[xdev->user_bar_idx] +
										 REGS_USER_OFS_INT);
	reg_trigger = (u32 *)(xdev->bar[xdev->user_bar_idx] +
						  REGS_USER_OFS_TRIGER + 0x7C);

	// dbg_video("video%d: \n", vc->num);
	
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
		dbg_video("video%d: vq->max_num_buffers  = %d\n", vc->num, vq->max_num_buffers );
	#else
		dbg_video("video%d: vq->num_buffers = %d\n", vc->num, vq->num_buffers);
	#endif
	mask = 0x1 << vc->ch;
	// while((ioread32(&intc_regs->ipr) & mask) && (ioread32(&intc_regs->ier) & mask));
	// iowrite32(mask, &intc_regs->iar);
	// iowrite32(mask, &intc_regs->cie);

	// if((ioread32(reg_trigger)>>(vc->ch*4))&0xf) {
	//	while((ioread32(&intc_regs->ipr) & (mask<<8)) && (ioread32(&intc_regs->ier) & (mask<<8)));
	//	iowrite32((mask<<8), &intc_regs->iar);
	//	iowrite32((mask<<8), &intc_regs->cie);
	// }

	msleep(100);
	while (list_empty(&vc->events_wq.head))
		;

	spin_lock_irqsave(&vc->kthread.lock, flags);
	vc->kthread.streaming_started = 0;
	vc->kthread.streaming_running = 0;
	vc->kthread.trigger_started = 0;
	vc->kthread.dummy_frame = 0;
	spin_unlock_irqrestore(&vc->kthread.lock, flags);

	spin_lock_irqsave(&vc->qlock, flags);
	xdma_video_clear_queue(vc, VB2_BUF_STATE_ERROR);
	spin_unlock_irqrestore(&vc->qlock, flags);
}

static int xdma_video_queue_setup(struct vb2_queue *vq,
								  unsigned int *nbuffers, unsigned int *nplanes,
								  unsigned int sizes[], struct device *alloc_devs[])
{
	struct xdma_video_channel *vc = vb2_get_drv_priv(vq);
	unsigned int szimage =
		(vc->width * vc->height * vc->format->depth) >> 3;

	dbg_video("video%d: nbuffers(%d),nplanes(%d),sizes[%d,%d,%d]\n", vc->num, *nbuffers, *nplanes, sizes[0], sizes[1], sizes[2]);

	#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
		dbg_video("video%d: vq->max_num_buffers = %d\n", vc->num, vq->max_num_buffers);
		if (vq->max_num_buffers + *nbuffers < 3)
			*nbuffers = 3 - vq->max_num_buffers;
	#else
		dbg_video("video%d: vq->num_buffers = %d\n", vc->num, vq->num_buffers);
		if (vq->num_buffers + *nbuffers < 3)
			*nbuffers = 3 - vq->num_buffers;
	#endif
	if (*nplanes)
	{
		if (*nplanes != 1 || sizes[0] < szimage)
			return -EINVAL;
		return 0;
	}

	sizes[0] = szimage;
	*nplanes = 1;

	dbg_video("video%d: nbuffers(%d),nplanes(%d),sizes[%d,%d,%d]\n", vc->num, *nbuffers, *nplanes, sizes[0], sizes[1], sizes[2]);

	return 0;
}

static int xdma_video_buf_prepare(struct vb2_buffer *vb)
{
	struct xdma_video_channel *vc = vb2_get_drv_priv(vb->vb2_queue);
	struct sg_table *sgt = vb2_dma_sg_plane_desc(vb, 0);
	unsigned int size =
		(vc->width * vc->height * vc->format->depth) >> 3;

	dbg_video("video%d: size(%d),vb2_plane_size(%lu) sgt(0x%p)\n", vc->num, size, vb2_plane_size(vb, 0), sgt);

	if (vb2_plane_size(vb, 0) < size)
		return -EINVAL;
	vb2_set_plane_payload(vb, 0, size);
	return 0;
}

static void xdma_video_buf_queue(struct vb2_buffer *vb)
{
	struct xdma_video_channel *vc = vb2_get_drv_priv(vb->vb2_queue);
	// struct sg_table *sgt = vb2_dma_sg_plane_desc(vb, 0);
	unsigned long flags;
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);
	struct xdma_video_v4l2_buf *buf =
		container_of(vbuf, struct xdma_video_v4l2_buf, vb);

	// dbg_video("video%d: sgt(0x%p)\n", vc->num, sgt);
	// dbg_video("video%d: vb2_buffer(0x%p)\n", vc->num, vb);
	spin_lock_irqsave(&vc->qlock, flags);
	list_add_tail(&buf->list, &vc->vidq_queued);
	spin_unlock_irqrestore(&vc->qlock, flags);
}

static struct user_frame_request *xdma_video_init_request(struct sg_table *sgt,
						 u64 ep_addr)
{
	struct user_frame_request  *pReq;
	pReq = kzalloc(sizeof(struct user_frame_request), GFP_KERNEL);
	pReq->sgt = sgt;
	pReq->ep_addr = ep_addr;
	spin_lock_init(&pReq->lock);
	INIT_LIST_HEAD(&pReq->entry);
	init_waitqueue_head(&pReq->finish_wq);
	pReq->transfer_finished = 0;
	return pReq;
}

static void xdma_video_request_free(struct user_frame_request *req)
{
	kfree(req);
	return;
}

static int xdma_video_buf_init(struct vb2_buffer *vb)
{
	struct xdma_video_channel *vc = vb2_get_drv_priv(vb->vb2_queue);
	struct sg_table *sgt = vb2_dma_sg_plane_desc(vb, 0);
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);
	struct xdma_video_v4l2_buf *buf =
		container_of(vbuf, struct xdma_video_v4l2_buf, vb);

	dbg_video("video%d: \n", vc->num);
#if 1
	if (vc->ch < 4)
	{
		buf->req[0] = xdma_video_init_request(sgt, 0x810000000 + 0x6000000 * vc->ch);
		buf->req[1] = xdma_video_init_request(sgt, 0x812000000 + 0x6000000 * vc->ch);
		buf->req[2] = xdma_video_init_request(sgt, 0x814000000 + 0x6000000 * vc->ch);
	}
	else
	{
		buf->req[0] = xdma_video_init_request(sgt, 0x410000000 + 0x6000000 * (vc->ch));
		buf->req[1] = xdma_video_init_request(sgt, 0x412000000 + 0x6000000 * (vc->ch));
		buf->req[2] = xdma_video_init_request(sgt, 0x414000000 + 0x6000000 * (vc->ch));
	}

#else
	if (vc->ch == 4)
	{
		buf->req[0] = xdma_init_request(sgt, 0x410000000 + 0x6000000 * vc->ch);
		buf->req[1] = xdma_init_request(sgt, 0x412000000 + 0x6000000 * vc->ch);
		buf->req[2] = xdma_init_request(sgt, 0x414000000 + 0x6000000 * vc->ch);
	}
	else
	{
		buf->req[0] = xdma_init_request(sgt, 0x810000000 + 0x6000000 * (vc->ch));
		buf->req[1] = xdma_init_request(sgt, 0x812000000 + 0x6000000 * (vc->ch));
		buf->req[2] = xdma_init_request(sgt, 0x814000000 + 0x6000000 * (vc->ch));
	}

#endif

	return 0;
}

static void xdma_video_buf_finish(struct vb2_buffer *vb)
{
	// struct xdma_video_channel *vc = vb2_get_drv_priv(vb->vb2_queue);
	// struct sg_table *sgt = vb2_dma_sg_plane_desc(vb, 0);
	// dbg_video("video%d: \n", vc->num);
}

static void xdma_video_buf_cleanup(struct vb2_buffer *vb)
{
	struct xdma_video_channel *vc = vb2_get_drv_priv(vb->vb2_queue);
	// struct sg_table *sgt = vb2_dma_sg_plane_desc(vb, 0);
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);
	struct xdma_video_v4l2_buf *buf =
		container_of(vbuf, struct xdma_video_v4l2_buf, vb);

	dbg_video("video%d: \n", vc->num);
	xdma_video_request_free(buf->req[0]);
	xdma_video_request_free(buf->req[1]);
	xdma_video_request_free(buf->req[2]);
}

static const struct vb2_ops xdma_video_qops = {
	.queue_setup = xdma_video_queue_setup,
	.buf_queue = xdma_video_buf_queue,
	.buf_prepare = xdma_video_buf_prepare,
	.start_streaming = xdma_video_start_streaming,
	.stop_streaming = xdma_video_stop_streaming,
	.wait_prepare = vb2_ops_wait_prepare,
	.wait_finish = vb2_ops_wait_finish,
	.buf_init = xdma_video_buf_init,
	.buf_finish = xdma_video_buf_finish,
	.buf_cleanup = xdma_video_buf_cleanup,
};

/*static int xdma_video_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct xdma_video_channel *vc;
	unsigned int ch;

	vc = container_of(ctrl->handler, struct xdma_video_channel,
			ctrl_handler);
	ch = vc->ch;

	dbg_video("ch=%d, id =0x%x!\n", ch, ctrl->id);
	switch (ctrl->id) {
		case V4L2_CID_BRIGHTNESS:
			return 0;

		case V4L2_CID_CONTRAST:
			return 0;

		case V4L2_CID_SATURATION:
			return 0;

		case V4L2_CID_HUE:
			return 0;
	}

	return -EINVAL;
}

static const struct v4l2_ctrl_ops ctrl_ops = {
	.s_ctrl = xdma_video_s_ctrl,
};*/

static int xdma_video_g_fmt_vid_cap(struct file *file, void *priv,
									struct v4l2_format *f)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	struct xdma_video_dev *dev = vc->dev;
#if 1

	struct axi_video_fmt *axi_video_fmt_regs;
	u32 video_width;
	u32 video_heigh;
	u32 video_fps;
	u32 video_format;
	void *user_intc_reg;
	struct xdma_dev *xdev = dev->xdev;
	axi_video_fmt_regs = (struct axi_video_fmt *)(xdev->bar[xdev->user_bar_idx] +
												  REGS_USER_OFS_VIDEO_FMT);

	user_intc_reg = (xdev->bar[xdev->user_bar_idx] + REGS_USER_OFS_INT_V2);
	video_width = read_register(user_intc_reg + 0x100 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->width);
	video_heigh = read_register(user_intc_reg + 0x104 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->height);
	video_fps = read_register(user_intc_reg + 0x24 + 0x04 * vc->ch);   // ioread32(&(axi_video_fmt_regs + vc->ch)->fps);
	//video_format = read_register(user_intc_reg + 0x08);				   // ioread32(&(axi_video_fmt_regs + vc->ch)->format);

	video_fps = ((0 == video_fps) ? 30 : (300000000 / video_fps));
	//video_format = (video_format >> (vc->ch * 4)) & 0x3;

#endif

	dbg_video("video%d: video_width%d video_heigh:%d video_fps:%d video_format:%d\n", vc->num, video_width, video_heigh, video_fps, video_format);
	f->fmt.pix.width = video_width;
	f->fmt.pix.height = video_heigh;
	f->fmt.pix.field = dev->field;
	f->fmt.pix.pixelformat = vc->format->fourcc;//formats[video_format].fourcc;
	f->fmt.pix.colorspace = V4L2_COLORSPACE_SMPTE170M;
	f->fmt.pix.bytesperline = (f->fmt.pix.width * vc->format->depth) / 8;
	f->fmt.pix.sizeimage = f->fmt.pix.height * f->fmt.pix.bytesperline;

	// f->fmt.pix.width = vc->width;
	// f->fmt.pix.height = vc->height;
	// f->fmt.pix.field = dev->field;
	// f->fmt.pix.pixelformat = vc->format->fourcc;
	// f->fmt.pix.colorspace = V4L2_COLORSPACE_SMPTE170M;
	// f->fmt.pix.bytesperline = (f->fmt.pix.width * vc->format->depth) / 8;
	// f->fmt.pix.sizeimage = f->fmt.pix.height * f->fmt.pix.bytesperline;
	return 0;
}

static int xdma_video_try_fmt_vid_cap(struct file *file, void *priv,
									  struct v4l2_format *f)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	struct xdma_video_dev *dev = vc->dev;
#if 1
	const struct xdma_video_format *format;

	struct axi_video_fmt *axi_video_fmt_regs;
	u32 video_width;
	u32 video_heigh;
	u32 video_fps;
	u32 video_format;
	void *user_intc_reg;
	struct xdma_dev *xdev = dev->xdev;
	axi_video_fmt_regs = (struct axi_video_fmt *)(xdev->bar[xdev->user_bar_idx] +
												  REGS_USER_OFS_VIDEO_FMT);

	user_intc_reg = (xdev->bar[xdev->user_bar_idx] + REGS_USER_OFS_INT_V2);
	video_width = read_register(user_intc_reg + 0x100 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->width);
	video_heigh = read_register(user_intc_reg + 0x104 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->height);
	video_fps = read_register(user_intc_reg + 0x24 + 0x04 * vc->ch);   // ioread32(&(axi_video_fmt_regs + vc->ch)->fps);
	//video_format = read_register(user_intc_reg + 0x08);				   // ioread32(&(axi_video_fmt_regs + vc->ch)->format);

	video_fps = ((0 == video_fps) ? 30 : (300000000 / video_fps));
	//video_format = (video_format >> (vc->ch * 4)) & 0x3;

	dbg_video("video%d: video_width%d video_heigh:%d video_fps:%d video_format:%d\n", vc->num, video_width, video_heigh, video_fps, video_format);
	format = format_by_fourcc(f->fmt.pix.pixelformat);
	if (!format)
	{
		format = &formats[vc->format->fourcc];
		f->fmt.pix.pixelformat = vc->format->fourcc;//format->fourcc;
	}
#endif

	f->fmt.pix.width = video_width;	 //;
	f->fmt.pix.height = video_heigh; // video_heigh;
	f->fmt.pix.pixelformat = format->fourcc;
	f->fmt.pix.bytesperline = (f->fmt.pix.width * format->depth) / 8;
	f->fmt.pix.sizeimage = f->fmt.pix.height * f->fmt.pix.bytesperline;
	f->fmt.pix.colorspace = V4L2_COLORSPACE_SMPTE170M;
	f->fmt.pix.field = dev->field;
	return 0;
}

static int xdma_video_set_format(struct xdma_video_channel *vc,
								 unsigned int pixelformat, unsigned int width,
								 unsigned int height)
{
	vc->format = format_by_fourcc(pixelformat);
	if(vc->format == NULL){
        return -EINVAL;
    }
	vc->width = width;
	vc->height = height;
	return 0;
}

static int xdma_video_s_fmt_vid_cap(struct file *file, void *priv,
									struct v4l2_format *f)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	int err;

	dbg_video("video%d: \n", vc->num);
	if (vb2_is_busy(&vc->vidq))
		return -EBUSY;

	//err = xdma_video_try_fmt_vid_cap(file, priv, f);
	//if (err)
	//	return err;

	err = xdma_video_set_format(vc, f->fmt.pix.pixelformat,
						  f->fmt.pix.width, f->fmt.pix.height);
	return err;
}

static int xdma_video_querycap(struct file *file, void *priv,
							   struct v4l2_capability *cap)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	struct xdma_video_dev *dev = vc->dev;

	dbg_video("video%d: \n", vc->num);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
	strscpy(cap->driver, "xdma_video", sizeof(cap->driver));
	strscpy(cap->card, dev->name, sizeof(cap->card));
#else
	strlcpy(cap->driver, "xdma_video", sizeof(cap->driver));
	strlcpy(cap->card, dev->name, sizeof(cap->card));
#endif

	
	snprintf(cap->bus_info, sizeof(cap->bus_info),
			 "PCI:%s", pci_name(dev->pci_dev));
	// cap->device_caps = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING;
	// cap->capabilities = cap->device_caps | V4L2_CAP_DEVICE_CAPS;

	return 0;
}

static int xdma_video_enum_framesizes(struct file *file, void *priv,
									  struct v4l2_frmsizeenum *fsize)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	struct xdma_video_dev *dev = vc->dev;
#if 1
	struct axi_video_fmt *axi_video_fmt_regs;
	u32 video_width;
	u32 video_heigh;
	u32 video_fps;
	u32 video_format;
	void *user_intc_reg;
	struct xdma_dev *xdev = dev->xdev;
	axi_video_fmt_regs = (struct axi_video_fmt *)(xdev->bar[xdev->user_bar_idx] +
												  REGS_USER_OFS_VIDEO_FMT);

	user_intc_reg = (xdev->bar[xdev->user_bar_idx] + REGS_USER_OFS_INT_V2);
	video_width = read_register(user_intc_reg + 0x100 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->width);
	video_heigh = read_register(user_intc_reg + 0x104 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->height);
	video_fps = read_register(user_intc_reg + 0x24 + 0x04 * vc->ch);   // ioread32(&(axi_video_fmt_regs + vc->ch)->fps);
	video_format = read_register(user_intc_reg + 0x08);				   // ioread32(&(axi_video_fmt_regs + vc->ch)->format);

	video_fps = ((0 == video_fps) ? 30 : (300000000 / video_fps));
	video_format = (video_format >> (vc->ch * 4)) & 0x3;
#endif

	dbg_video("video%d: video_width%d video_heigh:%d video_fps:%d video_format:%d\n", vc->num, video_width, video_heigh, video_fps, video_format);
	if (fsize->index)
		return -EINVAL;

	fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	fsize->discrete.width = video_width; // video_width;
	fsize->discrete.height = video_heigh;

	return 0;
}

static int xdma_video_enum_frameintervals(struct file *file, void *priv,
										  struct v4l2_frmivalenum *ival)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	struct xdma_video_dev *dev = vc->dev;
#if 1
	struct axi_video_fmt *axi_video_fmt_regs;
	u32 video_width;
	u32 video_heigh;
	u32 video_fps;
	u32 video_format;
	void *user_intc_reg;
	struct xdma_dev *xdev = dev->xdev;
	axi_video_fmt_regs = (struct axi_video_fmt *)(xdev->bar[xdev->user_bar_idx] +
												  REGS_USER_OFS_VIDEO_FMT);

	user_intc_reg = (xdev->bar[xdev->user_bar_idx] + REGS_USER_OFS_INT_V2);
	video_width = read_register(user_intc_reg + 0x100 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->width);
	video_heigh = read_register(user_intc_reg + 0x104 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->height);
	video_fps = read_register(user_intc_reg + 0x24 + 0x04 * vc->ch);   // ioread32(&(axi_video_fmt_regs + vc->ch)->fps);
	video_format = read_register(user_intc_reg + 0x08);				   // ioread32(&(axi_video_fmt_regs + vc->ch)->format);

	video_fps = ((0 == video_fps) ? 30 : (300000000 / video_fps));
	video_format = (video_format >> (vc->ch * 4)) & 0x3;
#endif

	dbg_video("video%d: video_width%d video_heigh:%d video_fps:%d video_format:%d\n", vc->num, video_width, video_heigh, video_fps, video_format);
	if (ival->index >= 1)
		return -EINVAL;

	ival->type = V4L2_FRMIVAL_TYPE_DISCRETE;
	ival->discrete.numerator = 1;
	ival->discrete.denominator = video_fps;
	// ival->discrete.denominator = vc->fps;

	return 0;
}

static int xdma_video_enum_fmt_vid_cap(struct file *file, void *priv,
									   struct v4l2_fmtdesc *f)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	struct xdma_video_dev *dev = vc->dev;
#if 1
	struct axi_video_fmt *axi_video_fmt_regs;
	u32 video_width;
	u32 video_heigh;
	u32 video_fps;
	u32 video_format;
	void *user_intc_reg;
	struct xdma_dev *xdev = dev->xdev;
	axi_video_fmt_regs = (struct axi_video_fmt *)(xdev->bar[xdev->user_bar_idx] +
												  REGS_USER_OFS_VIDEO_FMT);

	user_intc_reg = (xdev->bar[xdev->user_bar_idx] + REGS_USER_OFS_INT_V2);
	video_width = read_register(user_intc_reg + 0x100 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->width);
	video_heigh = read_register(user_intc_reg + 0x104 + 0x8 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->height);
	video_fps = read_register(user_intc_reg + 0x24 + 0x04 * vc->ch);   // ioread32(&(axi_video_fmt_regs + vc->ch)->fps);
	video_format = read_register(user_intc_reg + 0x08);				   // ioread32(&(axi_video_fmt_regs + vc->ch)->format);

	video_fps = ((0 == video_fps) ? 30 : (300000000 / video_fps));
	video_format = (video_format >> (vc->ch * 4)) & 0x3;
#endif

	dbg_video("video%d: video_width%d video_heigh:%d video_fps:%d video_format:%d\n", vc->num, video_width, video_heigh, video_fps, video_format);
	// if (f->index >= ARRAY_SIZE(formats))
	if (f->index >= 1)
		return -EINVAL;
	f->pixelformat = vc->format->fourcc; // formats[video_format].fourcc;;

	return 0;
}

/*static int xdma_video_set_standard(struct xdma_video_channel *vc, v4l2_std_id id)
{
	dbg_video("video%d: \n", vc->num);
	vc->video_standard = id;

	return 0;
}

static int xdma_video_s_std(struct file *file, void *priv, v4l2_std_id id)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	struct v4l2_format f;
	int ret;

	dbg_video("video%d: \n", vc->num);

	if (vc->video_standard == id)
		return 0;

	if (vb2_is_busy(&vc->vidq))
		return -EBUSY;

	ret = xdma_video_set_standard(vc, id);
	if (ret)
		return ret;

	xdma_video_g_fmt_vid_cap(file, priv, &f);
	xdma_video_s_fmt_vid_cap(file, priv, &f);

	xdma_video_set_framerate(vc, para_fps[vc->ch]);
	//xdma_video_set_framerate(vc, vc->fps);
	return 0;
}

static int xdma_video_querystd(struct file *file, void *priv, v4l2_std_id *std)
{
	struct xdma_video_channel *vc = video_drvdata(file);

	dbg_video("video%d: \n", vc->num);
	if (vb2_is_streaming(&vc->vidq))
		return -EBUSY;

	*std = V4L2_STD_UNKNOWN;
	return 0;
}

static int xdma_video_g_std(struct file *file, void *priv, v4l2_std_id *id)
{
	struct xdma_video_channel *vc = video_drvdata(file);

	dbg_video("video%d: \n", vc->num);

	*id = V4L2_STD_UNKNOWN;
	return 0;
}*/

static int xdma_video_g_parm(struct file *file, void *priv,
		struct v4l2_streamparm *sp)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	struct v4l2_captureparm *cp = &sp->parm.capture;

	dbg_video("video%d: \n", vc->num);
	if (sp->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;
	sp->parm.capture.readbuffers = 3;

	cp->capability = V4L2_CAP_TIMEPERFRAME;
	cp->timeperframe.numerator = 1;
	//cp->timeperframe.denominator = para_fps[vc->ch];
	cp->timeperframe.denominator = vc->fps;

	return 0;
}

static int xdma_video_s_parm(struct file *file, void *priv,
		struct v4l2_streamparm *sp)
{
	struct xdma_video_channel *vc = video_drvdata(file);
	struct v4l2_captureparm *cp = &sp->parm.capture;
	unsigned int denominator = cp->timeperframe.denominator;
	unsigned int numerator = cp->timeperframe.numerator;
	unsigned int fps;

	dbg_video("video%d: \n", vc->num);
	if (vb2_is_busy(&vc->vidq))
		return -EBUSY;

	fps = (!numerator || !denominator) ? 0 : denominator / numerator;
	if (vc->fps != fps)
		xdma_video_set_framerate(vc, fps);
	return xdma_video_g_parm(file, priv, sp);
}

static void xdma_video_set_input(struct xdma_video_channel *vc, unsigned int i)
{
	dbg_video("video%d: \n", vc->num);
	vc->input = i;
}

static int xdma_video_s_input(struct file *file, void *priv, unsigned int i)
{
	struct xdma_video_channel *vc = video_drvdata(file);

	dbg_video("video%d: \n", vc->num);
	if (i == vc->input)
		return 0;

	if (vb2_is_busy(&vc->vidq))
		return -EBUSY;

	xdma_video_set_input(vc, i);
	return 0;
}

static int xdma_video_g_input(struct file *file, void *priv, unsigned int *i)
{
	struct xdma_video_channel *vc = video_drvdata(file);

	dbg_video("video%d: \n", vc->num);
	*i = 0;
	return 0;
}

static int xdma_video_enum_input(struct file *file, void *priv,
		struct v4l2_input *i)
{
	struct xdma_video_channel *vc = video_drvdata(file);

	dbg_video("video%d: \n", vc->num);
	if (i->index >= vc->dev->channels) //XDMA_VIDEO_INPUTS_PER_CH
		return -EINVAL;

	snprintf(i->name, sizeof(i->name), "Composite%d", i->index);
	i->type = V4L2_INPUT_TYPE_CAMERA;
	i->std = V4L2_STD_UNKNOWN;
	//i->std = vc->device->tvnorms;
	//i->capabilities = V4L2_IN_CAP_STD;
	//i->status = 0;
	return 0;
}

static const struct v4l2_file_operations xdma_video_fops = {
	.owner = THIS_MODULE,
	.open = v4l2_fh_open,
	.unlocked_ioctl = video_ioctl2,
	.release = vb2_fop_release,
	.poll = vb2_fop_poll,
	.read = vb2_fop_read,
	.mmap = vb2_fop_mmap,
};

static const struct v4l2_ioctl_ops xdma_video_ioctl_ops = {
	.vidioc_querycap = xdma_video_querycap,

	.vidioc_g_fmt_vid_cap = xdma_video_g_fmt_vid_cap,
	.vidioc_s_fmt_vid_cap = xdma_video_s_fmt_vid_cap,
	.vidioc_enum_fmt_vid_cap = xdma_video_enum_fmt_vid_cap,
	.vidioc_try_fmt_vid_cap = xdma_video_try_fmt_vid_cap,

	//.vidioc_querystd		= xdma_video_querystd,
	//.vidioc_g_std			= xdma_video_g_std,
	//.vidioc_s_std			= xdma_video_s_std,

	.vidioc_g_parm		= xdma_video_g_parm,
	.vidioc_s_parm		= xdma_video_s_parm,

	.vidioc_enum_framesizes = xdma_video_enum_framesizes,
	.vidioc_enum_frameintervals = xdma_video_enum_frameintervals,

	.vidioc_enum_input		= xdma_video_enum_input,
	.vidioc_g_input		= xdma_video_g_input,
	.vidioc_s_input		= xdma_video_s_input,

	.vidioc_reqbufs = vb2_ioctl_reqbufs,
	.vidioc_querybuf = vb2_ioctl_querybuf,
	.vidioc_qbuf = vb2_ioctl_qbuf,
	.vidioc_dqbuf = vb2_ioctl_dqbuf,
	.vidioc_create_bufs = vb2_ioctl_create_bufs,
	.vidioc_streamon = vb2_ioctl_streamon,
	.vidioc_streamoff = vb2_ioctl_streamoff,
	.vidioc_prepare_buf = vb2_ioctl_prepare_buf,

	.vidioc_log_status = v4l2_ctrl_log_status,
	.vidioc_subscribe_event = v4l2_ctrl_subscribe_event,
	.vidioc_unsubscribe_event = v4l2_event_unsubscribe,
};

static void xdma_video_free(struct xdma_video_dev *dev)
{
	unsigned int ch;
	unsigned long flags;

	for (ch = 0; ch < max_channels(dev); ch++)
	{
		struct xdma_video_channel *vc = &dev->video_channels[ch];

		spin_lock_irqsave(&vc->events_lock, flags);
		vc->events_irq = 1;
		spin_unlock_irqrestore(&vc->events_lock, flags);
		kthread_stop(vc->kthread.task);

		video_unregister_device(vc->device);
	}
}

static int xdma_video_init(struct xdma_video_dev *dev)
{
	unsigned int ch;
	int err;

	struct axi_video_fmt *axi_video_fmt_regs;
	u32 video_width;
	u32 video_heigh;
	u32 video_fps;
	u32 video_format;
	void *user_intc_reg;
	struct xdma_dev *xdev = dev->xdev;
	axi_video_fmt_regs = (struct axi_video_fmt *)(xdev->bar[xdev->user_bar_idx] +
												  REGS_USER_OFS_VIDEO_FMT);
	user_intc_reg = (xdev->bar[xdev->user_bar_idx] + REGS_USER_OFS_INT_V2);

	err = v4l2_device_register(&dev->pci_dev->dev, &dev->v4l2_dev);
	if (err)
		return err;

	for (ch = 0; ch < max_channels(dev); ch++)
	{
		struct xdma_video_channel *vc = &dev->video_channels[ch];

		vc->dev = dev;
		vc->ch = ch;
	}

	for (ch = 0; ch < max_channels(dev); ch++)
	{
		struct xdma_video_channel *vc = &dev->video_channels[ch];
		struct video_device *vdev;

		mutex_init(&vc->vb_mutex);
		spin_lock_init(&vc->qlock);
		INIT_LIST_HEAD(&vc->vidq_queued);

		// err = xdma_video_set_standard(vc, V4L2_STD_NTSC);
		// err = xdma_video_set_standard(vc, V4L2_STD_UNKNOWN);
		// if (err)
		//	goto error;

		video_width = read_register(user_intc_reg + 0x100 + 0x8 * ch);	 // ioread32(&(axi_video_fmt_regs + vc->ch)->width);
		video_heigh = read_register(user_intc_reg + 0x104 + 0x8 * ch);	 // ioread32(&(axi_video_fmt_regs + vc->ch)->height);
		video_fps = read_register(user_intc_reg + 0x24 + 0x04 * vc->ch); // ioread32(&(axi_video_fmt_regs + vc->ch)->fps);
		video_format = read_register(user_intc_reg + 0x08);				 // ioread32(&(axi_video_fmt_regs + vc->ch)->format);

		video_fps = ((0 == video_fps) ? 30 : (300000000 / video_fps));
		video_format = (video_format >> (ch * 4)) & 0x3;

		err = xdma_video_set_format(vc, formats[video_format].fourcc,
									video_width, video_heigh);
		if (err)
			goto error;

		xdma_video_set_framerate(vc, video_fps);

		vc->vidq.io_modes = VB2_READ | VB2_MMAP | VB2_DMABUF;
		vc->vidq.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		vc->vidq.drv_priv = vc;
		vc->vidq.buf_struct_size = sizeof(struct xdma_video_v4l2_buf);
		vc->vidq.ops = &xdma_video_qops;
		vc->vidq.mem_ops = &vb2_dma_sg_memops;
		vc->vidq.timestamp_flags = V4L2_BUF_FLAG_TIMESTAMP_MONOTONIC;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
		vc->vidq.min_queued_buffers = 2;
#else
		vc->vidq.min_buffers_needed = 2;
#endif
		
		vc->vidq.lock = &vc->vb_mutex;
		vc->vidq.gfp_flags = GFP_DMA32;
		vc->vidq.dev = &dev->pci_dev->dev;

		err = vb2_queue_init(&vc->vidq);
		if (err)
		{
			v4l2_err(&dev->v4l2_dev,
					 "dma%d: cannot init vb2 queue\n", ch);
			goto error;
		}

		/*err = v4l2_ctrl_handler_init(&vc->ctrl_handler, 4);
		if (err) {
			v4l2_err(&dev->v4l2_dev,
					"dma%d: cannot init ctrl handler\n", ch);
			goto error;
		}
		v4l2_ctrl_new_std(&vc->ctrl_handler, &ctrl_ops,
				V4L2_CID_BRIGHTNESS, -128, 127, 1, 0);
		v4l2_ctrl_new_std(&vc->ctrl_handler, &ctrl_ops,
				V4L2_CID_CONTRAST, 0, 255, 1, 100);
		v4l2_ctrl_new_std(&vc->ctrl_handler, &ctrl_ops,
				V4L2_CID_SATURATION, 0, 255, 1, 128);
		v4l2_ctrl_new_std(&vc->ctrl_handler, &ctrl_ops,
				V4L2_CID_HUE, -128, 127, 1, 0);
		err = vc->ctrl_handler.error;
		if (err)
			goto error;

		err = v4l2_ctrl_handler_setup(&vc->ctrl_handler);
		if (err)
			goto error;*/

		vdev = video_device_alloc();
		if (!vdev)
		{
			v4l2_err(&dev->v4l2_dev,
					 "dma%d: unable to allocate device\n", ch);
			err = -ENOMEM;
			goto error;
		}

		snprintf(vdev->name, sizeof(vdev->name), "%s_video", dev->name);
		vdev->fops = &xdma_video_fops;
		vdev->ioctl_ops = &xdma_video_ioctl_ops;
		vdev->release = video_device_release;
		vdev->v4l2_dev = &dev->v4l2_dev;
		vdev->queue = &vc->vidq;
		// vdev->tvnorms = V4L2_STD_525_60 | V4L2_STD_625_50;
		// vdev->tvnorms = V4L2_STD_ALL;
		vdev->tvnorms = V4L2_STD_UNKNOWN;
		vdev->minor = -1;
		vdev->lock = &vc->vb_mutex;
		// vdev->ctrl_handler = &vc->ctrl_handler;
		vdev->device_caps = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING | V4L2_CAP_READWRITE;
		vc->device = vdev;
		video_set_drvdata(vdev, vc);

		#if PCI_VFL_TYPECHANGE
			err = video_register_device(vdev, VFL_TYPE_VIDEO, -1);
		#else
			err = video_register_device(vdev, VFL_TYPE_GRABBER, -1);
		#endif
		
		if (err < 0)
			goto error;

		vc->num = vdev->num;

		spin_lock_init(&vc->kthread.lock);
		init_waitqueue_head(&vc->kthread.stopped_wq);
		init_waitqueue_head(&vc->events_wq);
		vc->kthread.task = kthread_run(xthread_video, (void *)vc, "video_kthread_%d", vdev->num);
		if (IS_ERR(vc->kthread.task))
		{
			v4l2_err(&dev->v4l2_dev, "video_kthread_%d create failed!\n", ch);
			vc->kthread.task = NULL;
			goto error;
		}	
		spin_lock_init(&vc->events_lock);
		xdma_user_isr_register(vc->dev->xdev, 1 << ch, frame_sync_isr, vc);
		xdma_user_isr_register(vc->dev->xdev, 1 << (ch + 8), frame_trigger_isr, vc);
	}

	for (ch = 0; ch < max_channels(dev); ch++)
	{
		struct xdma_video_channel *vc = &dev->video_channels[ch];
		dbg_video("xdma%d: vc%d--video%d \n", xdev->idx, ch, vc->num);
	}

	return 0;

error:
	xdma_video_free(dev);
	return err;
}

static void xdma_video_dev_release(struct v4l2_device *v4l2_dev)
{
	struct xdma_video_dev *dev = container_of(v4l2_dev, struct xdma_video_dev,
											  v4l2_dev);
	// unsigned int ch;

	// for (ch = 0; ch < max_channels(dev); ch++)
	//	v4l2_ctrl_handler_free(&dev->video_channels[ch].ctrl_handler);

	v4l2_device_unregister(&dev->v4l2_dev);
	kfree(dev->video_channels);
}

int xdma_video_add(struct xdma_pci_dev *xpdev)
{
	struct xdma_video_dev *dev = &xpdev->video_dev;
	struct xdma_dev *xdev = xpdev->xdev;
	int err;
	struct axi_video_regs *status_regs;
	u32 hw_channels;
	u32 hw_revision;

	status_regs = (struct axi_video_regs *)(xdev->bar[xdev->user_bar_idx] +
											REGS_USER_OFS_VIDEO);

	hw_channels = 8; // ioread32(&status_regs->lanes_channels);
	hw_revision = 1; // ioread32(&status_regs->revisoin);
	dbg_video("hw_lanes: %d \n", (hw_channels & 0xff00) >> 8);
	dbg_video("hw_channels: %d \n", (hw_channels & 0xff) >> 0);
	dbg_video("hw_revision: %d \n", hw_revision);

	spin_lock_init(&dev->lock);
	// dev->channels = VIDEO_CHANNELS;
	// dev->channels = card_channels[xdev->idx];
	if (hw_channels & 0xf)
		dev->channels = hw_channels & 0xf;
	else
		dev->channels = 1;
	sprintf(dev->name, "xdma%d", xdev->idx);
	dev->magic = MAGIC_VIDEO;
	dev->xpdev = xpdev;
	dev->xdev = xdev;
	dev->pci_dev = xpdev->pdev;
	dev->field = V4L2_FIELD_NONE;

	dev->video_channels = kcalloc(max_channels(dev),
								  sizeof(*dev->video_channels), GFP_KERNEL);
	if (!dev->video_channels)
	{
		err = -ENOMEM;
		return err;
	}

	dev->v4l2_dev.release = xdma_video_dev_release;

	transfer_monitor_init(dev);

	err = xdma_video_init(dev);
	if (err)
	{
		dev_err(&dev->pci_dev->dev, "can't register video\n");
		goto free_video;
	}

	xdma_video_attr_create(dev);

	return 0;

free_video:
	transfer_monitor_free(dev);
	kfree(dev->video_channels);
	return err;
}

int xdma_video_remove(struct xdma_pci_dev *xpdev)
{
	struct xdma_video_dev *dev = &xpdev->video_dev;

	xdma_video_attr_remove(dev);
	transfer_monitor_free(dev);
	xdma_video_free(dev);
	v4l2_device_put(&dev->v4l2_dev);

	return 0;
}
