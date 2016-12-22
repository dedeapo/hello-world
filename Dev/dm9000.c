/*
   ����,CMD���ӵ�CPU��ADDR2,Ȼ��SD0~SD15���ӵ�����CPU��XM0DATA0~XM0DATA15.
   ��ADDRx��XMDATAx����Щʲô������?�����Ǵ�������SROM0���������ߺ͵�ַ����,
   ������Ϊ�����岢û��ʹ��SROM0,������Щ���߾͸�DM9000ʹ����.
   CMD���Žӵľ��Ǹõ�ַ���ߵĵ�[2]��,SROM0�ĵ�ַ��ӳ�䵽��������0x18000000,
   ���Ե����ַ0x18000000д����ʱ,DM9000��CMD������Ϊ�˵͵�ƽ,
   ��ʱ����������д���������Ϊ�Ĵ�����ѡ���ź�,
   Ȼ�����ַ0x18000004д����ʱ,��ʱDM9000��CMD���ű�����Ϊ�˸ߵ�ƽ
   ��ʱд���������ߵ����ݽ���д�뵽����һ��ѡ�еļĴ�����.
   ǧ��Ҫע���ֽ���Ŷ,������ʹ�õ��Ǵ�˸�ʽ,cpuʹ�õ���С�˸�ʽ
*/

#include "dm9000.h" 

#define DM_ADD (*((volatile unsigned short *)0x18000000))
#define DM_DAT (*((volatile unsigned short *)0x18000004))
#define SROM_BW (*((volatile unsigned short *)0x70000000)
#define SROM_BC1 (*((volatile unsigned short *)0x70000008)//Ҳ�п�����SROM_BC0
#define GPNCON (*((volatile unsigned short *)0x7F008830)
#define EINT0CON0 (*((volatile unsigned short *)0x7F008900)
#define EINT0MASK (*((volatile unsigned short *)0x7F008920)
#define EXT_INT_0_PEND (*((volatile unsigned short *)0x7F008924)
#define VIC0ADDRESS         *((volatile unsigned int *)0x71200f00)   
#define VIC1ADDRESS         *((volatile unsigned int *)0x71300f00)
#define VIC0INTENABLE  *((volatile unsigned int *)0x71200010) 
#define EINT7_VECTADDR      *((volatile unsigned int *)0x71200105)  //Ҳ�п�����0x7120_0006 7

u8 mac_addr[6] = {9,8,7,6,5,4};
u8 buffer[1000];

/*��ʼ��DM9000����Ƭѡ,�οhttp://www.linuxidc.com/Linux/2016-01/127965.htm*/
void cs_init(void)
{
	//select bus width
	SROM_BW &= (~(0xf<<4));
       SROM_BW |=  (0x1<<4);

	//����srom0��ʱ��
       SROM_BC1 =(0<<0)|(0x2<<4)|(0x2<<8)|(0x2<<12)|(0x2<<16)|(0x2<<24)|(0x2<<28);	
}

	/*�жϳ�ʼ��*/
void	int_init(void)
{
	//���ùܽ�
	GPNCON = GPNCON &(~(0x3<<14));
	GPNCON = GPNCON |(0x2<<14);
	
	//���øߵ�ƽ����
	EINT0CON0 = EINT0CON0 & (~(0x7<<28));
	EINT0CON0 = EINT0CON0 | (0x01<<12);
	
	//�����ж�
	EINT0MASK = EINT0MASK & (~(0X01<<7));
	
	//����жϱ�־��д1���
	EXT_INT_0_PEND = ~0x0; 
       VIC0ADDRESS = 0; 
       VIC1ADDRESS = 0;   

	//ʹ���ⲿ�ж�
	VIC0INTENABLE |= (0x01<<6);

	//��ת�жϳ���
	 EINT7_VECTADDR = (int)int_issue;//�жϳ���;
	
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

/*��dm9000�Ĵ���д������*/
void dm9000_reg_write(u16 reg,u16 data)
{
    DM_ADD = reg;	
    DM_DAT = data;	
}

/*��ȡdm9000�Ĵ�������*/
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

/*dm9000��ʼ��*/
void dm9000_init(void)
{
	u32 i;

	/*����Ƭѡ*/
	cs_init();

	/*�жϳ�ʼ��*/
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
    
     /*��ֹ�жϣ�dm9000�ڲ��Ĵ�������*/
    dm9000_reg_write(DM9000_IMR,0x80);

    /*д�뷢�����ݵĳ���*/
    dm9000_reg_write(DM9000_TXPLL, length & 0xff);
    dm9000_reg_write(DM9000_TXPLH, (length >> 8) & 0xff);

    /*д������͵�����*/
    DM_ADD = DM9000_MWCMD;
   
    for(i=0;i<length;i+=2)
    {
    	DM_DAT = data[i] | (data[i+1]<<8);
    }

    /*��������*/
    dm9000_reg_write(DM9000_TCR, TCR_TXREQ); 

	 /*�ȴ����ͽ���*/
    while(1)
    {
       u8 status;
       status = dm9000_reg_read(DM9000_TCR);
       if((status&0x01)==0x00)
           break;	
    }		

    /*�������״̬*/
    dm9000_reg_write(DM9000_NSR,0x2c);
    
    /*�ָ��ж�ʹ��*/
    dm9000_reg_write(DM9000_IMR,0x81);
	
}

void dm9000_rx()
{
    u8 status,len;
    u16 tmp;
    u32 i;
    
    /*�ж��Ƿ�����жϣ������*/
    if(dm9000_reg_read(DM9000_ISR) & 0x01)
        dm9000_reg_write(DM9000_ISR,0x01);
    else
        return 0;
        
    /*�ն�*/
    dm9000_reg_read(DM9000_MRCMDX);
    
    /*��ȡ״̬*/
    status = dm9000_reg_read(DM9000_MRCMD);
    
    /*��ȡ������*/
    len = DM_DAT;
    
    /*��ȡ������*/
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
	
    /* ����ж� */
    EXT_INT_0_PEND = ~0x0; 
    VIC0ADDRESS = 0; 
    VIC1ADDRESS = 0;   
    
    __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  ); 

}







