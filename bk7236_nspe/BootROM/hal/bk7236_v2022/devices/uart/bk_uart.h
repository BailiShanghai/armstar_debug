#ifndef _UART_H_
#define _UART_H_

#include "fifo.h"
#include "reg.h"

#define UART_DEBUG               0     
#if UART_DEBUG
#define UART_PRT                 os_null_printf
#define UART_WPRT                os_null_printf
#else
#define UART_PRT                 os_null_printf
#define UART_WPRT                os_null_printf
#endif

#define UART_SUCCESS                 (0)
#define UART_FAILURE                 ((UINT32)-1)

#define UART2_DEV_NAME               ("uart2")   /*debug purpose*/
#define UART1_DEV_NAME               ("uart1")   /*comm purpose*/

#define UART_CMD_MAGIC               (0xC124000)

#define UART1_PORT				0
#define UART2_PORT				1

enum
{
    CMD_SEND_BACKGROUND            = UART_CMD_MAGIC + 0,
    CMD_UART_RESET                 = UART_CMD_MAGIC + 1,
    CMD_RX_COUNT,
    CMD_RX_PEEK,
    CMD_UART_INIT,
    CMD_UART_SET_RX_CALLBACK,
    CMD_UART_SET_TX_CALLBACK,
    CMD_SET_STOP_END,
    CMD_UART_SET_TX_FIFO_NEEDWR_CALLBACK,
    CMD_SET_TX_FIFO_NEEDWR_INT,
    CMD_SET_BAUT,
};

/* CMD_RX_PEEK*/
#define URX_PEEK_SIG              (0x0ee)

typedef struct _peek_rx_
{
    UINT32 sig;

    UINT32 len;
    void *ptr;
} UART_PEEK_RX_T, *UART_PEEK_RX_PTR;

typedef void (*uart_callback)(int uport, void *param);

typedef struct uart_callback_des
{
    uart_callback callback;
    void  *param;
}UART_CALLBACK_RX_T, *UART_CALLBACK_RX_PTR;

/**
 * UART data width
 */
typedef enum
{
    BK_DATA_WIDTH_5BIT,
    BK_DATA_WIDTH_6BIT,
    BK_DATA_WIDTH_7BIT,
    BK_DATA_WIDTH_8BIT
} uart_data_width_t;

/**
 * UART stop bits
 */
typedef enum
{
    BK_STOP_BITS_1,
    BK_STOP_BITS_2,
} uart_stop_bits_t;

/**
 * UART flow control
 */
typedef enum
{
    FLOW_CTRL_DISABLED,
    FLOW_CTRL_CTS,
    FLOW_CTRL_RTS,
    FLOW_CTRL_RTS_CTS
} uart_flow_control_t;

/**
 * UART parity
 */
typedef enum
{
    BK_PARITY_NO,
    BK_PARITY_ODD,
    BK_PARITY_EVEN,
} uart_parity_t;

typedef struct
{
    uint32_t				  baud_rate;
    uart_data_width_t         data_width;
    uart_parity_t 	          parity;
    uart_stop_bits_t	      stop_bits;
    uart_flow_control_t       flow_control;
    uint8_t				      flags;	 /**< if set, UART can wake up MCU from stop mode, reference: @ref UART_WAKEUP_DISABLE and @ref UART_WAKEUP_ENABLE*/
} bk_uart_config_t;

#define UART_INDEX_1              (1)
#define UART_INDEX_2              (2)

#define DEBUG_PRT_MAX_CNT          (16)

#define RX_RB_LENGTH               (128) 
#define TX_RB_LENGTH               (64)

#define CARRIAGE_RETURN(buf, count) \
    do { \
        buf[count - 1] = '\r'; \
        buf[count] = '\n'; \
        buf[count + 1] = 0; \
        rc += 1; \
    } while (0)

/* uart parameter config----start*/
#define UART_BAUDRATE_3250000        3250000
#define UART_BAUDRATE_2000000        2000000
#define UART_BAUDRATE_1000000        1000000
#define UART_BAUDRATE_921600         921600
#define UART_BAUDRATE_460800         460800
#define UART_BAUDRATE_230400         230400
#define UART_BAUDRATE_115200         115200  //default
#define UART_BAUDRATE_3000           3250
#define UART_BAUDRATE_19200          19200

#define UART_BAUD_RATE               UART_BAUDRATE_1000000
#ifndef UART1_BAUD_RATE
#define UART1_BAUD_RATE              UART_BAUD_RATE
#endif
#ifndef UART2_BAUD_RATE
#define UART2_BAUD_RATE              UART_BAUD_RATE
#endif

#ifndef UART1_USE_FIFO_REC
#define UART1_USE_FIFO_REC           0
#endif

#ifndef UART2_USE_FIFO_REC
#define UART2_USE_FIFO_REC           0
#endif

#define UART_CLOCK_FREQ_10M          10000000
#define UART_CLOCK_FREQ_48M          48000000
#define UART_CLOCK_FREQ_24M          24000000
#define UART_CLOCK_FREQ_26M          26000000
#define UART_CLOCK_FREQ_52M          52000000
#define UART_CLOCK_FREQ_120M         120000000

#if (CFG_RUNNING_PLATFORM == FPGA_PLATFORM)
#define UART_CLOCK                   UART_CLOCK_FREQ_24M
#elif CFG_XTAL_FREQUENCE
#define UART_CLOCK                   CFG_XTAL_FREQUENCE
#else
#define UART_CLOCK                   UART_CLOCK_FREQ_26M
#endif // CFG_RUNNING_PLATFORM == FPGA_PLATFORM

#define TX_FIFO_THRD                (0x40)
#define RX_FIFO_THRD                (0x10)

#define DEF_TX_EN                   0x1
#define DEF_RX_EN                   0x1
#define DEF_IRDA_MODE               0x0    // 0:uart mode  1:IRDA MODE
#define DEF_DATA_LEN                0x3    // 0=5bit, 1=6bit, 2=7bit, 3=8bit
#define DEF_PARITY_EN               0x0    // 0=no parity  1: enable parity
#define DEF_PARITY_MODE             0x0    // 0:odd  1: even
#define DEF_STOP_BIT                0x0    // 1bit

#define FLOW_CTRL_HIGH_CNT          (96)
#define FLOW_CTRL_LOW_CNT           (32)

#define DEBUG_TX_FIFO_MAX_COUNT     16

/* uart parameter config----end*/

typedef struct _uart_
{
    UINT16 status;

    KFIFO_PTR rx;

    KFIFO_PTR tx;
} UART_S, *UART_PTR;

#define UART_BASE_ADDR                       (0x44820000)

#define REG_UART_DEVICE_ID                  (UART_BASE_ADDR + 0 * 4)
#define REG_UART_VENDOR_ID                  (UART_BASE_ADDR + 1 * 4)

#define REG_UART_CLK_RST_CFG                (UART_BASE_ADDR + 2 * 4)
#define FIELD_SOFT_RESETN_POSI                   (0)
#define FIELD_SOFT_RESETN_MASK                   (0x1)
#define FIELD_CLKG_BPS_POSI                      (1)
#define FIELD_CLKG_BPS_MASK                      (0x1)

#define REG_UART_STATUS                     (UART_BASE_ADDR + 3 * 4)

#define UART1_BASE_ADDR			            (0x44820000 + 4 * 4) /* offset 16 bytes, compared with UART_BASE_ADDR*/
#define UART2_BASE_ADDR			            (0x44820000 + 4 * 4)

#define REG_UART1_CONFIG                     (UART1_BASE_ADDR + 4 * 0)
#define REG_UART2_CONFIG                     (UART2_BASE_ADDR + 4 * 0)

#define UART_TX_ENABLE                         (1 << 0)
#define UART_RX_ENABLE                         (1 << 1)
#define FIELD_UART_NC                          (1 << 2)
#define UART_DATA_LEN_POSI                     (3)
#define UART_DATA_LEN_MASK                     (0x03)
#define UART_PAR_EN                            (1 << 5)
#define UART_PAR_ODD_MODE                      (1 << 6)
#define UART_STOP_LEN_2                        (1 << 7)
#define UART_CLK_DIVID_POSI                    (8)
#define UART_CLK_DIVID_MASK                    (0xFFFF)

#define REG_UART1_FIFO_CONFIG                 (UART1_BASE_ADDR + 4 * 1)
#define REG_UART2_FIFO_CONFIG                 (UART2_BASE_ADDR + 4 * 1)
#define TX_FIFO_THRESHOLD_MASK                 (0xFF)
#define TX_FIFO_THRESHOLD_POSI                 (0)
#define RX_FIFO_THRESHOLD_MASK                 (0xFF)
#define RX_FIFO_THRESHOLD_POSI                 (8)
#define RX_STOP_DETECT_TIME_MASK               (0x03)
#define RX_STOP_DETECT_TIME_POSI               (16)
#define RX_STOP_DETECT_TIME32                  (0)
#define RX_STOP_DETECT_TIME64                  (1)
#define RX_STOP_DETECT_TIME128                 (2)
#define RX_STOP_DETECT_TIME256                 (3)

#define REG_UART1_FIFO_STATUS                 (UART1_BASE_ADDR + 4 * 2)
#define REG_UART2_FIFO_STATUS                 (UART2_BASE_ADDR + 4 * 2)
#define TX_FIFO_COUNT_MASK                     (0xFF)
#define TX_FIFO_COUNT_POSI                     (0)
#define RX_FIFO_COUNT_MASK                     (0xFF)
#define RX_FIFO_COUNT_POSI                     (8)
#define TX_FIFO_FULL                           (1 << 16)
#define TX_FIFO_EMPTY                          (1 << 17)
#define RX_FIFO_FULL                           (1 << 18)
#define RX_FIFO_EMPTY                          (1 << 19)
#define FIFO_WR_READY                          (1 << 20)
#define FIFO_RD_READY                          (1 << 21)

#define REG_UART1_FIFO_PORT                   (UART1_BASE_ADDR + 4 * 3)
#define REG_UART2_FIFO_PORT                   (UART2_BASE_ADDR + 4 * 3)
#define UART_TX_FIFO_DIN_MASK                  (0xFF)
#define UART_TX_FIFO_DIN_POSI                  (0)
#define UART_RX_FIFO_DOUT_MASK                 (0xFF)
#define UART_RX_FIFO_DOUT_POSI                 (8)

#define REG_UART1_INTR_ENABLE                 (UART1_BASE_ADDR + 4 * 4)
#define REG_UART2_INTR_ENABLE                 (UART2_BASE_ADDR + 4 * 4)
#define TX_FIFO_NEED_WRITE_EN                  (1 << 0)
#define RX_FIFO_NEED_READ_EN                   (1 << 1)
#define RX_FIFO_OVER_FLOW_EN                   (1 << 2)
#define UART_RX_PARITY_ERR_EN                  (1 << 3)
#define UART_RX_STOP_ERR_EN                    (1 << 4)
#define UART_TX_STOP_END_EN                    (1 << 5)
#define UART_RX_STOP_END_EN                    (1 << 6)
#define UART_RXD_WAKEUP_EN                     (1 << 7)

#define REG_UART1_INTR_STATUS                 (UART1_BASE_ADDR + 4 * 5)
#define REG_UART2_INTR_STATUS                 (UART2_BASE_ADDR + 4 * 5)
#define TX_FIFO_NEED_WRITE_STA                  (1 << 0)
#define RX_FIFO_NEED_READ_STA                   (1 << 1)
#define RX_FIFO_OVER_FLOW_STA                   (1 << 2)
#define UART_RX_PARITY_ERR_STA                  (1 << 3)
#define UART_RX_STOP_ERR_STA                    (1 << 4)
#define UART_TX_STOP_END_STA                    (1 << 5)
#define UART_RX_STOP_END_STA                    (1 << 6)
#define UART_RXD_WAKEUP_STA                     (1 << 7)

#define REG_UART1_FLOW_CONFIG                 (UART1_BASE_ADDR + 4 * 6)
#define REG_UART2_FLOW_CONFIG                 (UART2_BASE_ADDR + 4 * 6)
#define FLOW_CTRL_LOW_CNT_MASK                   (0xFF)
#define FLOW_CTRL_LOW_CNT_POSI                   (0)
#define FLOW_CTRL_HIGH_CNT_MASK                  (0xFF)
#define FLOW_CTRL_HIGH_CNT_POSI                  (8)
#define FLOW_CONTROL_EN                          (1 << 16)
//only valid for uart1
#define RTS_POLARITY_SEL				(1<<17)
#define CTS_POLARITY_SEL				(1<<18)

#define REG_UART1_WAKE_CONFIG                 (UART1_BASE_ADDR + 4 * 7)
#define REG_UART2_WAKE_CONFIG                 (UART2_BASE_ADDR + 4 * 7)
#define UART_WAKE_COUNT_MASK                   (0x3FF)
#define UART_WAKE_COUNT_POSI                   (0)
#define UART_TXD_WAIT_CNT_MASK                 (0x3FF)
#define UART_TXD_WAIT_CNT_POSI                 (10)
#define UART_RXD_WAKE_EN                       (1 << 20)
#define UART_TXD_WAKE_EN                       (1 << 21)
#define RXD_NEGEDGE_WAKE_EN                    (1 << 22)

#define UART1_TX_WRITE_READY             (REG_READ(REG_UART1_FIFO_STATUS) & FIFO_WR_READY)
#define UART2_TX_WRITE_READY             (REG_READ(REG_UART2_FIFO_STATUS) & FIFO_WR_READY)
#define UART1_TX_FIFO_COUNT             (((REG_READ(REG_UART1_FIFO_STATUS)) >> TX_FIFO_COUNT_POSI) & TX_FIFO_COUNT_MASK)
#define UART2_TX_FIFO_COUNT             (((REG_READ(REG_UART2_FIFO_STATUS)) >> TX_FIFO_COUNT_POSI) & TX_FIFO_COUNT_MASK)



#define UART_WRITE_BYTE(ch,v)           do                                   \
										{                                     \
											v = (v & UART_TX_FIFO_DIN_MASK)   \
													<< UART_TX_FIFO_DIN_POSI; \
											if(0 == ch)						\
												REG_WRITE(REG_UART1_FIFO_PORT, v); \
											else					\
												REG_WRITE(REG_UART2_FIFO_PORT, v);\
										}while(0)
#define UART_READ_BYTE(ch,v)            do                                    \
										{                                     \
											if(0 == ch)			\
												v = (REG_READ(REG_UART1_FIFO_PORT) \
													>> UART_RX_FIFO_DOUT_POSI) \
													& UART_RX_FIFO_DOUT_MASK;\
											else					\
												v = (REG_READ(REG_UART2_FIFO_PORT) \
													>> UART_RX_FIFO_DOUT_POSI) \
													& UART_RX_FIFO_DOUT_MASK;\
										}while(0)

#define UART_READ_BYTE_DISCARD(ch)      do                                    \
										{                                     \
											if(0 == ch)			\
												REG_READ(REG_UART1_FIFO_PORT);\
											else					\
												REG_READ(REG_UART2_FIFO_PORT);\
										}while(0)



#define UART2_TX_FIFO_EMPTY_GET()       ((REG_READ(REG_UART2_FIFO_STATUS) & TX_FIFO_EMPTY)>>17)
#define UART1_TX_FIFO_EMPTY_GET()       ((REG_READ(REG_UART1_FIFO_STATUS) & TX_FIFO_EMPTY)>>17)


/*******************************************************************************
* Function Declarations
*******************************************************************************/
extern UINT32 uart_sw_init(UINT8 ch);
extern UINT32 uart_sw_uninit(UINT8 ch);
extern void uart_fifo_flush(UINT8 ch);
extern void uart_hw_uninit(UINT8 ch);
extern void uart_reset(UINT8 ch);
extern UINT32 uart_write_fifo_frame(UINT8 ch, KFIFO_PTR tx_ptr, UINT32 count);
extern UINT32 uart_read_fifo_frame(UINT8 ch, KFIFO_PTR rx_ptr);
extern void bk_uart1_init(void);
extern UINT32 uart1_open(UINT32 op_flag);
extern UINT32 uart1_close(void);
extern UINT32 uart1_read(char *user_buf, UINT32 count, UINT32 op_flag);
extern UINT32 uart1_write(char *user_buf, UINT32 count, UINT32 op_flag);
extern UINT32 uart1_ctrl(UINT32 cmd, void *parm);
extern void uart1_exit(void);
extern INT32 os_null_printf(const char *fmt, ...);
extern void bk_printf(const char *fmt, ...);
extern int uart_read_byte(int uport);
extern int uart_write_byte(int uport, char c);
extern int uart_rx_callback_set(int uport, uart_callback callback, void *param);
extern int uart_tx_fifo_needwr_callback_set(int uport, uart_callback callback, void *param);
extern int uart_tx_end_callback_set(int uport, uart_callback callback, void *param);
extern void uart_set_tx_stop_end_int(UINT8 uport, UINT8 set);
extern void uart_set_tx_fifo_needwr_int(UINT8 uport, UINT8 set);
extern void uart_hw_set_change(UINT8 uport, bk_uart_config_t *uart_config);
#endif // _UART_H_

