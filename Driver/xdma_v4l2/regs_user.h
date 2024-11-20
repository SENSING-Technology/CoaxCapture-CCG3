#ifndef REGS_USER_H
#define REGS_USER_H

#define REGS_USER_OFS_INT       (0x0UL)
#define REGS_USER_OFS_VIDEO     (0x10000UL)
#define REGS_USER_OFS_TRIGER    (0x20000UL)
#define REGS_USER_OFS_VIDEO_FMT (0x80000UL)

#define REGS_USER_OFS_INT_V2    (0x30000UL)

struct axi_intc_regs {
        u32 isr;
        u32 ipr;
        u32 ier;
        u32 iar;

        u32 sie;
        u32 cie;
        u32 ivr;
        u32 mer;

        u32 imr;
        u32 ilr;
} __packed;

struct axi_video_regs {
        u32 status0;
        u32 status1;
        u32 status2;
        u32 status3;

        u32 status4;
        u32 status5;
        u32 status6;
        u32 status7;

        u32 gpio_ctl;
        u32 rsv1;
        u32 rsv2;
        u32 rsv3;

        u32 rsv4;
        u32 rsv5;
        u32 rsv6;
        u32 rsv7;

        u32 serdes_rst;
        u32 cross_rst;
        u32 serdes_gpio0;
        u32 serdes_gpio1;

        u32 rsv8;
        u32 rsv9;
        u32 rsv10;
        u32 rsv11;

        u32 rsv12;
        u32 rsv13;
        u32 lanes_channels;
        u32 revisoin;

        u32 rsv14;
        u32 rsv15;
        u32 rsv16;
        u32 rsv17;
} __packed;

struct axi_video_fmt {
        u32 width;
        u32 height;
        u32 fps;
        u32 format;
} __packed;

#endif
