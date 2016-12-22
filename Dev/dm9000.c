/*
   首先,CMD连接到CPU的ADDR2,然后SD0~SD15连接到的是CPU的XM0DATA0~XM0DATA15.
   那ADDRx和XMDATAx又是些什么东西呢?它们是处理器的SROM0的数据总线和地址总线,
   但是因为开发板并没有使用SROM0,所以这些总线就给DM9000使用了.
   CMD引脚接的就是该地址总线的第[2]根,SROM0的地址被映射到处理器的0x18000000,
   所以当想地址0x18000000写数据时,DM9000的CMD被设置为了低电平,
   这时向数据总线写入的数据作为寄存器的选择信号,
   然后当想地址0x18000004写数据时,这时DM9000的CMD引脚被设置为了高电平
   这时写入数据总线的数据将被写入到上面一步选中的寄存器中.
   千万要注意字节序哦,网络上使用的是大端格式,cpu使用的是小端格式
*/

#include "dm9000.h" 

#define DM_ADD (*((volatile unsigned short *)0x18000000))
#define DM_DAT (*((volatile unsigned short *)0x18000004))
#define SROM_BW (*((volatile unsigned short *)0x70000000)
#define SROM_BC1 (*((volatile unsigned short *)0x70000008)//也有可能是SROM_BC0
#define GPNCON (*((volatile unsigned short *)0x7F008830)
#define EINT0CON0 (*((volatile unsigned short *)0x7F008900)
#define EINT0MASK (*((volatile unsigned short *)0x7F008920)
#define EXT_INT_0_PEND (*((volatile unsigned short *)0x7F008924)
#define VIC0ADDRESS         *((volatile unsigned int *)0x71200f00)   
#define VIC1ADDRESS         *((volatile unsigned int *)0x71300f00)
#define VIC0INTENABLE  *((volatile unsigned int *)0x71200010) 
#define EINT7_VECTADDR      *((volatile unsigned int *)0x71200105)  //也有可能是0x7120_0006 7

u8 mac_addr[6] = {9,8,7,6,5,4};
u8 buffer[1000];

/*初始化DM9000设置片选,参縣ttp://www.linuxidc.com/Linux/2016-01/127965.htm*/
void cs_init(void)
{
	//select bus width
	SROM_BW &= (~(0xf<<4));
       SROM_BW |=  (0x1<<4);

	//配置srom0的时序
       SROM_BC1 =(0<<0)|(0x2<<4)|(0x2<<8)|(0x2<<12)|(0x2<<16)|(0x2<<24)|(0x2<<28);	
}

	/*中断初始化*/
void	int_init(void)
{
	//配置管脚
	GPNCON = GPNCON &(~(0x3<<14));
	GPNCON = GPNCON |(0x2<<14);
	
	//设置高电平触发
	EINT0CON0 = EINT0CON0 & (~(0x7<<28));
	EINT0CON0 = EINT0CON0 | (0x01<<12);
	
	//屏蔽中断
	EINT0MASK = EINT0MASK & (~(0X01<<7));
	
	//清除中断标志，写1清除
	EXT_INT_0_PEND = ~0x0; 
       VIC0ADDRESS = 0; 
       VIC1ADDRESS = 0;   

	//使能外部中断
	VIC0INTENABLE |= (0x01<<6);

	//跳转中断程序
	 EINT7_VECTADDR = (int)int_issue;//中断程序;
	
}

/* RESET device */
void	dm9000_reset()
{
    dm9000_reg_write(DM9000_GPCR, GPCR_GPIO0_OUT);
    dm9000_reg_write(DM9000_GPR, 0);	
    
    dm9000_reg_write(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST));
    dm9000_reg_write(DM9000_NCR, 0);
    
    dm9000_reg_write(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST));
    dm9000_reg_write(DM9000_NCR, 0);
}

/*向dm9000寄存器写入数据*/
void dm9000_reg_write(u16 reg,u16 data)
{
    DM_ADD = reg;	
    DM_DAT = data;	
}

/*读取dm9000寄存器数据*/
u8 dm9000_reg_read(u16 reg)
{
    DM_ADD = reg;
    return DM_DAT;	
}

/* Search DM9000 board, allocate space and register it*/
void dm9000_probe(void)
{
	u32 id_val;
	id_val = dm9000_reg_read(DM9000_VIDL);
	id_val |= dm9000_reg_read(DM9000_VIDH) << 8;
	id_val |= dm9000_reg_read(DM9000_PIDL) << 16;
	id_val |= dm9000_reg_read(DM9000_PIDH) << 24;
	if (id_val == DM9000_ID) {
		printf("dm9000 is found !\n");
		return ;
	} else {
		printf("dm9000 is not found !\n");
		return ;
	}
}

/*dm9000初始化*/
void dm9000_init(void)
{
	u32 i;

	/*设置片选*/
	cs_init();

	/*中断初始化*/
	int_init();
	/* RESET device */
	dm9000_reset();

	/* Search DM9000 board, allocate space and register it*/
      dm9000_probe();
	
	/*mac init*/
       /* Program operating register, only internal phy supported */
	dm9000_reg_write(DM9000_NCR, 0x0);
	/* TX Polling clear */
	dm9000_reg_write(DM9000_TCR, 0);
	/* Less 3Kb, 200us */
	dm9000_reg_write(DM9000_BPTR, BPTR_BPHW(3) | BPTR_JPT_600US);
	/* Flow Control : High/Low Water */
	dm9000_reg_write(DM9000_FCTR, FCTR_HWOT(3) | FCTR_LWOT(8));
	/* SH FIXME: This looks strange! Flow Control */
	dm9000_reg_write(DM9000_FCR, 0x0);
	/* Special Mode */
	dm9000_reg_write(DM9000_SMCR, 0);
	/* clear TX status */
	dm9000_reg_write(DM9000_NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END);
	/* Clear interrupt status */
	dm9000_reg_write(DM9000_ISR, ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS);

	/* fill device MAC address registers */
       for (i = 0; i < 6; i++)
        dm9000_reg_write(DM9000_PAR+i, mac_addr[i]);
	   
	/* Activate DM9000 */

	
	
}

void dm9000_tx()
{
	u32 i;
    
     /*禁止中断，dm9000内部寄存器控制*/
    dm9000_reg_write(DM9000_IMR,0x80);

    /*写入发送数据的长度*/
    dm9000_reg_write(DM9000_TXPLL, length & 0xff);
    dm9000_reg_write(DM9000_TXPLH, (length >> 8) & 0xff);

    /*写入待发送的数据*/
    DM_ADD = DM9000_MWCMD;
   
    for(i=0;i<length;i+=2)
    {
    	DM_DAT = data[i] | (data[i+1]<<8);
    }

    /*启动发送*/
    dm9000_reg_write(DM9000_TCR, TCR_TXREQ); 

	 /*等待发送结束*/
    while(1)
    {
       u8 status;
       status = dm9000_reg_read(DM9000_TCR);
       if((status&0x01)==0x00)
           break;	
    }		

    /*清除发送状态*/
    dm9000_reg_write(DM9000_NSR,0x2c);
    
    /*恢复中断使能*/
    dm9000_reg_write(DM9000_IMR,0x81);
	
}

void dm9000_rx()
{
    u8 status,len;
    u16 tmp;
    u32 i;
    
    /*判断是否产生中断，且清除*/
    if(dm9000_reg_read(DM9000_ISR) & 0x01)
        dm9000_reg_write(DM9000_ISR,0x01);
    else
        return 0;
        
    /*空读*/
    dm9000_reg_read(DM9000_MRCMDX);
    
    /*读取状态*/
    status = dm9000_reg_read(DM9000_MRCMD);
    
    /*读取包长度*/
    len = DM_DAT;
    
    /*读取包数据*/
    if(len<PTK_MAX_LEN)
    {
       for(i=0;i<len;i+=2)
       {
           tmp = DM_DAT;
           data[i] = tmp & 0x0ff;
           data[i+1] = (tmp>>8)&0x0ff;
       }
    }

}


void int_issue()
{

    __asm__( 
    
    "sub lr, lr, #4\n"  
    "stmfd sp!, {r0-r12, lr}\n"       
    : 
    : 
  );
    u32 i;
    i = dm9000_rx(buffer);	
	
    /* 清除中断 */
    EXT_INT_0_PEND = ~0x0; 
    VIC0ADDRESS = 0; 
    VIC1ADDRESS = 0;   
    
    __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  ); 

}







