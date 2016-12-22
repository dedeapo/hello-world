#define GPICON (*(volatile unsigned long *)0x7F008100)
#define GPJCON (*(volatile unsigned long *)0x7F008100)

	VSPW
	VBPD
	LINEVAL
	VFPD
	VCLK
	HSPW
	HBPD
	HOZVAL
	HFPD

	void lcd_port_init()
	{
		GPICON = 0xaaaaaaaa;
		GPJCON = 0xaaaaaaaa;
	}

	void lcd_control_init()
	{


	}
