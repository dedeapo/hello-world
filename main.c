int gboot_main()
{
#ifdef MMU_ON
    mmu_init();
#endif
    uart_init();

    led_init();
    
    button_init();
    
    init_irq();
    
    led_off();
    
//≤‚ ‘git
    	
   	while(1)
	{
		//getc();

		printf("\n\r****************************************\n\r");
	    	printf("\n\r*****************GBOOT*****************\n\r");
	    	printf("1:Download Linux Kernel from TFTP Server!\n\r");
	    	printf("2:Boot Linux from RAM!\n\r");
	    	printf("3:Boor Linux from Nand Flash!\n\r");
	    	printf("\n Plese Select:");
	    	
	    	scanf("%d",&num);
	    
	        switch (num)
	        {
	            case 1:
	            //tftp_load();
	            break;
	            
	            case 2:
	            //boot_linux_ram();
	            break;
	            
	            case 3:
	            //boot_linux_nand();
	            break;
	            
	            default:
	                printf("Error: wrong selection!\n\r");
	            break;
	
			}
    }    
    return 0;    
}

